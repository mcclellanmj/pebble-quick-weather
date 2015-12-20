#include <pebble.h>
#include "scrolling_forecast_layer.h"

ScrollingForecastLayer* scrolling_forecast_layer_create(GRect frame, WeeklyForecast forecast) {
  Layer *root_layer = layer_create_with_data(frame, sizeof(ScrollingForecastLayer));
  ScrollingForecastLayer *scrolling_forecast_layer = (ScrollingForecastLayer *) layer_get_data(root_layer);
  *scrolling_forecast_layer = (ScrollingForecastLayer) {
    .root_layer = root_layer,
    .scroll_layer = scroll_layer_create(frame)
  };
  
  return scrolling_forecast_layer;
}

void scrolling_forecast_layer_destroy(ScrollingForecastLayer *scrolling_forecast_layer) {
  layer_destroy(scrolling_forecast_layer->root_layer);
  scroll_layer_destroy(scrolling_forecast_layer->scroll_layer);
}