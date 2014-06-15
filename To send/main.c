/*
* C 4th task
* By: Norbert Zardin 1311008
*
* This program mines for data in 15min.lt main page source code.
* It finds articles title, link to the article and link to the image if it exists.
*
* Only 15min.lt source code can be used.
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <string.h>
#define n 1024

/// This program is only a prototype, a lot of things is going to be changed in full version.
/*  ==== To do: ====
* -   Static struct array >> Dynamic struct array
* -   .c and .h files
* -   Requirements: 3-6
*/


struct body
{
    char title[n];  //Article title
    char href[n];   //Article link
    char src[n];    //Article image link
    int count;
};
struct body data[n];

//=========== Functions ==========//
void Push(char str[n], int which);//
void Print(int count);            //
int Sep_check (char);             //
//================================//

int count = 0,
    which = 0;

int main (int argc, char* argv[])
{
    char *input = argv[1];
    FILE *input_file;
    input_file = fopen(input, "r");

    char c;
    char word[1024];
    char str[1024];


    int a = 0, a_title = 0,
        open = 0, next = 0,
        href = 0, src = 0,
        found_word = 0,
        i = 0, j = 0;

    if (input_file){
        a_title = 0;

        while ((c = fgetc(input_file)) != EOF ){
            if( (a_title == 1) || (href == 1) || (src == 1)){
                    if(c != 34){

                        str[j] = c;
                        j++;

                    } else {

                        open++;
                        if(open == 1){

                            str[j] = '\0';
                            which++;
                            if(strcmp(str, "Skaitykite daugiau") == 0) {
                                break;
                            }
                            Push(str, which);

                            if(which == 3) {
                                which = 0;
                            }

                            strcpy(str, "");
                            open = 0;
                            j = 0;
                            a_title = 0;
                            href = 0;
                            src = 0;

                        }

                    }

            } else {

                if (!(Sep_check(c))){

                    found_word = 1;
                    word[i] = c;
                    i++;

                } else {

                    if (found_word) {
                        word[i] = '\0';
                        found_word = 0;

                        // if title was found we go on with the link
                        if( (next == 2) && (strcmp(word, "src=") == 0) ){
                            src = 1;
                            next = 0;
                        }

                        // if title was found we go on with the link
                        if( (next == 1) && (strcmp(word, "href=") == 0) ){
                            href = 1;
                            next = 2;
                        }

                        // if starts with a and goes on with title=, it meants its <a title="..."
                        if(a == 1){
                            if (strcmp(word, "title=") == 0){

                                a_title = 1;
                                next = 1;
                                a = 0;

                            } else {
                                a = 0;
                            }
                        }

                        // if starts with <a ...
                        if(strcmp(word, "a") == 0){
                            a = 1;
                        }

                        strcpy(str, "");
                        i = 0;
                    }
                }
            }


        }
    } else {
        puts("Couldn't open the specified file");
    }
    fclose(input_file);
    Print(count);

    return 0;
}

int Sep_check (char c) {
	if ((c == 0) || \
        (c == 10) || \
		(c == 32) || \
		(c == 34) || \
		((c > 38 ) && (c < 45 )) || \
		((c > 57) && (c < 63) && (c != 61)) || \
		(c == 91) || \
		(c == 93) || \
		(c == 94) || \
		(c == 9) || \
		((c > 122) && (c <127))){
		return 1;
	} else {
		return 0;
	}
}
void Push(char str[n], int which){
    switch(which) {
        case 1 : {
            strcpy(data[count].title, str);
            break;
        }
        case 2 : {
            strcpy(data[count].href, str);
            break;
        }
        case 3 : {
            strcpy(data[count].src, str);
            which = 0;
            count++;
            break;
        }
    }
}
void Print(int count){
    for (int k = 0; k <= count; k++){
        printf("Title: %s\n\n", data[k].title);
        printf("Link: 15min.lt%s\n\n", data[k].href);
        printf("Img link: %s\n\n", data[k].src);
        printf("---------------------------------------------\n");
        getch();
    }
}
