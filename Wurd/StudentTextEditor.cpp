#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

TextEditor* createTextEditor(Undo* un)
{
	return new StudentTextEditor(un);
}

//initilaize all our our variables to mark the beginning of a text document
StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo) {
     
     m_currRow =0;
     m_currCol = 0;
     maxRows = 1;
     m_lines.push_back("");
     m_iter = m_lines.begin();
     shouldISumbit = true;
     
     
}

StudentTextEditor::~StudentTextEditor()
{
}


bool StudentTextEditor::load(std::string file) {
	
    
    //if file couldnt be loaded return false
    ifstream infile(file);
    if(!infile)
    {
        return false;
    }
    
    //if it could, we need to clear out doucment so we call reset
    if(!m_lines.empty())
        reset();
    string line;
    
    //for every line of the document
    while(getline(infile, line))
    {
        //strip out the '\r'
        for(int i = 0; i<line.length(); i++)
        {
            if(line.at(i) == '\r')
                line.erase(line.begin()+i);
        }
        //and add the line to our text document, while increasing the number of maxrows in our document at that time
        m_lines.push_back(line);
        maxRows++;
    }
    
    //once all lines have been loaded, we set our start position
    m_currCol = 0;
    m_currRow = 0;
    m_iter = m_lines.begin();
    
    //return true to show the document was loaded properly
    return true;
}

bool StudentTextEditor::save(std::string file) {
	//if file wasn't able to be saved return false
    ofstream outfile(file);
        if ( ! outfile )
        {
           
            return false;
        }
    
    //if it could be save we need an iterator to go through every line of our text document and write it out to the target document appending the '\n'
    list<string>::iterator write = m_lines.begin();
    for(;write != m_lines.end();write++)
    {
        outfile << (*write) << '\n';
    }

    
    //return true because we could overwrite and save
    return true;
}

void StudentTextEditor::reset() {
	
    //resets our document in the case of a load, resets the undo as well, and the position of our cursor
    m_lines.clear();
    m_currCol =0;
    m_currRow = 0;
    maxRows =0;
    m_iter = m_lines.begin();
    getUndo()->clear();
    shouldISumbit = true;
    
    
}

void StudentTextEditor::move(Dir dir) {

    //switch case for all the different movements
    switch(dir)
    {
            //if up but we are already at the beginning line, break and do nothing
        case UP:
            if(m_currRow == 0)
                break;
            
            //otherwise we decrease our currrow and our iterator
            else
            {
                m_currRow--;
                m_iter--;
                
                //check whether the line above us is smaller so we can position our cursor to the end of that line
                if(*m_iter == "")
                    m_currCol = 0;
                
                else if(m_currCol>(*m_iter).length())
                    m_currCol = (*m_iter).length();
                    
                
            }
                
            break;
            
            //if down but we are at the last line, break and do nothing
        case DOWN:
            if(m_currRow == maxRows-1)
            {
               
                break;
            }
            
            //otherwise increase our currrow and our iterator to go to the next line
            else
            {
                m_currRow++;
                m_iter++;
                
                //checks if next line has less characters than our previous and changes the cursor to the appropriate column in that line
                    if(*m_iter == "")
                        m_currCol = 0;
                    else if(m_currCol>(*m_iter).length())
                        m_currCol = (*m_iter).length();
            }
            break;
            
            //if left
        case LEFT:
            //and we are at the beginning of a row
            if(m_currCol == 0)
            {
                //if it is the first row do nothing
                if(m_currRow == 0)
                {
                    break;
                }
                //otherwise move our cursor up and to the end of the line
                else
                {
                    this->move(UP);
                    this->move(END);
                    break;
                }
            }
                //if we arent at the beginning of a line we can simply move one column to the left
            else
                m_currCol--;
            break;
            
            //if right
        case RIGHT:
            //and we are at the end of a line
            if(m_currCol == (*m_iter).length())
            {
                //if it is the last line, break and do nothing
                if(m_currRow == maxRows-1)
                {
                    
                    break;
                }
                //otherwise move down and to the beginning of the next line
                else
                {
                    this->move(DOWN);
                    this->move(HOME);
                    break;
                }
            }
            //if we are not at the end of th eline we can simply go right 1 column
            else
                m_currCol++;
            break;
            
            //bring the cursor to the beginning of that line
        case HOME:
            m_currCol=0;
            break;
            
            //bring the cursor to the end of that line
        case END:
            m_currCol = (*m_iter).length();
            break;
            
        default:
            break;
    }
 
}

