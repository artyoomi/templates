#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define TARGET_DIR "/proc"

typedef struct {
    char *fpath;
    off_t fname_offset;
    int fpath_len;
} file_t;

// function to save all file paths of directory
void listdir(char *dir_path, file_t ***files, int *files_count)
{
    DIR *dir = opendir(dir_path);
    if (dir) {
        struct dirent *de = readdir(dir);
        while (de) {
            // len of entry_dir path + len of file name + space to slash and \0 symbol
            int new_fpath_len = strlen(dir_path) + strlen(de->d_name) + 2;
            char *new_fpath = (char *)malloc(new_fpath_len * sizeof(char));
            snprintf(new_fpath, new_fpath_len, "%s/%s", dir_path, de->d_name);
            
            // check what if thats directory what this
            // not current or previous directory
            if (de->d_type == DT_DIR &&
                strcmp(".", de->d_name) && strcmp("..", de->d_name)) {
                listdir(new_fpath, files, files_count);
                free(new_fpath);
            } else if (de->d_type == DT_REG) {
                file_t **new_files = (file_t **)realloc(*files,
                                                        ((*files_count) + 1) * sizeof(file_t *));
                if (!new_files) {
                    fprintf(stderr, "Error while reallocating memory!\n");
                    exit(EXIT_FAILURE);
                }
                
                *files = new_files;

                (*files)[(*files_count)] = (file_t *)malloc(sizeof(file_t));
                (*files)[(*files_count)]->fpath = new_fpath;
                (*files)[(*files_count)]->fname_offset = strlen(dir_path) + 1;
                (*files)[(*files_count)]->fpath_len = new_fpath_len;

                (*files_count)++;      
            } else {
                free(new_fpath);
            }
            
            de = readdir(dir);
        }
    }
}

// compare files throw fpath strings
int files_cmp(const void *file1, const void *file2)
{
    char *fpath1 = (*((file_t **)file1))->fpath;
    char *fpath2 = (*((file_t **)file2))->fpath;

    return strcmp(fpath1, fpath2);
}

int main()
{
    // array of files
    file_t **files = NULL;
    int files_count = 0;
    
    listdir(TARGET_DIR, &files, &files_count);
    qsort(files, files_count, sizeof(file_t *), files_cmp);

    for (int i = 0; i < files_count; i++) {
        printf("File %d:\n\tfpath: %s\n\tfname_offset: %ld\n\tfpath_len: %d\n",
               i, files[i]->fpath, files[i]->fname_offset, files[i]->fpath_len);
    }
    
    return 0;
}
