#ifdef __cplusplus
extern "C" {
#endif

#ifndef TUPLE_PROTOCOL_H
#define TUPLE_PROTOCOL_H
#include <stdint.h>

#define TS_YES      1
#define TS_NO       0
#define TS_INT      0
#define TS_FLOAT    1
#define TS_SUCCESS  1
#define TS_FAILURE  0

#define TS_CMD_OUT  0
#define TS_CMD_IN   1
#define TS_CMD_RD   2

typedef struct {
    int is_actual;  /* does the data member contains data */
    int type;       /* what is the type of the data member */
    union {
        int int_field;
        float float_field;
        char* string_field;
    } data;
} field_t;          /* a new type corresponding to one field of a tuple*/

/* API */
/* these functions return TS_SUCCESS or TS_FAILURE */
/* parameters : tuple name , other fields , no. of other fields */

// this function should adds a tuple with specified fields to the tuple space
int ts_out (char*, field_t *, int);

// this function should retrieve a tuple matching the specified fields from the tuple space and delete it
int ts_inp (char*, field_t *, int);

// this function should retrieve a tuple matching the specified fields from the tuple space and without deleting it
int ts_rdp (char*, field_t *, int);

#define COMMAND_TYPE_MASK   0x03
#define COMMAND_TYPE_POS    (sizeof(char) * 8 - 2)
#define NUM_FIELDS_POS      (sizeof(char) * 8 - 3)
#define FIELD_1_ACTUAL_POS  (sizeof(char) * 8 - 4)
#define FIELD_1_TYPE_POS    (sizeof(char) * 8 - 5)
#define FIELD_2_ACTUAL_POS  (sizeof(char) * 8 - 6)
#define FIELD_2_TYPE_POS    (sizeof(char) * 8 - 7)

int serializePacket(char* packet, int command, char* tuple_name, field_t* fields, int num_fields);

int deserializePacket(char* packet, int* command, char* tuple_name, field_t* fields, int* num_fields);

int intToBytes(uint32_t number, int index);
int floatToBytes(float number, int index);
int getBit(unsigned char byte, int position);

void displayProtocolBytes(unsigned char *packet, int total_packet_size, int tuple_name_len);

int bytesToInt(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4);
float bytesToFloat(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4);


#endif

#ifdef __cplusplus
}
#endif