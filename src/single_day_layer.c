#include <pebble.h>
#include "single_day_layer.h"
#include "copying_text_layer.h"
#include "condition_codes.h"

static CopyingTextLayer* create_date_layer(GRect frame, time_t current_time) {
  char high_low_text[9];
  // Formatted as 20 - Mon
  strftime(high_low_text, 9, "%d - %a", localtime(&current_time));

  return copying_text_layer_create(frame, high_low_text, 9);
}

static CopyingTextLayer* create_temperate_layer(GRect frame, int16_t high, int16_t low) {
  char high_low_text[10];
  snprintf(high_low_text, 10, "%d/%d", high, low);

  return copying_text_layer_create(frame, high_low_text, 10);
}

SingleDayWeatherLayer* single_day_weather_layer_create(GRect frame, SingleDayWeather weather) {
  Layer *root_layer = layer_create_with_data(frame, sizeof(SingleDayWeatherLayer));

  SingleDayWeatherLayer *single_day_weather_layer = (SingleDayWeatherLayer *) layer_get_data(root_layer);
  *single_day_weather_layer = (SingleDayWeatherLayer) {
    .root_layer = root_layer,
    .weather = weather,
    .temperature_layer = create_temperate_layer(frame, weather.high_temperature, weather.low_temperature),
    .date_layer = create_date_layer(frame, weather.date),
    .icon_layer = bitmap_layer_create(frame)
  };

  // Set the bitmap layer to the correct icon
  bitmap_layer_set_bitmap(
    single_day_weather_layer->icon_layer, 
    gbitmap_create_with_resource(condition_code_to_icon(weather.forecast_code))
  );

  return single_day_weather_layer;
}

void single_day_weather_layer_destroy(SingleDayWeatherLayer *single_day_weather_layer) {
  copying_text_layer_destroy(single_day_weather_layer->date_layer);
  copying_text_layer_destroy(single_day_weather_layer->temperature_layer);

  GBitmap* bitmap = (GBitmap*) bitmap_layer_get_bitmap(single_day_weather_layer->icon_layer);
  gbitmap_destroy(bitmap);
  bitmap_layer_destroy(single_day_weather_layer->icon_layer);

  layer_destroy(single_day_weather_layer->root_layer);
  free(single_day_weather_layer);
}