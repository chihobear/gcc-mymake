#ifndef FLAG2
#define FLAG2
#include"Node.h"
struct Edge{
	struct Node *from;
	struct Node *to;
	struct Edge *next;
};

struct Edge* create_edge();
void add(struct Edge *previous, struct Edge *p);
struct Edge* find_last(struct Edge *head);
void free_edge(struct Edge *head);

#endif

