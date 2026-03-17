/*
 * uplink_manager.c
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
 * \brief Uplink Manager task implementation.
 *
 * \author Miguel Boing <miguelboing13@gmail.com>
 *
 * \version 1.0.0
 *
 * \date 2024/09/09
 *
 * \addtogroup uplink_manager
 * \{
 */

#include <system/sys_log/sys_log.h>
#include <devices/radio/radio.h>
#include <structs/ttc_data.h>
#include <ngham/ngham.h>

#include "uplink_manager.h"
#include "startup.h"

#include <drivers/si446x/si446x.h>

#define PKT_ID_UPLINK_PCD_TRANSMIT_PAYLOAD 0x55

xTaskHandle xTaskUplinkManagerHandle;

typedef struct
{
    uint32_t pkt_id;
    uint32_t timestamp;
    uint16_t battery;
    uint32_t wind_speed; //todo: Deve ser float
    uint16_t wind_direction;
    uint32_t rainfall; //todo: Deve ser float
    uint16_t ground_humidity;
    uint16_t humidity;
    int16_t temperature;
    uint16_t co2;
} PCD_data_T;

void vTaskUplinkManager(void)
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE,
                        pdMS_TO_TICKS(TASK_UPLINK_MANAGER_INIT_TIMEOUT_MS));

    /* Delay before the first cycle */
    vTaskDelay(pdMS_TO_TICKS(TASK_UPLINK_MANAGER_INITIAL_DELAY_MS));

    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_UPLINK_MANAGER_NAME,
                                    "Initializing the Uplink Manager...");
    sys_log_new_line();

    ttc_data_buf.radio.rx_fifo_counter = 0U;
    ttc_data_buf.radio.rx_packet_counter = 0U;
    ttc_data_buf.radio.last_rx_packet_bytes = 0U;
    ttc_data_buf.radio.rssi = 0U;

    ttc_data_buf.up_buf.position_to_read = 0U;
    ttc_data_buf.up_buf.position_to_write = 0U;

    uint8_t rx_packet[230] = { 0 };
    uint8_t ngham_decoded_packet[220] = { 0 };
    uint16_t ngham_decoded_packet_len = 0;

    ttc_data_buf.n_conseq_failed_packets = 0U;

    while (1)
    {
        TickType_t last_cycle = xTaskGetTickCount();

        if (radio_available() == 0U)
        {
            sys_log_print_event_from_module(SYS_LOG_INFO,
            TASK_UPLINK_MANAGER_NAME,
                                            "Receiving a new package:");
            sys_log_new_line();

            if (radio_recv(rx_packet, 220U, 100U) > 0)
            {
//                if (radio_get_rssi(&ttc_data_buf.radio.rssi) == 0)
//                {
//                    sys_log_print_event_from_module(SYS_LOG_INFO,  TASK_UPLINK_MANAGER_NAME, "Latched RSSI is ");
//                    sys_log_print_uint((uint32_t)ttc_data_buf.radio.rssi);
//                    sys_log_print_msg(" dBm");
//                    sys_log_new_line();
//                }
//                else
//                {
//                    sys_log_print_event_from_module(SYS_LOG_ERROR,  TASK_UPLINK_MANAGER_NAME, "Failed to obtain the radio RSSI");
//                    sys_log_new_line();
//                }

                sys_log_print_event_from_module(SYS_LOG_INFO,
                TASK_UPLINK_MANAGER_NAME,
                                                "Decoding packet...");
                sys_log_new_line();

                if (ngham_decode(rx_packet, 220, ngham_decoded_packet,
                                 &ngham_decoded_packet_len) == 0)
                {

                    uplink_add_packet(ngham_decoded_packet,
                                      ngham_decoded_packet_len);

                    sys_log_print_event_from_module(
                            SYS_LOG_INFO, TASK_UPLINK_MANAGER_NAME,
                            "Recebido: ");
                        sys_log_new_line();
                    sys_log_dump_hex(ngham_decoded_packet, ngham_decoded_packet_len);
                    sys_log_new_line();

                    PCD_data_T payload = { 0 };
                    memcpy(&payload, &ngham_decoded_packet[1],
                           sizeof(PCD_data_T));
                    ttc_data_buf.n_conseq_failed_packets = 0U;

                    sys_log_print_event_from_module(
                            SYS_LOG_INFO, TASK_UPLINK_MANAGER_NAME,
                            "Packet successfully received.");
                    sys_log_new_line();

                    sys_log_print_event_from_module(SYS_LOG_INFO,
                    TASK_UPLINK_MANAGER_NAME,
                                                    "Bytes recebidos: ");
                    sys_log_print_uint(ngham_decoded_packet_len);
                    sys_log_new_line();

                    if (ngham_decoded_packet[0]
                            == PKT_ID_UPLINK_PCD_TRANSMIT_PAYLOAD)
                    {
                        sys_log_print_event_from_module(
                                SYS_LOG_INFO, TASK_UPLINK_MANAGER_NAME,
                                "PCD Packet Received");
                        sys_log_new_line();

                        sys_log_print_event_from_module(
                                SYS_LOG_INFO, TASK_UPLINK_MANAGER_NAME, "ID: ");
                        sys_log_print_uint(payload.pkt_id);
                        sys_log_new_line();

                        sys_log_print_event_from_module(
                                SYS_LOG_INFO, TASK_UPLINK_MANAGER_NAME,
                                "Timestamp: ");
                        sys_log_print_uint(payload.timestamp);
                        sys_log_new_line();

                        sys_log_print_event_from_module(
                                SYS_LOG_INFO, TASK_UPLINK_MANAGER_NAME,
                                "Battery: ");
                        sys_log_print_uint(payload.battery);
                        sys_log_new_line();

                        sys_log_print_event_from_module(
                                SYS_LOG_INFO, TASK_UPLINK_MANAGER_NAME,
                                "Humidity: ");
                        sys_log_print_uint(payload.humidity);
                        sys_log_new_line();

                        sys_log_print_event_from_module(
                                SYS_LOG_INFO, TASK_UPLINK_MANAGER_NAME,
                                "Precipitation: ");
                        sys_log_print_uint(payload.rainfall);
                        sys_log_new_line();

                        sys_log_print_event_from_module(
                                SYS_LOG_INFO, TASK_UPLINK_MANAGER_NAME,
                                "Ground Humidity: ");
                        sys_log_print_uint(payload.ground_humidity);
                        sys_log_new_line();

                        sys_log_print_event_from_module(
                                SYS_LOG_INFO, TASK_UPLINK_MANAGER_NAME,
                                "Temperature: ");
                        sys_log_print_uint(payload.temperature);
                        sys_log_new_line();

                        sys_log_print_event_from_module(
                                SYS_LOG_INFO, TASK_UPLINK_MANAGER_NAME,
                                "Wind Direction: ");
                        sys_log_print_uint(payload.wind_direction);
                        sys_log_new_line();

                        sys_log_print_event_from_module(
                                SYS_LOG_INFO, TASK_UPLINK_MANAGER_NAME,
                                "Wind Speed: ");
                        sys_log_print_uint(payload.wind_speed);
                        sys_log_new_line();

                        sys_log_print_event_from_module(
                                SYS_LOG_INFO, TASK_UPLINK_MANAGER_NAME,
                                "CO2: ");
                        sys_log_print_uint(payload.co2);
                        sys_log_new_line();
                    }
                }
                else
                {
//                    (void)radio_reset(); // TODO: radio_reset nao existia no firmware antigo, deve ser implementado ?
                    ttc_data_buf.n_conseq_failed_packets++;

                    sys_log_print_event_from_module(
                            SYS_LOG_ERROR, TASK_UPLINK_MANAGER_NAME,
                            "Failed to decode a new packet.");
                    sys_log_new_line();
                    sys_log_print_event_from_module(
                            SYS_LOG_ERROR, TASK_UPLINK_MANAGER_NAME,
                            "The number of consecutive failed packages is: ");
                    sys_log_print_uint(
                            (uint32_t) ttc_data_buf.n_conseq_failed_packets);
                    sys_log_new_line();
                }
            }
            else
            {
//                (void)radio_reset();
                ttc_data_buf.n_conseq_failed_packets++;

                sys_log_print_event_from_module(
                        SYS_LOG_ERROR, TASK_UPLINK_MANAGER_NAME,
                        "Failed to receive a new packet.");
                sys_log_new_line();
                sys_log_print_event_from_module(
                        SYS_LOG_ERROR, TASK_UPLINK_MANAGER_NAME,
                        "The number of consecutive failed packages is: ");
                sys_log_print_uint(
                        (uint32_t) ttc_data_buf.n_conseq_failed_packets);
                sys_log_new_line();
            }
        }

        vTaskDelayUntil(&last_cycle,
                        pdMS_TO_TICKS(TASK_UPLINK_MANAGER_PERIOD_MS));
    }
}

/** \} End of uplink_manager group */
