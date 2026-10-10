/*
 * Just educational program that reads binary file, containing three matrices
 * and their sizes in weird format and performs convolution on them.
 */

// For strdup, just in case...
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>


//! Predefined error types
typedef enum {
    OK  = 0,
    IO  = 1,
    MEM = 2,
    CLI = 10,
} error_code_t;

//! CLI options
typedef struct {
    char* input;
    char* output;
} opts_t;

//! Defines count of matrices, that file contains
#define MATRICES_COUNT 3

//! Representation of input file
typedef struct {
    uint32_t H;
    uint32_t W;
    //! We need to load and save data in this weird format, so if we will load
    //! it as is and make computations on it, we can reduce complexity of
    //! converting into convenient format on read and back to weird on write.
    uint8_t* ABC;
    uint16_t DH;
    uint16_t DW;
    int8_t*  D;
} in_data_t;

//! Abstractions to get {i,j} element from each matrix transparently
#define M(data, i, j, offset)\
    ((data)->ABC\
    [\
    ((size_t)(i) * (data)->W + (size_t)(j)) * MATRICES_COUNT + (size_t)(offset)\
    ])
#define A(data, i, j) M(data, i, j, 0)
#define B(data, i, j) M(data, i, j, 1)
#define C(data, i, j) M(data, i, j, 2)

//! Representation of output file
typedef struct {
    uint32_t H;
    uint32_t W;
    uint8_t* ABC;
} out_data_t;

static void opts_init   (opts_t* opts);
static int  opts_fill   (opts_t* opts, int argc, char* argv[]);
static void opts_destroy(opts_t* opts);

static void in_data_init   (in_data_t* data);
static int  in_data_fill   (in_data_t* data, const char* path);
static void in_data_destroy(in_data_t* data);

static void out_data_init   (out_data_t* data);
static int  out_data_create (out_data_t* out_data, const in_data_t* in_data);
static void out_data_fill   (out_data_t* out_data, const in_data_t* in_data);
static int  out_data_dump   (const out_data_t* data, const char* path);
static void out_data_destroy(out_data_t* data);



int main(int argc, char* argv[])
{
    int ret = OK;

    opts_t     opts;
    in_data_t  in_data;
    out_data_t out_data;
    opts_init    (&opts);
    in_data_init (&in_data);
    out_data_init(&out_data);

    if ((ret = opts_fill(&opts, argc, argv)) != OK) {
        fprintf(stderr, "Error encountered at opts_fill\n");
        goto cleanup;
    }

    if ((ret = in_data_fill(&in_data, opts.input)) != OK) {
        fprintf(stderr, "Error encountered at in_data_fill\n");
        goto cleanup;
    }

    if ((ret = out_data_create(&out_data, &in_data)) != OK) {
        fprintf(stderr, "Error encountered at out_data_create\n");
        goto cleanup;
    }
    out_data_fill(&out_data, &in_data);
    if ((ret = out_data_dump(&out_data, opts.output)) != OK) {
        fprintf(stderr, "Error encountered at out_data_dump\n");
        goto cleanup;
    }

cleanup:
    out_data_destroy(&out_data);
    in_data_destroy (&in_data);
    opts_destroy    (&opts);

    return ret;
}

static void opts_init(opts_t* opts)
{
    opts->input  = NULL;
    opts->output = NULL;
}
static int opts_fill(opts_t* opts, int argc, char* argv[])
{
    if (opts == NULL) {
        return CLI;
    }

    opts_init(opts);

    const char* optstring = "i:o:";
    int opt;
    while ((opt = getopt(argc, argv, optstring)) != -1) {
        switch (opt) {
        case 'i': // input
            if (opts->input) {
                fprintf(stderr, "Duplicated option -%c with argument %s\n", opt,
                        optarg);
                return CLI;
            }
            opts->input = strdup(optarg);
            if (!opts->input) {
                fprintf(stderr, "failed to allocate memory for input path: %s\n",
                        optarg);
                return MEM;
            }
            break;
        case 'o': // output
            if (opts->output) {
                fprintf(stderr, "Duplicated option -%c with argument %s\n", opt,
                        optarg);
                return CLI;
            }
            opts->output = strdup(optarg);
            if (!opts->output) {
                fprintf(stderr, "failed to allocate memory for output path: %s\n",
                        optarg);
                return MEM;
            }
            break;
        case '?': // unknown
            return CLI;
        }
    }
    if (!opts->input || !opts->output) {
        return CLI;
    }
    return OK;
}

