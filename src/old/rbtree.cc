/*
 * Implementation of Red-Black Trees
 *
 * Author: Howard Cheng
 * Reference:
 *   Cormen, T., Leiserson, C. and Rivest, R.  "Introduction to Algorithms"
 *   Chapter 14
 *
 * This file contains a set of routines to manipulate Red Black trees.
 * This is useful if you want to perform search, predecessor, successor,
 * minimum, maximum, insert, and delete in O(log n) time.  Other than the
 * fact that it is somewhat "balanced", it can be used just like any
 * other binary search tree (e.g. inorder, preorder, postorder traversals).
 *
 * It is assumed that if multiple nodes have the same key, it doesn't
 * matter which one is returned for searching, which one is deleted,
 * etc.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* replace the following with your own key type */
typedef int KEYTYPE;

class Node {
public:
  KEYTYPE key;
  char color;               /* 0 = red, 1 = black */
  Node *left, *right, *parent;
  
  /* you can remove the following if it is unnecessary */
  int num;                  /* number of elements in the subtree */

  Node(KEYTYPE k) {
    key = k;
    color = 0;
    left = right = parent = 0;
    num = 1;
  };

  int cmp(const Node *x) {
    return key - x->key;
  }
  int cmp(const KEYTYPE k) {
    return key - k;
  }
  
  /* copy only the data fields, including key */
  void copy_data(const Node *x) {
    // add more if there are more data fields
    key = x->key;
  }

  /* fix num field assuming that the values from the children are correct */
  void fix_num(void) {
    num = 1;
    if (left) {
      num += left->num;
    }
    if (right) {
      num += right->num;
    }
  }

};

class RBTree {
public:
  Node *root;

  /* free memory used by the tree */
  void free_tree(Node *tree)
  {
    if (tree) {
      free_tree(tree->left);
      free_tree(tree->right);
      delete tree;
    }
  }

