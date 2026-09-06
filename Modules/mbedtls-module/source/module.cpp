// SPDX-License-Identifier: Apache-2.0
#include <mbedtls/module.h>

#include <psa/crypto.h>
#include <mbedtls/error.h>

extern "C" {

static const ModuleSymbol SYMBOLS[] = {
    // PSA crypto (Mbed TLS 3.x / ESP-IDF 6.x) - the API the App Hub SDK links against.
    DEFINE_MODULE_SYMBOL(psa_crypto_init),
    DEFINE_MODULE_SYMBOL(psa_generate_random),
    DEFINE_MODULE_SYMBOL(psa_import_key),
    DEFINE_MODULE_SYMBOL(psa_destroy_key),
    DEFINE_MODULE_SYMBOL(psa_hash_compute),
    DEFINE_MODULE_SYMBOL(psa_cipher_encrypt),
    DEFINE_MODULE_SYMBOL(psa_cipher_decrypt),
    DEFINE_MODULE_SYMBOL(psa_cipher_encrypt_setup),
    DEFINE_MODULE_SYMBOL(psa_cipher_decrypt_setup),
    DEFINE_MODULE_SYMBOL(psa_cipher_set_iv),
    DEFINE_MODULE_SYMBOL(psa_cipher_update),
    DEFINE_MODULE_SYMBOL(psa_cipher_finish),
    DEFINE_MODULE_SYMBOL(psa_cipher_abort),
    DEFINE_MODULE_SYMBOL(psa_sign_hash),
    DEFINE_MODULE_SYMBOL(psa_verify_hash),
    DEFINE_MODULE_SYMBOL(psa_asymmetric_encrypt),
    DEFINE_MODULE_SYMBOL(psa_asymmetric_decrypt),
    DEFINE_MODULE_SYMBOL(psa_export_public_key),
    // Error strings
    DEFINE_MODULE_SYMBOL(mbedtls_strerror),
    MODULE_SYMBOL_TERMINATOR,
};

Module mbedtls_module = {
    .name = "mbedtls",
    .start = nullptr,
    .stop = nullptr,
    .drivers = nullptr,
    .symbols = SYMBOLS,
    .internal = nullptr,
};

}
