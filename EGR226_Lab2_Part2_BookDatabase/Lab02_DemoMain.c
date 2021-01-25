/*------------------------------------------------------------------
//	Author:			Trevor Ekin
//	Course:			EGR 226
//	File: 			Lab 02 Demo - Book Searching
//	Description:	This program reads in a list of books, stores
//						those books into an array of book structures,
//						then allows for user searching through those
//						books for title, author or ISBN.
//----------------------------------------------------------------*/

// -------------------------------------- Library Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// -------------------------------------- Library Includes

// -------------------------------------- Macro Definition
#define FILENAME "BookList.csv"
#define NUM_BOOKS 360
#ifndef NULL
#define NULL 0
#endif
// -------------------------------------- Macro Definition

// -------------------------------------- Structure Definition
typedef struct {
	char title[255];
	char author_name[50];
	char ISBN[10];
	int pages;
	int year_published;
}book;
// -------------------------------------- Structure Definition

// -------------------------------------- Function Prototypes
int get_input(int, int);
int parse_file(char[], book[]);
void print_book(book);
void search_title(book[], int, char[]);
void search_author(book[], int, char[]);
void search_ISBN(book[], int, char[]);
// -------------------------------------- Function Prototypes

int main() {
	book book_array[NUM_BOOKS];						// blank book array instantiation
	if (!parse_file(FILENAME, book_array)) {		// populate the book array or...
		printf("Failed to open %s\n",FILENAME);		// close out the program if the file could not be opened
		fflush(stdout);
		return -1;
	}

	while(1) {
		//	Determine which search the user would like
		printf("\nWhat would you like to search by? [0] Title, [1] Author, or [2] ISBN\n");
		fflush(stdout);
		int input = get_input(0, 2);

		// Determine what search criteria to sift the book list for
		printf("\nEnter your search criteria:\n");
		fflush(stdout);
		char search_crit[255];
		fgets(search_crit,255,stdin);
		search_crit[strlen(search_crit)-1] = 0;

		// Complete the requested search
		switch(input) {
			case 0: {search_title(book_array,NUM_BOOKS,search_crit); break;}
			case 1: {search_author(book_array,NUM_BOOKS,search_crit); break;}
			case 2: {search_ISBN(book_array,NUM_BOOKS,search_crit); break;}
		}

	}
	return 0;

}

/*-----------------------------------------------------------
//	Function: 		get_input
//	Description:	Collect integer user input between bounds
//	Inputs:
//			(int) minval:	minimum boundary to accept
//			(int) maxval:	maximum boundary to accept
//	Outputs:
//			(int) validated integer input
//---------------------------------------------------------*/
int get_input(int minval, int maxval) {
    int status, ret = -1, buf;
    do {
        status = fscanf(stdin,"%d", &ret);						// collect input
        if (!status || ret < minval || ret > maxval){			// validate input
            printf("\t<Please enter a value between %d and %d>",minval, maxval);
            fflush(stdout);
            ret = -1; 											// clear ret
            status = 0; 										// clear stat
            while ((buf  = getchar()) != '\n' && buf != EOF);	// Flush input stream
        }
    }while (!status);
	while ((buf  = getchar()) != '\n' && buf != EOF);			// Flush input stream
    return ret;
}

