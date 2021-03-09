#include "queue.h"
#include <stdio.h>

MAKE_QUEUE(int);

void traverse_queue(Queue *queue)
{
	if (queue->size == 0)
	{
		printf("队列没有元素，无需遍历\n");
		return;
	}
	Node *current = queue->head;
	printf("%d", current->value);
	while (current->next != NULL)
	{
		current = current->next;
		printf("-->%d", current->value);
	}
	printf("\n");
}

int main()
{
	Queue *queue = (Queue *)malloc(sizeof(Queue));
	queue->head = NULL;
	queue->size = 0;
	queue->tail = NULL;
	push(queue, 0);
	push(queue, 2);
	push(queue, 3);
	traverse_queue(queue);
	free(queue);
	return 0;
}