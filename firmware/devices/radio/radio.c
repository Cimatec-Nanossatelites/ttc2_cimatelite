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
#include <drivers/si446x/si446x.h>
#include <drivers/sx127x/Sx127x_port.h>
#include <devices/leds/leds.h>
#include "radio.h"

int radio_init(void)
{
    int err = -1;
    sys_log_print_event_from_module(SYS_LOG_INFO, RADIO_MODULE_NAME,
                                    "Initializing radio device...");
    sys_log_new_line();

    if (sx127x_radio_init() == 0)
    {
        if (sx127x_rx_init() == true)
        {
            err = 0;
        }
    }

    return err;
}

int radio_send(uint8_t *data, uint16_t len)
{
    int err = -1;

    sys_log_print_event_from_module(SYS_LOG_INFO, RADIO_MODULE_NAME,
                                    "Transmitting ");
    sys_log_print_uint(len);
    sys_log_print_msg(" byte(s)...");
    sys_log_new_line();
    sys_log_dump_hex(data, len);
    sys_log_new_line();

    led_set(LED_DOWNLINK);

    if (sx127x_transmit(&sx1278_phy, data, len, 2000))
    {
        led_clear(LED_DOWNLINK);

        if (sx127x_rx_init() == true)
        {
            err = 0;
        }
    }

    sx127x_rx_init();

    return err;
}

int radio_recv(uint8_t *data, uint16_t len, uint32_t timeout_ms)
{
    int res = 0;

    uint16_t i = 0;

    uint8_t irq = sx127x_get_irq_flags(&sx1278_phy);

    if (irq & FlagRxDone)
    {
        uint8_t size = sx127x_get_last_packet_size(&sx1278_phy);

        if (size > len)
            return -1;

        res = size;

        sx127x_read_fifo(&sx1278_phy, data, size);

        sx127x_clear_irq_flags(&sx1278_phy, irq);

        sx127x_rx_init();
    }

    return res;
}

int radio_available(void)
{
    uint8_t irq = sx127x_get_irq_flags(&sx1278_phy);
    if(irq & FlagRxDone)
        return 0;
    return -1;

//    return si446x_gpio_get_pin(SI446X_GPIO_NIRQ);//TODO: SUBSTITUIR PARA A VERIFICAÇÃO DO SX127X
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
    return sx127x_get_last_packet_rssi(&sx1278_phy);
}

/** \} End of radio group */
