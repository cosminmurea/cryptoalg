#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define ROW_BITS_MASK           0x0000840000000000
#define COLUMN_BITS_MASK        0x0000780000000000
#define LOWEST_BIT_MASK64       0x0000000000000001
#define LOWEST_BIT_MASK32       0x00000001
#define LOWEST28_BITS_MASK32    0x0FFFFFFF
#define LOWEST28_BITS_MASK64    0x000000000FFFFFFF
#define LOW_MASK64              0x00000000FFFFFFFF
#define HIGH_MASK64             0xFFFFFFFF00000000

static uint8_t permuted_choice1[56] = {
    57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4
};

static uint8_t permuted_choice2[48] = {
    14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47,
    55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

static uint8_t key_schedule[16] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

static uint8_t initial_permutation_table[64] = {
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9 , 1, 59, 51, 43, 35, 27, 19, 11, 3, 61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

static uint8_t expansion_table[48] = {
    32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
};

static uint8_t s_box_tables[8][64] = {
    {
        14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7, 0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
        4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0, 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
    },
    {
        15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10, 3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
        0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15, 13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
    },
    {
        10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8, 13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
        13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7, 1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
    },
    {
        7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15, 13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
        10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4, 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14
    },
    {
        2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9, 14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
        4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14, 11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3
    },
    {
        12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11, 10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
        9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6, 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
    },
    {
        4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1, 13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
        1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2, 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
    },
    {
        13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7, 1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
        7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8, 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
    }
};

static uint8_t p_permutation_table[32] = {
    16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10, 2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25
};

static uint8_t final_permutation_table[64] = {
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25
};

static uint64_t key_permutation(uint64_t key) {
    uint64_t permuted_key = 0;
    for (size_t i = 0; i < 56; i++) {
        permuted_key <<= 1;
        permuted_key |= (key >> (64 - permuted_choice1[i])) & LOWEST_BIT_MASK64;
    }
    return permuted_key;
}

static uint64_t subkey_permutation(uint64_t subkey) {
    uint64_t permuted_subkey = 0;
    for (size_t i = 0; i < 48; i++) {
        permuted_subkey <<= 1;
        permuted_subkey |= (subkey >> (56 - permuted_choice2[i])) & LOWEST_BIT_MASK64;
    }
    return permuted_subkey;
}

static uint32_t left_rotate_bits(uint32_t value, size_t x) {
    for (size_t i = 0; i < x; i++) {
        value = (LOWEST28_BITS_MASK32 & (value << 1)) | (LOWEST_BIT_MASK32 & (value >> 27));
    }
    return value;
}

static void print_bits64(uint64_t value, size_t x) {
    for (size_t i = 64 - x; i < 64; i++) {
        uint64_t temp = (value >> (64 - i - 1)) & LOWEST_BIT_MASK64;
        printf("%c", temp ? '1' : '0');
    }
    printf("\n");
}

static void print_bits32(uint32_t value, size_t x) {
    for (size_t i = 32 - x; i < 32; i++) {
        uint32_t temp = (value >> (32 - i - 1)) & LOWEST_BIT_MASK32;
        printf("%c", temp ? '1' : '0');
    }
    printf("\n");
}

static void generate_subkeys(uint64_t key, uint64_t* subkeys) {
    uint64_t permuted_key = key_permutation(key);
    uint32_t c_half = (uint32_t)((permuted_key >> 28) & LOWEST28_BITS_MASK64);
    uint32_t d_half = (uint32_t)((permuted_key & LOWEST28_BITS_MASK64));
    uint64_t temp = 0;
    for (size_t i = 0; i < 16; i++) {
        c_half = left_rotate_bits(c_half, key_schedule[i]);
        d_half = left_rotate_bits(d_half, key_schedule[i]);
        temp = (uint64_t)c_half << 28 | d_half;
        temp = subkey_permutation(temp);
        subkeys[i] = temp;
    }
}

static uint64_t initial_permutation(uint64_t message_block) {
    uint64_t permuted_msg_block = 0;
    for (size_t i = 0; i < 64; i++) {
        permuted_msg_block <<= 1;
        permuted_msg_block |= (message_block >> (64 - initial_permutation_table[i])) & LOWEST_BIT_MASK64;
    }
    return permuted_msg_block;
}

static uint64_t expansion(uint32_t message_block_half) {
    uint64_t expanded = 0;
    for (size_t i = 0; i < 48; i++) {
        expanded <<= 1;
        expanded |= (message_block_half >> (32 - expansion_table[i])) & LOWEST_BIT_MASK64;
    }
    return expanded;
}

static uint32_t s_boxes(uint64_t intermediate_block) {
    uint32_t s_box_result = 0;
    uint8_t row_bits = 0;
    uint8_t column_bits = 0;
    for (size_t i = 0; i < 8; i++) {
        row_bits = (uint8_t)((intermediate_block & (ROW_BITS_MASK >> (i * 6))) >> (42 - i * 6));
        row_bits = (row_bits >> 4) | (row_bits & 0x01);
        column_bits = (uint8_t)((intermediate_block & (COLUMN_BITS_MASK >> (i * 6))) >> (43 - i * 6));
        s_box_result <<= 4;
        s_box_result |= (uint32_t)(s_box_tables[i][16 * row_bits + column_bits]);
    }
    return s_box_result;
}

static uint32_t p_permutation(uint32_t s_box_block) {
    uint32_t permuted = 0;
    for (size_t i = 0; i < 32; i++) {
        permuted <<= 1;
        permuted |= (s_box_block >> (32 - p_permutation_table[i]) & LOWEST_BIT_MASK32);
    }
    return permuted;
}

static uint64_t f_function(uint32_t message_half, uint64_t round_key) {
    uint64_t expanded_half = expansion(message_half);
    uint64_t intermediate = expanded_half ^ round_key;
    uint32_t s_box_result = s_boxes(intermediate);
    return p_permutation(s_box_result);
}

static uint64_t rounds(uint32_t l_half, uint32_t r_half, uint64_t* subkeys, uint8_t mode) {
    uint64_t final_block = 0;
    uint32_t temp = 0;
    for (size_t i = 0; i < 16; i++) {
        temp = r_half;
        if (mode == 'd') {
            r_half = l_half ^ f_function(r_half, subkeys[15 - i]);
        } else if (mode == 'e') {
            r_half = l_half ^ f_function(r_half, subkeys[i]);
        } else {
            printf("Unsupported mode!!");
        }
        l_half = temp;
    }
    final_block = ((uint64_t)r_half << 32) | ((uint64_t)l_half);
    return final_block;
}

static uint64_t final_permutation(uint64_t intermediate_block) {
    uint64_t permuted = 0;
    for (size_t i = 0; i < 64; i++) {
        permuted <<= 1;
        permuted |= (intermediate_block >> (64 - final_permutation_table[i]) & LOWEST_BIT_MASK64);
    }
    return permuted;
}

static uint64_t des(uint64_t message_block, uint64_t key, uint8_t mode) {
    uint64_t output_block = 0;
    uint64_t permuted_message_block = 0;
    uint64_t rounds_block = 0;
    uint64_t subkeys[16] = {0};
    uint32_t l_half = 0;
    uint32_t r_half = 0;
    generate_subkeys(key, subkeys);
    permuted_message_block = initial_permutation(message_block);
    l_half = (uint32_t)(permuted_message_block >> 32);
    r_half = (uint32_t)(permuted_message_block & LOW_MASK64);
    rounds_block = rounds(l_half, r_half, subkeys, mode);
    output_block = final_permutation(rounds_block);
    return output_block;
}

int main() {
    uint64_t key = 0x133457799BBCDFF1;
    uint64_t message = 0x123456789ABCDEF;
    uint64_t encrypted_message = des(message, key, 'e');
    print_bits64(encrypted_message, 64);
    return 0;
}