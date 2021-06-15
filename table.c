#include <math.h>
#include "table.h"

/* Definice funkci NACITANI Z KONZOLE --------------------------------------------*/

bool GetString(char *line, int maxLen) {
    if (line == NULL) return false;
    ClearStdin();
    if (fgets(line,maxLen,stdin) == NULL) {
        printf("Konec souboru.\n");
        return false;
    }
    return true;
}

bool GetLine(char *line, int maxLen) {
    if (GetString(line, maxLen)) {
        int poziceEOL = strcspn(line, "\n\r"); // ustrihuti textu za koncem radku na tento znak vlozim NUL
        line[poziceEOL] = 0;
        return true;
    }
    return false;
}


bool GetChar(char *c) {
    if (c == NULL) return false;
    char line[BUFF_SIZE];
    if (GetString(line, BUFF_SIZE) == true) {
        *c = line[0];
        return true;
    }
    printf("Chyba");
    return false;
}

bool GetDouble(double *val){
    if (val == NULL) return false;
    char line[BUFF_SIZE];
    double hodnota;
    int ok = 0;

    while (ok != 1) {
        if (GetString(line, BUFF_SIZE) == false) return false;
        ok = sscanf(line,"%lf",&hodnota);
        if (ok != 1) printf("Chyba - je treba zadat cislo");
    }

    *val = hodnota;
    return true;
}

bool GetUInt(unsigned int *val) {
    if (val == NULL) return false;
    char line[BUFF_SIZE];
    unsigned int hodnota;
    int ok = 0;

    while (ok != 1) {
        if (GetString(line, BUFF_SIZE) == false) return false;
        ok = sscanf(line,"%u",&hodnota);
        if (ok != 1) printf("Chyba - je treba zadat cele kladne cislo");
    }
    *val = hodnota;
    return true;
}

void ClearStdin() {
    fflush(stdin);
}

/* Definice funkci PRACE S DATY ---------------------------------------------------*/

unsigned int HashIt(HashTable *table, Klic key) {

    if (table == NULL || strlen(key) == 0) return 0;
    unsigned index = strlen(key) % table->size;
    return index;
}

Hodnota *DataGet(unsigned int id) {
    Hodnota *data = malloc(sizeof (Hodnota));
    if (data == NULL) return NULL;

    /*
    printf("Zadej ID: ");
    if (GetUInt(&data->id) == false) return false;
    */

    data->id = id;
    data->timeout = 0x00;
    data->retry = false;
    strcpy(data->command,"");

    return data;
}





/* Definice funkci HESH TABLE -----------------------------------------------------*/

HashTable *HashTable_Init(unsigned int size) {
    if (size == 0) return NULL;

    HashTable *table = malloc(sizeof (HashTable));
    if (table == NULL) return NULL;

    table->buckets = calloc(size, sizeof (HashTableNode*));
    if (table->buckets == NULL) {
        free(table);
        return NULL;
    }

    table->size = size;
    table->countItems = 0;

    return table;
}


bool HashTable_Insert(HashTable *table, Klic key, Hodnota *data) {
    if (table == NULL || data == NULL || strlen(key) == 0) return false;

    // pocitame synonyma
    unsigned int count = 0;
    unsigned int index = HashIt(table, key);
    // hledani duplicitniho klice kontrola synonym
    HashTableNode *ptrNod = table->buckets[index];
    while (ptrNod != NULL) {
        ++count;
        if (strcmp(key, ptrNod->klic) == 0) {
            printf("Duplicitni klic\n");
            return false;
        }
        ptrNod = ptrNod->next;
    }

    // vytvoreni nodu, naplneni daty
    HashTableNode *newNode = malloc(sizeof (HashTableNode));
    if (newNode == NULL) return false;
    strcpy(newNode->klic,key);
    newNode->data = data;
    newNode->next = table->buckets[index];
    // ulozit do buckets ukazatel na prvni nod = novy nod
    table->buckets[index] = newNode;
    ++table->countItems;
    printf("Novy datovy nod pridan\n");

    // if count byl 1 a pridal sem na 2 - treba rehash
    if (count == 1) {
        printf("Delam REHASH\n");
        unsigned int newSize = (table->size)*2;
        HashTableNode **newBuckets = calloc(newSize, sizeof (HashTableNode*));
        if (newBuckets == NULL) return true;
        // mam nachystane nove buckets, kam presouvam ze starych nody
        // projdeme stare buckets a vsem prepocitame index z key a zaradime do novych
        for (unsigned int i = 0; i < table->size; ++i) {
            if (table->buckets[i] != NULL) {
                ptrNod = table->buckets[i];
                HashTableNode *ptrNext;
                while (ptrNod != NULL) {
                    ptrNext = ptrNod->next; // ulozim si ukaztel na potencialni dalsi nod
                    ptrNod->next = NULL; // aktivnimu nodu smazu ukazatel na next

                    index = strlen(ptrNod->klic) % newSize; // novy index pro zarazeni do zvetsene tabulky

                    ptrNod->next = newBuckets[index];
                    newBuckets[index] = ptrNod;

                    ptrNod = ptrNext; // posunu aktivni nod na byvaly next
                }
            }
        }
        // stara tabulka ukazatelu na nody nepotrebuju - uvolnit pamet
        free(table->buckets);
        // ukazatel bude ukazovat na novou tabulku
        table->buckets = newBuckets;
        table->size = newSize; // nastaveni spravne size
    }
    return true;
}


void HashTable_Print(HashTable *table) {
    if (table == NULL) return;

    for (unsigned int i = 0; i < table->size; ++i) {
        printf("Bucket: <index: %u> ||", i);
        if (table->buckets[i] != NULL) {
            HashTableNode *ptrNode = table->buckets[i];
            while (ptrNode != NULL) {
                printf(" <ID: %u>", ptrNode->data->id);
                ptrNode = ptrNode->next;
                if (ptrNode != NULL) printf(" |");
            }
        }
        printf("\n");
    }
}

void HashTable_Process(HashTable *table, NodeProc proc) {
    if (table == NULL || proc == NULL) return;

    for (unsigned int i = 0; i < table->size; ++i) {
        if (table->buckets[i] != NULL) {
            HashTableNode *ptrNode = table->buckets[i];
            while (ptrNode != NULL) {
                proc(i, ptrNode);
                printf("<TF> Hash: %u | ID: %u | Index: %u \n", (unsigned)strlen(ptrNode->klic), ptrNode->data->id, i);
                ptrNode = ptrNode->next;
            }
        }
    }
}


void ProcessFunction(unsigned int index, HashTableNode *node) {
    if (node == NULL) return;

    unsigned int hesh = strlen(node->klic);

    printf("<PF> Hash: %u | ID: %u | Index: %u \n", hesh, node->data->id, index);
}

















