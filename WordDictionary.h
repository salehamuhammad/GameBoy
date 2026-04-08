#ifndef WORD_DICTIONARY_H
#define WORD_DICTIONARY_H

#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>

using namespace std;

class WordDictionary {
private:
    // Maximum number of words to store
    static const int MAX_WORDS = 10000;
    static const int WORD_LENGTH = 5;

    // Array to store words
    char words[MAX_WORDS][WORD_LENGTH + 1];
    int wordCount;

    // Method to count total words in the file
    int countWordsInFile(const char* filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Could not open dictionary file";
        }

        int count = 0;
        char buffer[100];
        while (file.getline(buffer, sizeof(buffer))) {
            // Only count 5-letter words
            if (strlen(buffer) == WORD_LENGTH) {
                ++count;
            }
        }
        file.close();
        return count;
    }

    // Method to load words from file
    void loadWordsFromFile(const char* filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            cout << "Could not open dictionary file";
        }

        wordCount = 0;
        char buffer[100];
        while (file.getline(buffer, sizeof(buffer)) && wordCount < MAX_WORDS) {
            // Only add 5-letter words
            if (strlen(buffer) == WORD_LENGTH) {
                // Convert to uppercase manually
                for (int i = 0; i < WORD_LENGTH; ++i) {
                    if (buffer[i] >= 'a' && buffer[i] <= 'z') {
                        buffer[i] -= ('a' - 'A');
                    }
                }

                // Safely copy to words array
                for (int i = 0; i <= WORD_LENGTH; ++i) {
                    words[wordCount][i] = buffer[i];
                }
                ++wordCount;
            }
        }
        file.close();

        if (wordCount == 0) {
            cout << "No valid words found in dictionary";
        }
    }

public:
    // Constructor that loads words from file
    WordDictionary(const char* filename = "dictionary.txt") {
        // Initialize random seed
        srand(static_cast<unsigned>(time(nullptr)));

        // Load words from file
        loadWordsFromFile(filename);
    }

    // Get a random word from the dictionary
    const char* getRandomWord() {
        if (wordCount == 0) {
            cout << "No words in dictionary";
        }

        // Select a random word
        int index = std::rand() % wordCount;
        return words[index];
    }

    // Check if a word exists in the dictionary
    bool isValidWord(const char* word) {
        for (int i = 0; i < wordCount; ++i) {
            if (strcmp(word, words[i]) == 0) {
                return true;
            }
        }
        return false;
    }

    // Get total word count
    int getWordCount() const {
        return wordCount;
    }
};

#endif
