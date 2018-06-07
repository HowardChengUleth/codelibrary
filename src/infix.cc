/*
 * Infix expressions evaluation
 *
 * Author: Howard Cheng
 *
 * The evaluate() routine takes a string containing an infix arithmetic
 * expression, and return the numeric result after evaluation.  The
 * parameter error indicates whether an error has occurred (syntax
 * error, illegal operation, etc.).  If there is an error the result
 * returned is meaningless.
 *
 * The routine assumes that the operands in the input are integers
 * with no leading signs.  It supports the standard +, -, *, / and
 * parentheses.  If you need to support more operators, operand types,
 * etc., you will need to modify the code.  See comments below.
 * 
 */

#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cstdlib>

using namespace std;

// What is a token?  Modify if needed (e.g. to support variables, extra
// operators, etc.)
struct Token
{
  enum Type {NUMBER, PLUS, MINUS, TIMES, DIVIDE, LEFT_PAREN, RIGHT_PAREN};

  // priority of the operators: bigger number means higher priority
  // e.g. */ has priority 2, +- has priority 1, ( has priority 0
  int priority[7];
  
  // is the operator left associative?  It's assumed that all operators
  // of the same priority level has the same left/right associative property
  bool left_assoc[7];

  Type type;
  long val;

  Token()
  {
    priority[1] = priority[2] = 1;
    priority[3] = priority[4] = 2;
    priority[5] = 0;
    left_assoc[1] = left_assoc[2] = left_assoc[3] = left_assoc[4] = true;
  }

  int get_priority() {
    return priority[type];
  }
  
  bool is_left_assoc() {
    return left_assoc[type];
  }

  // returns true if there is a next token
  bool next_token(string &expr, int &start, bool &error)
  {
    int len = expr.length();

    error = false;
    while (start < len && isspace(expr[start])) {
      start++;
    }
    if (start >= len) {
      return false;
    }
    
    switch (expr[start]) {
    case '(':
      type = LEFT_PAREN;
      break;
    case ')':
      type = RIGHT_PAREN;
      break;
    case '*':
      type = TIMES;
      break;
    case '/':
      type = DIVIDE;
      break;
    case '+':
      type = PLUS;
      break;
    case '-':
      type = MINUS;
      break;
    default:
      // check for number
      const char *s = expr.c_str() + start;
      char *p;
      val = strtol(s, &p, 10);
      if (s == p) {
	error = true;
	return false;
      }
      type = NUMBER;
      start += (p - s);
    }
    if (type != NUMBER) {
      start++;
    }
    return true;
  }
};

// Modify this if you need to support more operators or change their
// meanings.
//
// returns true if operation is successful
bool apply_op(stack<long> &operands, Token token)
{
  long a, b;

  if (operands.size() < 2) {
    return false;
  }
  if (token.type == Token::PLUS) {
    b = operands.top(); operands.pop();
    a = operands.top(); operands.pop();
    operands.push(a+b);
  } else if (token.type == Token::MINUS) {
    b = operands.top(); operands.pop();
    a = operands.top(); operands.pop();
    operands.push(a-b);
  } else if (token.type == Token::TIMES) {
    b = operands.top(); operands.pop();
    a = operands.top(); operands.pop();
    operands.push(a*b);
  } else if (token.type == Token::DIVIDE) {
    b = operands.top(); operands.pop();
    a = operands.top(); operands.pop();
    if (b == 0) {
      return false;
    }
    operands.push(a/b);
  } else {
    return false;
  }
  return true;
}

long evaluate(string expr, bool &error)
{
  stack<Token> s;
  stack<long> operands;
  int i;
  Token token;

  error = false;
  i = 0;
  while (token.next_token(expr, i, error) && !error) {
    switch (token.type) {
    case Token::NUMBER:
      operands.push(token.val);
      break;
    case Token::LEFT_PAREN:
      s.push(token);
      break;
    case Token::RIGHT_PAREN:
      while (!(error = s.empty()) && s.top().type != Token::LEFT_PAREN) {
	if ((error = !apply_op(operands, s.top()))) {
	  break;
	}
	s.pop();
      }
      if (!error) {
	s.pop();
      }
      break;
    default:      // arithmetic operators
      while (!error && !s.empty() && 
	     (token.get_priority() < s.top().get_priority() ||
	      token.get_priority() == s.top().get_priority() &&
	      token.is_left_assoc())) {
	error = !apply_op(operands, s.top());
	s.pop();
      }
      if (!error) {
	s.push(token);
      }
    }
    if (error) {
      break;
    }
  }
  while (!error && !s.empty()) {
    error = !apply_op(operands, s.top());
    s.pop();
  }
  error |= (operands.size() != 1);
  if (error) {
    return 0;
  }
  return operands.top();
}

int main(void)
{
  int result;
  string expr;
  bool error; 

  getline(cin, expr);
  while (!cin.eof()) {
    result = evaluate(expr, error);
    if (error) {
      cout << "Invalid expression" << endl;
    } else {
      cout << " = " << result << endl;
    }
    getline(cin, expr);
  }
  return 0;

}
