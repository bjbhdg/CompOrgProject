#include "BoothsAlgBitString.h"

void BoothsAlgBitString::shiftRight()
{
    // Store the last character of AC, as that will become the first character
    // of Q.
    char bitShiftedOffAC = accumulator.at(accumulator.length() - 1);

    // Here, we check to see if the AC is negative, if it is, then we must use
    // a 1 when shifting right to maintain the AC as a negative number.
    if (accumulator.at(0) == '1')
        accumulator = '1' + accumulator.substr(0, accumulator.length() - 1);
    // If it is positive, then add a 0 instead.
    else
        accumulator = '0' + accumulator.substr(0, accumulator.length() - 1);

    // E will receive the last character of Q.
    extendedBit = multiplier.at(multiplier.length() - 1);
    multiplier = bitShiftedOffAC + multiplier.substr(0, multiplier.length() - 1);

    return;
}

void BoothsAlgBitString::add(const string& addend)
{
    // The sum will be built from the least to most significant bit.
    string sum = "";

    // We add from the least significant bit to the most significant, so each
    // index will be assigned the index pointing to the least significant bit.
    int accumulatorIndex = accumulator.length() - 1;
    int addendIndex = addend.length() - 1;

    char carryBit = '0';

    // It is essential here to have the addend have the same length as the AC.
    // We will be iterating through each bit simulatenously, so they must be
    // equal, or else we will end up walking off either the addend or the AC.
    while (accumulatorIndex >= 0 && addendIndex >= 0)
    {
        // bitValuesToBeAdded will be a three character long string containing
        // the values of the accumulator at accumulatorIndex, the addend at
        // addendIndex, and the carryBit. The string pattern generated will be
        // checked below.
        string bitValuesToBeAdded = (accumulator.substr(accumulatorIndex, 1) + addend.substr(addendIndex, 1) + carryBit);

        // If the sum of every character equates to 0, then we push a 0 to the
        // sum and a 0 as the carryBit.
        if (bitValuesToBeAdded == "000")
        {
            sum.insert(0, "0");
            carryBit = '0';
        }
        // If any pattern equates to 1, then a 1 is pushed to sum and a 0 as a
        // carry bit.
        else if(bitValuesToBeAdded == "001" || bitValuesToBeAdded == "010" || bitValuesToBeAdded == "100")
        {
            sum.insert(0, "1");
            carryBit = '0';
        }
        // If the patterns equate to a 2, then we have a carryBit valued at 1,
        // but the sum will be prepended with a 0.
        else if(bitValuesToBeAdded == "011" || bitValuesToBeAdded == "101" || bitValuesToBeAdded == "110")
        {
            sum.insert(0, "0");
            carryBit = '1';
        }
        // Lastly, a sum of 3 will result in a 1 added to the sum and a 1 as
        // the carryBit.
        else if(bitValuesToBeAdded == "111")
        {
            sum.insert(0, "1");
            carryBit = '1';
        }

        accumulatorIndex--;
        addendIndex--;
    }

    accumulator = sum;

    return;
}

string twosComplement(const string wordToComp)
{
    string onesComplementedWord = "";
    string twosComplementedWord = "";

    // A one is added later on to convert 1s complement to 2s complement.
    string addend = "1";

    // Flips each bit in wordToComp to its opposite value.
    for (int i = 0; i < wordToComp.length(); ++i)
    {
        (wordToComp[i] == '1') ? onesComplementedWord += '0' : onesComplementedWord += '1';

        // Since the values of the addend in augend when performing binary
        // addition should be equal, we pad 1 with one less than the length of
        // wordToComp of 0s.
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
    // Shifts the word to the left. Since wordToMult is in 2s complement, a 0
    // will always be added as the least significant bit of the shifted word.
    return (wordToMult.substr(1, wordToMult.length() - 1) + '0');
}
