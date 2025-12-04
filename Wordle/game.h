#pragma once
#include "raylib.h"
#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <random>
#include <ctime>
#include <unordered_map>
#include <algorithm>
#include <cctype>

enum class Letter_State { Gray, Yellow, Green, Unchecked };
extern char input[6]; // +1 for terminating NUL
extern std::uint8_t current_input_position; // store value 0–5
extern std::vector<std::string> guesses_used; // length 0-6
extern std::vector<std::string> all_possible_random_words;
extern std::vector<std::string> all_possible_guess_words;
extern std::string wordle_word;
void initialize_input(char(&input)[6]);
void initialize_wordle_game(char(&input)[6]);
void import_words(std::vector<std::string>& all_possible_random_words, std::vector<std::string>& all_possible_guess_words);
std::string pick_random_word(const std::vector<std::string>& all_possible_random_words);
bool handle_input(int clicked_key);
extern std::unordered_map<char, Letter_State> wordle_letter_state_map;
extern std::unordered_map<std::uint8_t, std::vector<Letter_State>> wordle_guess_number_letter_state_map;
std::vector<Letter_State> evaluate_guess(const std::string& secret_word, const std::string& guess);
void update_letter_state_map(std::unordered_map<char, Letter_State>& letter_state_map, const std::string& guess, const std::vector<Letter_State>& guess_letter_states);
bool is_letter_state_higher_priority(Letter_State a, Letter_State b);
extern bool is_wordle_word_guessed;