  /* Rotate left around node x, assumes that x and x->right is not NULL */
  void left_rotate(Node *x)
  {
    Node *y = x->right;
    if ((x->right = y->left)) {
      y->left->parent = x;
    }
    if (!(y->parent = x->parent)) {
      root = y;
    } else if (x == x->parent->left) {
      x->parent->left = y;
    } else {
      x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
    
    x->fix_num();
    y->fix_num();
  }

  /* Rotate right around node x, assumes that x and x->left is not NULL */
  void right_rotate(Node *x)
  {
    Node *y = x->left;
    if ((x->left = y->right)) {
      y->right->parent = x;
    }
    if (!(y->parent = x->parent)) {
      root = y;
    } else if (x == x->parent->left) {
      x->parent->left = y;
    } else {
      x->parent->right = y;
    }
    y->right = x;
    x->parent = y;
    
    x->fix_num();
    y->fix_num();
  }

  /* Insert a key into the proper place in the tree, but may violate */
  /* red-black properties.                                           */
  Node *insert_helper(KEYTYPE k)
  {
    Node *x, *y, *z;
    
    if (!(z = new Node(k))) {
      fprintf(stderr, "cannot allocate node\n");
      exit(255);
    }

    y = 0;
    x = root;
    while (x) {
      y = x;
      x->num++;
      if (z->cmp(x) < 0) {
	x = x->left;
      } else {
	x = x->right;
      }
    }
    z->parent = y;
    if (!y) {
      root = z;
    } else if (z->cmp(y) < 0) {
      y->left = z;
    } else {
      y->right = z;
    }
    return z;
  }

  /* Fix the red-black properties after deleting a node */
  void delete_helper(Node *x, Node *px)
  {
    Node *w;
    while (x != root && (!x || x->color)) {
      if (x == px->left) {
	w = px->right;
	if (!w->color) {
	  w->color = 1;
	  px->color = 0;
	  left_rotate(px);
	  w = px->right;
	}
	if ((!w->left || w->left->color) && (!w->right || w->right->color)) {
	  w->color = 0;
	  x = px;
	  px = px->parent;
	} else {
	  if (!w->right || w->right->color) {
	    w->left->color = 1;
	    w->color = 0;
	    right_rotate(w);
	    w = px->right;
	  }
	  w->color = px->color;
	  px->color = 1;
	  if (w->right) {
	    w->right->color = 1;
	  }
	  left_rotate(px);
	  x = root;
	}
      } else {
	w = px->left;
	if (!w->color) {
	  w->color = 1;
	  px->color = 0;
	  right_rotate(px);
	  w = px->left;
	}
	if ((!w->left || w->left->color) && (!w->right || w->right->color)) {
	  w->color = 0;
	  x = px;
	  px = px->parent;
	} else {
	  if (!w->left || w->left->color) {
	    w->right->color = 1;
	    w->color = 0;
	    left_rotate(w);
	    w = px->left;
	  }
	  w->color = px->color;
	  px->color = 1;
	  if (w->left) {
	    w->left->color = 1;
	  }
	  right_rotate(px);
	  x = root;
	}
      }
    }
    if (x) {
      x->color = 1;
    }
  }

  /* constructor */
  RBTree(void)
  {
    root = 0;
  }

  /* destructor */
  ~RBTree(void)
  {
    free_tree(root);
  }
  
  /* returns the number of element in the tree */
  int num_element(void) const
  {
    return (root) ? root->num : 0;
  }

  /* returns the node with the matching key, or NULL if not found */
  Node *search(KEYTYPE k) const
  {
    Node *tree = root;
    int t;
    while (tree) {
      if ((t = tree->cmp(k)) > 0) {
	tree = tree->left;
      } else if (t < 0) {
	tree = tree->right;
      } else {
	return tree;
      }
    }
    return 0;
  }

  /* returns the node with the minimum key, or NULL if tree is empty */
  Node *minimum(void) const
  {
    Node *tree = root;
    while (tree && tree->left) {
      tree = tree->left;
    }
    return tree;
  }


  /* returns the node with the maximum key, or NULL if tree is empty */
  Node *maximum(void) const
  {
    Node *tree = root;
    while (tree && tree->right) {
      tree = tree->right;
    }
    return tree;
  }

  /* returns the predecessor of the given node (the predecessor is based */
  /* on how the tree is built, so the result may be unpredictable if the */
  /* keys are not distinct.                                              */
  Node *predecessor(Node *x) const
  {
    Node *y;
    
    if (x->left) {
      y = x->left;
      while (y && y->right) {
	y = y->right;
      }
      return y;
    }
    y = x->parent;
    while (y && x == y->left) {
      x = y;
      y = y->parent;
    }
    return y;
  }
  
  /* returns the successor of the given node (the successor is based on */
  /* how the tree is built, so the result may be unpredictable if the   */
  /* keys are not distinct.                                             */
  Node *successor(Node *x) const
  {
    Node *y;
    
    if (x->right) {
      y = x->right;
      while (y && y->left) {
	y = y->left;
      }
      return y;
    }
    y = x->parent;
    while (y && x == y->right) {
      x = y;
      y = y->parent;
    }
    return y;
  }

  /* insert a node with the given key into the tree */
  void insert(KEYTYPE k)
  {
    Node *x, *y;

    x = insert_helper(k);
    while (x != root && !x->parent->color) {
      /* note: x->parent->parent != 0 since root is always black */
      if (x->parent == x->parent->parent->left) {
	y = x->parent->parent->right;
	if (y && !y->color) {
	  x->parent->color = y->color = 1;
	  x->parent->parent->color = 0;
	  x = x->parent->parent;
	} else {
	  if (x == x->parent->right) {
	    x = x->parent;
	    left_rotate(x);
	  }
	  x->parent->color = 1;
	  x->parent->parent->color = 0;
	  right_rotate(x->parent->parent);
	}
      } else {
	y = x->parent->parent->left;
	if (y && !y->color) {
	  x->parent->color = y->color = 1;
	  x->parent->parent->color = 0;
	  x = x->parent->parent;
	} else {
	  if (x == x->parent->left) {
	    x = x->parent;
	    right_rotate(x);
	  }
	  x->parent->color = 1;
	  x->parent->parent->color = 0;
	  left_rotate(x->parent->parent);
	}
      }
    }
    root->color = 1;
  }

  /* remove a node z */
  void remove(Node *z)
  {
    Node *x, *y, *p;

    y = (z->left && z->right) ? successor(z) : z;
    x = (y->left) ? y->left : y->right;
    if (x) {
      x->parent = y->parent;
    }
    if (!y->parent) {
      root = x;
    } else if (y == y->parent->left) {
      y->parent->left = x;
    } else {
      y->parent->right = x;
    }
    if (y != z) {
      z->copy_data(y);
    }
    for (p = y->parent; p; p = p->parent) {
      p->fix_num();
      
    }
    if (y->color) {
      delete_helper(x, y->parent);
    }

    delete y;
  }
};

int print(Node *t, int indent = 0, char prompt = ' ')
{
  int h, h2;
  if (t) {
    h = print(t->left, indent+4, '/');
    for (int i = 0; i < indent-1; i++) {
      putchar(' ');
    }
    putchar(prompt);
    printf("%3d\n", t->key);
    h2 = print(t->right, indent+4, '\\');
    if (h2 > h) {
      h = h2;
    }
    return h+1;
  } else {
    return 0;
  }
}

int main(void)
{
  RBTree tree;
  int i, t;
  char buffer[10];

  while (scanf("%1s", buffer) == 1 && buffer[0] != 'Q') {
    if (buffer[0] == '+') {
      scanf("%d", &i);
      tree.insert(i);
    } else if (buffer[0] == '-') {
      scanf("%d", &i);
      if (tree.search(i)) {
	tree.remove(tree.search(i));
      }
    } else if (buffer[0] == 'P') {
      t = print(tree.root);
      printf("height = %d\n", t);
    } else {
      fprintf(stderr, "Illegal command: '%s'\n", buffer);
    }
  }

  return 0;
}
