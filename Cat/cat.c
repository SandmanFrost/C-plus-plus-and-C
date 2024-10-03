#include "cat.h"

static const char *sp_symbol[255] = {
    [0] = "^@",      [1] = "^A",     [2] = "^B",     [3] = "^C",
    [4] = "^D",      [5] = "^E",     [6] = "^F",     [7] = "^G",
    [8] = "^H",      [9] = "^I",     [10] = "\n",    [11] = "^K",
    [12] = "^L",     [13] = "^M",    [14] = "^N",    [15] = "^O",
    [16] = "^P",     [17] = "^Q",    [18] = "^R",    [19] = "^S",
    [20] = "^T",     [21] = "^U",    [22] = "^V",    [23] = "^W",
    [24] = "^X",     [25] = "^Y",    [26] = "^Z",    [27] = "^[",
    [28] = "^\\",    [29] = "^]",    [30] = "^^",    [31] = "^_",
    [32] = " ",      [33] = "!",     [34] = "\"",    [35] = "#",
    [36] = "$",      [37] = "%",     [38] = "&",     [39] = "'",
    [40] = "(",      [41] = ")",     [42] = "*",     [43] = "+",
    [44] = ",",      [45] = "-",     [46] = ".",     [47] = "/",
    [48] = "0",      [49] = "1",     [50] = "2",     [51] = "3",
    [52] = "4",      [53] = "5",     [54] = "6",     [55] = "7",
    [56] = "8",      [57] = "9",     [58] = ":",     [59] = ";",
    [60] = "<",      [61] = "=",     [62] = ">",     [63] = "?",
    [64] = "@",      [65] = "A",     [66] = "B",     [67] = "C",
    [68] = "D",      [69] = "E",     [70] = "F",     [71] = "G",
    [72] = "H",      [73] = "I",     [74] = "J",     [75] = "K",
    [76] = "L",      [77] = "M",     [78] = "N",     [79] = "O",
    [80] = "P",      [81] = "Q",     [82] = "R",     [83] = "S",
    [84] = "T",      [85] = "U",     [86] = "V",     [87] = "W",
    [88] = "X",      [89] = "Y",     [90] = "Z",     [91] = "[",
    [92] = "\\",     [93] = "]",     [94] = "^",     [95] = "_",
    [96] = "`",      [97] = "a",     [98] = "b",     [99] = "c",
    [100] = "d",     [101] = "e",    [102] = "f",    [103] = "g",
    [104] = "h",     [105] = "i",    [106] = "j",    [107] = "k",
    [108] = "l",     [109] = "m",    [110] = "n",    [111] = "o",
    [112] = "p",     [113] = "q",    [114] = "r",    [115] = "s",
    [116] = "t",     [117] = "u",    [118] = "v",    [119] = "w",
    [120] = "x",     [121] = "y",    [122] = "z",    [123] = "{",
    [124] = "|",     [125] = "}",    [126] = "~",    [127] = "^?",
    [128] = "M-^@",  [129] = "M-^A", [130] = "M-^B", [131] = "M-^C",
    [132] = "M-^D",  [133] = "M-^E", [134] = "M-^F", [135] = "M-^G",
    [136] = "M-^H",  [137] = "M-^I", [138] = "M-^J", [139] = "M-^K",
    [140] = "M-^L",  [141] = "M-^M", [142] = "M-^N", [143] = "M-^O",
    [144] = "M-^P",  [145] = "M-^Q", [146] = "M-^R", [147] = "M-^S",
    [148] = "M-^T",  [149] = "M-^U", [150] = "M-^V", [151] = "M-^W",
    [152] = "M-^X",  [153] = "M-^Y", [154] = "M-^Z", [155] = "M-^[",
    [156] = "M-^\\", [157] = "M-^]", [158] = "M-^^", [159] = "M-^_"};

