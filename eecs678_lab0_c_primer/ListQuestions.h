#ifndef LIST_QUESTIONS_H
#define LIST_QUESTIONS_H

#include "LinkedList.h"
#include <stdbool.h>
/*
 * Problem: Determine if a linked list has a cycle
 *
 * Description:
 * Given the head of a linked list, determine if the list contains a cycle. A cycle occurs if a node can be
 * reached again by continuously following the `next` pointer. Internally, 'pos' is used to denote the index
 * of the node that the tail's next pointer is connected to, indicating a cycle. Note that 'pos' is not
 * accessible or passed as a parameter; it's only used for problem understanding and explanation.
 * Task:
 * Implement a function to check if the given linked list has a cycle. The function should return 'true' if a
 * cycle is present and 'false' otherwise.
 *
 * Prototype:
 * bool hasCycle(struct Node *head);
 */

static bool hasCycle(struct Node *head)
{
	//Placeholder return statement
	if (head == NULL) {
		return false;
	}
	struct Node* slow = head;
	struct Node* fast = head->next;


	while(fast != NULL && fast->next != NULL) 
	{
		if(slow == fast) {
			return true;
		}
		slow = slow->next;
		fast = fast->next->next;

	}
	return false;
}

/*
 * Problem: Merge Two Sorted Lists
 *
 * Description:
 * You are given the heads of two sorted linked lists, list1 and list2. Your task is to merge these two
 * lists into one single sorted list. The merged list should be constructed by splicing together the nodes
 * of the first two lists without creating new nodes, but by rearranging the nodes from the given lists.
 *
 * Task:
 * Implement a function that merges two sorted linked lists and returns the head of the newly merged sorted
 * linked list.
 *
 * Prototype:
 * struct Node* mergeTwoLists(struct Node* list1, struct Node* list2);
 *
 * Note:
 * Both list1 and list2 are sorted in non-decreasing order.
 */

static struct Node* mergeLists(struct Node* list1, struct Node* list2)
{
    if (list1 == NULL && list2 == NULL) {
		return NULL;
	}
	else if(list1 == NULL) {
		return list2;
	}
	else if (list2 == NULL) {
		return list1;
	}
	struct Node* head;
	struct Node* merger;
	struct Node* tail = NULL;
	if (list1->data < list2->data) {
		head = list1;
		merger = list2;
	}
	else {
		head = list2;
		merger = list1;
	}

    struct Node* temp = NULL;  
	struct Node* temp2 = NULL;
 
	//We always want the head to be the smaller valued list
	//And we don't want to lose the pointer to the head.
    if (list1->data <= list2->data) {
        head = list1;
        list1 = list1->next;
    } else {
        head = list2;
        list2 = list2->next;
    }

    tail = head; 
    while (list1 != NULL && list2 != NULL) {
		
        if (list1->data <= list2->data) {
            tail->next = list1; //asign tail the new value
            list1 = list1->next; //go foward one in list1
        } 
		else {
            tail->next = list2; //asign tail new value
            list2 = list2->next; //go foward one in list2
        }
        tail = tail->next; //go foward one in tail
    }

    if (list1) {
        tail->next = list1;
    } else {
       tail->next = list2;
    }

    return head; 
}


#endif
