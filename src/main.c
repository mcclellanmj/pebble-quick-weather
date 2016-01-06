#include <pebble.h>
#include "single_day_layer.h"
#define SECONDS_PER_DAY 86400
#define NUMBER_OF_FORECAST_DAYS 10
#define SIZE_OF_FORECAST_DAY 33

// TODO: Create a layer with 32 pixel height of the weather for a single day
enum {
  MESSAGE_TYPE = 0,
  WEATHER_START = 1,
  WEATHER_FORECASTS = 2
};

enum {
  PHONE_READY = 0,
  WEATHER_REPORT = 1,
  FETCH_WEATHER = 2,
  WEATHER_FAILED = 3
};

typedef struct {
  uint8_t forecast_code;
  int8_t high_temperature;
  int8_t low_temperature;
} PhoneWeatherModel;

typedef struct {
  Window *main_window;
} Application;

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

static SingleDayWeather phone_model_to_single_day(PhoneWeatherModel phone_model, time_t date) {
  return (SingleDayWeather) {
    .valid = true,
    .date = date,
    .forecast_code = phone_model.forecast_code,
    .high_temperature = phone_model.high_temperature,
    .low_temperature = phone_model.low_temperature
  };
}

static void inbox_received_handler(DictionaryIterator *iterator, void *context) {
  Application *application = (Application *) context;

  Tuple *init_tuple = dict_find(iterator, MESSAGE_TYPE);
  uint8_t request_type = init_tuple->value->uint8;
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Got a message with request type [%d]", request_type);
  
  if(request_type == PHONE_READY) {
    send_request();
  }
  
  // TODO: Break up this area into smaller functions
  // FIXME: remove repeated calls
  if(request_type == WEATHER_REPORT) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Got a weather report");
    time_t start_time = dict_find(iterator, WEATHER_START)->value->int32;

    ScrollLayer *scrolling_layer = scroll_layer_create(
      GRect(0, 0, 144, 168)
    );
    scroll_layer_set_click_config_onto_window(scrolling_layer, application->main_window);
    scroll_layer_set_paging(scrolling_layer, true);
    scroll_layer_set_content_size(scrolling_layer, GSize(144, 330));
    scroll_layer_set_content_offset(scrolling_layer, GPoint(0, 168), true);

    PhoneWeatherModel* forecast_array = (PhoneWeatherModel*) malloc(
      sizeof(PhoneWeatherModel) * NUMBER_OF_FORECAST_DAYS
    );

    memcpy(
      forecast_array, 
      dict_find(iterator, WEATHER_FORECASTS)->value->data, 
      NUMBER_OF_FORECAST_DAYS * sizeof(PhoneWeatherModel)
    );

    SingleDayWeatherLayer* weather_layers[10];
    Layer *root_layer = window_get_root_layer(application->main_window);
    for(int i = 0; i < NUMBER_OF_FORECAST_DAYS; i++) {
      PhoneWeatherModel phone_model = forecast_array[i];
      SingleDayWeather weather = phone_model_to_single_day(phone_model, start_time + (i * SECONDS_PER_DAY));

      weather_layers[i] = single_day_weather_layer_create(
        GRect(0, i * SIZE_OF_FORECAST_DAY, 144, SIZE_OF_FORECAST_DAY), 
        weather
      );

      scroll_layer_add_child(scrolling_layer, single_day_weather_layer_get_layer(weather_layers[i]));
    }
    
    layer_add_child(root_layer, scroll_layer_get_layer(scrolling_layer));
  }
}

void handle_init(Application *application) {
  application->main_window = window_create();
  Layer *window_layer = window_get_root_layer(application->main_window);

  GRect window_bounds = layer_get_bounds(window_layer);

  app_message_register_inbox_received(inbox_received_handler);
  app_message_set_context(application);
  app_message_open(531, 9);

  window_set_background_color(application->main_window, GColorBlack);
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