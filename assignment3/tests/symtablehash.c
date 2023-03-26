    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <assert.h>
    #include <errno.h>
    #define buckets_s 509
    #define HASH_MULTIPLIER 65599




    typedef struct Entry {
        char* key;
        void* value;
        struct Entry* next;
    }Entry;



    typedef struct {
        *bucket[buckets_s];
    }SymTable_S;

    typedef SymTable_S *SymTable_T;
    
    
    int SymTable_contains(SymTable_T oSymTable, const char *pcKey); //for smoother reading for the Assignment to not change the order


    static unsigned int T_hash(const char *pcKey){
	    size_t ui;
	    unsigned int uiHash = 0U;
	    for (ui = 0U; pcKey[ui] != '\0'; ui++) uiHash = uiHash * HASH_MULTIPLIER + pcKey[ui];
        return uiHash % buckets_s;
    }



    SymTable_T SymTable_new(void){  
        SymTable_T new_table;
        size_t i;

        new_table = malloc(sizeof(SymTable_S));
        assert(new_table);

        for(i=0U;i<buckets_s;i++){
	        new_table->bucket[i]= NULL;
        }
        return new_table;
    };


    void SymTable_free(SymTable_T oSymTable){
        int i;
        for(i=0;i<buckets_s;i++){
            Entry *current = oSymTable->bucket[i];
            while (current != NULL) {
                Entry *temp = current->next;
                free(current->key);
                free(current);
                current = temp;
            }
            free(oSymTable->bucket);
            free(oSymTable);
        }
    };

	unsigned int SymTable_getLength(SymTable_T oSymTable){
        int count=0;
        int i=0;
        assert(oSymTable != NULL);
        
        for(i;i<buckets_s;i++){
            Entry* current = oSymTable->bucket[i];
            while (current != NULL) {
                current = current->next;
                count++;
            }
        }
        return count;
    };

	int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void *pvValue){
        Entry* put_entry = malloc(sizeof(Entry));

        assert(pcKey != NULL);
        if(SymTable_contains(oSymTable,pcKey)==1){
            return 0;
        }

        
        put_entry->key = malloc(sizeof(char) * strlen(pcKey)+1);
        strcpy(put_entry->key, pcKey);
        put_entry->value = (void *)pvValue;
        put_entry->next = oSymTable->bucket[T_hash(pcKey)%buckets_s];
        oSymTable->bucket[T_hash(pcKey)%buckets_s] = put_entry;
        return 1;
    };

	int SymTable_remove(SymTable_T oSymTable, const char *pcKey){
        Entry* previous = NULL;
        Entry* current = oSymTable->bucket[T_hash(pcKey)];
        assert(pcKey != NULL);
        
        while (current != NULL) {
            if(strcmp(current->key, pcKey) == 0){
                if (previous == NULL) {
                    oSymTable->bucket[T_hash(pcKey)] = current->next;  
                }else{
                    previous->next = current->next;
                }
                free(current);
                return 1;
            }
            previous = current;
            current = current->next;
        }
        
    };

	int SymTable_contains(SymTable_T oSymTable, const char *pcKey){
        Entry* current = oSymTable->bucket[T_hash(pcKey)];
        assert(pcKey != NULL);
        while (current != NULL) {
            if(strcmp(current->key, pcKey) == 0) return 1; 
            current = current->next;
        }
        return 0;

        
       
    };

	void *SymTable_get(SymTable_T oSymTable, const char *pcKey){
        Entry* current = oSymTable->bucket[T_hash(pcKey)];
        assert(pcKey != NULL);
        assert(oSymTable != NULL);
        while (current != NULL) {
            if(strcmp(current->key, pcKey) == 0) return current->value;
            current = current->next;
        }
        
        return NULL;
    };

	void SymTable_map(SymTable_T oSymTable, void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),const void *pvExtra){
        assert(oSymTable != NULL);
        assert(pfApply != NULL);
        for(int i=0;i<buckets_s;i++){
            Entry* current = oSymTable->bucket[i];
            while (current != NULL) {
                pfApply(current->key,current->value,(void *)pvExtra);
                current = current->next;
            }
        }
    };

