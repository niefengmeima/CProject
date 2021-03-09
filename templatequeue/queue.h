#ifndef QUEUE_H__
#define QUEUE_H__

#include <stdio.h>
#include <stdlib.h>

#define MAKE_QUEUE(ETYPE)                                \
	typedef struct node                                  \
	{                                                    \
		ETYPE value;                                     \
		struct node *next;                               \
	} Node;                                              \
	typedef struct queue                                 \
	{                                                    \
		size_t size;                                     \
		Node *head;                                      \
		Node *tail;                                      \
	} Queue;                                             \
	void push(Queue *queue, ETYPE value)                 \
	{                                                    \
		Node *newNode = (Node *)malloc(sizeof(Node));    \
		newNode->value = value;                          \
		if (queue->tail == NULL)                         \
		{                                                \
			queue->tail = newNode;                       \
			queue->head = queue->tail;                   \
			++(queue->size);                             \
			return;                                      \
		}                                                \
		queue->tail->next = newNode;                     \
		queue->tail = newNode;                           \
		queue->tail->next = NULL;                        \
		++(queue->size);                                 \
	}                                                    \
	ETYPE pop(Queue *queue)                              \
	{                                                    \
		if (queue->head == NULL)                         \
		{                                                \
			fprintf(stderr, "队列为空，还没有元素！\n"); \
			return;                                      \
		}                                                \
		Node *cur = queue->head;                         \
		if (queue->size == 1)                            \
		{                                                \
			queue->head = NULL;                          \
			queue->tail = NULL;                          \
			ETYPE result = cur->value;                   \
			free(cur);                                   \
			--(queue->size);                             \
			printf("队列已经清空！\n");                  \
			return result;                               \
		}                                                \
		queue->head = queue->head->next;                 \
		ETYPE result = cur->value;                       \
		free(cur);                                       \
		--(queue->size);                                 \
		return result;                                   \
	}                                                    \
	int is_empty(Queue *queue)                           \
	{                                                    \
		return queue->size == 0;                         \
	}                                                    \
	int get_size(Queue *queue)                           \
	{                                                    \
		return queue->size;                              \
	}

#endif