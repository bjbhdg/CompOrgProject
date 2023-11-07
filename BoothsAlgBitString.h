# ifndef BOOTHSALGBITSTRING_H
# define BOOTHSALGBITSTRING_H

#include <string>

using std::string;

class BoothsAlgBitString
{
    private:
        string accumulator; // Can be referred to as AC.
        string multiplier;  // Can be referred to as Q.
        char extendedBit;   // Can be referred to as E.

    public:
        // Purpose: Creates a BoothsAlgBitString object.
        // Pre: inputMultiplier should be a binary string in 2s complement.
        // Post: A BoothsAlgBitString is created.
        BoothsAlgBitString(const string& inputMultiplier)
        {
            // Fill the accumulator with zeroes equal to the entire length
            // of inputMultipler (ex: inputMultiplier = 110/accumulator = 000).
            for(int i = 0; i < inputMultiplier.length(); ++i)
                accumulator += '0';

            multiplier = inputMultiplier;
            extendedBit = '0'; // Will always be instantiated as '0'.
        }

        // Purpose: At the end of every iteration of (Modified) Booth's
        //          Algorithm, a right shift will always be performed. This
        //          right shift will be performed across the accumulator, the
        //          multiplier, and the extended bit (AC -> Q -> E).
        // Pre: none
        // Post: The set of bits created from ACQE is shifted to the right.
        void shiftRight();

        // Purpose: At certain points of the algorithm, we will be adding
        //          the multiplicand to the object's accumulator. This function
        //          serves as subtraction too, since we can simply add the twos
        //          complement of the addend instead.
        // Pre: addend must be a binary string in 2s complement that shares the
        //      same string length as the accumulator and the multiplier.
        // Post: The accumulator is assigned the sum of its own contents plus
        //       the addend.
        void add(const string& addend);

        // Purpose: Returns the double register storing the product of the
        //          multiplier and the external multiplicand.
        // Pre: none
        // Post: Returns a string containing the accumulator and the
        //       multiplier.
        string getProduct() const
        {
            return (accumulator + multiplier);
        }

        // Purpose: For normal Booth's Algorithm, at the beginning of each
        //          iteration, we must check the last two bits of ACQE. So,
        //          getLastTwoBits() returns the last character of the
        //          multiplier concatenated with the extendedBit.
        // Pre: none
        // Post: The final two bits of the string ACQE is returned.
        string getLastTwoBits() const
        {
            return (multiplier.substr(multiplier.length() - 1, 1) + extendedBit);
        }

        // Purpose: Similar to getLastTwoBits(), this function returns the last
        //          three bits of ACQE specifically for Modified Booth's
        //          Algorithm.
        // Pre: none
        // Post: Returns the concatenation of the last two bits of the
        //       multiplier and the extendedBit.
        string getLastThreeBits() const
        {
            return (multiplier.substr(multiplier.length() - 2, 2) + extendedBit);
        }

        // Purpose: Prints out the accumulator, the multiplier, and the
        //          extendedBit in a readable format.
        // Pre: none
        // Post: Returns a readable string of the combined bit string: ACQE.
        string print() const { return (accumulator + " " + multiplier + " " + extendedBit); }
};

// Purpose: twosComplement() will generate the 2s complement of the passed in
//          wordToComp. This is done by flipping every bit in wordToComp and
//          adding one to it.
// Pre: wordToComp should be a binary word in 2s complement form.
// Post: The 2s complement conversion of wordToComp is returned.
string twosComplement(const string wordToComp);

// Purpose: Multiplies the passed in 2s complement form word by two. This is
//          done by performing an arithmetic left shift on the word.
// Pre: wordToMult should be a binary string in 2s complement form.
// Post: A left shifted wordToMult is returned.
string timesTwo(const string wordToMult);

# endif
