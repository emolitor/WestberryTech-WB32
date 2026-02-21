/*
 * Board header for WB32FQ95 Multi-Transfer DMA Test
 */

#ifndef BOARD_H
#define BOARD_H

/*
 * Board identifier
 */
#define WB32FQ95xC
#if !defined(WB32FQ95xx)
  #define WB32FQ95xx
#endif

/*
 * External declarations
 */
#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif

#endif /* BOARD_H */
