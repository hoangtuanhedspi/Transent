#include <directory.h>

_Bool existFile (const char *path, const char *file) {
    DIR *d;
    struct dirent *dir;

    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (!strcmp(dir->d_name, file))
                return 1;
        }
        closedir(d);
    }

    return 0;
}

void printListFile (const char *path) {
    DIR *d;
    struct dirent *dir;

    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
}