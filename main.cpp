#include <iostream>

#include "BoothsAlgBitString.h"

using std::cout;
using std::endl;

int main()
{
    // 1010001100000
    // 0000011001110

    string myMultiplicand = "001011";
    string myMultiplier = "011001";
    string myComplementedMultiplicand = twosComplement(myMultiplicand);

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
        {
            cout << normalBoothsAlgBitString.print() << " | " << normalBoothsAlgCheck << " - SHR" << endl;
        }
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
        {
            cout << modifiedBoothsAlgBitString.print() << " | " << modifiedBoothsAlgCheck << " - SHR x2" << endl;
        }
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
    // ========================================================================

    return 0;
}
