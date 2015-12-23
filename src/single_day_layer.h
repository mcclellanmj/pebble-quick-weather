#pragma once
#include <pebble.h>
#include "copying_text_layer.h"

typedef struct {
  uint8_t forecast_code;
  int16_t high_temperature;
  int16_t low_temperature;
} WeatherInformation;

typedef struct {
  WeatherInformation weather_information;
  time_t date;
} SingleDayWeather;

typedef struct {
  SingleDayWeather weather;
  CopyingTextLayer *temperature_layer;
  CopyingTextLayer *day_layer;
  BitmapLayer *icon_layer;
} SingleDayWeatherLayer;

SingleDayWeatherLayer* single_day_weather_layer_create(GRect frame, SingleDayWeather weather);
void single_day_weather_layer_destroy(SingleDayWeatherLayer *copying_text_layer);