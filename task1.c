#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdarg.h>
#include <getopt.h>
#include <stdbool.h>

int get_fd_to_write(char *file_name)
{
    int fd_to_write = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd_to_write == -1)
    {
        fprintf(stderr, "error: failed to open file for writting.\n");
        exit(EXIT_FAILURE);
    }
    return fd_to_write;
}
void complete_program(int fd_to_read, int fd_to_write){
    if (close(fd_to_read) == -1 || close(fd_to_write) == -1) 
    {
        fprintf(stderr, "error: failed to close file.\n");
        exit(EXIT_FAILURE);
    };
}

int main(int argc, char **argv)
{
    int fd_to_read;
    int fd_to_write;
    int block_size = 4096;
    char *restrict buffer;
    int opt;

    while ((opt = getopt(argc, argv, "b:")) != -1)
    {
        switch (opt)
        {
        case 'b':
            block_size = atoi(optarg);
            if (block_size < 1)
            {
                fprintf(stderr, "error: block size must be a positive number.\n");
                exit(EXIT_FAILURE);
            };
            break;
        default:
            fprintf(stderr, "error: wrong number of arguments.\nUsage: file_name [file_name] [-b block_size]\n");
            exit(EXIT_FAILURE);
        }
    }
    if (optind + 1 == argc)
    {
        fd_to_read = 0;
        fd_to_write = get_fd_to_write(argv[optind]);
    }
    else if (optind + 2 == argc)
    {
        if (!strcmp(argv[optind], argv[optind + 1]))
        {
            fprintf(stderr, "error: files must not match.\n");
            exit(EXIT_FAILURE);
        }
        fd_to_write = get_fd_to_write(argv[optind + 1]);
        fd_to_read = open(argv[optind], O_RDONLY);
        if (fd_to_read  == -1)
        {
            fprintf(stderr, "error: failed to open file for reading.\n");
            if (close(fd_to_write) == -1) 
            {
                fprintf(stderr, "error: failed to close file.\n");
            }
            exit(EXIT_FAILURE);
        }
    }
    buffer = calloc(block_size, sizeof(char));
    if (buffer == NULL)
    {
        fprintf(stderr, "error: memory allocation problem.\n");
        complete_program(fd_to_read, fd_to_write);
        exit(EXIT_FAILURE);
    }

    while (true)
    {
        bool is_zero_block = true;
        int read_block = 0;
        read_block = read(fd_to_read, buffer, block_size);
        if (read_block == -1)
        {
            fprintf(stderr, "error: file reading problem.\n");
            complete_program(fd_to_read, fd_to_write);
            free(buffer);
            exit(EXIT_FAILURE);
        }
        else if (!read_block)
        {
            break;
        }

        for (int i = 0; i < read_block; i++)
        {
            if (buffer[i])
            {
                is_zero_block = false;
                break;
            }
        }

        if (is_zero_block)
        {
            if (lseek(fd_to_write, read_block, SEEK_CUR) == -1)
            {
                fprintf(stderr, "error: failed to seek.\n");
                complete_program(fd_to_read, fd_to_write);
                free(buffer);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            int written_block = write(fd_to_write, buffer, read_block);
            if (written_block != read_block)
            {
                fprintf(stderr, "error: failed to write bytes.\n");
                complete_program(fd_to_read, fd_to_write);
                free(buffer);
                exit(EXIT_FAILURE);
            }
        }
    }
    free(buffer);
    complete_program(fd_to_read, fd_to_write);
}
