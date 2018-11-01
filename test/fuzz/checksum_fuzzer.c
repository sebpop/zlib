#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <inttypes.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t len) {
  uLong crc1 = crc32(0L, NULL, 0);
  uLong crc2 = crc32(0L, NULL, 0);
  uLong adler1 = adler32(0L, NULL, 0);
  uLong adler2 = adler32(0L, NULL, 0);;
  /* Checksum with a buffer of size equal to the first byte in the input. */
  uInt buffSize = data[0];
  uLong offset = 0;
  uInt dataLen = len;

  /* Discard inputs larger than 1Mb. */
  static size_t kMaxSize = 1024 * 1024;
  if (dataLen < 1 || dataLen > kMaxSize)
    return 0;

  /* Make sure the buffer has at least a byte. */
  if (buffSize == 0)
    ++buffSize;

  /* CRC32 */
  for (offset = 0; offset + buffSize <= dataLen; offset += buffSize)
    crc1 = crc32(crc1, data + offset, buffSize);
  crc1 = crc32(crc1, data + offset, dataLen % buffSize);

  crc2 = crc32(crc2, data, dataLen);

  assert(crc1 == crc2);
  assert(crc32_combine(crc1, crc2, dataLen) ==
         crc32_combine(crc1, crc1, dataLen));

  /* Adler32 */
  for (offset = 0; offset + buffSize <= dataLen; offset += buffSize)
    adler1 = adler32(adler1, data + offset, buffSize);
  adler1 = adler32(adler1, data + offset, dataLen % buffSize);

  adler2 = adler32(adler2, data, dataLen);

  assert(adler1 == adler2);
  assert(adler32_combine(adler1, adler2, dataLen) ==
         adler32_combine(adler1, adler1, dataLen));

  /* This function must return 0. */
  return 0;
}
