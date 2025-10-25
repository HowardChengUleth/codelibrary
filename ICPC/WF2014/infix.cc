// Infix expressions evaluation
struct Token {  // modify as needed
  enum Type {NUMBER, PLUS, MINUS, TIMES, DIVIDE, LEFT_PAREN, RIGHT_PAREN};
  int priority[7]; // priority of the operators: bigger number means higher priority
  bool left_assoc[7];  // is operator left assoc
  Type type;
  long val;

  Token() {
    priority[1] = priority[2] = 1;
    priority[3] = priority[4] = 2;
    priority[5] = 0;
    left_assoc[1] = left_assoc[2] = left_assoc[3] = left_assoc[4] = true;
  }

  int get_priority() const { return priority[type]; };
  bool is_left_assoc() const { return left_assoc[type]; }

  bool next_token(string &expr, int &start, bool &error) {
    int len = expr.length();
    error = false;
    while (start < len && isspace(expr[start])) start++;
    if (start >= len) return false;
    
    switch (expr[start]) {
    case '(': type = LEFT_PAREN;   break;
    case ')': type = RIGHT_PAREN;  break;
    case '*': type = TIMES;        break;
    case '/': type = DIVIDE;       break;
    case '+': type = PLUS;         break;
    case '-': type = MINUS;        break;
    default:
      const char *s = expr.c_str() + start; char *p;
      val = strtol(s, &p, 10);
      if (s == p) { error = true;  return false; }
      type = NUMBER;     start += (p - s);
    }
    if (type != NUMBER) start++;
    return true;
  }
};

#define F(T) case Token::T: \
  b = operands.top(); operands.pop(); a = operands.top(); operands.pop();

// returns true if operation is successful
bool apply_op(stack<long> &operands, Token token){  // modify for more tokens
  long a, b;
  if (operands.size() < 2) return false;
  switch(token.type){
    F(PLUS) operands.push(a+b); break;
    F(MINUS) operands.push(a-b); break;
    F(TIMES) operands.push(a*b); break;
    F(DIVIDE) if(b == 0) return false; operands.push(a/b); break;
    default: return false;
  }
  return true;
}

long evaluate(string expr, bool &error){
  stack<Token> s;
  stack<long> operands;
  int i;
  Token token;

  error = false;       i = 0;
  while (token.next_token(expr, i, error) && !error) {
    switch (token.type) {
    case Token::NUMBER:
      operands.push(token.val);      break;
    case Token::LEFT_PAREN:
      s.push(token);                 break;
    case Token::RIGHT_PAREN:
      while (!(error = s.empty()) && s.top().type != Token::LEFT_PAREN) {
	if ((error = !apply_op(operands, s.top()))) break;
	s.pop();
      }
      if (!error) s.pop();
      break;
    default: 
      while (!error && !s.empty() &&
	     (token.get_priority() < s.top().get_priority() ||
	      (token.get_priority() == s.top().get_priority() && 
               token.is_left_assoc()))) {
	error = !apply_op(operands, s.top());   	s.pop();
      }
      if (!error) s.push(token);
    }
    if (error) break;
  }
  while (!error && !s.empty()) {
    error = !apply_op(operands, s.top());               s.pop();
  }
  error |= (operands.size() != 1);
  return (error) ? 0 : operands.top();
}
