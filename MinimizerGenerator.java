import java.io.*;

import java.util.*;

public class MinimizerGenerator {

    private int windowSize; // size of the sliding window
    private int k; // size of k-mers

    public String tekst;

    String outFile;

    public List<HashMap<String, List<Integer>>> minimizersList;
    public List<HashMap<String, List<Integer>>> minimizersListRC;

public    String[] sekvence;
  public  String[] RCsekvence;

    public MinimizerGenerator(int windowSize, int k,String filename,String outFile)  {
        tekst= "";
        this.windowSize = windowSize;
        this.k = k;
        this.outFile=outFile;
        minimizersList = new ArrayList<>();
        minimizersListRC=new ArrayList<>();
       minimizersList=generateMinimizers(filename);
      //  minimizersListRC=generateMinimizersRC(filename);
        System.out.println();



    }

    private List<HashMap<String, List<Integer>>> generateMinimizersRC(String filename) {
        System.out.println("uso");
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            String line;

            StringBuilder builder = new StringBuilder();
            while ((line = reader.readLine()) != null) {
                if (!line.startsWith(">")) {
                    builder.append(line);
                } else {
                    builder.append(",");
                }
              //  System.out.println(line);
            }
            tekst = builder.toString();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        //System.out.println(tekst.length());
        String[] sve = tekst.split(",");
        sekvence=new String[sve.length];
        for(int i =1;i<sve.length;i++) {
            sekvence[i-1]=sve[i];
        }
        //System.out.println(sve.length);
        List<HashMap<String, List<Integer>>> lista = new ArrayList<>();

        List<Integer> positions = new ArrayList<>(); // list of positions for a given minimizer
        sve=getReverseComplements(sve);
        System.out.println("proslo");
        RCsekvence=new String[sve.length];
        for(int i =1;i<sve.length;i++) {
            RCsekvence[i-1]=sve[i];
        }


        for(int i = 1; i < sve.length; i++) {
            System.out.println(i);

            HashMap<String, List<Integer>> index = new HashMap<>(); // hash table for minimizers

            for (int j = 0; j <= sve[i].length() - windowSize; j++) {
                String window = sve[i].substring(j, j + windowSize);
                String minimizer = "";
                for(int m =0;m<k;m++) {
                    minimizer+="T";

                }


                for (int z = 0; z <= windowSize - k; z++) {
                    String kmer = window.substring(z, z + k);


                    if (kmer.compareTo(minimizer)<0) {
                        minimizer = kmer;
                    }
                }

                // add the position to the list for the given minimizer
                positions = index.get(minimizer);
                if (positions == null) {
                    positions = new ArrayList<>();
                    index.put(minimizer, positions);
                }
                positions.add(j);
            }

           // lista.add(index);
            // add the map to the list
            try {
                //write(outFile, index.toString());
            }
            catch (Exception e) {

            }
        }

        return lista;

    }

    public void write(String fileName,String content)
            throws IOException {
        File file = new File(fileName);
        FileWriter fr = new FileWriter(file, true);
        fr.write(content);
        fr.write("---");
        fr.close();
    }

    public List<HashMap<String, List<Integer>>> generateMinimizers(String filename) {




        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            String line;

            StringBuilder builder = new StringBuilder();
            while ((line = reader.readLine()) != null) {
                if (!line.startsWith(">")) {
                    builder.append(line);
                } else {
                    builder.append(",");
                }
               // System.out.println(line);
            }
            tekst = builder.toString();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
       // System.out.println(tekst.length());
        String[] sve = tekst.split(",");
        //System.out.println(sve.length);
        List<HashMap<String, List<Integer>>> lista = new ArrayList<>();

        List<Integer> positions = new ArrayList<>(); // list of positions for a given minimizer
        System.out.println();


        for(int i = 1; i < sve.length; i++) {

            System.out.println(i);
            HashMap<String, List<Integer>> index = new HashMap<>(); // hash table for minimizers

            for (int j = 0; j <= sve[i].length() - windowSize; j++) {
                String window = sve[i].substring(j, j + windowSize);
                String minimizer = "";
                for(int m =0;m<k;m++) {
                    minimizer+="T";

                }


                for (int z = 0; z <= windowSize - k; z++) {
                    String kmer = window.substring(z, z + k);


                    if (kmer.compareTo(minimizer)<0) {
                        minimizer = kmer;
                    }
                }

                // add the position to the list for the given minimizer
                positions = index.get(minimizer);
                if (positions == null) {
                    positions = new ArrayList<>();
                    index.put(minimizer, positions);
                }
                positions.add(j);
            }


             // add the map to the list
            try {
                write(outFile, index.toString());
            }
            catch (Exception e) {

            }
        }

        return lista;

    }

    public  String[] getReverseComplements(String[] dnaSequences) {
        String[] reverseComplements = new String[dnaSequences.length];

        for (int i = 1; i < dnaSequences.length; i++) {
            reverseComplements[i] = getReverseComplement(dnaSequences[i]);
        }

        return reverseComplements;
    }

    private  String getReverseComplement(String dnaSequence) {
        StringBuilder reverseComplement = new StringBuilder();

        for (int i = dnaSequence.length() - 1; i >= 0; i--) {
            char base = dnaSequence.charAt(i);
            switch (base) {
                case 'A':
                    reverseComplement.append('T');
                    break;
                case 'T':
                    reverseComplement.append('A');
                    break;
                case 'G':
                    reverseComplement.append('C');
                    break;
                case 'C':
                    reverseComplement.append('G');
                    break;
                default:
                    throw new IllegalArgumentException("Invalid base in DNA sequence: " + base);
            }
        }

        return reverseComplement.toString();
    }

        }