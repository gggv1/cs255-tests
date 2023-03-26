    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <assert.h>


    typedef struct Entry {
        char* key;
        void* value;
        struct Entry* next;
    }Entry;


    typedef struct {
        Entry* list;
    }SymTable_S;

    typedef SymTable_S *SymTable_T;

    SymTable_T SymTable_new(void){
        SymTable_T new_table = malloc(sizeof(SymTable_T)); 
        assert(new_table);
        new_table->list = NULL;
        return new_table;
    };

    void SymTable_free(SymTable_T oSymTable){
        Entry *current = oSymTable->list;
        while (current != NULL) {
            Entry *temp = current->next;
            free(current);
            current = temp;
        }
        free(oSymTable->list);
        free(oSymTable);

    };

	unsigned int SymTable_getLength(SymTable_T oSymTable){
        int count=0;
        Entry* current = oSymTable->list;
        assert(oSymTable);
        while (current != NULL) {
            count++;
            current = current->next;
        }
        return count;
    };

	int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void *pvValue){
        Entry* current = oSymTable->list;
        Entry* put_entry = malloc(sizeof(Entry));
        assert(pcKey);
        while (current != NULL) {
          
            if(strcmp(current->key, pcKey) == 0){
            
             return 0;
            }
            current = current->next;
        }
      
        
        put_entry->key = malloc(sizeof(char) * strlen(pcKey)+1);
        strcpy(put_entry->key, pcKey);
        put_entry->value = (void *)pvValue;
        
        put_entry->next = oSymTable->list;
        oSymTable->list = put_entry;
       
        return 1;
    };

	int SymTable_remove(SymTable_T oSymTable, const char *pcKey){
        Entry* previous = NULL;
        Entry* current = oSymTable->list;
        assert(pcKey);
        while (current != NULL) {
            if(strcmp(current->key, pcKey) == 0){
                if (previous == NULL) {
                    oSymTable->list = current->next;
                } else {
                    previous->next = current->next;
                }
                free(current);
                return 1;
            }
            previous = current;
            current = current->next;
        }
        
        return 0;
    };

	int SymTable_contains(SymTable_T oSymTable, const char *pcKey){
        Entry* current = oSymTable->list;
        assert(pcKey);
        while (current != NULL) {
            if(strcmp(current->key, pcKey) == 0) return 1;
            current = current->next;
        }
        
        return 0;
    };

	void *SymTable_get(SymTable_T oSymTable, const char *pcKey){
        Entry* current = oSymTable->list;
        assert(pcKey);
        assert(oSymTable);

        while (current != NULL) {
            if(strcmp(current->key, pcKey) == 0) return current->value;
            current = current->next;
        }
        return NULL;

    };

	void SymTable_map(SymTable_T oSymTable, void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra) ,const void *pvExtra){
        Entry* current = oSymTable->list;
        assert(oSymTable);
        assert(pfApply);
        while (current) {
            pfApply(current->key,current->value,(void *)pvExtra);
            current = current->next;
        }
    };