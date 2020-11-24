#include "init.h"
#include "cx.h"

// Init public keys
void init_application_context(swap_app_context_t *ctx) {
#if defined(TEST_PUBLIC_KEY)
    // this key is the ERC20signer test key
    unsigned char LedgerPubKey[] = {0x4,                                             //
                                    0x20, 0xDA, 0x62, 0x00, 0x3C, 0x0C, 0xE0, 0x97,  //
                                    0xE3, 0x36, 0x44, 0xA1, 0x0F, 0xE4, 0xC3, 0x04,  //
                                    0x54, 0x06, 0x9A, 0x44, 0x54, 0xF0, 0xFA, 0x9D,  //
                                    0x4E, 0x84, 0xF4, 0x50, 0x91, 0x42, 0x9B, 0x52,  //

                                    0x20, 0xAF, 0x9E, 0x35, 0xC0, 0xB2, 0xD9, 0x28,  //
                                    0x93, 0x80, 0x13, 0x73, 0x07, 0xDE, 0x4D, 0xD1,  //
                                    0xD4, 0x18, 0x42, 0x8C, 0xF2, 0x1A, 0x93, 0xB3,  //
                                    0x35, 0x61, 0xBB, 0x09, 0xD8, 0x8F, 0xE5, 0x79};
#elif defined(OLD_TEST_PUBLIC_KEY)
    // this key was created from private key sha256('Ledger'), see test/tools folder
    unsigned char LedgerPubKey[] = {0x4,                                             //
                                    0x5,  0xC5, 0x2E, 0xC5, 0xFE, 0x24, 0x5A, 0x55,  //
                                    0x7B, 0x86, 0x1D, 0x22, 0x18, 0x50, 0x1A, 0x81,  //
                                    0x2D, 0x32, 0xE0, 0x34, 0xE1, 0x5E, 0x9D, 0x96,  //
                                    0x1C, 0x1B, 0x1A, 0x13, 0x8C, 0x7F, 0xB1, 0x49,  //

                                    0x6B, 0x4F, 0xBA, 0x66, 0x65, 0x56, 0x66, 0x62,  //
                                    0x3E, 0xB7, 0x8C, 0x93, 0xE9, 0xF0, 0x0,  0x8F,  //
                                    0xCC, 0xA6, 0xA,  0x53, 0x85, 0x88, 0x13, 0x1A,  //
                                    0x2A, 0xC7, 0xBA, 0x98, 0xE1, 0xF6, 0x20, 0xCE};
#else
    // this key is the ExchangeConfigSigner key
    unsigned char LedgerPubKey[] = {0x04,                                            //
                                    0x0c, 0x17, 0xfd, 0xc5, 0xc0, 0x62, 0x9e, 0xca,  //
                                    0x77, 0xe0, 0xa7, 0x10, 0xcd, 0xe9, 0x66, 0xba,  //
                                    0xe1, 0xa3, 0x2c, 0x84, 0xe3, 0x83, 0xe3, 0x28,  //
                                    0xa5, 0x52, 0x1c, 0x28, 0x1e, 0xfb, 0xa4, 0x99,  //

                                    0x07, 0x44, 0xea, 0xbc, 0xb5, 0x91, 0xb0, 0xd8,  //
                                    0xbe, 0x59, 0x8a, 0x2f, 0xfa, 0xca, 0x8d, 0x29,  //
                                    0xbc, 0x79, 0xfb, 0x20, 0x68, 0x6c, 0xc7, 0xc0,  //
                                    0x06, 0x0c, 0x53, 0x86, 0x2c, 0x5c, 0xe3, 0x8c};

#endif
    os_memset(ctx, 0, sizeof(*ctx));
    cx_ecfp_init_public_key(CX_CURVE_SECP256K1,
                            LedgerPubKey,
                            sizeof(LedgerPubKey),
                            &(ctx->ledger_public_key));
    ctx->state = INITIAL_STATE;
}
