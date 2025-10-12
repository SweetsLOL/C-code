#include <stdio.h>
#include <string.h>

typedef struct student
{
	char name[32];
	int sid;
	int score;
} Student;

typedef struct department
{
	char name[32];
	int id;
	Student *students;
} Department;

typedef enum genre
{
	Pop,
	Jazz,
	Rock
} Genre;

int main()
{
	// Single instance, assign values using . operator
	Student student1;
	strcpy(student1.name, "John doe");
	student1.sid = 12345;
	student1.score = 5;

	printf("student1 Name : %s\n", student1.name);
	printf("student1 ID : %d\n", student1.sid);
	printf("student1 Score : %d\n", student1.score);

	// Single instance, assign values upon declaration
	Student student2 = {"Jane doe", 67890, 5};

	printf("\nstudent2 Name : %s\n", student2.name);
	printf("student2 ID : %d\n", student2.sid);
	printf("student2 Score : %d\n", student2.score);

	// Array
	Student student_arrays[10];
	strcpy(student_arrays[0].name, "John Doe");
	student_arrays[0].sid = 12345;
	student_arrays[0].score = 5;

    printf("\nstudent_arrays[0] Name : %s\n", student_arrays[0].name);
	printf("student_arrays[0] ID : %d\n", student_arrays[0].sid);
	printf("student_arrays[0] Score : %d\n", student_arrays[0].score);

	strcpy(student_arrays[1].name, "Jane Doe");
	student_arrays[1].sid = 67890;
	student_arrays[1].score = 5;

    printf("\nstudent_arrays[1] Name : %s\n", student_arrays[1].name);
	printf("student_arrays[1] ID : %d\n", student_arrays[1].sid);
	printf("student_arrays[1] Score : %d\n", student_arrays[1].score);

	// Department
	Department fecs;
	strcpy(fecs.name, "FECS");
	fecs.id = 123;
	fecs.students = student_arrays;

	fecs.students[1].score = 5;

	printf("\n\nName of second student in %s (ID: %i), is %s with score %i\n", fecs.name, fecs.id, fecs.students[1].name, fecs.students[1].score);

	// Enum
	Genre g = Pop;
	printf("\nEnum example: Genre number is %d\n", g);
}
