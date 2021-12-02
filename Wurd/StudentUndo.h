#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"
#include <stack>
using namespace std;

class StudentUndo : public Undo {
public:

	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();

private:
    
    //creating our struct with all the info needed
    struct charUndo
    {
        string character;
        int m_row;
        int m_col;
        Undo::Action act;
    };
    
    //creating the stack
    stack<charUndo> undoStack;
};

#endif // STUDENTUNDO_H_
