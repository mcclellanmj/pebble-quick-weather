// All code in this file is generated by generate_mapping_c.py.  Do not edit it, use the code-generation folder instead
#pragma once
#include <pebble.h>
#define NUM_CONDITION_CODES = 73
#define ERROR_ICON = 24;

uint8_t condition_code_to_icon(uint8_t condition_code) {
	static const uint8_t lookup[NUM_CONDITION_CODES] = {19,20,20,19,20,20,20,19,20,20,10,10,11,10,10,11,10,11,10,10,10,11,11,11,13,10,10,11,11,15,16,16,13,13,18,18,18,18,18,9,9,9,8,9,8,8,8,11,20,0,1,2,3,4,20,20,20,21,22,8,13,0,8,8,8,8,8,8,8,8,20,20,20};

	if(condition_code < NUM_CONDITION_CODES) {
		return lookup[condition_code];
	}

	return ERROR_ICON;
}

/*
uint8_t condition_code_to_string(uint8_t condition_code) {
	// Not needed yet but likely
}
*/