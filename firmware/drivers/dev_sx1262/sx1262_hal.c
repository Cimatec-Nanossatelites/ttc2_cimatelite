#include "drivers/drivers.h"
#include <FreeRTOS.h>
#include <task.h>
#include "config.h"

#include "drivers/dev_sx1262/sx1262_hal.h"

#define SX126X_SPI_PORT         SPI_PORT_3
#define SX126X_SPI_CS_PIN       SPI_CS_0
#define SX126X_SPI_MODE         SPI_MODE_0
#define SX126X_SPI_SPEED        CONFIG_SPI_PORT_0_SPEED_BPS

/* GPIO configuration */
#define SX126X_GPIO_RESET_PIN           GPIO_PIN_16
#define SX126X_GPIO_NIRQ_PIN            GPIO_PIN_18
#define SX126X_GPIO_BUSY_PIN            GPIO_PIN_17

//static SemaphoreHandle_t sx126x_mutex = NULL;

/* Mutex cofig. */
#define sx126x_MUTEX_WAIT_TIME_MS 100
#define sx126x_MODULE_NAME "sx126x"

/* Private Functions Prototype */

/**
 * @brief
 */
static int sx126x_spi_write(uint8_t *data, uint16_t len);

/**
 * @brief
 */
static int sx126x_spi_read(uint8_t *data, uint16_t len);

extern uint8_t spi_transfer_byte(uint16_t base_address, uint8_t wb);

int sx126x_hal_gpio_init(void)
{
    int err = 0;

    gpio_config_t conf;

    conf.mode = GPIO_MODE_OUTPUT;

    if (gpio_init(SX126X_GPIO_RESET_PIN, conf) != 0)
        err = -1;

    conf.mode = GPIO_MODE_INPUT;

    if (gpio_init(SX126X_GPIO_NIRQ_PIN, conf) != 0)
        err = -1;

    conf.mode = GPIO_MODE_INPUT;

    if(gpio_init(SX126X_GPIO_BUSY_PIN, conf) != 0)
        err = -1;


    return err;
}

uint8_t sx1262_hal_gpio_read_busy(void)
{
    return gpio_get_state(SX126X_GPIO_BUSY_PIN);
}

void sx1262_hal_gpio_write_reset(uint8_t state)
{
    gpio_set_state(SX126X_GPIO_RESET_PIN, state);
}

int sx126x_gpio_read_nirq(void)
{
    return gpio_get_state(SX126X_GPIO_NIRQ_PIN);
}

int sx126x_hal_spi_init(void)
{
    spi_config_t conf = { 0 };

    conf.speed_hz = SX126X_SPI_SPEED;
    conf.mode = SX126X_SPI_MODE;

    return spi_init(SX126X_SPI_PORT, conf);
}

#define CS_ON spi_select_slave(SX126X_SPI_PORT, SX126X_SPI_CS_PIN, true);
#define CS_OFF spi_select_slave(SX126X_SPI_PORT, SX126X_SPI_CS_PIN, false);

void sx1262_hal_spi_cs_low(void)
{
    CS_ON
}

void sx1262_hal_spi_cs_high(void)
{
    CS_OFF
}

static int sx126x_spi_write(uint8_t *data, uint16_t len)
{
    int err = 0;

    uint16_t base_address = 0;

    switch (SX126X_SPI_PORT)
    {
    case SPI_PORT_0:
        base_address = USCI_A0_BASE;
        break;
    case SPI_PORT_1:
        base_address = USCI_A1_BASE;
        break;
    case SPI_PORT_2:
        base_address = USCI_A2_BASE;
        break;
    case SPI_PORT_3:
        base_address = USCI_B0_BASE;
        break;
    case SPI_PORT_4:
        base_address = USCI_B1_BASE;
        break;
    case SPI_PORT_5:
        base_address = USCI_B2_BASE;
        break;
    default:
#if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during writing: Invalid port!");
            sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1; /* Invalid SPI port */

        break;
    }

    for (uint16_t i = 0; i < len; i++)
    {
        uint8_t dummy = spi_transfer_byte(base_address, data[i]);
    }

    return err; //spi_write(SX127X_SPI_PORT, SX127X_SPI_CS_PIN, data, len);
}

static int sx126x_spi_read(uint8_t *data, uint16_t len)
{
    int err = 0;

    uint16_t base_address = 0;

    switch (SX126X_SPI_PORT)
    {
    case SPI_PORT_0:
        base_address = USCI_A0_BASE;
        break;
    case SPI_PORT_1:
        base_address = USCI_A1_BASE;
        break;
    case SPI_PORT_2:
        base_address = USCI_A2_BASE;
        break;
    case SPI_PORT_3:
        base_address = USCI_B0_BASE;
        break;
    case SPI_PORT_4:
        base_address = USCI_B1_BASE;
        break;
    case SPI_PORT_5:
        base_address = USCI_B2_BASE;
        break;
    default:
#if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
          sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during reading: Invalid port!");
          sys_log_new_line();
  #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1; /* Invalid SPI port */

        break;
    }

    if (err == 0)
    {
        /* Read data */
        uint16_t i = 0;
        for (i = 0; i < len; i++)
        {
            data[i] = spi_transfer_byte(base_address, 0xff);
        }
    }

    return err; //spi_read(SX127X_SPI_PORT, SX127X_SPI_CS_PIN, data, len);
}

void sx1262_hal_spi_transmit_receive(uint8_t *tx_data, uint8_t *rx_data,
                                     uint16_t size, uint32_t timeout)
{
    CS_ON
//    sx126x_spi_write(tx_data, size);
//    sx126x_spi_read(rx_data, size);

    spi_transfer(SX126X_SPI_PORT, SX126X_SPI_CS_PIN, tx_data, rx_data, size);
    CS_OFF
}

void sx1262_hal_delay_ms(uint32_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

uint32_t sx1262_hal_get_tick(void)
{
    return (uint32_t) xTaskGetTickCount();
}

void rx_callback(uint8_t *data, uint8_t length)
{
    return;
}
