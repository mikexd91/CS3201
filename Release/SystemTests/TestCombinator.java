import java.util.ArrayList;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;

class TestCombinator {
	private static ArrayList<String> output;
	
	public static void main(String[] args) {
		output = new ArrayList<String>();
		int lineCount = 0;

		// combine each file read in from args in turn
		for(int i=0; i<args.length; i++) {
			String currentFile = args[i];
			
			try {
				lineCount = processFile(currentFile, lineCount);
			} catch(IOException e) {
				System.err.println(currentFile + " read error...");
				e.printStackTrace();
			}
		}

		try {
			// write to output file
			outputResult("output.txt");
		} catch(IOException e) {
			System.err.println("write error...");
			e.printStackTrace();
		}
	}

	public static int processFile(String fileName, int count) throws IOException {
		FileInputStream fstream = new FileInputStream(fileName);
		BufferedReader reader = new BufferedReader(new InputStreamReader(fstream));

		String line;
		while((line = reader.readLine()) != null) {
			// header of each test case
			if(count%5 == 0) {
				try {
					String numRemoved = line.substring(line.indexOf(' '));
					output.add(Integer.toString(count/5+1).concat(numRemoved));
				} catch(Exception e) {
					System.err.println("Error processing line " + (count+1));
					e.printStackTrace();
				}
			} else {
				output.add(line);
			}

			count++;
		}

		reader.close();
		fstream.close();

		return count;
	}

	public static void outputResult(String fileName) throws IOException {
		FileOutputStream foutstream = new FileOutputStream(fileName);
		BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(foutstream));

		for(int i=0; i<output.size(); i++) {
			writer.write(output.get(i));

			if(i != output.size()-1) {
				writer.newLine();
			}
		}

		writer.close();
		foutstream.close();
	}
}
