#ifndef TABLE_H
#define TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define BUFF_SIZE 255

/* Definice struktur ---------------------------------------------------------------*/

typedef char Klic[BUFF_SIZE];

typedef struct _Hodnota {
    unsigned int id;
    uint16_t timeout;
    bool retry;
    char command[BUFF_SIZE];

} Hodnota;

typedef struct _HashTableNode {
    Klic klic;
    Hodnota *data;
    struct _HashTableNode *next;

} HashTableNode;

typedef struct _HashTable {
    HashTableNode **buckets;
    unsigned int countItems;
    unsigned int size;

} HashTable;

typedef void (*NodeProc)(unsigned int index, HashTableNode *node);

/* Deklarace funkci NACITANI Z KONZOLE ---------------------------------------------*/

bool GetString(char *line, int maxLen);
bool GetLine(char *line, int maxLen);
bool GetChar(char *c);
bool GetDouble(double *val);
bool GetUInt(unsigned int *val);
void ClearStdin();

/* Deklarace funkci PRACE S DATY ---------------------------------------------------*/

unsigned int HashIt(HashTable *table, Klic key);
Hodnota *DataGet(unsigned int Id);

/* Deklarace funkci HESH TABLE -----------------------------------------------------*/

HashTable *HashTable_Init(unsigned int size);
bool HashTable_Insert(HashTable *table, Klic key, Hodnota *data);
void ProcessFunction(unsigned int index, HashTableNode *node);
void HashTable_Process(HashTable *table, NodeProc proc);
void HashTable_Print(HashTable *table);

#endif // TABLE_H
