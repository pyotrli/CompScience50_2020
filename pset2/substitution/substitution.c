// Substitution cipher. User provides key and plaintext. Programme outputs ciphertext
#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
//#include <math.h>

// Check if string is alpha chars only
bool alphastring(string s);
// Check if each char in string is unique (case insensitive)
bool uniquestring(string s);
// Encipher plaintext provided by user and print resulting ciphertext
int encipher(string key, string plaintext);
// Get key
int main(int argc, string argv[])
{
    // Validate how many command line arguments. If !=1, quit.
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    // Check key validity
    string key = argv[1];
    if (strlen(key) != 26 || !alphastring(key) || !uniquestring(key))
    {
        printf("Key invalid. Key must containt 26 unique alpha characters only\n");
        return 1;
    }
    // Get plaintext from user
    string plaintext = get_string("plaintext: ");
    // Encipher and print resulting ciphertext
    encipher(key, plaintext);
    printf("\n");
    return 0;
}

// Check if string is alpha characters only
bool alphastring(string s)
{
    for (int i = 0; i < strlen(s); i++)
    {
        if (isalpha(s[i]))
        {
            continue;
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

// Check if each char in string is unique (case insensitive)
bool uniquestring(string s)
{
    for (int i = 0; i < strlen(s); i++)
    {
        int occurences = 0;
        for (int j = 0; j < strlen(s); j++)
        {
            if (tolower(s[j]) == tolower(s[i]))
            {
                occurences++;
            }
        }
        if (occurences != 1)
        {
            return 0;
        }
    }
    return 1;
}

// Encipher and print
int encipher(string key, string plaintext)
{
    string a = "abcdefghijklmnopqrstuvwxyz";
    char ciphertext[strlen(plaintext)];
    for (int i = 0; i < strlen(plaintext); i++)
    {
        // Check for punctuation or spaces first
        if (ispunct(plaintext[i]) || isspace(plaintext[i]) || isdigit(plaintext[i]))
        {
            ciphertext[i] = plaintext[i];
        }
        // Loop through and return appropriate cipher letter. Case sensitive.
        for (int j = 0; j < strlen(a); j++)
        {
            if (tolower(plaintext[i]) == tolower(a[j]))
            {
                if (islower(plaintext[i]))
                {
                    ciphertext[i] = tolower(key[j]);
                }
                else
                {
                    ciphertext[i] = toupper(key[j]);
                }
            }
        }
    }
    printf("ciphertext: %s", ciphertext);
    return 1;
}

