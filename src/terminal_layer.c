#include <pebble.h>
#include "terminal_layer.h"
#include "copying_text_layer.h"

TerminalLayer* terminal_layer_create(GRect frame, uint8_t number_of_lines, uint8_t max_line_width) {
  Layer *root_layer = layer_create_with_data(frame, sizeof(TerminalLayer));

  // FIXME: THis would probably work better as a single giant text layer instead of multiple little ones
  TerminalLayer *terminal_layer = (TerminalLayer *) layer_get_data(root_layer);
  *terminal_layer = (TerminalLayer) {
    .root_layer = root_layer,
    .number_of_lines = number_of_lines,
    .max_line_width = max_line_width,
    .entry_number = 0,
    .line_layer = (CopyingTextLayer *) malloc(sizeof(CopyingTextLayer) * number_of_lines) 
  };

  return terminal_layer;
}

void terminal_layer_output(TerminalLayer *terminal_layer, char* output) {

}

void terminal_layer_destroy(TerminalLayer *terminal_layer) {

}

Layer* terminal_layer_get_layer(TerminalLayer *terminal_layer) {

}