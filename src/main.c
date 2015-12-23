#include <pebble.h>
#include "scrolling_forecast_layer.h"

// TODO: Create a layer with 32 pixel height of the weather for a single day
enum {
  MESSAGE_TYPE = 0,
};

enum {
  PHONE_READY = 0,
  WEATHER_REPORT = 1,
  FETCH_WEATHER = 2,
  WEATHER_FAILED = 3
};

typedef struct {
  Window *main_window;
  ScrollingForecastLayer* forecast_layer;
} Application;

WeeklyForecast empty_forecast() {
  return (WeeklyForecast) {
    .valid = false
  };
}
static bool send_request() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Sending the Fetch Weather command to the phone");
  
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  if (iter == NULL) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "null iter");
    return false;
  }

  dict_write_uint8(iter, MESSAGE_TYPE, FETCH_WEATHER);
  dict_write_end(iter);

  app_message_outbox_send();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Finished the send");
  return true;
}

static void inbox_received_handler(DictionaryIterator *iterator, void *context) {
  Tuple *init_tuple = dict_find(iterator, MESSAGE_TYPE);
  uint8_t request_type = init_tuple->value->uint8;
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Got a message with request type [%d]", request_type);
  
  if(request_type == PHONE_READY) {
    send_request();
  }
  
  if(request_type == WEATHER_REPORT) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Got a weather report");
  }
}

void handle_init(Application *application) {
  application->main_window = window_create();
  Layer *window_layer = window_get_root_layer(application->main_window);
  GRect window_bounds = layer_get_bounds(window_layer);
  application->forecast_layer = scrolling_forecast_layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h), empty_forecast());

  app_message_register_inbox_received(inbox_received_handler);
  app_message_set_context(application);
  app_message_open(531, 9);
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