void StudentTextEditor::del() {
//we are checking to see if the delete happens at the last positinin the text document, if so do nothing
    int checkRow = m_currRow;
    int checkCol = m_currCol;
    move(RIGHT);
    if(checkRow == m_currRow && checkCol == m_currCol)
    {
        
    }
    //otherwise we can implement this exactly how we implement backspace since we have move to the right already (assuming our RIGHT move switch case works): see commenting under backspace member function for the next bit of code's explanation (its the same)
    else
    {
        
        if(m_currCol == 0 || (*m_iter) == "")
     {
         if(m_currRow == 0)
         {
             
         }
         else
         {
             
             m_iter--;
             m_currRow--;
             m_currCol = (*m_iter).length();
             *m_iter =  (*m_iter)+(*next(m_iter));
             m_lines.erase(next(m_iter));
             maxRows--;
             
             if(shouldISumbit)
                 getUndo()->submit(Undo::Action:: JOIN, m_currRow,m_currCol);
         }
     }
     else
     {
         char ch = (*m_iter).at(m_currCol-1);
         (*m_iter).erase(m_currCol-1,1);
         
         m_currCol--;
         if(shouldISumbit)
             getUndo()->submit(Undo::Action:: DELETE, m_currRow,m_currCol, ch);
     }
        
        
    }
}

void StudentTextEditor::backspace() {
	//check whether our cursor is currently on an empty line or if it is in the beginning of a line, this calls for a merge with the previous line
       if(m_currCol == 0 || (*m_iter) == "")
    {
        //if we are in the first row we don't have to do anything
        if(m_currRow == 0)
        {
            
        }
        //if we aren't we need to merge
        else
        {
            
            //do this by first going to the line above us and setting our cursor to the very end of that line (to fit in spec)
            m_iter--;
            m_currRow--;
            m_currCol = (*m_iter).length();
            
            //merge the strings of the current line we are on and the next line (we used to be on)
            *m_iter =  (*m_iter)+(*next(m_iter));
            //erase the next line since we have already merged its contents
            m_lines.erase(next(m_iter));
            //and decrease our maxrows by one due to th eerase
            maxRows--;
            
            //submit only if our if statement lets us (this checks wheteher we are merging because of an undo operation or because we actually clicked backspace)
            if(shouldISumbit)
                getUndo()->submit(Undo::Action:: JOIN, m_currRow,m_currCol);
        }
    }
    
    //if it isn't a special case,
    else
    {
        //we can just remove the character one behind our current cursor (since we are backspacing), and set our cursor to what the spec wants
        char ch = (*m_iter).at(m_currCol-1);
        (*m_iter).erase(m_currCol-1,1);
        
        m_currCol--;
        
        //submit only if our if statement lets us (this checks wheteher we are deleting because of an undo operation or because we actually clicked backspace)
        if(shouldISumbit)
            getUndo()->submit(Undo::Action:: DELETE, m_currRow,m_currCol, ch);
    }
    
    //note our submits are at the end because spec wants us to submit after the cursor has been moved due to the function

}


//inserting a char c
void StudentTextEditor::insert(char ch) {
	
    
    //get the substirng of our line before our insertion position
    string fhalf = (*m_iter).substr(0,m_currCol);
    //get the substring after our insertion positon and merge it with the character we are inserting
    string shalf = ch + (*m_iter).substr(m_currCol);
    
    //merge both the strings together to get th emodified string
    (*m_iter) = fhalf + shalf;
   
    //move our cursor to the right one
    m_currCol++;
    
    
    //submit only if our if statement lets us (this checks wheteher we are inserting because of an undo operation or because we actually clicked a key)
    if(shouldISumbit)
        getUndo()->submit(Undo::Action::INSERT,m_currRow,m_currCol, ch);
    

    //note submit is after function due to spec
}

