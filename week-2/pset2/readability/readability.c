#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

string getText(void);
int getLetters(string text);
int getWords(string text);
int getCentences(string text);
void getGrade(int letters, int words, int centences);

int main(void)
{
    string text = getText();
    int letters = getLetters(text);
    int words = getWords(text);
    int centences = getCentences(text);
    getGrade(letters, words, centences);
}

string getText(void)
{
    string text = get_string("Text: ");
    return text;
}

int getLetters(string text)
{
    int counter = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (isalpha(text[i]))
        {
            counter++;
        }
    }

    return counter;
}

int getWords(string text)
{
    int counter = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == ' ')
        {
            counter++;
        }
    }

    return counter + 1;
}

int getCentences(string text)
{
    int counter = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            counter++;
        }
    }

    return counter;
}

void getGrade(int letters, int words, int centences)
{
    double index = 0.0588 * ((double) letters / words * 100) - 0.296 * ((double) centences / words * 100) - 15.8;

    if (index > 15)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
        printf("Grade %i\n", (int) round(index));
}