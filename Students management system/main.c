/*
 *  main.c
 *  Created on: Sep 25, 2023
 *  Author: Abdullah karkour
 */

#include "Platform_Types.h"
#include "Program.h"

// Create Data Buffer
element_type buffer1[FIFO_length];

int main ()
{
	extern FIFO_Buf_t fifo ;
	// initialization of FIFO
	FIFO_init(&fifo, buffer1, FIFO_length);

	uint8 temp_text[40] ;
	uint32 op_number ;

	while(1)
	{
		// show options
		Operation();
		gets(temp_text);
		op_number = atoi(temp_text);

		switch(op_number)
		{
		case 1 :
			Add_student_manually(&fifo);
			break;
		case 2 :
			Add_student_file(&fifo);
			break;
		case 3 :
			Find_roll(&fifo);
			break ;
		case 4:
			Find_Name(&fifo);
			break;
		case 5 :
			Find_course_student(&fifo);
			break;
		case 6 :
			Student_Tot_Number(&fifo);
			break;
		case 7 :
			Delete_Student(&fifo);
			break ;
		case 8:
			Update_Student(&fifo);
			break;
		case 9 :
			show_All_Student(&fifo);
			break;
		case 10:
			return 0;
		default :
			DPRINTF("\nEnter correct Number from choice\n");
		}
	}
}
