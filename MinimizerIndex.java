import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;

public class MinimizerIndex {
    public HashMap<String, List<Integer>> index; // hash table for minimizers
    private int windowSize; // size of the sliding window
    private int k; // size of k-mers


    private String referenceGenome;

    public MinimizerIndex(int windowSize, int k,String referenceGenome) {


        this.index = new HashMap<>();
        this.windowSize = windowSize;
        this.k = k;
        this.referenceGenome=referenceGenome;


        indexReferenceGenome(referenceGenome);

    }

    public void indexReferenceGenome(String referenceGenome) {
        for (int i = 0; i <= referenceGenome.length() - windowSize; i++) {
            String window = referenceGenome.substring(i, i + windowSize);
            String minimizer = "";
            for(int m =0;m<k;m++) {
                minimizer+="T";

            }

            // iterate over all k-mers in the window to find the minimum
            for (int j = 0; j <= windowSize - k; j++) {
                String kmer = window.substring(j, j + k);



                if (kmer.compareTo(minimizer)<0) {
                    minimizer = kmer;
                }
                //if(hash==0)
                   // System.out.println(kmer);
            }

            // add the minimizer to the hash table
            if (!index.containsKey(minimizer)) {
                index.put(minimizer, new ArrayList<>());
            }
            index.get(minimizer).add(i);

        }
    }

    private long kmerToHash(String kmer) {
        long hash = 0;
        for (int i = 0; i < kmer.length(); i++) {
            char c = kmer.charAt(i);
            long val = 0;
            if (c == 'A') {
                val = 0;
            } else if (c == 'C') {
                val = 1;
            } else if (c == 'G') {
                val = 2;
            } else if (c == 'T') {
                val = 3;
            }
            hash = (hash << 2) | val;
        }
        return hash;
    }

}
