#include "queue.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int straight = 5;
int end = 8;
int tShape = 7;
int lShape = 12;

typedef struct Tile {
  int value;
  int connected;
} Tile;

void printArray(int **arr, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      printf("%d", arr[i][j]);
    }
    printf("\n");
  }
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

int hasLeft(Tile *t) { return (t->value & 0b1000) >> 3; }

int hasRight(Tile *t) { return (t->value & 0b0010) >> 1; }

int hasUp(Tile *t) { return (t->value & 0b0100) >> 2; }

int hasDown(Tile *t) { return t->value & 0b0001; }

void DrawTile(Tile *t, int x, int y, int size) {
  int borderWidth = 1;
  DrawRectangle(x, y, size, size, GRAY);
  DrawRectangle(x + borderWidth, y + borderWidth, size - (borderWidth * 2),
                size - (borderWidth * 2), WHITE);
  int center = ((size - (borderWidth * 2)) / 2);
  if (hasLeft(t)) {
    if (t->connected) {
      DrawRectangle(x, y + 43, 45, 5, GREEN);
    } else {
      DrawRectangle(x, y + 43, 45, 5, GRAY);
    }
  }
  if (hasUp(t)) {
    if (t->connected) {
      DrawRectangle(x + 43, y, 5, 45, GREEN);
    } else {
      DrawRectangle(x + 43, y, 5, 45, GRAY);
    }
  }
  if (hasRight(t)) {
    if (t->connected) {
      DrawRectangle(x + 43 + 5, y + 43, 45, 5, GREEN);
    } else {
      DrawRectangle(x + 43 + 5, y + 43, 45, 5, GRAY);
    }
  }
  if (hasDown(t)) {
    if (t->connected) {
      DrawRectangle(x + 43, y + 43 + 5, 5, 45, GREEN);
    } else {
      DrawRectangle(x + 43, y + 43, 5, 45, GRAY);
    }
  }
}

Tile ***InitBoard(int rows, int cols) {
  Tile ***brd = malloc(rows * sizeof(Tile *));
  for (int i = 0; i < rows; i++) {
    brd[i] = malloc(cols * sizeof(Tile));
    for (int j = 0; j < cols; j++) {
      brd[i][j] = malloc(sizeof(Tile));
    }
  }
  return brd;
}

Tile *tile(int val, int source) {
  Tile *t = malloc(sizeof(Tile *));
  t->value = val;
  t->connected = 0;
  return t;
}

int r(int min, int max) { return min + rand() % (max - min + 1); }

int roll30P() {
  int result = rand() % 100;
  printf("random: %d\n", result);
  return result < 50;
}

void GenBoard(Tile ***brd, int x, int y, int rows, int cols, int **reserved) {
  printf("reserved:\n");
  printArray(reserved, rows, cols);
  printf("x: %d, y: %d\n", x, y);
  if (x < 0 || x >= rows) {
    return;
  }
  if (y < 0 || y >= cols) {
    return;
  }
  int genValue = 0;
  int branches = 0;
  if (y - 1 >= 0) {
    if (hasRight(brd[x][y - 1]) || (!reserved[x][y - 1] && roll30P())) {
      genValue = genValue | 0b1000;
      reserved[x][y - 1] = 1;
      branches++;
      if (brd[x][y - 1] != 0) {
        GenBoard(brd, x, y - 1, rows, cols, reserved);
      }
    }
  }
  if (x - 1 >= 0) {
    if (hasDown(brd[x - 1][y]) || (!reserved[x - 1][y] && roll30P())) {
      genValue = genValue | 0b0100;
      reserved[x - 1][y] = 1;
      branches++;
      if (brd[x - 1][y]->value == 0) {
        GenBoard(brd, x - 1, y, rows, cols, reserved);
      }
    }
  }
  if (y + 1 < cols) {
    if (hasLeft(brd[x][y + 1]) || (!reserved[x][y + 1] && roll30P())) {
      genValue = genValue | 0b0010;
      reserved[x][y + 1] = 1;
      branches++;
      if (brd[x][y + 1]->value == 0) {
        GenBoard(brd, x, y + 1, rows, cols, reserved);
      }
    }
  }
  if (x + 1 < cols) {
    if (hasUp(brd[x + 1][y]) || (!reserved[x + 1][y] && roll30P())) {
      genValue = genValue | 0b0001;
      branches++;
      if (brd[x + 1][y]->value == 0) {
        GenBoard(brd, x + 1, y, rows, cols, reserved);
      }
    }
  }
  printBinary(genValue);
  brd[x][y]->value = genValue;
}

void GenerateBoard(Tile ***brd, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      brd[i][j] = tile(0, 0);
    }
  }
  int **reserved = malloc(rows * sizeof(int *));
  for (int i = 0; i < rows; i++) {
    reserved[i] = malloc(cols * sizeof(int));
    for (int j = 0; j < cols; j++) {
      reserved[i][j] = 0;
    }
  }
  // Hardcoded start
  reserved[5][5] = 1;
  GenBoard(brd, 5, 5, rows, cols, reserved);
  for (int i = 0; i < rows; i++) {
    free(reserved[i]);
  }
  free(reserved);
}

void GenerateStaticBoard(Tile ***brd, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      brd[i][j] = tile(0, 0);
    }
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
    free(brd[i]);
  }
  free(brd);
}

int main(int argc, char **argv) {
  srand(time(NULL));
  int tileSizePx = 100;

  int screenW = 1920;
  int screenH = 1080;

  int rows = 11;
  int cols = 11;

  Tile ***brd = InitBoard(rows, cols);
  GenerateBoard(brd, rows, cols);

  printf("values:");
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      printf("%d, ", brd[i][j]->value);
    }
    printf("\n");
  }

  InitWindow(screenW, screenH, "Branch");
  while (!WindowShouldClose()) {
    BeginDrawing();
    int x = 10;
    int y = 10;
    for (int i = 0; i < cols; i++) {
      for (int j = 0; j < rows; j++) {
        DrawTile(brd[i][j], x, y, tileSizePx);
        x += 90;
      }
      x = 10;
      y += 90;
    }
    HandleBoardUpdate(brd);
    ClearBackground(WHITE);
    EndDrawing();
  }
  FreeBoard(brd, rows, cols);
  CloseWindow();
  return 0;
}
