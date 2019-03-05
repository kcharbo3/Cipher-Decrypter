#include "provided.h"
#include <string>
#include <list>
#include "MyHash.h"
using namespace std;

class TranslatorImpl
{
public:
    TranslatorImpl();
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
private:
    string currentMap;
    list<string> stack;
    int mapsPushed;
    int mapsPopped;
    
};

TranslatorImpl::TranslatorImpl()
:mapsPushed(0), mapsPopped(0)
{
    currentMap = "??????????????????????????";
    stack.push_back(currentMap);
    mapsPushed++;
}

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
    if(ciphertext.size() != plaintext.size())
        return false;
    string tempMap = currentMap;
    string upperCipher = ciphertext;
    string upperPlain = plaintext;
    for(int i = 0; i < ciphertext.size(); i++)
    {
        upperCipher[i] = toupper(ciphertext[i]);
        upperPlain[i] = toupper(plaintext[i]);
        if(!isalpha(upperCipher[i]) || !isalpha(upperPlain[i]))
            return false;
        int pos = upperCipher[i] - 65;
        if(tempMap[pos] != '?' && tempMap[pos] != upperPlain[i])
            return false;
        tempMap[pos] = upperPlain[i];
    }
    MyHash<char, int> duplicate;
    for(int i = 0; i < tempMap.size(); i++)
    {
        if(tempMap[i] != '?')
        {
            if(duplicate.find(tempMap[i]) != nullptr)
                return false;
            else
            {
                duplicate.associate(tempMap[i], 1);
            }
        }
    }
    stack.push_back(currentMap);
    currentMap = tempMap;
    mapsPushed++;
    return true;
}

bool TranslatorImpl::popMapping()
{
    if(mapsPushed == mapsPopped)
    {
        return false;
    }
    currentMap = stack.back();
    stack.pop_back();
    mapsPopped++;
    return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
    string ans = ciphertext;
    for(int i = 0; i < ciphertext.size(); i++)
    {
        int pos = toupper(ciphertext[i]) - 65;
        if(isupper(ciphertext[i]))
        {
            ans[i] = currentMap[pos];
        }
        else if(islower(ciphertext[i]))
        {
            ans[i] = tolower(currentMap[pos]);
        }
        else
        {
            ans[i] = ciphertext[i];
        }
    }
    return ans;
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}
