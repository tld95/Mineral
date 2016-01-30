#include <pebble_worker.h>

#define ENABLESEARCH 0

static int16_t enable_search = 0;

static void tap_handler(AccelAxisType axis, int32_t direction) {
  if (axis == ACCEL_AXIS_Y){
	enable_search = 1;	
  }
}

static void send_to_foreground() {
  // Construct a data packet
  AppWorkerMessage msg_data = {
    .data0 = enable_search
  };

  app_worker_send_message(ENABLESEARCH, &msg_data);
  enable_search = 0;
}

static void worker_init() {
  accel_tap_service_subscribe(tap_handler);
}

static void worker_deinit() {
  accel_tap_service_unsubscribe();
}

int main(void) {
  worker_init();
  worker_event_loop();
  worker_deinit();
}
