#ifndef TUPLE_SPACE_H
#define TUPLE_SPACE_H
#define TS_YES 1
#define TS_NO 0
#define TS_INT 0
#define TS_FLOAT 1
#define TS_SUCCESS 1
#define TS_FAILURE 0
typedef struct {
    int is_actual; /* does the data member contains data */
        int type; /* what is the type of the data member */
        union {
            int int_field;
            float float_field;
        } data;
    } field_t; /* a new type corresponding to one field of a tuple */
/* API */
/* these functions return TS_SUCCESS or TS_FAILURE */
/* parameters: tuple name, other fields, no. of other fields */
int ts_out(char*, field_t*, int);
int ts_inp(char*, field_t*, int);
int ts_rdp(char*, field_t*, int);
#endif