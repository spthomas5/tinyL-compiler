/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Spring 2023                              *
 *  Student Version                          *
 *  Author: Sean Thomas                                  *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "InstrUtils.h"
#include "Utils.h"

/*
* Will return log2(n) if it is an integer (meaning n is a power of 2), and -1 otherwise
*/
int int_log2(int n) {

	int s = 1, i = 0;

	while (s < n) {
		s = s << 1;
		i++;
	}

	if (s == n)
		return i;
	else
		return -1;

}

/*
* Will traverse instructions in groups of two, searching for a patten of LOADI and MUL | DIV instructions 
* where LOADI is a power of 2, and collapse these instructions into on shift operation of LSHIFTI | RSHIFTI
*/
Instruction *strength_reduction(Instruction *head) {

		/* STUDENTS - BEGIN */

		Instruction *start = head;
		Instruction *mid;
		Instruction *tail;

		if (start -> next != NULL) {
			mid = start->next;
		}
		if (mid != NULL && mid -> next != NULL) {
			tail = mid->next;
		}

		while (tail != NULL) {
			if (start -> opcode == LOADI && int_log2(start -> field1) != -1 && mid -> opcode == DIV) {
				mid -> opcode = RSHIFTI;
				mid -> field2 = int_log2(start -> field1);
			}
			else if (start -> opcode == LOADI && int_log2(start -> field1) != -1 && tail -> opcode == DIV) {
				tail -> opcode = RSHIFTI;
				tail -> field2 = int_log2(start -> field1);
			}
			
			else if (start -> opcode == LOADI && int_log2(start -> field1) != -1 && mid -> opcode == MUL) {
				mid -> opcode = LSHIFTI;
				mid -> field2 = int_log2(start -> field1);	
			}
			else if (start -> opcode == LOADI && int_log2(start -> field1) != -1 && tail -> opcode == MUL) {
				tail -> opcode = LSHIFTI;
				tail -> field2 = int_log2(start -> field1);	
			}

			start = tail;
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

	head = strength_reduction(head);

	if (head) 
		PrintInstructionList(stdout, head);
	
	return EXIT_SUCCESS;
}

