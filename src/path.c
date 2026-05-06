#include "include/internal/path.h"

#include <stdlib.h>
#include <string.h>

#include "include/internal/dir.h"
#include "include/internal/inode.h"
#include "include/internal/state.h"
#include "include/public/error.h"

path_t *parse_path(const char *path_str);
void free_path(path_t *path);

int resolve_path(const char *path, dirent_t *found_dirent) {
    const ino_t root_ino = get_superblock_state()->root_ino;
    path_t *path_obj = parse_path(path);
    if (path_obj == NULL) return -INVALID_PATH;

    inode_t current_inode;
    read_inode(root_ino, &current_inode);

    dirent_t found_dirent_tmp = {
        .ino = root_ino,
        .name = "."
    };

    for (uint32_t i = 0; i < path_obj->token_count; i++) {
        const char *token = path_obj->tokens[i];
        HANDLE_THEN_PROPAGATE_ERROR(dir_lookup(
            &current_inode,
            &found_dirent_tmp,
            token
        ), { free_path(path_obj); });

        HANDLE_THEN_PROPAGATE_ERROR(read_inode(
            found_dirent_tmp.ino,
            &current_inode
        ), { free_path(path_obj); })
    }

    memcpy(found_dirent, &found_dirent_tmp, sizeof(dirent_t));
    free_path(path_obj);
    return NO_ERROR;
}

/**
 *
 * PRIVATE IMPLEMENTATION GOES BELOW
 */

path_t *parse_path(const char *path_str) {
    path_t *path = calloc(1, sizeof(path_t));
    path->token_capacity = 8;
    path->tokens = calloc(path->token_capacity, sizeof(char *));

    char path_str_cp[strlen(path_str + 1)];
    strcpy(path_str_cp, path_str);
    char *token = strtok(path_str_cp, "/");
    while (token != NULL) {
        if (path->token_count == path->token_capacity) {
            path->token_capacity += 4;
            char** tmp = realloc(path->tokens, path->token_capacity * sizeof(char *));
            if (tmp == NULL) return NULL;
            path->tokens = tmp;
        }

        path->tokens[path->token_count] = token;
        path->token_count++;
        token = strtok(NULL, "/");
    }

    return path;
}

void free_path(path_t *path) {
    free(path->tokens);
    free(path);
}




