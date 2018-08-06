#ifndef FLAG1
#define FLAG1
#include"Edge.h"
struct Node{
	int visited;
	char str[65];
	char *cmd;
	struct Node *next_node;
	struct Edge *next_edge;
};

struct Node* create_node();
void add_node(struct Node *head, struct Node *p);
void add_edge(struct Node *head, struct Edge *p);
struct Node* find_node(struct Node *head, char str[]);
void post_traversal(struct Node *p);
void free_whole(struct Node *head);
int check_time(struct Node *target);
#endif

