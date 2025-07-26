#include "data_loader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

std::vector<User> DataLoader::loadUsersFromFile(const std::string& filename) {
    std::unordered_map<std::string, User> userMap;

    std::ifstream file(filename);
    if(!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return {};
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::string userId;
        std::string movieName;
        std::string ratingStr;

        std::stringstream ss(line);

        // Read userId
        std::getline(ss, userId, ',');

        // Read movieName (handle quotes)
        if (ss.peek() == '"') {
            ss.get(); // Remove opening quote
            std::getline(ss, movieName, '"');
            ss.get(); // Remove comma after closing quote
        }   else {
         std::getline(ss, movieName, ',');
        }
        // Read rating
        std::getline(ss, ratingStr, ',');

        try {
            double rating = std::stod(ratingStr);

           if (!userMap.count(userId)) {
                userMap[userId] = User(userId);
            }

            userMap[userId].addRating(movieName, rating);
           
        } catch (const std::invalid_argument& e) {
            std::cerr << "Skipping malformed line: " << line << std::endl;
        }
    }

    std::vector<User> users;
    for (const auto& [id, user] : userMap) {
        users.push_back(user);
    }

    return users;
}