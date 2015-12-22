#pragma once
#include <pebble.h>
#include "copying_text_layer.h"

typedef struct {
  int16_t high_temperature;
  int16_t low_temperature;
  uint8_t icon;
} SingleDayWeather;

typedef struct {
  SingleDayWeather weather;
  CopyingTextLayer *temperature_layer;
  CopyingTextLayer *day_layer;
  BitmapLayer *icon_layer;
} SingleDayWeatherLayer;

SingleDayWeatherLayer* single_day_weather_layer_create(GRect frame, const char *text, size_t max);
void single_day_weather_layer_destroy(SingleDayWeatherLayer *copying_text_layer);