#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bencode.h"
#include <ctype.h>

static bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

static BencodeValue* decode_string(const char** bencoded_value) {
    char* endptr;
    int length = (int)strtoll(*bencoded_value, &endptr, 10);

    if (*endptr == ':' && length >= 0) {
        const char* start = endptr + 1;
        if((int)strlen(start) <  length){
            fprintf(stderr, "Invalid encoded value: %s\n", *bencoded_value);
            exit(1);
        }
        BencodeValue* dec_str = malloc(sizeof(BencodeValue));
        dec_str -> v_string.data = (char*)malloc(length + 1);
        strncpy(dec_str -> v_string.data, start, length);
        dec_str -> v_string.data[length] = '\0';
        dec_str -> v_string.length = length;
        dec_str -> Type = Bencode_String;
        *bencoded_value = start+ length;
        return dec_str;
    } else {
        fprintf(stderr, "Invalid encoded value: %s\n", *bencoded_value);
        exit(1);
    }
}

static BencodeValue* decode_dict(const char** val){
    (*val)++;
    BencodeValue* dec = malloc(sizeof(BencodeValue));
    dec -> Type = Bencode_Dict;
    dec -> v_dict.count = 0;
    dec -> v_dict.pair = NULL;
    while(**val != 'e' && **val != '\0'){
        BencodeValue* key = decode_parser(val);
        if(key->Type != Bencode_String){
            fprintf(stderr, "Invalid dict formant\n");
            exit(1);
        }
        dec -> v_dict.count++;
        dec -> v_dict.pair = realloc(dec->v_dict.pair, sizeof(BencodePair)* dec->v_dict.count);
        dec -> v_dict.pair[dec->v_dict.count - 1].key = key->v_string.data;
        dec -> v_dict.pair[dec->v_dict.count - 1].value  = decode_parser(val);
        free(key);
    }
    if(**val != 'e'){
        fprintf(stderr,"Dict must end whith e\n");
        exit(1);
    }
    (*val)++;
    return dec;
}
static BencodeValue* decode_list(const char** val){
    (*val)++;
    BencodeValue* dec = malloc(sizeof(BencodeValue));
    dec -> Type = Bencode_List;
    dec -> v_list.count =0;
    dec -> v_list.elements = NULL;
    while(**val != 'e' && **val != '\0'){
        BencodeValue* element = decode_parser(val);
        dec -> v_list.count++;
        dec -> v_list.elements = realloc(dec->v_list.elements, sizeof(BencodeValue*)*dec->v_list.count);
        dec -> v_list.elements[dec->v_list.count - 1] = element;
    }
    if(**val != 'e'){
        fprintf(stderr,"List must end whith e\n");
        exit(1);
    }
    (*val)++;
    return dec;
}
static BencodeValue* decode_int(const char** val){
    if((*val)[0] != 'i'){
        fprintf(stderr, "Unsupported int fromat\n");
        exit(1);
    }
    (*val)++;
    char* endptr;
    long long num = strtoll(*val, &endptr, 10);
    if(*endptr != 'e'){
        fprintf(stderr, "Unsupported int value fromat\n");
        exit(1);
    }
    *val = endptr + 1;
    BencodeValue* res = malloc(sizeof(BencodeValue));
    res -> v_integer = num;
    res -> Type = Bencode_Integer;
    return res;
}

//parser
BencodeValue* decode_parser(const char** val){
    BencodeValue* dec;
    if(*(val[0]) == 'i'){
        dec = decode_int(val);
        return dec;
    }
    else if(*(val[0]) == 'l'){
        dec = decode_list(val);
        return dec;
    }
    else if(*(val[0]) == 'd'){
        dec = decode_dict(val);
        return dec;
    }
    else if(is_digit(*(val[0]))){
       dec = decode_string(val);
       return dec;
    }
    fprintf(stderr, "Unkown type\n");
    exit(1);
}