static void opts_destroy(opts_t* opts)
{
    free(opts->input);
    free(opts->output);

    opts_init(opts);
}

static void in_data_init(in_data_t* data)
{
    data->H   = 0;
    data->W   = 0;
    data->ABC = NULL;
    data->DH  = 0;
    data->DW  = 0;
    data->D   = NULL;
}

static int in_data_fill(in_data_t* data, const char* path)
{
    in_data_init(data);

    int ret = OK;

    uint8_t need_check_fptr = 0;
    uint8_t need_free_ABC   = 0;
    uint8_t need_free_D     = 0;

    FILE* fptr = fopen(path, "rb");
    if (!fptr) {
        fprintf(stderr, "%s: cannot open binary file\n", path);
        return IO;
    }

    if (fread(&data->H, sizeof(data->H), 1, fptr) != 1) {
        need_check_fptr = 1;
        ret = IO;
        goto cleanup;
    }
    if (fread(&data->W, sizeof(data->W), 1, fptr) != 1) {
        need_check_fptr = 1;
        ret = IO;
        goto cleanup;
    }

    if (data->H != 0 && data->W > SIZE_MAX / MATRICES_COUNT / data->H) {
        fprintf(stderr, "%s: matrix is too big\n", path);
        ret = MEM;
        goto cleanup;
    }
    const size_t ABC_size = (size_t)MATRICES_COUNT * data->H * data->W;
    data->ABC = (uint8_t*)malloc(ABC_size ? ABC_size : 1);
    if (!data->ABC) {
        fprintf(stderr, "Cannot allocate %zu bytes for ABC\n", ABC_size);
        ret = MEM;
        goto cleanup;
    }

    if (fread(data->ABC, 1, ABC_size, fptr) != ABC_size) {
        need_check_fptr = 1;
        need_free_ABC   = 1;
        ret = IO;
        goto cleanup;
    }


    if (fread(&data->DH, sizeof(data->DH), 1, fptr) != 1) {
        need_check_fptr = 1;
        need_free_ABC   = 1;
        ret = IO;
        goto cleanup;
    }
    if (fread(&data->DW, sizeof(data->DW), 1, fptr) != 1) {
        need_check_fptr = 1;
        need_free_ABC   = 1;
        ret = IO;
        goto cleanup;
    }

    const size_t D_size = (size_t)data->DH * data->DW;
    data->D = (int8_t*)malloc(D_size ? D_size : 1);
    if (!data->D) {
        fprintf(stderr, "Cannot allocate %zu bytes for D\n", D_size);
        need_free_ABC   = 1;
        ret = MEM;
        goto cleanup;
    }

    if (fread(data->D, 1, D_size, fptr) != D_size) {
        need_check_fptr = 1;
        need_free_ABC   = 1;
        need_free_D     = 1;
        ret = IO;
        goto cleanup;
    }
cleanup:
    if (need_check_fptr) {
        if (ferror(fptr)) {
            fprintf(stderr, "%s: read error\n", path);
        } else {
            fprintf(stderr, "%s: unexpected EOF\n", path);
        }
    }
    if (need_free_ABC) {
        free(data->ABC);
        data->ABC = NULL;
    }
    if (need_free_D) {
        free(data->D);
        data->D = NULL;
    }
    fclose(fptr);
    return ret;
}

