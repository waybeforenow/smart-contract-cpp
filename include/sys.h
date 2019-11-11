#ifndef WAVELET_SMART_CONTRACT_CPP_SYS_H_
#define WAVELET_SMART_CONTRACT_CPP_SYS_H_

#include <stdint.h>

namespace sys {

extern "C" {
size_t _payload_len();

void _payload(uint8_t *out);

void _log(const uint8_t *content, size_t len);

void _result(const uint8_t *result, size_t len);

void _send_transaction(uint8_t tag, const uint8_t *payload, size_t payload_len);

int32_t _verify_ed25519(const uint8_t *pubkey, size_t pubkey_len,
                        const uint8_t *data, size_t data_len,
                        const uint8_t *sig, size_t sig_len);

int32_t _hash_blake2b_256(const uint8_t *data, size_t data_len, uint8_t **out,
                          size_t out_len);

int32_t _hash_blake2b_512(const uint8_t *data, size_t data_len, uint8_t **out,
                          size_t out_len);

int32_t _hash_sha256(const uint8_t *data, size_t data_len, uint8_t **out,
                     size_t *out_len);

int32_t _hash_sha512(const uint8_t *data, size_t data_len, uint8_t **out,
                     size_t *out_len);
}

}  // namespace sys

#endif  // WAVELET_SMART_CONTRACT_CPP_SYS_H_
