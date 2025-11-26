#pragma once
#include <stack>
#include "raylib.h"
#include <iostream>
#include "game.h"

enum Screen {
	start,
	wordle,
	duordle,
	quordle,
};

void handle_screens(std::stack<Screen>& screen_stack);
void start_screen(std::stack<Screen>& screen_stack);
void wordle_screen(std::stack<Screen>& screen_stack);

typedef struct {
	float duration;
	float timeLeft;
	bool active;
} Fading_Text;