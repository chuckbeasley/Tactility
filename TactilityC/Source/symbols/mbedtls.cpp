#include <private/elf_symbol.h>
#include <cstddef>

#include <symbols/mbedtls.h>

#include <psa/crypto.h>
#include <mbedtls/error.h>

const esp_elfsym mbedtls_symbols[] = {
    // PSA crypto (Mbed TLS v4 / ESP-IDF 6.x)
    ESP_ELFSYM_EXPORT(psa_crypto_init),
    ESP_ELFSYM_EXPORT(psa_generate_random),
    ESP_ELFSYM_EXPORT(psa_import_key),
    ESP_ELFSYM_EXPORT(psa_destroy_key),
    ESP_ELFSYM_EXPORT(psa_hash_compute),
    ESP_ELFSYM_EXPORT(psa_cipher_encrypt),
    ESP_ELFSYM_EXPORT(psa_cipher_decrypt),
    ESP_ELFSYM_EXPORT(psa_cipher_encrypt_setup),
    ESP_ELFSYM_EXPORT(psa_cipher_decrypt_setup),
    ESP_ELFSYM_EXPORT(psa_cipher_set_iv),
    ESP_ELFSYM_EXPORT(psa_cipher_update),
    ESP_ELFSYM_EXPORT(psa_cipher_finish),
    ESP_ELFSYM_EXPORT(psa_cipher_abort),
    ESP_ELFSYM_EXPORT(psa_sign_hash),
    ESP_ELFSYM_EXPORT(psa_verify_hash),
    ESP_ELFSYM_EXPORT(psa_asymmetric_encrypt),
    ESP_ELFSYM_EXPORT(psa_asymmetric_decrypt),
    ESP_ELFSYM_EXPORT(psa_export_public_key),
    // Error strings
    ESP_ELFSYM_EXPORT(mbedtls_strerror),
    // delimiter
    ESP_ELFSYM_END
};
