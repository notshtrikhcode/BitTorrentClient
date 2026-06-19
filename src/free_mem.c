#include <stdio.h>
#include <stdlib.h>
#include "bencode.h"

typedef void (*HandlerFree)(BencodeValue* val);
static void free_int(BencodeValue* val){
    val -> v_integer = 0;
}
static void free_string(BencodeValue* val){
    val -> v_string.length = 0;
    free(val -> v_string.data);
}
static void free_list(BencodeValue* val){
    for(int i = 0 ; i < val -> v_list.count; i++){
        free_all(val -> v_list.elements[i]);
    }
    free(val -> v_list.elements);
}
static void free_dict(BencodeValue* val){
    for(int i = 0; i < val -> v_dict.count; i++){
        free(val -> v_dict.pair[i].key);
        free_all(val -> v_dict.pair[i].value);
    }
    free(val -> v_dict.pair);
}
//free routes
static const HandlerFree free_table[COUNT] = {
    [Bencode_String] = free_string,
    [Bencode_Integer] = free_int,
    [Bencode_List] = free_list,
    [Bencode_Dict] = free_dict
};

void free_all(BencodeValue* val){
    Bencodetype type = val -> Type;
    if(free_table[type] != NULL){
        free_table[type](val);
        free(val);
    }else{
        fprintf(stderr, "Unavailable type\n");
        exit(1);
    }
}
