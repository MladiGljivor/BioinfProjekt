import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.*;

public class Main {
    static class Match {
        String minimizer;
        int pos1;
        int pos2;

        Match(String minimizer, int pos1, int pos2) {
            this.minimizer = minimizer;
            this.pos1 = pos1;
            this.pos2 = pos2;
        }

        @Override
        public String toString() {
            return "(" + minimizer + ", " + pos1 + ", " + pos2 + ")";
        }
    }
    public static void main(String[] args) {
        //BigFileReader readerLambda = new BigFileReader("Bioinfo_19_20_train_data/lambda.fasta");
        //BigFileReader readerEcoli = new BigFileReader("Bioinfo_19_20_train_data/ecoli.fasta");
        //System.out.println(readerLambda.tekst.length());
        //System.out.println(readerLambda.genComplement().length());
        //System.out.println(readerEcoli.tekst.length());
        //System.out.println(readerEcoli.genComplement().length());
        //System.out.println("AAAA".compareTo("GTTT"));
       // System.out.println("Gotovo citanje");
        try{
           // write("lambda.txt",readerLambda.tekst);
           // write("ecoliComplement.txt",ecoliComplement);
           // write("lambdaReverse.txt",lambdaReverse);
            //write("ecoliReverse.txt",ecoliReverse);

            //System.out.println("Upisani minimizeri za genome");
        }
        catch(Exception e){

        }

       // MinimizerIndex minimizerLamda = new MinimizerIndex(20,15,readerLambda.tekst);
        System.out.println("ejj");
        MinimizerGenerator mgLambda = new MinimizerGenerator(20, 15,"Bioinfo_19_20_train_data/ecoli_simulated_reads.fasta","ecoli-sequences-minimizers.txt"); // use a window size of 20 and k-mer size of 5
        //System.out.println(mgLambda.minimizersList);
       // System.out.println(mgLambda.minimizersList.get(0).toString());
        //System.out.println("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
        //System.out.println(mgLambda.minimizersList.get(1).toString());
        // MinimizerIndex minimizerEcoli = new MinimizerIndex(40,5,ecoliComplement);
        //MinimizerIndex minimizerLamdaReverse = new MinimizerIndex(40,5,lambdaReverse);
        //MinimizerIndex minimizerEcoliReverse = new MinimizerIndex(40,5,ecoliReverse);
        //System.out.println("Stvoreni minimizeri za genome");

        try{
            //write("minimizers-lambda.txt",minimizerLamda.index.toString());

           // write("minimizers-ecoli-complement.txt",minimizerEcoli.index.toString());
            //write("minimizers-lambda-reverse.txt",minimizerLamdaReverse.index.toString());
            //write("minimizers-ecoli-reverse.txt",minimizerEcoliReverse.index.toString());

            System.out.println("Upisani minimizeri za genome");
        }
        catch(Exception e){

        }
        //System.out.println(readerLambda.tekst.substring(0,20));
        //System.out.println(minimizerLamda.index.toString().substring(0,1000));
        //System.out.println(readerLambda.tekst.substring(26,36));
       // System.out.println("Upisani minimizeri sekvenca za Lambdu");
        //MinimizerGenerator mgEcoli = new MinimizerGenerator(40, 5,"Bioinfo_19_20_train_data/ecoli_simulated_reads.fasta","ecoli-sequences-minimizers.txt");
        //System.out.println("Upisani minizeri sekvenca za Ecoli");
        //Mapper map = new Mapper(readerLambda.tekst,mg.tekst.get(0),m.index,mg.minimizersList.get(0));



    }
    public static void write(String fileName, String content)
             { try{
        File file = new File(fileName);
        FileWriter fr = new FileWriter(file, true);
        fr.write(content);
        fr.write("---");
        fr.close(); }
             catch (Exception e) {

             }
    }
    public static List<Match> getMatches(HashMap<String, List<Integer>> map1, HashMap<String, List<Integer>> map2) {
        //System.out.println(map1.keySet());
        //System.out.println(map2.keySet());
        Set<String> commonKeys = new HashSet<>(map1.keySet());
        commonKeys.retainAll(map2.keySet());
        //System.out.println("coomonkeys "+commonKeys);

        List<Match> matches = new ArrayList<>();
        for (String key : commonKeys) {
            for (int pos1 : map1.get(key)) {
                for (int pos2 : map2.get(key)) {
                    matches.add(new Match(key, pos1, pos2));
                }
            }
        }

        return matches;
    }

    public static int[] longestIncreasingSubsequence(List<Match> matches) {
        int n = matches.size();
        int[] tailIndices = new int[n];
        int[] prevIndices = new int[n];

        int length = 1;
        tailIndices[0] = 0;
        prevIndices[0] = -1;

        for (int i = 1; i < n; i++) {
            if (matches.get(i).pos2 < matches.get(tailIndices[0]).pos2) {
                tailIndices[0] = i;
            } else if (matches.get(i).pos2 > matches.get(tailIndices[length - 1]).pos2) {
                prevIndices[i] = tailIndices[length - 1];
                tailIndices[length++] = i;
            } else {
                int l = 0, r = length-1;
                while (l < r) {
                    int m = l + (r - l) / 2;
                    if (matches.get(tailIndices[m]).pos2 < matches.get(i).pos2)
                        l = m + 1;
                    else
                        r = m;
                }
                prevIndices[i] = (l > 0) ? tailIndices[l - 1] : -1;
                tailIndices[l] = i;
            }
        }

        int[] longestSubsequence = new int[length];
        for (int i = tailIndices[length - 1], j = length-1; i >= 0; i = prevIndices[i], j--) {
            longestSubsequence[j] = i;
        }

        return longestSubsequence;
    }
}
