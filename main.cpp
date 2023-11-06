#include <iostream>

#include "BoothsAlgBitString.h"

using std::cout;
using std::endl;

int main()
{
    string myMultiplicand = "000001";
    string myMultiplier = "000001";
    string myComplementedMultiplicand = twosComplement(myMultiplicand);

    // The following block performs normal Booth's Algorithm.
    cout << "Normal Booth's Algorithm:\n" << endl;

    BoothsAlgBitString normalBoothsAlgBitString(myMultiplier);

    cout << "Multiplicand: " << myMultiplicand << "\nMultiplier: " << myMultiplier << '\n' << endl;

    int numAdditions = 0;
    int numSubtractions = 0;

    string iterationDivisior = "";
    for (int i = 0; i < normalBoothsAlgBitString.print().length(); ++i)
        iterationDivisior += "-";

    for(int i = 0; i < myMultiplicand.length(); i++)
    {
        string normalBoothsAlgCheck = normalBoothsAlgBitString.getLastTwoBits();

        if (i > 0)
            cout << iterationDivisior << endl;

        if (normalBoothsAlgCheck == "00" || normalBoothsAlgCheck == "11")
        {
            cout << normalBoothsAlgBitString.print() << " | " << normalBoothsAlgCheck << " - SHR" << endl;
        }
        else if (normalBoothsAlgCheck == "01")
        {
            cout << normalBoothsAlgBitString.print() << " | " << normalBoothsAlgCheck << " - ADD" << endl;
            cout << myMultiplicand << endl;

            normalBoothsAlgBitString.add(myMultiplicand);

            cout << normalBoothsAlgBitString.print() << " | SHR" << endl;

            ++numAdditions;
        }
        else if (normalBoothsAlgCheck == "10")
        {
            cout << normalBoothsAlgBitString.print() << " | " << normalBoothsAlgCheck << " - SUB" << endl;
            cout << myComplementedMultiplicand << endl;

            normalBoothsAlgBitString.add(myComplementedMultiplicand);

            cout << normalBoothsAlgBitString.print() << " | SHR" << endl;

            ++numSubtractions;
        }

        normalBoothsAlgBitString.shiftRight();
    }

    cout << iterationDivisior << '\n' << normalBoothsAlgBitString.print() << " | DONE" << endl;

    cout << "\nResult: " << normalBoothsAlgBitString.getProduct() << endl;
    cout << "Number of Additions Used: " << numAdditions << "\nNumber of Subtractions Used: " << numSubtractions << endl;

    // The following block performs Modified Booth's Algorithm.

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

    iterationDivisior = "";
    for (int i = 0; i < modifiedBoothsAlgBitString.print().length(); ++i)
        iterationDivisior += "-";

    for(int i = 0; i < myMultiplicand.length() / 2; ++i)
    {
        string modifiedBoothsAlgCheck = modifiedBoothsAlgBitString.getLastThreeBits();

        if (i > 0)
            cout << iterationDivisior << endl;

        if (modifiedBoothsAlgCheck == "000" || modifiedBoothsAlgCheck == "111")
        {
            cout << modifiedBoothsAlgBitString.print() << " | " << modifiedBoothsAlgCheck << " - SHR x2" << endl;
        }
        else if (modifiedBoothsAlgCheck == "001" || modifiedBoothsAlgCheck == "010")
        {
            cout << modifiedBoothsAlgBitString.print() << " | " << modifiedBoothsAlgCheck << " - ADD" << endl;
            cout << myMultiplicand << endl;

            modifiedBoothsAlgBitString.add(myMultiplicand);
            ++numAdditions;

            cout << modifiedBoothsAlgBitString.print() << " | SHR x2" << endl;
        }
        else if (modifiedBoothsAlgCheck == "011")
        {
            cout << modifiedBoothsAlgBitString.print() << " | " << modifiedBoothsAlgCheck << " - ADD x2" << endl;
            cout << timesTwo(myMultiplicand) << endl;

            modifiedBoothsAlgBitString.add(timesTwo(myMultiplicand));
            ++numAdditions;

            cout << modifiedBoothsAlgBitString.print() << " | SHR x2" << endl;
        }
        else if (modifiedBoothsAlgCheck == "100")
        {
            cout << modifiedBoothsAlgBitString.print() << " | " << modifiedBoothsAlgCheck << " - SUB x2" << endl;
            cout << timesTwo(myComplementedMultiplicand) << endl;

            modifiedBoothsAlgBitString.add(timesTwo(myComplementedMultiplicand));
            ++numSubtractions;

            cout << modifiedBoothsAlgBitString.print() << " | SHR x2" << endl;

        }
        else if (modifiedBoothsAlgCheck == "101" || modifiedBoothsAlgCheck == "110")
        {
            cout << modifiedBoothsAlgBitString.print() << " | " << modifiedBoothsAlgCheck << " - SUB" << endl;
            cout << timesTwo(myComplementedMultiplicand) << endl;

            modifiedBoothsAlgBitString.add(myComplementedMultiplicand);
            ++numSubtractions;

            cout << modifiedBoothsAlgBitString.print() << " | SHR x2" << endl;
        }

        modifiedBoothsAlgBitString.shiftRight();
        modifiedBoothsAlgBitString.shiftRight();
    }

    cout << iterationDivisior << '\n' << modifiedBoothsAlgBitString.print() << " | DONE" << endl;

    cout << "\nResult: " << modifiedBoothsAlgBitString.getProduct() << endl;
    cout << "Number of Additions Used: " << numAdditions << "\nNumber of Subtractions Used: " << numSubtractions << endl;

    return 0;
}
