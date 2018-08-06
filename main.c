/*
*  Project: mymake
*  Author: Chenghao Xiong
*  Purpose: Mimic "make".
*  Example command parameters: test0 spellcheck
*  test0: @target spellcheck.exe : util.h spellcheck.h spellcheck.c
          @cmd gcc -g spellcheck.c -o spellcheck
*  Files exist: util.h spellcheck.h spellcheck.c
*  Example output: gcc -g spellcheck.c -o spellcheck
*  Generated file: spellcheck.exe
*  data structure:      ------
                       | head| 
                        ------
                          |
						------   ------   ------
	                   | node|->| edge|->| edge|
	                   ------   ------   ------
					      |      
					   ------   ------   ------
	                   | node|->| edge|->| edge|
	                   ------   ------   ------
					      |       	 
						------ 
	                   | node|
	                   ------   
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"Node.h"
#include"Edge.h"


#define TargetFile 1
#define ComponentFile 2
#define Command 3
#define Colon 4

int exam_word(char str[]);
void initialize(char *p, int num);

int main(int argc, char *argv[]) {
	int flag = 0, addNode = 0, command_count = 1, token_number = 0, len_previous = 0, len = 0;
	char str[65];
	struct Node *head = NULL, *node_p = NULL, *target = NULL;
	struct Edge *edge_p = NULL;
	FILE *fp;
	//If the number of the input arguments is more than 2, print error.
	if(argc != 3)
	{
		fprintf(stderr, "Usage mymake <input file> <target>\n");
		exit(1);
	}
	//Create an empty head node.
	head = create_node();
	fp = fopen(argv[1], "r");
	//If cannot open the file, print error.
	if(fp == NULL)
	{
		perror(argv[1]);
		exit(1);
	}
	
	//Read each word in the file and process.
	while(fscanf(fp, "%s", str) != EOF)
	{
		//If the first word read in is not "@target", print eror.
		if(token_number == 0)
		{
			if(strcmp("@target", str))
			{
				fprintf(stderr, "Missing @target\n");
				exit(1);
			}
		}
		
		//Process when meeting "@target".
		if(strcmp(str, "@target") == 0)
		{
			if(command_count == 0)
			{
				fprintf(stderr, "Missing command\n");
				exit(1);
			}
			flag = TargetFile;
			command_count = 0;
			token_number++;
		}
		
		//Process when meeting "@cmd".
		else if(strcmp(str, "@cmd") == 0)
		{
			flag = Command;
			token_number++;
		}
		else
		{
			//If the word started by "@", print the error.
			if(exam_word(str) == 1)
			{
				fprintf(stderr, "Illegal word: %s\n", str);
				exit(1);
			}
			//Process when meeting the target file.(create a node and put it in the linked list)
			if(flag == TargetFile)
			{
				node_p = create_node();
				strcpy(node_p->str, str);
				add_node(head, node_p);
				target = node_p;
				flag = Colon;
			}
			//Process when meeting the source file.
			//(Create a node if not already exsist. And create an edge and append it to the target node)
			else if(flag == ComponentFile)
			{
				addNode = 0;
				if(find_node(head, str) == NULL)
				{
					addNode = 1;
					node_p = create_node();
					strcpy(node_p->str, str);
				}
				else
				{
					node_p = find_node(head, str);
				}
				//Create an edge from the target to the source.
				edge_p = create_edge();
				edge_p->from = target;
				edge_p->to = node_p;
				if(target->next_edge == NULL)
				{
					add_edge(target, edge_p);
				}
				else
				{
					add(find_last(target->next_edge), edge_p);
				}
				//If the node not exsist then create.
				if(addNode == 1)
				{
					add_node(head, node_p);
				}
			}
			
			//Process when meeting commands.
			else if(flag == Command)
			{
				len = strlen(str);
				if(target->cmd == NULL)
				{
					len_previous = 0;
				}
				else
				{
					len_previous = strlen(target->cmd);
				}
				if(command_count == 0)
				{
				    target->cmd = (char*)realloc(target->cmd, (len + len_previous + 1) * sizeof(char));
					initialize(target->cmd, len + len_previous + 1);
				}
				else
				{
					target->cmd = (char*)realloc(target->cmd, (len + len_previous + 2) * sizeof(char));
					strcat(target->cmd, " ");
				}
				strcat(target->cmd, str);
				command_count++;
			}
			//Process when meeting ":".
			else if(flag == Colon)
			{
				//If there is no ":", print the error.
				if(strcmp(str, ":") != 0)
				{
					fprintf(stderr, "Missing :\n");
					exit(1);
				}
				//If meeting a ":", set the "flag" to "ComponentFile" to tell what should to do next.
				else
				{
					flag = ComponentFile;
				}
			}
			token_number++;
		}
	}
	//If there is no word after "@cmd", print the error.
	if(command_count == 0)
	{
		fprintf(stderr, "Missing command\n");
		exit(1);
	}

    //Find the position of the target node from the input.
	node_p = find_node(head, argv[2]);
	//If there is no such target.
	if(node_p == NULL)
	{
		fprintf(stderr, "Target not found: %s\n", argv[2]);
		exit(1);
	}
	//Print out all the nodes.
	post_traversal(node_p);
	
	//Free the memory.
	free_whole(head);
	fclose(fp);
	
	return 0;
}

//Exam if the word starts with "@". If so, return 1. Return 0 otherwise.
int exam_word(char str[])
{
	if(str != NULL)
	{
		if(str[0] != '@')
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	return 0;
}

//Initialize a string. Set every element into '\0'.
void initialize(char *p, int num)
{
	int i = 0;
	for(i = 0;i < num;i++)
	{
		p[i] = '\0';
	}
}