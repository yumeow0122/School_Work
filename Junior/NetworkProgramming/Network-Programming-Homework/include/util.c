#include <dirent.h>
#include <stddef.h>

char existBin[100][100];
int existBinCount = 0;

void build_exist_command()
{
    // scan the bin directory for executables, use it if exist, or else use the system executables.
    DIR *d;
    struct dirent *dir;
    d = opendir("./bin");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_type == 8)
            {
                strcpy(existBin[existBinCount], dir->d_name);
                existBinCount++;
            }
        }
        closedir(d);
    }
}