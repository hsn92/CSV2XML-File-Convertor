
// C program to search and replace
// all occurrences of a word with
// other word.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int rowID = 1;

// Function to replace a string with another
// string
char *replaceWord(const char *s, const char *oldW,
                  const char *newW)
{
    char *result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);

    // Counting the number of times old word
    // occur in the string
    for (i = 0; s[i] != '\0'; i++)
    {
        if (strstr(&s[i], oldW) == &s[i])
        {
            cnt++;

            // Jumping to index after the old word.
            i += oldWlen - 1;
        }
    }

    // Making new string of enough length
    result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1);

    i = 0;
    while (*s)
    {
        // compare the substring with the result
        if (strstr(s, oldW) == s)
        {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }

    result[i] = '\0';
    return result;
}

int main(int argc, char *argv[])
{

    char filename[strlen(argv[1])];
    strcpy(filename, argv[1]);
    char xmlfilename[strlen(argv[2])];
    strcpy(xmlfilename, argv[2]);
    int separator = atoi(argv[4]);
    int opsysC = atoi(argv[6]);
    char help[2];
    if (argv[7] != NULL)
    {

        strcpy(help, argv[7]);
    }

    if (strcmp(help, "-h") == 0)
    {
        printf("The layout of the system is Program <inputfile> <outputfile> [-separator <P1>][-opsys <P2>][-h]\n");
        printf("separator = 0 for comma \",\"  separator = 1 for tab \" \" separator = 2 for semicolon \";\"  ");
        printf("opsys = 0 for Linux opsys = 1 for macOS opsys = 2 for Windows ");
    }

    printf("%s", help);

    FILE *iFile;
    char *line = NULL;
    size_t len = 0;
    size_t read;

    iFile = fopen(filename, "r");
    if (iFile == NULL)
        exit(EXIT_FAILURE); // Reading file

    FILE *oFile;
    oFile = fopen(xmlfilename, "w+"); //Creating an output file

    // ./tt contacts.csv output.xml -separator 0 -opsys 0 -h

    /////////////////////////////////////////////////////////////////////////////// 2xml

    char str[3];
    char opsys[3];

    if (separator == 0)
    {
        strcpy(str, ",");
    }

    else if (separator == 1)
    {
        strcpy(str, "    ");
    }

    else if (separator == 2)
    {
        strcpy(str, ";");
    }

    if (opsysC == 0)
    {
        strcpy(opsys, "\n");
    }
    else if (opsysC == 1)
    {
        strcpy(opsys, "\r");
    }
    else if (opsysC == 2)
    {
        strcpy(opsys, "\r\n");
    }

    char *result;
    int count = 0;
    char *token;
    char store[500];

    if (read = getline(&line, &len, iFile))
    {
        strcpy(store, line);
        result = replaceWord(line, opsys, "");
        token = strtok(result, str);
        free(result);
        fprintf(oFile, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        fprintf(oFile, "<contacts>\n");
        while (token != NULL)
        {
            count++;
            token = strtok(NULL, str);
        }

        char titles[count][30];
        token = strtok(store, str);

        count = 0;

        while (token != NULL)
        {
            result = replaceWord(token, opsys, "");
            result = replaceWord(result, " ", "_");
            strlwr(result);
            strcpy(titles[count], result);
            token = strtok(NULL, str);
            free(result);
            count++;
        }
        char old[] = ",,"; 
        char new[] = ",*,";

        while ((read = getline(&line, &len, iFile)) != -1)
        {
            result = replaceWord(line, opsys, "*");
            result = replaceWord(result, old, new);
            token = strtok(result, str);
            count = 0;

            fprintf(oFile, "  <row id=”%d”>\n", rowID);

            while (token != NULL)
            {
                if (strcmp(token, "*") == 0 || strcmp(token, "\n") == 0)
                {
                    fprintf(oFile, "        <%s/>\n", titles[count]); 
                }
                else
                {
                    fprintf(oFile, "        <%s>", titles[count]);
                    fprintf(oFile, "%s", token);
                    fprintf(oFile, "</%s>\n", titles[count]);
                }
                count++;
                token = strtok(NULL, str);
            }
            fprintf(oFile, "  </row>\n");
            rowID++;
            ///////////////////////////////////////////////////////////////
        }

        fprintf(oFile, "</contacts>");

        fclose(oFile); // Finshing operations
        fclose(iFile);
        if (line)
            free(line);
        exit(EXIT_SUCCESS);

        return 0;
    }
}