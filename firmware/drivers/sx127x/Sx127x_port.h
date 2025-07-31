/*
 * Sx127x_port.h
 *
 *  Created on: 9 de ago de 2024
 *      Author: SENAI
 */

#ifndef DRIVERS_SX127X_SX127X_PORT_H_
#define DRIVERS_SX127X_SX127X_PORT_H_


#include "sx127x.h"
#include "sx127x_private.h"
#include <stdbool.h>


extern sx127x_dev_t sx1278_phy;

int sx127x_radio_init(void);


/**
 * \brief Changes the radio state to RX.
 *
 * \return None.
 */
bool sx127x_rx_init(void);

/**
 * \brief Creates a mutex to use the sx127x chip.
 *
 * \return The status/error code.
 */
int sx127x_mutex_create(void);

/**
 * \brief Holds the resource (sx127x chip).
 *
 * \return The status/error code.
 */
int sx127x_mutex_take(void);

/**
 * \brief Frees the resource (sx127x chip).
 *
 * \return The status/error code.
 */
int sx127x_mutex_give(void);


#endif /* DRIVERS_SX127X_SX127X_PORT_H_ */
