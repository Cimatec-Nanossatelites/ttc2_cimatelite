/*
 * Sx127x_Port.c
 *
 *  Created on: 9 de ago de 2024
 *      Author: SENAI
 */

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>


#include <system/sys_log/sys_log.h>

#include <devices/leds/leds.h>
#include <drivers/gpio/gpio.h>
#include <drivers/spi/spi.h>
#include <config.h>

#include "Sx127x_port.h"

#include <hal/usci_a_spi.h>
#include <hal/usci_b_spi.h>

static SemaphoreHandle_t sx127x_mutex = NULL;


/* Mutex config. */
#define sx127x_MUTEX_WAIT_TIME_MS      100
#define sx127x_MODULE_NAME             "Si446x"



uint32_t sx127x_spi_transmit(uint8_t* buffer, uint32_t size);
uint32_t sx127x_spi_transmit_receive(uint8_t* tx_buffer, uint32_t tx_size, uint8_t* rx_buffer, uint32_t rx_size);
void sx127x_delay(uint32_t delay);
void sx127x_reset_control(bool state);
void sx127x_rx_callback(uint8_t* buffer, uint8_t size);
void sx127x_tx_callback(void);
void sx127x_rx_timeout_callback(void);
void sx127x_rx_crc_error_callback(void);

const sx127x_spi_t sx127x_spi =
{
 .transmit = &sx127x_spi_transmit,
 .transmit_receive = &sx127x_spi_transmit_receive,
};

const sx127x_callbacks_t sx127x_callbacks =
{
 .tx_done = &sx127x_tx_callback,
 .rx_done = &sx127x_rx_callback,
 .rx_timeout = &sx127x_rx_timeout_callback,
 .rx_crc_error = &sx127x_rx_crc_error_callback,
};

const sx127x_common_t sx127x_common =
{
 .delay = &sx127x_delay,
 .reset_control = &sx127x_reset_control,
};

sx127x_dev_t sx1278_phy =
{
 .spi = &sx127x_spi,
 .callbacks = &sx127x_callbacks,
 .common = &sx127x_common,
};


#define SX127X_SPI_PORT         SPI_PORT_3
#define SX127X_SPI_CS_PIN       SPI_CS_0
#define SX127X_SPI_MODE         SPI_MODE_0
#define SX127X_SPI_SPEED        CONFIG_SPI_PORT_0_SPEED_BPS

/* GPIO configuration */
#define SX127X_GPIO_RESET_PIN           GPIO_PIN_16
#define SX127X_GPIO_NIRQ_PIN            GPIO_PIN_17

int sx127x_gpio_init(void)
{
    int err = 0;

    gpio_config_t conf = {0};

    conf.mode = GPIO_MODE_OUTPUT;

    /* RESET pin */
    if (gpio_init(SX127X_GPIO_RESET_PIN, conf) != 0)
    {
        err = -1;
    }

    conf.mode = GPIO_MODE_INPUT;

    /* nIRQ pin */
    if (gpio_init(SX127X_GPIO_NIRQ_PIN, conf) != 0)
    {
        err = -1;
    }

    return err;
}

int sx127X_gpio_write_reset(bool state)
{
    return gpio_set_state(SX127X_GPIO_RESET_PIN, state);
}

int sx127x_gpio_read_nirq(void)
{
    return gpio_get_state(SX127X_GPIO_NIRQ_PIN);
}

int sx127x_spi_init(void)
{
    spi_config_t conf = {0};

    conf.speed_hz   = SX127X_SPI_SPEED;
    conf.mode       = SX127X_SPI_MODE;

    return spi_init(SX127X_SPI_PORT, conf);
}


#define CS_ON spi_select_slave(SX127X_SPI_PORT, SX127X_SPI_CS_PIN, true);
#define CS_OFF spi_select_slave(SX127X_SPI_PORT, SX127X_SPI_CS_PIN, false);

extern uint8_t spi_transfer_byte(uint16_t base_address, uint8_t wb);

int sx127x_spi_write(uint8_t *data, uint16_t len)
{
    int err = 0;

    uint16_t base_address = 0;

    switch(SX127X_SPI_PORT)
    {
    case SPI_PORT_0:    base_address = USCI_A0_BASE;    break;
    case SPI_PORT_1:    base_address = USCI_A1_BASE;    break;
    case SPI_PORT_2:    base_address = USCI_A2_BASE;    break;
    case SPI_PORT_3:    base_address = USCI_B0_BASE;    break;
    case SPI_PORT_4:    base_address = USCI_B1_BASE;    break;
    case SPI_PORT_5:    base_address = USCI_B2_BASE;    break;
    default:
#if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during writing: Invalid port!");
        sys_log_new_line();
#endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;   /* Invalid SPI port */

        break;
    }

    for(uint16_t i = 0; i < len; i++)
    {
        uint8_t dummy = spi_transfer_byte(base_address, data[i]);
    }


    return err; //spi_write(SX127X_SPI_PORT, SX127X_SPI_CS_PIN, data, len);
}

