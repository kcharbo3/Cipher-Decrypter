#include "provided.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cctype>
#include <random>
#include <algorithm>
#include <numeric>
using namespace std;

const string WORDLIST_FILE = "/users/KyleCharbonnet/Desktop/engmix.txt";
const string DECRYPT_THIS ="Jxwpjq qwrla glcu pcx qcn xkvv dw uclw ekarbbckpjwe dq jzw jzkpta jzrj qcn ekep'j ec jzrp dq jzw cpwa qcn eke ec. -Urls Jxrkp";

string encrypt(string plaintext)
{
    // Generate a to z
    char plaintextAlphabet[26+1];
    iota(plaintextAlphabet, plaintextAlphabet+26, 'a');
    plaintextAlphabet[26] = '\0';
    
    // Generate permutation
    string ciphertextAlphabet(plaintextAlphabet);
    default_random_engine e((random_device()()));
    shuffle(ciphertextAlphabet.begin(), ciphertextAlphabet.end(), e);
    
    // Run translator (opposite to the intended direction)
    Translator t;
    t.pushMapping(plaintextAlphabet, ciphertextAlphabet);
    return t.getTranslation(plaintext);
}

bool decrypt(string ciphertext)
{
    Decrypter d;
    if ( ! d.load(WORDLIST_FILE))
    {
        cout << "Unable to load word list file " << WORDLIST_FILE << endl;
        return false;
    }
    for (const auto& s : d.crack(ciphertext))
        cout << s << endl;
    return true;
}


bool test(string plaintext)
{
    cout << "Original: " << plaintext << endl;
    string encrypted = encrypt(plaintext);
    cout << "Encrypted: " << encrypted << endl;
    cout << "Decrypted: ";
    decrypt(encrypted);
    return true;
}

int main()
{
    Decrypter d;

    if ( ! d.load(WORDLIST_FILE))
    {
        cout << "Unable to load word list file " << WORDLIST_FILE << endl;
        return 1;
    }
    
    cout << "Test 1: " << endl;
    test("Mother died today. Or maybe yesterday, I do not know. I had a telegram from the home: Mother passed away. Funeral tomorrow. Yours sincerely. That does not mean anything. It may have been yesterday.");
    
    cout << "Test 2: " << endl;
    test("In the beginning God created the heaven and the earth. And the earth was without form, and void; and darkness was upon the face of the deep. And the Spirit of God moved upon the face of the waters.");
    
    cout << "Test 3: " << endl;
    test("In the late summer of that year we lived in a house in a village that looked across the river and the plain to the mountains. In the bed of the river there were pebbles and boulders, dry and white in the sun, and the water was clear and swiftly moving and blue in the channels. Troops went by the house and down the road and the dust they raised powdered the leaves of the trees. The trunks of the trees too were dusty and the leaves fell early that year and we saw the troops marching along the road and the dust rising and leaves, stirred by the breeze, falling and the soldiers marching and afterward the road bare and white except for the leaves.");
         
    cout << "\n \nDecrypted Message: " << endl;
    vector<string> vec = d.crack(DECRYPT_THIS);
    if(vec.size() == 0)
    {
        cout << "No possible solution given the current list of words." << endl;
    }
    for(int i = 0; i < vec.size(); i++)
    {
        cout << vec[i] << endl;
    }
    
    return 0;
}









