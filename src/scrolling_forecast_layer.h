#include <pebble.h>
#include "global_constants.h"
#include "single_day_layer.h"

typedef struct {
  SingleDayWeather days[NUMBER_OF_DAYS];
} Forecast;

typedef struct {
  Layer* root_layer;
  ScrollLayer* scroll_layer;
  SingleDayWeatherLayer *single_day_layers[NUMBER_OF_DAYS];
} ScrollingForecastLayer;

ScrollingForecastLayer* scrolling_forecast_layer_create(GRect frame, Forecast weather);
void scrolling_forecast_layer_set_mode(ScrollingForecastLayer *scrolling_forecast_layer, Mode mode);
void scrolling_forecast_layer_destroy(ScrollingForecastLayer *scrolling_forecast_layer);
Layer* scrolling_forecast_layer_get_layer(ScrollingForecastLayer *scrolling_forecast_layer);