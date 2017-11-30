#ifndef SLIST_H
#define SLIST_H

typedef void *Var;

typedef struct _Node Node;

struct _Node{
  Var data;
  Node* next;
};

typedef int (*compare_function)(const Var val1,const Var val2);

typedef int (*equal_function)(const Var val1,const Var val2);

Node *new_node(Var data);
Node *n_prepend(Node *list, Var data);
Node *n_append(Node *list, Var data);
Node *prepend(Node **list, Var data);
Node *append(Node **list, Var data);
Node *next_node(Node *node);
void free_node(Node *list);
Var get_data(Node *node);
void set_data(Node *node, Var data);
Node *get_at(Node *list, unsigned int index);
unsigned int length(Node *list);
unsigned int remove_node(Node **list, Node *node);
int remove_data(Node **list,
                         equal_function callback,
                         Var data);
Node *find_data(Node *list,
                equal_function callback,
                Var data);
#endif
