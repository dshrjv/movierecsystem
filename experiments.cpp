#include "experiments.hpp"
#include <iostream>
#include <algorithm>
#include <random>
#include <map>
#include <iomanip>
#include <set>

void Experiments::runTrainTestSplit(std::vector<User>& users, double testRatio) {
    std::cout << "Running train-test split." << std::endl;
    std::vector<std::pair<std::string, double>> testSet;
    std::map<std::string, std::unordered_map<std::string, double>> originalRatings;

    for (User& user : users) {
        originalRatings[user.id] = user.ratings;

        std::vector<std::string> movieIds;
        for (const auto& pair : user.ratings) {
            movieIds.push_back(pair.first);

        }

        std::shuffle(movieIds.begin(), movieIds.end(), std::mt19937{std::random_device{}()});
        int numTest = static_cast<int>(testRatio * movieIds.size());

        for (int i = 0; i < numTest; ++i) {
            std::string movieId = movieIds[i];
            testSet.emplace_back(user.id + ":" + movieId, user.getRating(movieId));
            user.ratings.erase(movieId);
        }
    }

    Recommender recommender(users);
    double totalError = 0.0;
    int count = 0;

    for (const auto& entry : testSet) {
        size_t delim = entry.first.find(':');
        std::string userId = entry.first.substr(0, delim);
        std::string movieId = entry.first.substr(delim + 1);

        User* user = recommender.getUserById(userId);
        if (!user) continue;

        double actual = entry.second;
        double predicted = recommender.predictRating(*user, movieId);
        double error = std::abs(predicted - actual);
        totalError += error;
        ++count;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << std::left << std::setw(8)  << "User:"
          << std::setw(5)  << userId
          << std::setw(10) << "| Movie:"
          << std::setw(25) << movieId
          << std::setw(10) << "| Actual:"
          << std::setw(6)  << actual
          << std::setw(12) << "| Predicted: "
          << std::setw(6)  << predicted
          << std::setw(8)  << "| Error: "
          << std::setw(6)  << std::abs(error)
          << std::endl;
    }

    if (count > 0) {
        std::cout << "Average absolute error on test set: " << (totalError / count) << "\n";
    } else {
        std::cout << "No predictions made" << "\n";
    }

    for (User& user : users) {
        user.ratings = originalRatings[user.id];
    }

}

void Experiments::runLOOCV(const std::vector<User>& users) {
    double totalError = 0.0;
    int count = 0;
    std::cout << "Running Leave-One-Out Cross-Validation (LOOCV)." << std::endl;

    for (const auto& user : users) {
        for (const auto& pair : user.ratings) {
            std::string movieId = pair.first;
            double actual = pair.second;

            // Create a copy of the user without the current movie rating
            std::vector<User> usersCopy = users;
            for (auto& u : usersCopy) {
                if (u.id == user.id) {
                    u.ratings.erase(movieId);
                    break;
                }
            }

            Recommender recommender(usersCopy);
            const User* targetUser = recommender.getUserById(user.id);
            if (!targetUser) continue;
            
            double predicted = recommender.predictRating(*targetUser, movieId);
            double error = std::abs(predicted - actual);

            std::cout << "User: " << std::setw(4) << user.id
            << "   | Movie:  " << std::setw(25) << std::left << movieId
            << " | Actual: " << std::setw(5) << std::fixed << std::setprecision(2) << actual
            << " | Predicted: " << std::setw(5) << predicted
            << " | Error: " << std::setw(5) << error << "\n";
            totalError += error;
            ++count;
        }
    }
    double avgError = (count == 0) ? 0.0 : totalError / count;
    std::cout << "\nAverage absolute error (LOOCV): " 
              << std::fixed << std::setprecision(5) << avgError << "\n";
}