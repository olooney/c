#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "rabin_karp.h"

char *slurp(const char *filename);

int main(int argc, char** argv) {
    const char* filename = "../data/brothers_karamazov.txt";
    // const char* filename = "../data/repo.txt";
    char* patterns[90] = {
    "amen", "angels", "apocrypha", "apostles", "ascension", "atonement",
    "baptism", "beatitudes", "belief", "benediction", "bible", "christ",
    "church", "clergy", "commandment", "communion", "compassion", "confession",
    "consecration", "conversion", "creed", "crucifixion", "deity", "demons",
    "devotion", "divine", "doctrine", "dogma", "doxology", "ecclesiastical",
    "epiphany", "eschatology", "eternal", "eucharist", "evangelical",
    "evangelism", "evil", "faith", "forgiveness", "goodness", "gospel",
    "grace", "hallelujah", "heaven", "hell", "heresy", "holy", "immaculate",
    "immutable", "intercession", "jesus", "judgment", "love", "martyrdom",
    "mercy", "messiah", "miracles", "mortal", "orthodoxy", "parable",
    "parousia", "penance", "pentecost", "prayer", "predestination", "prophets",
    "providence", "purgatory", "purity", "redemption", "regeneration",
    "repentance", "reprobate", "resurrection", "revelations", "sacrament",
    "saints", "salvation", "sanctification", "sermon", "spirit", "spiritual",
    "suffering", "temptation", "tenets", "theodicy", "theology",
    "transfiguration", "trinity", "worship"
    };

    char* text = slurp(filename);

    printf("searching %zu characters of file \"%s\" for \"%s", strlen(text), filename, patterns[0]);
    for ( int i=1; i < 90; i++ ) {
        printf("|%s", patterns[i]);
    }
    printf("\"\n");

    // start timers
    clock_t cpu_start, cpu_end;
    struct timeval wall_start, wall_end;
    double cpu_time_used;
    gettimeofday(&wall_start, NULL);
    cpu_start = clock();

    
    rabin_karp_many(text, patterns, 90, true);

    // end timers
    gettimeofday(&wall_end, NULL);
    cpu_end = clock();
    cpu_time_used = ((double) (cpu_end - cpu_start)) / CLOCKS_PER_SEC;
    printf("CPU Time: %f seconds\n", cpu_time_used);
    long seconds = wall_end.tv_sec - wall_start.tv_sec;
    long useconds = wall_end.tv_usec - wall_start.tv_usec;
    double elapsed = seconds + useconds * 1e-6;
    printf("Elapsed Time: %f seconds\n", elapsed);

    // cleanup
    free(text);
}

/*
int main(int argc, char** argv) {
    if ( argc >= 3 ) {
        char* text = slurp(argv[2]);
        printf("searching %zu characters of file \"%s\" for \"%s\"\n", strlen(text), argv[2], argv[1]);
        clock_t cpu_start, cpu_end;
        struct timeval wall_start, wall_end;
        double cpu_time_used;

        // start timers
        gettimeofday(&wall_start, NULL);
        cpu_start = clock();

        rabin_karp(text, argv[1]);

        // end timers
        gettimeofday(&wall_end, NULL);
        cpu_end = clock();


        // report time
        cpu_time_used = ((double) (cpu_end - cpu_start)) / CLOCKS_PER_SEC;
        printf("CPU Time: %f seconds\n", cpu_time_used);

        long seconds = wall_end.tv_sec - wall_start.tv_sec;
        long useconds = wall_end.tv_usec - wall_start.tv_usec;
        double elapsed = seconds + useconds * 1e-6;
        printf("Elapsed Time: %f seconds\n", elapsed);

        // cleanup
        free(text);
    } else {
        rabin_karp("TEST THIS IS A LONG TEST STRING FOR A TEST", "TEST");
    }
    return 0;
}
*/

// TODO: utility function
char *slurp(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Failed to open file");
        return NULL;
    }

    // Seek to the end of the file to determine the file size
    if (fseek(file, 0, SEEK_END) != 0) {
        perror("Failed to seek file");
        fclose(file);
        return NULL;
    }
    
    long size = ftell(file);
    if (size == -1) {
        perror("Failed to tell file position");
        fclose(file);
        return NULL;
    }
    
    // Allocate memory for the entire file content plus a null terminator
    char *buffer = malloc(size + 1);
    if (buffer == NULL) {
        perror("Failed to allocate memory");
        fclose(file);
        return NULL;
    }
    
    // Go back to the start of the file to read contents
    rewind(file);
    size_t readSize = fread(buffer, 1, size, file);
    if (readSize < (size_t)size) {
        perror("Failed to read file");
        free(buffer);
        fclose(file);
        return NULL;
    }

    // Null-terminate the string
    buffer[size] = '\0';

    fclose(file);
    return buffer;
}
