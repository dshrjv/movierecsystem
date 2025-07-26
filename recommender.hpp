#pragma once
#include <vector>
#include <string>
#include "user.hpp"

class Recommender {
public:
    Recommender(const std::vector<User>& users);
    // Recommender function that returns top N movie recommendations
    std::vector<std::string> recommendMovies(const User& target, int topN);
    User* getUserById(const std::string& id);
    std::vector<std::string> recommend(const User& target, int topN);
    double predictRating(const User& target, const std::string& movieId);
    
private:
    std::vector<User> users;
};
