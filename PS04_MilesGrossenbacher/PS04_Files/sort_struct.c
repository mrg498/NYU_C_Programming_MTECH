#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define MAX_STUDENTS	100
#define LINE_LEN	80
#define NAME_LEN	32
#define ID_LEN		6

typedef struct {
	char first[NAME_LEN];
	char last[NAME_LEN];
	int id;
} Student;

bool read_student(FILE *fp, Student *p)
{

	//your code here
	// separate the student line into its paramenters
	char line[80];
	char* token;
	int token_num = 0;
	char ch;


	if(fgets(line, LINE_LEN, fp) == NULL){
			return false;
	}


	token = strtok(line, ",\n");
	strcpy(p->first, token);
	token_num++;

	while ( (token = strtok(NULL, ",\n") ) != NULL) {
		 	if(token_num == 1){
				strcpy(p->last,token);
			}

			else{
				p->id = atoi(token);
			}
			token_num++;
	 }
	 //printf("%s,%s,%d",p->first,p->last,p->id);

	 return true;

}

int comp_first(const void * a, const void * b)
{
	//your code here
	Student* pa = (Student*)a;
	Student* pb = (Student*)b;
	return(strcmp(pa->first,pb->first));
}

int comp_last(const void * a, const void * b)
{
	//your code here
	//your code here
	Student* pa = (Student*)a;
	Student* pb = (Student*)b;
	return(strcmp(pa->last,pb->last));
}

int comp_id(const void * a, const void * b)
{
	//your code here
	Student* pa = (Student*)a;
	Student* pb = (Student*)b;
	return((pa->id)-(pb->id));
}

//usage
int usage(char * arg0){
	printf("Usage: %s first|last|id infile.txt outfile.txt\n", arg0);
	return 0;
}

int main(int argc, char *argv[])
{
	int i, num_students;
	char *sort_key;
	FILE *fp1, *fp2;
	Student student[MAX_STUDENTS];

	/* parse command line */
			//if incorrect number of arguments, call usage and return -1
	if(argc != 4){
		usage(argv[0]);
		return -1;
	}
			//parse command line
			// sort_key is either first,last,or id
	sort_key = argv[1];
	char* infile = argv[2];
	char* outfile = argv[3];

	printf("%s\n%s\n%s\n", sort_key, infile, outfile);

			// open input and output files
			/* open input file */
    if ( (fp1 = fopen(infile, "r")) == NULL ) {
        fprintf(stderr, "ERROR: Cannot open %s\n", infile);
        return -1;
    }
		/* open output file */
    if ( (fp2 = fopen(outfile, "w")) == NULL ) {
        fprintf(stderr, "ERROR: Cannot open %s\n", outfile);
        return -1;
    }

		printf("Opened the files!\n");



	/* read input into student array */
	i=0;
	while ( i<MAX_STUDENTS && read_student(fp1, &student[i]) ) {
		printf("%4d %s %s %6d\n", i, student[i].first, student[i].last, student[i].id);
		i++;
	}
	num_students = i;
	printf("Read records for %d students\n", num_students);
	printf("%s\n", sort_key);
	/* use quicksort to sort student array */
	if(sort_key[0] == 'f'){
		qsort(&student[0], num_students, sizeof(student[0]), comp_first);
	}
	else if(sort_key[0] == 'l'){
		qsort(&student[0], num_students, sizeof(student[0]), comp_last);
	}
	else{
		qsort(&student[0], num_students, sizeof(student[0]), comp_id);
	}
	//write sorted students to an output file
	for(int i = 0; i < num_students; i++){
		fprintf(fp2,"%4d %s,%s,%d\n", i, student[i].first, student[i].last, student[i].id);
	}

	fclose(fp1);
	fclose(fp2);

	return 0;
}