//splits or adds new lines
void StudentTextEditor::enter() {
	
    
    //submit only if our if statement lets us (this checks wheteher we are splitting because of an undo operation or because we actually clicked enter)
    if(shouldISumbit)
        getUndo()->submit(Undo::Action:: SPLIT, m_currRow,m_currCol);
    
    //first check whether we are at the end of a line or at an empty string
   if(m_currCol == (*m_iter).length() || (*m_iter) == "")
    {
        
        //if we are at the end of the document all we need to do is add another line to our list and position ourselves on the next line
        if(m_currRow == maxRows-1)
        {
            
            m_lines.push_back("");
            maxRows++;
            m_currCol = 0;
            m_currRow++;
            m_iter++;
           
        }
        //if we are in the middle, this means we have to insert an empty line in between two (possibly) filled lines
        else
        {
            //so we move forward, add line, move backward to get on the empty line and set our cursor position to the right spot
            m_iter++;
            m_lines.insert(m_iter,"");
            m_iter--;
            
            m_currRow++;
            m_currCol = 0;
            maxRows++;
           
        }
    }
    //this deals with splitting
    else
    {
        
        //first create the empty line needed to hold the split
        m_iter++;
        m_lines.insert(m_iter,"");
        m_iter--;
        m_iter--;
        
        
 //split the line into two strings and attach them to their respective lines, the first half to the top line and the second half to the bototm line
       (*next(m_iter)) = (*m_iter).substr(m_currCol);
       (*m_iter) = (*m_iter).substr(0,m_currCol);
        
        
        //position our cursor to where we need it, the beginning of the split on the next line
        m_iter++;
        m_currRow++;
        m_currCol = 0;
        maxRows++;
     
    }
    
    
    //note the submit in the beginnign due to spec

}


//return curren tposition of our cursor
void StudentTextEditor::getPos(int& row, int& col) const {
	row = 0; col = 0;
    

    row = m_currRow;
    col = m_currCol;
  
}



int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {

    
    //check constraints
    if(startRow <0 || numRows <0 || startRow >= maxRows)
    {
        return -1;
    }
    
    //start our iterator from the beginning
    list<string>::const_iterator temp = m_lines.begin();
    
    //move along till we get to the row we need to start on
    for(int i = 0;i<startRow;i++)
        temp++;
    
    lines.clear();
    
    //one by one add each line needed to our lines vector until we hit the numrows or the end of the document
    for(int i = 0; i<numRows && temp != m_lines.end(); i++, temp++)
    {
        lines.push_back(*temp);
    }
    
    
  //return how many lines we were able to put
    return lines.size();
}

void StudentTextEditor::undo() {
	
    //make sure our document actually has stuff to undo
    if(getUndo() == nullptr)
    {
        return;
    }
    
    //initilaize our variables we will use and get the action needed to undo
    int cool = 0;
    int row = 0;
    int col = 0;
    string ch = "";
   Undo::Action temp = getUndo()->get(m_currRow, m_currCol, cool ,ch);
    
    //if action was an error we do nothing
    if(temp == Undo::Action::ERROR)
        return;
    
    
    
    //move our iterator to the line that our action will have to be done on
    m_iter = m_lines.begin();
    for(int i = 0; i<m_currRow;i++)
        m_iter++;
    
    //switch case fo rth edifferent actions
    switch(temp)
    {
            
            //if delete,
        case Undo::Action::DELETE:
            row = m_currRow;
            col = m_currCol;
            
            //make sure we don't submit our undo changes to our undo stack resulting in a loop
            shouldISumbit = false;
            
            //how many ever characters need to be deleted, delete those at the current position
            for(int i = 0; i<cool; i++)
        {
                del();
        }
            
            //set this back to true in case we now actually use our keyboard other than undoing
            shouldISumbit = true;
            
            m_currRow = row;
            m_currCol = col;
        
            break;
            
            
            //if insert
        case Undo::Action::INSERT:
            row = m_currRow;
            col = m_currCol;
            
            //same thing as before
            
            shouldISumbit = false;
            //insert the amount of characters that need to be inserted, one by one (batching)
            for(int i = 0; i<ch.length(); i++)
                insert(ch.at(i));
            //set it back
            shouldISumbit = true;
            
            //set our cursor back to follow spec
            m_currCol = col;
            m_currRow = row;
            break;
            
            //if split
        case Undo::Action::SPLIT:
            row = m_currRow;
            col = m_currCol;
            
            //same thing as before
            
            shouldISumbit = false;
            //split where we need to be calling our enter key
            enter();
            //set back for same reason
            shouldISumbit = true;
            
            //reset cursor and iterator
            m_currRow = row;
            m_iter--;
            m_currCol = (*m_iter).length();
            break;
            
            //if join
        case Undo::Action::JOIN:
            row = m_currRow;
            col = m_currCol;
            //same hting as before
            shouldISumbit = false;
            
            //move to the right and backspace (or just a delete operaion) wasnt confident about my delete function at first so i decided to use backspace
            move(RIGHT);
            backspace();
            //reset for the same reason
            shouldISumbit = true;

        default:
            break;
    }

}
