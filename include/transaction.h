#ifndef WAVELET_SMART_CONTRACT_CPP_TRANSACTION_H_
#define WAVELET_SMART_CONTRACT_CPP_TRANSACTION_H_

#include <stdint.h>

#include <array>
#include <optional>
#include <vector>

#include "payload.h"

namespace transaction {

enum TransactionTag { Nop, Transfer, Contract, Stake };

class Transfer {
 public:
  struct Invocation {
    uint64_t gas_limit;
    uint64_t gas_deposit;
    std::vector<uint8_t> func_name;
    std::vector<uint8_t> func_params;
  };

  std::array<uint8_t, 32> destination;
  uint64_t amount;
  std::optional<Invocation> invocation;

  TransactionTag tag() { return TransactionTag::Transfer; }
  void send_transaction();
};

struct Contract {
  std::vector<uint8_t> payload;
  std::vector<uint8_t> code;
};

}  // namespace transaction

namespace payload {

template <>
void write_to(std::vector<uint8_t>& buffer,
              const class transaction::Transfer& source) {
  write_to(buffer, source.destination);
  write_to(buffer, source.amount);

  if (source.invocation) {
    write_to(buffer, source.invocation->gas_limit);
    write_to(buffer, source.invocation->gas_deposit);
    write_to(buffer, source.invocation->func_name);
    write_to(buffer, source.invocation->func_params);
  }
}

template <>
void read_from(const std::vector<uint8_t>& buffer, uint64_t& pos,
               class transaction::Transfer& params) {
  read_from(buffer, pos, params.destination);
  read_from(buffer, pos, params.amount);

  if (pos < buffer.size()) {
    transaction::Transfer::Invocation invocation;

    // ignore gas limit and deposit
    uint64_t _;  // unused
    read_from(buffer, pos, _);
    read_from(buffer, pos, _);

    if (pos < buffer.size()) {
      read_from(buffer, pos, invocation.func_name);
    }

    if (pos < buffer.size()) {
      read_from(buffer, pos, invocation.func_params);
    }

    params.invocation = invocation;
  }
}

template <>
void read_from(const std::vector<uint8_t>& buffer, uint64_t& pos,
               struct transaction::Contract& params) {
  uint64_t _;                 // unused
  read_from(buffer, pos, _);  // ignore gas limit
  read_from(buffer, pos, _);  // ignore gas deposit
  read_from(buffer, pos, params.payload);
  read_from(buffer, pos, params.code);
}

}  // namespace payload

namespace transaction {

void Transfer::send_transaction() {
  std::vector<uint8_t> payload;
  payload::write_to(payload, *this);

  sys::_send_transaction(tag(), &payload[0], payload.size());
}

}  // namespace transaction

#endif  // WAVELET_SMART_CONTRACT_CPP_TRANSACTION_H_
