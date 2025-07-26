#include "recommender.hpp"
#include "utils.hpp"
#include <unordered_map>
#include <set>
#include <algorithm>
#include <iostream>

Recommender::Recommender(const std::vector<User>& users) : users(users) {}

std::vector<std::string> Recommender::recommendMovies(const User& target, int topN) {
    return recommend(target, topN); // Calls recommend to get the movies
}

User* Recommender::getUserById(const std::string& id) {
    for (auto& user : users) {
        if (user.id == id) {
            return &user; 
        }
    }
    return nullptr; 
}

std::vector<std::string> Recommender::recommend(const User& target, int topN) {
    std::unordered_map<std::string, double> scoreSum;
    std::unordered_map<std::string, double> simSum;

    // Iterate over all other users
    for (const auto& other : users) {
        if (other.id == target.id) continue; 

        double sim = pearsonCorrelation(target, other); 
         if (sim <= 0.0) continue; 
         

        // For each movie rated by another user, predict a rating for the target user
        for (const auto& [movieId, rating] : other.ratings) {
            if (!target.hasRated(movieId)) { 
                double predictedRating = predictRating(target, movieId); 
                scoreSum[movieId] += sim * predictedRating; 
                simSum[movieId] += sim; // Sum similarity scores for normalization
            }
        }
    }

    // Normalize the scores and sort by predicted rating
    std::vector<std::pair<std::string, double>> predicted;
    for (const auto& [movieID, score] : scoreSum) {
        predicted.emplace_back(movieID, score / simSum[movieID]);
    }

    std::sort(predicted.begin(), predicted.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    // Collect the top N recommended movies
    std::vector<std::string> recommendations;
    for (int i = 0; i < topN && i < static_cast<int>(predicted.size()); ++i) {
        recommendations.push_back(predicted[i].first);
    }

    return recommendations;
}

double Recommender::predictRating(const User& target, const std::string& movieId) {
    double scoreSum = 0.0;
    double simSum = 0.0;

    // Iterate over all other users
    for (const auto& other : users) {
        if (other.id == target.id) continue; 

        double sim = pearsonCorrelation(target, other); 
        if (sim <= 0.0) continue; 

        auto it = other.ratings.find(movieId); 
        if (it != other.ratings.end()) {
            scoreSum += sim * it->second; 
            simSum += sim; 
        }
    }

    if (simSum == 0) {
        return 0.0; 
    }

    return scoreSum / simSum; 
}
