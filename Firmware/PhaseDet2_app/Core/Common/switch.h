/**
  * @file       switch.h
  * @version    $(APP_VERSION)
  * @date       $(RELEASE_DATE)
  * @brief      DIP switch driver
  * @author     radek.salom
  *
  * @copyright  Logic Elements Copyright
  *
  * @defgroup grSwitch
  * @{
  * @brief Driver to handle DIP switches
  *
  * This module contains
  *
  * @par Main features:
  *
  * @par Example
  * @code
  *
  * @endcode
  */
#ifndef SWITCH_H_
#define SWITCH_H_

/* Includes ------------------------------------------------------------------*/

#include "common.h"


/* Definitions----------------------------------------------------------------*/

#define SWITCH_NUMBER           4          ///< Number of supported DIP switches

#define SWITCH_1_PORT     GPIOB
#define SWITCH_1_PIN      GPIO_PIN_15

#define SWITCH_2_PORT     GPIOB
#define SWITCH_2_PIN      GPIO_PIN_14

#define SWITCH_3_PORT     GPIOB
#define SWITCH_3_PIN      GPIO_PIN_13

#define SWITCH_4_PORT     GPIOB
#define SWITCH_4_PIN      GPIO_PIN_12


/* Typedefs-------------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

/**
 * Initialize DIP Switch module
 *
 * @param none
 * @return void
 */
void Switch_Init(void);

/**
 * Get values of all dip switches in a single number
 * @return
 */
uint16_t Switch_GetAll(void);

/**
 * Get value of bootstrap pins
 * @return 4-bit value of bootstrap pins
 */
uint16_t Switch_GetBootstrap(void);

/**
 * Get value of DIP-switch address pins
 * @return 5-bit address value
 */
uint16_t Switch_GetAddress(void);

/**
 * Get value of sample time of button pressed
 * @return Value of button pressed
 */
uint32_t Switch_GetPressed(void);

/**
 * Get value of sample time when button is released
 * @return Value of button released
 */
uint32_t Switch_GetReleased(void);

#endif /* SWITCH_H_ */
/** @} */
