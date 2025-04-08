/**
 * @file      system_msp.c
 * @brief     Microcontroller specific function module
 *
 * @author    richard.linhart@logicelements.cz
 * @date      Aug 2022
 * @copyright Logic Elements, s.r.o.
 *
 * @addtogroup grSystemMsp
 * @{
 */

/* Includes ------------------------------------------------------------------*/

#include "system_msp.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/**
 * @brief  Gets the page number of a given address
 * @param  None
 * @retval The page number of a given address
 */
static uint32_t System_GetPageNr(uint32_t address);

/* Public function prototypes -----------------------------------------------*/

/**
 * Weak prototype of user hook function on entering low-power Stop mode.
 */
__weak void System_EnterStopMode(void);

/**
 * Weak prototype of user hook function on exiting low-power Stop mode.
 */
__weak void System_ExitStopMode(void);


int16_t System_GetZoneFromTimestamp(uint32_t timestamp);

/* Public functions ----------------------------------------------------------*/

/* Perform a systen RESET */
void System_Reset(void)
{
  NVIC_SystemReset();
}


/* Refresh the system watchdog */
Status_t System_ReloadWdg(void)
{
  Status_t ret = STATUS_OK;

  IWDG->KR = IWDG_KEY_RELOAD;
  return ret;
}


/* Start application at specified address */
void System_StartApplication(uint32_t address)
{
  uint32_t JumpAddress;
  System_Callback_t jump_to_application;

  // prepare jump address
  JumpAddress = *(uint32_t*) (address + 4);
  jump_to_application = (System_Callback_t) (JumpAddress);

  // disable all interrupts
  // + 0 is a workaround of "assignment to itself code check"
  NVIC->ICER[0] = NVIC->ICER[0] + 0;
  NVIC->ICER[1] = NVIC->ICER[1] + 0;
  NVIC->ICER[2] = NVIC->ICER[2] + 0;

  // clear pending interrupts
  // + 0 is a workaround of "assignment to itself code check"
  NVIC->ICPR[0] = NVIC->ICPR[0] + 0;
  NVIC->ICPR[1] = NVIC->ICPR[1] + 0;
  NVIC->ICPR[2] = NVIC->ICPR[2] + 0;

  // set the reset vector
  SCB->VTOR = address;

  // initialize application Stack Pointer
  __set_MSP(*(uint32_t*) address);

  // jump to the application finally ;)
  jump_to_application();
}


/* System delay based on system tick */
void System_Delay(uint32_t milliseconds)
{
  HAL_Delay(milliseconds);
}


/* Get the current system tick count */
uint32_t System_GetTick(void)
{
  return HAL_GetTick();
}


/* Get random number using RNG */
uint32_t System_GetRandomNumber(void)
{
  uint32_t countdown = 40;  // 40 cycles max (see RefMan)
  RNG->CR = 0x04;

  while(((RNG->SR & 0x01) == 0) && countdown != 0)
    countdown--;

  RNG->CR = 0x00;
  RNG->SR = 0x00;
  return RNG->DR;
}


/* Unlock Flash interface for programming */
void System_FlashEnable(void)
{
  // unlock the Program memory
  HAL_FLASH_Unlock();

  // clear all FLASH flags
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_PROGERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR | FLASH_FLAG_PGSERR | FLASH_FLAG_MISERR | FLASH_FLAG_FASTERR | FLASH_FLAG_RDERR | FLASH_FLAG_OPTVERR);
}


/* Lock Flash interface for programming */
void System_FlashDisable(void)
{
  HAL_FLASH_Lock();
}


/* Program double 32-bit word to Flash */
int16_t System_FlashProgramDoubleWord(uint32_t address, uint64_t data)
{
  HAL_StatusTypeDef ret;

  ret = HAL_FLASH_Program(TYPEPROGRAM_DOUBLEWORD, address, data);
  return (Status_t) ret;
}


/* Program a bunch of data into Flash memory */
int16_t System_FlashProgram(uint32_t address, uint8_t *data, uint32_t dataLength)
{
  Status_t ret = STATUS_OK;
  uint32_t bytesWritten = 0;
  uint64_t value;

  // unlock Flash memory
  ret = HAL_FLASH_Unlock();

  // address must be multiple of 4, data length must be multiple of 8 (double word write)
  if (((uint32_t)address % 4) || (dataLength % 8))
  {
    ret = STATUS_ERROR;
  }

  while(ret == STATUS_OK && bytesWritten < dataLength)
  {
    // write a double word
	value = data[7]; value <<= 8;
	value |= data[6]; value <<= 8;
	value |= data[5]; value <<= 8;
	value |= data[4]; value <<= 8;
	value |= data[3]; value <<= 8;
	value |= data[2]; value <<= 8;
	value |= data[1]; value <<= 8;
	value |= data[0];
    ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (uint32_t)address, value);
    bytesWritten += 8;
    address += 8;
    data += 8;
  }

  // lock Flash memory
  HAL_FLASH_Lock();

  return (int16_t) ret;
}


/* Erase the selected Flash area */
int16_t System_FlashErase(uint32_t startAddr, uint32_t endAddr)
{
  HAL_StatusTypeDef ret;
  FLASH_EraseInitTypeDef EraseInitStruct;
  uint32_t PageError = 0;
  uint32_t FirstPage = 0;
  uint32_t NbOfPages = 0;

  // get the First page number to erase
  FirstPage = System_GetPageNr(startAddr);

  // get the number of pages to erase
  NbOfPages = System_GetPageNr(endAddr) - FirstPage + 1;

  // configure the page erase structure
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Banks = FLASH_BANK_1;
  EraseInitStruct.Page = FirstPage;
  EraseInitStruct.NbPages = NbOfPages;

  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPERR | FLASH_FLAG_PROGERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR | FLASH_FLAG_PGSERR | FLASH_FLAG_MISERR | FLASH_FLAG_FASTERR | FLASH_FLAG_RDERR | FLASH_FLAG_OPTVERR);

  // enable flash access, erase sector, disable flash access
  System_FlashEnable();
  ret = HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
  System_FlashDisable();

  return (int16_t) ret;
}


/* Check if the flash area is empty */
Status_t System_IsFlashNotEmpty(uint32_t *address, uint32_t size)
{
  Status_t ret = STATUS_ERROR;
  uint32_t i = 0;

  // go through the given flash
  while (i < size && ret == STATUS_ERROR)
  {
    // if there is not a reset value, exit
    if (address[i/4] != 0xFFFFFFFF)
    {
      ret = STATUS_OK;
    }
    // increment step
    i += 0x10;
  }

  return ret;
}

/* Private functions ----------------------------------------------------------*/

/* Gets the page number of a given address */
static uint32_t System_GetPageNr(uint32_t address)
{
  return((address - 0x08000000) / FLASH_PAGE_SIZE);
}

/* ---------------------------------------------------------------------------*/

/** @} */
