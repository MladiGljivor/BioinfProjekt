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

using namespace std;

#include <fstream>

// void saveMapsToFile(const std::unordered_map<int, std::vector<std::tuple<int, char, char, char>>>& mutationDictionary,
//                     const std::unordered_map<int, int>& numberOfTimesLocationVisited,
//                     const std::string& filename) {
//     std::ofstream file(filename);

//     if (file.is_open()) {
//         // Save mutationDictionary
//         for (const auto& entry : mutationDictionary) {
//             int location = entry.first;
//             const auto& mutations = entry.second;

//             file << "Location: " << location << std::endl;
//             for (const auto& mutation : mutations) {
//                 int sequenceNum = std::get<0>(mutation);
//                 char originalChar = std::get<1>(mutation);
//                 char mutatedChar = std::get<2>(mutation);
//                 char mutationType = std::get<3>(mutation);

//                 file << "Sequence: " << sequenceNum << " Mutation: " << mutationType << " Original: " << originalChar << " Mutated: " << mutatedChar << std::endl;
//             }
//         }

//         // Save numberOfTimesLocationVisited
//         for (const auto& entry : numberOfTimesLocationVisited) {
//             int location = entry.first;
//             int count = entry.second;

//             file << "Location: " << location << " Count: " << count << std::endl;
//         }

//         file.close();
//     }
// }

// Dictionary to store mutations

std::unordered_map<int, std::vector<std::tuple<int, char, char, char>>> mutationDictionary;
std::unordered_map<int, int> numberOfTimesLocationVisited;

// Function to perform string comparison and mutation detection
void compareSequences(const std::string& str1, const std::string& str2, int startLocation, int compareLength, int sequenceNumber) {
    // Count the number of "-" signs at the beginning and end of str2
    int beginningGaps1 = 0;
    int endingGaps1 = 0;

    int beginningGaps2 = 0;
    int endingGaps2 = 0;

    char firstInOriginal;

    for (char c : str2) {
        if (c == '-')
            beginningGaps1++;
        else
            break;
    }

    for (int i = str2.length() - 1; i >= 0; i--) {
        if (str2[i] == '-')
            endingGaps1++;
        else
            break;
    }

    for (char c : str1) {
        if (c == '-')
            beginningGaps2++;
        else
            firstInOriginal = c;
            break;
    }

    for (int i = str1.length() - 1; i >= 0; i--) {
        if (str2[i] == '-')
            endingGaps2++;
        else
            break;
    }

    int begginingGaps = 0;
    int endingGaps = 0;

    if(beginningGaps1 >= beginningGaps2){
        begginingGaps = beginningGaps1;
    }
    else {
        begginingGaps = beginningGaps2;
    }

    if(endingGaps1 >= endingGaps2){
        endingGaps = endingGaps1;
    }
    else {
        endingGaps = endingGaps2;
    }

    // Adjust the compare length based on the gaps
    int adjustedCompareLength = compareLength - endingGaps;

    bool firstMatch = false;
    // Perform string comparison and mutation detection
    for (int i = 0 + begginingGaps; i < adjustedCompareLength; i++) {
        int index1 = i;
        int index2 = i;

        // Check for out-of-bounds indices
        if (index1 >= str1.length() || index2 >= str2.length())
            break;

        char char1 = str1[index1];
        char char2 = str2[index2];

         if (char1 != char2) {


            numberOfTimesLocationVisited[startLocation]++;
            startLocation++;

            int location = startLocation;
            char mutationType;
            char originalChar;
            char mutatedChar;

            if (char1 == '-') {
                // Insertion mutation
                mutationType = 'I';
                originalChar = char1; // Save character from string 1
                mutatedChar = char2; // Save character from string 2
            } else if (char2 == '-') {
                // Deletion mutation
                mutationType = 'D';
                originalChar = char1; // Save character from string 1
                mutatedChar = char2; // Save character from string 2
            } else {
                // Substitution mutation
                mutationType = 'S';
                originalChar = char1; // Save character from string 1
                mutatedChar = char2; // Save character from string 2
            }

            // Check if the location already exists in the dictionary
            if (mutationDictionary.find(location) != mutationDictionary.end()) {
                // Location exists, append the new mutation
                mutationDictionary[location].push_back({ sequenceNumber, originalChar, mutatedChar, mutationType });
            } else {
                // Location does not exist, create a new entry in the dictionary
                mutationDictionary[location] = { { sequenceNumber, originalChar, mutatedChar, mutationType} };
            }
        }

        else{
            numberOfTimesLocationVisited[startLocation]++;
            startLocation++;
        }
    }
}

