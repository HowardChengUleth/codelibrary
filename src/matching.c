/* unweighted matching in a bipartite graph.
 * author: Matthew McNaughton, Jan 16, 1999.
 * mcnaught@cs.ualberta.ca
 *
 * The bipartite graph G is split into two sets, U and V,
 * of user-defined maximum size MAXU and MAXV.
 * the input graph is in bipgraph[MAXU][MAXV].
 * there is an edge between node u \in U and node v \in V
 * iff bipgraph[u][v] != 0.
 *
 * The output is in matching[MAXU]. 
 * node u \in U and node v \in V are matched iff matching[u] == v.
 *
 * parameters match(int u, int v) mean: u is the number of vertices
 * in U, v the  number in V. They are assumed to be numbered 0 .. u-1
 * and 0 .. v-1, respectively.
 *
 * Technique: given a non-maximum matching M on G, find an "alternating path"
 * u_1 v_1 ... u_n v_n so that u_1 and v_n are not matched in M, but
 * v_k u_k+1 are matched with each other. Then "flip" the edges so
 * that edges on this path which were not in the matching are, and edges
 * which were are not. This increases the size of the matching by one.
 * It is a fact that if no such path exists, then M is maximum.
 *
 * This algorithm finds several alternating paths at once by performing
 * bfs starting at all unmatched nodes u \in U. Paths which do not
 * have intersecting nodes can be alternated in the same bfs run.
 * bfs is performed repeated until the matching cannot be expanded.
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

FILE *in, *out;

/* change these as necessary */
#define MAXU 100
#define MAXV 100

#define U(i) (i)
#define V(i) ((i) + MAXU)
#define isU(i) ((i) < MAXU)
#define isV(i) ((i) >= MAXU)

#define isMatched(i) (isU(i) ? flagUmatched[(i)] : flagVmatched[(i)-MAXU])
#define isUsed(i) (isU(i) ? flagUused[(i)] : flagVused[(i)-MAXU])
#define isVisited(i) (isU(i) ? flagUvisited[(i)] : flagVvisited[(i)-MAXU])

#define setMatched(i) (isU(i)?(flagUmatched[(i)]=1):(flagVmatched[(i)-MAXU]=1))
#define setUsed(i) (isU(i)?(flagUused[(i)]=1):(flagVused[(i)-MAXU]=1))
#define setVisited(i) (isU(i)?(flagUvisited[(i)]=1):(flagVvisited[(i)-MAXU]=1))

char bipgraph[MAXU][MAXV];
int matching[MAXU]; /* matching[u] == v, _not_ plus MAXU */
char flagUmatched[MAXU], flagVmatched[MAXV];
char flagUvisited[MAXU], flagVvisited[MAXV];
char flagUused[MAXU],    flagVused[MAXV];
int predecessor[MAXU+MAXV], queue[MAXU+MAXV];

/* u and v are the number of vertices in sets U, and V, respectively,
 * filling up bipgraph[0..u-1][0..v-1].
 * result:
 *  matching[u0]==v0 iff u0 and v0 are in the matching, 
 * otherwise matching[u0] = -1 */
