#include <pebble.h>

static DictationSession *s_dictation_session;
static char s_last_text[512];
static Window *window;
static TextLayer *text_layer;

static void dictation_session_callback(DictationSession *session, DictationSessionStatus status,
	char *transcription, void  *context) {
	
	if (status == DictationSessionStatusSuccess) {
		snprintf(s_last_text, sizeof(s_last_text), "%s", transcription);
		text_layer_set_text(text_layer, s_last_text);
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
