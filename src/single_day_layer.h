#pragma once
#include <pebble.h>
#include "copying_text_layer.h"

typedef struct {
  bool valid;
  uint8_t forecast_code;
  int16_t high_temperature;
  int16_t low_temperature;
  time_t date;
} SingleDayWeather;

typedef struct {
  Layer *root_layer;
  SingleDayWeather weather;
  CopyingTextLayer *temperature_layer;
  CopyingTextLayer *date_layer;
  BitmapLayer *icon_layer;
  GBitmap* bitmap;
} SingleDayWeatherLayer;

SingleDayWeatherLayer* single_day_weather_layer_create(GRect frame, SingleDayWeather weather);
void single_day_weather_layer_destroy(SingleDayWeatherLayer *single_day_weather_layer);
Layer* single_day_weather_layer_get_layer(SingleDayWeatherLayer *single_day_weather_layer);