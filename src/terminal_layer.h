#include <pebble.h>
#include "copying_text_layer.h"

typedef struct {
  Layer *root_layer;
  TextLayer *text_layer;
  uint8_t max_bytes;
  uint8_t max_lines;
  uint8_t num_lines;
  char *characters;
} TerminalLayer;

TerminalLayer* terminal_layer_create(GRect frame, uint8_t max_characters);
void terminal_layer_output(TerminalLayer *terminal_layer, char* output);
void terminal_layer_destroy(TerminalLayer *terminal_layer);
Layer* terminal_layer_get_layer(TerminalLayer *terminal_layer);