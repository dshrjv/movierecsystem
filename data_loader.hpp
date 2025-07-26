#pragma once
#include "user.hpp"
#include <vector>
#include <string>

class DataLoader {
    public:
        static std::vector<User> loadUsersFromFile(const std::string& filename);

};