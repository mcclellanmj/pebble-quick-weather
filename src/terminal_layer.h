#include <pebble.h>
#include "copying_text_layer.h"

typedef struct {
  Layer *root_layer;
  CopyingTextLayer *line_layer;
  uint8_t number_of_lines;
  uint8_t max_line_width;
  uint8_t entry_number;
} TerminalLayer;

TerminalLayer* terminal_layer_create(GRect frame, uint8_t number_of_lines, uint8_t max_line_width);
void terminal_layer_output(TerminalLayer *terminal_layer, char* output);
void terminal_layer_destroy(TerminalLayer *terminal_layer);
Layer* terminal_layer_get_layer(TerminalLayer *terminal_layer);