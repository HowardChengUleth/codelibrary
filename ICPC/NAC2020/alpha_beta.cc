// alpha-beta pruning: Exponential time, but a good heuristic
// -- Use for mini-max searches (Player 1 is maximizing, Player -1 is minimizing).
// -- Call from main with f(start,-inf,inf,1);

int f(state S,int alpha,int beta,int p){
  if(s.is_done()) return p*s.value();
  
  for_all_states_from(s,p){     // We want "next" to run through all possible
    state next = child_of(S,p); // moves that player p can take from state s.
    alpha = max(alpha,-f(next,-beta,-alpha,-p));
    if(beta <= alpha) return alpha;
  }
  return alpha;
}
