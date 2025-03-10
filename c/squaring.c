/*
 * Squaring square with minimal amount of squares
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

// For strange indexing, starting from 1
#define MOEVM_STYLE true

typedef struct {
        int x;
        int y;
        int w; // width
} square_t;

// Mock for a while
size_t primes[]    = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41};
size_t primes_size = 13;

// Cover square with even side width
// NOTE: return must be manualy freed
square_t* cover_even(int n, size_t* division_size)
{
        // If n is less than 1 or n is odd
        if (n < 1 || (n & 1) != 0) {
                printf("Incorrect value for n [%d]!\n", n);
                exit(EXIT_FAILURE);
        }

        square_t* division = (square_t*)malloc(4 * sizeof(square_t));
        
        // Left-up corner
        division[0].x = 0;
        division[0].y = 0;
        division[0].w = n >> 1;

        // Right-up corner
        division[1].x = n >> 1;
        division[1].y = 0;
        division[1].w = n >> 1;

        // Left-down corner
        division[2].x = 0;
        division[2].y = n >> 1;
        division[2].w = n >> 1;

        // Right-down corner
        division[3].x = n >> 1;
        division[3].y = n >> 1;
        division[3].w = n >> 1;
        
        *division_size = 4;

        return division;
}

// Check if can place square with given width to givern coordinates
bool can_place_square(const int* field, int n,
                      int x0, int y0, int w)
{
        
        /*
         * Here we need to check only 4 corners:
         * - (x; y)
         * - (x + w; y)
         * - (x; y + w)
         * - (x + w; y + w)
         */

        if (x0 >= 0 && y0 >= 0 && x0 + w - 1 < n && y0 + w - 1 < n) {
                return field[y0 * n + x0] == 0 &&
                       field[y0 * n + (x0 + w - 1)] == 0 &&
                       field[(y0 + w - 1) * n + x0] == 0 &&
                       field[(y0 + w - 1) * n + (x0 + w - 1)] == 0;
        }

        return false;
}

// Get max possible square width for given coordinates
int max_square_width(const int* field, int n,
                     int x0, int y0)
{
        int max_width = 0;
        while (can_place_square(field, n, x0, y0, max_width + 1))
                ++max_width;

        return max_width;
}

// Place new square to field
void place_square(int* field, int n,
                  int x0, int y0, int w, int id)
{
        if (!can_place_square(field, n, x0, y0, w)) {
                fprintf(stderr, "Cannot place square with width %d at (%d; %d)\n",
                                w, x0, y0);
                exit(EXIT_FAILURE);
        }

        for (size_t y = y0; y < y0 + w; ++y) {
                for (size_t x = x0; x < x0 + w; ++x)
                        field[y * n + x] = id;
        }
}

// Remove square from field
void remove_square(int* field, int n,
                   int x0, int y0, int w)
{
        if (x0 >= 0 && y0 >= 0 && x0 + w - 1 < n && y0 + w - 1 < n) {
                for (size_t y = y0; y < y0 + w; ++y) {
                        for (size_t x = x0; x < x0 + w; ++x)
                                field[y * n + x] = 0;
                }
        }
}

// Print field matrix
void print_field(const int* field, int n)
{
        for (size_t y = 0; y < n; ++y) {
                for (size_t x = 0; x < n; ++x)
                        printf("%d ", field[y * n + x]);
                printf("\n");
        }
}

static size_t calls_count = 0;
void backtrack(int* field, int n,
               int remaining_cells,
               square_t* division,
               size_t division_size,
               square_t* best_division,
               size_t* best_division_size)
{
        ++calls_count;
        
        // Current division longer than best
        if (division_size >= *best_division_size && *best_division_size != 0)
                return;

        // No cells to insert remain
        if (remaining_cells == 0) {
                *best_division_size = division_size;
                memcpy(best_division, division, division_size * sizeof(square_t));
                return;
        }
        
        // Get first empty cell coordinates
        int x0 = -1, y0 = -1;
        bool found = false;
        for (size_t y = 0; y < n && !found; ++y) {
                for (size_t x = 0; x < n && !found; ++x) {
                        if (field[y * n + x] == 0) {
                                x0 = x;
                                y0 = y;
                                found = true;
                        }
                }
        }

        // Probe all possible square widths for current position
        for (int width = max_square_width(field, n, x0, y0); width >= 1; --width) {
                place_square(field, n, x0, y0, width, division_size + 1 + 3);

                division[division_size].x = x0;
                division[division_size].y = y0;
                division[division_size].w = width;

                /*
                 * Add 1 to division size, so it will remain
                 * unchanged after function execution.
                 */
                backtrack(field, n,
                          remaining_cells - width * width,
                          division, division_size + 1,
                          best_division, best_division_size);

                remove_square(field, n, x0, y0, width);
        }
}

