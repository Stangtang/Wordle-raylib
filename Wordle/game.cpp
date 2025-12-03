#include "game.h"

char input[6];
std::uint8_t current_input_position;
std::vector<std::string> guesses_used;
std::string wordle_word;
std::vector<std::string> all_possible_words;
std::unordered_map<char, Letter_State> wordle_letter_state_map;
std::unordered_map<std::uint8_t, std::vector<Letter_State>> wordle_guess_number_letter_state_map;
bool is_wordle_word_guessed;

void import_words(std::vector<std::string>& all_possible_words)
{
    std::ifstream file("words.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            //std::transform(line.begin(), line.end(), line.begin(),
            //    [](unsigned char c) { return std::toupper(c); });
            all_possible_words.push_back(line);
        }
    }

    std::cout << "Imported Words:\n";
    for (std::string word : all_possible_words) {
        std::cout << word << '\n';
    }

    file.close();
}

std::string pick_random_word(const std::vector<std::string>& all_possible_words)
{
    std::mt19937 rd(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_int_distribution<std::size_t> dist(0, all_possible_words.size() - 1);
    std::string random_word = all_possible_words[dist(rd)];
    return random_word;
}

void initialize_input(char(&input)[6]) 
{
    for (int i = 0; i < 5; i++) {
        input[i] = '\0'; // null
    }

    current_input_position = 0;
    guesses_used.clear();
}

void initialize_wordle_game(char(&input)[6])
{
    initialize_input(input);
    wordle_word = pick_random_word(all_possible_words);
    for (char c = 'A'; c <= 'Z'; c++) {
        wordle_letter_state_map[c] = Letter_State::Unchecked;
    }
    wordle_guess_number_letter_state_map.clear();
    is_wordle_word_guessed = false;
    std::cout << "The random wordle word is: " + wordle_word << '\n';
}

bool handle_input(int clicked_key)
{
    if (is_wordle_word_guessed) {
        return true;
    }

    if (guesses_used.size() == 6) {
        return true;
    }

    int input_key = GetKeyPressed();

    if (clicked_key != KEY_NULL) { // letter was clicked
        input_key = clicked_key;
    }

    if (input_key >= KEY_A && input_key <= KEY_Z) {
        if (current_input_position == 5) { // already inputted 5 letters
            std::cout << "INVALID LETTER INPUT: already inputted 5 letters\n";
            std::string inputted_guess_string(input);
            std::cout << "Current Input: " << inputted_guess_string << '\n';
            return false; 
        }

        char input_char = 'A' + (input_key - KEY_A);
        input[current_input_position] = input_char;
        current_input_position++;
        std::cout << "inputted letter: " << input_char << '\n';
        std::string inputted_guess_string(input);
        std::cout << "Current Input: " << inputted_guess_string << '\n';
        return true;
    }

    if (input_key == KEY_ENTER) {
        if (current_input_position <= 4) { // inputted 4 letters or less
            std::cout << "INVALID ENTER: inputted 4 letters or less\n";
            std::string inputted_guess_string(input);
            std::cout << "Current Input: " << inputted_guess_string << '\n';
            return false; 
        }

        std::string inputted_guess_string(input);

        if (std::find(all_possible_words.begin(), all_possible_words.end(), inputted_guess_string) == all_possible_words.end()) { // input is not in the list
            std::cout << "INVALID ENTER: input is not a valid word\n";
            std::string inputted_guess_string(input);
            std::cout << "Current Input: " << inputted_guess_string << '\n';
            return false;
        }

        // inputted guess is valid
        guesses_used.push_back(inputted_guess_string);
        std::vector<Letter_State> guess_letter_states = evaluate_guess(wordle_word, inputted_guess_string);
        wordle_guess_number_letter_state_map[guesses_used.size()] = guess_letter_states;
        update_letter_state_map(wordle_letter_state_map, inputted_guess_string, guess_letter_states);

        for (int i = 0; i < 5; i++) {
            input[i] = '\0'; // null
        }
        current_input_position = 0;

        if (inputted_guess_string == wordle_word) {
            is_wordle_word_guessed = true;
        }

        return true;
    }

    if (input_key == KEY_BACKSPACE) {
        if (current_input_position == 0) {
            std::cout << "nothing was erased\n";
            std::string inputted_guess_string(input);
            std::cout << "Current Input: " << inputted_guess_string << '\n';
            return true;
        }

        input[current_input_position - 1] = '\0'; // null
        current_input_position--;
        std::cout << "one letter erased\n";
        std::string inputted_guess_string(input);
        std::cout << "Current Input: " << inputted_guess_string << '\n';
        return true;
    }

    return true;
}

bool is_letter_state_higher_priority(Letter_State a, Letter_State b) 
{
    auto p = [](Letter_State m) {
        switch (m) {
        case Letter_State::Green:  return 3;
        case Letter_State::Yellow: return 2;
        case Letter_State::Gray:   return 1;
        }
        return 0;
    };
    return p(a) > p(b);
}

void update_letter_state_map(std::unordered_map<char, Letter_State>& letter_state_map, const std::string& guess, const std::vector<Letter_State>& guess_letter_states)
{
    for (int i = 0; i < 5; i++) {
        char c = guess[i];
        Letter_State m = guess_letter_states[i];

        if (!letter_state_map.count(c) || is_letter_state_higher_priority(m, letter_state_map[c])) {
            letter_state_map[c] = m;
        }
    }
}

std::vector<Letter_State> evaluate_guess(const std::string& secret_word, const std::string& guess) 
{
    std::vector<Letter_State> result(5, Letter_State::Gray);
    std::unordered_map<char, int> freq;

    for (char c : secret_word) {
        freq[c]++;
    }

    // mark greens
    for (int i = 0; i < 5; i++) {
        if (guess[i] == secret_word[i]) {
            result[i] = Letter_State::Green;
            freq[guess[i]]--;
        }
    }

    // mark yellows
    for (int i = 0; i < 5; i++) {
        if (result[i] == Letter_State::Green) continue;

        char g = guess[i];
        if (freq[g] > 0) {
            result[i] = Letter_State::Yellow;
            freq[g]--;
        }
    }

    return result;
}