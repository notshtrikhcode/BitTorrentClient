#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



typedef enum{
    Bencode_String,
    Bencode_Integer,
    Bencode_List,

    COUNT
}Bencodetype;

struct BencodeValue;

typedef struct BencodeValue{
    Bencodetype Type;
    union{
      //string
      struct{
          char* data;
          int lenght;
      }v_string;
      // int
      long long v_integer;
      // list
      struct{
          struct BencodeValue** elements;
          int count;
      }v_list;
    };
}BencodeValue;

typedef void (*HandlerPrint)(const BencodeValue* val);
typedef void (*HandlerFree)(BencodeValue* val);
BencodeValue* decode_parser(const char** val);
void print_val(const BencodeValue* val);
void free_all(BencodeValue* val);

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

BencodeValue* decode_bencode_new(const char** bencoded_value) {
    // int length = atoi(*bencoded_value);
    char* endptr;
    int length = (int)strtoll(*bencoded_value, &endptr, 10);
    if (*endptr == ':') {
        const char* start = endptr + 1;
        BencodeValue* dec_str = malloc(sizeof(BencodeValue));
        dec_str -> v_string.data = (char*)malloc(length + 1);
        strncpy(dec_str-> v_string.data, start, length);
        dec_str -> v_string.data[length] = '\0';
        dec_str -> v_string.lenght = length;
        dec_str -> Type = Bencode_String;
        *bencoded_value = start+ length;
        return dec_str;
    } else {
        fprintf(stderr, "Invalid encoded value: %s\n", *bencoded_value);
        exit(1);
    }
}


BencodeValue* decode_list(const char** val){
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
BencodeValue* decode_int(const char** val){
    if(*val[0] != 'i'){
        fprintf(stderr, "Unsupported fromat\n");
        exit(1);
    }
    (*val)++;
    char* endptr;
    long long num = strtoll(*val, &endptr, 10);
    if(*endptr != 'e'){
        fprintf(stderr, "Unsupported fromat\n");
        exit(1);
    }
    *val = endptr + 1;
    BencodeValue* res = malloc(sizeof(BencodeValue));
    res -> v_integer = num;
    res -> Type = Bencode_Integer;
    return res;

}
//asdsad
//parser
BencodeValue* decode_parser(const char** val){
    BencodeValue* dec;
    if(*val[0] == 'i'){
        dec = decode_int(val);
        return dec;
    }
    else if(*val[0] == 'l'){
        dec = decode_list(val);
        return dec;
    }
    else if(is_digit(*val[0])){
       dec = decode_bencode_new(val);
       return dec;
    }
    fprintf(stderr, "Unkown type\n");
    exit(1);
}
static void print_decoded_str(const BencodeValue* val){
    printf("\"%s\"\n", val -> v_string.data);
}
static void print_decoded_int(const BencodeValue* val){
    printf("%lld\n", val -> v_integer);
}
static void print_decoded_list(const BencodeValue* val){
    for(int i = 0; i < val -> v_list.count; i++){
        print_val(val->v_list.elements[i]);
    }
}
//routes
static const HandlerPrint print_table[COUNT] = {
    [Bencode_String] = print_decoded_str,
    [Bencode_Integer] = print_decoded_int,
    [Bencode_List] = print_decoded_list
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
static void free_int(BencodeValue* val){
    val -> v_integer = 0;
}
static void free_string(BencodeValue* val){
    val -> v_string.lenght = 0;
    free(val -> v_string.data);
}
static void free_list(BencodeValue* val){
    for(int i =0 ; i < val->v_list.count; i++){
        free_all(val->v_list.elements[i]);
    }
}
static const HandlerFree free_table[COUNT] = {
    [Bencode_String] = free_string,
    [Bencode_Integer] = free_int,
    [Bencode_List] = free_list
};

void free_all(BencodeValue* val){
    Bencodetype type = val->Type;
    if(free_table[type] != NULL){
        free_table[type](val);
    }else{
        fprintf(stderr, "Unavailable type\n");
        exit(1);
    }
}


int main(int argc, char* argv[]) {
	// Disable output buffering
	setbuf(stdout, NULL);
 	setbuf(stderr, NULL);

    if (argc < 3) {
        fprintf(stderr, "Usage: your_program.sh <command> <args>\n");
        return 1;
    }

    const char* command = argv[1];

    if (strcmp(command, "decode") == 0) {
    	// You can use print statements as follows for debugging, they'll be visible when running tests.
        fprintf(stderr, "Logs from your program will appear here!\n");
        // TODO: Uncomment the code below to pass the first stage
         const char* encoded_str = argv[2];
         BencodeValue* dec = NULL;
         dec = decode_parser(&encoded_str);
         print_val(dec);
         free_all(dec);
    } else {
        fprintf(stderr, "Unknown command: %s\n", command);
        return 1;
    }

    return 0;
}





/*---------------------------------------------------------------*/
/*
char* decode_bencode(const char* bencoded_value) {
    if (is_digit(bencoded_value[0])) {
        int length = atoi(bencoded_value);
        const char* colon_index = strchr(bencoded_value, ':');
        if (colon_index != NULL) {
            const char* start = colon_index + 1;
            char* decoded_str = (char*)malloc(length + 1);
            strncpy(decoded_str, start, length);
            decoded_str[length] = '\0';

            return decoded_str;
        } else {
            fprintf(stderr, "Invalid encoded value: %s\n", bencoded_value);
            exit(1);
        }
    } else {
        fprintf(stderr, "Only strings are supported at the moment\n");
        exit(1);
    }
}
char* decode_intbencode(const char* val){
    long length = strlen(val);
    if(val[0] != 'i' || val[length-1] != 'e' || length < 3 ){
        fprintf(stderr, "Unsupported fromat\n");
        exit(1);
    }
    int num_len = length - 2;
    char* res_char = malloc(num_len + 1);

    for(int i = 1; i < length - 1; i++){
        res_char[i-1] = val[i];
    }
    res_char[num_len] = '\0';
    if(!is_valid_value(res_char)){
        fprintf(stderr, "Value is not valid\n");
        exit(1);
    }
    return res_char;
    }*/



/*if(encoded_str[0] == 'i'){
    dec = decode_int(encoded_str);
    printf("%lld\n", dec -> v_integer);
    free(dec);
}
else if(encoded_str[0] == 'l'){

}
else{
   printf("NEW\n");
   dec = decode_bencode_new(encoded_str);
   printf("\"%s\"\n", dec -> v_string.data);
   printf("OLD\n");
   free(dec->v_string.data);
   free(dec);
}*/