// Cover square with prime side width
// NOTE: return must be manualy freed
square_t* cover_prime(int n, size_t* division_size)
{
        int field[n][n];
        memset(&field[0][0], 0, n * n * sizeof(int));

        square_t* division = (square_t*)malloc(128 * sizeof(square_t));
        
        square_t* best_division = (square_t*)malloc(128 * sizeof(square_t));
        size_t   best_division_size = 0;
        
        // Right-down
        place_square(&field[0][0], n, n >> 1, n >> 1, (n >> 1) + 1, 1);
        
        // Right-up
        place_square(&field[0][0], n, (n >> 1) + 1, 0, n >> 1, 2);
        
        // Left-down
        place_square(&field[0][0], n, 0, (n >> 1) + 1, n >> 1, 3);

        // Recursively search for best possible division
        backtrack(&field[0][0], n,
                  pow(n, 2) - pow((n >> 1) + 1, 2) - 2 * pow(n >> 1, 2),
                  division, 0,
                  best_division, &best_division_size);

        // Add right-down square to division
        best_division[best_division_size].x = n >> 1;
        best_division[best_division_size].y = n >> 1;
        best_division[best_division_size].w = (n >> 1) + 1;
        ++best_division_size;

        // Add right-up square to division
        best_division[best_division_size].x = (n >> 1) + 1;
        best_division[best_division_size].y = 0;
        best_division[best_division_size].w = n >> 1;
        ++best_division_size;
        
        // Add left-down square to division
        best_division[best_division_size].x = 0;
        best_division[best_division_size].y = (n >> 1) + 1;
        best_division[best_division_size].w = n >> 1;
        ++best_division_size;
        
        *division_size = best_division_size;

        free(division);

        return best_division;
}

square_t* solve(int n, size_t* division_size)
{
        // Incorrect input
        if (n < 2) {
                fprintf(stderr, "Error: cannot process n = %d\n", n);
                return NULL;
        }

        square_t* division = NULL;

        // If square width is even
        if ((n & 1) == 0) {
                division = cover_even(n, division_size);
                if (MOEVM_STYLE) {       
                        for (size_t i = 0; i < *division_size; ++i) {
                                ++division[i].x;
                                ++division[i].y;
                        }
                }               
        } else {
                // Find first prime divider of n
                size_t first_prime = 0;
                for (size_t i = 0; i < primes_size; ++i) {
                        if (n % primes[i] == 0) {
                                first_prime = primes[i];
                                break;
                        }
                }
                if (first_prime == 0) {
                        fprintf(stderr, "Prime divider of n was not found!\n");
                        exit(EXIT_FAILURE);
                }

                division = cover_prime(first_prime, division_size);
                if (!division) {
                        fprintf(stderr, "Something went wrong in cover_prime!\n");
                        exit(EXIT_FAILURE);
                }

                // Multiply division on mult_factor if necessary (for composite numbers like 15)
                int mult_factor = n / first_prime;
                for (size_t i = 0; i < *division_size; ++i) {
                        division[i].x *= mult_factor;
                        division[i].y *= mult_factor;
                        division[i].w *= mult_factor;

                        if (MOEVM_STYLE) {
                                ++division[i].x;
                                ++division[i].y;
                        }
                }
                
        }

        return division;
}

int main(void)
{
        int n = 0;
        scanf("%d", &n);

        square_t* division = NULL;
        size_t    division_size = 0;
        
        division = solve(n, &division_size);
        if (!division) {
                fprintf(stderr, "solve() returned NULL\n");
                exit(EXIT_FAILURE);
        }

        // Print result
        printf("%ld\n", division_size);
        for (size_t i = 0; i < division_size; ++i) {
                printf("%d %d %d\n", division[i].x,
                                     division[i].y,
                                     division[i].w);
        }

        printf("\nCalls count: %ld\n", calls_count);

        free(division);
        
        return 0;
}
