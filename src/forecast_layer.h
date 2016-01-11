#include <pebble.h>
#include "global_constants.h"
#include "single_day_layer.h"

typedef struct {
  SingleDayWeather days[NUMBER_OF_DAYS];
} Forecast;

typedef struct {
  Layer* root_layer;
  SingleDayWeatherLayer *single_day_layers[NUMBER_OF_DAYS];
} ForecastLayer;

ForecastLayer* forecast_layer_create(GRect frame, Forecast weather);
void forecast_layer_set_mode(ForecastLayer *forecast_layer, Mode mode);
void forecast_layer_destroy(ForecastLayer *forecast_layer);
Layer* forecast_layer_get_layer(ForecastLayer *forecast_layer);