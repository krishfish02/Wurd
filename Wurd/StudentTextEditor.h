#ifndef STUDENTTEXTEDITOR_H_
#define STUDENTTEXTEDITOR_H_

#include "TextEditor.h"
#include <list>
using namespace std;

class Undo;

class StudentTextEditor : public TextEditor {
public:

	StudentTextEditor(Undo* undo);
	~StudentTextEditor();
	bool load(std::string file);
	bool save(std::string file);
	void reset();
	void move(Dir dir);
	void del();
	void backspace();
	void insert(char ch);
	void enter();
	void getPos(int& row, int& col) const;
	int getLines(int startRow, int numRows, std::vector<std::string>& lines) const;
	void undo();

private:
    //private variables
    //our document of text
    list<string> m_lines;
    //iterator to move between lines
    list<string>::iterator m_iter;
    //cursor position/relative rows
    int m_currRow;
    int m_currCol;
    int maxRows;
    //to check for undo calls vs keyboard calls
    bool shouldISumbit;
};

#endif // STUDENTTEXTEDITOR_H_
