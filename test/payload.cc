#include "../include/payload.h"

#include <array>

#include "lib/catch.hpp"

TEST_CASE("Parameters build correctly", "[ParameterBuilder]") {
  using namespace payload;

  const uint64_t ROUND_IDX = 100;
  const std::array<uint8_t, 32> ROUND_ID = {42};
  const std::array<uint8_t, 32> TRANSACTION_ID = {0};
  const std::array<uint8_t, 32> SENDER = {1};
  const uint64_t AMOUNT = 20;

  auto builder = new ParametersBuilder();
  builder->with_round_idx(ROUND_IDX)
      ->with_round_id(ROUND_ID)
      ->with_transaction_id(TRANSACTION_ID)
      ->with_sender(SENDER)
      ->with_amount(AMOUNT);

  builder->write(static_cast<uint64_t>(100));
  builder->write(std::string("Hello"));

  auto params = builder->build();

  REQUIRE(params.round_idx == ROUND_IDX);
  REQUIRE(params.round_id == ROUND_ID);
  REQUIRE(params.transaction_id == TRANSACTION_ID);
  REQUIRE(params.sender == SENDER);
  REQUIRE(params.amount == AMOUNT);

  REQUIRE(params.read<uint64_t>() == 100);
  REQUIRE(params.read<std::string>() == "Hello");

  delete builder;
}
