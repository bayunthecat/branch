#include "queue.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

Queue *NewQueue() {
  Queue *q = malloc(sizeof(Queue));
  q->tail = NULL;
  q->head = NULL;
  q->len = 0;
  return q;
}

void Offer(Queue *q, void *val) {
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

void *Poll(Queue *q) {
  if (q->head == NULL) {
    return NULL;
  }
  q->len--;
  Node *head = q->head;
  void *d = head->data;
  q->head = q->head->next;
  if (q->head == NULL) {
    q->tail = NULL;
  }
  free(head);
  return d;
}

void FreeQueue(Queue *q) {
  Node *head = q->head;
  while (head != NULL) {
    free(head->data);
    Node *tmp = head->next;
    free(head);
    head = tmp;
  }
  free(q);
}
