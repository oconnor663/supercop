#include "namespace.h"
/** Public header */
#ifndef __THREE_BEARS_THREEBEARS624R2CPA_H__
#define __THREE_BEARS_THREEBEARS624R2CPA_H__

#include <stdint.h>
#include <stddef.h> /* for size_t */
#include <string.h> /* for memcpy */

#define PQCRYPTO_KEM_THREEBEARS624R2CPA_IMPL_PRIV_KEYGEN_SEED_BYTES 40
#define PQCRYPTO_KEM_THREEBEARS624R2CPA_IMPL_PRIV_PRIVATE_KEY_BYTES PQCRYPTO_KEM_THREEBEARS624R2CPA_IMPL_PRIV_KEYGEN_SEED_BYTES
#define PQCRYPTO_KEM_THREEBEARS624R2CPA_IMPL_PRIV_SHARED_SECRET_BYTES 32
#define PQCRYPTO_KEM_THREEBEARS624R2CPA_IMPL_PRIV_ENC_SEED_AND_IV_BYTES 32
#define PQCRYPTO_KEM_THREEBEARS624R2CPA_IMPL_PRIV_PUBLIC_KEY_BYTES 804
#define PQCRYPTO_KEM_THREEBEARS624R2CPA_IMPL_PRIV_CAPSULE_BYTES 917

/**
 * Expand a secret seed to a public key.
 *
 * @param[out] pk The public key.
 * @param[in] sk The private seed, which must be uniformly random.
 */
void pqcrypto_kem_threebears624r2cpa_impl_priv_get_pubkey (
    uint8_t pk[PQCRYPTO_KEM_THREEBEARS624R2CPA_IMPL_PRIV_PUBLIC_KEY_BYTES],
    const uint8_t seed[PQCRYPTO_KEM_THREEBEARS624R2CPA_IMPL_PRIV_PRIVATE_KEY_BYTES]
);

/**
 * Expand a secret seed to a public/private keypair.
 *
 * @param[out] pk The public key.
 * @param[out] sk The private key.
 * @param[in] seed The private seed, which must be uniformly random.
 */
static inline void pqcrypto_kem_threebears624r2cpa_impl_priv_keygen (
    uint8_t pk[PQCRYPTO_KEM_THREEBEARS624R2CPA_IMPL_PRIV_PUBLIC_KEY_BYTES],
    uint8_t sk[PQCRYPTO_KEM_THREEBEARS624R2CPA_IMPL_PRIV_PRIVATE_KEY_BYTES],
    const uint8_t seed[PQCRYPTO_KEM_THREEBEARS624R2CPA_IMPL_PRIV_KEYGEN_SEED_BYTES]
) {
    pqcrypto_kem_threebears624r2cpa_impl_priv_get_pubkey(pk,seed);
    memcpy(sk,seed,PQCRYPTO_KEM_THREEBEARS624R2CPA_IMPL_PRIV_KEYGEN_SEED_BYTES);
}
    
/**
 * Create a shared secret using a random seed and another party's public key.
 *
 * Input and output parameters may not alias.
 *
 * @param[out] shared_secret The shared secret key.
 * @param[out] capsule A ciphertext to send to the other party.
 * @param[in] pk The other party's public key.
 * @param[in] seed A random seed.
 */
void pqcrypto_kem_threebears624r2cpa_impl_priv_encapsulate (
    uint8_t shared_secret[PQCRYPTO_KEM_THREEBEARS624R2CPA_IMPL_PRIV_SHARED_SECRET_BYTES],
    uint8_t capsule[PQCRYPTO_KEM_THREEBEARS624R2CPA_IMPL_PRIV_CAPSULE_BYTES],
    const uint8_t pk[PQCRYPTO_KEM_THREEBEARS624R2CPA_IMPL_PRIV_PUBLIC_KEY_BYTES],
    const uint8_t seed[PQCRYPTO_KEM_THREEBEARS624R2CPA_IMPL_PRIV_ENC_SEED_AND_IV_BYTES]
);

/**
 * Extract the shared secret from a capsule using the private key.
 * Has a negligible but nonzero probability of failure.
 *
 * Input and output parameters may not alias.
 *
 * @param[out] shared_secret The shared secret.
 * @param[in] capsule The capsule produced by encapsulate_cca2.
 * @param[in] sk The private key.
 */
void pqcrypto_kem_threebears624r2cpa_impl_priv_decapsulate ( 
    uint8_t shared_secret[PQCRYPTO_KEM_THREEBEARS624R2CPA_IMPL_PRIV_SHARED_SECRET_BYTES],
    const uint8_t capsule[PQCRYPTO_KEM_THREEBEARS624R2CPA_IMPL_PRIV_CAPSULE_BYTES],
    const uint8_t sk[PQCRYPTO_KEM_THREEBEARS624R2CPA_IMPL_PRIV_PRIVATE_KEY_BYTES]
);

void secure_bzero (void *s,size_t size);

#endif /*__THREE_BEARS_THREEBEARS624R2CPA_H__*/

