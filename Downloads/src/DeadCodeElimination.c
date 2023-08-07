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

int determinesRegister(Instruction *instruction, int reg) {
	int found = 0;
	
	switch (instruction -> opcode) {
		case LOADI:
			if (instruction -> field2 == reg) {
				found = 1;
			}
			break;
		case LOADAI:
			if (instruction -> field3 == reg) {
				found = 1;
			}
			break;
		case STOREAI: 
			break;
		case ADD: 
		case SUB: 
		case MUL: 
		case DIV: 
		case LSHIFTI: 
		case RSHIFTI: 
			if (instruction -> field3 == reg) {
				found = 1;
			}
	}
	return found;
}

int findStoreAI(Instruction *instruction, int reg, int offset) {
    if (instruction -> opcode == STOREAI && instruction -> field1 == reg && instruction -> field2 == offset) {
        return 1;
    }
    return 0;
}

Instruction *dead_code_elimination(Instruction *head) {

		/* STUDENTS - BEGIN */

		Instruction *ptr = head;

		// Sets OUTPUTAI instruction to critical
		while (ptr -> next != NULL) {
			(ptr -> opcode == OUTPUTAI) ? (ptr -> critical = 1) : (ptr -> critical = 0);
			ptr = ptr -> next;
		}

		(ptr -> opcode == OUTPUTAI) ? (ptr -> critical = 1) : (ptr -> critical = 0);
		
		
		while (ptr -> prev != NULL) {
			// Iterate until the next critical instruction
			if (ptr -> critical == 0) {
				ptr = ptr -> prev;
				continue;
			}

			// Initializes a ptr used to iterate backwards from an instruction
			Instruction *reversePtr = ptr -> prev;

						// Testing
			// printCritical(head);
			// printf("PTR REGISTERS: %d|%d\n ", ptr -> field1, ptr -> field2);
			// printf("REVERSEPTR REGISTERS: %d|%d\n ", reversePtr -> field1, reversePtr -> field2);

			// Identifies which function to call, based on the number of registers an instruction uses
			switch (ptr -> opcode) {
				case LOADI:
					break;
				case LOADAI:
					while (reversePtr != NULL) {
						if (reversePtr -> opcode == STOREAI && reversePtr -> field2 == ptr -> field1 && reversePtr -> field3 == ptr -> field2) {
							reversePtr -> critical = 1;
							break;
						}
		
						reversePtr = reversePtr -> prev;
					}
					break;
				case STOREAI:
					while (reversePtr != NULL) {
						if (reversePtr -> critical == 0) {
							determinesRegister(reversePtr, ptr -> field1) ? (reversePtr -> critical = 1) : (reversePtr -> critical = 0);
						}
						reversePtr = reversePtr -> prev;
					}
					break;
				case ADD: 
				case SUB: 
				case MUL: 
				case DIV: 
				case LSHIFTI: 
				case RSHIFTI:
					;
					Instruction *reversePtr2 = reversePtr;
					while (reversePtr != NULL) {
						if (reversePtr -> critical == 0) {
							determinesRegister(reversePtr, ptr -> field1) ? (reversePtr -> critical = 1) : (reversePtr -> critical = 0);
						}
						reversePtr = reversePtr -> prev;
					} 

					while (reversePtr2 != NULL) {
						if (reversePtr2 -> critical == 0) {
							determinesRegister(reversePtr2, ptr -> field2) ? (reversePtr2 -> critical = 1) : (reversePtr2 -> critical = 0);
						}
						reversePtr2 = reversePtr2 -> prev;
					} 
					break;
				case OUTPUTAI:	
					while (reversePtr != NULL) {
						if (reversePtr -> opcode == STOREAI && reversePtr -> field2 == ptr -> field1 && reversePtr -> field3 == ptr -> field2) {
							reversePtr -> critical = 1;
							break;
						}
						reversePtr = reversePtr -> prev;
					}
					break;
			}
			ptr = ptr -> prev;
		}

		Instruction *reversePtr = ptr;

		switch (ptr -> opcode) {
				case LOADI:
					break;
				case LOADAI:
					while (reversePtr != NULL) {
						if (reversePtr -> opcode == STOREAI) {
							findStoreAI(reversePtr, ptr -> field1, ptr -> field2) ? (reversePtr -> critical = 1) : (reversePtr -> critical = 0);
						}
						reversePtr = reversePtr -> prev;
					}
					break;
				case STOREAI:
					while (reversePtr != NULL) {
						determinesRegister(reversePtr, reversePtr -> field1) ? (reversePtr -> critical = 1) : (reversePtr -> critical = 0);
						reversePtr = reversePtr -> prev;
					}
					break;
				case ADD: 
				case SUB: 
				case MUL: 
				case DIV: 
				case LSHIFTI: 
				case RSHIFTI:
					while (reversePtr -> prev != NULL) {
						reversePtr = reversePtr -> prev;
						determinesRegister(reversePtr, reversePtr -> field1) ? (reversePtr -> critical = 1) : (reversePtr -> critical = 0);
					} 
					reversePtr = ptr -> prev;
					while (reversePtr -> prev != NULL) {
						reversePtr = reversePtr -> prev;
						determinesRegister(reversePtr, reversePtr -> field2) ? (reversePtr -> critical = 1) : (reversePtr -> critical = 0);
					} 
					break;
				case OUTPUTAI:
					while (reversePtr -> opcode != STOREAI && reversePtr != NULL) {
						reversePtr = reversePtr -> prev;
						if (findStoreAI(reversePtr, reversePtr -> field1, reversePtr -> field2)) {
							reversePtr -> critical = 1;
							break;
						}
						else {
							reversePtr -> critical = 0;
						}
					} 
		}


		if (head -> opcode == LOADI && head -> field2 == 0) {
			head -> critical = 1;;
		}

		Instruction *prev = head;
		Instruction *curr = prev -> next;

		while (curr != NULL) {
			if (curr->critical == 0) {
				prev->next = curr->next;
				free(curr);
				curr = prev->next;
			} 
			else {
				prev = curr;
				curr = curr->next;
			}
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

	head = dead_code_elimination(head);

	if (head) 
		PrintInstructionList(stdout, head);
	
	return EXIT_SUCCESS;
}

