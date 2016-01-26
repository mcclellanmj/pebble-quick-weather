// All code in this file is generated by generate_mapping_c.py.  Do not edit it, use the code-generation folder instead
#include <pebble.h>
#define NUM_CONDITION_CODES 73
#define ERROR_ICON 24;

uint8_t condition_code_to_icon(uint8_t condition_code) {
  static const uint8_t lookup[NUM_CONDITION_CODES] = {RESOURCE_ID_WEATHER_ICON_19,RESOURCE_ID_WEATHER_ICON_20,RESOURCE_ID_WEATHER_ICON_20,RESOURCE_ID_WEATHER_ICON_19,RESOURCE_ID_WEATHER_ICON_20,RESOURCE_ID_WEATHER_ICON_20,RESOURCE_ID_WEATHER_ICON_20,RESOURCE_ID_WEATHER_ICON_19,RESOURCE_ID_WEATHER_ICON_20,RESOURCE_ID_WEATHER_ICON_20,RESOURCE_ID_WEATHER_ICON_10,RESOURCE_ID_WEATHER_ICON_10,RESOURCE_ID_WEATHER_ICON_11,RESOURCE_ID_WEATHER_ICON_10,RESOURCE_ID_WEATHER_ICON_10,RESOURCE_ID_WEATHER_ICON_11,RESOURCE_ID_WEATHER_ICON_10,RESOURCE_ID_WEATHER_ICON_11,RESOURCE_ID_WEATHER_ICON_10,RESOURCE_ID_WEATHER_ICON_10,RESOURCE_ID_WEATHER_ICON_10,RESOURCE_ID_WEATHER_ICON_11,RESOURCE_ID_WEATHER_ICON_11,RESOURCE_ID_WEATHER_ICON_11,RESOURCE_ID_WEATHER_ICON_13,RESOURCE_ID_WEATHER_ICON_10,RESOURCE_ID_WEATHER_ICON_10,RESOURCE_ID_WEATHER_ICON_11,RESOURCE_ID_WEATHER_ICON_11,RESOURCE_ID_WEATHER_ICON_15,RESOURCE_ID_WEATHER_ICON_16,RESOURCE_ID_WEATHER_ICON_16,RESOURCE_ID_WEATHER_ICON_13,RESOURCE_ID_WEATHER_ICON_13,RESOURCE_ID_WEATHER_ICON_18,RESOURCE_ID_WEATHER_ICON_18,RESOURCE_ID_WEATHER_ICON_18,RESOURCE_ID_WEATHER_ICON_18,RESOURCE_ID_WEATHER_ICON_18,RESOURCE_ID_WEATHER_ICON_9,RESOURCE_ID_WEATHER_ICON_9,RESOURCE_ID_WEATHER_ICON_9,RESOURCE_ID_WEATHER_ICON_8,RESOURCE_ID_WEATHER_ICON_9,RESOURCE_ID_WEATHER_ICON_8,RESOURCE_ID_WEATHER_ICON_8,RESOURCE_ID_WEATHER_ICON_8,RESOURCE_ID_WEATHER_ICON_11,RESOURCE_ID_WEATHER_ICON_20,RESOURCE_ID_WEATHER_ICON_0,RESOURCE_ID_WEATHER_ICON_1,RESOURCE_ID_WEATHER_ICON_2,RESOURCE_ID_WEATHER_ICON_3,RESOURCE_ID_WEATHER_ICON_4,RESOURCE_ID_WEATHER_ICON_20,RESOURCE_ID_WEATHER_ICON_20,RESOURCE_ID_WEATHER_ICON_20,RESOURCE_ID_WEATHER_ICON_21,RESOURCE_ID_WEATHER_ICON_22,RESOURCE_ID_WEATHER_ICON_8,RESOURCE_ID_WEATHER_ICON_13,RESOURCE_ID_WEATHER_ICON_0,RESOURCE_ID_WEATHER_ICON_8,RESOURCE_ID_WEATHER_ICON_8,RESOURCE_ID_WEATHER_ICON_8,RESOURCE_ID_WEATHER_ICON_8,RESOURCE_ID_WEATHER_ICON_8,RESOURCE_ID_WEATHER_ICON_8,RESOURCE_ID_WEATHER_ICON_8,RESOURCE_ID_WEATHER_ICON_8,RESOURCE_ID_WEATHER_ICON_20,RESOURCE_ID_WEATHER_ICON_20,RESOURCE_ID_WEATHER_ICON_20};

  if(condition_code < NUM_CONDITION_CODES) {
    return lookup[condition_code];
  }

  return ERROR_ICON;
}

const char* condition_code_to_text(uint8_t condition_code) {
  static const char* lookup[NUM_CONDITION_CODES] = {"t-storm w/ light rain","t-storm w/ rain","t-storm w/ heavy rain","light t-storm","t-storm","heavy t-storm","ragged t-storm","t-storm w/ light drizzle","t-storm w/ drizzle","t-storm w/ heavy drizzle","light drizzle","drizzle","heavy drizzle","light drizzle rain","drizzle rain","heavy drizzle rain","shower rain and drizzle","heavy shower rain and drizzle","shower drizzle","light rain","moderate rain","heavy rain","very heavy rain","extreme rain","freezing rain","light shower rain","shower rain","heavy shower rain","ragged shower rain","light snow","snow","heavy snow","sleet","shower sleet","light rain and snow","rain and snow","light shower snow","shower snow","heavy shower snow","mist","smoke","haze","sand, dust whirls","fog","sand","dust","volcanic ash","squalls","tornado","clear sky","few clouds","scattered clouds","broken clouds","overcast clouds","tornado","tropical storm","hurricane","cold","hot","windy","hail","calm","light breeze","gentle breeze","moderate breeze","fresh breeze","strong breeze","high wind, near gale","gale","severe gale","storm","violent storm","hurricane"};

  if(condition_code < NUM_CONDITION_CODES) {
    return lookup[condition_code];
  }

  return "ERROR";
}

uint8_t condition_code_to_text_size(uint8_t condition_code) {
  static const uint8_t lookup[NUM_CONDITION_CODES] = {23,17,23,15,9,15,16,26,20,26,15,9,15,20,14,20,25,31,16,12,15,12,17,14,15,19,13,19,20,12,6,12,7,14,21,15,19,13,19,6,7,6,19,5,6,6,14,9,9,11,12,18,15,17,9,16,11,6,5,7,6,6,14,15,17,14,15,22,6,13,7,15,11};

  if(condition_code < NUM_CONDITION_CODES) {
    return lookup[condition_code];
  }

  return 0;
}