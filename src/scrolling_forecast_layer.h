#pragma once
#include <pebble.h>

typedef struct {
  Layer *root_layer;
  ScrollLayer *scroll_layer;
} ScrollingForecastLayer;

typedef struct {
  uint16_t high_temperature;
  uint16_t low_temperature;
  uint8_t icon_offset;
} DayForecast;

typedef struct {
  bool valid;
  DayForecast day_forecast[7];
  time_t start_time;
} WeeklyForecast;

ScrollingForecastLayer* scrolling_forecast_layer_create(GRect frame, WeeklyForecast forecast);
void scrolling_forecast_layer_destroy(ScrollingForecastLayer *forecast);