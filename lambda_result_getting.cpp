#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "alignment.h"
#include <chrono>
#include <tuple>
#include <thread>
#include <unordered_map>
#include <algorithm>



// void removeLeadingTrailingSpaces(std::string& str) {
//     // Remove leading spaces
//     str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) {
//         return !std::isspace(ch);
//     }));

//     // Remove trailing spaces
//     str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) {
//         return !std::isspace(ch);
//     }).base(), str.end());
// }

// Function to get the letter with the maximum occurrences
char getMaxOccurringLetter(const std::unordered_map<char, int>& letterCount) {
    char maxLetter = ' ';
    int maxCount = 0;

    for (const auto& pair : letterCount) {
        if (pair.second > maxCount) {
            maxLetter = pair.first;
            maxCount = pair.second;
        }
    }

    return maxLetter;
}

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        auto h1 = std::hash<T1>{}(pair.first);
        auto h2 = std::hash<T2>{}(pair.second);
        return h1 ^ h2;
    }
};

std::pair<char, char> getMaxOccurringPair(const std::unordered_map<std::pair<char, char>, int, pair_hash>& pairCount) {
    std::pair<char, char> maxPair = { ' ', ' ' };
    int maxCount = 0;

    for (const auto& pair : pairCount) {
        if (pair.second > maxCount) {
            maxPair = pair.first;
            maxCount = pair.second;
        }
    }

    return maxPair;
}


double calculateMutationRatio(int location, const std::unordered_map<int, std::vector<std::tuple<int, char, char, char>>>& mutationDictionary,
                              const std::unordered_map<int, int>& numberOfTimesLocationVisited) {
    if (numberOfTimesLocationVisited.find(location) != numberOfTimesLocationVisited.end()) {
        int numMutations = 0;
        int timesVisited = numberOfTimesLocationVisited.at(location);

        if (timesVisited >= 10) {
            if (mutationDictionary.find(location) != mutationDictionary.end()) {
                numMutations = mutationDictionary.at(location).size();
            }

            return static_cast<double>(numMutations) / timesVisited;
        }
    }

    return 0.0;
}

