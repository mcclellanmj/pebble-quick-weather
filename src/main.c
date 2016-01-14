#include <pebble.h>
#include "single_day_layer.h"
#include "global_constants.h"
#include "scrolling_forecast_layer.h"
#include "util.h"

#define SECONDS_PER_DAY 86400

typedef enum {
  CELSIUS, 
  FAHRENHEIT
} TemperatureUnit;

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
  ScrollingForecastLayer *scrolling_forecast_layer;
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

static int16_t convert_to_unit(int8_t temperature, TemperatureUnit unit) {
  if(unit == CELSIUS) {
    return temperature;
  }

  return celsius_to_fahrenheit(temperature);
}

static SingleDayWeather phone_model_to_single_day(PhoneWeatherModel phone_model, time_t date, TemperatureUnit unit) {
  return (SingleDayWeather) {
    .valid = true,
    .date = date,
    .forecast_code = phone_model.forecast_code,
    .high_temperature = convert_to_unit(phone_model.high_temperature, FAHRENHEIT),
    .low_temperature = convert_to_unit(phone_model.low_temperature, FAHRENHEIT)
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
  
  if(request_type == WEATHER_REPORT) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Got a weather report");
    time_t start_time = dict_find(iterator, WEATHER_START)->value->int32;

    // Load the phone model, which is more optimized for size
    PhoneWeatherModel phone_model_array[NUMBER_OF_DAYS];
    memcpy(
      phone_model_array, 
      dict_find(iterator, WEATHER_FORECASTS)->value->data, 
      NUMBER_OF_DAYS * sizeof(PhoneWeatherModel)
    );

    // Convert from the phone model to the internal model
    SingleDayWeather single_day_weather[NUMBER_OF_DAYS];
    for(int i = 0; i < NUMBER_OF_DAYS; i++) {
      PhoneWeatherModel phone_model = phone_model_array[i];

      single_day_weather[i] = phone_model_to_single_day(
        phone_model, 
        start_time + (i * SECONDS_PER_DAY),
        // Needs to be pulling from the application
        FAHRENHEIT
      );
    }

    Forecast forecast;
    memcpy(forecast.days, single_day_weather, sizeof(SingleDayWeather) * NUMBER_OF_DAYS);

    // TODO: Need to move the scrolling layer setup to when the window first gets created
    // and just give it a not valid forecast that doesn't draw
    ScrollingForecastLayer *scrolling_forecast_layer = scrolling_forecast_layer_create(
      GRect(0, 0, 144, 168),
      forecast
    );

    scrolling_forecast_layer_set_click_on_window(scrolling_forecast_layer, application->main_window);

    Layer *root_layer = window_get_root_layer(application->main_window);
    layer_add_child(root_layer, scrolling_forecast_layer_get_layer(scrolling_forecast_layer));
  }
}

void handle_init(Application *application) {
  application->main_window = window_create();

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