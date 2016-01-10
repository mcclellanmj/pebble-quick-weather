#include "forecast_layer.h"

ForecastLayer* forecast_layer_create(GRect frame, Forecast forecast) {
  Layer *root_layer = layer_create_with_data(frame, sizeof(ForecastLayer));

  ForecastLayer *forecast_layer = (ForecastLayer *) layer_get_data(root_layer);

  for(int i = 0; i < NUMBER_OF_DAYS; i++) {
    SingleDayWeather day = forecast.days[i];
    SingleDayWeatherLayer *single_day_weather_layer = forecast_layer->single_day_layers[i];
    single_day_weather_layer = single_day_weather_layer_create(
      GRect(0, i * HEIGHT_OF_DAY, 144, HEIGHT_OF_DAY), 
      day
    );

    layer_add_child(root_layer, single_day_weather_layer_get_layer(single_day_weather_layer));
  }

  forecast_layer->root_layer = root_layer;

  return forecast_layer;
}

void forecast_layer_destroy(ForecastLayer *forecast_layer) {
  for(int i = 0; i < NUMBER_OF_DAYS; i++) {
    SingleDayWeatherLayer *single_day_weather_layer = forecast_layer->single_day_layers[i];

    single_day_weather_layer_destroy(single_day_weather_layer);
  }

  layer_destroy(forecast_layer->root_layer);
  free(forecast_layer);
}

Layer* forecast_layer_get_layer(ForecastLayer *forecast_layer) {
  return forecast_layer->root_layer;
}