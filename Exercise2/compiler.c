#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "compiler.h"

int E();
void STMT();
void IF();
void BLOCK();

int tempIdx = 0, labelIdx = 0;

#define nextTemp() (tempIdx++)
#define nextLabel() (labelIdx++)
#define emit printf

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
  emit("if not T%d goto L%d\n", e, whileEnd);
  skip(")");
  STMT();
  emit("goto L%d\n", whileBegin);
  emit("(L%d)\n", whileEnd);
}

void IF() {
  skip("if");
  skip("(");
  int e = E();
  emit("if not T%d goto L%d\n", e, labelIdx++);
  skip(")");
  STMT();
  if (isNext("else")) {
    skip("else");
    STMT();
  }
}

void DOWHILE() {
  int dowhileBegin = nextLabel();
  emit("(L%d)\n", dowhileBegin);
  skip("do");
  STMT();
  skip("while");
  skip("(");
  int e = E();
  emit("if T%d goto L%d\n", e, dowhileBegin);
  skip(")");
}

void STMT() {
  if (isNext("while"))
    WHILE();
  else if (isNext("if"))
    IF();
  else if (isNext("{"))
    BLOCK();
  else
    ASSIGN();
}

void STMTS() {
  while (!isEnd() && !isNext("}")) {
    STMT();
  }
}

void BLOCK() {
  skip("{");
  STMTS();
  skip("}");
}

void PROG() {
  STMTS();
}

void parse() {
  printf("============ parse =============\n");
  tokenIdx = 0;
  PROG();
}
