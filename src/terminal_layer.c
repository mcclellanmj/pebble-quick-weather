#include <pebble.h>
#include "terminal_layer.h"
#include "copying_text_layer.h"
#include "util.h"

TerminalLayer* terminal_layer_create(GRect frame, uint8_t max_characters) {
  Layer *root_layer = layer_create_with_data(frame, sizeof(TerminalLayer));

  TerminalLayer *terminal_layer = (TerminalLayer *) layer_get_data(root_layer);
  *terminal_layer = (TerminalLayer) {
    .root_layer = root_layer,
    .text_layer = text_layer_create(frame),
    .max_characters = 255,
    .max_lines = 10,
    .characters = (char *) malloc(sizeof(char) * max_characters),
    .num_characters = 0,
    .num_lines = 0
  };

  layer_add_child(terminal_layer->root_layer, text_layer_get_layer(terminal_layer->text_layer));
  text_layer_set_text(terminal_layer->text_layer, terminal_layer->characters);
  return terminal_layer;
}

/*
static int count_characters(char* string, char* character) {
  char* retValue = strtok(string, character);
  if(retValue == NULL) {
    return 0; 
  }

  int i = 1;
  while(strtok(NULL, character) != NULL) {
    i++;
  }

  return i;
}
*/

void terminal_layer_output(TerminalLayer *terminal_layer, char *output) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "adding [%s]", output);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "current line count: [%d]", terminal_layer->num_lines);

  while(terminal_layer->num_lines > terminal_layer->max_lines) {
    delete_line(terminal_layer->characters, terminal_layer->max_characters);
    terminal_layer->num_lines--;
  }

  strcat(terminal_layer->characters, "> ");
  strcat(terminal_layer->characters, output);
  strcat(terminal_layer->characters, "\n");
  terminal_layer->num_lines++;

  layer_mark_dirty(terminal_layer->root_layer);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "characters is now [%s]", terminal_layer->characters);
}

void terminal_layer_destroy(TerminalLayer *terminal_layer) {
  text_layer_destroy(terminal_layer->text_layer);
  layer_destroy(terminal_layer->root_layer);
}

Layer* terminal_layer_get_layer(TerminalLayer *terminal_layer) {
  return terminal_layer->root_layer;
}