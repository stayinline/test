
#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>
#include <string>
#include <ncurses.h>
#include <string.h>

class window{
	public:
		window();
		void putStringToWin(WINDOW *w, int y, int x, std::string &msg);
		void getStringFromWin(WINDOW *w, std::string &outString);
		void clearWinLines(WINDOW *w, int start, int nums);
		void drawHeader();
		void drawOutput();
		void drawInput();
		void drawFlist();
		~window();
	public:
		WINDOW *header;
		WINDOW *output;
		WINDOW *flist;
		WINDOW *input;
};

#endif
