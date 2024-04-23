#include <iostream>
#include <filesystem>
#include <vector>
#include <random>
#include <string>

namespace fs = std::filesystem;

std::vector<std::string> getFilesInDirectory(const fs::path& directory) {
    std::vector<std::string> files;
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (fs::is_regular_file(entry)) {
            files.push_back(entry.path().filename().string());
        }
    }
    return files;
}

std::string chooseRandomFile(const fs::path& directory) {
    std::vector<std::string> files = getFilesInDirectory(directory);
    
    if (files.empty()) {
        return "No files found in the directory.";
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, files.size() - 1);

    return files[dis(gen)];
}

void playGame(const fs::path& directory) {
    std::string chosenFile = chooseRandomFile(directory);

    if (chosenFile == "No files found in the directory.") {
        std::cout << chosenFile << std::endl;
        return;
    }

    std::cout << "Guess the name of a file in the directory: " << directory << std::endl;

    int attempts = 3;
    std::string userGuess;

    while (attempts > 0) {
        std::cout << "Enter your guess: ";
        std::cin >> userGuess;

        if (userGuess == chosenFile) {
            std::cout << "Congratulations! You guessed the correct file name." << std::endl;
            return;
        } else {
            std::cout << "Incorrect guess. Try again." << std::endl;
            attempts--;
        }
    }

    std::cout << "You lose! The correct file was: " << chosenFile << std::endl;
}

int main() {
    fs::path directory = ".";

    playGame(directory);

    return 0;
}
