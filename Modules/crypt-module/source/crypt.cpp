// SPDX-License-Identifier: Apache-2.0
#include <crypt/crypt.h>

#include <tactility/check.h>
#include <tactility/log.h>

#include <psa/crypto.h>
#include <mbedtls/platform_util.h>
#include <cstring>
#include <cstdint>

#ifdef ESP_PLATFORM
#include "esp_mac.h"
#include "esp_random.h"
#include "nvs_flash.h"
#else
#include <random>
#endif

constexpr auto* TAG = "crypt";

#define TT_NVS_NAMESPACE "tt_secure"

/**
 * Fills a buffer with cryptographically secure random bytes.
 * @param[out] out output buffer
 * @param[in] length number of bytes to fill
 */
static void fill_random(uint8_t* out, size_t length) {
#ifdef ESP_PLATFORM
    esp_fill_random(out, length);
#else
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 255);
    for (size_t i = 0; i < length; ++i) {
        out[i] = static_cast<uint8_t>(dist(gen));
    }
#endif
}

#ifdef ESP_PLATFORM
/**
 * Get a key based on hardware parameters.
 * @param[out] key the output key
 */
static void get_hardware_key(uint8_t key[32]) {
    uint8_t mac[8];
    // MAC can be 6 or 8 bytes
    size_t mac_length = esp_mac_addr_len_get(ESP_MAC_EFUSE_FACTORY);
    LOG_I(TAG, "Using MAC with length %zu", mac_length);
    check(mac_length <= 8);
    ESP_ERROR_CHECK(esp_read_mac(mac, ESP_MAC_EFUSE_FACTORY));

    // Fill buffer with repeating MAC
    for (size_t i = 0; i < 32; ++i) {
        key[i] = mac[i % mac_length];
    }
}
#endif

#ifdef ESP_PLATFORM
/**
 * The key is built up as follows:
 * - Fetch 32 bytes from NVS storage and store as key data
 * - Fetch 6-8 MAC bytes and overwrite the first 6-8 bytes of the key with this info
 *
 * When flash encryption is disabled:
 *   Without the MAC data, an attack would look like this:
 *   - Retrieve all the partitions from the ESP32
 *   - Read the key from NVS flash
 *   - Use the key to decrypt
 *   With the MAC data added, an attacker would have to do much more:
 *   - Retrieve all the partitions from the ESP32 (copy app)
 *   - Upload custom app to retrieve internal MAC
 *   - Read the key from NVS flash
 *   - Re-flash original app and combine it with the MAC
 *   - Use the key to decrypt
 *   - Re-flash the device with original firmware.
 *
 * Adding the MAC doesn't add a lot of extra security, but I think it's worth it.
 *
 * @param[out] key the output key
 */
static void get_nvs_key(uint8_t key[32]) {
    nvs_handle_t handle;
    esp_err_t result = nvs_open(TT_NVS_NAMESPACE, NVS_READWRITE, &handle);

    if (result != ESP_OK) {
        LOG_E(TAG, "Failed to get key from NVS (%s)", esp_err_to_name(result));
        check(false, "NVS error");
    }

    size_t length = 32;
    if (nvs_get_blob(handle, "key", key, &length) == ESP_OK) {
        LOG_I(TAG, "Fetched key from NVS (%zu bytes)", length);
        check(length == 32);
    } else {
        fill_random(key, 32);
        ESP_ERROR_CHECK(nvs_set_blob(handle, "key", key, 32));
        ESP_ERROR_CHECK(nvs_commit(handle));
        LOG_I(TAG, "Stored new key in NVS");
    }

    nvs_close(handle);
}
#endif

/**
 * Performs XOR on 2 memory regions and stores it in a third
 * @param[in] inLeft input buffer for XOR
 * @param[in] inRight second input buffer for XOR
 * @param[out] out output buffer for result of XOR
 * @param[in] length data length (all buffers must be at least this size)
 */
static void xorKey(const uint8_t* inLeft, const uint8_t* inRight, uint8_t* out, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        out[i] = inLeft[i] ^ inRight[i];
    }
}

/**
 * Combines a stored key and a hardware key into a single reliable key value.
 * @param[out] key the key output
 */
