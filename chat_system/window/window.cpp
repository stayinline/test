#include "window.h"

window::window()
{
	initscr();
}

void window::drawHeader()
{
	int y = 0;
	int x = 0;
	int h = LINES/5;
	int w = COLS;
	header = newwin(h, w, y, x);
	box(header, 0, 0);
}

void window::drawOutput()
{
	int y = LINES/5;
	int x = 0;
	int h = (LINES*3)/5;
	int w = (COLS*3)/4;
	output = newwin(h, w, y, x);
	box(output, 0, 0);
}
void window::drawFlist()
{
	int y = LINES/5;
	int x = (COLS*3)/4;
	int h = (LINES*3)/5;
	int w = COLS/4;
	flist = newwin(h, w, y, x);
	box(flist, 0, 0);
}
void window::drawInput()
{
	int y = (LINES*4)/5;
	int x = 0;
	int h = LINES/5;
	int w = COLS;
	input = newwin(h, w, y, x);
	box(input, 0, 0);
}

void window::putStringToWin(WINDOW *w, int y, int x, std::string &msg)
{
	mvwaddstr(w, y, x, msg.c_str());
}

void window::getStringFromWin(WINDOW *w, std::string &outString)
{
	char buf[1024];
	wgetnstr(w, buf, sizeof(buf) - 1);
	outString = buf;
}

void window::clearWinLines(WINDOW *w, int start, int nums)
{
	while(nums-- > 0){
		wmove(w, start++, 0);
		wclrtoeol(w);
	}
}

window::~window()
{
	delwin(header);
	delwin(output);
	delwin(flist);
	delwin(input);
	endwin();
}



