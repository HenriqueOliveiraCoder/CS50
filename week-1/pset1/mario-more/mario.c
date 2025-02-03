#include <cs50.h>
#include <stdio.h>

void getHeight(void);
void printPyramids(void);
int height;
int main(void)
{
    getHeight();
    printPyramids();
}

void getHeight(void)
{
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);
}

void printPyramids(void)
{
    for (int i = 1; i < height + 1; i++)
    {
        for (int j = 0; j < height - i; j++)
        {
            printf(" ");
        }
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }
        printf("  ");
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}