#include "user.hpp"

User::User() : id("") {}

User::User(const std::string& id) : id(id) {}

// Add a rating for a specific movie
void User::addRating(const std::string& movieId, double rating) {
    ratings[movieId] = rating;
}

// Check if the user has rated a specific movie
bool User::hasRated(const std::string& movieId) const {
    return ratings.count(movieId); 
}

// Get the rating
double User::getRating(const std::string& movieId) const {
    auto it = ratings.find(movieId);
    return (it != ratings.end()) ? it->second : 0.0;
}

// Calculates average rating for all movies rated by the user
double User::getAverageRating() const {
    if (ratings.empty()) {
        return 0.0;
    }

    double total = 0.0;
    for (const auto& [movie, rating] : ratings) {
        total += rating;
    }

    return total / ratings.size();
}

const std::string& User::getId() const {
    return id;
}
