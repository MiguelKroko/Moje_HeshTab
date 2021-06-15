#include <stdio.h>
#include <stdlib.h>
#include "table.h"
#define INIT_SIZE 2
/* MAIN ------------------------------------------------- */
int main() {
    Klic key;
    Hodnota *data;

    printf("Inicializace HeshTabulky\n");
    HashTable *table = HashTable_Init(INIT_SIZE);
    if (table == NULL) return 1;
    printf("Inicializace HeshTabulky OK\n");

    //printf("Zadej klic\n");
    //GetLine(key, BUFF_SIZE);

    // DATA ID = 0
    data = DataGet(0);
    strcpy(key,"Cache");

    printf("KEY: %s ",key);
    printf("LEN: %u\n",(unsigned)strlen(key));

    if (HashTable_Insert(table,key,data) == false) return 1;
    else printf("Data ID 0 vlozena\n");

    // DATA ID = 1
    data = DataGet(1);
    strcpy(key,"Data");

    printf("KEY: %s ",key);
    printf("LEN: %u\n",(unsigned)strlen(key));

    if (HashTable_Insert(table,key,data) == false) return 1;
    else printf("Data ID 1 vlozena\n");

    // Tisk HashTable_Print
    HashTable_Print(table);

    // DATA ID = 2
    data = DataGet(2);
    strcpy(key,"SmallCache");

    printf("KEY: %s ",key);
    printf("LEN: %u\n",(unsigned)strlen(key));

    if (HashTable_Insert(table,key,data) == false) return 1;
    else printf("Data ID 2 vlozena\n");

    // DATA ID = 3
    data = DataGet(3);
    strcpy(key,"NotData");

    printf("KEY: %s ",key);
    printf("LEN: %u\n",(unsigned)strlen(key));

    if (HashTable_Insert(table,key,data) == false) return 1;
    else printf("Data ID 3 vlozena\n");

    // Tisk HashTable_Print
    HashTable_Print(table);

    //Process fce
    HashTable_Process(table, ProcessFunction);

    return 0;
}
