#ifndef _GOL_H_
#define _GOL_H_

#include <ncursesw/ncurses.h>

// Высота
#define X 40
// Ширина
#define Y 100
// Слои для соседсва (0 - видит пользователь, 1 - скрытый слой)
#define Z 2

void InitMap(char map[][X][Y]);
void PrintMap(char map[][X][Y]);
void Pause(char map[][X][Y]);
void RuleGameForLife(char map[][X][Y]);
void MapNull(char map[][X][Y]);
void RuleGameForLife(char map[][X][Y]);
void StartPos(char map[][X][Y]);

#endif