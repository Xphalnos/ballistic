#include "decoder.h"
#include <stddef.h>

bal_decoder_instruction_metadata_t *
bal_decoder_arm64_decode (const uint32_t instruction)
{
#if 0
    /* Extract hash key: Bits [27:20] and [7:4] */
    const uint32_t major = (instruction >> 20U) & 0xFFU;
    const uint32_t minor = (instruction >> 4U) & 0xFU;

    const uint16_t index = (uint16_t)((major << 4U) | minor);

    /*TODO: Implement hash table */
    return NULL;
#endif
    (void)instruction; // Bypass unused aprameter warning.
    return NULL;
}
