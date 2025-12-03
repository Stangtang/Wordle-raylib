#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <filesystem>

bool read_file(const std::string& file_name, std::vector<std::string>& words) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return false;
    }

    std::string word;
    while (std::getline(file, word)) {
        words.push_back(word);
    }

    file.close();
    return true;
}

void write_file(const std::string& file_name, const std::vector<std::string>& words) {
    std::ofstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    for (const auto& word : words) {
        file << word << std::endl;
    }

    file.close();
}

bool insert_word_sorted(std::vector<std::string>& words, const std::string& new_word) {
    if (std::find(words.begin(), words.end(), new_word) != words.end()) {
        std::cout << "The word \"" << new_word << "\" already exists in the list!" << std::endl;
        return false;
    }

    auto it = std::lower_bound(words.begin(), words.end(), new_word);
    words.insert(it, new_word);
    return true;
}

bool verify_file_contents(const std::vector<std::string>& words) {
    for (std::string word : words) {
        if (word.length() != 5) {
            std::cout << "Invalid word length for word: " << word << std::endl;
            return false;
        }

        for (char c : word) {
            if (!('A' <= c && 'Z' >= c)) {
                std::cout << "Invalid character '" << c << "' in word: " << word << std::endl;
                return false;
            }
        }
    }
    return true;
}

int main() {
    std::filesystem::path file_path = std::filesystem::current_path();
    file_path = file_path.parent_path() / "Wordle" / "words.txt";   // navigate to words.txt

    std::vector<std::string> words;

    if (!read_file(file_path.string(), words)) {
        return 1;
    }

    if (!verify_file_contents(words)) {
        std::cout << "Errors found with file contents!\n";
    }

    std::string new_word;
    std::cout << "Enter 5-letter words to insert (type 'quit' to stop):" << std::endl;

    while (true) {
        std::cout << "Enter a 5-letter word (or 'quit' to stop): ";
        std::cin >> new_word;

        if (new_word == "quit") {
            //std::cout << "Exiting..." << std::endl;
            break;
        }

        if (new_word.length() != 5) {
            std::cout << "Invalid input! Please enter a 5-letter word." << std::endl;
            continue;
        }

        std::transform(new_word.begin(), new_word.end(), new_word.begin(), [](unsigned char c) { return std::toupper(c); });

        if (insert_word_sorted(words, new_word)) {
            write_file(file_path.string(), words);
            std::cout << "Word inserted and file updated!" << std::endl;
        }
    }

    return 0;
}
