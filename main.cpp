#include <ncursesw/curses.h>
#include <windows.h>

#include "Player.h"

int main()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);

    Player p;

    bool exit = false;

    p.setup();

    do
    {
        p.startNewGame();

        while( ! p.isOver() )
        {
            p.display();
            p.getmv();
            p.printScore();
            p.moving();
            p.checkSpeed();

            // controls the speed of the game
            napms(p.getSpeed());
        }

        p.playAgain(exit);
    }
    while ( ! exit);

    endwin();

    return 0;
}
