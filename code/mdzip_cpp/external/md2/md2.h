/*********************************************************************
* Filename:   md2.h
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Defines the API for the corresponding MD2 implementation.
*********************************************************************/

#ifndef MD2_H
#define MD2_H

/*************************** HEADER FILES ***************************/
#include <stddef.h>

/****************************** MACROS ******************************/
#define MD2_BLOCK_SIZE 16

/**************************** DATA TYPES ****************************/
/* MD2 context */
typedef struct {
   uint8_t data[16];
   uint8_t state[48];
   uint8_t checksum[16];
   uint32_t len;
} MD2_CTX;


/*********************** FUNCTION DECLARATIONS **********************/
static void md2_transform(MD2_CTX *ctx, uint8_t data[]);
void md2_init(MD2_CTX *ctx);
void md2_update(MD2_CTX *ctx, const void *in, size_t inlen);
void md2_final(MD2_CTX *ctx, void *out);
void md2(const void *in, size_t inlen, void *out);

#endif   // MD2_H
