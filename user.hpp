#pragma once
#include <string>
#include <unordered_map>

class User {
public:

    std::string id;
    std::unordered_map<std::string, double> ratings;
    User();
    User(const std::string& id);
    void addRating(const std::string& movieId, double rating);
    bool hasRated(const std::string& movieId) const;
    double getRating(const std::string& movieId) const;
    // Calculate the average rating for all movies rated by the user
    double getAverageRating() const;
    const std::string& getId() const;
};
