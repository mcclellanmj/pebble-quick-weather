#include <pebble.h>
#include "single_day_layer.h"
#include "global_constants.h"
#include "forecast_layer.h"
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
  CopyingTextLayer *loading_layer;
  ScrollingForecastLayer *scrolling_forecast_layer;
} Application;

static const Mode INITIAL_MODE = ICON;

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

// FIXME: WARNING DRAGONS AHEAD, function varies from call to call with the same inputs :(
static void middle_click(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Got a click");
  ForecastLayer *forecast_layer = (ForecastLayer *) context;

  static Mode mode = INITIAL_MODE;
  Mode next_mode = mode == ICON ? TEXT : ICON;
  mode = next_mode;

  forecast_layer_set_mode(forecast_layer, next_mode);
}

void main_click_provider(ClickRecognizerRef recognizer) {
  window_single_click_subscribe(BUTTON_ID_SELECT, middle_click);
}

static ScrollLayer* create_weather_scroll_layer(Window *main_window, void *context) {
    ScrollLayer *scrolling_layer = scroll_layer_create(
      GRect(0, 0, 144, 168)
    );

    scroll_layer_set_callbacks(
      scrolling_layer, 
      (ScrollLayerCallbacks) {
        .click_config_provider = main_click_provider
      }
    );
    scroll_layer_set_context(scrolling_layer, context);
    scroll_layer_set_click_config_onto_window(scrolling_layer, main_window);
    scroll_layer_set_paging(scrolling_layer, true);
    scroll_layer_set_content_size(scrolling_layer, GSize(144, 330));
    scroll_layer_set_content_offset(scrolling_layer, GPoint(0, 168), true);

    return scrolling_layer;
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
    Layer *root_layer = window_get_root_layer(application->main_window);
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

    // Create the forecast layer and add it to a scrolling layer due to the size of it
    ForecastLayer *forecast_layer = forecast_layer_create(
      GRect(0, 0, 144, HEIGHT_OF_DAY * NUMBER_OF_DAYS), 
      forecast
    );

    forecast_layer_set_mode(forecast_layer, INITIAL_MODE);
    ScrollLayer *scrolling_layer = create_weather_scroll_layer(application->main_window, forecast_layer);
    scroll_layer_add_child(scrolling_layer, forecast_layer_get_layer(forecast_layer));

    layer_add_child(root_layer, scroll_layer_get_layer(scrolling_layer));
  }
}

static void draw_command(Layer *layer, GContext *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "drawing initial window");
  GDrawCommandImage **image = (GDrawCommandImage **) layer_get_data(layer);

  gdraw_command_image_get_bounds_size(*image);
  gdraw_command_image_draw(context, *image, GPoint(0, 0));
}

static void initial_window_load(Window* window) {
  Layer *root_layer = window_get_root_layer(window);

  Layer *happy_layer = layer_create_with_data(layer_get_frame(root_layer), sizeof(GDrawCommandImage **));
  GDrawCommandImage *happy_watch_command = gdraw_command_image_create_with_resource(RESOURCE_ID_HAPPY_WATCH);

  GDrawCommandImage **data = (GDrawCommandImage **) layer_get_data(happy_layer);
  *data = happy_watch_command;

  layer_set_update_proc(happy_layer, draw_command);
  layer_add_child(root_layer, happy_layer);
}

static void initial_window_unload(Window* window) {
  Application *application = (Application *) window_get_user_data(window);

}

void handle_init(Application *application) {
  application->main_window = window_create();

  /*
  window_set_window_handlers(application->main_window, (WindowHandlers) {
    .load = initial_window_load,
    .unload = initial_window_unload
  });
  */

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