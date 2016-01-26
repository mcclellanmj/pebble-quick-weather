#include "scrolling_forecast_layer.h"
static const Mode INITIAL_MODE = ICON;

static void middle_click(ClickRecognizerRef recognizer, void *context) {
  UNUSED(recognizer);

  ScrollingForecastLayer *scrolling_forecast_layer = (ScrollingForecastLayer *)context;

  scrolling_forecast_layer->mode = scrolling_forecast_layer->mode == ICON ? TEXT : ICON;

  scrolling_forecast_layer_set_mode(scrolling_forecast_layer, scrolling_forecast_layer->mode);
  layer_mark_dirty(scrolling_forecast_layer_get_layer(scrolling_forecast_layer));
}

static void click_provider(ClickRecognizerRef recognizer) {
  UNUSED(recognizer);
  
  window_single_click_subscribe(BUTTON_ID_SELECT, middle_click);
}

static ScrollLayer* create_scrolling_layer(GRect frame) {
  ScrollLayer *scrolling_layer = scroll_layer_create(frame);

  scroll_layer_set_paging(scrolling_layer, true);
  scroll_layer_set_content_size(scrolling_layer, GSize(frame.size.w, NUMBER_OF_DAYS * HEIGHT_OF_DAY));
  scroll_layer_set_content_offset(scrolling_layer, GPoint(0, frame.size.h), false);

  return scrolling_layer;
}

void scrolling_forecast_layer_set_click_on_window(ScrollingForecastLayer *scrolling_forecast_layer, Window *window) {
  scroll_layer_set_callbacks(
    scrolling_forecast_layer->scroll_layer, 
    (ScrollLayerCallbacks) {
      .click_config_provider = click_provider
    }
  );

  scroll_layer_set_context(scrolling_forecast_layer->scroll_layer, scrolling_forecast_layer);
  scroll_layer_set_click_config_onto_window(scrolling_forecast_layer->scroll_layer, window);
}

ScrollingForecastLayer* scrolling_forecast_layer_create(GRect frame, Forecast forecast) {
  ScrollingForecastLayer *scrolling_forecast_layer = (ScrollingForecastLayer *) malloc(sizeof(ScrollingForecastLayer));
  scrolling_forecast_layer->mode = INITIAL_MODE;

  ScrollLayer *scroll_layer = create_scrolling_layer(frame);
  scrolling_forecast_layer->scroll_layer = scroll_layer;

  for(int i = 0; i < NUMBER_OF_DAYS; i++) {
    SingleDayWeather day = forecast.days[i];
    SingleDayWeatherLayer *single_day_weather_layer = scrolling_forecast_layer->single_day_layers[i];
    single_day_weather_layer = single_day_weather_layer_create(
      GRect(0, i * HEIGHT_OF_DAY, 144, HEIGHT_OF_DAY), 
      day
    );

    scrolling_forecast_layer->single_day_layers[i] = single_day_weather_layer;
    scroll_layer_add_child(scroll_layer, single_day_weather_layer_get_layer(single_day_weather_layer));
  }

  return scrolling_forecast_layer;
}

void scrolling_forecast_layer_set_mode(ScrollingForecastLayer *scrolling_forecast_layer, Mode mode) {
  for(int i = 0; i < NUMBER_OF_DAYS; i++) {
    SingleDayWeatherLayer *single_day_weather_layer = scrolling_forecast_layer->single_day_layers[i];
    single_day_weather_layer_set_mode(single_day_weather_layer, mode);
  }
}

void scrolling_forecast_layer_destroy(ScrollingForecastLayer *scrolling_forecast_layer) {
  for(int i = 0; i < NUMBER_OF_DAYS; i++) {
    SingleDayWeatherLayer *single_day_weather_layer = scrolling_forecast_layer->single_day_layers[i];
    single_day_weather_layer_destroy(single_day_weather_layer);
  }

  scroll_layer_destroy(scrolling_forecast_layer->scroll_layer);
}

Layer* scrolling_forecast_layer_get_layer(ScrollingForecastLayer *scrolling_forecast_layer) {
  return scroll_layer_get_layer(scrolling_forecast_layer->scroll_layer);
}