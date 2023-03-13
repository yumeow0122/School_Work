void free_data(char ***data, size_t xlen, size_t ylen)
{
    size_t i, j;

    for (i = 0; i < xlen; ++i)
    {
        if (data[i] != NULL)
        {
            for (j = 0; j < ylen; ++j)
                free(data[i][j]);
            free(data[i]);
        }
    }
    free(data);
}

char ***alloc_data(size_t xlen, size_t ylen, size_t zlen)
{
    char ***p;
    size_t i, j;

    if ((p = malloc(xlen * sizeof *p)) == NULL)
    {
        perror("malloc 1");
        return NULL;
    }

    for (i = 0; i < xlen; ++i)
        p[i] = NULL;

    for (i = 0; i < xlen; ++i)
        if ((p[i] = malloc(ylen * sizeof *p[i])) == NULL)
        {
            perror("malloc 2");
            free_data(p, xlen, ylen);
            return NULL;
        }

    for (i = 0; i < xlen; ++i)
        for (j = 0; j < ylen; ++j)
            p[i][j] = NULL;

    for (i = 0; i < xlen; ++i)
        for (j = 0; j < ylen; ++j)
            if ((p[i][j] = malloc(zlen * sizeof *p[i][j])) == NULL)
            {
                perror("malloc 3");
                free_data(p, xlen, ylen);
                return NULL;
            }

    return p;
}

void print_command(int argc, char **args)
{
    for (int i = 0; i < argc; i++)
        printf("%s ", args[i]);
    printf("\n");
}

// void print_command_error(int argc, char ** args){
//     printf(" Unknown command: [ ");
//     for(int i=0; i<argc; i++)
//         printf("%s ", args[i]);
//     printf("]\n");
// }

void print_command_error(char **args)
{
    printf(" Unknown command: [ ");
    for (int i = 0; args[i] != NULL; i++)
        printf("%s ", args[i]);
    printf("]\n");
}