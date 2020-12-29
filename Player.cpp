//#include <ncursesw/curses.h>
#include <windows.h>
#include <ctime>
#include <string>
//#include <simple_wrappers.h>
#include "Player.h"

//using namespace wrap;
using std::string;

void Player::setup()
{
    srand(time(nullptr));

    initscr();
    noecho();
    cbreak();

    getmaxyx(stdscr, yMax, xMax);

    playwin = newwin(yMax - 3, xMax - 20, 1, 7);
    box(playwin, 0, 0);
    refresh();
    wrefresh(playwin);

    getmaxyx(playwin, yMax, xMax);

    keypad(playwin, true);
    nodelay(playwin, true);

    curs_set(0); // make the blinking cursor invisible
}

void Player::startNewGame()
{
    yLoc = yMax/2;
    xLoc = xMax/2;
    snake_head = 'o';

    int r_min = 1;

    yFood = rand() % (yMax - 2) + r_min;
    xFood = rand() % (xMax - 2) + r_min;

    nTail = 0;
    dir = STOP;

    score = 0;
    snake_speed = 100;

    game_over = false;
    exit_game = false;
}

int Player::getSpeed()
{
    return snake_speed;
}

bool Player::isOver()
{
    return game_over;
}

void Player::printScore()
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

void Player::checkSpeed()
{
    switch(score)
    {
    case 100:
        snake_speed = 85;
        break;

    case 300:
        snake_speed = 70;
        break;

    case 350:
        snake_speed = 60;
        break;

    case 500:
        snake_speed = 50;
        break;
    }
}

void Player::mvup()
{
    // if (!game_over) // if the snake hits the wall or eats its its own body the head will keep showing up
    mvwaddch(playwin, yLoc, xLoc, ' ');
    --yLoc;

//   if (yLoc < 1)
    //     yLoc = yMax - 2;
}

void Player::mvdown()
{
    mvwaddch(playwin, yLoc, xLoc, ' ');
    ++yLoc;

    //  if (yLoc > (yMax - 2))
    //    yLoc = 1;
}

void Player::mvright()
{
    mvwaddch(playwin, yLoc, xLoc, ' ');
    ++xLoc;

    //  if (xLoc > (xMax - 2))
    //     xLoc = 1;
}

void Player::mvleft()
{
    mvwaddch(playwin, yLoc, xLoc, ' ');
    --xLoc;

    // if (xLoc < 1)
    //    xLoc = xMax - 2;
}

void Player::getmv()
{
    int choice = wgetch(playwin);

    switch(choice)
    {
    case 52: // number pad 4
    case 'a':
    case KEY_LEFT:
        if (nTail == 0)
            dir = LEFT;
        else if(dir == RIGHT && nTail > 1)
            dir = RIGHT;
        else if (dir != RIGHT)
            dir = LEFT;
        break;

    case 54: // number pad 6
    case 'd':
    case KEY_RIGHT:
        if (nTail == 0)
            dir = RIGHT;
        else if(dir == LEFT && nTail > 1)
            dir = LEFT;
        else if (dir != LEFT)
            dir = RIGHT;
        break;

    case 50: // number pad 2
    case 's':
    case KEY_DOWN:
        if (nTail == 0)
            dir = DOWN;
        else if(dir == UP && nTail > 1)
            dir = UP;
        else if (dir != UP)
            dir = DOWN;
        break;

    case 56: // number pad 8
    case 'w':
    case KEY_UP:
        if (nTail == 0)
            dir = UP;
        else if(dir == DOWN && nTail > 1)
            dir = DOWN;
        else if (dir != DOWN)
            dir = UP;
        break;

    case ' ':
    case 'P':
    case 'p':
        wattron(playwin, A_REVERSE);
        mvwprintw(playwin, yMax/2 - 5, xMax/2 - 6, " PAUSE ");
        wattroff(playwin, A_REVERSE);
        nodelay(playwin, false);

        char c;
        do
        {
            c = wgetch(playwin);
        }
        while (c != 'p' && c != 'P' && c != ' ');

        nodelay(playwin, true);
        break;

    case 'q':
    case 'Q':
        quit();
        break;
    }
}

