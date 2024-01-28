/*
 *  Advanced Programming Course Assignment 1
 *  Linked List Implementation File
 *  Copyright (C) 2024  Roy Simanovich and Almog Shor
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "../include/LinkedList.h"
#include <stdio.h>
#include <stdlib.h>

PLinkedList createLinkedList() {
	PLinkedList list = (PLinkedList)malloc(sizeof(LinkedList));

	if (list == NULL)
	{
		perror("Error: createLinkedList() falied: malloc() failed");
		return NULL;
	}

	list->head = NULL;
	list->tail = NULL;

	return list;
}

int addNode(PLinkedList list, void *data) {
	if (list == NULL)
	{
		fprintf(stderr, "Error: addNode() failed: list is NULL\n");
		return 1;
	}

	else if (data == NULL)
	{
		fprintf(stderr, "Error: addNode() failed: data is NULL\n");
		return 1;
	}
	
	PNode node = (PNode)malloc(sizeof(Node));

	if (node == NULL)
	{
		perror("Error: addNode() falied: malloc() failed");
		return 1;
	}

	node->data = data;
	node->next = NULL;

	if (list->head == NULL)
	{
		list->head = node;
		list->tail = node;
	}

	else
	{
		list->tail->next = node;
		list->tail = node;
	}

	return 0;
}

int removeNode(PLinkedList list, void *data) {
	if (list == NULL)
	{
		fprintf(stderr, "Error: removeNode() failed: list is NULL\n");
		return 1;
	}

	else if (list->head == NULL)
	{
		fprintf(stderr, "Error: removeNode() failed: list is empty\n");
		return 1;
	}

	else if (data == NULL)
	{
		fprintf(stderr, "Error: removeNode() failed: data is NULL\n");
		return 1;
	}

	PNode node = list->head;
	PNode prev = NULL;

	while (node != NULL)
	{
		if (node->data == data)
		{
			if (prev == NULL)
				list->head = node->next;

			else
				prev->next = node->next;

			if (node == list->tail)
				list->tail = prev;

			free(node);
			return 0;
		}

		prev = node;
		node = node->next;
	}

	return 1;
}

void *getHead(PLinkedList list) {
	if (list->head == NULL)
	{
		fprintf(stderr, "Error: getHead() failed: list is NULL\n");
		return NULL;
	}
	
	return list->head->data;
}

void *getTail(PLinkedList list) {
	if (list->tail == NULL)
	{
		fprintf(stderr, "Error: getTail() failed: list is NULL\n");
		return NULL;
	}

	return list->tail->data;
}

int destroyLinkedList(PLinkedList list) {
	if (list == NULL)
	{
		fprintf(stderr, "Error: destroyLinkedList() failed: list is NULL\n");
		return 1;
	}

	PNode node = list->head;
	PNode next;

	while (node != NULL)
	{
		next = node->next;
		free(node);
		node = next;
	}

	free(list);

	return 0;
}