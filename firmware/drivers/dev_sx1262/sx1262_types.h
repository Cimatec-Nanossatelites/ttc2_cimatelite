/*
 * sx1262_types.h
 *
 *  Created on: Jan 26, 2026
 *      Author: ramon.papes
 */

#ifndef LIBS_DEV_SX1262_SX1262_TYPES_H_
#define LIBS_DEV_SX1262_SX1262_TYPES_H_

/** @brief sx1262 boolean type enumeration definition
 */
typedef enum {
	SX1262_BOOL_FALSE = 0x00, /**< disable function */
	SX1262_BOOL_TRUE = 0x01, /**< enable function */
} sx1262_bool_t;

/**
 * @brief sx1262 configuration structure
 */
typedef struct {
	uint32_t frequency;
	uint8_t spreading_factor;
	uint8_t bandwidth;
	uint8_t coding_rate;
	uint8_t tx_power;
	uint16_t preamble_length;
	sx1262_bool_t crc_enabled;
	sx1262_bool_t implicit_header;
	sx1262_bool_t iq_inverted;
} sx1262_settings_t;

typedef enum {
	SX1262_OK = 0,
	SX1262_ERROR = 1,
	SX1262_BUSY = 2,
	SX1262_TIMEOUT = 3,
	SX1262_INVALID_PARAM = 4,
	SX1262_ERR_NOT_INITIALIZED = 5
} sx1262_status_t;

/**
 * chip status enum
 */
typedef enum {
	CHIP_MODE_SLEEP = 0x00,
	CHIP_MODE_RESERVED = 0x01,
	CHIP_MODE_STDBY_RC = 0x02,
	CHIP_MODE_STDBY_XOSC = 0x03,
	CHIP_MODE_FS = 0x04,
	CHIP_MODE_RX = 0x05,
	CHIP_MODE_TX = 0x06,
	CHIP_MODE_UNKNOWN = 0x07
} chip_mode_t;

/**
 * command status enum
 */
typedef enum {
	CMD_STATUS_RESERVED = 0x00,
	CMD_STATUS_RFU = 0X01,
	CMD_STATUS_DATA_AVAILABLE = 0x02,
	CMD_STATUS_CMD_TIMEOUT = 0x03,
	CMD_STATUS_CMD_PROCCESS_ERROR = 0x04,
	CMD_STATUS_CMD_FAILED = 0x05,
	CMD_STATUS_TX_DONE = 0x06,
} command_status_t;

/**
 * @brief packet type enum
 */
typedef enum {
	PACKET_TYPE_GFSK = 0x00, PACKET_TYPE_LORA = 0x01,
} packet_type_t;

/**
 * @brief sx1262 lora spreading factor enumeration definition
 */
typedef enum {
	SX1262_LORA_SF_5 = 0x05, /**< spreading factor 5 */
	SX1262_LORA_SF_6 = 0x06, /**< spreading factor 6 */
	SX1262_LORA_SF_7 = 0x07, /**< spreading factor 7 */
	SX1262_LORA_SF_8 = 0x08, /**< spreading factor 8 */
	SX1262_LORA_SF_9 = 0x09, /**< spreading factor 9 */
	SX1262_LORA_SF_10 = 0x0A, /**< spreading factor 10 */
	SX1262_LORA_SF_11 = 0x0B, /**< spreading factor 11 */
	SX1262_LORA_SF_12 = 0x0C, /**< spreading factor 12 */
} sx1262_lora_sf_t;

/**
 * @brief sx1262 lora bandwidth enumeration definition
 */
typedef enum {
	SX1262_LORA_BANDWIDTH_7P81_KHZ = 0x00, /**< 7.81 kHz */
	SX1262_LORA_BANDWIDTH_10P42_KHZ = 0x08, /**< 10.42 kHz */
	SX1262_LORA_BANDWIDTH_15P63_KHZ = 0x01, /**< 15.63 kHz */
	SX1262_LORA_BANDWIDTH_20P83_KHZ = 0x09, /**< 20.83 kHz */
	SX1262_LORA_BANDWIDTH_31P25_KHZ = 0x02, /**< 31.25 kHz */
	SX1262_LORA_BANDWIDTH_41P67_KHZ = 0x0A, /**< 41.67 kHz */
	SX1262_LORA_BANDWIDTH_62P50_KHZ = 0x03, /**< 62.50 kHz */
	SX1262_LORA_BANDWIDTH_125_KHZ = 0x04, /**< 125 kHz */
	SX1262_LORA_BANDWIDTH_250_KHZ = 0x05, /**< 250 kHz */
	SX1262_LORA_BANDWIDTH_500_KHZ = 0x06, /**< 500 kHz */
} sx1262_lora_bandwidth_t;

typedef enum {
	SX126X_DIO3_OUTPUT_1_6 = 0x00, /**< 1.6 V */
	SX126X_DIO3_OUTPUT_1_7 = 0x01, /**< 1.7 V */
	SX126X_DIO3_OUTPUT_1_8 = 0x02, /**< 1.8 V */
	SX126X_DIO3_OUTPUT_2_2 = 0x03, /**< 2.2 V */
	SX126X_DIO3_OUTPUT_2_4 = 0x04, /**< 2.4 V */
	SX126X_DIO3_OUTPUT_2_7 = 0x05, /**< 2.7 V */
	SX126X_DIO3_OUTPUT_3_0 = 0x06, /**< 3.0 V */
	SX126X_DIO3_OUTPUT_3_3 = 0x07, /**< 3.3 V */
} sx1262_dio3_output_t;

/**
 * @brief sx1262 lora coding rate enumeration definition
 */
