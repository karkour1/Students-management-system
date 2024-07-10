/*
 *  Prorgrame.c
 *  Created on: Sep 25, 2023
 *  Author: Abdullah karkour
 */

#include "Platform_Types.h"
#include "Program.h"
#include <string.h>
#include<stdlib.h>

FIFO_Buf_t fifo ;
uint8 temp_text[40] ;

/*******************************   				 APIS  					  **********************************/
FIFO_Buffer_status FIFO_init(FIFO_Buf_t* fifo , element_type* buf , uint32 length)
{
	if(buf && length)
	{
		fifo->length = length ;
		fifo->count = 0;
		fifo->base = buf;
		fifo->tail = fifo->base ;
		fifo->head = fifo->base ;

		return FIFO_no_error ;
	}

	return FIFO_null ;
}
void Operation ()
{
	DPRINTF("\nChoose the task that you want to perform \n");
	DPRINTF("1. Add The Student Details Manually \n");
	DPRINTF("2. Add The Student Details From Text File\n");
	DPRINTF("3. Find The Student Details by roll number \n");
	DPRINTF("4. Find The Student Details by name \n");
	DPRINTF("5. Find The Student Details by Course Id \n");
	DPRINTF("6. Find The Total number of students \n");
	DPRINTF("7. Delete The Student Details by roll number \n");
	DPRINTF("8. Update The Student Details by roll number \n");
	DPRINTF("9. Show All informations \n");
	DPRINTF("10. To Exist \n");
	DPRINTF("Enter your choice to perform the task : ");
}
void Add_student_manually(FIFO_Buf_t* fifo)
{
	if(FIFO_is_full(fifo) == FIFO_no_error)
	{
		DPRINTF("Enter the Student Name : ");
		gets(fifo->head->name);

		DPRINTF("Enter the Student roll : ");
		gets(temp_text);
		fifo->head->roll = atoi(temp_text);

		DPRINTF("Enter the Student GPA : ");
		gets(temp_text);
		fifo->head->GPA = atof(temp_text);

		uint32 C_count =0 , i=0;
		DPRINTF("Enter number of courses : ");
		gets(temp_text);
		C_count = atoi(temp_text);

		for( i =0 ; i<C_count ; i++)
		{
			DPRINTF("Enter the course %d Id : " ,i+1);
			gets(temp_text);
			fifo->head->course_Id[i] = atoi(temp_text);
		}

		// Check the roll number
		element_type* temp = Get_Roll_loction(fifo, fifo->head->roll);
		if(!temp)
		{
			// Add Student Details to buffer
			fifo->count ++ ;

			// Circler FIFO move
			FIFO_Circler_move(fifo, &(fifo->head));

			DPRINTF("\nADD STUDENT DONE \n");
		}
		else
		{
			DPRINTF("\nADD STUDENT FAILED , The Roll Number is Exist \n");
		}
	}
	else
	{
		DPRINTF("\nThe Student system is Full\n");
	}
}
void Add_student_file(FIFO_Buf_t* fifo)
{
	if(FIFO_is_full(fifo) == FIFO_no_error)
	{
		FILE* fptr;
		fptr = fopen("Students.txt","r");
		fflush(stdin);
		fflush(stdout);
		uint8 ch[500];
		element_type New_student ;
		uint32 i;

		if( fptr == NULL)
		{
			DPRINTF("...Cann't open the file... \n");
		}
		else
		{
			while(fgets(ch,500,fptr))
			{
				fscanf(fptr,"%d %s %f %d %d %d %d %d",&New_student.roll,&New_student.name,
						&New_student.GPA, &New_student.course_Id[0], &New_student.course_Id[1], &New_student.course_Id[2],
						&New_student.course_Id[3], &New_student.course_Id[4]);
				fflush(stdin);
				fflush(stdout);
				// Check the roll number
				element_type* temp = Get_Roll_loction(fifo, New_student.roll);
				if(!temp)
				{
					// Add Student Details to buffer
					fifo->count ++ ;

					// Add Data
					strcpy(fifo->head->name, New_student.name);
					fifo->head->roll = New_student.roll;
					fifo->head->GPA = New_student.GPA;
					uint32  i=0;
					for( i =0 ; i<10 ; i++)
					{
						fifo->head->course_Id[i] = New_student.course_Id[i];
					}

					// Circler FIFO move
					FIFO_Circler_move(fifo, &(fifo->head));

					DPRINTF("\nADD STUDENT DONE \n");
				}
				else
				{
					DPRINTF("\nADD STUDENT FAILED , The Roll Number is Exist \n");
				}
			}
		}
		fclose(fptr);
	}
	else
	{
		DPRINTF("\nThe Student system is Full\n");
	}
}
void Find_roll(FIFO_Buf_t* fifo)
{
	// Check if FIFO is exist
	if(!fifo->base || !fifo->tail || !fifo->head)
	{
		DPRINTF("FIFO is NULL\n");
	}

	// Check if FIFO is empty
	else if( fifo->count == 0)
	{
		DPRINTF("FIFO is empty\n");
	}

	else
	{
		uint32 roll ;
		element_type* temp ;
		DPRINTF("Enter the Student roll : ");
		gets(temp_text);
		roll = atoi(temp_text);

		temp = Get_Roll_loction(fifo, roll);
		if(temp)
		{
			DPRINTF("\nThe Student Details : \n");
			DPRINTF("Name : %s\n" , temp->name);
			DPRINTF("The roll : %d\n" , temp->roll);
			DPRINTF("The GPA : %.2f\n" , temp->GPA);
			DPRINTF("The Student courses : \n");
			uint32 i ;
			for(i=0 ; temp->course_Id[i] != 0 ; i++)
			{
				DPRINTF("\tThe Course %d Id : %d\n" ,i+1 ,temp->course_Id[i]);
			}
		}
		else
		{
			DPRINTF("\nThe Roll does not Exist\n");
		}
	}

}
void Find_Name(FIFO_Buf_t* fifo)
{
	// Check if FIFO is exist
	if(!fifo->base || !fifo->tail || !fifo->head)
	{
		DPRINTF("FIFO is NULL\n");
	}

	// Check if FIFO is empty
	else if( fifo->count == 0)
	{
		DPRINTF("\nFIFO is empty\n");
	}

	else
	{
		DPRINTF("Enter the Student Name : ");
		gets(temp_text);

		element_type* temp = fifo->tail;
		uint32 i , flag = 1;

		for(i=0 ; i< fifo->count ;i++)
		{
			if(!strcmp(temp->name, temp_text))
			{
				flag =0 ;
				DPRINTF("\nThe Student Details : \n");
				DPRINTF("Name : %s\n" , temp->name);
				DPRINTF("The roll : %d\n" , temp->roll);
				DPRINTF("The GPA : %.2f\n" , temp->GPA);
				uint32 i ;
				for(i=0 ; temp->course_Id[i] != 0 ; i++)
				{
					DPRINTF("The course %d Id : %d\n" ,i+1, temp->course_Id[i]);
				}
			}

			FIFO_Circler_move(fifo, &temp);
		}
		if(flag)
			DPRINTF("\nThe Name does not Exist\n");
	}
}
void Find_course_student(FIFO_Buf_t* fifo)
{
	// Check if FIFO is exist
	if(!fifo->base || !fifo->tail || !fifo->head)
	{
		DPRINTF("FIFO is NULL\n");
	}

	// Check if FIFO is empty
	else if( fifo->count == 0)
	{
		DPRINTF("FIFO is empty\n");
	}

	else
	{
		uint32 Id ;
		DPRINTF("Enter the Course Id : ");
		gets(temp_text);
		Id = atoi(temp_text);

		element_type* temp = fifo->tail;
		uint32 i , j , Count_student = 0;

		DPRINTF("\nThe Students in course Details : \n");
		for(i=0 ; i< fifo->count ;i++)
		{
			for(j=0 ; temp->course_Id[j] != 0 ; j++)
			{
				if(temp->course_Id[j] == Id)
				{
					Count_student ++ ;
					DPRINTF("\tThe Student %d Details : \n" , Count_student);
					DPRINTF("\tName : %s\n" , temp->name);
					DPRINTF("\tThe roll : %d\n" , temp->roll);
					DPRINTF("\tThe GPA : %.2f\n" , temp->GPA);
				}
			}

			FIFO_Circler_move(fifo, &temp);
		}
		if(Count_student)
		{
			DPRINTF("\nThe Number of student take course with Id : %d  is %d student\n" , Id , Count_student);
		}
		else
		{
			DPRINTF("\nThere are no student take course with Id : %d\n" , Id);
		}
	}
}
element_type* Get_Roll_loction(FIFO_Buf_t* fifo ,uint32 roll )
{
	// Check if FIFO is exist
	if(!fifo->base || !fifo->tail || !fifo->head)
	{
		return NULL ;
	}

	// Check if FIFO is empty
	if( fifo->count == 0)
	{
		return NULL ;
	}

	element_type* temp = fifo->tail;
	uint32 i ;

	for(i=0 ; i< fifo->count ;i++)
	{
		// check the roll
		if (temp->roll == roll)
			return temp ;

		FIFO_Circler_move(fifo, &temp);
	}

	return NULL;
}
FIFO_Buffer_status FIFO_is_full (FIFO_Buf_t*  fifo )
{
	// Check if FIFO is exist
	if(!fifo->base || !fifo->tail || !fifo->head)
	{
		DPRINTF("FIFO is NULL\n");
		return FIFO_null ;
	}
	// Check if FIFO is full
	if( fifo->length == fifo->count)
	{
		DPRINTF("FIFO is full\n");
		return FIFO_full ;
	}

	// the FIFO is not Full and can store data
	return FIFO_no_error ;

}
void FIFO_Circler_move(FIFO_Buf_t*  fifo , element_type** pToMove)
{
	if ((*pToMove)== (fifo->base + (fifo->length * sizeof(element_type))))
		(*pToMove) = fifo->base ;
	else
		(*pToMove)++;
}
void Student_Tot_Number(FIFO_Buf_t* fifo)
{
	// Check if FIFO is exist
	if(!fifo->base || !fifo->tail || !fifo->head)
	{
		DPRINTF("FIFO is NULL\n");
	}

	// Check if FIFO is empty
	else if( fifo->count == 0)
	{
		DPRINTF("FIFO is empty\n");
	}
	else
	{
		DPRINTF("The total number of Student is %d \n" , fifo->count);

	}
}
void Delete_Student(FIFO_Buf_t* fifo)
{
	// Check if FIFO is exist
	if(!fifo->base || !fifo->tail || !fifo->head)
	{
		DPRINTF("FIFO is NULL\n");
	}

	// Check if FIFO is empty
	else if( fifo->count == 0)
	{
		DPRINTF("FIFO is empty\n");
	}
	else
	{
		uint32 roll ;
		element_type* temp ;
		DPRINTF("Enter the Student roll : ");
		gets(temp_text);
		roll = atoi(temp_text);

		temp = Get_Roll_loction(fifo, roll);

		if(!temp)
		{
			DPRINTF("\nThe Student does not Exist : \n");
		}
		else
		{
			while(temp && (temp+1 != fifo->head))
			{
				// Swap data
				strcpy(temp->name, (temp+1)->name);
				temp->roll = (temp+1)->roll ;
				temp->GPA  = (temp+1)->GPA ;
				uint32 i ;
				for(i=0 ; temp->course_Id[i] != 0 ; i++)
				{
					temp->course_Id[i] = (temp+1)->course_Id[i] ;
				}

				FIFO_Circler_move(fifo, &temp);
			}

			// update head
			fifo->head = temp ;
			fifo->count--;
			DPRINTF("\nStudent Data has been deleted !!\n");
		}
	}
}
void Update_Student(FIFO_Buf_t* fifo)
{
	// Check if FIFO is exist
	if(!fifo->base || !fifo->tail || !fifo->head)
	{
		DPRINTF("FIFO is NULL\n");
	}

	// Check if FIFO is empty
	else if( fifo->count == 0)
	{
		DPRINTF("FIFO is empty\n");
	}
	else
	{
		uint32 roll , task_number ;
		element_type* temp ;
		DPRINTF("Enter the Student roll : ");
		gets(temp_text);
		roll = atoi(temp_text);

		temp = Get_Roll_loction(fifo, roll);
		if(!temp)
		{
			DPRINTF("\nThe Student does not Exist : \n");
		}
		else
		{
			DPRINTF("\nChoose the task that you want to perform \n");
			DPRINTF("1. Update Name \n");
			DPRINTF("2. Update Roll\n");
			DPRINTF("3. Update GPA \n");
			DPRINTF("4. Update Courses Id \n");
			DPRINTF("the task that you want to perform :  ");
			gets(temp_text);
			task_number = atoi(temp_text);

			uint32  i=0 , C_count =0 ;
			switch(task_number)
			{
			case 1 :
				DPRINTF("Enter the Student Name : ");
				gets(temp->name);
				DPRINTF("\nStudent is Updated \n");
				break;
			case 2 :
				DPRINTF("Enter the Student roll : ");
				gets(temp_text);
				if(!Get_Roll_loction(fifo, atoi(temp_text)))
				{
					temp->roll = atoi(temp_text);
					DPRINTF("\nStudent is Updated \n");
				}
				else
				{
					DPRINTF("\nUpdate Student Failed , The Roll Number is Exist \n");
				}
				break;
			case 3 :
				DPRINTF("Enter the Student GPA : ");
				gets(temp_text);
				temp->GPA = atof(temp_text);
				DPRINTF("\nStudent is Updated \n");
				break;
			case 4 :
				DPRINTF("Enter number of courses : ");
				gets(temp_text);
				C_count =atoi(temp_text) ;
				for( i =0 ; i< C_count ; i++)
				{
					DPRINTF("Enter the course %d Id : " ,i+1);
					gets(temp_text);
					temp->course_Id[i] = atoi(temp_text);
				}
				DPRINTF("\nStudent is Updated \n");
				break;
			default :
				DPRINTF("Enter correct number from choice!\n");
			}
		}
	}
}
void show_All_Student(FIFO_Buf_t* fifo)
{
	// Check if FIFO is exist
	if(!fifo->base || !fifo->tail || !fifo->head)
	{
		DPRINTF("FIFO is NULL\n");
	}

	// Check if FIFO is empty
	else if( fifo->count == 0)
	{
		DPRINTF("FIFO is empty\n");
	}
	else
	{
		element_type* temp = fifo->tail;
		uint32 i ;

		DPRINTF("\nAll Student in the system :  \n");
		for(i=0 ; i< fifo->count ;i++)
		{
			DPRINTF("\nThe Student %d Details : \n" , i+1);
			DPRINTF("Name : %s\n" , temp->name);
			DPRINTF("The roll : %d\n" , temp->roll);
			DPRINTF("The GPA : %.2f\n" , temp->GPA);
			uint32 j ;
			for(j=0 ; temp->course_Id[j] != 0 ; j++)
			{
				DPRINTF("The Course %d Id : %d\n" ,j+1 , temp->course_Id[j]);
			}
			FIFO_Circler_move(fifo, &temp);
		}
	}

}