static void getKey(uint8_t key[32]) {
#if !defined(CONFIG_SECURE_BOOT) || !defined(CONFIG_SECURE_FLASH_ENC_ENABLED)
    LOG_W(TAG, "Using tt_secure_* code with secure boot and/or flash encryption disabled.");
    LOG_W(TAG, "An attacker with physical access to your ESP32 can decrypt your secure data.");
#endif

#ifdef ESP_PLATFORM
    uint8_t hardware_key[32];
    uint8_t nvs_key[32];

    get_hardware_key(hardware_key);
    get_nvs_key(nvs_key);
    xorKey(hardware_key, nvs_key, key, 32);
    mbedtls_platform_zeroize(hardware_key, sizeof(hardware_key));
    mbedtls_platform_zeroize(nvs_key, sizeof(nvs_key));
#else
    LOG_W(TAG, "Using unsafe key for debugging purposes.");
    memset(key, 0, 32);
#endif
}

void crypt_get_iv(const void* data, size_t dataLength, uint8_t iv[16]) {
    uint8_t hash[32];
    size_t hash_len = 0;
    psa_hash_compute(PSA_ALG_SHA_256, static_cast<const uint8_t*>(data), dataLength, hash, sizeof(hash), &hash_len);
    memcpy(iv, hash, 16);
    mbedtls_platform_zeroize(hash, sizeof(hash));
}

void crypt_generate_iv(uint8_t iv[16]) {
    fill_random(iv, 16);
}

static int aes256CryptCbcPsa(
    const uint8_t key[32],
    bool encrypt,
    size_t length,
    const uint8_t iv[16],
    const uint8_t* input,
    uint8_t* output
) {
    check(key && iv && input && output);

    if ((length % 16) || (length == 0)) {
        return -1;
    }

    psa_key_attributes_t attrs = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_type(&attrs, PSA_KEY_TYPE_AES);
    psa_set_key_bits(&attrs, 256);
    psa_set_key_usage_flags(&attrs, encrypt ? PSA_KEY_USAGE_ENCRYPT : PSA_KEY_USAGE_DECRYPT);
    psa_set_key_algorithm(&attrs, PSA_ALG_CBC_NO_PADDING);

    psa_key_id_t key_id = PSA_KEY_ID_NULL;
    psa_status_t status = psa_import_key(&attrs, key, 32, &key_id);
    psa_reset_key_attributes(&attrs);
    if (status != PSA_SUCCESS) return -1;

    psa_cipher_operation_t op = PSA_CIPHER_OPERATION_INIT;
    if (encrypt) {
        status = psa_cipher_encrypt_setup(&op, key_id, PSA_ALG_CBC_NO_PADDING);
    } else {
        status = psa_cipher_decrypt_setup(&op, key_id, PSA_ALG_CBC_NO_PADDING);
    }
    if (status != PSA_SUCCESS) {
        psa_destroy_key(key_id);
        return -1;
    }

    status = psa_cipher_set_iv(&op, iv, 16);
    if (status != PSA_SUCCESS) {
        psa_cipher_abort(&op);
        psa_destroy_key(key_id);
        return -1;
    }

    size_t out_len = 0;
    status = psa_cipher_update(&op, input, length, output, length, &out_len);
    if (status != PSA_SUCCESS) {
        psa_cipher_abort(&op);
        psa_destroy_key(key_id);
        return -1;
    }

    size_t finish_len = 0;
    status = psa_cipher_finish(&op, output + out_len, length - out_len, &finish_len);
    psa_cipher_abort(&op);
    psa_destroy_key(key_id);
    return status == PSA_SUCCESS ? 0 : -1;
}

int crypt_encrypt(const uint8_t iv[16], const uint8_t* inData, uint8_t* outData, size_t dataLength) {
    uint8_t key[32];
    getKey(key);
    int result = aes256CryptCbcPsa(key, true, dataLength, iv, inData, outData);
    mbedtls_platform_zeroize(key, sizeof(key));
    return result;
}

int crypt_decrypt(const uint8_t iv[16], const uint8_t* inData, uint8_t* outData, size_t dataLength) {
    uint8_t key[32];
    getKey(key);
    int result = aes256CryptCbcPsa(key, false, dataLength, iv, inData, outData);
    mbedtls_platform_zeroize(key, sizeof(key));
    return result;
}
