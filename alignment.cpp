#include <iostream>
#include <string>
#include <tuple>
#include <algorithm>
#include <vector>
#include <chrono>
//#include <Python.h>
//#include <boost/python.hpp>
#include "alignment.h"


std::tuple<std::string, std::string, int> semi_global_alignment(const std::string& seqA, const std::string& seqB, int score_match, int score_mismatch, int gap_penalty) {
    int m = seqA.length();
    int n = seqB.length();

    // Initialize the dynamic programming matrix
    std::vector<std::vector<int>> DP(m + 1, std::vector<int>(n + 1, 0));

    // Fill in the DP matrix
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            int match_mismatch_score = seqA[i - 1] == seqB[j - 1] ? score_match : score_mismatch;
            DP[i][j] = std::max({ DP[i - 1][j - 1] + match_mismatch_score,
                                 DP[i - 1][j] + gap_penalty,
                                 DP[i][j - 1] + (j == n ? 0 : gap_penalty) });  // Modify gap penalty for seqB
        }
    }

    // Find the maximum score in the last row and last column
    int max_score_row = *std::max_element(DP[m].begin(), DP[m].end());
    int max_score_col = DP[0][n]; // Initialize with the first element of the last column

    for (int i = 1; i <= m; i++) {
        max_score_col = std::max(max_score_col, DP[i][n]);
    }

    int max_score = 0;
    int i = 0;
    int j = 0;

    // Determine the starting point for traceback
    if (max_score_row >= max_score_col) {
        max_score = max_score_row;
        i = m;
        j = std::distance(DP[m].begin(), std::max_element(DP[m].begin(), DP[m].end()));
    }
    else {
        max_score = max_score_col;
        for (int k = 0; k <= m; ++k) {
            if (DP[k][n] == max_score_col) {
                i = k;
                j = n;
                break;
            }
        }
    }

    // Traceback to build the aligned sequences
    std::string A_aligned;
    std::string B_aligned;

    // Check for cost-free gaps at the ends
    if (i < m) {
        A_aligned = seqA.substr(i) + A_aligned;
        B_aligned = std::string(m - i, '-') + B_aligned;
    }
    if (j < n) {
        A_aligned = std::string(n - j, '-') + A_aligned;
        B_aligned = seqB.substr(j) + B_aligned;
    }

    while (i > 0 && j > 0) {
        if (DP[i][j] == DP[i - 1][j] + gap_penalty) {
            A_aligned = seqA[i - 1] + A_aligned;
            B_aligned = '-' + B_aligned;
            --i;
        }
        else if (DP[i][j] == DP[i][j - 1] + (j == n ? 0 : gap_penalty)) {  // Modify gap penalty for seqB
            A_aligned = '-' + A_aligned;
            B_aligned = seqB[j - 1] + B_aligned;
            --j;
        }
        else {
            A_aligned = seqA[i - 1] + A_aligned;
            B_aligned = seqB[j - 1] + B_aligned;
            --i;
            --j;
        }
    }

    // Add any remaining characters from seqA or seqB
    while (i > 0) {
        A_aligned = seqA[i - 1] + A_aligned;
        B_aligned = '-' + B_aligned;
        --i;
    }
    while (j > 0) {
        A_aligned = '-' + A_aligned;
        B_aligned = seqB[j - 1] + B_aligned;
        --j;
    }

    return std::make_tuple(A_aligned, B_aligned, max_score);
}

// BOOST_PYTHON_MODULE(alignment_module) {
//     using namespace boost::python;
//     def("semi_global_alignment", semi_global_alignment);
// }

int maini() {
    std::string seqA = "CAGCACTTGGATTCTCGG";
    std::string seqB = "CAGCGTGG";

    int score_match = 2;
    int score_mismatch = -1;
    int gap_penalty = -2;

    auto start = std::chrono::high_resolution_clock::now();

    // Perform semi-global alignment
    std::tuple<std::string, std::string, int> alignment = semi_global_alignment(seqA, seqB, score_match, score_mismatch, gap_penalty);

    auto end = std::chrono::high_resolution_clock::now();

    // Retrieve the aligned sequences and the maximum score
    std::string alignedA = std::get<0>(alignment);
    std::string alignedB = std::get<1>(alignment);
    int max_score = std::get<2>(alignment);

    // Print the results
    std::cout << "Aligned Sequence A: " << alignedA << std::endl;
    std::cout << "Aligned Sequence B: " << alignedB << std::endl;
    std::cout << "Max Score: " << max_score << std::endl;


    // Calculate the duration in milliseconds
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    // Print the execution time
    std::cout << "Execution time: " << duration << " nanoseconds" << std::endl;

    return 0;
}