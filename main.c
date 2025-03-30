#include "arrays.h"
#include "queue.h"
#include "raylib.h"
#include "raymath.h"
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Tile {
  Vector3 position;
  Vector3 size;
  Vector3 rotation;
  Vector3 scale;
  int value;
  int connected;
  int x, y;
} Tile;

Tile *tile(Vector3 position, int val, int x, int y) {
  Tile *t = malloc(sizeof(Tile));
  t->position = position;
  t->rotation = (Vector3){0.0f, -1.0f, 0.0f};
  t->scale = (Vector3){1.0f, 1.0f, 1.0f};
  t->size = (Vector3){1.0f, 1.0f, 1.0f};
  t->x = x;
  t->y = y;
  t->connected = 0;
  return t;
}

int hasLeft(Tile *t) { return (t->value & 0b1000) >> 3; }

int hasRight(Tile *t) { return (t->value & 0b0010) >> 1; }

int hasUp(Tile *t) { return (t->value & 0b0100) >> 2; }

int hasDown(Tile *t) { return t->value & 0b0001; }

void DrawTile(Tile *t, Model *model) {

  DrawModelWiresEx(*model, t->position, t->rotation, 0, t->scale, BLACK);
}

Tile ***InitBoard(int rows, int cols) {
  // TODO Assume it's always 8 by 8 for now
  Vector3 pos = {-16.0f, 0.0f, -16.0f};
  Vector3 jInc = {4.0f, 0.0f, 0.0f};
  Vector3 iInc = {0.0f, 0.0f, 4.0f};
  Tile ***brd = malloc(rows * sizeof(Tile **));
  for (int i = 0; i < rows; i++) {
    brd[i] = malloc(cols * sizeof(Tile *));
    for (int j = 0; j < cols; j++) {
      brd[i][j] = tile(pos, 0, i, j);
      pos = Vector3Add(pos, jInc);
    }
    pos.x = -16.0f;
    pos = Vector3Add(pos, iInc);
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
  Offer(queue, brd[4][4]);
  int **visited = NewTwoDimArray(rows, cols, 0);
  while (queue->len != 0) {
    int len = queue->len;
    for (int i = 0; i < len; i++) {
      Tile *t = Poll(queue);
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

void HandleBoardUpdate(Tile ***brd, Camera *cam, int rows, int cols) {
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    Ray ray = GetScreenToWorldRay(GetMousePosition(), *cam);
    printf("pos: %f, %f, %f; dir: %f, %f, %f\n", ray.position.x, ray.position.y,
           ray.position.z, ray.direction.x, ray.direction.y, ray.direction.z);
  }
  cleanConnected(brd, rows, cols);
  markConnected(brd, 4, 4, rows, cols);
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

  int screenW = 1920;
  int screenH = 1080;

  int rows = 8;
  int cols = 8;

  Tile ***brd = InitBoard(rows, cols);
  GenerateBoard(brd, rows, cols);
  InitWindow(screenW, screenH, "Branch");
  SetTargetFPS(60);

  Model model = LoadModel("assets/branch_t.glb");

  Camera cam = {
      {0.0f, 20.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 100.0f, 0};

  Ray ray = {0};
  RayCollision co = {0};

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(WHITE);
    BeginMode3D(cam);
    for (int i = 0; i < cols; i++) {
      for (int j = 0; j < rows; j++) {
        DrawTile(brd[i][j], &model);
      }
    }
    HandleBoardUpdate(brd, &cam, rows, cols);
    EndMode3D();
    EndDrawing();
  }
  CloseWindow();
  UnloadModel(model);
  FreeBoard(brd, rows, cols);
  return 0;
}
