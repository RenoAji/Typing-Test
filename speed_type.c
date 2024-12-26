#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LENGTH 20
#define WORDS_COUNT 2009

// printf("\nWord Accuracy : %.3f\n", word_acc);
// printf("Character Accuracy : %.3f\n", char_acc);
// printf("Time : %.2f\n", miliseconds / 1000.0);
// printf("WPM : %.2f\n", wpm);

typedef struct
{
    float word_acc;
    float char_acc;
    unsigned long int miliseconds;
    float wpm;
    int words;
} History;

void play(int n, History arr[], int *h_count);
void print_hist(History arr[], int n);

int main()
{
    // Determine how many words will be generated from user input
    int n;
    int history_count = 0;
    History histories[10];

    int option;
    do
    {
        printf("Select Menu : \n");
        printf("1. Play (Input 1)\n2. Print History (Input 2)\n3. End (Input any key other than 1 or 2)\n");
        printf("Select : ");
        scanf("%d", &option);

        if (option == 1)
        {
            printf("\nEnter amount of words : ");
            scanf("%d", &n);
            play(n, histories, &history_count);
        }
        else if (option == 2)
        {
            print_hist(histories, history_count);
        }
    } while (option == 1 || option == 2);
}

void play(int n, History arr[], int *h_count)
{
    // Allocate memory for words
    char **rand_words = malloc(n * sizeof(char *));

    // Load the words from txt file
    FILE *source = fopen("words.txt", "r");
    if (!source)
    {
        printf("Error opening file.\n");
        free(rand_words);
        return;
    }
    char buf[MAX_LENGTH];
    int i = 0;
    srand(time(NULL));
    while (fscanf(source, "%s", buf) != EOF)
    {
        strcat(buf, " ");
        if (i < n)
        {
            rand_words[i] = strdup(buf);
        }
        else
        {
            int index = rand() % i;
            if (index < n)
            {
                free(rand_words[index]);
                rand_words[index] = strdup(buf);
            }
        }
        i++;
    }

    // Print the words to the console
    for (int i = 0; i < n; i++)
    {
        if (i == 0)
        {
            printf("\033[s");
        }
        printf("%s", rand_words[i]);
    }
    printf("\033[u");

    char **input_words = malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++)
    {
        input_words[i] = malloc((strlen(rand_words[i]) + 1) * sizeof(char));
    }

    unsigned long int bef;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; rand_words[i][j] != '\0'; j++)
        {
            if (i == n - 1 && rand_words[i][j] == ' ')
            {
                input_words[i][j] = ' ';
                break;
            }

            char ch = getch();
            if (ch == 8)
            {
                if (j == 0 && i > 0)
                {
                    i -= 1;
                    j = strlen(rand_words[i]) - 2;
                    printf("\033[1D");
                }
                else if (j > 0)
                {
                    j -= 1;
                    printf("\033[1D");
                    printf("\033[0m%c", rand_words[i][j]);
                    printf("\033[1D");
                    j -= 1;
                }
                continue;
            }

            if (i == 0 && j == 0)
            {
                bef = clock() * 1000 / CLOCKS_PER_SEC;
            }

            if (ch == rand_words[i][j])
            {
                // true
                if (ch != ' ')
                {
                    printf("\033[32m");
                }
                printf("%c", rand_words[i][j]);
            }
            else
            {
                // false
                printf("\033[31m");
                printf("%c", rand_words[i][j]);
            }

            input_words[i][j] = ch;
        }
    }
    unsigned long int after = clock() * 1000 / CLOCKS_PER_SEC;

    printf("\n");
    // Compare rand word with input
    int words_correct = 0;
    int char_correct = 0;
    int char_count = 0;
    for (int i = 0; i < n; i++)
    {
        int is_correct = 1;
        for (int j = 0; rand_words[i][j] != '\0'; j++)
        {
            if (rand_words[i][j] == input_words[i][j])
            {
                char_correct++;
            }
            else
            {
                is_correct = 0;
            }
            char_count++;
        }
        if (is_correct == 1)
        {
            words_correct++;
        }
    }
    printf("\033[0m");

    // Count Accuracy and time
    float word_acc = (float)(words_correct) / n;
    float char_acc = (float)(char_correct) / char_count;
    unsigned long int miliseconds = after - bef;
    float wpm = (float)(words_correct) * 60 / (miliseconds / 1000);

    printf("\nWord Accuracy : %.3f\n", word_acc);
    printf("Character Accuracy : %.3f\n", char_acc);
    printf("Time in seconds : %.3f\n", miliseconds / 1000.0);
    printf("WPM : %.2f\n", wpm);

    // Add History
    int idx = *h_count % 10;
    arr[idx].char_acc = char_acc;
    arr[idx].miliseconds = miliseconds;
    arr[idx].word_acc = word_acc;
    arr[idx].words = n;
    arr[idx].wpm = wpm;

    (*h_count)++;

    // Free Memory
    for (int i = 0; i < n; i++)
    {
        free(input_words[i]);
        free(rand_words[i]);
    }
    free(rand_words);
    free(input_words);
}

void print_hist(History arr[], int n)
{
    if (n > 10)
    {
        n = 10;
    }

    printf("-----------------------------------------------------------------------------------------------------------\n");
    printf("Word Accuracy\tCharacter Accuracy\tTime in Seconds \tWord per Minutes\tTotal Words\n");
    printf("-----------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++)
    {
        printf("%.3f\t\t%.3f\t\t\t%.5f\t\t\t %.3f\t\t\t%d\n", arr[i].word_acc, arr[i].char_acc, arr[i].miliseconds / 1000.0, arr[i].wpm, arr[i].words);
    }
}