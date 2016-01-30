#include <pebble_worker.h>

static void tap_handler(AccelAxisType axis, int32_t direction) {
  if (axis == ACCEL_AXIS_Y){
	 worker_launch_app();
  }
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
