#include "gol.h"

int main() {
  char map[Z][X][Y];
  int command = '1';
  int tout = 200;
  float t = (float)tout / 1000;
  initscr();
  InitMap(map);
  StartPos(map);
  noecho();
  curs_set(0);
  int count = 5;
  while (true) {
    timeout(900);
    clear();
    printw("Start for: %d sec.", count);
    if (count == 0) break;
    --count;

    getch();
  }
  while (command != 'q') {
    clear();
    RuleGameForLife(map);
    PrintMap(map);
    t = (float)tout / 1000;
    printw("Pause - space, speed+ - x, speed- - z, speed: %.2f sec.", t);
    command = getch();
    if (command == ' ') Pause(map);
    if (command == '+') tout -= 10;
    if (command == '-') tout += 10;
    timeout(tout);
  }
  endwin();
}