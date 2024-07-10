/*
 *  Program.h
 *  Created on: Sep 25, 2023
 *  Author: Abdullah karkour
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "Platform_Types.h"
#include "Program.h"
#include <stdio.h>

#define DPRINTF(...)      {fflush(stdout);\
						   fflush(stdin); \
						   printf(__VA_ARGS__);\
						   fflush(stdout);\
						   fflush(stdin);}
// configuration of user

typedef struct {
	uint8 name[50];
	uint32 roll ;
	float GPA ;
	uint32 course_Id[10] ;
}Sinfo_t;

// Select data type (uint8 , uint16,unit32,uint64)
#define element_type Sinfo_t
// create the buffer from the same data type of element_type
// size of data Buffer
#define FIFO_length  50


// FIFO data type definition
typedef struct {
	uint32 length ;
	uint32 count ;
	element_type* base;
	element_type* head;
	element_type* tail;
}FIFO_Buf_t;

// FIFO Buffer status
typedef enum{
	FIFO_no_error,
	FIFO_error,
	FIFO_full,
	FIFO_empty,
	FIFO_null
}FIFO_Buffer_status;
/*******************************   				 APIS  					  **********************************/
FIFO_Buffer_status FIFO_init(FIFO_Buf_t* fifo , element_type* buf , uint32 length); // initialization of FIFO Buffer
FIFO_Buffer_status FIFO_is_full (FIFO_Buf_t*  fifo );
void FIFO_Circler_move( FIFO_Buf_t*  fifo , element_type** pToMove); // make circler FIFO check and move on head and tail
element_type* Get_Roll_loction(FIFO_Buf_t* fifo ,uint32 roll );// get a pointer to the student with that roll
void Operation ();
void Add_student_file(FIFO_Buf_t* fifo);
void Add_student_manually(FIFO_Buf_t* fifo);
void Find_roll(FIFO_Buf_t* fifo);
void Find_Name(FIFO_Buf_t* fifo);
void Find_course_student(FIFO_Buf_t* fifo);
void Student_Tot_Number(FIFO_Buf_t* fifo);
void Delete_Student(FIFO_Buf_t* fifo);
void Update_Student(FIFO_Buf_t* fifo);
void show_All_Student(FIFO_Buf_t* fifo);

#endif /* PROGRAM_H_ */
