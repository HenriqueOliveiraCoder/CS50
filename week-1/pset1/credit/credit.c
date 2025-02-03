#include <cs50.h>
#include <stdio.h>
#include <string.h>

void getValues(void); // declaring functions
bool getValidation(void);
string getOutput(bool isValid);

int firstTwoDigits; // declaring variables
string card;
int length;

int main(void)
{
    getValues();                      // Calling the function that get the values
    bool isValid = getValidation();   // Testing the Lunh's algorythm
    printf("%s", getOutput(isValid)); // Printing the type of the code
}

void getValues(void)
{
    // Getting the value of the card, length of the card and the first two digits of it.

    // getting the card number from the user
    card = get_string("Number: ");
    // getting the length of the card variable
    length = strlen(card);
    // getting the first two digits
    firstTwoDigits = (((int) card[0] - 48) * 10) + ((int) card[1] - 48);
}

bool getValidation(void)
{
    // Function to apply the lunh's algorythm to the card number

    // Initialyzing the variables used in the function
    int sum = 0;
    int digit;
    int i = length - 1;
    int doubled;
    int product;
    // Loop for iterating through all the digits of the credit card
    while (i >= 0)
    {
        // Changing the ASCII value of the individual digits into the int value
        digit = card[i] - 48;

        // Checking if it is even(if looked in reverse)
        // Because it should start from te second-to-last digit
        if ((length - i) % 2 == 0)
        {
            product = digit * 2;

            // cheking if the digits need to be separated and sumed toguether
            if (product > 9)
            {
                // The '/ 10' code gets the first digit and '% 10' gets the last
                sum += (product / 10) + (product % 10);
            }
            else
            {
                sum += product;
            }
        }

        else
        {
            sum += digit;
        }

        i--;
    }

    // Checking if the total modulo 10 is congruent to 0
    if (sum % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

string getOutput(bool isValid)
{
    // Function to get the correct output to each credit card

    if (!isValid)
    {
        return "INVALID\n";
    }
    else if (length == 15 && (firstTwoDigits == 34 || firstTwoDigits == 37))
    {
        return "AMEX\n";
    }
    else if (length == 16 &&
             (firstTwoDigits == 51 || firstTwoDigits == 52 || firstTwoDigits == 53 || firstTwoDigits == 54 || firstTwoDigits == 55))
    {
        return "MASTERCARD\n";
    }
    else if ((length == 13 || length == 16) && firstTwoDigits / 10 == 4)
    {
        return "VISA\n";
    }
    else
    {
        return "INVALID\n";
    }
}