// Readability analyses string input and evaluates how hard it is to read
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>

// Identify character type functions
bool letter(char c);
bool space(char c);
bool sentence(char c);

int main(void)
{
    // Get input from user
    string s = get_string("Text: ");
    int string_length = strlen(s);
    int letters = 0, spaces = 0, sentences = 0;
    // Go through char by char. If letter, end of sentence or new word, add to appropriate counter.
    for (int i = 0; i < string_length; i++)
    {
        if (letter(s[i]))
        {
            letters++;
        }
        if (space(s[i]))
        {
            spaces++;
        }
        if (sentence(s[i]))
        {
            sentences++;
        }
    }
//    printf("there are %i letters\n", letters);
//    printf("there are %i spaces\n", spaces);
//    printf("there are %i sentences\n", sentences);

    //this float declaration important - upscales words and hence the 2 below calculation to float for better accuracy. 
    float words = spaces + 1;
    // L is the average number of letters per 100 words in the text
    float L = letters * 100 / words;
    // S is the average number of sentences per 100 words in the text.
    float S = sentences * 100 / words;
    // Coleman-Liau index = 0.0588 * L - 0.296 * S - 15.8
    float clindex = 0.0588 * L - 0.296 * S - 15.8;

    if (clindex > 15)
    {
        printf("Grade 16+\n");
    }
    else if (clindex < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        int grade = round(clindex);
        printf("Grade %i\n", grade);
    }
}


bool letter(char c)
{
    if (c > 64 && c < 91)
    {
        return 1;
    }
    if (c > 96 && c < 123)
    {
        return 1;
    }
    return 0;
}

bool space(char c)
{
    if (c == ' ')
    {
        return 1;
    }
    return 0;
}

bool sentence(char c)
{
    if (c == '.' || c == '?' || c == '!')
    {
        return 1;
    }
    return 0;
}

