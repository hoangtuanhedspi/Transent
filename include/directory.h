#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

_Bool existFile (const char *path, const char *file);
void printListFile (const char *path);
#endif