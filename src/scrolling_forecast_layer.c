#include "scrolling_forecast_layer.h"

static ScrollLayer* create_scrolling_layer(GRect frame, void *context) {
    ScrollLayer *scrolling_layer = scroll_layer_create(
      GRect(0, 0, frame.size.w, frame.size.h)
    );

    scroll_layer_set_context(scrolling_layer, context);
    //scroll_layer_set_click_config_onto_window(scrolling_layer, main_window);
    scroll_layer_set_paging(scrolling_layer, true);
    scroll_layer_set_content_size(scrolling_layer, GSize(frame.size.w, NUMBER_OF_DAYS * HEIGHT_OF_DAY));
    scroll_layer_set_content_offset(scrolling_layer, GPoint(0, frame.size.h), true);

    return scrolling_layer;
}

void scrolling_forecast_layer_set_click_on_window(ScrollingForecastLayer *scrolling_forecast_layer, Window *window) {
  // TODO: Set the click on this point 
}

ScrollingForecastLayer* scrolling_forecast_layer_create(GRect frame, Forecast forecast) {
  Layer *root_layer = layer_create_with_data(frame, sizeof(ScrollingForecastLayer));

  ScrollingForecastLayer *scrolling_forecast_layer = (ScrollingForecastLayer *) layer_get_data(root_layer);

  for(int i = 0; i < NUMBER_OF_DAYS; i++) {
    SingleDayWeather day = forecast.days[i];
    SingleDayWeatherLayer *single_day_weather_layer = scrolling_forecast_layer->single_day_layers[i];
    single_day_weather_layer = single_day_weather_layer_create(
      GRect(0, i * HEIGHT_OF_DAY, 144, HEIGHT_OF_DAY), 
      day
    );

    scrolling_forecast_layer->single_day_layers[i] = single_day_weather_layer;
    layer_add_child(root_layer, single_day_weather_layer_get_layer(single_day_weather_layer));
  }

  ScrollLayer *scroll_layer = scroll_layer_create(frame);

  scrolling_forecast_layer->scroll_layer = scroll_layer;
  scrolling_forecast_layer->root_layer = root_layer;

  return scrolling_forecast_layer;
}

void scrolling_forecast_layer_set_mode(ScrollingForecastLayer *scrolling_forecast_layer, Mode mode) {
  for(int i = 0; i < NUMBER_OF_DAYS; i++) {
    SingleDayWeatherLayer *single_day_weather_layer = scrolling_forecast_layer->single_day_layers[i];
    single_day_weather_layer_set_mode(single_day_weather_layer, mode);

    layer_mark_dirty(scrolling_forecast_layer->root_layer);
  }
}

void scrolling_forecast_layer_destroy(ScrollingForecastLayer *scrolling_forecast_layer) {
  for(int i = 0; i < NUMBER_OF_DAYS; i++) {
    SingleDayWeatherLayer *single_day_weather_layer = scrolling_forecast_layer->single_day_layers[i];

    single_day_weather_layer_destroy(single_day_weather_layer);
  }

  scroll_layer_destroy(scrolling_forecast_layer->scroll_layer);
  layer_destroy(scrolling_forecast_layer->root_layer);
  free(scrolling_forecast_layer);
}

Layer* scrolling_forecast_layer_get_layer(ScrollingForecastLayer *scrolling_forecast_layer) {
  return scrolling_forecast_layer->root_layer;
}