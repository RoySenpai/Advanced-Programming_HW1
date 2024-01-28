/*
 *  Advanced Programming Course Assignment 1
 *  Linked List Header File
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

#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

/*************************/
/* Structures Definition */
/*************************/

/*
 * @brief A structure representing a node in a singly linked list.
 * @param data A pointer to the data stored in the node.
 * @param next A pointer to the next node in the list.
 * @note The data pointer is void, so it can be used to store any type of data.
 * @note The next pointer is NULL if the node is the last node in the list.
*/
typedef struct _Node {
	/*
	 * @brief A pointer to the data stored in the node.
	 * @note The data pointer is void, so it can be used to store any type of data.
	 * @note The data pointer cannot be NULL.
	 * @note The data pointer must be allocated in the heap.
	 * @note The data pointer must be freed by the user.
	 * @note The data pointer must be casted to the correct type by the user, before it is used.
	*/
	void *data;

	/*
	 * @brief A pointer to the next node in the list.
	 * @warning Users should not change the next pointer directly.
	 * @note The next pointer is NULL if the node is the last node in the list.
	*/
	struct _Node *next;
} Node, *PNode;

/*
 * @brief A structure representing a singly linked list.
 * @param head A pointer to the first node in the list.
 * @param tail A pointer to the last node in the list.
 * @note The head pointer is NULL if the list is empty.
 * @note The tail pointer is NULL if the list is empty.
*/
typedef struct _LinkedList {

	/*
	 * @brief A pointer to the first node in the list.
	 * @warning Users should not change the head pointer directly.
	 * @note The head pointer is NULL if the list is empty.
	*/
	PNode head;

	/*
	 * @brief A pointer to the last node in the list.
	 * @warning Users should not change the tail pointer directly.
	 * @note The tail pointer is NULL if the list is empty.
	 * @note The tail pointer exists for efficiency reasons, so the user can add a node to the end of the list in O(1) time.
	*/
	PNode tail;
} LinkedList, *PLinkedList;


/*************************/
/* Functions Declaration */
/*************************/

/*
 * @brief Creates a new linked list.
 * @return A pointer to the new linked list.
 * @note The returned pointer must be freed by the user using the destroyLinkedList() function.
*/
PLinkedList createLinkedList();

/*
 * @brief Adds a new node to the end of the list.
 * @param list A pointer to the list.
 * @param data A pointer to the data to be stored in the new node.
 * @return 0 if the node was added successfully, 1 otherwise.
 * @note The data pointer cannot be NULL.
 * @note The data pointer must be allocated in the heap.
 * @note The data pointer must be freed by the user.
*/
int addNode(PLinkedList list, void *data);

/*
 * @brief Removes a node from the list.
 * @param list A pointer to the list.
 * @param data A pointer to the data stored in the node to be removed.
 * @return 0 if the node was removed successfully, 1 otherwise.
 * @note The data pointer cannot be NULL.
 * @note The data pointer must be casted to the correct type by the user, before it is used.
 * @note The data pointer must be allocated in the heap.
 * @note The data pointer must be freed by the user.
*/
int removeNode(PLinkedList list, void *data);

/*
 * @brief Returns the data stored in the head of the list.
 * @param list A pointer to the list.
 * @return A pointer to the data stored in the head of the list.
*/
void *getHead(PLinkedList list);

/*
 * @brief Returns the data stored in the tail of the list.
 * @param list A pointer to the list.
 * @return A pointer to the data stored in the tail of the list.
*/
void *getTail(PLinkedList list);

/*
 * @brief Destroys a linked list.
 * @param list A pointer to the list.
 * @return 0 if the list was destroyed successfully, 1 otherwise.
 * @note If the list is not empty, the function will release all the nodes in the list automatically.
 * @note The data pointers stored in the nodes will not be freed automatically, and it is the user's responsibility to free them.
*/
int destroyLinkedList(PLinkedList list);


#endif // _LINKED_LIST_H