/* 
 * This implementation comes from LLVM although I have changed it to a
 * C file, removed the C++ namespace, the inline directive, the pad to
 * argument and changed how results and errors are returned.
 */

//===- llvm/Support/LEB128.h - [SU]LEB128 utility functions -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares some utility functions for encoding SLEB128 and
// ULEB128 values.
//
//===----------------------------------------------------------------------===//

#include "decoder.h"

/// Utility function to encode a SLEB128 value to a buffer. Returns
/// the length in bytes of the encoded value.
unsigned encodeSLEB128(int64_t Value, uint8_t *p) {
  uint8_t *orig_p = p;
  int More;
  do {
    uint8_t Byte = Value & 0x7f;
    // NOTE: this assumes that this signed shift is an arithmetic right shift.
    Value >>= 7;
    More = !((((Value == 0 ) && ((Byte & 0x40) == 0)) ||
              ((Value == -1) && ((Byte & 0x40) != 0))));
    if (More)
      Byte |= 0x80; // Mark this byte to show that more bytes will follow.
    *p++ = Byte;
  } while (More);
  
  return (unsigned)(p - orig_p);
}
 
/// Utility function to encode a ULEB128 value to a buffer. Returns
/// the length in bytes of the encoded value.
unsigned encodeULEB128(uint64_t Value, uint8_t *p) {
  uint8_t *orig_p = p;
  do {
    uint8_t Byte = Value & 0x7f;
    Value >>= 7;
    if (Value != 0)
      Byte |= 0x80; // Mark this byte to show that more bytes will follow.
    *p++ = Byte;
  } while (Value != 0);
 
  return (unsigned)(p - orig_p);
}
 
/// Utility function to decode a ULEB128 value.
unsigned_decode_result decodeULEB128(const uint8_t *p, const uint8_t *end) {
  const uint8_t *orig_p = p;
  uint64_t Value = 0;
  unsigned Shift = 0;
  do {
    if (p == end) {
      unsigned_decode_result result = { 0, ERROR_INSUFFICIENT_INPUT };
      return result;
    }
    uint64_t Slice = *p & 0x7f;
    if ((Shift >= 64 && Slice != 0) || Slice << Shift >> Shift != Slice) {
      unsigned_decode_result result = { 0, ERROR_TOO_BIG };
      return result;
    }
    Value += Slice << Shift;
    Shift += 7;
  } while (*p++ >= 128);
  unsigned_decode_result result = { Value, (unsigned)(p - orig_p) };
  return result;
}
 
/// Utility function to decode a SLEB128 value.
signed_decode_result decodeSLEB128(const uint8_t *p, const uint8_t *end) {
  const uint8_t *orig_p = p;
  int64_t Value = 0;
  unsigned Shift = 0;
  uint8_t Byte;
  do {
    if (p == end) {
      signed_decode_result result = { 0, ERROR_INSUFFICIENT_INPUT };
      return result;
    }
    Byte = *p;
    uint64_t Slice = Byte & 0x7f;
    // This handles decoding padded numbers, otherwise we might be
    // able to test very easily at the end of the loop.
    if ((Shift >= 64 && Slice != (Value < 0 ? 0x7f : 0x00)) ||
        (Shift == 63 && Slice != 0 && Slice != 0x7f)) {
      signed_decode_result result = { 0, ERROR_TOO_BIG };
      return result;
    }
    Value |= Slice << Shift;
    Shift += 7;
    ++p;
  } while (Byte >= 128);
  // Sign extend negative numbers if needed.
  if (Shift < 64 && (Byte & 0x40))
    Value |= (-1ULL) << Shift;
  signed_decode_result result = { Value, (p - orig_p) };
  return result;
}
