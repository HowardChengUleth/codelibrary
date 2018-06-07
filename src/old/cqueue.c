#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
/* expanding circular queue */

/* redefine this to whatever you want. */
typedef int item_t;

typedef struct {
  int head, tail, length, ilen;
  item_t *items;
} queue_t;


queue_t *
create_queue(int startlength) {
  queue_t *q;
  q = malloc(sizeof(*q)); assert(q);
  q->head = q->tail = 0;
  q->ilen = q->length = startlength;
  q->items = malloc(startlength * sizeof(q->items[0])); assert(q->items);
  return q;
}

void
destroy_queue(queue_t *q) {
  free(q->items);
  free(q);
}

void
queue_resize(queue_t *q, int newlength) {
  int i;
  if( (q->tail + 1)%q->length == q->head) {
    fprintf(stderr, "resizing, length is %d, head %d, tail %d\n",
	    q->length, q->head, q->tail);
    if( newlength == 0 ) newlength = q->ilen + q->length;
    q->items = realloc(q->items, newlength * sizeof(q->items[0]) );
    assert(q->items);
  } else return;

  if( q->head != 0 ) {
    for( i = newlength-1; i >= q->head + (newlength - q->length); i-- ) {
      q->items[i] = q->items[ i-(newlength - q->length) ];
    }
    q->head += newlength - q->length;
    q->head %= newlength;
  }

  q->length = newlength;
  return;
}

void
enqueue(queue_t *q, item_t i) {
  fprintf(stderr, "enqueue %d\n", i);
  queue_resize(q, 0);
  q->items[q->tail] = i;
  q->tail = (q->tail+1) % q->length;
}

item_t
dequeue(queue_t *q) {
  item_t i;

  i = q->items[q->head];
  fprintf(stderr, "dequeueing %d\n", i);
  q->head = (q->head+1) % q->length;
  return i;
}

int
main() {
  queue_t *q;
  item_t t;
  int i,j;

  q = create_queue(3);
  i = 0;
  while( fscanf(stdin, "%d", &t) == 1 ) {
    if( ! t ) {
      if( ! i ) fprintf(stderr, "queue is alread empty\n");
      else {
	fprintf(stdout, "%d\n", dequeue(q));
	i--;
      }
    } else {
      enqueue(q, t);
      i++;
    }
  }

  for( j = 0; j < i; j++ ) {
    fprintf(stdout, "%d\n", dequeue(q));
  }

  destroy_queue(q);
  return 0;
}
