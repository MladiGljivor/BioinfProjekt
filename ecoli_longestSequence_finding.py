from bisect import bisect_left


def lis(triplets):
    if len(triplets) == 0:  # Boundary case
        return []

    tail = [triplets[0]]  # Initialize the tail with the first triplet
    length = 1  # Length of the longest increasing subsequence

    for i in range(1, len(triplets)):
        if triplets[i].position_sequence > tail[length-1].position_sequence:
            # triplets[i] extends the largest subsequence
            tail.append(triplets[i])
            length += 1
        else:
            # triplets[i] will extend a subsequence and discard older subsequences

            # Find the largest value just smaller than triplets[i].position_sequence in tail
            index = bisect_left(tail, triplets[i].position_sequence, 0, length-1, key=lambda x: x.position_sequence)

            if index == length:
                # triplets[i] is smaller than all elements in tail, so replace the last element
                tail[length-1] = triplets[i]
            else:
                # triplets[i] can replace the appropriate element in tail
                tail[index] = triplets[i]

    return len(tail), tail

class MinimizerTriplet:
    def __init__(self, minimizer, position_original, position_sequence):
        self.minimizer = minimizer
        self.position_original = position_original
        self.position_sequence = int(position_sequence)

    def __str__(self):
        return f"({self.minimizer}, {self.position_original}, {self.position_sequence})"
    

class Used_Minimizer:
    def __init__(self, minimizer, location):
        self.minimizer = minimizer
        self.location = location

    def __str__(self):
        return f"({self.minimizer}, {self.location})"


def main():
    minimizers_sequences_file_path = "ecoli/ecoli-sequences-minimizers.txt"  
    with open(minimizers_sequences_file_path, "r") as file:
        text_sequences = file.read()
    chunks_original = text_sequences.split("---")
    #print(len(chunks_original))


    minimizers_reverseComplement_sequences_file_path = "ecoli/ecoli-sequences-reverseComplement-minimizers.txt"
    with open(minimizers_reverseComplement_sequences_file_path, "r") as file:
        text_reverseComplement_sequences = file.read()
    chunks_reverseComplement = text_reverseComplement_sequences.split("---")
    

    minimizers_file_path = "ecoli/minimizers-ecoli.txt"
    with open(minimizers_file_path, "r") as file:
        text_original_code = file.read().lstrip("{").rstrip("}---") #micem nepotrebne znakove s pocetka i kraja
    
    minimizer_original_dictionary = {}
    reverse_code_values_array = text_original_code.split("], ")
    for i in range(len(reverse_code_values_array)):
            #print(reverse_code_values_array[i])

            minimizer_hash = reverse_code_values_array[i].split("=")[0].strip()
            #print(minimizer_hash)
            if i == len(reverse_code_values_array) - 1:
                values_text = reverse_code_values_array[i].split("=")[1]
            else:
                values_text = reverse_code_values_array[i].split("=")[1] + "]"
            #print(values_text)
            locations_array = values_text[0:len(values_text)-3].lstrip("[").rstrip("]").split(",")
            minimizer_original_dictionary[minimizer_hash] = locations_array
            #print(locations_array)

    
    with open("ecoli-mapping.txt", "w") as file:
        brojac = 0
        print("Tu sam")
        for chunk in chunks_original:

            #print(chunk)
            print(str(brojac) +"\n")

            if brojac == len(chunks_original) or chunk == "" :
                continue
            brojac += 1
            minimizer_values_array = chunk.split("], ")

            #Ovdje prolazimo kroz reversed genom vrijednosti
            for i in range(len(minimizer_values_array)):
                minimizers_triples_list = []
                #Ovdje moramo stvoriti trojke za odredeni minimizer 
                minimizer_hash_new = reverse_code_values_array[i].split("=")[0].strip()
                if i == len(reverse_code_values_array) - 1:
                    values_text = reverse_code_values_array[i].split("=")[1]
                else:
                    values_text = reverse_code_values_array[i].split("=")[1] + "]"
                sequence_locations_array = values_text[0:len(values_text)-3].lstrip("[").rstrip("]").split(",")

                if minimizer_hash_new in minimizer_original_dictionary:
                    original_code_locations_array = minimizer_original_dictionary[minimizer_hash_new]
                    
                    for original_location in original_code_locations_array:
                        if(original_location == ""):
                            continue
                        for sequence_location in sequence_locations_array:
                            if sequence_location == "" or sequence_location == " ":
                                continue
                            minimizers_triples_list.append(MinimizerTriplet(minimizer_hash_new, original_location, int(sequence_location.strip())))
    
                else:
                    continue
            
            minimizers_triples_list.sort(key=lambda obj: obj.position_original)
            if(len(minimizers_triples_list) == 0):
                continue
            max_length_in_original_code, longest_sequence_in_original_code = lis(minimizers_triples_list)
            used_code_text_file = "original"


            
            location = longest_sequence_in_original_code[0].position_original
            string = str(brojac) + "->" + str(location).replace(" ","") + ":" + used_code_text_file
            if brojac == len(chunks_original) - 1:
                file.write(string + ",")
            else:
                file.write(string + ",")

        for chunk in chunks_reverseComplement:

            if brojac == len(chunks_reverseComplement) * 2 or chunk == "" :
                continue
            brojac += 1
            minimizer_values_array = chunk.split("], ")

            #Ovdje prolazimo kroz reversed genom vrijednosti
            for i in range(len(minimizer_values_array)):
                minimizers_triples_list = []
                #Ovdje moramo stvoriti trojke za odredeni minimizer 
                minimizer_hash_new = reverse_code_values_array[i].split("=")[0].strip()
                if i == len(reverse_code_values_array) - 1:
                    values_text = reverse_code_values_array[i].split("=")[1]
                else:
                    values_text = reverse_code_values_array[i].split("=")[1] + "]"
                sequence_locations_array = values_text[0:len(values_text)-3].lstrip("[").rstrip("]").split(",")

                if minimizer_hash_new in minimizer_original_dictionary:
                    original_code_locations_array = minimizer_original_dictionary[minimizer_hash_new]
                    
                    for original_location in original_code_locations_array:
                        if(original_location == ""):
                            continue
                        for sequence_location in sequence_locations_array:
                            if sequence_location == "" or sequence_location == " ":
                                continue
                            minimizers_triples_list.append(MinimizerTriplet(minimizer_hash_new, original_location, int(sequence_location.strip())))
    
                else:
                    continue
            
            minimizers_triples_list.sort(key=lambda obj: obj.position_original)
            if(len(minimizers_triples_list) == 0):
                continue
            max_length_in_reversedComplement_code, longest_sequence_in_reverseComplement_code = lis(minimizers_triples_list)
            used_code_text_file = "reversedComplement"

            printani = ", ".join(map(str, longest_sequence_in_original_code))
            print(printani)
            
            location = longest_sequence_in_reverseComplement_code[0].position_original
            string = str(brojac) + "->" + str(location).replace(" ","") + ":" + used_code_text_file
            if brojac == len(chunks_reverseComplement) * 2 - 2:
                file.write(string)
            else:
                file.write(string + ",")
            

if __name__ == "__main__":
    main()