#include "gol.h"

void InitMap(char map[][X][Y]) {
  for (int x = 0; x < X; ++x) {
    for (int y = 0; y < Y; ++y) {
      map[0][x][y] = ' ';
      map[1][x][y] = 0;
    }
  }
}

void PrintMap(char map[][X][Y]) {
  for (int x = 0; x < X; ++x) {
    for (int y = 0; y < Y; ++y) {
      printw("%c", map[0][x][y]);
    }
    printw("\n");
  }
}

void Pause(char map[][X][Y]) {
  char c = '1';
  while (c != ' ') {
    timeout(10000);
    clear();
    PrintMap(map);
    printw("Pause - space, speed+ - x, speed- - z, speed: PAUSE");
    c = getch();
  }
}

void MapNull(char map[][X][Y]) {
  for (int x = 0; x < X; ++x) {
    for (int y = 0; y < Y; ++y) {
      map[1][x][y] = 0;
    }
  }
}

void RuleGameForLife(char map[][X][Y]) {
  for (int x = 0; x < X; ++x) {
    for (int y = 0; y < Y; ++y) {
      if (map[1][x][y] == 3) map[0][x][y] = 'X';
      if (!(map[1][x][y] == 2 || map[1][x][y] == 3)) map[0][x][y] = ' ';
    }
  }
  MapNull(map);
  for (int x = 0; x < X; ++x) {
    for (int y = 0; y < Y; ++y) {
      if (map[0][x][y] == 'X') {
        if (x < X - 1 && y < Y - 1) ++map[1][x + 1][y + 1];
        if (x < X - 1) ++map[1][x + 1][y];
        if (x < X - 1 && y > 0) ++map[1][x + 1][y - 1];
        if (y < Y - 1) ++map[1][x][y + 1];
        if (y > 0) ++map[1][x][y - 1];
        if (x > 0 && y < Y - 1) ++map[1][x - 1][y + 1];
        if (x > 0) ++map[1][x - 1][y];
        if (x > 0 && y > 0) ++map[1][x - 1][y - 1];
      }
    }
  }
}

void StartPos(char map[][X][Y]) {
  map[0][1][25] = 'X';  // 1
  map[0][2][23] = 'X';  // 2
  map[0][2][25] = 'X';  // 3
  map[0][3][13] = 'X';  // 4
  map[0][3][14] = 'X';  // 5
  map[0][3][21] = 'X';  // 6
  map[0][3][22] = 'X';  // 7
  map[0][3][35] = 'X';  // 8
  map[0][3][36] = 'X';  // 9
  map[0][4][12] = 'X';  // 10
  map[0][4][16] = 'X';  // 11
  map[0][4][21] = 'X';  // 12
  map[0][4][22] = 'X';  // 13
  map[0][4][35] = 'X';  // 14
  map[0][4][36] = 'X';  // 15
  map[0][5][1] = 'X';   // 16
  map[0][5][2] = 'X';   // 17
  map[0][5][11] = 'X';  // 18
  map[0][5][17] = 'X';  // 19
  map[0][5][21] = 'X';  // 20
  map[0][5][22] = 'X';  // 21
  map[0][6][1] = 'X';   // 22
  map[0][6][2] = 'X';   // 23
  map[0][6][11] = 'X';  // 24
  map[0][6][15] = 'X';  // 25
  map[0][6][17] = 'X';  // 26
  map[0][6][18] = 'X';  // 27
  map[0][6][23] = 'X';  // 28
  map[0][6][25] = 'X';  // 29
  map[0][7][11] = 'X';  // 30
  map[0][7][17] = 'X';  // 31
  map[0][7][25] = 'X';  // 32
  map[0][8][12] = 'X';  // 33
  map[0][8][16] = 'X';  // 34
  map[0][9][13] = 'X';  // 35
  map[0][9][14] = 'X';  // 36

  map[0][5][47] = 'X';
  map[0][6][46] = 'X';
  map[0][6][47] = 'X';
  map[0][6][48] = 'X';
  map[0][7][47] = 'X';

  for (int x = 0; x < X; ++x) {
    for (int y = 0; y < Y; ++y) {
      if (map[0][x][y] == 'X') {
        if (x < X - 1 && y < Y - 1) ++map[1][x + 1][y + 1];
        if (x < X - 1) ++map[1][x + 1][y];
        if (x < X - 1 && y > 0) ++map[1][x + 1][y - 1];
        if (y < Y - 1) ++map[1][x][y + 1];
        if (y > 0) ++map[1][x][y - 1];
        if (x > 0 && y < Y - 1) ++map[1][x - 1][y + 1];
        if (x > 0) ++map[1][x - 1][y];
        if (x > 0 && y > 0) ++map[1][x - 1][y - 1];
      }
    }
  }
}