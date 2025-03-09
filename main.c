#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int straight = 5;
int end = 8;
int tShape = 7;
int lShape = 12;

typedef struct Tile {
  int value;
  int x;
  int y;
  int size;
  int connected;
  int source;
} Tile;

int hasLeft(Tile *t) { return (t->value & 0b1000) >> 3; }

int hasRight(Tile *t) { return (t->value & 0b0010) >> 1; }

int hasUp(Tile *t) { return (t->value & 0b0100) >> 2; }

int hasDown(Tile *t) { return t->value & 0b0001; }

void DrawVerticalDashedRect(int x, int y, int width, int height) {
  for (int i = y; i < y + height; i = i + 10) {
    DrawRectangle(x, i, width, 5, BLACK);
  }
}

void DrawHorizontalDashedRect(int x, int y, int width, int height) {
  for (int i = x; i < x + width; i = i + 10) {
    DrawRectangle(i, y, 5, height, BLACK);
  }
}

void DrawTile(Tile *t) {
  DrawRectangle(t->x, t->y, t->size, t->size, BLACK);
  DrawRectangle(t->x + 5, t->y + 5, t->size - 10, t->size - 10, WHITE);
  if (hasLeft(t)) {
    if (t->connected) {
      DrawRectangle(t->x, t->y + 43, 45, 5, BLACK);
    } else {
      DrawHorizontalDashedRect(t->x, t->y + 43, 45, 5);
    }
  }
  if (hasUp(t)) {
    if (t->connected) {
      DrawRectangle(t->x + 43, t->y, 5, 45, BLACK);
    } else {
      DrawVerticalDashedRect(t->x + 43, t->y, 5, 45);
    }
  }
  if (hasRight(t)) {
    if (t->connected) {
      DrawRectangle(t->x + 43 + 5, t->y + 43, 45, 5, BLACK);
    } else {
      DrawHorizontalDashedRect(t->x + 43 + 5, t->y + 43, 45, 5);
    }
  }
  if (hasDown(t)) {
    if (t->connected) {
      DrawRectangle(t->x + 43, t->y + 43 + 5, 5, 45, BLACK);
    } else {
      DrawVerticalDashedRect(t->x + 43, t->y + 43, 5, 45);
    }
  }
}

Tile ***InitBoard(int rows, int cols) {
  Tile ***brd = malloc(rows * sizeof(Tile *));
  for (int i = 0; i < rows; i++) {
    brd[i] = malloc(cols * sizeof(Tile));
    for (int j = 0; j < cols; j++) {
      brd[i][j] = malloc(sizeof(Tile *));
    }
  }
  return brd;
}

Tile *tile(int x, int y, int size, int val, int source) {
  Tile *t = malloc(sizeof(Tile *));
  t->x = x;
  t->y = y;
  t->size = size;
  t->value = val;
  t->connected = 0;
  t->source = source;
  return t;
}