static void in_data_destroy(in_data_t* data)
{
    free(data->ABC);
    free(data->D);

    in_data_init(data);
}

static void out_data_init(out_data_t* data)
{
    data->H   = 0;
    data->W   = 0;
    data->ABC = NULL;
}

static int out_data_create(out_data_t* out_data, const in_data_t* in_data)
{
    out_data_init(out_data);

    out_data->H = in_data->H;
    out_data->W = in_data->W;
    const size_t ABC_size = (size_t)MATRICES_COUNT * out_data->H * out_data->W;
    out_data->ABC = (uint8_t*)malloc(ABC_size ? ABC_size : 1);
    if (!out_data->ABC) {
        fprintf(stderr, "cannot alloc %zu bytes for resulting ABC\n", ABC_size);
        return MEM;
    }

    return OK;
}

static uint8_t normalize_helper(int64_t value)
{
    if (value > 255) return (uint8_t)(value % 251);
    if (value < 0)   return (uint8_t)(-value % 241);
    return (uint8_t)value;
}

static void out_data_fill(out_data_t* out_data, const in_data_t* in_data)
{
    uint32_t offset_H = in_data->DH / 2;
    uint32_t offset_W = in_data->DW / 2;

    if (in_data->H == 0 || in_data->W == 0) {
        return;
    }

    for (size_t i = 0; i < in_data->H; ++i) {
        for (size_t j = 0; j < in_data->W; ++j) {
            int64_t sum_A = 0;
            int64_t sum_B = 0;
            int64_t sum_C = 0;
            for (size_t p = 0; p < in_data->DH; ++p) {
                const int64_t y = (int64_t)i - offset_H + (int64_t)p;
                if (y < 0 || y >= in_data->H) {
                    continue;
                }

                for (size_t q = 0; q < in_data->DW; ++q) {
                    const int64_t x = (int64_t)j - offset_W + (int64_t)q;
                    if (x < 0 || x >= in_data->W) {
                        continue;
                    }

                    const int64_t k = in_data->D[p * in_data->DW + q];
                    sum_A += A(in_data, y, x) * k;
                    sum_B += B(in_data, y, x) * k;
                    sum_C += C(in_data, y, x) * k;
                }
            }

            A(out_data, i, j) = normalize_helper(sum_A);
            B(out_data, i, j) = normalize_helper(sum_B);
            C(out_data, i, j) = normalize_helper(sum_C);
        }
    }
}

static int out_data_dump(const out_data_t* data, const char* path)
{
    int ret = OK;

    uint8_t need_remove_file = 0;

    FILE* fptr = fopen(path, "wb");
    if (!fptr) {
        fprintf(stderr, "%s: cannot create file\n", path);
        return IO;
    }

    if (fwrite(&data->H, sizeof(data->H), 1, fptr) != 1) {
        fprintf(stderr, "%s: write error of H\n", path);
        need_remove_file = 1;
        ret = IO;
        goto cleanup;
    }
    if (fwrite(&data->W, sizeof(data->W), 1, fptr) != 1) {
        fprintf(stderr, "%s: write error of W\n", path);
        need_remove_file = 1;
        ret = IO;
        goto cleanup;
    }

    const size_t ABC_size = (size_t)MATRICES_COUNT * data->H * data->W;
    if (fwrite(data->ABC, 1, ABC_size, fptr) != ABC_size) {
        fprintf(stderr, "%s: write error of ABC\n", path);
        need_remove_file = 1;
        ret = IO;
        goto cleanup;
    }

cleanup:
    if (fclose(fptr) == EOF) {
        fprintf(stderr, "%s: write error on close\n", path);
        remove(path);
        ret = IO;
    }
    if (need_remove_file) {
        remove(path);
    }
    return ret;
}

static void out_data_destroy(out_data_t* data)
{
    free(data->ABC);

    out_data_init(data);
}
