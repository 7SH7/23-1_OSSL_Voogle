#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define input_size 100
#define line_size 300
#define bible_line 31102

// #define bible_line 1600
#define input_max_num 8

FILE *fp_niv;

char *read_a_line()
{

    static char buf[BUFSIZ];
    static int buf_n = 0;
    static int curr = 0;

    if (feof(fp_niv) && curr == buf_n - 1)
        return 0x0;

    char *s = 0x0;
    size_t s_len = 0;
    do
    {
        int end = curr;
        while (!(end >= buf_n || !iscntrl(buf[end])))
        {
            end++;
        }
        if (curr < end && s != 0x0)
        {
            curr = end;
            break;
        }
        curr = end;
        while (!(end >= buf_n || iscntrl(buf[end])))
        {
            end++;
        }
        if (curr < end)
        {
            if (s == 0x0)
            {
                s = strndup(buf + curr, end - curr);
                s_len = end - curr;
            }
            else
            {
                s = realloc(s, s_len + end - curr + 1);
                s = strncat(s, buf + curr, end - curr);
                s_len = s_len + end - curr;
            }
        }
        if (end < buf_n)
        {
            curr = end + 1;
            break;
        }

        buf_n = fread(buf, 1, sizeof(buf), fp_niv);
        curr = 0;
    } while (buf_n > 0);
    return s;
}

