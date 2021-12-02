#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>
using namespace std;

class StudentSpellCheck : public SpellCheck {
public:
    StudentSpellCheck() { root = getnewTrieNode();}
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
    vector<string> dicWords;
    //creating our trie structure that gives us 27 possibl enext letters, alphabet plus apostrophe, also lets us check if that letter is the last in a known word through the boolean
    
    struct TrieNode
    {
         TrieNode *nextLetter[27];
         bool endWord;
    };
    //we have our starting point
    TrieNode* root;
    
    void deleteTree(TrieNode* subtree);
    //this gives us a new trie node that has null appointed to all of its next letters so that we can check for a fresh sequence of letters
    TrieNode* getnewTrieNode()
    {
        TrieNode* newNode =  new TrieNode;
        
        //appoint null to all next letters
        for (int i = 0; i < 27; i++)
        {
            newNode->nextLetter[i] = NULL;
        }
        //show that we haven't hit the end of a word yet
        newNode->endWord = false;
      
        return newNode;
    }
    
    
      
    //inserts our trie node into the possible dictionary list
    void insert(TrieNode* root, string word)
    {
        TrieNode* inserting = root;
      
        //for every letter in word
        for (int i = 0; i < word.length(); i++)
        {
            
            //find out which next letter index position the letter should be checked at
            int j = word[i] - 'A';
            //if its an apostrophe make it the last index to check
            if(word[i] == '\'')
            {
                j = 26;
            }
            
            //if that index spot doesn't have a next letter, we can go ahead and add one and then traverse down the sequence of letters
            if (inserting->nextLetter[j] == NULL)
                inserting->nextLetter[j] = getnewTrieNode();
      
            //even if it does have one, we can traverse down that unique letter
            inserting = inserting->nextLetter[j];
        }
      
        // once we are out of our for loop, we know that we have reached the end of a word since we went through all the letters
        inserting->endWord = true;
    }
      
   
    //searches for the key in the trie dictionary, if its there, returns true, otherwise false
    bool search(TrieNode* root, string key)
    {
        TrieNode* search = root;
      
        //for every letter in the key
        for (int i = 0; i < key.length(); i++)
        {
            
            //need to know which position to check the letter in
            int j = key[i] - 'A';
            
            //if its an apostrophe we check the last position in the array
            if(key[i] == '\'')
            {
                j = 26;
            }
            
            //if our key gets to a point where the sequence of letters doesn't exist, it means that our word cannot be found in the dictionary, so we return false
            if (search->nextLetter[j] == NULL)
                return false;
      
            //otherwise we keep going down the sequence
            search = search->nextLetter[j];
        }
      
        //in th eend we need to make sure that our key marks the end of a word and that the lasst letter does exist in the sequence
        return (search != NULL && search->endWord);
    } 
};

#endif  // STUDENTSPELLCHECK_H_
