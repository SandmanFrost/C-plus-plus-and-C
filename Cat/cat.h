#ifndef CAT_H
#define CAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int b, E, e, n, s, t, T, v;
} FlagType;

void cat(int argc, char **argv);
int FlagsInit(FlagType *flags);
int Flags(char *argv, FlagType *flags);
int Flag_b(char Pred, int *LineEmpety, char ch);
int Flag_E(char ch);
int Flag_n(int LineStart, int *Line, char ch);
int Flag_s(char Pred, char Next, char ch);
int Flag_T(char ch);
int Flag_v(char *ch);

#endif