/*
 * sx1262.h
 *
 *  Created on: Jan 15, 2026
 *      Author: ramon.papes
 */

#ifndef LIBS_DEV_SX1262_SX1262_H_
#define LIBS_DEV_SX1262_SX1262_H_

#include "sx1262_defines.h"
#include "sx1262_hal.h"
#include "sx1262_types.h"
#include "FreeRTOS.h"
#include "task.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief SX1262 driver functions
 * @defgroup sx1262_driver_functions SX1262 driver functions
 * @{
 */

/**
 * @brief Reset sx1262 device
 * @param[in] dev pointer to an sx1262 handle structure
 * @note    none
 */
sx1262_status_t sx1262_reset(sx1262_t *dev);

/**
 * @brief sx1262 init function
 * @param[in] dev pointer to an sx1262 handle structure
 * @param[in] hal pointer to an sx1262 hal structure
 * @note    none
 */
sx1262_status_t sx1262_init(sx1262_t *dev, const sx1262_hal_t *hal);


/**
 * @brief Set device to sleep mode
 * @param[in] dev Pointer to device handle
 * @param[in] output DIO3 output voltage
 * @return SX1262_OK on success | error code
 */

sx1262_status_t sx1262_set_dio3_as_tcxo_ctrl(sx1262_t *dev,sx1262_dio3_output_t output);

//sx1262_status_t sx1262_set_mode_sleep(sx1262_t *dev);

/**
 * @brief Set device to standby mode
 * @param[in] dev Pointer to device handle
 * @return SX1262_OK on success | error code
 */
sx1262_status_t sx1262_set_mode_standby(sx1262_t *dev);

/**
 * @brief Set device to receive mode
 * @param[in] dev Pointer to device handle
 * @return SX1262_OK on success | error code
 */
sx1262_status_t sx1262_lora_receive_it(sx1262_t *dev);

/**
 * @brief Set device to TX mode
 * @param[in] dev Device handle
 * @return SX1262_OK on success | error code
 */
sx1262_status_t sx1262_set_tx(sx1262_t *dev);

/**
 * @brief Set device to RX mode
 * @param[in] dev Device handle
 * @return SX1262_OK on success | error code
 */
sx1262_status_t sx1262_set_rx(sx1262_t *dev);

/**
 * @brief Set RX/TX fallback mode
 * @param[in] dev Device handle
 * @param[in] mode Fallback mode
 */
sx1262_status_t sx1262_set_fallback_mode(sx1262_t *dev,
		sx1262_rx_tx_fallback_mode_t mode);

/**
 * @brief Set sync word
 * @param[in] dev Pointer to device handle
 * @param[in] sync_word Sync word to set
 * @return SX1262_OK on success | error code
 */
sx1262_status_t sx1262_get_lora_sync_word(sx1262_t *dev, uint16_t *sync_word);

/**
 * @brief Set LoRa sync word
 * @param[in] dev Device handle
 * @param[in] sync_word 16-bit sync word
 * @return SX1262_OK on success | error code
 */
sx1262_status_t sx1262_set_lora_sync_word(sx1262_t *dev, uint16_t sync_word);

/**
 * @brief Set packet type
 * @param[in] dev Pointer to device handle
 * @param[in] packet_type Packet type to set
 * @return SX1262_OK on success | error code
 */
sx1262_status_t sx1262_set_packet_type(sx1262_t *dev, packet_type_t pkt_type);

//TODO: TESTING
/**
 * @brief Get packet type
 * @param[in] dev Pointer to device handle
 * @param[out] pkt_type Pointer to packet type variable
 * @return SX1262_OK on success | error code
 */
sx1262_status_t sx1262_get_packet_type(sx1262_t *dev, packet_type_t *pkt_type);

/**
 * @brief Get chip status
 * @param[in] dev Pointer to device handle
 * @param[out] mode Pointer to chip mode variable
 * @param[out] cmd_status Pointer to command status variable
 * @return chip status
 */
sx1262_status_t sx1262_get_chip_status(sx1262_t *dev, chip_mode_t *mode,
		command_status_t *cmd_status);

/**
 * @brief Set RF frequency
 * @param[in] dev Device handle
 * @param[in] frequency_hz Frequency in Hz
 *
 */
sx1262_status_t sx1262_set_frequency(sx1262_t *dev, uint32_t frequency);
/**
 * @brief Set LoRa modulation parameters
 * @param[in] dev Device handle
 * @param[in] spreading_factor SF7 to SF12
 * @param[in] bandwidth Bandwidth index
 * @param[in] coding_rate Coding rate (1=4/5, 2=4/6, 3=4/7, 4=4/8)
 * @param[in] low_data_rate_optimize Enable for SF11 and SF12
 */
sx1262_status_t sx1262_set_lora_modulation_params(sx1262_t *dev,
		sx1262_lora_sf_t spreading_factor, sx1262_lora_bandwidth_t bandwidth,
		sx1262_lora_cr_t coding_rate, sx1262_bool_t low_data_rate_optimize);

/**
 * @brief Set LoRa packet parameters
 * @param[in] dev Device handle
 * @param[in] preamble_length Preamble length in symbols
 * @param[in] implicit_header Enable implicit header mode
 * @param[in] payload_length Payload length (0-255)
 * @param[in] crc_enable Enable CRC
 * @param[in] invert_iq Invert IQ (for LoRaWAN)
 */
sx1262_status_t sx1262_set_lora_packet_params(sx1262_t *dev,
		uint16_t preamble_length, sx1262_lora_header_t implicit_header,
		uint8_t payload_length, sx1262_lora_crc_type_t crc_enable,
		sx1262_bool_t invert_iq);

/**
 * @brief Set buffer base addresses
 * @param[in] dev Device handle
 * @param[in] tx_base_addr TX buffer base address (usually 0x00)
 * @param[in] rx_base_addr RX buffer base address (usually 0x00)
 */
sx1262_status_t sx1262_set_buffer_base_address(sx1262_t *dev,
		uint8_t tx_base_addr, uint8_t rx_base_addr);

/**
 * @brief Configure DIO interrupts
 * @param[in] dev Device handle
 * @param[in] irq_mask Mask of interrupts to enable
 * @param[in] dio1_mask Interrupts to map to DIO1 pin
 */
sx1262_status_t sx1262_set_dio_irq_params(sx1262_t *dev, uint16_t irq_mask,
		uint16_t dio1_mask);

/**
 * @brief Set DIO2 as RF switch control
 * @param[in] dev Device handle
 * @return SX1262_OK on success | error code
 * @note This function configures DIO2 pin to control the RF switch
 * for TX/RX switching.
 */
sx1262_status_t sx1262_set_dio2_as_rf_switch_crtl(sx1262_t *dev);

/**
 * @brief Disable stop timer on sync word detection
 * @param[in] dev Device handle
 * @return SX1262_OK on success | error code
 * @note This function disables the stop timer on sync word detection.
 */
sx1262_status_t sx1262_set_stop_timer_on_sync_word(sx1262_t *dev);

/**
 * @brief Set PA configuration
 * @param[in] dev Device handle
 * @param[in] duty_cycle PA duty cycle
 * @param[in] hp_max Max power (0x00-0x07)
 * @param[in] device_select 0x00=SX1262, 0x01=SX1261
 * @param[in] paLut Reserved, always 1
 */
sx1262_status_t sx1262_set_pa_config(sx1262_t *dev, uint8_t duty_cycle,
		uint8_t hp_max, uint8_t device_select, uint8_t paLut);

/**
 * @brief Calibrate the device
 * @param[in] dev Device handle
 * @param[in] calib_param Calibration parameter (bitmask)
 * @return SX1262_OK on success | error code
 * @note This function performs calibration based on the provided bitmask parameter.
 * The calib_param can be a combination of the following bits:
 * - 0x01: Calibrate image
 * - 0x02: Calibrate PLL
 * - 0x04: Calibrate ADC
 * - 0x08: Calibrate RC oscillator
 * - 0x10: Calibrate all (image + PLL + ADC + RC)
 * Example usage:
 * sx1262_calibrate(dev, 0x01); // Calibrate image only
 * sx1262_calibrate(dev, 0x1F); // Calibrate all components
 */
sx1262_status_t sx1262_calibrate(sx1262_t *dev, uint8_t calib_param);

/**
 * @brief Calibrate image
 * @param[in] dev Device handle
 * @param[in] freq1 Frequency 1
 * @param[in] freq2 Frequency 2
 * @return SX1262_OK on success | error code
 * @note This function performs image calibration for the given frequencies.
 */
sx1262_status_t sx1262_calibrate_image(sx1262_t *dev, uint32_t freq_hz);

/**
 * @brief Set TX parameters
 * @param[in] dev Device handle
 * @param[in] power TX power (-17 to +22 dBm)
 * @param[in] rampTime Ramp time (sx1262_ramp_time_t)
 */
sx1262_status_t sx1262_set_tx_params(sx1262_t *dev, int8_t power,
		sx1262_ramp_time_t rampTime);

/**
 * @brief Set LoRa symbol timeout
 * @param[in] dev Device handle
 * @param[in] symb_num Number of symbols for timeout
 * @return SX1262_OK on success | error code
 * @note This function sets the number of symbols after which
 * the RX timeout occurs in LoRa mode.
 */
sx1262_status_t sx1262_set_lora_symbol_timeout(sx1262_t *dev, uint8_t symb_num);

//TODO: Testing
/**
 * @brief Set LoRa CAD parameters
 * @param[in] dev Device handle
 * @param[in] symbol_num Number of symbols for CAD
 * @param[in] det_peak Detection peak
 * @param[in] det_min Detection minimum
 * @param[in] exit_mode CAD exit mode
 * @return SX1262_OK on success | error code
 */
sx1262_status_t sx1262_set_lora_cad_params(sx1262_t *dev);

/**
 * @brief Get current IRQ status from device
 * @param[in] dev Device handle
 * @param[out] irq_status 16-bit IRQ status mask
 * @return SX1262_OK on success
 */
sx1262_status_t sx1262_get_irq_status(sx1262_t *dev, uint16_t *irq_status);

/**
 * @brief Clear IRQ status flags
 * @param[in] dev Device handle
 * @return SX1262_OK on success
 */
sx1262_status_t sx1262_clear_irq_status(sx1262_t *dev);

/**
 * @brief SX1262 IRQ handler
 * @param[in] dev Device handle
 * @return SX1262_OK on success
 */
sx1262_status_t sx1262_irq_handler(sx1262_t *dev);

/**
 * @brief Get packet status (RSSI, SNR)
 * @param[in] dev Device handle
 * @param[out] status Pointer to packet status byte
 * @param[out] rssi Pointer to RSSI value
 * @param[out] snr Pointer to SNR value
 * @param[out] signal_rssi Pointer to signal RSSI value
 * @return SX1262_OK on success
 */
sx1262_status_t sx1262_get_packet_status(sx1262_t *dev, uint8_t *status, uint8_t *rssi, uint8_t *snr, uint8_t *signal_rssi);

/**
 * @brief Handle RX done event
 * @param[in] dev Device handle
 * @param[out] buf Buffer to store received data
 * @param[out] len Length of received data
 * @return SX1262_OK on success
 */
sx1262_status_t sx1262_handle_rx_done(sx1262_t *dev, uint8_t *buf, uint8_t *len);

/**
 * @brief Handle TX done event
 * @param[in] dev Device handle
 * @param[in] status Status of the transmission
 * @return SX1262_OK on success
 */
sx1262_status_t sx1262_handle_tx_done(sx1262_t *dev);

//TODO: TESTING
/**
 * @brief Transmit data in LoRa mode (interrupt-based)
 * @param[in] dev Device handle
 * @param[in] data Data to transmit
 * @param[in] length Length of data
 * @return SX1262_OK on success
 */
sx1262_status_t sx1262_lora_transmit(sx1262_t *dev, uint8_t *data, uint8_t length, uint32_t timeout_ms);

//TODO: TESTING
/**
 * @brief Transmit data in interrupt mode
 * @param[in] dev Device handle
 * @param[in] data Data buffer to transmit
 * @param[in] length Length of data to transmit
 * @return SX1262_OK on success
 */
sx1262_status_t sx1262_lora_transmit_it(sx1262_t *dev, uint8_t *data, uint8_t length);

/**
 * @brief Receive single packet
 * @param[in] dev Device handle
 * @param[out] buf Buffer to store received data
 * @param[out] len Length of received data
 * @return SX1262_OK on success
 */
sx1262_status_t sx1262_lora_receive(sx1262_t *dev, uint8_t *buf, uint8_t *len, uint32_t timeout);

/**
 * @brief Write data to buffer
 * @param[in] dev Device handle
 * @param[in] offset Offset in buffer to start writing
 * @param[in] data Data to write
 * @param[in] len Length of data to write
 * @return SX1262_OK on success | error code
 */
sx1262_status_t sx1262_write_buffer(sx1262_t *dev, uint8_t offset, const uint8_t *data, uint8_t len);

/**
 * @brief Get RX buffer status
 * @param[in] dev Device handle
 * @param[out] payload_len Pointer to payload length variable
 * @param[out] start_addr Pointer to start address variable
 * @return SX1262_OK on success | error code
 */
sx1262_status_t sx1262_get_rx_buffer_status(sx1262_t *dev, uint8_t *payload_length, uint8_t *rx_start_buffer_pointer);

/**
 * @brief Read data from buffer
 * @param[in] dev Device handle
 * @param[in] offset Offset in buffer to start reading
 * @param[out] data Buffer to store read data
 * @param[in] len Length of data to read
 * @return SX1262_OK on success | error code
 */
sx1262_status_t sx1262_read_buffer(sx1262_t *dev, uint8_t offset, uint8_t *data, uint8_t len);

//TODO: TESTING
/**
 * @brief Get RSSI instantaneous value
 * @param[in] dev Device handle
 * @param[out] rssi Pointer to RSSI variable
 * @return SX1262_OK on success | error code
 */
sx1262_status_t sx1262_get_rssi_inst(sx1262_t *dev, int8_t *rssi);

/**
 * @brief Set TX done callback function
 * @param[in] dev Device handle
 * @param[in] callback Pointer to TX done callback function
 */
void sx1262_set_tx_done_callback(sx1262_t *dev, sx1262_tx_done_cb_t callback);

/**
 * @brief Set RX done callback function
 * @param[in] dev Device handle
 * @param[in] callback Pointer to RX done callback function
 */
void sx1262_set_rx_done_callback(sx1262_t *dev, sx1262_rx_done_cb_t callback);

/**
 * @brief Set RX error callback function
 * @param[in] dev Device handle
 * @param[in] callback Pointer to RX error callback function
 */
void sx1262_set_rx_error_callback(sx1262_t *dev, sx1262_rx_error_cb_t callback);

/**
 * @brief Set timeout callback function
 * @param[in] dev Device handle
 * @param[in] callback Pointer to timeout callback function
 */
void sx1262_set_timeout_callback(sx1262_t *dev, sx1262_timeout_cb_t callback);

/**
 * @}
 * end of sx1262_driver_functions
 */
#ifdef __cplusplus
}
#endif

#endif /* LIBS_DEV_SX1262_SX1262_H_ */
