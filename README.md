# Typing Test

### Video Demo: <URL HERE>

### Description:

My project is a typing test coded in C. This program will ask user if they want to play, see history, or exit the program. If the user want to play, program will ask user how many words will be generated for the typing test and run the main() function. If the user want to see history, program will run print_his() function.

#### Struct History

Fields :

```
float word_acc;
float char_acc;
unsigned long int miliseconds;
float wpm;
int words;
```

`float word_acc` to store the word accuracy, `float char_acc` store the character accuracy, `unsigned long int miliseconds` store the number of milliseconds user takes to type, `float wpm` to store words per minute, and `int words` to store how many words in that typing test.

#### main() Function

Variables :

```

int n;
int history_count = 0;
History histories[10];

```

If the user want to play, the program will ask the user how many words will be generated to them to type and store that input to `int n` variable.

`int history_count` is used to keep track of how many user do the typing test.

`History histories[10]` is used to store an array of History.

#### play() Function

This function start with allocating memory for the random words that the user will type.
This is the code look like with n is amount of words will be generated, based on user input in the main function before :

```

// Allocate memory for words
char _rand*words = malloc(n * sizeof(char _));

```

This will make an array of random words.

The words is loaded from `words.txt` file. So this program will open that file to load the words.

```

FILE \*source = fopen("words.txt", "r");
if (!source)
{
printf("Error opening file.\n");
free(rand_words);
return;
}

```

Then this program will load the words randomly using fscanf function

```

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
        if (index < n);
        {
            free(rand_words[index]);
            rand_words[index] = strdup(buf);
        }
    }
    i++;
}

```

`srand(time(NULL));` is used to set seed for the random generator. It ensure that `rand()` will generate different value each time the program run, because `time(NULL)` is different. `while (fscanf(source, "%s", buf) != EOF)` will loop through the words in the `words.txt` file until it reach the end of file (EOF). Then, each word will stored in buf variable. `strcat(buf, " ");` is used to add a space for each word. The if else statement is used to store the words randomly, if i < n (remember n is amount of words will be generated) `rand_words[i] = strdup(buf);` will assign rand_words[i] equal to buf. If i > n, the index is determined by `int index = rand() % i;`. Then, if index < n, `rand_words[index] = strdup(buf);` will assign rand_words[index] equal to buf. This way, rand_words will contain an array of a random words.

Then, that random words will be printed to the console.

```

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

```

`printf("\033[s");` is used to save the cursor position. In the context above, its saved the position of the first character in the first word. `printf("%s", rand_words[i]);` will print the word in current iteration. And `printf("\033[u");` will set the current cursor position to the saved position before. So after the words printed to the console, the cursor position will be in the first character, first word.

Then, the program will allocate momory for `input_words` to store the words user typed :

```

char \*_input_words = malloc(n _ sizeof(char _));
for (int i = 0; i < n; i++)
{
input_words[i] = malloc((strlen(rand_words[i]) + 1) _ sizeof(char));
}

```

Then the program will let the user type :

```

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

```

The words that the user typed will be stored in `input_words`. When the user type the right character, program will print that character from `rand_words` to the console with green color using `printf("\033[32m");`. But when the user type the wrong character, program will print the correct character from `rand_words` to the console with red color using `printf("\033[31m");`. Overriding the character that already printed in the cursor position.

`bef` and `after` variables are used to calculate the user's time to type. When the user start typing the first character in the first-word `bef = clock() * 1000 / CLOCKS_PER_SEC;` it will store how much time has elapsed since the program started in milliseconds. After user finishes typing, `unsigned long int after = clock() * 1000 / CLOCKS_PER_SEC;` will store how much time has elapsed since the program started in milliseconds after the user finished typing. So, when we need the number of milliseconds the user takes to type we calculate `after - bef`.

Then the program compared `rand_words` and `input_words` to calculate how many words and character is typed correctly, and also calculate the total character.

```
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
```

`printf("\033[0m");` is used to reset the printed color, so the next text printed not green or red.

Then the program will calculate word accuracy, character accuracy, time the user takes to type, and words per minute. And also print it to the console.

```
// Count Accuracy and time
float word_acc = (float)(words_correct) / n;
float char_acc = (float)(char_correct) / char_count;
unsigned long int miliseconds = after - bef;
float wpm = (float)(words_correct) \* 60 / (miliseconds / 1000);

printf("\nWord Accuracy : %.3f\n", word_acc);
printf("Character Accuracy : %.3f\n", char_acc);
printf("Time in seconds : %.3f\n", miliseconds / 1000.0);
printf("WPM : %.2f\n", wpm);
```

Also store that record to the history

```
// Add History
int idx = *h_count % 10;
arr[idx].char_acc = char_acc;
arr[idx].miliseconds = miliseconds;
arr[idx].word_acc = word_acc;
arr[idx].words = n;
arr[idx].wpm = wpm;
```

Because the max histories is 10 so the index of the record is `*h_count % 10` to ensure that the index is not out of bound. For example id `*h_count` is 12, 12 % 10 = 2, so the index is 2 and it will overide the `arr[2]` value.

`arr` is pointing to `History histories[10]` in the `main()` function. and `h_count` pointing to `int history_count`.

#### print_hist() Function

```
if (n > 10)
{
    n = 10;
}

void print_hist(History arr[], int n)
{
    printf("-----------------------------------------------------------------------------------------------------------\n");
    printf("Word Accuracy\tCharacter Accuracy\tTime in Seconds \tWord per Minutes\tTotal Words\n");
    printf("-----------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++)
    {
        printf("%.3f\t\t%.3f\t\t\t%.5f\t\t\t %.3f\t\t\t%d\n", arr[i].word_acc, arr[i].char_acc, arr[i].miliseconds / 1000.0, arr[i].wpm, arr[i].words);
    }
}
```

This printed all the histories that stored in `History arr[]` (reffer to `History histories[10]` in main function) by looping from 0 to `n` (Amount of histories).
