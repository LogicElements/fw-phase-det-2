/**
 * @file      system_msp.h
 * @brief     Microcontroller specific function module
 *
 * @author    richard.linhart@logicelements.cz
 * @date      Aug 2022
 * @copyright Logic Elements, s.r.o.
 *
 * @addtogroup grSystemMsp
 * @{
 */

#ifndef SYSTEM_MSP_H_
#define SYSTEM_MSP_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "common.h"

/* Definitions----------------------------------------------------------------*/
/* Typedefs-------------------------------------------------------------------*/

/**
 * General system pointer to function type
 */
typedef  void (*System_Callback_t)(void);

/* Functions -----------------------------------------------------------------*/

/**
 * @brief Perform a systen RESET
 */
extern void System_Reset(void);

/**
 * @brief Refresh the system watchdog
 */
extern Status_t System_ReloadWdg(void);

/**
 * @brief Start application at specified address
 *
 * This function is used by bootloaders and reset managers to jump to
 * an application image located at the specified address.
 * Before jumping this function disables all interrupts, sets Stack Pointer,
 * sets the Reset Vector, and jumps to the application address + 4.
 *
 * @param address Beginning address of the application to run
 */
extern void System_StartApplication(uint32_t address);

/**
 * @brief System delay based on system tick
 * @param miliseconds Delay in msec
 */
extern void System_Delay(uint32_t miliseconds);

/**
 * @brief Get the current system tick count
 * @return System tick count
 */
extern uint32_t System_GetTick(void);

/**
 * @brief Get random number using RNG
 * @return Random number
 */
extern uint32_t System_GetRandomNumber(void);

/**
 * @brief Unlock Flash interface for programming
 */
extern void System_FlashEnable(void);

/**
 * @brief Lock Flash interface for programming
 */
extern void System_FlashDisable(void);

/**
 * @brief Program double 32-bit word to Flash
 * @param address Target address in Flash
 * @param data Data to write
 * @return HAL status
 */
extern int16_t System_FlashProgramDoubleWord(uint32_t address, uint64_t data);

/**
 * @brief Program a bunch of data into Flash memory
 * @param address Address in Flash memory where the data should be stored
 * @param data Pointer to the data to write to Flash
 * @param dataLength Length of the data to write
 * @return Status
 */
extern int16_t System_FlashProgram(uint32_t address, uint8_t *data, uint32_t dataLength);

/**
 * @brief Erase the selected Flash area
 * @param[in] startAddr Start address of the memory to erase
 * @param[in] endAddr End address of the memory to erase
 * @return HAL status
 */
extern int16_t System_FlashErase(uint32_t startAddr, uint32_t endAddr);

/**
 * @brief Check if the flash area is empty
 * @param address Start address to check
 * @param size Size of the flash to check
 * @return Result (OK - empty, ERROR - not empty)
 */
extern Status_t System_IsFlashNotEmpty(uint32_t *address, uint32_t size);

/* ---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_MSP_H_ */

/** @} */