int main(int argc, char **argv)
{

    fp_niv = fopen("NIV.txt", "r");

    char *s = 0x0;
    char input_str[input_size];
    char fileline[line_size];
    char input_str_len[input_max_num];

    int token_num = 0;

    char **tokens = (char **)malloc(sizeof(char *) * input_size);

    fgets(input_str, sizeof(input_str), stdin);
    for (int i = 0; i < strlen(input_str); i++)
    {
        input_str[i] = tolower(input_str[i]);
    }
    char *token = strtok(input_str, " ");

    int tokens_input = 0;

    while (token != NULL)
    {
        tokens[tokens_input] = (char *)malloc(sizeof(char) * strlen(token));
        input_str_len[tokens_input] = strlen(token);
        strcpy(tokens[tokens_input], token);
        token = strtok(NULL, " ");

        tokens_input = tokens_input + 1; // tokens의 size 의미
    }
    if (tokens_input > 8)
    {
        return printf("you enter the too much number");
    }

    int *number_of_line = (int *)malloc(sizeof(int) * bible_line);
    char **NIV_arr = (char **)malloc(sizeof(char *) * bible_line);
    char **lower_NIV = (char **)malloc(sizeof(char *) * bible_line);

    for (int k = 0; k < bible_line; k++)
    {
        char *tmp_str = read_a_line();
        int tmp_str_len = strlen(tmp_str); // 글자 수..

        int tmp_str_cnt = 0;
        int double_tmp_str_cnt = 0;
        for (int i = 0; i < tmp_str_len - 1; i++)
        {
            if (tmp_str[i] == ' ')
                tmp_str_cnt++;

            if (tmp_str[i] == ' ')
                if (tmp_str[i + 1] == ' ')
                    double_tmp_str_cnt++;
        }

        number_of_line[k] = tmp_str_cnt - double_tmp_str_cnt;

        NIV_arr[k] = (char *)malloc(sizeof(char *) * (tmp_str_len));
        lower_NIV[k] = (char *)malloc(sizeof(char *) * (tmp_str_len));

        strcpy(NIV_arr[k], tmp_str);

        for (int i = 0; i < tmp_str_len; i++)
        {
            lower_NIV[k][i] = tolower(NIV_arr[k][i]);
        }
    }

    int ans_line[9][bible_line] = {
        {
            0,
        },
    };

    for (int i = 0; i < tokens_input; i++)
    {

        if (strchr(tokens[i], '*') != NULL)
        {

            char **copy_lower_NIV = (char **)malloc(sizeof(char *) * bible_line);
            for (int copy_i = 0; copy_i < bible_line; copy_i++)
            {
                char *copy_tmp_str = lower_NIV[copy_i];

                int copy_tmp_str_len = strlen(copy_tmp_str);

                copy_lower_NIV[copy_i] =
                    (char *)malloc(sizeof(char *) * (copy_tmp_str_len));

                strcpy(copy_lower_NIV[copy_i], copy_tmp_str);
            }

            char *nostar = strtok(tokens[i], "*");

            for (int j = 0; j < bible_line; j++)
            {
                strtok(copy_lower_NIV[j], " ");
                for (int k = 0; k <= number_of_line[j]; k++)
                {

                    if (strstr(copy_lower_NIV[j], nostar)) // != NULL)
                    {
                        ans_line[i][j] = 1;
                        break;
                    }
                    copy_lower_NIV[j] = strtok(NULL, " ");
                }
            }
            free(copy_lower_NIV);
        }

        // token
        else if ((strchr(tokens[i], '-')) == NULL &&
                 strstr(tokens[i], "book:") == NULL &&
                 strstr(tokens[i], "chapter:") == NULL)
        {

            char **copy_lower_NIV = (char **)malloc(sizeof(char *) * bible_line);
            for (int copy_i = 0; copy_i < bible_line; copy_i++)
            {
                char *copy_tmp_str = lower_NIV[copy_i];

                int copy_tmp_str_len = strlen(copy_tmp_str);

                copy_lower_NIV[copy_i] =
                    (char *)malloc(sizeof(char *) * (copy_tmp_str_len));

                strcpy(copy_lower_NIV[copy_i], copy_tmp_str);
            }

            char *ptr = tokens[i];
            int normal_len = strlen(tokens[i]);
            ptr[normal_len - 1] = '\0';

            for (int j = 0; j < bible_line; j++)
            {
                strtok(copy_lower_NIV[j], " ");
                for (int k = 0; k <= number_of_line[j] - 1; k++)
                {
                    if ((strcmp(copy_lower_NIV[j], ptr)) == 0)
                    {
                        ans_line[i][j] = 1;
                        break;
                    }

                    copy_lower_NIV[j] = strtok(NULL, " ");
                }
            }
            free(copy_lower_NIV);
        }
        // book:(Code)
        else if (strchr(tokens[i], ':') != NULL)
        {
            int _check = 0;
            char *find_int = strchr(tokens[i], ':') - 1;
            char key = 'k'; // book진입을 위해서
            if (find_int[0] == key)
            {
                _check = 1;
            }

            if (_check == 1)
            {
                char **copy_lower_NIV = (char **)malloc(sizeof(char *) * bible_line);
                for (int copy_i = 0; copy_i < bible_line; copy_i++)
                {

                    char *copy_tmp_str = lower_NIV[copy_i];

                    int copy_tmp_str_len = strlen(copy_tmp_str);

                    copy_lower_NIV[copy_i] =
                        (char *)malloc(sizeof(char *) * (copy_tmp_str_len));

                    strcpy(copy_lower_NIV[copy_i], copy_tmp_str);
                }

                char *book1 = strchr(tokens[i], ':') + 1; // book: 다음 첫 글자
                char *book2 = book1 + 1;                  // book: 다음 두번째 글자
                char *book3 = book2 + 1;                  // book: 다음 세번째 글자

                for (int j = 0; j < bible_line; j++)
                {
                    if (strchr(copy_lower_NIV[j], *book1) == copy_lower_NIV[j])
                    {
                        if (strchr(copy_lower_NIV[j], *book2) ==
                            strchr(copy_lower_NIV[j], *book1) + 1)
                        {
                            if (strchr(copy_lower_NIV[j], *book3) ==
                                strchr(copy_lower_NIV[j], *book2) + 1)
                            {
                                ans_line[i][j] = 1;
                            }
                        }
                    }
                    copy_lower_NIV[j] = strtok(NULL, " ");
                }
                free(copy_lower_NIV);
            }

            // chapter:(Num)
            else if (_check == 0)
            {
                char **copy_lower_NIV = (char **)malloc(sizeof(char *) * bible_line);
                for (int copy_i = 0; copy_i < bible_line; copy_i++)
                {
                    char *copy_tmp_str = lower_NIV[copy_i];

                    int copy_tmp_str_len = strlen(copy_tmp_str);

                    copy_lower_NIV[copy_i] =
                        (char *)malloc(sizeof(char *) * (copy_tmp_str_len));

                    strcpy(copy_lower_NIV[copy_i], copy_tmp_str);
                }

                char *ptr_number = strchr(tokens[i], ':') + 1;
                int num = atoi(ptr_number);
                char *str_number = (char *)malloc(sizeof(char) * input_size);
                char *real_number = (char *)malloc(sizeof(char) * input_size);
                char *space = " ";
                sprintf(str_number, "%d", num);
                strcat(real_number, space);
                strcat(real_number, str_number);
                strcat(real_number, ":");

                for (int j = 0; j < bible_line; j++)
                {
                    if (strstr(copy_lower_NIV[j], real_number))
                    {
                        ans_line[i][j] = 1;
                    }
                    copy_lower_NIV[j] = strtok(NULL, " ");
                }
                free(copy_lower_NIV);
                free(str_number);
                free(real_number);
            }
        }

        else if (strchr(tokens[i], '-') != NULL)
        {
            char **copy_lower_NIV = (char **)malloc(sizeof(char *) * bible_line);
            for (int copy_i = 0; copy_i < bible_line; copy_i++)
            {

                char *copy_tmp_str = lower_NIV[copy_i];

                int copy_tmp_str_len = strlen(copy_tmp_str);

                copy_lower_NIV[copy_i] =
                    (char *)malloc(sizeof(char *) * (copy_tmp_str_len));

                strcpy(copy_lower_NIV[copy_i], copy_tmp_str);
            }
            char *minus_ptr = tokens[i];
            char *minus = (char *)malloc(sizeof(char *) * sizeof(minus_ptr));
            strncpy(minus, minus_ptr + 1, strlen(tokens[i]) - 2);

            for (int j = 0; j < bible_line; j++)
            {

                strtok(copy_lower_NIV[j], " ");
                ans_line[i][j] = 0;
                for (int k = 0; k <= number_of_line[j]; k++)
                {

                    if (strstr(copy_lower_NIV[j], minus)) // != NULL)
                    {
                        ans_line[i][j] = 1;
                    }
                    copy_lower_NIV[j] = strtok(NULL, " ");
                }
            }
            for (int j = 0; j < bible_line; j++)
            {
                if (ans_line[i][j] == 1)
                    ans_line[i][j] = 2;
                if (ans_line[i][j] == 0)
                    ans_line[i][j] = 1;
                if (ans_line[i][j] == 2)
                    ans_line[i][j] = 0;
            }
            free(copy_lower_NIV);
            free(minus);
        }
    }

    for (int i = 0; i < tokens_input; i++)
    {
        for (int j = 0; j < bible_line; j++)
        {
            if (ans_line[i][j] == 1)
            {

                ans_line[tokens_input][j] = ans_line[tokens_input][j] + 1;
            }
        }
    }

    for (int i = 0; i < bible_line; i++)
    {
        if (ans_line[tokens_input][i] == tokens_input)
        {
            printf("%s\n", NIV_arr[i]);
        }
    }
    free(tokens);
    free(number_of_line);
    free(NIV_arr);
    free(lower_NIV);

    fclose(fp_niv);
}