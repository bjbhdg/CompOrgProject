#include "BoothsAlgBitString.h"

void BoothsAlgBitString::shiftRight()
{
    char bitShiftedOffAC = accumulator.at(accumulator.length() - 1);

    if (accumulator.at(0) == '1')
        accumulator = '1' + accumulator.substr(0, accumulator.length() - 1);
    else
        accumulator = '0' + accumulator.substr(0, accumulator.length() - 1);

    extendedBit = multiplier.at(multiplier.length() - 1);
    multiplier = bitShiftedOffAC + multiplier.substr(0, multiplier.length() - 1);

    return;
}

void BoothsAlgBitString::add(const string& addend)
{
    string sum = "";

    int augendIndex = accumulator.length() - 1;
    int addendIndex = addend.length() - 1;

    char carryBit = '0';

    while (augendIndex >= 0 && addendIndex >= 0)
    {
        string bitValuesToBeAdded = (accumulator.substr(augendIndex, 1) + addend.substr(addendIndex, 1) + carryBit);

        if (bitValuesToBeAdded == "000")
        {
            sum.insert(0, "0");
            carryBit = '0';
        }
        else if(bitValuesToBeAdded == "001" || bitValuesToBeAdded == "010" || bitValuesToBeAdded == "100")
        {
            sum.insert(0, "1");
            carryBit = '0';
        }
        else if(bitValuesToBeAdded == "011" || bitValuesToBeAdded == "101" || bitValuesToBeAdded == "110")
        {
            sum.insert(0, "0");
            carryBit = '1';
        }
        else if(bitValuesToBeAdded == "111")
        {
            sum.insert(0, "1");
            carryBit = '1';
        }

        augendIndex--;
        addendIndex--;
    }

    accumulator = sum;

    return;
}

string twosComplement(const string wordToComp)
{
    string onesComplementedWord = "";
    string twosComplementedWord = "";

    string addend = "1";

    for (int i = 0; i < wordToComp.length(); ++i)
    {
        (wordToComp[i] == '1') ? onesComplementedWord += '0' : onesComplementedWord += '1';

        if (i < wordToComp.length() - 1)
            addend.insert(0, "0");
    }

    int onesCompWordIndex = onesComplementedWord.length() - 1;
    int addendIndex = addend.length() - 1;
    char carryBit = '0';

    while (onesCompWordIndex >= 0 && addendIndex >= 0)
    {
        string bitValuesToBeAdded = (onesComplementedWord.substr(onesCompWordIndex, 1) + addend.substr(addendIndex, 1) + carryBit);

        if (bitValuesToBeAdded == "000")
        {
            twosComplementedWord.insert(0, "0");
            carryBit = '0';
        }
        else if(bitValuesToBeAdded == "001" || bitValuesToBeAdded == "010" || bitValuesToBeAdded == "100")
        {
            twosComplementedWord.insert(0, "1");
            carryBit = '0';
        }
        else if(bitValuesToBeAdded == "011" || bitValuesToBeAdded == "101" || bitValuesToBeAdded == "110")
        {
            twosComplementedWord.insert(0, "0");
            carryBit = '1';
        }
        else if(bitValuesToBeAdded == "111")
        {
            twosComplementedWord.insert(0, "1");
            carryBit = '1';
        }

        --onesCompWordIndex;
        --addendIndex;
    }

    return twosComplementedWord;
}

string timesTwo(const string wordToMult)
{
    return (wordToMult.substr(1, wordToMult.length() - 1) + '0');
}
