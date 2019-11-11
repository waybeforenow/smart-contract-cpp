#ifndef WAVELET_SMART_CONTRACT_CPP_WAVELET_H_
#define WAVELET_SMART_CONTRACT_CPP_WAVELET_H_

#include <string>

namespace wavelet {

#include "crypto.h"
#include "payload.h"
#include "sys.h"
#include "transaction.h"

void log(const std::string& msg) { sys::_log(&msg[0], msg.length()); }

}  // namespace wavelet

#endif  // WAVELET_SMART_CONTRACT_CPP_WAVELET_H_
