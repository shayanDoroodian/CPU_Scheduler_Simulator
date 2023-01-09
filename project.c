#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE *ptr;
    char ch;
    // Opening file in reading mode
    ptr = fopen("./text.txt", "r");

    if (NULL == ptr)
    {
        printf("file can't be opened \n");
    }

    printf("content of this file are \n");

    do
    {
        ch = fgetc(ptr);
        printf("%c", ch);

    } while (ch != EOF);
    fclose(ptr);
    return 0;
}