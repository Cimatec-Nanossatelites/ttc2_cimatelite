/*
 * sx1262_hal.h
 *
 *  Created on: Jan 15, 2026
 *      Author: ramon.papes
 */

#ifndef LIBS_DEV_SX1262_SX1262_HAL_H_
#define LIBS_DEV_SX1262_SX1262_HAL_H_

/*
 * sx1262_hal.h
 * HAL Abstraction Layer for SX1262
 */

#ifndef SX1262_HAL_H_
#define SX1262_HAL_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif



/**
 * @brief HAL function pointers structure
 * This structure contains all hardware-dependent functions
 */
typedef struct {
    // SPI functions
    int (*spi_init)(void);
    void (*spi_transmit_receive)(uint8_t *tx_data, uint8_t *rx_data, uint16_t size, uint32_t timeout);
    void (*spi_cs_low)(void);
    void (*spi_cs_high)(void);

    // GPIO functions
    int (*gpio_init)(void);
    uint8_t (*gpio_read_busy)(void);  // For BUSY pin
    void (*gpio_write_reset)(uint8_t state);  // For RESET pin
    void (*delay_ms)(uint32_t ms);
    uint32_t (*get_tick)(void);

    // Callback for received data
    void (*rx_callback)(uint8_t *data, uint8_t length);
} sx1262_hal_t;

//void sx1262_hal_default_init(sx1262_hal_t *hal);
//void sx1262_hal_default_deinit(sx1262_hal_t *hal);
int sx126x_hal_spi_init(void);
int sx126x_hal_gpio_init(void);
void sx1262_hal_delay_ms(uint32_t ms);
void sx1262_hal_spi_transmit_receive(uint8_t *tx_data, uint8_t *rx_data, uint16_t size, uint32_t timeout);
void sx1262_hal_spi_cs_low(void);
void sx1262_hal_spi_cs_high(void);
void sx1262_hal_gpio_write_reset(uint8_t state);
uint8_t sx1262_hal_gpio_read_busy(void);
uint32_t sx1262_hal_get_tick(void);
void rx_callback(uint8_t *data, uint8_t length);



#ifdef __cplusplus
}
#endif

#endif /* SX1262_HAL_H_ */


#endif /* LIBS_DEV_SX1262_SX1262_HAL_H_ */
