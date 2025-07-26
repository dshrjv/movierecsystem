#pragma once

#include <vector>
#include "user.hpp"
#include "recommender.hpp"

class Experiments {
    public:
        // Run Train-Test Split
        void runTrainTestSplit(std::vector<User>& users, double testRatio);

        // Run Leave-One-Out Cross-Validation (LOOCV)
        void runLOOCV(const std::vector<User>& users);
};
