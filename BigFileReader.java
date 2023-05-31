import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class BigFileReader { // ULAZNI PARAMETAR PATH , U VARIJABLI TEKST(KOJA JE JAVNO DOSTUPNA) SE NALAZI CIJELI TEKST PROCITANOG FILE-a
    public  String tekst;
    BigFileReader( String filename) {

        tekst="";
        try (BufferedReader br = new BufferedReader(new FileReader(filename))) {
            String line;
            while ((line = br.readLine()) != null) {
                if(!line.startsWith(">"))
                tekst+=line;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public String genComplement() {
            StringBuilder complement = new StringBuilder();
            for (char nucleotide : tekst.toCharArray()) {
                if (nucleotide == 'A') {
                    complement.append('T');
                } else if (nucleotide == 'T') {
                    complement.append('A');
                } else if (nucleotide == 'C') {
                    complement.append('G');
                } else if (nucleotide == 'G') {
                    complement.append('C');
                }
            }
            return complement.toString();
        }

    public  String genReverse() {
        // Create a StringBuilder object and append the input string
        StringBuilder sb = new StringBuilder(tekst);

        // Use the reverse() method of StringBuilder to reverse the string
        sb.reverse();

        // Convert the StringBuilder back to a string
        return sb.toString();
    }



}
