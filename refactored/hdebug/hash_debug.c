/* Hash Collision Debugger for ZynkVM
 * Usage:
 *   ./hash_debug [-f file] [-a] [-i]
 * Options:
 *   -f file : Load words from file (one per line)
 *   -a      : Test all possible combinations
 *   -i      : Interactive mode (manual input)
 */

#include "../core/hash.h"
#include "../core/map.h"
#include "../common/common.h"
#include "../core/core.h"
#include "../core/types/value.h"
#include "../core/chunk.h"
#include "../core/types/object.h"
#include "../core/vm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#define MAX_WORDS 1000000
#define WORD_SIZE 8

typedef struct {
    char word[WORD_SIZE];
    uint32_t hash;
} HashEntry;

void test_word(HashEntry *entries, size_t *count, const char *word, FILE *log) {
    char padded[WORD_SIZE] = {0};
    strncpy(padded, word, WORD_SIZE);
    
    uint32_t h = hash8Str(padded);
    
    // Buscar colisiones
    for (size_t i = 0; i < *count; i++) {
        if (entries[i].hash == h && strncmp(entries[i].word, padded, WORD_SIZE) != 0) {
            fprintf(log, "Colisión detectada:\n");
            fprintf(log, "  Palabra 1: '");
            for(int j = 0; j < WORD_SIZE; j++) {
                fprintf(log, "%c", zynkMapToASCII(entries[i].word[j]));
            }
            fprintf(log, "' (hex: ");
            for(int j = 0; j < WORD_SIZE; j++) {
                fprintf(log, "%02x ", (unsigned char)entries[i].word[j]);
            }
            fprintf(log, ")\n  Palabra 2: '");
            for(int j = 0; j < WORD_SIZE; j++) {
                fprintf(log, "%c", zynkMapToASCII(padded[j]));
            }
            fprintf(log, "' (hex: ");
            for(int j = 0; j < WORD_SIZE; j++) {
                fprintf(log, "%02x ", (unsigned char)padded[j]);
            }
            fprintf(log, ")\n  Hash: %u\n\n", h);
            fflush(log);
            
            printf("Presiona ENTER para continuar...");
            getchar();
        }
    }
    
    // Agregar nueva entrada
    if (*count < MAX_WORDS) {
        memcpy(entries[*count].word, padded, WORD_SIZE);
        entries[*count].hash = h;
        (*count)++;
    }
}

void generate_combinations(HashEntry *entries, size_t *count, char *current, int pos, FILE *log) {
    if (pos == WORD_SIZE) {
        test_word(entries, count, current, log);
        return;
    }
    
    for (int i = 0; i < ZYNK_MAP_RANGE; i++) {
        current[pos] = i;
        generate_combinations(entries, count, current, pos + 1, log);
    }
}

int main(int argc, char *argv[]) {
    HashEntry *entries = malloc(sizeof(HashEntry) * MAX_WORDS);
    size_t count = 0;
    FILE *log = fopen("collisions.log", "w");
    char opt;
    
    if (!entries || !log) {
        printf("Error: No se pudo inicializar\n");
        return 1;
    }

    while ((opt = getopt(argc, argv, "f:ai")) != -1) {
        switch (opt) {
            case 'f': {
                FILE *f = fopen(optarg, "r");
                if (!f) {
                    printf("Error: No se pudo abrir %s\n", optarg);
                    return 1;
                }
                char line[256];
                while (fgets(line, sizeof(line), f)) {
                    if (line[strlen(line)-1] == '\n') 
                        line[strlen(line)-1] = '\0';
                    test_word(entries, &count, line, log);
                }
                fclose(f);
                break;
            }
            case 'a': {
                char word[WORD_SIZE + 1] = {0};
                generate_combinations(entries, &count, word, 0, log);
                break;
            }
            case 'i': {
                printf("Modo interactivo (Ctrl+D para salir)\n");
                char input[256];
                while (fgets(input, sizeof(input), stdin)) {
                    if (input[strlen(input)-1] == '\n')
                        input[strlen(input)-1] = '\0';
                    test_word(entries, &count, input, log);
                }
                break;
            }
        }
    }

    fclose(log);
    free(entries);
    return 0;
}