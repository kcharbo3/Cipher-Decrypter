#include "provided.h"
#include "MyHash.h"
#include <string>
#include <vector>
#include <set>
using namespace std;

class DecrypterImpl
{
public:
    DecrypterImpl();
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
    WordList* dict;
    Tokenizer* token;
    Translator* trans;
    set<string> answers;
    
    void recursiveDecode(const string& ciphertext);
    int numUnknowns(string word);
    
};

int DecrypterImpl::numUnknowns(string word)
{
    string decoded = trans->getTranslation(word);
    int numQs = 0;
    for(int i = 0; i < decoded.size(); i++)
    {
        if(decoded[i] == '?')
            numQs++;
    }
    return numQs;
}


DecrypterImpl::DecrypterImpl()
{
    dict = new WordList();
    token = new Tokenizer("1234567890 ,;:.!()[]{}-\"#$%^&");
    trans = new Translator();
}

bool DecrypterImpl::load(string filename)
{
    if(!dict->loadWordList(filename))
        return false;
    return true;
}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{
    //step 1
    delete trans;
    trans = new Translator();
    
    set<string>::iterator setIt = answers.begin();
    while(setIt != answers.end())
    {
        setIt = answers.erase(setIt);
    }
    
    vector<string> returnValue;
    if(ciphertext == "")
        return returnValue;
    

    recursiveDecode(ciphertext);
    
    setIt = answers.begin();
    while(setIt != answers.end())
    {
        returnValue.push_back(*setIt);
        setIt++;
    }
    
    return returnValue;
}


void DecrypterImpl::recursiveDecode(const string& ciphertext)
{
    //step 2
    vector<string> tokenized = token->tokenize(ciphertext);
    string word;
    for(int i = 0; i < tokenized.size(); i++)
    {
        string w = tokenized[i];
        if(numUnknowns(w) > numUnknowns(word))
        {
            word = w;
        }
    }
    
    //step 3
    string currTrans = trans->getTranslation(word);
    
    //step 4
    vector<string> matches = dict->findCandidates(word, currTrans);
    set<string> matchSet;

    for(int i = 0; i < matches.size(); i++)
    {
        matchSet.insert(matches[i]);
    }
    
    //step 5
    if(matchSet.empty())
    {
        trans->popMapping();
        return;
    }
    
    //step 6
    set<string>::iterator it = matchSet.begin();
    
    while(it != matchSet.end())
    {
        bool breakOut = false;
        if(trans->pushMapping(word, *it))
        {
            int fullyTrans = 0;
            for(int i = 0; i < tokenized.size(); i++)
            {
                string curr = trans->getTranslation(tokenized[i]);
                if(numUnknowns(tokenized[i]) == 0 && !(dict->contains(curr)))
                {
                    trans->popMapping();
                    breakOut = true;
                    break;
                }
                if(numUnknowns(tokenized[i]) == 0)
                {
                    fullyTrans++;
                }
            }
            if(fullyTrans == tokenized.size() && breakOut == false)
            {
                answers.insert(trans->getTranslation(ciphertext));
                trans->popMapping();
            }
            else if(breakOut == false)
            {
                recursiveDecode(ciphertext);
            }
        }
        it++;
    }
    
    trans->popMapping();
    return;
}




//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
    m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
    delete m_impl;
}

bool Decrypter::load(string filename)
{
    return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
    return m_impl->crack(ciphertext);
}