/*-----------------------------------------------------------
//	Function: 		parse_file
//	Description:	Sift through a file of books to populate
//						an empty book array
//	Inputs:
//			(char*) filename:	file to open and parse
//			(book*) book_array:	array of book structures to
//									populate
//	Outputs:
//			(int) parse status, 1 if successful, 0 otherwise.
//---------------------------------------------------------*/
int parse_file(char filename[], book book_array[]) {
	FILE* infile = fopen(filename, "r");				// Attempt to open file
	if (infile == NULL)									// Return 0 (failure) if file could not open
		return 0;

	char buffer[512];									// Create temporary string buffer variable
	int i = 0;											// Indexer for book array

	while (fgets(buffer, 512, infile)) {				// Loop collecting a line from the file until no more lines are there
		char * ptr = strtok(buffer,",");				// Parse string by commas and newline
		if(strcmp(ptr,"N/A"))							// Validate string strcmp returns 0 when strings match)
			strcpy(book_array[i].title,ptr);			// First parse is title

		ptr = strtok(NULL,",\n");
		if(strcmp(ptr,"N/A"))							// Validate string
			strcpy(book_array[i].author_name,ptr);		// Second parse is author

		ptr = strtok(NULL,",\n");
		if(strcmp(ptr,"N/A"))							// Validate string
			strcpy(book_array[i].ISBN,ptr);				// Third parse is ISBN

		ptr = strtok(NULL,",\n");
		if(strcmp(ptr,"N/A"))							// Validate string
			book_array[i].pages = atoi(ptr);			// Fourth parse is page count
		else
			book_array[i].pages = -1;					// Handle null value

		ptr = strtok(NULL,",\n");
		if(strcmp(ptr,"N/A"))							// Validate string
			book_array[i].year_published = atoi(ptr);	// Fifth parse is year published
		else
			book_array[i].year_published = -1;			// Handle null value

		i++;
	}

	return 1;
}

/*-----------------------------------------------------------
//	Function: 		print_book
//	Description:	Print the contents of a book structure
//	Inputs:
//			(book) pbook:	book to print
//	Outputs:
//			N/A
//---------------------------------------------------------*/
void print_book(book pbook) {
	printf("\n\t|Title:\t %s\n",strlen(pbook.title) ? pbook.title : "N/A");
	printf("\t|Author: %s\n",strlen(pbook.author_name) ? pbook.author_name : "N/A");
	printf("\t|ISBN:\t %s\n",strlen(pbook.ISBN) ? pbook.ISBN : "N/A");
	if (pbook.pages >= 0){printf("\t|Pages:\t %d\n", pbook.pages);}
	else {printf("\t|Pages:\t N/A\n");}
	if (pbook.year_published >= 0) {printf("\t|Year:\t %d\n\n",pbook.year_published);}
	else {printf("\t|Year:\t N/A\n");}
	fflush(stdout);
}

/*-----------------------------------------------------------
//	Function: 		search_title
//	Description:	Search through book array for books with
//						a title matching the search parameter
//	Inputs:
//			(book*) book_array:	books to search through
//			(int) num_books:	number of books in array
//			(char*) title:		title to search for
//	Outputs:
//			N/A
//---------------------------------------------------------*/
void search_title(book book_array[], int num_books, char title[]) {
	int i;
	printf("Searching books for title containing [%s]...\n",title);
	fflush(stdout);
	for (i=0; i<num_books; i++) {
		if (strstr(book_array[i].title,title))
			print_book(book_array[i]);
	}
}

/*-----------------------------------------------------------
//	Function: 		search_author
//	Description:	Search through book array for books with
//						an author name matching the search
//						parameter
//	Inputs:
//			(book*) book_array:	books to search through
//			(int) num_books:	number of books in array
//			(char*) author:		author to search for
//	Outputs:
//			N/A
//---------------------------------------------------------*/
void search_author(book book_array[], int num_books, char author[]) {
	int i;
	printf("Searching books for author names containing [%s]...\n",author);
	fflush(stdout);
	for (i=0; i<num_books; i++) {
		if (strstr(book_array[i].author_name,author))
			print_book(book_array[i]);
	}
}

/*-----------------------------------------------------------
//	Function: 		search_ISBN
//	Description:	Search through book array for books with
//						an ISBN matching the search parameter
//	Inputs:
//			(book*) book_array:	books to search through
//			(int) num_books:	number of books in array
//			(char*) ISBN:		ISBN to search for
//	Outputs:
//			N/A
//---------------------------------------------------------*/
void search_ISBN(book book_array[], int num_books, char ISBN[]) {
	int i;
	printf("Searching books for ISBN containing [%s]...\n",ISBN);
	fflush(stdout);
	for (i=0; i<num_books; i++) {
		if (strstr(book_array[i].ISBN,ISBN))
			print_book(book_array[i]);
	}
}
