#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


void deleteLinesAfter(const std::string& filename, int lineNumber) {
    // Open the file for reading and writing
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << " for reading." << std::endl;
        return;
    }

    std::ofstream outputFile("test/temp.txt"); // Temporary file to store modified content
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to create temporary file." << std::endl;
        return;
    }

    std::string line;
    int currentLineNumber = 0;

    // Read lines from the input file and copy them to the temporary file until the given line number
    while (std::getline(inputFile, line) && currentLineNumber < lineNumber) {
        outputFile << line << std::endl;
        currentLineNumber++;
    }

    // Close both files
    inputFile.close();
    outputFile.close();

    // // Replace the original file with the temporary file
    if (std::rename("temp.txt", filename.c_str()) != 0) {
        std::cerr << "Error: Failed to replace file " << filename << std::endl;
    }
	else {
		cout << "success" <<endl;
	}


}

int main(int argc, char *argv[])
{

 	std::string filename = "test/matrixsmall.txt";
    int lineNumber = 1000000; // Example line number

    deleteLinesAfter(filename, lineNumber);
}