int main() {
    string lambda_path = "lambda/lambda.txt";
    ifstream lambda_file(lambda_path);
    string lambda_content((istreambuf_iterator<char>(lambda_file)), (istreambuf_iterator<char>()));
    lambda_file.close();

    string lambdaMapping_path = "lambda-mapping.txt";
    ifstream lambdaMapping_file(lambdaMapping_path);
    string lambdaMapping_content((istreambuf_iterator<char>(lambdaMapping_file)), (istreambuf_iterator<char>()));
    lambdaMapping_file.close();

    unordered_map<string, string> lambdaMappingDictionary;
    vector<string> mappingArray;
    size_t pos = 0;
    string delimiter = ",";
    while ((pos = lambdaMapping_content.find(delimiter)) != string::npos) {
        string mapPair = lambdaMapping_content.substr(0, pos);
        mappingArray.push_back(mapPair);
        lambdaMapping_content.erase(0, pos + delimiter.length());
    }
    for (const string& mapPair : mappingArray) {
        size_t delimiter_pos = mapPair.find("->");
        string key = mapPair.substr(0, delimiter_pos);
        string value = mapPair.substr(delimiter_pos + 2);
        lambdaMappingDictionary[key] = value;
    }

    unordered_map<int, string> lambda_sequence_dict;
    int ordinal_number = 0;
    string sequence_text;
    string lambda_sequence_file_path = "lambda/lambda_simulated_reads.fasta";
    ifstream lambda_sequence_file(lambda_sequence_file_path);
    string line;
    while (getline(lambda_sequence_file, line)) {
        if (line[0] == '>') {
            if (!sequence_text.empty()) {
                lambda_sequence_dict[ordinal_number] = sequence_text;
                sequence_text.clear();
            }
            ordinal_number++;
        }
        else {
            sequence_text += line;
        }
    }
    lambda_sequence_file.close();
    if (!sequence_text.empty()) {
        lambda_sequence_dict[ordinal_number] = sequence_text;
    }



    ofstream output_file("lambda_mapping_result.txt");
    for (int i = 1; i < 248; i++) {
        string mappingInfoOriginal = lambdaMappingDictionary[to_string(i)];
        string original_or_complement = mappingInfoOriginal.substr(mappingInfoOriginal.find(":") + 1);
        string startLocationOriginal = mappingInfoOriginal.substr(0, mappingInfoOriginal.find(":"));
        string sequence = lambda_sequence_dict[i];
        int sequenceLength = sequence.length();
        //output_file << "Sequence " << i << ", is of length " << sequenceLength << ", mapped to " << original_or_complement << ", on beginning location of: " << startLocation << "\n";
        //cout << "Sequence " << i << ", is of length " << sequenceLength << ", mapped to " << original_or_complement << ", on beginning location of: " << startLocation << "\n";

        string substring_original = lambda_content.substr(stoi(startLocationOriginal), sequenceLength);
        auto start_time = chrono::steady_clock::now();
        tuple<string, string, int> original_alignment_result = semi_global_alignment(substring_original, sequence, 2, -1, -2);
        auto end_time = chrono::steady_clock::now();
        double execution_time = chrono::duration<double>(end_time - start_time).count();
        //cout << "Execution time 1 is: " << execution_time << endl;

        string mappingInfoComplement = lambdaMappingDictionary[to_string(i + 247)];
        original_or_complement = mappingInfoComplement.substr(mappingInfoComplement.find(":") + 1);
        string startLocationComplement = mappingInfoComplement.substr(0, mappingInfoComplement.find(":"));
        if(i == 247){
            original_or_complement = "reversedComplement";
            startLocationComplement = "12933";
        }
        sequence = lambda_sequence_dict[i];
        sequenceLength = sequence.length();

        string A_1 = std::get<0>(original_alignment_result); 
        string B_1 = std::get<1>(original_alignment_result);

        int size = A_1.size();
        int size2 = B_1.size();


        string substring_complement = lambda_content.substr(stoi(startLocationComplement) , sequenceLength );
        start_time = chrono::steady_clock::now();
        tuple<string, string, int> complement_alignment_result = semi_global_alignment(substring_complement, sequence, 2, -1, -2);
        end_time = chrono::steady_clock::now();
        execution_time = chrono::duration<double>(end_time - start_time).count();
        //cout << "Execution time 2 is: " << execution_time << endl;

        int firstScore = std::get<2>(original_alignment_result); 
        int secondScore = std::get<2>(complement_alignment_result);

        string A_2 = std::get<0>(complement_alignment_result); 
        string B_2 = std::get<1>(complement_alignment_result);

        cout << "Alignment result for original is : " << firstScore << ", and for complement is : " << secondScore << "\n";
        cout << "Sequence number is : " << i << endl;

        string resultOriginal,resultSequence;
        int startLocation;


        if(firstScore >= secondScore) {
            resultOriginal = A_1;
            resultSequence = B_1;
            original_or_complement = "Original";
            startLocation = std::stoi(startLocationOriginal);
        }
        else{
            resultOriginal = A_2;
            resultSequence = B_2;
            original_or_complement = "Complement";
            startLocation = std::stoi(startLocationComplement);
        }
        int compareLength = resultOriginal.size();

        output_file << "Sequence " << i << ", is of length " << sequenceLength << ", mapped to " << original_or_complement << ", on beginning location of: " << startLocation << "\n";

        compareSequences(resultOriginal, resultSequence, startLocation, compareLength, i);

    }
    output_file.close();

    ofstream output_file_result("lambda_mutations.txt");

    for (const auto& entry : mutationDictionary) {
        int location = entry.first;
        const auto& mutations = entry.second;

        output_file_result << "Location: " << location;
        
        // Check if the location is in numberOfTimesLocationVisited
        if (numberOfTimesLocationVisited.count(location) > 0) {
            int count = numberOfTimesLocationVisited[location];
            output_file_result << " Visited: " << count << " time(s)";
        }
        
        output_file_result << std::endl;

        for (const auto& mutation : mutations) {
            int sequenceNum = std::get<0>(mutation);
            char originalChar = std::get<1>(mutation);
            char mutatedChar = std::get<2>(mutation);
            char mutationType = std::get<3>(mutation);

            output_file_result << "Sequence: " << sequenceNum << " Mutation: " << mutationType << " Original: " << originalChar << " Mutated: " << mutatedChar << std::endl;
        }
    }

    output_file_result.close();

    //saveMapsToFile(mutationDictionary, numberOfTimesLocationVisited, "stored_maps.txt");


    return 0;
}