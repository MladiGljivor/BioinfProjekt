# Locating mutations with DNA sequences and minimizers 

In this project we are having the reference DNA code and a lot of sequences that are chunks of mutated DNA code. We are using MinimizerGenerator.java 
and MinimizerIndex.java to find minimizers for the reference code, for the sequences code and minimizers of the reverse complement of the sequences(we are 
doing this because we do not know wether the sequence has been read from one DNA chain or the other. For every minimizer we are mapping his location in the code.
Then, when we have all the minimizers, we use the longest increasing subsequence algorithm (for every sequence) implemented in time complexity of O(n * logn) to find
the most probable start location for the beggining of that sequence in reference code(we mapped it in txt file x-mapping.txt in this way:
        number_of_sequence -> startLocation, [original or reversedComplement] 
Original or reverse complement tells us if that sequence is original sequence or reversedComplement of that seqeunce. Then for every mapping we have here, 
In x_mapping.cpp we do semi-global alignment with scores (match: +2, substitute: -1, indel: -2) and from doing alignment for every seqeunce on normal and
reverse-complement code we look which of these two has the higher result and from that better alignment we write every mutation we found in txt file: x_mutations.txt.
For alignment we use the code in alignment.cpp file.
And then for the last code file, we analyze all locations in genome, we take the ratio of:
      how much times we found the mutation on that location / how much times we visited that location during the process of alignment
In the end, we take as the mutations all the locations that have this ratio higher than certain percentage and locations that have been visited a certain ammount 
of times. We finally write all the mutations in x_final_result.txt. 