void Player::moving()
{
    int prevX = xTail[0];
    int prevY = yTail[0];
    int prev2X, prev2Y;
    xTail[0] = xLoc;
    yTail[0] = yLoc;

    for (int i = 1; i < nTail; i++)
    {
        prev2X = xTail[i];
        prev2Y = yTail[i];
        xTail[i] = prevX;
        yTail[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch(dir)
    {
    case LEFT:
        mvleft();
        break;

    case RIGHT:
        mvright();
        break;

    case UP:
        mvup();
        break;

    case DOWN:
        mvdown();
        break;
    }

    // if the snake eats the food
    if (xLoc == xFood && yLoc == yFood)
    {
        int xr_max = xMax - 2; // x random max
        int yr_max = yMax - 2; // y random max
        int r_min = 1;

        //    yFood = gen_random_number(r_min, yr_max);
        //  xFood = gen_random_number(r_min, xr_max);

        yFood = rand() % yr_max + r_min;
        xFood = rand() % xr_max + r_min;

        if (yFood == 0 || yFood == (yMax -1) || xFood == 0 || xFood == (xMax - 1))
            beep();

        /*
                // Beep(1080, 90);
                yFood = r_min + rand() % ( ( yr_max + 1) - r_min);
                xFood = r_min + rand() % ( ( xr_max + 1) - r_min);
        */

        ++nTail;
        score += 10;
    }

    // checks if the snake hits the wall
    if(xLoc == 0 || xLoc == xMax - 1 || yLoc == 0 || yLoc == yMax - 1)
    {
        game_over = true;
    }

    // check if the snake eats its own body
    for(int i = 0; i < nTail; ++i)
        if(xTail[i] == xLoc && yTail[i] == yLoc)
        {
            game_over = true;
            break;
        }
}

void Player::display()
{
    werase(playwin);
    box(playwin, 0, 0);

    mvwaddch(playwin, yLoc, xLoc, snake_head);
    mvwaddch(playwin, yFood, xFood, '*');

    for (int x = 0; x < nTail; ++x)
    {
        mvwaddch(playwin, yTail[ x ], xTail[ x ], 'o');
    }

    wrefresh(playwin);
}

void Player::quit()
{
    wattron(playwin, A_BOLD);
    mvwprintw(playwin, yMax/2 - 5, xMax/2 - 6, "---Quit---");
    wattroff(playwin, A_BOLD);
    nodelay(playwin, false);

    string choices [] = {" Yes ", " No "};
    int choice;
    int selected = 0;

    while(true)
    {
        int len = 2;

        for(int i = 0; i < len; ++i)
        {
            mvwprintw(playwin, yMax/2 - 3 + i, xMax/2 - 6, "->  " );

            if(i == selected)
                wattron(playwin, A_REVERSE);

            mvwprintw(playwin, yMax/2 - 3 + i, xMax/2 - 3, choices[ i ].c_str() );
            wattroff(playwin, A_REVERSE);
        }

        choice = wgetch(playwin);

        if(choice == 10 || choice == ' ')
            break;

        switch(choice)
        {
        case KEY_UP:
            --selected;

            if(selected < 0)
                selected = len - 1;
            break;

        case KEY_DOWN:
            ++selected;

            if(selected > (len -1))
                selected = 0;
            break;
        }
    }

    if (selected == 1)
        game_over = true;

    nodelay(playwin, true);
}

void Player::playAgain(bool& exit)
{
    Sleep(100);

    wattron(playwin, A_BOLD);
    mvwprintw(playwin, yMax/2 - 5, xMax/2 - 6, "GAME OVER");
    wattroff(playwin, A_BOLD);

    string choices [] = {" -Play Again ", " -Exit       "};
    int choice;
    int selected = 0;

    while ( 1 )
    {
        for (int i = 0; i < 2; ++i)
        {
            if(i == selected)
                wattron(playwin, A_REVERSE);

            mvwprintw(playwin, yMax/2 - 4 + i + 1, xMax/2 - 8, choices[i].c_str());
            wattroff(playwin, A_REVERSE);
        }

        choice = wgetch(playwin);

        if(choice == 10 || choice == ' ')
            break;

        switch(choice)
        {
        case KEY_UP:
            --selected;

            if (selected == -1)
                selected = 1;
            break;

        case KEY_DOWN:
            ++selected;

            if (selected == 2)
                selected = 0;
            break;
        }
    }

    if (selected == 1)
        exit = true;
}
