#include <pebble.h>
#include "terminal_layer.h"
#include "copying_text_layer.h"

TerminalLayer* terminal_layer_create(GRect frame, uint8_t max_characters) {
  Layer *root_layer = layer_create_with_data(frame, sizeof(TerminalLayer));

  TerminalLayer *terminal_layer = (TerminalLayer *) layer_get_data(root_layer);
  *terminal_layer = (TerminalLayer) {
    .root_layer = root_layer,
    .text_layer = text_layer_create(frame),
    .max_characters = max_characters,
    .characters = (char *) malloc(sizeof(char) * max_characters),
    .num_characters = 0,
  };

  layer_add_child(terminal_layer->root_layer, text_layer_get_layer(terminal_layer->text_layer));
  return terminal_layer;
}

// FIXME: This needs testing, seems a little risky
static void delete_line(char* lines, uint8_t max_characters) {
  char* new_line = index(lines, '\n') + sizeof(char);

  // TODO: Return an error code?
  if(new_line == NULL) {
    return;
  }

  memmove(lines, new_line, max_characters - (lines - new_line));
}

void terminal_layer_output(TerminalLayer *terminal_layer, char *output) {
  int output_size = strnlen(output, terminal_layer->max_characters);

  while(terminal_layer->num_characters + output_size > terminal_layer->max_characters) {
    delete_line(terminal_layer->characters, terminal_layer->max_characters);
  }

  strcat(terminal_layer->characters, output);
}

void terminal_layer_destroy(TerminalLayer *terminal_layer) {
  text_layer_destroy(terminal_layer->text_layer);
  layer_destroy(terminal_layer->root_layer);
}

Layer* terminal_layer_get_layer(TerminalLayer *terminal_layer) {
  return terminal_layer->root_layer;
}