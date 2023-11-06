# ifndef BOOTHSALGBITSTRING_H
# define BOOTHSALGBITSTRING_H

#include <string>

using std::string;

class BoothsAlgBitString
{
    private:
        string accumulator;
        string multiplier;
        char extendedBit;

    public:
        BoothsAlgBitString(const string& inputMultiplier)
        {
            for(int i = 0; i < inputMultiplier.length(); ++i)
                accumulator += '0';

            multiplier = inputMultiplier;
            extendedBit = '0';
        }

        void shiftRight();
        void add(const string& addend);

        string getProduct() const
        {
            return (accumulator + multiplier);
        }

        string getLastTwoBits() const
        {
            return (multiplier.substr(multiplier.length() - 1, 1) + extendedBit);
        }

        string getLastThreeBits() const
        {
            return (multiplier.substr(multiplier.length() - 2, 2) + extendedBit);
        }

        string print() const { return (accumulator + " " + multiplier + " " + extendedBit); }
};

string twosComplement(const string wordToComp);

string timesTwo(const string wordToMult);

# endif