void GenerateBoard(Tile ***brd, int rows, int cols) {
  int x = 10;
  int y = 10;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      brd[i][j] = tile(x, y, 100, 0, 0);
      x += 90;
    }
    x = 10;
    y += 90;
  }
  //
  // 12 - L shape
  // 7 - T shape
  // 8 - end

  brd[1][3]->value = end;

  brd[2][1]->value = lShape;
  brd[2][2]->value = tShape;
  brd[2][3]->value = tShape;
  brd[2][4]->value = end;
  brd[2][5]->value = 0;
  brd[2][6]->value = end;
  brd[2][7]->value = tShape;
  brd[2][8]->value = tShape;
  brd[2][9]->value = end;

  brd[3][1]->value = end;
  brd[3][2]->value = lShape;
  brd[3][3]->value = straight;
  brd[3][4]->value = lShape;
  brd[3][5]->value = 0;
  brd[3][6]->value = end;
  brd[3][7]->value = straight;
  brd[3][8]->value = lShape;
  brd[3][9]->value = end;

  brd[4][1]->value = lShape;
  brd[4][2]->value = tShape;
  brd[4][3]->value = end;
  brd[4][4]->value = lShape;
  brd[4][5]->value = lShape;
  brd[4][6]->value = tShape;
  brd[4][7]->value = tShape;
  brd[4][8]->value = end;
  brd[4][9]->value = 0;

  brd[5][1]->value = straight;
  brd[5][2]->value = lShape;
  brd[5][3]->value = lShape;
  brd[5][4]->value = end;

  brd[5][5]->value = tShape;
  brd[5][5]->source = 1;
  brd[5][5]->connected = 1;

  brd[5][6]->value = tShape;
  brd[5][7]->value = tShape;
  brd[5][8]->value = tShape;
  brd[5][9]->value = lShape;

  brd[6][1]->value = end;
  brd[6][2]->value = lShape;
  brd[6][3]->value = tShape;
  brd[6][4]->value = lShape;
  brd[6][5]->value = end;
  brd[6][6]->value = lShape;
  brd[6][7]->value = straight;
  brd[6][8]->value = end;
  brd[6][9]->value = tShape;

  brd[7][1]->value = lShape;
  brd[7][2]->value = lShape;
  brd[7][3]->value = lShape;
  brd[7][4]->value = tShape;
  brd[7][5]->value = lShape;
  brd[7][6]->value = lShape;
  brd[7][7]->value = lShape;
  brd[7][8]->value = end;
  brd[7][9]->value = tShape;

  brd[8][1]->value = end;
  brd[8][2]->value = lShape;
  brd[8][3]->value = lShape;
  brd[8][4]->value = end;
  brd[8][5]->value = lShape;
  brd[8][6]->value = tShape;
  brd[8][7]->value = lShape;
  brd[8][8]->value = end;
  brd[8][9]->value = straight;

  brd[9][1]->value = end;
  brd[9][2]->value = tShape;
  brd[9][3]->value = straight;
  brd[9][4]->value = lShape;
  brd[9][5]->value = end;
  brd[9][6]->value = tShape;
  brd[9][7]->value = lShape;
  brd[9][8]->value = 0;
  brd[9][9]->value = end;
}

// for debug
void printBinary(int n) {
  printf("printing binary\n");
  char *c = malloc(5 * sizeof(char));
  memset(c, 0, 5);
  int i = 0;
  while (i != 4) {
    c[3 - i] = n & 1 ? '1' : '0';
    n = n >> 1;
    i++;
  }
  c[4] = '\0';
  printf("%s\n", c);
  free(c);
}

void RotateLeft(Tile *t) {
  t->value = ((t->value << 1) & 0b1111) | (t->value >> 3);
}

void cleanConnected(Tile ***brd, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      brd[i][j]->connected = 0;
    }
  }
}

void markConnected(Tile ***brd, int i, int j, int rows, int cols) {
  Tile *t = brd[i][j];
  t->connected = 1;
  if (hasLeft(t)) {
    if (j - 1 >= 0 && hasRight(brd[i][j - 1]) && !brd[i][j - 1]->connected) {
      markConnected(brd, i, j - 1, rows, cols);
    }
  }
  if (hasUp(t)) {
    if (i - 1 >= 0 && hasDown(brd[i - 1][j]) && !brd[i - 1][j]->connected) {
      markConnected(brd, i - 1, j, rows, cols);
    }
  }
  if (hasRight(t)) {
    if (j + 1 < cols && hasLeft(brd[i][j + 1]) && !brd[i][j + 1]->connected) {
      markConnected(brd, i, j + 1, rows, cols);
    }
  }
  if (hasDown(t)) {
    if (i + 1 < rows && hasUp(brd[i + 1][j]) && !brd[i + 1][j]->connected) {
      markConnected(brd, i + 1, j, rows, cols);
    }
  }
}

void HandleBoardUpdate(Tile ***brd) {
  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    Vector2 mouseClick = GetMousePosition();
    int j = mouseClick.x / 90;
    int i = mouseClick.y / 90;
    RotateLeft(brd[i][j]);
  }
  cleanConnected(brd, 11, 11);
  markConnected(brd, 5, 5, 11, 11);
}

void FreeBoard(Tile ***brd, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      free(brd[i][j]);
    }
  }
  free(brd);
}

int main() {
  int rows = 11;
  int cols = 11;
  Tile ***brd = InitBoard(rows, cols);
  GenerateBoard(brd, rows, cols);
  InitWindow(1920, 1080, "Hello raylib");
  while (!WindowShouldClose()) {
    BeginDrawing();
    int x = 10;
    int y = 10;
    for (int i = 0; i < cols; i++) {
      for (int j = 0; j < rows; j++) {
        DrawTile(brd[i][j]);
        x += 90;
      }
      x = 10;
      y += 90;
    }
    HandleBoardUpdate(brd);
    ClearBackground(WHITE);
    EndDrawing();
  }
  CloseWindow();
  FreeBoard(brd, 11, 11);
  return 0;
}
