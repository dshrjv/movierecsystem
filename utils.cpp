#include "utils.hpp"
#include <cmath>
#include <vector>

double pearsonCorrelation(const User& a, const User& b) {
    std::vector<std::string> commonMovies;
    
    for (const auto& [movie, rating] : a.ratings) {
        if (b.hasRated(movie)) {
            commonMovies.push_back(movie);
        }
    }

    if (commonMovies.empty()) {
        return 0.0;
    }

    double sumA = 0.0, sumB = 0.0;
    for (const auto& movie : commonMovies) {
        sumA += a.getRating(movie);
        sumB += b.getRating(movie);
    }

    double meanA = sumA / commonMovies.size();
    double meanB = sumB / commonMovies.size();

    double numerator = 0.0, denomA = 0.0, denomB = 0.0;
    for (const auto& movie : commonMovies) {
        double diffA = a.getRating(movie) - meanA;
        double diffB = b.getRating(movie) - meanB;
        numerator += diffA * diffB;
        denomA += diffA * diffA;
        denomB += diffB * diffB;
    }

    if (denomA == 0.0 || denomB == 0.0) {
        return 0.0;
    }

    return numerator / (std::sqrt(denomA) * std::sqrt(denomB));
}