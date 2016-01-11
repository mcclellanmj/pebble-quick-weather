#pragma once
#include <pebble.h>

uint8_t condition_code_to_icon(uint8_t condition_code);
const char* condition_code_to_text(uint8_t condition_code);
uint8_t condition_code_to_text_size(uint8_t condition_code);