#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;




SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}

StudentSpellCheck::~StudentSpellCheck() {
	
    //calls the function to clear up our tree by deleting each node from the bottom up
    deleteTree(root);
    
}

bool StudentSpellCheck::load(std::string dictionaryFile) {
   

    //if file wasn't able to be loaded return false
    ifstream infile(dictionaryFile);
    if(!infile)
    {
        return false;
    }
    
    //if it could be loaded then we have to start from scratch, so clear up our trie and reset it so we can start adding new words to our dictionary
    deleteTree(root);
    root = getnewTrieNode();
    
    string line;
    
    //repeatedly for each line in the dicitonary
    while(getline(infile, line))
    {
        string toBeInserted = "";
        for(int i = 0; i<line.length(); i++)
        {
            //only create the word with letters and apostrophes in the line (uppercase)
            if(isalpha(line.at(i)) ||  line.at(i) == '\'')
            {
                toBeInserted += toupper(line.at(i));
            }
        }
        
        //if there were letters or apostrophes added to create a word, add it to our word vector
        if(toBeInserted.length() != 0)
        {
            dicWords.push_back(toBeInserted);
        }
        
    }
    
    //next insert every word in our word vector to our trie structure that acts like a dictionary
    for(int i = 0; i<dicWords.size();i++)
    {
        insert(root,dicWords[i]);
    }
    
    
    //return true since the dictionary was loaded into our trie
    return true;

    
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions)
{
    
    string Word;
    //create new string with the uppercase version
    for(int i = 0; i<word.length(); i++)
    {
        Word += toupper(word.at(i));
    }
    
    //check if the word is even a word and if it is return true, its correctly spelled
    bool inDic = search(root,Word);
    if(inDic)
        return true;
    
    //if not that means it isn't a word and could have been mispelled, so clear up our suggestions
    string suggestionWord;
    suggestions.clear();
    int counter = 0;
    //repeatedly go through each letter position in the mispelled word, until we have hit every position or have hit th emaximum amount of suggesitons
    for(int i = 0; i<Word.length() && counter < max_suggestions;i++)
    {
        suggestionWord = Word;
        //subsitute every other CAPITAL letter and check if that makes a word
        for(int j = 0; j<26 && counter < max_suggestions ;j++)
        {
            suggestionWord[i] = j + 'A';
            inDic = search(root,suggestionWord);
            //if it does then add it to our suggestions
            if(inDic)
            {
                suggestions.push_back(suggestionWord);
                counter++;
            }
            
        }
        //finally after all letters have been checked, we can check the apostrophe case
        if(counter <max_suggestions)
        {
            suggestionWord[i] = '\'';
            inDic = search(root,suggestionWord);
            if(inDic)
            {
                suggestions.push_back(suggestionWord);
                counter++;
            }
        }
        
    }
    
    //reutrn false because the word was mispelled and we checked for suggestions
    return false;
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
	
    
    
    vector<string> word;
    vector<Position> pos;
    int counter = 0;
    problems.clear();
    string templine = line;
    string toBeChecked = "";
    
    
    //repeatedly for ever positon in the line
    for(int i = 0; i<templine.length();i++)
    {
        //check if it is a letter or apostrophe
        if(isalpha(templine.at(i)) || templine.at(i) == '\'' )
        {
            //if it is we can add it to the end of our running word
            toBeChecked += toupper(templine.at(i));
            counter++;
        }
        //if its not
        else
        {
            //and we have a running word
            if(counter>0)
            {
                //have that word be put into a vector to check spelling later, and place its position in the corresponding index in the position vector
                    word.push_back(toBeChecked);
                    Position temp;
                    temp.start = i-counter;
                    temp.end = i-1;
                    pos.push_back(temp);
                
            }
            //reset our running word
            toBeChecked = "";
            counter = 0;
        }
    }
    
    
    //check for the instance where our running word is at the end of the line
    if(counter>0)
    {
            word.push_back(toBeChecked);
            Position temp;
            temp.start = templine.length()-counter;
            temp.end = templine.length()-1;
            pos.push_back(temp);
        
    }

    //repeatedly search whether the word was mispelled or is an actual word, if it is place it into the problems vector with its correspodning position
    bool isWord;
    for(int i = 0; i<word.size();i++)
    {
        isWord = search(root, word[i]);
        if(!isWord)
            problems.push_back(pos[i]);
    }
    
    
    
     
}


void StudentSpellCheck::deleteTree(TrieNode* subtree)
{
    //making sure we get to the end of the trie
    if(subtree !=nullptr)
    {
        //for every next letter a letter can have, we need to traverse through it to ge tto the end
        for(int i = 0; i <27; i++)
            deleteTree(subtree->nextLetter[i]);
        
        //once we are at the end, we know that we can delete without worrying
        delete subtree;
    }
}