void cat(int argc, char **argv) {
  FlagType flags;
  FlagsInit(&flags);
  int Error = 0;
  int i = 1;
  for (; i < argc; ++i) {
    if (strspn(argv[i], "-") == 0) break;
    if (Flags(argv[i], &flags) == 1) Error = 1;
  }
  if (flags.b == 1) flags.n = 0;
  if (!Error && i == argc) {
    printf("cat: please input filename...\n");
  }
  for (; !Error && i < argc; ++i) {
    FILE *f = fopen(argv[i], "r");
    if (f) {
      int LineStart = 1;
      int LineEmpety = 1;
      char Pred = '\n';
      char Next = ' ';
      char ch[1000] = {'\0'};
      while ((*ch = fgetc(f)) != EOF) {
        if ((flags.s == 1) && Flag_s(Pred, Next, *ch)) continue;
        if (flags.n == 1) LineStart = Flag_n(LineStart, &LineEmpety, *ch);
        if (flags.b == 1) Flag_b(Pred, &LineEmpety, *ch);
        if (flags.E == 1) Flag_E(*ch);
        if (flags.v == 1) Flag_v(ch);
        Next = Pred;
        Pred = *ch;
        if ((flags.T == 1) && Flag_T(*ch)) continue;
        if (*ch == '\0') {
          fputc(*ch, stdout);
        } else {
          fputs(ch, stdout);
        }
        memset(ch, '\0', 1000);
      }
      fclose(f);
    } else {
      printf("cat: Can't open '%s': No such file or directory!\n", argv[i]);
    }
  }
  if (Error)
    printf(
        "cat: illegal option -- '%s'.\nTry 'man cat' for more information.\n",
        argv[i - 1]);
}

int FlagsInit(FlagType *flags) {
  flags->b = 0;
  flags->e = 0;
  flags->E = 0;
  flags->v = 0;
  flags->n = 0;
  flags->s = 0;
  flags->t = 0;
  flags->T = 0;
  return 1;
}

int Flags(char *argv, FlagType *flags) {
  int ret = 0;
  if (strlen(argv) == 1 || strlen(argv) != strspn(argv, "-bEensTtv")) ret = 1;
  if (!strcmp(argv, "--number")) {
    ret = 0;
    flags->n = 1;
  } else {
    if (!strcmp(argv, "--number-nonblank")) {
      ret = 0;
      flags->b = 1;
    } else {
      if (!strcmp(argv, "--squeeze-blank")) {
        ret = 0;
        flags->s = 1;
      } else {
        if (strchr(argv, 'b')) flags->b = 1;
        if (strchr(argv, 'E')) flags->E = 1;
        if (strchr(argv, 'e')) {
          flags->v = 1;
          flags->E = 1;
        }
        if (strchr(argv, 'n')) flags->n = 1;
        if (strchr(argv, 's')) flags->s = 1;
        if (strchr(argv, 'T')) flags->T = 1;
        if (strchr(argv, 't')) {
          flags->v = 1;
          flags->T = 1;
        }
        if (strchr(argv, 'v')) flags->v = 1;
      }
    }
  }
  return ret;
}

int Flag_b(char Pred, int *LineEmpety, char ch) {
  if (Pred == '\n' && ch != '\n') printf("%6d\t", (*LineEmpety)++);
  return ch == '\n';
}

int Flag_E(char ch) {
  if (ch == '\n') printf("$");
  return ch == '\n';
}

int Flag_n(int LineStart, int *Line, char ch) {
  if (LineStart) printf("%6d\t", (*Line)++);
  return ch == '\n';
}

int Flag_s(char Pred, char Next, char ch) {
  return ch == '\n' && Pred == '\n' && Next == '\n';
}

int Flag_T(char ch) {
  int t = 0;
  if (ch == '\t') t = printf("^I");
  return t;
}

int Flag_v(char *ch) {
  unsigned char tmp = *ch;
  if ((tmp <= 8) || (tmp >= 11 && tmp <= 31) || (tmp >= 127 && tmp <= 159))
    strcpy(ch, sp_symbol[tmp]);
  return tmp;
}
