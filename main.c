#include "arrays.h"
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
  int x;
  int y;
  int value;
  int connected;
} Tile;

Tile *tile(int val, int x, int y) {
  Tile *t = malloc(sizeof(Tile));
  t->value = val;
  t->connected = 0;
  t->x = x;
  t->y = y;
  return t;
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
  Tile ***brd = malloc(rows * sizeof(Tile **));
  for (int i = 0; i < rows; i++) {
    brd[i] = malloc(cols * sizeof(Tile *));
    for (int j = 0; j < cols; j++) {
      brd[i][j] = tile(0, i, j);
    }
  }
  return brd;
}

int r(int min, int max) { return min + rand() % (max - min + 1); }

int roll30P() {
  int result = rand() % 100;
  return result < 50;
}

void GenerateBoard(Tile ***brd, int rows, int cols) {
  int **res = NewTwoDimArray(rows, cols, 0);
  Queue *queue = NewQueue();
  // Hardcoded start
  Offer(queue, brd[5][5]);
  int **visited = NewTwoDimArray(rows, cols, 0);
  while (queue->len != 0) {
    int len = queue->len;
    for (int i = 0; i < len; i++) {
      Tile *t = Poll(queue);
      printf("%d, %d\n", t->x, t->y);
      if (visited[t->x][t->y]) {
        continue;
      }
      visited[t->x][t->y] = 1;
      int genValue = 0;
      if (t->y - 1 >= 0) {
        if (hasRight(brd[t->x][t->y - 1]) ||
            (!res[t->x][t->y - 1] && roll30P())) {
          genValue = genValue | 0b1000;
          res[t->x][t->y - 1] = 1;
          Offer(queue, brd[t->x][t->y - 1]);
        }
      }
      if (t->x - 1 >= 0) {
        if (hasDown(brd[t->x - 1][t->y]) ||
            (!res[t->x - 1][t->y] && roll30P())) {
          genValue = genValue | 0b0100;
          res[t->x - 1][t->y] = 1;
          Offer(queue, brd[t->x - 1][t->y]);
        }
      }
      if (t->y + 1 < cols) {
        if (hasLeft(brd[t->x][t->y + 1]) ||
            (!res[t->x][t->y + 1] && roll30P())) {
          genValue = genValue | 0b0010;
          res[t->x][t->y + 1] = 1;
          Offer(queue, brd[t->x][t->y + 1]);
        }
      }
      if (t->x + 1 < cols) {
        if (hasUp(brd[t->x + 1][t->y]) || (!res[t->x + 1][t->y] && roll30P())) {
          genValue = genValue | 0b0001;
          res[t->x + 1][t->y] = 1;
          Offer(queue, brd[t->x + 1][t->y]);
        }
      }
      t->value = genValue;
    }
  }
  FreeTwoDimArray(res, rows, cols);
  FreeTwoDimArray(visited, rows, cols);
  FreeQueue(queue);
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
  CloseWindow();
  FreeBoard(brd, rows, cols);
  return 0;
}
