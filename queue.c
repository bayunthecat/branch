#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  void *data;
  struct Node *next;
} Node;

typedef struct {
  Node *head;
  Node *tail;
  int len;
} Queue;

Queue *newQueue() {
  Queue *q = malloc(sizeof(Queue));
  q->tail = NULL;
  q->head = NULL;
  q->len = 0;
  return q;
}

void offer(Queue *q, void *val) {
  Node *new = malloc(sizeof(Node));
  new->data = val;
  new->next = NULL;
  Node *tail = q->tail;
  q->len++;
  if (tail == NULL) {
    q->head = new;
    q->tail = new;
    return;
  }
  q->tail->next = new;
  q->tail = new;
}

void *poll(Queue *q) {
  if (q->head == NULL) {
    return NULL;
  }
  q->len--;
  Node *head = q->head;
  q->head = q->head->next;
  void *d = head->data;
  free(head);
  return d;
}

void freeQueue(Queue *q) {
  Node *head = q->head;
  while (head != NULL) {
    free(head->data);
    Node *tmp = head->next;
    free(head);
    head = tmp;
  }
  free(q);
}

int main() {
  Queue *queue = newQueue();
  offer(queue, "first");
  offer(queue, "second");
  offer(queue, "third");

  printf("polled %s, len %d\n", (char *)poll(queue), queue->len);
  printf("polled %s, len %d\n", (char *)poll(queue), queue->len);
  printf("polled %s, len %d\n", (char *)poll(queue), queue->len);

  freeQueue(queue);
}
