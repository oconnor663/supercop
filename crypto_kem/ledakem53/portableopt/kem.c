/**
 *
 * <kem.c>
 *
 * @version 2.0 (March 2019)
 *
 * Reference ISO-C11 Implementation of LEDAcrypt using GCC built-ins.
 *
 * In alphabetical order:
 *
 * @author Marco Baldi <m.baldi@univpm.it>
 * @author Alessandro Barenghi <alessandro.barenghi@polimi.it>
 * @author Franco Chiaraluce <f.chiaraluce@univpm.it>
 * @author Gerardo Pelosi <gerardo.pelosi@polimi.it>
 * @author Paolo Santini <p.santini@pm.univpm.it>
 *
 * This code is hereby placed in the public domain.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **/

#include "niederreiter_keygen.h"
#include "niederreiter_encrypt.h"
#include "niederreiter_decrypt.h"
#include "rng.h"
#include "crypto_kem.h"
#include "sha3.h"
#include <string.h>

/*----------------------------------------------------------------------------*/
/* Generates a keypair - pk is the public key and 
 *                       sk is the secret key. 
 */
int crypto_kem_keypair( unsigned char *pk,
                        unsigned char *sk )
{

   key_gen_niederreiter((publicKeyNiederreiter_t *) pk,
                        (privateKeyNiederreiter_t *) sk);
   return 0;
} // end crypto_kem_keypair

/*----------------------------------------------------------------------------*/
/* Encrypt - pk is the public key, 
 *           ct is a key encapsulation message (ciphertext), 
 *           ss is the shared secret.
 */
int crypto_kem_enc( unsigned char *ct,
                    unsigned char *ss,
                    const unsigned char *pk )
{
    
   encrypt_niederreiter_indcpa((unsigned char * const) ct,
                               (unsigned char * const) ss,
                               (const publicKeyNiederreiter_t *const) pk); 
   
   return 0; // NIST convention: 0 == no encryption failure
} // end crypto_kem_enc

/*----------------------------------------------------------------------------*/
/* Decrypt - ct is a key encapsulation message (ciphertext), 
 *           sk is the private key, 
 *           ss is the shared secret 
 */
int crypto_kem_dec( unsigned char *ss,
                    const unsigned char *ct,
                    const unsigned char *sk )
{
   int decryption_ok;
   decryption_ok = (1 == decrypt_niederreiter_indcpa((unsigned char *const) ss, 
                                               (const unsigned char *const) ct, 
                                  (const privateKeyNiederreiter_t * const) sk)); 
   if (decryption_ok == 1) {
      return 0; // NIST convention: 0 == no decryption failure
   }
   else 
      return -1; // NIST convention: -1 == decryption failure
} // end crypto_kem_dec
