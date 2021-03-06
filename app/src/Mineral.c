#include <pebble.h>

static DictationSession *s_dictation_session;
static DictionaryIterator *itr;
static char main_search_value[512];
static Window *window;
static TextLayer *text_layer;
static ScrollLayer *scroll_layer;
static GBitmap *bitmap = NULL;
static BitmapLayer *bitmap_layer;
static GBitmapSequence *sequence = NULL;

static void timer_handler(void *context) {
  uint32_t next_delay;

  if(gbitmap_sequence_update_bitmap_next_frame(sequence, bitmap, &next_delay)) {
    bitmap_layer_set_bitmap(bitmap_layer, bitmap);
    layer_mark_dirty(bitmap_layer_get_layer(bitmap_layer));

    app_timer_register(next_delay, timer_handler, NULL);
  } else {
    gbitmap_sequence_restart(sequence);
  }
}

static void load_sequence() {
  if(sequence) {
    gbitmap_sequence_destroy(sequence);
    sequence = NULL;
  }
  if(bitmap) {
    gbitmap_destroy(bitmap);
    bitmap = NULL;
  }

  sequence = gbitmap_sequence_create_with_resource(RESOURCE_ID_ANIMATION);

  bitmap = gbitmap_create_blank(gbitmap_sequence_get_bitmap_size(sequence),
 	GBitmapFormat8Bit);

  app_timer_register(1, timer_handler, NULL);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
	Tuple *result_one = dict_find(iterator, 2);
	char *value = result_one->value->cstring;
	snprintf(main_search_value, sizeof(main_search_value), "%s", value);
	
	bitmap_layer_destroy(bitmap_layer);

	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text(text_layer, main_search_value); 
	scroll_layer_set_content_size(scroll_layer, text_layer_get_content_size(text_layer));
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  bitmap_layer_destroy(bitmap_layer);
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  bitmap_layer_destroy(bitmap_layer);
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void dictation_session_callback(DictationSession *session, DictationSessionStatus status, char *transcription, void  *context) {
	if (status == DictationSessionStatusSuccess) {
		snprintf(main_search_value, sizeof(main_search_value), "%s", transcription);
		text_layer_set_text(text_layer, "Loading...");

		load_sequence();
		
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
	bitmap_layer = bitmap_layer_create(bounds);
	text_layer = text_layer_create((GRect) { .origin = { 0, 0 }, .size = {10 *
	bounds.size.w, 10 * bounds.size.h } });

    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	text_layer_set_overflow_mode(text_layer, GTextOverflowModeWordWrap);

	scroll_layer_add_child(scroll_layer, text_layer_get_layer(text_layer));
	layer_add_child(window_layer, scroll_layer_get_layer(scroll_layer));

	text_layer_enable_screen_text_flow_and_paging(text_layer, 2);

	scroll_layer_set_paging(scroll_layer, true);
	scroll_layer_add_child(scroll_layer, text_layer_get_layer(text_layer));
  	
	layer_add_child(window_layer, scroll_layer_get_layer(scroll_layer));
	layer_add_child(window_layer, bitmap_layer_get_layer(bitmap_layer));
	

	scroll_layer_set_click_config_onto_window(scroll_layer, window);
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
  scroll_layer_destroy(scroll_layer);
  bitmap_layer_destroy(bitmap_layer);
}


static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;

  s_dictation_session = dictation_session_create(sizeof(main_search_value), 
  dictation_session_callback, NULL);

  window_stack_push(window, animated);

  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);   
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
	
  app_message_open(1024, 1024); 
  vibes_short_pulse();
  dictation_session_start(s_dictation_session);
}

static void deinit(void) {
  dictation_session_destroy(s_dictation_session);
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
