#include <iostream>
#include <fstream>
#include <cstdlib>

#include "BoothsAlgBitString.h"

using std::cout;
using std::cerr;
using std::endl;
using std::rand;
using std::ifstream;
using std::ofstream;

const string TESTDATAFILENAME = "TestData.dat";
const string OUTPUTDATAFILENAME = "simulatorOutput.txt";

int main()
{
    // Seed RNG.
    srand(time(NULL));

    // Input and output file streams.
    ifstream testDataFile;
    ofstream simulatorOutput;

    // Used to determine if we are receiving output from an external file, or sending
    // output to an external file.
    bool testDataFileFound = false;
    bool writingToOutput = false;

    // Strings to hold our multiplier and multiplicand in.
    string myMultiplicand = "";
    string myMultiplier = "";
    string myComplementedMultiplicand = "";

    // Attempt to open the input file stream.
    try
    {
        testDataFile.open(TESTDATAFILENAME);
        cout << TESTDATAFILENAME << " was open, reading sample data..." << endl;

        testDataFileFound = true;
    }
    catch (const ifstream::failure& ex)
    {
        cerr << TESTDATAFILENAME << " was not found in the current directory. ";
        cerr << "A randomly generated string will be used instead for multiplier and multiplicand." << endl;
    }

    // Attempt to open the output file stream.
    try
    {
        simulatorOutput.open(OUTPUTDATAFILENAME);
        cout << "Writing output to: " << OUTPUTDATAFILENAME << endl;

        writingToOutput = true;
    }
    catch (const ofstream::failure& ex)
    {
        cerr << "An error occurred when attempting to open file for output streaming." << endl;
        cerr << "\nOutput will NOT be streamed to a file." << endl;
    }

    // Read in data if the file was found.
    if (testDataFileFound)
    {
        testDataFile >> myMultiplier;
        testDataFile >> myMultiplicand;
    }
    // Otherwise, generate random strings for both the multiplier and multiplicand that
    // are between 4 to 12 characters long.
    else
    {
        // Generates an integer between 4 and 12, the size constraints of strings
        // this program will be multiplying.
        int stringSize = rand() % (12 - 4 + 1) + 4;

        // Creates a randomly generated binary string of size stringSize.
        for (int i = 0; i < stringSize; ++i)
        {
            if ((rand() % 10 + 1) > 5)
                myMultiplicand += '1';
            else
                myMultiplicand += '0';

            if ((rand() % 10 + 1) > 5 )
                myMultiplier += '1';
            else
                myMultiplier += '0';
        }
    }

    myComplementedMultiplicand = twosComplement(myMultiplicand);

    // A do-while loop is used because the algorithms on the inside will always be ran once. This is important
    // in the case when we are using randomly generated strings, because we will only need one iteration of the
    // loops contents, instead of multiple in the case of an input file stream.
    do
    {
        // Defines the start of a block of modified/unmodified Booth's Algorithm in the output file.
        if (writingToOutput)
        {
            simulatorOutput << "===============================================" << endl;
            simulatorOutput << "Multiplier: " << myMultiplier << " Multiplicand: " << myMultiplicand << endl;
        }

        // ========================================================================
        // The following block performs normal Booth's Algorithm.                 |
        // ========================================================================
        cout << "Normal Booth's Algorithm:\n" << endl;

        BoothsAlgBitString normalBoothsAlgBitString(myMultiplier);

        cout << "Multiplicand: " << myMultiplicand << "\nMultiplier: " << myMultiplier << '\n' << endl;

        int numAdditions = 0;
        int numSubtractions = 0;
        int numIterations = 0;

        string iterationDivider = "";
        for (int i = 0; i < normalBoothsAlgBitString.print().length(); ++i)
            iterationDivider += "-";

        // The number of iterations performed in Booth's Algorithm is equivalent to
        // the size of either the multiplicand and the multiplier (since each are
        // the same size).
        for(int i = 0; i < myMultiplicand.length(); i++)
        {
            // Get last two bits of ACQE.
            string normalBoothsAlgCheck = normalBoothsAlgBitString.getLastTwoBits();

            if (i > 0)
                cout << iterationDivider << endl;

            // "00" or "11" found, only shift right.
            if (normalBoothsAlgCheck == "00" || normalBoothsAlgCheck == "11")
                cout << normalBoothsAlgBitString.print() << " | " << normalBoothsAlgCheck << " - SHR" << endl;
            // "01" found, add multiplicand then shift right.
            else if (normalBoothsAlgCheck == "01")
            {
                cout << normalBoothsAlgBitString.print() << " | " << normalBoothsAlgCheck << " - ADD" << endl;
                cout << myMultiplicand << endl;

                normalBoothsAlgBitString.add(myMultiplicand);

                cout << normalBoothsAlgBitString.print() << " | SHR" << endl;

                ++numAdditions;
            }
            // "10" found, subtract multiplicand then shift right.
            else if (normalBoothsAlgCheck == "10")
            {
                cout << normalBoothsAlgBitString.print() << " | " << normalBoothsAlgCheck << " - SUB" << endl;
                cout << myComplementedMultiplicand << endl;

                normalBoothsAlgBitString.add(myComplementedMultiplicand);

                cout << normalBoothsAlgBitString.print() << " | SHR" << endl;

                ++numSubtractions;
            }

            // Every outcome of getLastTwoBits() will be shifted right here.
            normalBoothsAlgBitString.shiftRight();

            ++numIterations;
        }

        cout << iterationDivider << '\n' << normalBoothsAlgBitString.print() << " | DONE" << endl;

        cout << "\nResult: " << normalBoothsAlgBitString.getProduct() << endl;
        cout << "Number of Additions Used: " << numAdditions << "\nNumber of Subtractions Used: " << numSubtractions << endl;
        cout << "Total Number of Iterations: " << numIterations << endl;

        // Format output to external file to show statistics of normal Booth's Algorithm.
        if (writingToOutput)
        {
            simulatorOutput << "\nBooth's Algorithm:\n" << "Result: " << normalBoothsAlgBitString.getProduct() << endl;
            simulatorOutput << "Number of Additions Used: " << numAdditions << "\nNumber of Subtractions Used: " << numSubtractions << endl;
            simulatorOutput << "Total Number of Iterations: " << numIterations << endl;
        }
        // ========================================================================

        // ========================================================================
        // The following block performs Modified Booth's Algorithm.               |
        // ========================================================================
        // For Modified Booth's Algorithm, the length of the words needs to be even, if it is not, then we must
        // pad the two bits to the next greatest even length accordingly.
        if (myMultiplicand.length() % 2 != 0 || myMultiplier.length() % 2 != 0)
        {
            // Add a one to the front of the number if it is negative.
            if (myMultiplicand[0] == '1')
                myMultiplicand = '1' + myMultiplicand;
            // Add a zero otherwise.
            else
                myMultiplicand = '0' + myMultiplicand;

            // Do the same for the multiplier as the multiplicand.
            if (myMultiplier[0] == '1')
                myMultiplier = '1' + myMultiplier;
            else
                myMultiplier = '0' + myMultiplier;

            // Adjust the twos complement of the multiplicand after padding.
            myComplementedMultiplicand = twosComplement(myMultiplicand);
        }

        cout << "\n\nModified Booth's Algorithm:\n" << endl;

        BoothsAlgBitString modifiedBoothsAlgBitString(myMultiplier);

        cout << "Multiplicand: " << myMultiplicand << "\nMultiplier: " << myMultiplier << '\n' << endl;

        numAdditions = 0;
        numSubtractions = 0;
        numIterations = 0;

        iterationDivider = "";
        for (int i = 0; i < modifiedBoothsAlgBitString.print().length(); ++i)
            iterationDivider += "-";

        // In the case of Modified Booth's Algorithm, the total number of
        // iterations performed is equivalent to half of the size of either the
        // multiplicand or the multiplier.
        for(int i = 0; i < myMultiplicand.length() / 2; ++i)
        {
            // Get last three bits of ACQE.
            string modifiedBoothsAlgCheck = modifiedBoothsAlgBitString.getLastThreeBits();

            if (i > 0)
                cout << iterationDivider << endl;

            // "000" or "111" found, only shift right twice.
            if (modifiedBoothsAlgCheck == "000" || modifiedBoothsAlgCheck == "111")
                cout << modifiedBoothsAlgBitString.print() << " | " << modifiedBoothsAlgCheck << " - SHR x2" << endl;
            // "001" or "010" found, add multiplicand then shift right twice.
            else if (modifiedBoothsAlgCheck == "001" || modifiedBoothsAlgCheck == "010")
            {
                cout << modifiedBoothsAlgBitString.print() << " | " << modifiedBoothsAlgCheck << " - ADD" << endl;
                cout << myMultiplicand << endl;

                modifiedBoothsAlgBitString.add(myMultiplicand);
                ++numAdditions;

                cout << modifiedBoothsAlgBitString.print() << " | SHR x2" << endl;
            }
            // "011" found, add two times the multiplicand then shift right twice.
            else if (modifiedBoothsAlgCheck == "011")
            {
                cout << modifiedBoothsAlgBitString.print() << " | " << modifiedBoothsAlgCheck << " - ADD x2" << endl;
                cout << timesTwo(myMultiplicand) << endl;

                modifiedBoothsAlgBitString.add(timesTwo(myMultiplicand));
                ++numAdditions;

                cout << modifiedBoothsAlgBitString.print() << " | SHR x2" << endl;
            }
            // "100" found, subtract two times the multiplicand then shift right twice.
            else if (modifiedBoothsAlgCheck == "100")
            {
                cout << modifiedBoothsAlgBitString.print() << " | " << modifiedBoothsAlgCheck << " - SUB x2" << endl;
                cout << timesTwo(myComplementedMultiplicand) << endl;

                modifiedBoothsAlgBitString.add(timesTwo(myComplementedMultiplicand));
                ++numSubtractions;

                cout << modifiedBoothsAlgBitString.print() << " | SHR x2" << endl;

            }
            // "101' or "110" found, subtract the multiplicand then shift right twice.
            else if (modifiedBoothsAlgCheck == "101" || modifiedBoothsAlgCheck == "110")
            {
                cout << modifiedBoothsAlgBitString.print() << " | " << modifiedBoothsAlgCheck << " - SUB" << endl;
                cout << timesTwo(myComplementedMultiplicand) << endl;

                modifiedBoothsAlgBitString.add(myComplementedMultiplicand);
                ++numSubtractions;

                cout << modifiedBoothsAlgBitString.print() << " | SHR x2" << endl;
            }

            // Every outcome of getLastTwoBits() will be shifted right twice here.
            modifiedBoothsAlgBitString.shiftRight();
            modifiedBoothsAlgBitString.shiftRight();

            ++numIterations;
        }

        cout << iterationDivider << '\n' << modifiedBoothsAlgBitString.print() << " | DONE" << endl;

        cout << "\nResult: " << modifiedBoothsAlgBitString.getProduct() << endl;
        cout << "Number of Additions Used: " << numAdditions << "\nNumber of Subtractions Used: " << numSubtractions << endl;
        cout << "Total Number of Iterations: " << numIterations << endl;

        // Format output to external file to show statistics of Modified Booth's Algorithm.
        if (writingToOutput)
        {
            simulatorOutput << "\nModified Booth's Algorithm:\n" << "Result: " << modifiedBoothsAlgBitString.getProduct() << endl;
            simulatorOutput << "Number of Additions Used: " << numAdditions << "\nNumber of Subtractions Used: " << numSubtractions << endl;
            simulatorOutput << "Total Number of Iterations: " << numIterations << endl;
            simulatorOutput << "===============================================\n" << endl;
        }
        // ========================================================================

        // If we are streaming sample data from a file, read in the next multiplier, multiplicand pair.
        if (testDataFileFound)
        {
            testDataFile >> myMultiplier;
            testDataFile >> myMultiplicand;

            myComplementedMultiplicand = twosComplement(myMultiplicand);
        }
    }
    // The do-while loop will continue so long as we have not reached the end of the testDataFile stream. The loop also stops
    // after one iteration if no external file is being read from.
    while (testDataFileFound && !testDataFile.eof());

    // Close input stream if it was opened.
    if (testDataFileFound)
        testDataFile.close();

    // Close output stream if it was opened.
    if (writingToOutput)
        simulatorOutput.close();

    return 0;
}
