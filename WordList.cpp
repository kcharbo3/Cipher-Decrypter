#include "provided.h"
#include "MyHash.h"
#include <fstream>
#include <functional>
#include <string>
#include <vector>
using namespace std;


class WordListImpl
{
public:
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
    MyHash<int, vector<string>> table;
    
    int getPattern(const string& word) const;
    bool translationCipherMatch(const string& wordCipher, const string& currTranslation) const;
    bool translationMatch(const string& currTranslation, const string& word) const;
};

int WordListImpl::getPattern(const string& word) const
{
    int ans = 0;
    string upperWord = word;
    for(int i = 0; i < word.size(); i++)
    {
        upperWord[i] = toupper(word[i]);
    }
    MyHash<char, int> temp;
    int numChars = 0;

    for(int i = 0; i < upperWord.size(); i++)
    {
        if(temp.find(upperWord[i]) == nullptr)
        {
            numChars++;
            temp.associate(upperWord[i], numChars);
        }
        ans = (ans * 10) + *(temp.find(upperWord[i]));
    }
    return ans;
}



bool WordListImpl::translationCipherMatch(const string& wordCipher, const string& currTranslation) const
{
    if(currTranslation.size() != wordCipher.size())
        return false;
    for(int i = 0; i < wordCipher.size(); i++)
    {
        if(isalpha(currTranslation[i]) && !isalpha(wordCipher[i]))
            return false;
        if(currTranslation[i] == '?' && !isalpha(wordCipher[i]))
            return false;
        if(currTranslation[i] == '\'' && wordCipher[i] != '\'')
            return false;
    }
    return true;
}


bool WordListImpl::translationMatch(const string& currTranslation, const string& word) const
{
    if(currTranslation.size() != word.size())
        return false;
    for(int i = 0; i < word.size(); i++)
    {
        if(currTranslation[i] != '?')
        {
            if(toupper(currTranslation[i]) != toupper(word[i]))
                return false;
        }
        else
        {
            if(!isalpha(toupper(word[i])))
                return false;
        }
    }
    return true;
}


bool WordListImpl::loadWordList(string filename)
{
    table.reset();
    ifstream wordFile(filename);
    if ( ! wordFile )
    {
        return false;
    }
    
    string word;
    while (getline(wordFile, word))
    {
        bool skip = false;

        for(int i = 0; i < int(word.size()); i++)
        {
            if(!isalpha(word[i]) && word[i] != '\'')
                skip = true;
            else
                word[i] = toupper(word[i]);
        }
        int pattern = getPattern(word);
        if(skip == false)
        {
            if(table.find(pattern) == nullptr)
            {
                vector<string> newV;
                newV.push_back(word);
                table.associate(pattern, newV);
            }
            else
            {
                vector<string>* newV = table.find(pattern);
                newV->push_back(word);
            }
        }
    }
    return true;
}

bool WordListImpl::contains(string word) const
{
    string upperWord;
    for(int i = 0; i < int(word.size()); i++)
    {
        upperWord += toupper(word[i]);
    }
    int pattern = getPattern(upperWord);
    
    if(table.find(pattern) == nullptr)
    {
        return false;
    }
    else
    {
        const vector<string>* v = table.find(pattern);
        vector<string>::const_iterator it = v->begin();
        while(it != v->end())
        {
            if((*it) == upperWord)
                return true;
            it++;
        }
    }
    return false;
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
    vector<string> words;
    if(!translationCipherMatch(cipherWord, currTranslation))
        return words;
    int cipherPattern = getPattern(cipherWord);
    if(table.find(cipherPattern) == nullptr)
        return words;
    else
    {
        const vector<string>* v = table.find(cipherPattern);
        vector<string>::const_iterator it = v->begin();
        while(it != v->end())
        {
            if(translationMatch(currTranslation, (*it)))
                words.push_back(*it);
            it++;
        }
    }
    return words;
}

//***** hash functions for string, int, and char *****

unsigned int hash(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
    return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(string filename)
{
    return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
    return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
    return m_impl->findCandidates(cipherWord, currTranslation);
}




