#ifndef SERCOM_H_
#define SERCOM_H_

#include <stdint.h>
#include <stdbool.h>
#include "sam.h"

typedef uint8_t RingBuf;
typedef uint8_t Buf;

typedef enum {
    SERCOM_MODE_NONE,
    SERCOM_MODE_UART, /**< UART async mode */
    SERCOM_MODE_I2C,  /**< I2C master mode */
    SERCOM_MODE_SPI   /**< SPI mode */
} SercomMode_t;

typedef struct {
    sercom_registers_t *const inst; /**< Register instance */
    const unsigned int pch_num;     /**< Peripheral channel # */
    bool enabled;                   /**< Peripherial configured and active */
    SercomMode_t mode;              /**< Current peripheral protocol */
    RingBuf *rx_buf;                /**< Recieve buffer */
    Buf *tx_buf;                    /**< Send buffer */
} Sercom_t;

extern Sercom_t sercom0;
extern Sercom_t sercom1;
extern Sercom_t sercom2;
extern Sercom_t sercom3;
extern Sercom_t sercom4;
extern Sercom_t sercom5;

#endif /* SERCOM_H_ */
