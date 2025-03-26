#ifndef QUEUE_H

typedef struct Node {
  void *data;
  struct Node *next;
} Node;

typedef struct {
  Node *head;
  Node *tail;
  int len;
} Queue;

Queue *NewQueue();

void Offer(Queue *q, void *val);

void *Poll(Queue *q);

void FreeQueue(Queue *q);

#endif
