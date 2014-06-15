/*
* C 4th task
* By: Norbert Zardin 1311008
*
* This program mines for data in 15min.lt main page source code.
* It finds articles title, link to the article and link to the image if it exists.
*
* Only 15min.lt source code can be used.
*
* Tested on Windows 8.1 Pro, 64 bit.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <string.h>
#include <locale.h>
#define n 1024

/// This program is only a prototype, a lot of things is going to be changed in full version.
/*  ==== To do: ====
* -   Static struct array >> Dynamic struct array
* -   .c and .h files
* -   Requirements: 3-6
*/

struct body{
    char *title;  //Article title
    char *href;   //Article link
    char *src;    //Article image link
    int id;       //Total items
};
struct body data[n];

//============ Functions ==================================
void Push(char *STRING, int which, int size, int counter);
void Print(int counter);
int Sep_check (char);
void Write_Bin(int count);
void PrintBin(int);
//=========================================================

int count = 0,
    which = 0;

int main (int argc, char* argv[])
{
    //setlocale(LC_ALL, "Lithuanian");

    char *input = argv[1];
    FILE *input_file;
    input_file = fopen(input, "r");

    char c;
    char word[n];
    char str[n];
    char*p_string; // to push string

    int a = 0, a_title = 0,
        open = 0, next = 0,
        href = 0, src = 0,
        found_word = 0,
        i = 0, j = 0;
    if (input_file){

        a_title = 0;

        while ((c = fgetc(input_file)) != EOF ){
            if(feof(input_file)) break;
            if( (a_title == 1) || (href == 1) || (src == 1)){   // if true => save the following string, if false => search for <a title="..." href=".."
                    if(c != 34){                                // if char != " then add to string
                        str[j] = c;
                        j++;

                    } else {                                    // if char == " then end string creation
                        open++;
                        if(open == 1){
                            str[j] = '\0';
                            which++;
                            p_string = (char*) malloc(sizeof(char) * (j-1) );
                            p_string = (char*) str;

                            // when we reach the part of source-code where we don't collect data - we break.
                            if(strcmp(p_string, "Skaitykite daugiau") == 0) {
                                break;
                            } else {
                                //Push the string into the structure
                                Push(p_string, which, j, count);
                            }

                            if(which == 3) {
                                which = 0;  //Reset which
                                count++;
                            }

                            // Reset all
                            open = 0;
                            j = 0;
                            a_title = 0;
                            href = 0;
                            src = 0;
                            free(p_string);
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
                        i = 0;

                    }
                }
            }
        }
    } else {
        puts("Couldn't open the specified file");
    }
    fclose(input_file);
    //Print(count);
    Write_Bin(count);
    system("pause");
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
void Push(char *STRING, int which, int size, int counter){
    switch(which) {
        case 1 : {
            //Multiply the size by two because every lithuanian char needs 2 bytes,
            //so as we don't know how many lithuanian letters there's going to be we go by a safe 2x,
            //so if a whole string consists only of those chars.
            data[counter].title = (char*)malloc(sizeof(char)*size*2);
            strcpy(data[counter].title, STRING);
            break;
        }
        case 2 : {
            data[counter].href = (char*)malloc(sizeof(char)*size+1);
            strcpy(data[counter].href, STRING);
            break;
        }
        case 3 : {
            data[counter].src = (char*)malloc(sizeof(char)*size+1);
            strcpy(data[counter].src, STRING);
            data[counter].id = counter + 1;
            break;
        }
    }
}
void Print(int counter){
    for (int k = 0; k < counter; k++){
        printf("[#%d]\n\n", data[k].id);                //Print id
        printf("Title: %s\n\n", data[k].title);         //Print title
        printf("Link: 15min.lt%s\n\n", data[k].href);   //Print url
        printf("Img link  : %s\n\n", data[k].src);      //Print img url
        printf("----------------\n");
    }
}

void Write_Bin(int count){
    FILE *output = fopen("last.bin", "ab");
    if(output){
        for (int k = 0; k < count; k++){
            int sizeOfString;
            char *temp;
                sizeOfString = strlen(data[k].title);
                // Title to binary
                temp = data[k].title;
                sizeOfString = strlen(temp);

                fwrite(&sizeOfString, sizeof(int), 1, output);
                fwrite(temp, sizeof(char), sizeOfString, output);
                fwrite("\0", sizeof(char), 1, output);
                fwrite("\n", sizeof(char), 1, output);

                // URL to binary
                temp = data[k].href;
                sizeOfString = strlen(temp);
                fwrite(&sizeOfString, sizeof(int), 1, output);
                fwrite(temp, sizeof(char), sizeOfString, output);
                fwrite("\0", sizeof(char), 1, output);
                fwrite("\n", sizeof(char), 1, output);

                // Img URL to binary
                temp = data[k].src;
                sizeOfString = strlen(temp);

                fwrite(&sizeOfString, sizeof(int), 1, output);
                fwrite(temp, sizeof(char), sizeOfString, output);
                fwrite("\0", sizeof(char), 1, output);
                fwrite("\n\n", sizeof(char)*2, 1, output);
        }

   } else {
    puts("Couldn't open/create the specified file");
   }
}
void Read_File(){
}

void Read_Bin(){
}






























                /*//Print string size to binary file
                // this used to be in Write_Bin()
                int length = sizeOfString;
                char max[] = "0000000000000000";

                for (int i = 0; i < 16; i++, length /= 2){
                    if (length%2) (max[15-i] = '1');
                }
                fwrite(max, sizeof(char), 16, output);
                fwrite("\n", sizeof(char), 1, output);

                for (int j = 0; j <= sizeOfString; j++) {
                    int ch = data[k].title[j];
                    char bin[] = "00000000";

                    for (int i = 0; i < 8; i++, ch /= 2){
                        if (ch%2) (bin[7-i] = '1');

                    }
                    fwrite(bin, sizeof(char), 8, output);
                    fwrite("\n", sizeof(char), 1, output);

                }
                fwrite("\n", sizeof(char), 1, output);

                // At first I thought that it is required to do so* until Ernestas Mitkus told me it is not
                // so* - to save data as a binary number (00101011 and such).
                */

