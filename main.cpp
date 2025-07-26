#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "recommender.hpp"
#include "data_loader.hpp"
#include <map>
#include "experiments.hpp"

int main() {
    DataLoader loader;
    std::string filename = "small_ratings.csv";
    std::vector<User> users = loader.loadUsersFromFile(filename);  // Load users from file

    Recommender recommender(users); 
    int topN = 5;
    std::string input;
    Experiments experiments;

   while (true) {
        std::cout << "\n===== Movie Recommender System =====\n";
        std::cout << "1. Run Train-Test Split\n";
        std::cout << "2. Run Leave-One-Out Cross-Validation (LOOCV)\n";
        std::cout << "3. Create New User and Get Recommendations\n";
        std::cout << "4. Get Recommendations for Existing User\n";
        std::cout << "5. Exit\n";
        std::cout << "Select an option (1-5): ";

        int choice;
        std::cin >> choice;

        if (choice == 5) {
            std::cout << "Exiting... Goodbye!\n";
            break;
        }

        switch (choice) {
            case 1:
                experiments.runTrainTestSplit(users, 0.2);
                break;
            case 2:
                experiments.runLOOCV(users);
                break;
            case 3: {
                std::cout << "\nCreating new user...\n";
                std::string newUserId = std::to_string(users.size() + 1);
                std::cout << "Created a new user with ID: " << newUserId << "\n";
                User newUser(newUserId);

                std::vector<std::string> movies = {"The Matrix", "Inception", "Titanic", "The Godfather", "Interstellar"};
                std::cout << "\nPlease rate the following movies (1-5):\n";

                for (const auto& movie : movies) {
                    double rating;
                    std::cout << movie << ": ";
                    std::cin >> rating;
                    newUser.addRating(movie, rating);
                }

                users.push_back(newUser);
                recommender = Recommender(users); // Update recommender with new user

                std::vector<std::string> recs = recommender.recommendMovies(newUser, topN);
                std::cout << "\nRecommendations for New User " << newUserId << ":\n";
                int rank = 1;
                std::ofstream outFile("recommendations.txt");
                outFile << "Top " << topN << " recommendations for new user " << newUserId << ":\n";

                for (const auto& movieId : recs) {
                    double pred = recommender.predictRating(newUser, movieId);
                    std::cout << rank << ". " << movieId << " | Predicted Rating: " << pred << std::endl;
                    outFile << rank++ << ". " << movieId << " | Predicted Rating: " << pred << std::endl;
                }

                outFile.close();
                std::cout << "Recommendations saved to recommendations.txt\n";
                break;
            }
            case 4: {
                std::cout << "\nEnter a user ID from the dataset (1 to " << users.size() << "): ";
                std::cin >> input;

                User* user = recommender.getUserById(input);
                if (!user) {
                    std::cout << "User not found.\n";
                    break;
                }

                std::vector<std::string> recs = recommender.recommendMovies(*user, topN);
                std::cout << "\nRecommendations for User " << input << ":\n";
                int rank = 1;
                std::ofstream outFile("recommendations.txt");
                outFile << "Top " << topN << " recommendations for user " << input << ":\n";

                for (const auto& movieId : recs) {
                    double pred = recommender.predictRating(*user, movieId);
                    std::cout << rank << ". " << movieId << " | Predicted Rating: " << pred << std::endl;
                    outFile << rank++ << ". " << movieId << " | Predicted Rating: " << pred << std::endl;
                }

                outFile.close();
                std::cout << "Recommendations saved to recommendations.txt\n";
                break;
            }
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }

   }
    return 0;
}
