#ifndef WAVELET_SMART_CONTRACT_CPP_PAYLOAD_H_
#define WAVELET_SMART_CONTRACT_CPP_PAYLOAD_H_

#include <stdint.h>
#include <stdlib.h>

#include <array>
#include <string>
#include <vector>

#include "sys.h"

namespace payload {

template <typename T>
void write_to(std::vector<uint8_t>& buffer, const T& source) {
  for (size_t i = 0; i < sizeof(source); i++) {
    buffer.push_back(static_cast<uint8_t>(source >> (i * 8)));
  }
}

template <typename T>
void write_to(std::vector<uint8_t>& buffer, const std::array<T, 32>& source) {
  for (const auto& elem : source) {
    write_to(buffer, elem);
  }
}

template <typename T>
void write_to(std::vector<uint8_t>& buffer, const std::vector<T>& source) {
  write_to(buffer, source.size());

  for (const auto& elem : source) {
    write_to(buffer, elem);
  }
}

template <>
void write_to(std::vector<uint8_t>& buffer, const std::string& source) {
  write_to(buffer, source.length());

  for (const auto& elem : source) {
    write_to(buffer, elem);
  }
}

template <typename T>
void read_from(const std::vector<uint8_t>& buffer, uint64_t& pos, T& dest) {
  dest = 0;
  for (size_t i = 0; i < sizeof(dest); i++) {
    dest |= static_cast<T>(buffer.at(pos++) << (i * 8));
  }
}

template <typename T>
void read_from(const std::vector<uint8_t>& buffer, uint64_t& pos,
               std::array<T, 32>& dest) {
  for (size_t i = 0; i < 32; i++) {
    T dest_elem;
    read_from(buffer, pos, dest_elem);
    dest.at(i) = std::move(dest_elem);
  }
}

template <typename T>
void read_from(const std::vector<uint8_t>& buffer, uint64_t& pos,
               std::vector<T>& dest) {
  size_t size;
  read_from(buffer, pos, size);

  for (size_t i = 0; i < size; i++) {
    T dest_elem;
    read_from(buffer, pos, dest_elem);
    dest.push_back(std::move(dest_elem));
  }
}

template <>
void read_from(const std::vector<uint8_t>& buffer, uint64_t& pos,
               std::string& dest) {
  size_t length;
  read_from(buffer, pos, length);

  for (size_t i = 0; i < length; i++) {
    char dest_elem;
    read_from(buffer, pos, dest_elem);
    dest.push_back(std::move(dest_elem));
  }
}

struct Parameters {
  uint64_t round_idx;
  std::array<uint8_t, 32> round_id;
  std::array<uint8_t, 32> transaction_id;
  std::array<uint8_t, 32> sender;
  uint64_t amount;

  std::vector<uint8_t> parameters;
  uint64_t pos;

  Parameters(bool read_ = true) : pos(0) {
    auto payload_len = sys::_payload_len();
    std::vector<uint8_t> payload_bytes(payload_len);
    sys::_payload(&payload_bytes[0]);

    this->parameters = payload_bytes;

    if (read_) {
      this->round_idx = read<uint8_t>();
      this->round_id = read<std::array<uint8_t, 32>>();
      this->transaction_id = read<std::array<uint8_t, 32>>();
      this->sender = read<std::array<uint8_t, 32>>();
      this->amount = read<uint8_t>();
    }
  }

  template <typename T>
  T read() {
    T dest;
    read_from(this->parameters, this->pos, dest);
    return std::move(dest);
  }
};

class ParametersBuilder {
 public:
  ParametersBuilder() : params(false) {}

  ParametersBuilder* with_round_idx(uint64_t round_idx) {
    this->params.round_idx = round_idx;
    return this;
  }

  ParametersBuilder* with_round_id(std::array<uint8_t, 32> round_id) {
    this->params.round_id = round_id;
    return this;
  }

  ParametersBuilder* with_transaction_id(
      std::array<uint8_t, 32> transaction_id) {
    this->params.transaction_id = transaction_id;
    return this;
  }

  ParametersBuilder* with_sender(std::array<uint8_t, 32> sender) {
    this->params.sender = sender;
    return this;
  }

  ParametersBuilder* with_amount(uint64_t amount) {
    this->params.amount = amount;
    return this;
  }

  Parameters build() { return std::move(params); }

  template <typename T>
  void write(T source) {
    write_to(params.parameters, source);
  }

 private:
  Parameters params;
};

}  // namespace payload

#endif  // WAVELET_SMART_CONTRACT_CPP_PAYLOAD_H_
