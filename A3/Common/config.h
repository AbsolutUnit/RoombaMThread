#pragma once

#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <functional>
#include <vector>
#include <fstream>
#include <regex>

const int ARGNOTFOUND = -1;
const std::string CONFIGFILEPATH = "config.ini";
const std::vector<std::string> CONFIGPARAMS = {"CHARGINGCONSTANT", "DEFAULT_NUM_THREADS"};

inline std::unordered_map<std::string, int> readConfigFile(const std::string &configPath) {
    auto isNumber = [](std::string &s) {
    s.erase(std::remove_if(s.begin(), s.end(), isspace), s.end());
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
    };

    std::ifstream inf{ configPath };
    std::unordered_map<std::string, int>  values;
    // File could not be opened
    if (!inf) {
        return values;
    }
  
    std::vector<std::string> tokens;
    std::string input;
    
    // Tokenize file 
    std::getline(inf, input);
    if (input[input.size() - 1] == '\r') // Remove carriage returns
        input.erase(input.size() - 1);
    while (inf) { // While buffer is not EOF
        tokens.push_back(std::regex_replace(input, std::regex("="), " = ")); //replace all '=' with " = " for easy delimitation
        // tokens.push_back(input);
        std::getline(inf, input);
        if (input[input.size() - 1] == '\r')
            input.erase(input.size() - 1);
    }

    std::string s;
    std::vector<std::string> line_info;
    for (int i = 0; i < tokens.size(); ++i) {
        std::stringstream ss(tokens[i]);
        while (std::getline(ss, s, '=')) {
            s.erase(std::remove_if(s.begin(), s.end(), isspace), s.end());
            line_info.push_back(s); // Tokenize line by '='
        }
        
        // check if the current line is equal to each parameter
        for (int j = 0; j < CONFIGPARAMS.size(); ++j) {
            if (line_info[0] == CONFIGPARAMS[j] && isNumber(line_info[1])) { 
                values[CONFIGPARAMS[j]] = stoi(line_info[1]);
            }
        }
        line_info.clear();
    }
    return values;
}