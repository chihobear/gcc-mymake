#include<stdlib.h>
#include"Edge.h"

//Create an edge.
struct Edge* create_edge()
{
	struct Edge *p = (struct Edge*)malloc(sizeof(struct Edge));
	p->next = NULL;
	p->from = NULL;
	p->to = NULL;
	return p;	
}

//Add an edge after the edge.
void add(struct Edge *previous, struct Edge *p)
{
	previous->next = p;
}

//Find the last edge in this edge linked list.
struct Edge* find_last(struct Edge *head)
{
	struct Edge *p = head;
	if(head == NULL)
	{
		return NULL;
	}
	while(p->next != NULL)
	{
		p = p->next;
	}
	return p;
}

//Free the whole edge linked list.
void free_edge(struct Edge *head)
{
	struct Edge *p = head;
	struct Edge *q = NULL;
	while(p != NULL)
	{
		q = p->next;
		free(p);
		p = q;
	}
}

