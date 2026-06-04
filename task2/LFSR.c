/* =====================================================
 *
 * LFSR.c -- 32-bit  LFSR pseudo-random
 *                shift register simulation
 *
 * Functions: lfsr32_next(), main().
 * Author:   Athnsh
 * Created:  2024-06-01
 *
 * ===================================================
 */

#include <stdint.h>
#include <stdio.h>

#define LFSR_POLY 0xB4BCD35Cu // maximum 32 bit poly
#define LFSR_SEED 0xFAC2u     // starting val
#define N_STEPS 16            // num of outputs

uint32_t lfsr32_next(uint32_t *state) {
  uint32_t lsb = *state & 1u; // pick out the lsb
  *state >>= 1;               // shift the bits
  if (lsb)
    *state ^= LFSR_POLY; // if the lsb was 1, apply the feedback polynomial
  return *state;
}

int main(void) {
  uint32_t state = LFSR_SEED;

  printf("Step  Output\n");
  printf("----  ----------\n");
  for (int i = 0; i < N_STEPS; i++)
    printf("%-4d  0x%08X\n", i + 1, lfsr32_next(&state));

  return 0;
}
