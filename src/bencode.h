#ifndef BENCODE_H
#define BENCODE_H

typedef enum{
    Bencode_String,
    Bencode_Integer,
    Bencode_List,
    Bencode_Dict,

    COUNT
}Bencodetype;

struct BencodeValue;

typedef struct BencodePair{
    char* key;
    struct BencodeValue* value;
}BencodePair;

typedef struct BencodeValue{
    Bencodetype Type;
    union{
      //string
      struct{
          char* data;
          int length;
      }v_string;
      // int
      long long v_integer;
      // list
      struct{
          struct BencodeValue** elements;
          int count;
      }v_list;
      //dict
      struct{
          BencodePair* pair;
          int count;
      }v_dict;
    };
}BencodeValue;

BencodeValue* decode_parser(const char** val);
void print_val(const BencodeValue* val);
void free_all(BencodeValue* val);
#endif
