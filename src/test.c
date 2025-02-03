/*************************************************************************
 * This is a simple program to calculate test vectors and compare them   *
 * to known good values for XChaCha20.
 *************************************************************************/
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ychacha.h"


/** Calculate and compare the newest test vectors from the IETF
 * Draft for XchaCha20. The test vectors were taken from
 * version 03 of the draft:
 * https://tools.ietf.org/html/draft-arciszewski-xchacha-03
 * This version was published on: October 11, 2018 and
 * expired on: June 21, 2019
 * The biggest difference between this IETF test vector and the
 * other one is that this version initializes the XChaCha20 internal
 * counter to 1 instead of 0.
 * @returns 0 on success, -1 on failure or error
 *
 */
int check_second_ietf(void){
    XChaCha_ctx ctx;
    uint8_t *buffer;
    uint8_t counter[8] = {0x1};

    /* Test vectors from IETF XChaCha20 draft 03 */
    uint8_t plaintext[] = {
        0x54, 0x68, 0x65, 0x20, 0x64, 0x68, 0x6f, 0x6c,
        0x65, 0x20, 0x28, 0x70, 0x72, 0x6f, 0x6e, 0x6f,
        0x75, 0x6e, 0x63, 0x65, 0x64, 0x20, 0x22, 0x64,
        0x6f, 0x6c, 0x65, 0x22, 0x29, 0x20, 0x69, 0x73,
        0x20, 0x61, 0x6c, 0x73, 0x6f, 0x20, 0x6b, 0x6e,
        0x6f, 0x77, 0x6e, 0x20, 0x61, 0x73, 0x20, 0x74,
        0x68, 0x65, 0x20, 0x41, 0x73, 0x69, 0x61, 0x74,
        0x69, 0x63, 0x20, 0x77, 0x69, 0x6c, 0x64, 0x20,
        0x64, 0x6f, 0x67, 0x2c, 0x20, 0x72, 0x65, 0x64,
        0x20, 0x64, 0x6f, 0x67, 0x2c, 0x20, 0x61, 0x6e,
        0x64, 0x20, 0x77, 0x68, 0x69, 0x73, 0x74, 0x6c,
        0x69, 0x6e, 0x67, 0x20, 0x64, 0x6f, 0x67, 0x2e,
        0x20, 0x49, 0x74, 0x20, 0x69, 0x73, 0x20, 0x61,
        0x62, 0x6f, 0x75, 0x74, 0x20, 0x74, 0x68, 0x65,
        0x20, 0x73, 0x69, 0x7a, 0x65, 0x20, 0x6f, 0x66,
        0x20, 0x61, 0x20, 0x47, 0x65, 0x72, 0x6d, 0x61,
        0x6e, 0x20, 0x73, 0x68, 0x65, 0x70, 0x68, 0x65,
        0x72, 0x64, 0x20, 0x62, 0x75, 0x74, 0x20, 0x6c,
        0x6f, 0x6f, 0x6b, 0x73, 0x20, 0x6d, 0x6f, 0x72,
        0x65, 0x20, 0x6c, 0x69, 0x6b, 0x65, 0x20, 0x61,
        0x20, 0x6c, 0x6f, 0x6e, 0x67, 0x2d, 0x6c, 0x65,
        0x67, 0x67, 0x65, 0x64, 0x20, 0x66, 0x6f, 0x78,
        0x2e, 0x20, 0x54, 0x68, 0x69, 0x73, 0x20, 0x68,
        0x69, 0x67, 0x68, 0x6c, 0x79, 0x20, 0x65, 0x6c,
        0x75, 0x73, 0x69, 0x76, 0x65, 0x20, 0x61, 0x6e,
        0x64, 0x20, 0x73, 0x6b, 0x69, 0x6c, 0x6c, 0x65,
        0x64, 0x20, 0x6a, 0x75, 0x6d, 0x70, 0x65, 0x72,
        0x20, 0x69, 0x73, 0x20, 0x63, 0x6c, 0x61, 0x73,
        0x73, 0x69, 0x66, 0x69, 0x65, 0x64, 0x20, 0x77,
        0x69, 0x74, 0x68, 0x20, 0x77, 0x6f, 0x6c, 0x76,
        0x65, 0x73, 0x2c, 0x20, 0x63, 0x6f, 0x79, 0x6f,
        0x74, 0x65, 0x73, 0x2c, 0x20, 0x6a, 0x61, 0x63,
        0x6b, 0x61, 0x6c, 0x73, 0x2c, 0x20, 0x61, 0x6e,
        0x64, 0x20, 0x66, 0x6f, 0x78, 0x65, 0x73, 0x20,
        0x69, 0x6e, 0x20, 0x74, 0x68, 0x65, 0x20, 0x74,
        0x61, 0x78, 0x6f, 0x6e, 0x6f, 0x6d, 0x69, 0x63,
        0x20, 0x66, 0x61, 0x6d, 0x69, 0x6c, 0x79, 0x20,
        0x43, 0x61, 0x6e, 0x69, 0x64, 0x61, 0x65, 0x2e
    };

    uint8_t key[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
        0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
        0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
        0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f
    };

    uint8_t iv[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
        0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x58,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    uint8_t correct_ciphertext[] = {
        0x7d, 0x0a, 0x2e, 0x6b, 0x7f, 0x7c, 0x65, 0xa2,
        0x36, 0x54, 0x26, 0x30, 0x29, 0x4e, 0x06, 0x3b,
        0x7a, 0xb9, 0xb5, 0x55, 0xa5, 0xd5, 0x14, 0x9a,
        0xa2, 0x1e, 0x4a, 0xe1, 0xe4, 0xfb, 0xce, 0x87,
        0xec, 0xc8, 0xe0, 0x8a, 0x8b, 0x5e, 0x35, 0x0a,
        0xbe, 0x62, 0x2b, 0x2f, 0xfa, 0x61, 0x7b, 0x20,
        0x2c, 0xfa, 0xd7, 0x20, 0x32, 0xa3, 0x03, 0x7e,
        0x76, 0xff, 0xdc, 0xdc, 0x43, 0x76, 0xee, 0x05,
        0x3a, 0x19, 0x0d, 0x7e, 0x46, 0xca, 0x1d, 0xe0,
        0x41, 0x44, 0x85, 0x03, 0x81, 0xb9, 0xcb, 0x29,
        0xf0, 0x51, 0x91, 0x53, 0x86, 0xb8, 0xa7, 0x10,
        0xb8, 0xac, 0x4d, 0x02, 0x7b, 0x8b, 0x05, 0x0f,
        0x7c, 0xba, 0x58, 0x54, 0xe0, 0x28, 0xd5, 0x64,
        0xe4, 0x53, 0xb8, 0xa9, 0x68, 0x82, 0x41, 0x73,
        0xfc, 0x16, 0x48, 0x8b, 0x89, 0x70, 0xca, 0xc8,
        0x28, 0xf1, 0x1a, 0xe5, 0x3c, 0xab, 0xd2, 0x01,
        0x12, 0xf8, 0x71, 0x07, 0xdf, 0x24, 0xee, 0x61,
        0x83, 0xd2, 0x27, 0x4f, 0xe4, 0xc8, 0xb1, 0x48,
        0x55, 0x34, 0xef, 0x2c, 0x5f, 0xbc, 0x1e, 0xc2,
        0x4b, 0xfc, 0x36, 0x63, 0xef, 0xaa, 0x08, 0xbc,
        0x04, 0x7d, 0x29, 0xd2, 0x50, 0x43, 0x53, 0x2d,
        0xb8, 0x39, 0x1a, 0x8a, 0x3d, 0x77, 0x6b, 0xf4,
        0x37, 0x2a, 0x69, 0x55, 0x82, 0x7c, 0xcb, 0x0c,
        0xdd, 0x4a, 0xf4, 0x03, 0xa7, 0xce, 0x4c, 0x63,
        0xd5, 0x95, 0xc7, 0x5a, 0x43, 0xe0, 0x45, 0xf0,
        0xcc, 0xe1, 0xf2, 0x9c, 0x8b, 0x93, 0xbd, 0x65,
        0xaf, 0xc5, 0x97, 0x49, 0x22, 0xf2, 0x14, 0xa4,
        0x0b, 0x7c, 0x40, 0x2c, 0xdb, 0x91, 0xae, 0x73,
        0xc0, 0xb6, 0x36, 0x15, 0xcd, 0xad, 0x04, 0x80,
        0x68, 0x0f, 0x16, 0x51, 0x5a, 0x7a, 0xce, 0x9d,
        0x39, 0x23, 0x64, 0x64, 0x32, 0x8a, 0x37, 0x74,
        0x3f, 0xfc, 0x28, 0xf4, 0xdd, 0xb3, 0x24, 0xf4,
        0xd0, 0xf5, 0xbb, 0xdc, 0x27, 0x0c, 0x65, 0xb1,
        0x74, 0x9a, 0x6e, 0xff, 0xf1, 0xfb, 0xaa, 0x09,
        0x53, 0x61, 0x75, 0xcc, 0xd2, 0x9f, 0xb9, 0xe6,
        0x05, 0x7b, 0x30, 0x73, 0x20, 0xd3, 0x16, 0x83,
        0x8a, 0x9c, 0x71, 0xf7, 0x0b, 0x5b, 0x59, 0x07,
        0xa6, 0x6f, 0x7e, 0xa4, 0x9a, 0xad, 0xc4, 0x09
    };

    /* Allocate a buffer to hold our calculated ciphertext */
    if((buffer = malloc(1024 * sizeof(uint8_t))) == NULL){
        perror("malloc() error");
        return(-1);
    }

    xchacha_keysetup(&ctx, key, iv);

    /*This version of the IETF draft initializes their counter to 1
    * instead of 0 */
    xchacha_set_counter(&ctx, counter);
    xchacha_encrypt_bytes(&ctx, plaintext, buffer, 304);

    /* Make sure our ciphertext matches */
    if(memcmp(buffer, correct_ciphertext, 304) != 0){
        free(buffer);
        return(-1);
    }

    free(buffer);

    return(0);
}

/** Calculate and compare the test vectors from the IETF
 * Draft for XchaCha20. The test vectors were taken from
 * this version of the draft:
 * https://tools.ietf.org/html/draft-arciszewski-xchacha-02
 * This version was published on: October 11, 2018 and
 * expired on: April 14, 2019
 * @returns 0 on success, -1 on failure or error
 *
 */
int check_ietf(void){
    XChaCha_ctx ctx;
    uint8_t *buffer;

    /* Test vectors from IETF XChaCha20 draft */
    uint8_t plaintext[] = {
        0x54, 0x68, 0x65, 0x20, 0x64, 0x68, 0x6f, 0x6c,
        0x65, 0x20, 0x28, 0x70, 0x72, 0x6f, 0x6e, 0x6f,
        0x75, 0x6e, 0x63, 0x65, 0x64, 0x20, 0x22, 0x64,
        0x6f, 0x6c, 0x65, 0x22, 0x29, 0x20, 0x69, 0x73,
        0x20, 0x61, 0x6c, 0x73, 0x6f, 0x20, 0x6b, 0x6e,
        0x6f, 0x77, 0x6e, 0x20, 0x61, 0x73, 0x20, 0x74,
        0x68, 0x65, 0x20, 0x41, 0x73, 0x69, 0x61, 0x74,
        0x69, 0x63, 0x20, 0x77, 0x69, 0x6c, 0x64, 0x20,
        0x64, 0x6f, 0x67, 0x2c, 0x20, 0x72, 0x65, 0x64,
        0x20, 0x64, 0x6f, 0x67, 0x2c, 0x20, 0x61, 0x6e,
        0x64, 0x20, 0x77, 0x68, 0x69, 0x73, 0x74, 0x6c,
        0x69, 0x6e, 0x67, 0x20, 0x64, 0x6f, 0x67, 0x2e,
        0x20, 0x49, 0x74, 0x20, 0x69, 0x73, 0x20, 0x61,
        0x62, 0x6f, 0x75, 0x74, 0x20, 0x74, 0x68, 0x65,
        0x20, 0x73, 0x69, 0x7a, 0x65, 0x20, 0x6f, 0x66,
        0x20, 0x61, 0x20, 0x47, 0x65, 0x72, 0x6d, 0x61,
        0x6e, 0x20, 0x73, 0x68, 0x65, 0x70, 0x68, 0x65,
        0x72, 0x64, 0x20, 0x62, 0x75, 0x74, 0x20, 0x6c,
        0x6f, 0x6f, 0x6b, 0x73, 0x20, 0x6d, 0x6f, 0x72,
        0x65, 0x20, 0x6c, 0x69, 0x6b, 0x65, 0x20, 0x61,
        0x20, 0x6c, 0x6f, 0x6e, 0x67, 0x2d, 0x6c, 0x65,
        0x67, 0x67, 0x65, 0x64, 0x20, 0x66, 0x6f, 0x78,
        0x2e, 0x20, 0x54, 0x68, 0x69, 0x73, 0x20, 0x68,
        0x69, 0x67, 0x68, 0x6c, 0x79, 0x20, 0x65, 0x6c,
        0x75, 0x73, 0x69, 0x76, 0x65, 0x20, 0x61, 0x6e,
        0x64, 0x20, 0x73, 0x6b, 0x69, 0x6c, 0x6c, 0x65,
        0x64, 0x20, 0x6a, 0x75, 0x6d, 0x70, 0x65, 0x72,
        0x20, 0x69, 0x73, 0x20, 0x63, 0x6c, 0x61, 0x73,
        0x73, 0x69, 0x66, 0x69, 0x65, 0x64, 0x20, 0x77,
        0x69, 0x74, 0x68, 0x20, 0x77, 0x6f, 0x6c, 0x76,
        0x65, 0x73, 0x2c, 0x20, 0x63, 0x6f, 0x79, 0x6f,
        0x74, 0x65, 0x73, 0x2c, 0x20, 0x6a, 0x61, 0x63,
        0x6b, 0x61, 0x6c, 0x73, 0x2c, 0x20, 0x61, 0x6e,
        0x64, 0x20, 0x66, 0x6f, 0x78, 0x65, 0x73, 0x20,
        0x69, 0x6e, 0x20, 0x74, 0x68, 0x65, 0x20, 0x74,
        0x61, 0x78, 0x6f, 0x6e, 0x6f, 0x6d, 0x69, 0x63,
        0x20, 0x66, 0x61, 0x6d, 0x69, 0x6c, 0x79, 0x20,
        0x43, 0x61, 0x6e, 0x69, 0x64, 0x61, 0x65, 0x2e
    };

    uint8_t key[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
        0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
        0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
        0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f
    };

    uint8_t iv[] = {
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
        0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x58,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    uint8_t correct_ciphertext[] = {
        0x45, 0x59, 0xab, 0xba, 0x4e, 0x48, 0xc1, 0x61,
        0x02, 0xe8, 0xbb, 0x2c, 0x05, 0xe6, 0x94, 0x7f,
        0x50, 0xa7, 0x86, 0xde, 0x16, 0x2f, 0x9b, 0x0b,
        0x7e, 0x59, 0x2a, 0x9b, 0x53, 0xd0, 0xd4, 0xe9,
        0x8d, 0x8d, 0x64, 0x10, 0xd5, 0x40, 0xa1, 0xa6,
        0x37, 0x5b, 0x26, 0xd8, 0x0d, 0xac, 0xe4, 0xfa,
        0xb5, 0x23, 0x84, 0xc7, 0x31, 0xac, 0xbf, 0x16,
        0xa5, 0x92, 0x3c, 0x0c, 0x48, 0xd3, 0x57, 0x5d,
        0x4d, 0x0d, 0x2c, 0x67, 0x3b, 0x66, 0x6f, 0xaa,
        0x73, 0x10, 0x61, 0x27, 0x77, 0x01, 0x09, 0x3a,
        0x6b, 0xf7, 0xa1, 0x58, 0xa8, 0x86, 0x42, 0x92,
        0xa4, 0x1c, 0x48, 0xe3, 0xa9, 0xb4, 0xc0, 0xda,
        0xec, 0xe0, 0xf8, 0xd9, 0x8d, 0x0d, 0x7e, 0x05,
        0xb3, 0x7a, 0x30, 0x7b, 0xbb, 0x66, 0x33, 0x31,
        0x64, 0xec, 0x9e, 0x1b, 0x24, 0xea, 0x0d, 0x6c,
        0x3f, 0xfd, 0xdc, 0xec, 0x4f, 0x68, 0xe7, 0x44,
        0x30, 0x56, 0x19, 0x3a, 0x03, 0xc8, 0x10, 0xe1,
        0x13, 0x44, 0xca, 0x06, 0xd8, 0xed, 0x8a, 0x2b,
        0xfb, 0x1e, 0x8d, 0x48, 0xcf, 0xa6, 0xbc, 0x0e,
        0xb4, 0xe2, 0x46, 0x4b, 0x74, 0x81, 0x42, 0x40,
        0x7c, 0x9f, 0x43, 0x1a, 0xee, 0x76, 0x99, 0x60,
        0xe1, 0x5b, 0xa8, 0xb9, 0x68, 0x90, 0x46, 0x6e,
        0xf2, 0x45, 0x75, 0x99, 0x85, 0x23, 0x85, 0xc6,
        0x61, 0xf7, 0x52, 0xce, 0x20, 0xf9, 0xda, 0x0c,
        0x09, 0xab, 0x6b, 0x19, 0xdf, 0x74, 0xe7, 0x6a,
        0x95, 0x96, 0x74, 0x46, 0xf8, 0xd0, 0xfd, 0x41,
        0x5e, 0x7b, 0xee, 0x2a, 0x12, 0xa1, 0x14, 0xc2,
        0x0e, 0xb5, 0x29, 0x2a, 0xe7, 0xa3, 0x49, 0xae,
        0x57, 0x78, 0x20, 0xd5, 0x52, 0x0a, 0x1f, 0x3f,
        0xb6, 0x2a, 0x17, 0xce, 0x6a, 0x7e, 0x68, 0xfa,
        0x7c, 0x79, 0x11, 0x1d, 0x88, 0x60, 0x92, 0x0b,
        0xc0, 0x48, 0xef, 0x43, 0xfe, 0x84, 0x48, 0x6c,
        0xcb, 0x87, 0xc2, 0x5f, 0x0a, 0xe0, 0x45, 0xf0,
        0xcc, 0xe1, 0xe7, 0x98, 0x9a, 0x9a, 0xa2, 0x20,
        0xa2, 0x8b, 0xdd, 0x48, 0x27, 0xe7, 0x51, 0xa2,
        0x4a, 0x6d, 0x5c, 0x62, 0xd7, 0x90, 0xa6, 0x63,
        0x93, 0xb9, 0x31, 0x11, 0xc1, 0xa5, 0x5d, 0xd7,
        0x42, 0x1a, 0x10, 0x18, 0x49, 0x74, 0xc7, 0xc5
    };

    /* Allocate a buffer to hold our calculated ciphertext */
    if((buffer = malloc(1024 * sizeof(uint8_t))) == NULL){
        perror("malloc() error");
        return(-1);
    }

    xchacha_keysetup(&ctx, key, iv);
    xchacha_encrypt_bytes(&ctx, plaintext, buffer, 304);

    /* Make sure our ciphertext matches */
    if(memcmp(buffer, correct_ciphertext, 304) != 0){
        free(buffer);
        return(-1);
    }

    free(buffer);

    return(0);
}


/** Compare our output to the output of a known good XChaCha20 library.
 * The test vectors used here are from examples given of the Crypto++
 * cryptographic library's XChaCha20 examples. These values can be
 * found here:
 * https://www.cryptopp.com/wiki/XChaCha20
 * @returns 0 on success, -1 on failure or error
 *
 */
int check_cpp(void){
    XChaCha_ctx ctx;
    uint8_t *buffer;
    uint8_t counter[8] = {0x1};

    /* Test values from Crypto++ documentation */
    uint8_t key[] = {
        0x5E, 0xC5, 0x8B, 0x6D, 0x51, 0x4F, 0xE0, 0xA5,
        0x6F, 0x1E, 0x0D, 0xEA, 0x7B, 0xDC, 0x09, 0x5A,
        0x10, 0xF5, 0xB6, 0x18, 0xBD, 0xB6, 0xF2, 0x26,
        0x2F, 0xCC, 0x59, 0x7B, 0xB2, 0x30, 0xB3, 0xEF
    };

    uint8_t iv[] = {
        0xA3, 0x45, 0xF5, 0xCF, 0x80, 0x23, 0x51, 0x7C,
        0xC0, 0xFC, 0xF0, 0x75, 0x74, 0x8C, 0x86, 0x5F,
        0x7D, 0xE8, 0xCA, 0x0C, 0x72, 0x36, 0xAB, 0xDA,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    uint8_t correct_ciphertext[] = {
        0xEE, 0xA7, 0xC2, 0x71, 0x19, 0x10, 0x65, 0x69,
        0x92, 0xE1, 0xCE, 0xD8, 0x16, 0xE2, 0x0E, 0x62,
        0x1B, 0x25, 0x17, 0x82, 0x36, 0x71, 0x6A, 0xE4,
        0x99, 0xF2, 0x97, 0x37, 0xA7, 0x2A, 0xFC, 0xF8,
        0x6C, 0x72
    };

    uint8_t plaintext[] = "My Plaintext!! My Dear plaintext!!";
    uint32_t msglen = strlen((char *)plaintext);

    /* Allocate a buffer to hold our calculated ciphertext */
    if((buffer = malloc(50 * sizeof(uint8_t))) == NULL){
        perror("malloc() error");
        return(-1);
    }

    xchacha_keysetup(&ctx, key, iv);

    /* Crypto++ initializes their counter to 1 instead of 0 */
    xchacha_set_counter(&ctx, counter);
    xchacha_encrypt_bytes(&ctx, plaintext, buffer, msglen);

    /* Compare our ciphertext to the correct ciphertext */
    if(memcmp(buffer, correct_ciphertext, msglen) != 0){
        free(buffer);
        return(-1);
    }

    free(buffer);
    return(0);
}

int main(void){
    if((check_ietf()) == 0 && (check_cpp()) == 0 && (check_second_ietf() == 0)){
        printf("Cryptographic tests passed\n");
    } else {
        printf("Cryptographic tests failed!\n");
    }

    return(0);
}
