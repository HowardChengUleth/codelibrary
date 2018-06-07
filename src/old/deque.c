/*
 * Double-ended Queues (DEQUE)
 *
 * Author: Howard Cheng
 *
 * This file implements a bounded queue as a circular array.  The user must
 * initially supply the maximum number of elements that can be in the queue
 * at any one time.  Supported operations are:
 *
 * initialization (allocate memory, etc)
 * free memory
 * add element at either the head or the tail
 * remove element from either the head or the tail
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* use whatever element type as necessary                       */
/* assume that ELEMTYPE has proper assignment operation defined */
#define ELEMTYPE int

typedef struct {
  int n;
  ELEMTYPE *elem;
  int head, tail;
} Queue;

void init_queue(Queue *q, int n)
{
  n++;                                /* one bigger than necessary */
  q->elem = malloc(n * sizeof(*(q->elem)));
  assert(q->elem);
  q->n = n;
  q->head = q->tail = 0;
}

void free_queue(Queue *q)
{
  free(q->elem);
  q->elem = 0;
  q->n = 0;
  q->head = q->tail = 0;
}

int empty_queue(Queue *q)
{
  return (q->head == q->tail);
}

int add_tail(Queue *q, ELEMTYPE item)
{
  int new_tail = (q->tail + 1) % (q->n);
  if (new_tail == q->head) {
    fprintf(stderr, "Queue full: cannot add\n");
    return 0;
  } else {
    q->elem[q->tail] = item;
    q->tail = new_tail;
    return 1;
  }
}

int add_head(Queue *q, ELEMTYPE item)
{
  int new_head = q->head - 1;
  if (new_head < 0) {
    new_head += q->n;
  }
  if (new_head == q->tail) {
    fprintf(stderr, "Queue full: cannot add\n");
    return 0;
  } else {
    q->elem[new_head] = item;
    q->head = new_head;
    return 1;
  }
}

ELEMTYPE remove_head(Queue *q)
{
  ELEMTYPE item;
  if (empty_queue(q)) {
    fprintf(stderr, "Queue empty: cannot delete\n");
    assert(0);
  }
  item = q->elem[q->head];
  q->head = (q->head + 1) % (q->n);
  return item;
}

ELEMTYPE remove_tail(Queue *q)
{
  int new_tail = q->tail - 1;
  if (empty_queue(q)) {
    fprintf(stderr, "Queue empty: cannot delete\n");
    assert(0);
  }
  if (new_tail < 0) {
    new_tail += q->n;
  }
  q->tail = new_tail;
  return q->elem[q->tail];
}

int num_in_queue(Queue *q)
{
  int res = q->tail - q->head;
  return (res >= 0) ? res : res + q->n;
}

void print_queue(FILE *out, Queue *q)
{
  /* substitute appropriate routines for printing ELEMTYPE */
  int i, count;

  count = 0;
  fprintf(out, "Elements:");
  for (i = q->head; i != q->tail; ) {
    fprintf(out, " %d", q->elem[i]);
    if (++i == q->n) {
      i = 0;
    }
    count++;
  }
  fprintf(out, "\ncount = %d\n", count);
  assert(count == num_in_queue(q));
}

int main(void)
{
  Queue queue;
  char buffer[1000];
  int num, item;

  while (scanf("%d", &num) == 1 && num > 0) {
    init_queue(&queue, num);
    printf("Queue of size %d allocated, enter commands (\"quit\" to quit)\n",
           num);
    while (scanf("%s", buffer) == 1 && strcmp(buffer, "quit")) {
      if (!strcmp(buffer, "ah")) {
        scanf("%d", &item);
        add_head(&queue, item);
      } else if (!strcmp(buffer, "at")) {
        scanf("%d", &item);
        add_tail(&queue, item);
      } else if (!strcmp(buffer, "rh")) {
        item = remove_head(&queue);
        printf("Removed: %d\n", item);
      } else if (!strcmp(buffer, "rt")) {
        item = remove_tail(&queue);
        printf("Removed: %d\n", item);
      } else {
        fprintf(stderr, "Unknown command\n");
      }
      print_queue(stdout, &queue);
    }
    free_queue(&queue);
  }
  return 0;
}
