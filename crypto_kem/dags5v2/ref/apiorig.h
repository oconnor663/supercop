/*********************************************************************************************
* DAGS: Key Encapsulation using Dyadic GS Codes.                              *
* This code is exclusively intended for submission to the NIST Post=Quantum Cryptography.    *
* For any other usage , contact the author(s) to ask permission.                             *
**********************************************************************************************
*/

#define DAGS_5
#define CRYPTO_ALGNAME "DAGS_5"
#define CRYPTO_SECRETKEYBYTES 6400
#define CRYPTO_PUBLICKEYBYTES 630784
#define CRYPTO_CIPHERTEXTBYTES 2144
#define CRYPTO_BYTES 64

int crypto_kem_keypair(
    unsigned char *pk,
    unsigned char *sk);

int crypto_kem_enc(
    unsigned char *ct,
    unsigned char *ss,
    const unsigned char *pk);

int crypto_kem_dec(
    unsigned char *ss,
    const unsigned char *ct,
    const unsigned char *sk);
