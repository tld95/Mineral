#include <pebble.h>

static DictationSession *s_dictation_session;
static DictionaryIterator *itr;
static char main_search_value[512];
static Window *window;
static TextLayer *text_layer;
static ScrollLayer *scroll_layer;
static int index;

/*
static void click_config_provider(void *context) {

}
*/
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
	// Main search result value
	Tuple *result_one = dict_find(iterator, 2);
	char *value = result_one->value->cstring;
	snprintf(main_search_value, sizeof(main_search_value), "%s", value);

	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text(text_layer, main_search_value); 
	scroll_layer_set_content_size(scroll_layer, text_layer_get_content_size(text_layer));
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void dictation_session_callback(DictationSession *session, DictationSessionStatus status, char *transcription, void  *context) {
	
	if (status == DictationSessionStatusSuccess) {
		snprintf(main_search_value, sizeof(main_search_value), "%s", transcription);
		text_layer_set_text(text_layer, "Loading...");
		uint32_t key = 1;

		app_message_outbox_begin(&itr);
		dict_write_cstring(itr, key, main_search_value);
		app_message_outbox_send();

	} else {
		text_layer_set_text(text_layer, "Error Transcribing\n");
	}	
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

	scroll_layer = scroll_layer_create(bounds);

	text_layer = text_layer_create((GRect) { .origin = { 0, 0 }, .size = {10 * bounds.size.w, 
	10 * bounds.size.h } });
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);

	text_layer_set_overflow_mode(text_layer, GTextOverflowModeWordWrap);
	text_layer_set_background_color(text_layer, GColorBlue);
	text_layer_set_text_color(text_layer, GColorWhite);	

	// Add the TextLayer and ScrollLayer to the view heirachy
	scroll_layer_add_child(scroll_layer, text_layer_get_layer(text_layer));
	layer_add_child(window_layer, scroll_layer_get_layer(scroll_layer));

	// Enable TextLayer text flow and paging with inset size 2px
	text_layer_enable_screen_text_flow_and_paging(text_layer, 2);

	// Enable ScrollLayer paging
	scroll_layer_set_paging(scroll_layer, true);

	scroll_layer_add_child(scroll_layer, text_layer_get_layer(text_layer));
  layer_add_child(window_layer, scroll_layer_get_layer(scroll_layer));	

	scroll_layer_set_click_config_onto_window(scroll_layer, window);
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
	scroll_layer_destroy(scroll_layer);
}


static void init(void) {
  window = window_create();
 // window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;

  s_dictation_session = dictation_session_create(sizeof(main_search_value), 
  dictation_session_callback, NULL);

  window_stack_push(window, animated);

  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);   
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	
  vibes_short_pulse();
  dictation_session_start(s_dictation_session);
}

static void deinit(void) {
	dictation_session_destroy(s_dictation_session);
  window_destroy(window);
}

int main(void) {
  init();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
  app_event_loop();
  deinit();
}
