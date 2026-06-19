#include <stdio.h>
#include <stdlib.h>
#include "bencode.h"


typedef void (*HandlerPrint)(const BencodeValue* val);

static void print_decoded_str(const BencodeValue* val){
    printf("\"%s\"\n", val -> v_string.data);
}
static void print_decoded_int(const BencodeValue* val){
    printf("%lld\n", val -> v_integer);
}
static void print_decoded_list(const BencodeValue* val){
    for(int i = 0; i < val -> v_list.count; i++){
        print_val(val -> v_list.elements[i]);
    }
}
static void print_decoded_dict(const BencodeValue* val){
    for(int i = 0; i < val -> v_dict.count; i++){
        printf("%s: ", val-> v_dict.pair -> key);
        print_val(val -> v_dict.pair[i].value);
    }
}
// print routes
static const HandlerPrint print_table[COUNT] = {
    [Bencode_String] = print_decoded_str,
    [Bencode_Integer] = print_decoded_int,
    [Bencode_List] = print_decoded_list,
    [Bencode_Dict] = print_decoded_dict
};

void print_val(const BencodeValue *val){
    Bencodetype type = val->Type;
    if(print_table[type] != NULL){
        print_table[type](val);
    }
    else{
        fprintf(stderr, "Unavailable type\n");
        exit(1);
    }
}