int main() {
    std::ifstream file("lambda_mutations.txt");
    std::string line;

    std::unordered_map<int, std::vector<std::tuple<int, char, char, char>>> mutationDictionary;
    std::unordered_map<int, int> numberOfTimesLocationVisited;

    int currentLocation = -1;

    while (std::getline(file, line)) {
        if (line.find("Location: ") != std::string::npos) {
            // Extract the location from the line
            int location = std::stoi(line.substr(line.find("Location: ") + 10));
            currentLocation = location;

            // Extract the number of times visited from the line
            int visitedPos = line.find("Visited: ");
            int visitedCount = visitedPos != std::string::npos ? std::stoi(line.substr(visitedPos + 9)) : 0;
            numberOfTimesLocationVisited[currentLocation] = visitedCount;
        } else if (line.find("Sequence: ") != std::string::npos) {
            // Extract the sequence number from the line
            int sequenceNumber = std::stoi(line.substr(line.find("Sequence: ") + 10));

            // Extract the mutation type, original letter, and mutated letter from the line
            std::string mutationType = line.substr(line.find("Mutation: ") + 10, 1);
            std::string originalLetter = line.substr(line.find("Original: ") + 10, 1);
            std::string mutatedLetter = line.substr(line.find("Mutated: ") + 9, 1);

            // Add the tuple to the mutationDictionary for the current location
            mutationDictionary[currentLocation].emplace_back(sequenceNumber, mutationType[0], originalLetter[0], mutatedLetter[0]);
        }
    }

    // // Print the contents of mutationDictionary and numberOfTimesLocationVisited
    // for (const auto& entry : numberOfTimesLocationVisited) {
    //     std::cout << "Location: " << entry.first << " Visited: " << entry.second << " time(s)" << std::endl;
    //     if (mutationDictionary.find(entry.first) != mutationDictionary.end()) {
    //         for (const auto& tuple : mutationDictionary[entry.first]) {
    //             std::cout << "Sequence: " << std::get<0>(tuple) << " Mutation: " << std::get<1>(tuple)
    //                       << " Original: " << std::get<2>(tuple) << " Mutated: " << std::get<3>(tuple) << std::endl;
    //         }
    //     }
    // }

    std::vector<int> selectedLocations; // List to store selected locations

    for (int location = 1; location <= 48100; location += 100) {
    std::vector<std::pair<double, int>> locationRatios; // Stores location ratios and corresponding location

    // Calculate and store ratios for eligible locations
    for (int j = location; j <= location + 99; ++j) {
        double ratio = calculateMutationRatio(j, mutationDictionary, numberOfTimesLocationVisited);

        if (ratio >= 0.70) {
            locationRatios.push_back({ ratio, j });
        }
    }

    // Sort the location ratios in descending order
    std::sort(locationRatios.rbegin(), locationRatios.rend());

    // Select the top three different locations
    int numSelected = 0;
    for (const auto& pair : locationRatios) {
        int loc = pair.second;
        if (std::find(selectedLocations.begin(), selectedLocations.end(), loc) == selectedLocations.end()) {
            selectedLocations.push_back(loc);
            ++numSelected;
        }

        if (numSelected == 3) {
            break;
        }
    }
    }

    // // Output the selected locations
    // std::cout << "Selected Locations: ";
    // for (int location : selectedLocations) {
    //     std::cout << location << " ";
    // }
    // std::cout << std::endl;

    
    // Analyze mutationDictionary for each selected location
std::ofstream output_file("lambda_final_result.txt");
for (int location : selectedLocations) {
    const auto& mutations = mutationDictionary[location];

    // Count the number of each mutation type
    int deletionCount = 0;
    int insertionCount = 0;
    int substitutionCount = 0;

    // Count the occurrences of each letter for deletion and insertion mutations
    std::unordered_map<char, int> deletionLetterCount;
    std::unordered_map<char, int> insertionLetterCount;

    // Count the occurrences of each letter pair for substitution mutations
    std::unordered_map<std::pair<char, char>, int, pair_hash> substitutionPairCount;

    // Iterate over the mutations for the current location
    for (const auto& mutation : mutations) {
        char mutationType = std::get<1>(mutation);
        char originalChar = std::get<2>(mutation);
        char mutatedChar = std::get<3>(mutation);

        // Update mutation type counts
        if (mutationType == 'D') {
            deletionCount++;
            deletionLetterCount[originalChar]++;
        } else if (mutationType == 'I') {
            insertionCount++;
            insertionLetterCount[mutatedChar]++;
        } else if (mutationType == 'S') {
            substitutionCount++;
            substitutionPairCount[std::make_pair(originalChar, mutatedChar)]++;
        }
    }

    // Determine the dominant mutation type
    char dominantMutationType;
    if (deletionCount > insertionCount && deletionCount > substitutionCount) {
        dominantMutationType = 'D';
    } else if (insertionCount > deletionCount && insertionCount > substitutionCount) {
        dominantMutationType = 'I';
    } else {
        dominantMutationType = 'S';
    }


    // Output the results based on the dominant mutation type
    output_file << "Location: " << location << " ";
    if (dominantMutationType == 'D') {
        char dominantLetterOfOriginal = getMaxOccurringLetter(deletionLetterCount);
        output_file << "Mutation Type: D deleted Letter: " << dominantLetterOfOriginal << std::endl;
    } else if (dominantMutationType == 'I') {
        char dominantLetterOfSequence = getMaxOccurringLetter(insertionLetterCount);
        output_file << "Mutation Type: I inserted Letter: " << dominantLetterOfSequence << std::endl;
    } else if (dominantMutationType == 'S') {
        std::pair<char, char> dominantPair = getMaxOccurringPair(substitutionPairCount);
        output_file << "Mutation Type: S substitute pair: (" << dominantPair.first << ", " << dominantPair.second << ")" << std::endl;
    }
    //std::cout << "Deletion count: " << deletionCount << ", insertion count: " << insertionCount << ", substitution count: "<< substitutionCount << std::endl;  
}
    output_file.close();
    return 0;
    
}