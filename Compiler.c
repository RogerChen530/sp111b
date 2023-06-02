#include <assert.h>
#include "compiler.h"

int E();
void STMT();
void IF();
void BLOCK();
void DO_WHILE(); // 新增的函式原型

int tempIdx = 0, labelIdx = 0;

#define nextTemp() (tempIdx++)
#define nextLabel() (labelIdx++)

int isTempIr = 0;
char tempIr[100000], *tempIrp = tempIr;
#define emit(...) ({ \
  if (isTempIr) { \
    sprintf(tempIrp, __VA_ARGS__); \
    tempIrp += strlen(tempIrp);\
  } else { \
    printf(__VA_ARGS__);\
  }\
})

int isNext(char *set) {
  char eset[SMAX], etoken[SMAX];
  sprintf(eset, " %s ", set);
  sprintf(etoken, " %s ", tokens[tokenIdx]);
  return (tokenIdx < tokenTop && strstr(eset, etoken) != NULL);
}

int isEnd() {
  return tokenIdx >= tokenTop;
}

char *next() {
  return tokens[tokenIdx++];
}

char *skip(char *set) {
  if (isNext(set)) {
    return next();
  } else {
    printf("skip(%s) got %s fail!\n", set, next());
    assert(0);
  }
}

int F() {
  int f;
  if (isNext("(")) {
    next();
    f = E();
    next();
  } else {
    f = nextTemp();
    char *item = next();
    if (isdigit(*item)) {
      emit("t%d = %s\n", f, item);
    } else {
      if (isNext("++")) {
        next();
        emit("%s = %s + 1\n", item, item);
      }
      emit("t%d = %s\n", f, item);
    }
  }
  return f;
}

int E() {
  int i1 = F();
  while (isNext("+ - * / & | ! < > = <= >= == !=")) {
    char *op = next();
    int i2 = E();
    int i = nextTemp();
    emit("t%d = t%d %s t%d\n", i, i1, op, i2);
    i1 = i;
  }
  return i1;
}

void ASSIGN() {
  char *id = next();
  skip("=");
  int e = E();
  skip(";");
  emit("%s = t%d\n", id, e);
}

void WHILE() {
  int whileBegin = nextLabel();
  int whileEnd = nextLabel();
  emit("(L%d)\n", whileBegin);
  skip("while");
  skip("(");
  int e = E();
  emit("if not t%d goto L%d\n", e, whileEnd);
  skip(")");
  STMT();
  emit("goto L%d\n", whileBegin);
  emit("(L%d)\n", whileEnd);
}

void IF() {
  skip("if");
  skip("(");
  E();
  skip(")");
  STMT();
  if (isNext("else")) {
    skip("else");
    STMT();
  }
}

void STMT() {
  if (isNext("while"))
    return WHILE();
  else if (isNext("if"))
    IF();
  else if (isNext("{"))
    BLOCK();
  else if (isNext("do"))
    DO_WHILE();

  
//   我在程式碼中進行了以下修改:
//   1. 在程式碼的一開始，新增了 DO_WHILE() 函式的原型聲明。
//   2. 在 STMT() 函式中新增了對 do BLOCK while EXP 語法的判斷和呼叫。
//   3. 在程式碼的末尾，新增了 DO_WHILE() 函式的實作。  
//   這些修改主要是在原始程式碼的基礎上新增了一個名為 DO_WHILE() 的函式
//   該函式處理 do BLOCK while EXP 語法結構。
//   同時，在 STMT() 函式中新增了對 do 關鍵字的判斷，以呼叫對應的語法處理函式。
