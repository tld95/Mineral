#include <pebble.h>

static DictationSession *s_dictation_session;
static DictionaryIterator *itr;
static char s_last_text[512];
static uint8_t buffer[10];
static Window *window;
static TextLayer *text_layer;


static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
	Tuple *result = dict_read_first(iterator);
	char *value = result->value->cstring;
	snprintf(s_last_text, sizeof(s_last_text), "%s", value);
	text_layer_set_text(text_layer, s_last_text);	
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

static void dictation_session_callback(DictationSession *session, DictationSessionStatus status,
	char *transcription, void  *context) {
	
	if (status == DictationSessionStatusSuccess) {
		snprintf(s_last_text, sizeof(s_last_text), "%s", transcription);
		text_layer_set_text(text_layer, "Loading...");
		uint32_t key = 1;

		app_message_outbox_begin(&itr);
		dict_write_cstring(itr, key, s_last_text);
		app_message_outbox_send();

	} else {
		text_layer_set_text(text_layer, "Error Transcribing\n");
	}	
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;

  s_dictation_session = dictation_session_create(sizeof(s_last_text), dictation_session_callback,
  NULL);

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
