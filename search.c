/*
* Copyright (C) 2015
*
*
* Written by Raphael S. Carvalho <raphael.scarv@gmail.com>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <assert.h>
#include "vector.h"

void search(const char *path, const char *name, vector *v, int max_depth) {
    if (max_depth == 0) {
        return;
    }

    DIR *dir = opendir(path);
    assert(dir);
    struct dirent *dir_entry;
    char *next_path;

    while ((dir_entry = readdir(dir)) != NULL) {
        // skip "." and ".."
        if (!strcmp(dir_entry->d_name, ".") || !strcmp(dir_entry->d_name, "..")) {
            continue;
        }

        if (dir_entry->d_type == DT_DIR) {
            next_path = malloc(strlen(path) + 1 + strlen(dir_entry->d_name) + 1);
            assert(next_path);
            sprintf(next_path, "%s/%s", path, dir_entry->d_name);
            search(next_path, name, v, max_depth - 1);
            free(next_path);
        }
        // find occurrence of substring name in string d_name.
        if (name && !strstr(dir_entry->d_name, name)) {
            continue;
        }

        (void) vector_insert(v, dir_entry);
    }
}

int main(int argc, char **argv) {
    if (argc != 2 && argc != 3) {
        fprintf(stderr, "%s <path> <string>\n", argv[0]);
        return -1;
    }
    
    vector v = vector_create(1.75f, sizeof(struct dirent));
    search(argv[1], (argc == 2) ? NULL : argv[2], &v, 3);

    int i;
    for (i = i; i < vector_length(&v); i++) {
        struct dirent *entry = vector_access(&v, i);
        printf("%s\n", entry->d_name);
    }
    
    printf("vector footprint: %d\n", vector_footprint(&v));

    vector_destroy(&v);

    return 0;
}