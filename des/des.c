#include <stdint.h>
#include <string.h>
#include "../utils.h"

char permuted_choice1[56] = {
    57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4
};

char permuted_choice2[48] = {
    14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

char key_schedule[16] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

char key[64] = {
    0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1,
    1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1
};

/**
 * @brief Prints an array of bits.
 */
void print_bit_array(char* array, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%u", array[i]);
    }
    printf("\n");
}

/**
 * @brief Rotates the elements of an array by x positions to the left.
 */
void left_rotate(char* array, size_t size, size_t x) {
    char temp[size];

    memcpy(temp, array + x, (size - x));
    memcpy(temp + size - x, array, x);
    memcpy(array, temp, 28);
}

/**
 * @brief Applies the PC-1 permutation to a key.
 */
void key_permutation(char* initial_key, char* permuted_key) {
    for (size_t i = 0; i < 56; i++) {
        permuted_key[i] = initial_key[permuted_choice1[i] - 1];
    }
}

/**
 * @brief Applies the PC-2 permutation to a subkey.
 */
void subkey_permutation(char* initial_subkey, char* permuted_subkey) {
    for (size_t i = 0; i < 48; i++) {
        permuted_subkey[i] = initial_subkey[permuted_choice2[i] - 1];
    }
}

/**
 * @brief Uses the key schedule and the permuted key to generate 16 subkeys.
 */
void generate_subkeys(char* permuted_key, char* subkeys) {
    char c_half[28];
    char d_half[28];
    char temp[56];

    // Split the permuted key into C0 and D0.
    memcpy(c_half, permuted_key, 28);
    memcpy(d_half, permuted_key + 28, 28);

    for (size_t i = 0; i < 16; i++) {
        char* permuted_subkey = safe_malloc(48 * sizeof *permuted_subkey);

        // Generate Cn and Dn according to the key schedule.
        left_rotate(c_half, 28, key_schedule[i]);
        left_rotate(d_half, 28, key_schedule[i]);

        // Concatenate the pairs of halves CnDn.
        memcpy(temp, c_half, 28);
        memcpy(temp + 28, d_half, 28);

        // Apply PC-2 to the concatenated pairs.
        subkey_permutation(temp, permuted_subkey);

        // Copy the permuted subkey into the subkeys array.
        memcpy(subkeys + i * 48, permuted_subkey, 48);
        free(permuted_subkey);
    }
}

int main() {
    char* initial_key = safe_malloc(64 * sizeof *initial_key);
    char* permuted_key = safe_malloc(56 * sizeof *permuted_key);
    char* subkeys = safe_malloc(16 * 48 * sizeof *subkeys);

    memcpy(initial_key, key, 64);
    key_permutation(initial_key, permuted_key);
    generate_subkeys(permuted_key, subkeys);

    for (size_t i = 0; i < 16; i++) {
        print_bit_array(subkeys + i * 48, 48);
    }

    free(initial_key);
    free(permuted_key);
    free(subkeys);
    return 0;
}