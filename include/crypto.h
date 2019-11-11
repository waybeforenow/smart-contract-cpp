#ifndef WAVELET_SMART_CONTRACT_CPP_CRYPTO_H_
#define WAVELET_SMART_CONTRACT_CPP_CRYPTO_H_

#include <stdint.h>

#include "sys.h"

namespace crypto {

enum SignatureAlgorithm { Ed25519 };

enum HashAlgorithm { Blake2b256, Blake2b512, Sha256, Sha512 };

const size_t BLAKE2B256_OUTPUT_SIZE = 32;
const size_t BLAKE2B512_OUTPUT_SIZE = 64;
const size_t SHA256_OUTPUT_SIZE = 32;
const size_t SHA512_OUTPUT_SIZE = 64;

void verify(SignatureAlgorithm alg, const std::vector<uint8_t>& pubkey,
            const std::vector<uint8_t>& data, const std::vector<uint8_t>& sig) {
  switch (alg) {
    case SignatureAlgorithm::Ed25519:
      sys::_verify_ed25519(&pubkey[0], pubkey.length(), &data[0], data.length(),
                           &sig[0], sig.length());
      break;
  }
}

void hash(HashAlgorithm alg, const std::vector<uint8_t>& data, uint8_t** out,
          size_t* out_len) {
  switch (alg) {
    case HashAlgorithm::Blake2b256:
      sys::_hash_blake2b_256(&data[0], data.length(), out, out_len);
      break;

    case HashAlgorithm::Blake2b512:
      sys::_hash_blake2b_512(&data[0], data.length(), out, out_len);
      break;

    case HashAlgorithm::Sha256:
      sys::_hash_sha256(&data[0], data.length(), out, out_len);
      break;

    case HashAlgorithm::Sha512:
      sys::_hash_sha512(&data[0], data.length(), out, out_len);
      break;
  }
}

}  // namespace crypto

#endif  // WAVELET_SMART_CONTRACT_CPP_CRYPTO_H_
