#include <tuple>
#include <string>

#ifndef ALIGNMENT_H
#define ALIGNMENT_H

std::tuple<std::string, std::string, int> semi_global_alignment(const std::string& seqA, const std::string& seqB, int score_match, int score_mismatch, int gap_penalty);

#endif