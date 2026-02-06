/*
 * gpio.h
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
 * along with TTC 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief GPIO driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.3.0
 * 
 * \date 2020/01/13
 * 
 * \defgroup gpio GPIO
 * \ingroup drivers
 * \{
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>
#include <stdbool.h>

#define GPIO_MODULE_NAME    "GPIO"

/* Pin states */
#define GPIO_STATE_LOW      0
#define GPIO_STATE_HIGH     1

/**
 * \brief GPIO pins.
 */
typedef enum
{
    GPIO_PIN_0  = 0,   /**< P1.0 */
    GPIO_PIN_1,        /**< P1.1 */
    GPIO_PIN_2,        /**< P1.2 */
    GPIO_PIN_3,        /**< P1.3 */
    GPIO_PIN_4,        /**< P1.4 */
    GPIO_PIN_5,        /**< P1.5 */
    GPIO_PIN_6,        /**< P1.6 */
    GPIO_PIN_7,        /**< P1.7 */

    GPIO_PIN_8,        /**< P2.0 */
    GPIO_PIN_9,        /**< P2.1 */
    GPIO_PIN_10,       /**< P2.2 */
    GPIO_PIN_11,       /**< P2.3 */
    GPIO_PIN_12,       /**< P2.4 */
    GPIO_PIN_13,       /**< P2.5 */
    GPIO_PIN_14,       /**< P2.6 */
    GPIO_PIN_15,       /**< P2.7 */

    GPIO_PIN_16,       /**< P3.0 */
    GPIO_PIN_17,       /**< P3.1 */
    GPIO_PIN_18,       /**< P3.2 */
    GPIO_PIN_19,       /**< P3.3 */
    GPIO_PIN_20,       /**< P3.4 */
    GPIO_PIN_21,       /**< P3.5 */
    GPIO_PIN_22,       /**< P3.6 */
    GPIO_PIN_23,       /**< P3.7 */

    GPIO_PIN_24,       /**< P4.0 */
    GPIO_PIN_25,       /**< P4.1 */
    GPIO_PIN_26,       /**< P4.2 */
    GPIO_PIN_27,       /**< P4.3 */
    GPIO_PIN_28,       /**< P4.4 */
    GPIO_PIN_29,       /**< P4.5 */
    GPIO_PIN_30,       /**< P4.6 */
    GPIO_PIN_31,       /**< P4.7 */

    GPIO_PIN_32,       /**< P5.0 */
    GPIO_PIN_33,       /**< P5.1 */
    GPIO_PIN_34,       /**< P5.2 */
    GPIO_PIN_35,       /**< P5.3 */
    GPIO_PIN_36,       /**< P5.4 */
    GPIO_PIN_37,       /**< P5.5 */
    GPIO_PIN_38,       /**< P5.6 */
    GPIO_PIN_39,       /**< P5.7 */

    GPIO_PIN_40,       /**< P6.0 */
    GPIO_PIN_41,       /**< P6.1 */
    GPIO_PIN_42,       /**< P6.2 */
    GPIO_PIN_43,       /**< P6.3 */
    GPIO_PIN_44,       /**< P6.4 */
    GPIO_PIN_45,       /**< P6.5 */
    GPIO_PIN_46,       /**< P6.6 */
    GPIO_PIN_47,       /**< P6.7 */

    GPIO_PIN_48,       /**< P7.2 */
    GPIO_PIN_49,       /**< P7.3 */
    GPIO_PIN_50,       /**< P7.4 */
    GPIO_PIN_51,       /**< P7.5 */
    GPIO_PIN_52,       /**< P7.6 */
    GPIO_PIN_53,       /**< P7.7 */

    GPIO_PIN_54,       /**< P8.0 */
    GPIO_PIN_55,       /**< P8.1 */
    GPIO_PIN_56,       /**< P8.2 */
    GPIO_PIN_57,       /**< P8.3 */
    GPIO_PIN_58,       /**< P8.4 */
    GPIO_PIN_59,       /**< P8.5 */
    GPIO_PIN_60,       /**< P8.6 */
    GPIO_PIN_61,       /**< P8.7 */

    GPIO_PIN_62,       /**< P9.0 */
    GPIO_PIN_63,       /**< P9.1 */
    GPIO_PIN_64,       /**< P9.2 */
    GPIO_PIN_65,       /**< P9.3 */
    GPIO_PIN_66,       /**< P9.4 */
    GPIO_PIN_67,       /**< P9.5 */
    GPIO_PIN_68,       /**< P9.6 */
    GPIO_PIN_69        /**< P9.7 */
} gpio_pin_t;

/**
 * \brief GPIO modes.
 */
typedef enum
{
    GPIO_MODE_OUTPUT=0,     /**< GPIO as output. */
    GPIO_MODE_INPUT         /**< GPIO as input. */
} gpio_mode_t;

/**
 * \brief GPIO pin configuration.
 */
typedef struct
{
    gpio_mode_t mode;
} gpio_config_t;

/**
 * \brief Initialization routine of a GPIO pin.
 *
 * \param[in] pin is the GPIO pin to initialize.
 *
 * \param[in] config is the configuration to initialize the GPIO pin.
 *
 * \return The status/error code.
 */
int gpio_init(gpio_pin_t pin, gpio_config_t config);

/**
 * \brief Sets the state of a given output GPIO pin.
 *
 * \param[in] pin is the GPIO pin to set the state.
 *
 * \param[in] level is the new state of the given GPIO pin. It can be:
 * \parblock
 *      - GPIO_STATE_HIGH
 *      - GPIO_STATE_LOW
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int gpio_set_state(gpio_pin_t pin, bool state);

/**
 * \brief Gets the state of a given input GPIO pin.
 *
 * \param[in] pin is the the GPIO pin to get the state.
 *
 * \return The GPIO pin state. It can be:
 * \parblock
 *      - GPIO_STATE_HIGH
 *      - GPIO_STATE_LOW
 *      - -1 on error
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int gpio_get_state(gpio_pin_t pin);

/**
 * \brief Toggles the state of a GPIO pin.
 *
 * \param[in] pin is the the GPIO pin to get the state.
 *
 * \return The status/error code.
 */
int gpio_toggle(gpio_pin_t pin);

/**
 * \brief Configures the MR GPIO pin to initialize in high as output.
 *
 * Adaptation from the hal/gpio function, this function must be used to configure the
 * external watchdog MR pin as output and avoid accidental resets during configuration.
 *
 * \see SLVS165L - TPS382x Voltage Monitor With Watchdog Timer.
 *
 * \param[in] mr_pin is the watchdog Manual Reset pin.
 *
 * \return status/error code.
 */
int gpio_init_mr_pin(gpio_pin_t mr_pin);

#endif /* GPIO_H_ */

/** \} End of gpio group */