void
match(int u, int v) {
  int i,j, head,tail, bad, last, increased;

  for( i = 0; i < u; i++ ) {
    matching[i] = -1;
    flagUmatched[i] = 0;
  }
  for( i = 0; i < v; i++ ) flagVmatched[i] = 0;

  do { /* find alternating paths by repeated bfs. */
    for( i = 0; i < MAXU+MAXV; i++ ) predecessor[i] = -1;
    for( i = 0; i < MAXU; i++ ) flagUused[i] = flagUvisited[i] = 0;
    for( i = 0; i < MAXV; i++ ) flagVused[i] = flagVvisited[i] = 0;
  
    head = tail = 0;

    /* put all the unmatched u's on the queue. They start the 
     * alternating path. */
    for( i = 0; i < u; i++ ) {
      if( ! isMatched(U(i))) {
	queue[tail++] = U(i);
	predecessor[i] = -1; /* redundant statement */
	setVisited(U(i));
      }
    }

    /* flag that at least one path was found by the bfs.
     * when the bfs does not find an alternating path we are done. */
    increased = 0;

    while( head != tail ) {
      i = queue[head++];

      /* this node appeared on some previously found alternating path. */
      if( isUsed(i) ) continue;
    
      if( isV(i) && !isMatched(i) ) {
	/* we got to the end of an alternating path. see if
	 * it is disjoint with other paths found so far. only
	 * then can we mess it up a bit. */
	bad = 0;
	for( j = i; j != -1; j = predecessor[j]) {
	  if( isUsed(j)) {
	    bad = 1;
	    break;
	  }
	}
	
	if( ! bad ) {
	  /* this path is pristine. switch "polarity" of edges
	   * in the matching on this path. */

	  /* flag and instrumention - whether (not) to quit,
	   * and how many paths we found this bfs. */
	  increased++; 
	  for( j = i; j != -1; last = j, j = predecessor[j] ) {
	    if( isV(j) && !isMatched(j)) {
	      /* the only unmatched v - actually this means we
	       * are on the first iteration of this loop. */
	      setMatched(j);

	    } else if( isU(j) ) {
	      if( isMatched(j) ) {
		/* the node we saw in the previous iteration of
		 * this loop must be a V. We will match with it
		 * instead of the one we used to match with, which
		 * must be the next node visited in this loop. */
		assert(isV(last));
		matching[j] = last - MAXU;
	      } else {
		/* we are the very first u, one of the ones the
		 * bfs queue was "seeded" with. We should have ...*/
		assert(predecessor[j] == -1);
		setMatched(j);
		assert(isV(last));
		matching[j] = last - MAXU;
	      }
	    }
	    setUsed(j); /* this node cannot be used for other
			 * paths we might run across in the future
			 * on this bfs. */
	  } /* for */
	} /* if ! bad */
      } /* isV and !isMatched */

      else if( isV(i) ) {
	/* this must be a matched V - find the matching U and put it on 
	 * the queue if it is not visited or used. */

	bad = 1;

	for( j = 0; j < u; j++ ) {
	  if( isMatched(U(j)) && matching[j] == i - MAXU ) {
	    /* this is the one. */
	    if( ! isVisited(U(j)) && !isUsed(U(j))) {
	      setVisited(U(j));
	      queue[tail++] = U(j); 
	      predecessor[U(j)] = i;
	    }
	    bad = 0;
	    break;
	  }
	}
	assert(!bad);
      } /* isV */
      else if( isU(i) ) {
	/* we are at U. whether it is unmatched (a "seed"),
	 * or matched, we do the same thing - put on the queue
	 * all V's which it is connected to in the graph but
	 * which it is _not_ paired to in the current matching. */

	for( j = 0; j < v; j++ ) {
	  if( bipgraph[i][j] &&
	      !isVisited(V(j)) && 
	      !isUsed(V(j)) && 
	      matching[i] != j ) {
	    /* we can put this one on the queue. */
	    queue[tail++] = V(j);
	    predecessor[V(j)] = i;
	    setVisited(V(j));
	  }
	}
      } else {
	assert(0); /* should be no other cases. */
      }
      /* this is the end of the bfs. */
    } 
  } while( increased );

  return;
}
	
	      
int
main() {
  int i,j,u,v,setnum;

  in = stdin; out = stdout; setnum = 0;

  while( fscanf(in, "%d %d", &u, &v) == 2 ) {

    for( i = 0; i < u; i++ ) for( j = 0; j < v; j++ ) bipgraph[i][j] = 0;

    while( fscanf(in, "%d %d", &i, &j) == 2 && i != -1 && j != -1 ) {
      bipgraph[i][j] = 1;
    }

    match(u,v);

    fprintf(out, "Problem #%d:\n", ++setnum);
    for( i = 0; i < u; i ++ ) {
      if( matching[i] != -1 ) 
	fprintf(out, "match %d to %d\n", i, matching[i]);
    }
  }
  return 0;
}

	      

	      
	      



