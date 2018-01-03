#include <stdlib.h>
#include "../include/slist.h"

Node *new_node(Var data){
  Node *res;
  res = malloc(sizeof(Node));
  if (!res) return NULL;
  res->data = data;
  res->next = NULL;
  return res;
}

Node *n_prepend(Node *list, Var data){
  if (!list) return NULL;
  if (list->data==NULL)
  {
    list->data = data;
    return list;
  }
  return prepend(&list,data);
}

Node *n_append(Node *list, Var data){
  if (!list) return NULL;
  if (list->data==NULL)
  {
    list->data = data;
    return list;
  }
  return append(&list,data);
}

Node *prepend(Node **list, Var data){
  Node * res;
  res = malloc(sizeof(Node));
  if (!res) return NULL;
  res->data = data;
  res->next = *list;
  *list = res;
  return res;
}

Node *append(Node **list, Var data){
  Node * res;
  res = malloc(sizeof(Node));
  if (!res) return NULL;
  res->data = data;
  res->next = NULL;
  if (!*list) {
    *list = res;
  }else{
    Node* tmp;
    tmp = *list;

    while(tmp->next != NULL)
      tmp = tmp->next;
    tmp->next = res;
  }
  return res;
}

Node *next_node(Node *node){
  return !node ? NULL : node->next;
}

void free_node(Node *list){
  if (!list) return;

  Node * tmp;

  while (list!=NULL){
    tmp = list->next;
    free(list);
    list = tmp;
  }

}

Var get_data(Node *node){
  return !node ? NULL : node->data;
}

Var clone_data(Node* node){
  if(!node) return NULL;
  if(!node->data) return NULL;
  Var res = malloc(sizeof(node->data));
  memcpy(res,node->data,sizeof(node->data));
  return res;
}

void set_data(Node *node, Var data){
  if (!node) return;
  node->data = data;
}

Node *get_at(Node *list, unsigned int index){
  Node * tmp;
  unsigned int position = 0;
  tmp = list;
  for(position = 0;position<index;position++) {
    if (!tmp) return NULL;
    tmp = tmp->next;
  }
  return tmp;
}

unsigned int length(Node *list){
  if (!list) return 0;
  else return 1+length(list->next);
}

unsigned int remove_node(Node **list, Node *node){
  if (*list == NULL || node == NULL) return 0;
  if (*list==node) {
    *list = node->next;
  }else{
    Node * tmp;
    tmp = *list;
    while (tmp!=NULL && tmp->next!=node)
      tmp=tmp->next;
    if (tmp==NULL) return 0;
    tmp->next = node->next;
  }
  free(node);
  return 1;
}

int remove_data(Node **list,
            equal_function callback,
            Var data){
  if (!*list) return -1;
  Node * tmp;
  Node **root;
  unsigned int position = -1;

  root = list;

  for (position = 0; *root!=NULL; ++position) {
    if (callback((*root)->data,data)!=0) {
      tmp = (*root)->next;
			free(*root);
			*root = tmp;
    }else {
			root = &((*root)->next);
		}
  }

  return position;
}

Node *find_data(Node *list,
                equal_function callback,
                Var data){
  Node *tmp;
	tmp=list;
  while(tmp!= NULL){
		if (callback(tmp->data, data)!=0) {
      return tmp;
		}
    tmp = tmp->next;
  }
	return NULL;
}