typedef enum {
	SX1262_LORA_CR_4_5 = 0x01, /**< cr 4/5 */
	SX1262_LORA_CR_4_6 = 0x02, /**< cr 4/6 */
	SX1262_LORA_CR_4_7 = 0x03, /**< cr 4/7 */
	SX1262_LORA_CR_4_8 = 0x04, /**< cr 4/8 */
} sx1262_lora_cr_t;

typedef enum {
	SX1262_LORA_HEADER_EXPLICIT = 0x00, /**< explicit header */
	SX1262_LORA_HEADER_IMPLICIT = 0x01, /**< implicit header */
} sx1262_lora_header_t;

/**
 * @brief sx1262 lora crc type enumeration definition
 */
typedef enum {
	SX1262_LORA_CRC_TYPE_OFF = 0x00, /**< crc off */
	SX1262_LORA_CRC_TYPE_ON = 0x01, /**< crc on */
} sx1262_lora_crc_type_t;

/**
 * @brief sx1262 ramp time enumeration definition
 */
typedef enum {
	SX1262_RAMP_TIME_10US = 0x00, /**< 10us */
	SX1262_RAMP_TIME_20US = 0x01, /**< 20us */
	SX1262_RAMP_TIME_40US = 0x02, /**< 40us */
	SX1262_RAMP_TIME_80US = 0x03, /**< 80us */
	SX1262_RAMP_TIME_200US = 0x04, /**< 200us */
	SX1262_RAMP_TIME_800US = 0x05, /**< 800us */
	SX1262_RAMP_TIME_1700US = 0x06, /**< 1700us */
	SX1262_RAMP_TIME_3400US = 0x07, /**< 3400us */
} sx1262_ramp_time_t;

/**
 * @brief sx1262 rx tx fallback mode enumeration definition
 */
typedef enum {
	SX1262_RX_TX_FALLBACK_MODE_FS = 0x40, /**< the radio goes into fs mode after tx or rx */
	SX1262_RX_TX_FALLBACK_MODE_STDBY_XOSC = 0x30, /**< the radio goes into standby_xosc mode after tx or rx */
	SX1262_RX_TX_FALLBACK_MODE_STDBY_RC = 0x20, /**< the radio goes into standby_rc mode after tx or rx */
} sx1262_rx_tx_fallback_mode_t;

/**
 * @brief SX1262 interrupt flags enumeration
 * @note Based on SX126X_CMD_SET_DIO_IRQ_PARAMS defines
 * @note 16-bit mask for interrupt configuration
 */
typedef enum {
	/* Individual interrupt flags (bits 0-14) */
	SX1262_IRQ_NONE = SX126X_IRQ_NONE, /**< 0x0000 - No interrupts */
	SX1262_IRQ_TX_DONE = SX126X_IRQ_TX_DONE, /**< 0x0001 - Packet transmission completed (bit 0) */
	SX1262_IRQ_RX_DONE = SX126X_IRQ_RX_DONE, /**< 0x0002 - Packet received (bit 1) */
	SX1262_IRQ_PREAMBLE_DETECTED = SX126X_IRQ_PREAMBLE_DETECTED, /**< 0x0004 - Preamble detected (bit 2) */
	SX1262_IRQ_SYNC_WORD_VALID = SX126X_IRQ_SYNC_WORD_VALID, /**< 0x0008 - Valid sync word detected (bit 3) */
	SX1262_IRQ_HEADER_VALID = SX126X_IRQ_HEADER_VALID, /**< 0x0010 - Valid LoRa header received (bit 4) */
	SX1262_IRQ_HEADER_ERR = SX126X_IRQ_HEADER_ERR, /**< 0x0020 - LoRa header CRC error (bit 5) */
	SX1262_IRQ_CRC_ERR = SX126X_IRQ_CRC_ERR, /**< 0x0040 - Wrong CRC received (bit 6) */
	SX1262_IRQ_CAD_DONE = SX126X_IRQ_CAD_DONE, /**< 0x0080 - Channel activity detection finished (bit 7) */
	SX1262_IRQ_CAD_DETECTED = SX126X_IRQ_CAD_DETECTED,/**< 0x0100 - Channel activity detected (bit 8) */
	SX1262_IRQ_TIMEOUT = SX126X_IRQ_TIMEOUT, /**< 0x0200 - Rx or Tx timeout (bit 9) */
} sx1262_irq_t;

/**
 * @brief SX1262 TX done callback type
 */
typedef void (*sx1262_rx_done_cb_t)(uint8_t *data, uint8_t length);

/**
 * @brief SX1262 RX error callback type
 */
typedef void (*sx1262_rx_error_cb_t)(sx1262_status_t error);

/**
 * @brief SX1262 timeout callback type
 */
typedef void (*sx1262_timeout_cb_t)(void);

typedef void (*sx1262_tx_done_cb_t)(void);

/**
 * @brief SX1262 callback functions structure
 *
 */
typedef struct sx1262_callbacks {
	sx1262_rx_done_cb_t rx_done;
	sx1262_rx_error_cb_t rx_error;
	sx1262_timeout_cb_t timeout;
	sx1262_tx_done_cb_t tx_done;
} sx1262_callbacks_t;

/**
 * @brief SX1262 device handle structure
 *
 */
typedef struct sx1262_t{
	sx1262_hal_t hal;
	chip_mode_t mode;
	sx1262_settings_t config;
	sx1262_bool_t initialized;
	uint8_t in_receive_mode;
	sx1262_callbacks_t callbacks;
	uint8_t tx_buffer[SX126X_MAX_PACKET_LENGTH];
	uint8_t rx_buffer[SX126X_MAX_PACKET_LENGTH];
} sx1262_t;

#endif /* LIBS_DEV_SX1262_SX1262_TYPES_H_ */
