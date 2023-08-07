/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Spring 2023                              *
 *  Student Version                          *
 *  Author: Sean Thomas                                 *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"

/*
* Will traverse instructions in groups of 3, searching for a patten of LOADI, LOADI, and ADD | SUB | MUL instructions 
*/

Instruction *constant_folding(Instruction *head) {
		Instruction *mid;
		Instruction *tail;


		/* STUDENTS - BEGIN */
		Instruction *start = head;
		
		if (start -> next != NULL) {
			mid = start->next;
		}
		if (mid != NULL && mid -> next != NULL) {
			tail = mid->next;
		}

		while (tail != NULL) {
			if (start -> opcode == LOADI && mid -> opcode == LOADI) {
				if (tail -> opcode == ADD && (start -> field2 == tail -> field1) && (mid -> field2 == tail -> field2)) {
					tail -> opcode = LOADI;
					tail -> field1 = start -> field1 + mid -> field1;
					tail -> field2 = tail -> field3;
				}
				else if (tail -> opcode == SUB && (start -> field2 == tail -> field1) && (mid -> field2 == tail -> field2)) {
					tail -> opcode = LOADI;
					tail -> field1 = start -> field1 - mid -> field1;
					tail -> field2 = tail -> field3;
				}
				else if (tail -> opcode == MUL && (start -> field2 == tail -> field1) && (mid -> field2 == tail -> field2)) {
					tail -> opcode = LOADI;
					tail -> field1 = start -> field1 * mid -> field1;
					tail -> field2 = tail -> field3;
				}
			}

			start = mid;
			mid = tail;
			tail = tail -> next;
		}
		
		
		


 		/* STUDENTS - END */

	return head;
}

int main()
{
	Instruction *head;

	head = ReadInstructionList(stdin);
	if (!head) {
		WARNING("No instructions\n");
		exit(EXIT_FAILURE);
	}

	head = constant_folding(head);

	if (head) 
		PrintInstructionList(stdout, head);
	
	return EXIT_SUCCESS;
}

