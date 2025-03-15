//
// Created by Oliver Fogelin on 15/03/2025.
//

#include "env.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>

class UndefinedEnvVarException : public std::runtime_error
{
public:
    explicit UndefinedEnvVarException(const std::string& var_name): runtime_error(
        "Environment variable " + var_name + " not set"
    )
    {
    }
};

std::string safe_getenv(const std::string& name)
{
    const char* var = std::getenv(name.data());
    if (var == nullptr)
    {
        throw UndefinedEnvVarException(name);
    }
    return var;
}
