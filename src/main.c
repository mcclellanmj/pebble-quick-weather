#include <pebble.h>
#include "scrolling_forecast_layer.h"

// TODO: Create a layer with 32 pixel height of the weather for a single day

typedef struct {
  Window *main_window;
  ScrollingForecastLayer* forecast_layer;
} Application;

WeeklyForecast empty_forecast() {
  return (WeeklyForecast) {
    .valid = false
  };
}

void handle_init(Application *application) {
  application->main_window = window_create();
  Layer *window_layer = window_get_root_layer(application->main_window);
  GRect window_bounds = layer_get_bounds(window_layer);
  application->forecast_layer = scrolling_forecast_layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h), empty_forecast());

  window_stack_push(application->main_window, true);
}

void handle_deinit(Application *application) {
  window_destroy(application->main_window);
}

int main(void) {
  Application application;
  handle_init(&application);
  app_event_loop();
  handle_deinit(&application);
}