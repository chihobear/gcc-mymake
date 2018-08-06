#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include"Node.h"

//Create a new node.
struct Node* create_node()
{
	struct Node *p = (struct Node*)malloc(sizeof(struct Node));
	p->visited = 0;
	p->next_node = NULL;
	p->next_edge = NULL;
	p->cmd = NULL;
	return p;
}

//Add a node right after the head.
void add_node(struct Node *head, struct Node *p)
{
	p->next_node = head->next_node;
	head->next_node = p;
}

//Add an edge right after the node.
void add_edge(struct Node *head, struct Edge *p)
{
	head->next_edge = p;
}

//Given a string, find the position of the node in the linked stack.
struct Node* find_node(struct Node *head,char str[])
{
	struct Node *p = head->next_node;
	while(p != NULL)
	{
		if(strcmp(p->str, str) == 0)
		{
			return p;
		}
		p = p->next_node;
	}
	return NULL;
}

//Traversal the graph in post order, print out and process necessary commands.
void post_traversal(struct Node *p)
{
	struct Edge *edge_p;
	//If the node has not been visited.
	if(p->visited == 0)
	{
		edge_p = p->next_edge;
		while(edge_p != NULL)
		{
			post_traversal(edge_p->to);
			edge_p = edge_p->next;
		}
		//printf("%s  %s\n", p->str, p->cmd);
		if(p->cmd == NULL)
		{
			if(access(p->str, F_OK) != 0)
			{
				fprintf(stderr, "Missing dependency: %s\n", p->str);
				exit(1);
			}	
		}
		
		if(p->cmd != NULL && check_time(p) != 0)
		{
			printf("%s\n", p->cmd);
			if(system(p->cmd) != 0)
			{
				perror(p->cmd);
				exit(1);
			}
		}
		//Set this node to be visited.
		p->visited = 1;
	}
}

//Free the whole graph.
void free_whole(struct Node *head)
{
	struct Node *p = head->next_node;
	struct Node *q = NULL;
	free(head);
	
	while(p != NULL)
	{
		//Free edges after this node.
		free_edge(p->next_edge);
		q = p->next_node;
		//Free this node.
		if(p->cmd != NULL)
		{
			free(p->cmd);
		}
		free(p);
		p = q;
	}
}

//If the modified time of the target file is later than the source files, return 0. Otherwise, return 1.
int check_time(struct Node *target)
{
	struct stat buf;
	struct Edge *p = target->next_edge;
	time_t time_target = 0, time_child = 0;
	buf.st_mtime = 0;
	stat(target->str, &buf);
	time_target = buf.st_mtime;
	while(p != NULL)
	{
		buf.st_mtime = 0;
		stat(p->to->str, &buf);
		if(time_child < buf.st_mtime)
		{
			time_child = buf.st_mtime;
		}
		p = p->next;
	}
	if(time_child > time_target)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