int sx127x_spi_read(uint8_t *data, uint16_t len)
{

    int err = 0;

    uint16_t base_address = 0;

    switch(SX127X_SPI_PORT)
    {
    case SPI_PORT_0:    base_address = USCI_A0_BASE;    break;
    case SPI_PORT_1:    base_address = USCI_A1_BASE;    break;
    case SPI_PORT_2:    base_address = USCI_A2_BASE;    break;
    case SPI_PORT_3:    base_address = USCI_B0_BASE;    break;
    case SPI_PORT_4:    base_address = USCI_B1_BASE;    break;
    case SPI_PORT_5:    base_address = USCI_B2_BASE;    break;
    default:
#if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Error during reading: Invalid port!");
        sys_log_new_line();
#endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;   /* Invalid SPI port */

        break;
    }

    if (err == 0)
    {
        /* Read data */
        uint16_t i = 0;
        for(i = 0; i < len; i++)
        {
            data[i] = spi_transfer_byte(base_address, 0xff);
        }
    }

    return err; //spi_read(SX127X_SPI_PORT, SX127X_SPI_CS_PIN, data, len);
}


uint32_t sx127x_spi_transmit(uint8_t* buffer, uint32_t size)
{
    CS_ON

    sx127x_spi_write(buffer, size);

    CS_OFF

    return 0;
}

uint32_t sx127x_spi_transmit_receive(uint8_t* tx_buffer, uint32_t tx_size, uint8_t* rx_buffer, uint32_t rx_size)
{
    CS_ON
    sx127x_spi_write(tx_buffer, tx_size);
    sx127x_spi_read(rx_buffer, rx_size);
    CS_OFF
    return 0;
}

void sx127x_delay(uint32_t delay)
{
    vTaskDelay(pdMS_TO_TICKS(delay));
}

void sx127x_reset_control(bool state)
{
    sx127X_gpio_write_reset(state);
}


void sx127x_tx_callback()
{
    sx127x_receive_continuous_it(&sx1278_phy);
}

void sx127x_rx_timeout_callback()
{

}

void sx127x_rx_callback(uint8_t* buffer, uint8_t size)
{
    sys_log_print_event_from_module(SYS_LOG_INFO, "LoRa:  ", buffer);
    sys_log_new_line();
}

void sx127x_rx_crc_error_callback()
{

}

int sx127x_radio_init(void)
{
    sx127x_mutex_create();

    int err = sx127x_spi_init();

    err = sx127x_gpio_init();

    sx127x_radio_settings_t settings =
    {
       .modulation = LORA,
       .pa_select = PA_BOOST,
       .power = 0x0a,
       .spreading_factor = SF_10,
       .band_width = BW_125_KHz,
       .coding_rate = CR_4_5,
       .payload_crc_on = true,
       .preamble_length = 10,
       .frequency = 433000000,
       .sync_word = 0x12,
    };

    if (sx127x_init(&sx1278_phy, &settings) == 0)
    {
        err = 0;
        sx127x_rx_init();
    }

    return err;
}


bool sx127x_rx_init(void)
{
    return (sx127x_receive_continuous_it(&sx1278_phy) == 0);
}

int sx127x_mutex_create(void)
{
    int err = 0;

    sx127x_mutex = xSemaphoreCreateMutex();

    if (sx127x_mutex == NULL)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, sx127x_MODULE_NAME, "Error creating a mutex!");
        sys_log_new_line();

        err = -1;
    }

    return err;
}

int sx127x_mutex_take(void)
{
    int err = -1;

    if (sx127x_mutex != NULL)
    {
        /* See if we can obtain the semaphore. If the semaphore is not */
        /* available wait sx127x_MUTEX_WAIT_TIME_MS ms to see if it becomes free */
        if (xSemaphoreTake(sx127x_mutex, pdMS_TO_TICKS(sx127x_MUTEX_WAIT_TIME_MS)) == pdTRUE)
        {
            err = 0;
        }
    }

    return err;
}

int sx127x_mutex_give(void)
{
    int err = -1;
    if (sx127x_mutex != NULL)
    {
        xSemaphoreGive(sx127x_mutex);
        err = 0;
    }
    return err;
}






