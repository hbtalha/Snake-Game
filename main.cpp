#include <ncursesw/curses.h>

#include "Player.h"

void printScore(int xMax, int score)
{
    // comment out this line and play until the score is 100 and
    // then lose the game to start again and you will see in the
    // score the reason for the line below
    mvprintw(2, xMax + 7, "Score:        ");

    mvprintw(2, xMax + 7, "Score: %d", score);
    mvprintw(4, xMax + 7, "p/P -> Pause");
    mvprintw(6, xMax + 7, "q/Q -> Quit");

    refresh();
}

int main()
{
    initscr();
    noecho();
    cbreak();

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW *playwin = newwin(yMax - 3, xMax - 20, 1, 7);
    box(playwin, 0, 0);
    refresh();
    wrefresh(playwin);

    getmaxyx(playwin, yMax, xMax);

    int x = xMax; // 100
    int y = yMax; // 27
    curs_set(0); // make the blinking cursor invisible

    bool exit = false;

    nodelay(playwin, false);
    Player p;

    do
    {
        p.setup(playwin, yMax/2, xMax/2, 'o');

        while( ! p.isOver() )
        {
            printScore(xMax, p.getScore());
            p.display();
            wrefresh(playwin);
            p.getmv();
            p.moving();
            p.checkSpeed();

            napms(p.getSpeed());
        }

        p.playAgain(exit);
    }
    while ( ! exit);

    endwin();

    return 0;
}
