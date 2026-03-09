/*&sx
 * radio.c
 * 
 * Copyright The TTC 2.0 Contributors.
 * 
 * This file is part of TTC 2.0.
 * 
 * TTC 2.0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * TTC 2.0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with TTC 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Radio device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 0.5.1
 *
 * \date 2024/04/22
 *
 * \addtogroup radio
 * \{
 */

#include <FreeRTOS.h>
#include <task.h>

#include <system/sys_log/sys_log.h>
#include "drivers/dev_sx1262/sx1262.h"
#include <devices/leds/leds.h>
#include "radio.h"

sx1262_t dev_sx1262 = { 0 };

//todo: Lora Config
static int lora_config(){
    sx1262_hal_t sx1262_hal = { 0 };

    sx1262_hal.spi_init = sx126x_hal_spi_init;
    sx1262_hal.gpio_init = sx126x_hal_gpio_init;
    sx1262_hal.spi_transmit_receive = sx1262_hal_spi_transmit_receive;
    sx1262_hal.spi_cs_low = sx1262_hal_spi_cs_low;
    sx1262_hal.spi_cs_high = sx1262_hal_spi_cs_high;
    sx1262_hal.gpio_read_busy = sx1262_hal_gpio_read_busy;
    sx1262_hal.gpio_write_reset = sx1262_hal_gpio_write_reset;
    sx1262_hal.delay_ms = sx1262_hal_delay_ms;
    sx1262_hal.get_tick = sx1262_hal_get_tick;
    sx1262_hal.rx_callback = NULL;
    dev_sx1262.initialized = SX1262_BOOL_FALSE;

    sx1262_hal.spi_init();
    sx1262_hal.gpio_init();

//    chip_mode_t mode;
//    command_status_t status;
    sx1262_status_t ret = sx1262_init(&dev_sx1262, &sx1262_hal);

    //Calibra all components


//    sx1262_get_chip_status(&dev_sx1262, &mode, &status);
//
//    if (ret != SX1262_OK)
//    {
//        return -1;
//    }

    ret = sx1262_set_dio2_as_rf_switch_crtl(&dev_sx1262);

    if (ret != SX1262_OK)
    {
        return -1;
    }

    ret = sx1262_set_packet_type(&dev_sx1262, PACKET_TYPE_LORA);

    if (ret != SX1262_OK)
    {
        return -1;
    }

//    sx1262_calibrate(&dev_sx1262, 0x1F);
//
//    sx1262_calibrate_image(&dev_sx1262, 433000000);

    ret = sx1262_set_frequency(&dev_sx1262, 433000000);

    if (ret != SX1262_OK)
    {
        return -1;
    }

    ret = sx1262_set_stop_timer_on_sync_word(&dev_sx1262);

    if (ret != SX1262_OK)
    {
        return -1;
    }

    ret = sx1262_set_lora_modulation_params(&dev_sx1262, SX1262_LORA_SF_10,
                                            SX1262_LORA_BANDWIDTH_125_KHZ,
                                            SX1262_LORA_CR_4_5,
                                            SX1262_BOOL_FALSE);

    if (ret != SX1262_OK)
    {
        return -1;
    }

    ret = sx1262_set_pa_config(&dev_sx1262, 0x04, 0x07, 0, 0x01);

    if (ret != SX1262_OK)
    {
        return -1;
    }

    ret = sx1262_set_tx_params(&dev_sx1262, 0, SX1262_RAMP_TIME_200US);

    if (ret != SX1262_OK)
    {
        return -1;
    }

    ret = sx1262_set_lora_symbol_timeout(&dev_sx1262, 0x00);

    if (ret != SX1262_OK)
    {
        return -1;
    }

    //Correct settings
    ret = sx1262_set_lora_packet_params(&dev_sx1262, (uint16_t)10, SX1262_LORA_HEADER_EXPLICIT, (uint8_t)0xFF, SX1262_LORA_CRC_TYPE_ON, SX1262_BOOL_FALSE);

//Test settings
//    ret = sx1262_set_lora_packet_params(&dev_sx1262, (uint16_t) 10,
//                                        SX1262_LORA_HEADER_EXPLICIT,
//                                        (uint8_t) 0xFF,
//                                        SX1262_LORA_CRC_TYPE_OFF,
//                                        SX1262_BOOL_FALSE);

    if (ret != SX1262_OK)
    {
        return -1;
    }

    const uint16_t irq_mask = SX1262_IRQ_TX_DONE | SX1262_IRQ_RX_DONE
            | SX1262_IRQ_CRC_ERR | SX1262_IRQ_HEADER_ERR | SX1262_IRQ_TIMEOUT;

    const uint16_t dio1_mask = SX1262_IRQ_RX_DONE | SX1262_IRQ_TX_DONE
            | SX1262_IRQ_TIMEOUT;

    ret = sx1262_set_dio_irq_params(&dev_sx1262, irq_mask, dio1_mask);

    if (ret != SX1262_OK)
    {
        return -1;
    }

    ret = sx1262_set_fallback_mode(&dev_sx1262,
                                   SX1262_RX_TX_FALLBACK_MODE_STDBY_XOSC);

    if (ret != SX1262_OK)
    {
        return -1;
    }

    return 0;
}

int radio_init(void)
{

    if(lora_config() != 0)
        return -1;

//    sx1262_status_t ret = sx1262_set_rx(&dev_sx1262);
    sx1262_status_t ret = sx1262_lora_receive_it(&dev_sx1262);

    if (ret != SX1262_OK)
    {
        return -1;
    }

    return 0;

}

int radio_send(uint8_t *data, uint16_t len)
{
    int err = 0;

    sys_log_print_event_from_module(SYS_LOG_INFO, RADIO_MODULE_NAME,
                                    "Transmitting ");
    sys_log_print_uint(len);
    sys_log_print_msg(" byte(s)...");
    sys_log_new_line();
    sys_log_dump_hex(data, len);
    sys_log_new_line();

    led_set(LED_DOWNLINK);

    sx1262_status_t ret;

    ret = sx1262_lora_transmit(&dev_sx1262, data, len, 2000);

    if (ret != SX1262_OK)
    {
        led_clear(LED_DOWNLINK);
        err = -1;
    }

    ret = sx1262_set_rx(&dev_sx1262);

    if (ret != SX1262_OK)
    {
        err = -2;
    }

    return err;
}

int radio_recv(uint8_t *data, uint16_t len, uint32_t timeout_ms)
{

    //TODO: VERIFICAR PORQUE LEN ESTA COM 16 BITS

    sx1262_status_t ret = sx1262_handle_rx_done(&dev_sx1262, data, &len);

    if (ret != SX1262_OK)
    {
        return -1;
    }

    return len;
}

int radio_available(void)
{
    sx1262_irq_t irq;

    sx1262_status_t ret = sx1262_get_irq_status(&dev_sx1262, &irq);

    if (ret != SX1262_OK)
        return -1;

    if (irq & SX1262_IRQ_RX_DONE)
        return 0;

    return -1;
}

int radio_sleep(void)
{
    int err = -1;

    return err;
}

int radio_get_temperature(radio_temp_t *temp)
{
    /* TODO */
    return -1;
}

int radio_get_rssi(radio_rssi_t *rssi)
{
    //TODO: VERIFICAR SE O TIPO DO RSSI EH 8 OU 16 BITS
    sx1262_status_t ret = sx1262_get_packet_status(&dev_sx1262, NULL, rssi,
    NULL,
                                                   NULL);

    if (ret != SX1262_OK)
    {
        return -1;
    }

    return 0;
}

/** \} End of radio group */
