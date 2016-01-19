#include <pebble.h>
#include "single_day_layer.h"
#include "copying_text_layer.h"
#include "condition_codes.h"

static CopyingTextLayer* create_date_layer(GRect frame, time_t current_time) {
  char date_text[9];

  // Formatted as 20 - Mon
  strftime(date_text, 9, "%d - %a", localtime(&current_time));

  return copying_text_layer_create(frame, date_text, 9);
}

static CopyingTextLayer* create_temperature_layer(GRect frame, int16_t high, int16_t low) {
  char high_low_text[10];
  snprintf(high_low_text, 10, "%d/%d", high, low);

  CopyingTextLayer* copying_text_layer = copying_text_layer_create(frame, high_low_text, 10);
  copying_text_layer_set_font(copying_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));

  return copying_text_layer;
}

SingleDayWeatherLayer* single_day_weather_layer_create(GRect frame, SingleDayWeather weather) {
  Layer *root_layer = layer_create_with_data(frame, sizeof(SingleDayWeatherLayer));

  SingleDayWeatherLayer *single_day_weather_layer = (SingleDayWeatherLayer *) layer_get_data(root_layer);

  *single_day_weather_layer = (SingleDayWeatherLayer) {
    .root_layer = root_layer,
    .weather = weather,
    .temperature_layer = create_temperature_layer(
      GRect(12, 8, 50, 30),
      weather.high_temperature,
      weather.low_temperature
    ),
    .date_layer = create_date_layer(
      GRect(4, 0, 80, 15),
      weather.date
    ),
    .description_layer = copying_text_layer_create(
      GRect(64, 0, 70, 33), 
      condition_code_to_text(weather.forecast_code), 
      condition_code_to_text_size(weather.forecast_code)
    ),
    .bitmap = gbitmap_create_with_resource(condition_code_to_icon(weather.forecast_code)),
    .icon_layer = bitmap_layer_create(GRect(100, 1, 33, 33))
  };

  copying_text_layer_set_overflow(single_day_weather_layer->description_layer, GTextOverflowModeFill);
  copying_text_layer_set_text_alignment(single_day_weather_layer->description_layer, GTextAlignmentRight);

  // Set the bitmap layer to the correct icon
  bitmap_layer_set_bitmap(
    single_day_weather_layer->icon_layer, 
    single_day_weather_layer->bitmap
  );

  bitmap_layer_set_background_color(single_day_weather_layer->icon_layer, GColorClear);

  layer_add_child(root_layer, copying_text_layer_get_layer(single_day_weather_layer->temperature_layer));
  layer_add_child(root_layer, copying_text_layer_get_layer(single_day_weather_layer->date_layer));
  layer_add_child(root_layer, copying_text_layer_get_layer(single_day_weather_layer->description_layer));
  layer_add_child(root_layer, bitmap_layer_get_layer(single_day_weather_layer->icon_layer));

  single_day_weather_layer_set_mode(single_day_weather_layer, ICON);

  return single_day_weather_layer;
}

Layer* single_day_weather_layer_get_layer(SingleDayWeatherLayer *single_day_weather_layer) {
  return single_day_weather_layer->root_layer;
}

void single_day_weather_layer_set_mode(SingleDayWeatherLayer *single_day_weather_layer, Mode mode) {
  switch(mode) {
    case TEXT:
      layer_set_hidden(bitmap_layer_get_layer(single_day_weather_layer->icon_layer), true);
      layer_set_hidden(copying_text_layer_get_layer(single_day_weather_layer->description_layer), false);
      break;
    case ICON:
      layer_set_hidden(bitmap_layer_get_layer(single_day_weather_layer->icon_layer), false);
      layer_set_hidden(copying_text_layer_get_layer(single_day_weather_layer->description_layer), true);
      break;
  }
}

void single_day_weather_layer_destroy(SingleDayWeatherLayer *single_day_weather_layer) {
  copying_text_layer_destroy(single_day_weather_layer->date_layer);
  copying_text_layer_destroy(single_day_weather_layer->temperature_layer);
  gbitmap_destroy(single_day_weather_layer->bitmap);
  bitmap_layer_destroy(single_day_weather_layer->icon_layer);

  layer_destroy(single_day_weather_layer->root_layer);
}