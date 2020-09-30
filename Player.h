#ifndef PAYER_H_INCLUDED
#define PAYER_H_INCLUDED

#include <ncursesw/curses.h>

class Player
{
    int xFood, yFood;
    int xTail[1000], yTail[1000];
    int nTail;
    int xLoc, yLoc, xMax, yMax;
    char snake_head, snake_food;
    WINDOW *curwin;
    bool game_over, exit_game;
    int snake_speed;
    int score;

public:
    void setup(WINDOW *win, int y, int x, char c);

    void mvup();
    void mvdown();
    void mvright();
    void mvleft();
    void getmv();
    void display();
    void quit();

    void moving();
    enum eDirection {STOP = 0, RIGHT, LEFT, UP, DOWN};
    eDirection dir;

    int getScore();
    int getSpeed();
    void checkSpeed();
    bool isOver();
    void playAgain(bool& exit);
};


#endif // PAYER_H_INCLUDED
