#include "StudentUndo.h"
#include <stack>

Undo* createUndo()
{
    
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
    
    
    //create a temp undo to add to our stack with all th einformeation that is given to us by the submit
    charUndo temp;
    temp.m_row = row;
    temp.m_col = col;
    temp.act = action;
    temp.character = ch;
    
    
    //this next bit is all for batching
    //make sure we don't access top when stack is empty
    if(!undoStack.empty())
    {
    
    switch(action)
      {
              
              //batching for delete submits
          case DELETE:
          {
              
              //check whetehr the deletion all happened at the same spot (checking for consecutive occurences of delete key)
                  if(undoStack.top().m_row == temp.m_row && undoStack.top().m_col == temp.m_col && temp.act == undoStack.top().act)
                  {
                      //if consecutive occurences are found set our varibales accordingly, batch together, and remove the unbatched stack bit
                      temp.character = undoStack.top().character + temp.character;
                      undoStack.pop();
                  }
              //check whetehr the deletion all happened at a sequence of spots (checking for consecutive occurences of backspace key)
                else if(undoStack.top().m_row == row && undoStack.top().m_col == (temp.m_col+1) && temp.act == undoStack.top().act)
                {
                    //if consecutive occurences are found set our varibales accordingly, batch together, and remove the unbatched stack bit
                    temp.character =  temp.character + undoStack.top().character;
                    
                    undoStack.pop();
                }

              break;
          }
               //batching for insert submits
          case INSERT:
          {
              //check whetehr the insertion all happened at a sequence of spots (checking for consecutive occurences of insertions)
              if(undoStack.top().m_row == row && undoStack.top().m_col == (temp.m_col-undoStack.top().character.length()) && temp.act == undoStack.top().act)
                  {
                      //if consecutive occurences are found set our varibales accordingly, batch together, and remove the unbatched stack bit
                      temp.character = undoStack.top().character + temp.character;
                      temp.m_col = undoStack.top().m_col;
                      undoStack.pop();
                  }
              
              break;
          }
          default:
              break;
          
      }
    }
    
    //push our either batched or unbatched undo to the stack
    undoStack.push(temp);
    
    
}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
    
    //make sure we aren't accessing empty stacks
    if(undoStack.empty())
        return Undo::Action::ERROR;
    
    
    //this sets our variables to the most recent function (the top of the stack)
    row = undoStack.top().m_row;
    col = undoStack.top().m_col;
    count = undoStack.top().character.length();
    text = undoStack.top().character;
    
    
    Action temp =undoStack.top().act;
    
    //once all of our variables are set and we have what we need to return, we remove the current undo from the stack to prevent multiple undos of the same thing
    undoStack.pop();
    
    switch(temp)
    {
            //if the action was an insert, we need to return a delete for our studtexteditor to know to delete
        case INSERT:
            col--;
            return Action::DELETE;
            
            //if the action was a delete, we need to return an insert for our studtexteditor to know to insert
        case DELETE:
            count = 1;
            return Action::INSERT;
            
            //if the action was a join, we need to return a split for our studtexteditor to know to split
        case JOIN:
            count = 1;
            return Action::SPLIT;
            //if the action was a split, we need to return an join for our studtexteditor to know to join
        case SPLIT:
            count =1;
            return Action::JOIN;
        default:
            return Action::ERROR;
            
    }
    

}

void StudentUndo::clear() {
    //goes through stack until its empty and pops eveyrthign
    while(!undoStack.empty())
        undoStack.pop();
    
}
