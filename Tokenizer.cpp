#include "provided.h"
#include <string>
#include <vector>
using namespace std;

class TokenizerImpl
{
public:
    TokenizerImpl(string separators);
    vector<string> tokenize(const std::string& s) const;
private:
    string sep;
};

TokenizerImpl::TokenizerImpl(string separators)
:sep(separators)
{
}

vector<string> TokenizerImpl::tokenize(const std::string& s) const
{
    vector<string> test;
    int startIndex = 0;
    for(int i = 0; i < s.size(); i++)
    {
        for(int k = 0; k < sep.size(); k++)
        {
           if(sep[k] == s[i])
           {
               if(i - startIndex == 0)
               {
                   startIndex = i + 1;
                   break;
               }
               string newWord = s.substr(startIndex, (i - startIndex));
               test.push_back(newWord);
               startIndex = i + 1;
           }
        }
        if(startIndex != s.size() && i == s.size() - 1)
        {
            string newWord = s.substr(startIndex, (s.size() - startIndex));
            test.push_back(newWord);
        }
    }
    
    return test;
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const
{
    return m_impl->tokenize(s);
}
