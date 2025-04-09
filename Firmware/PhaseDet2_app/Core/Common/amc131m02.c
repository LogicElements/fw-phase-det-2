/**
 * @file       amc131m02.c
 * @brief      file_brief
 * @addtogroup gr
 * @{
 */

/* Includes ------------------------------------------------------------------*/

#include "amc131m02.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "system_msp.h"

/* Private defines -----------------------------------------------------------*/

#define AMC_OP_NULL     0x0000
#define AMC_OP_RESET    0x0011
#define AMC_OP_STANDBY  0x0022
#define AMC_OP_WAKEUP   0x0033
#define AMC_OP_LOCK     0x0555
#define AMC_OP_UNLOCK   0x0666
#define AMC_OP_RREG     0xA000
#define AMC_OP_WREG     0x6000


/* Private macros  -----------------------------------------------------------*/

/* Private typedefs ----------------------------------------------------------*/

/**
 * Declaration of all private variables
 */
typedef struct
{
  uint8_t irqFlag;
}amc131m02_Private_t;

/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
 * Instance of all private variables (except HAL handles)
 */
static amc131m02_Private_t amc;

/* Private variables ---------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions -----------------------------------------------------------------*/

Status_t Amc_Init(void)
{
  Status_t ret = STATUS_OK;

  /* Initialize hardware */
  HAL_GPIO_WritePin(CS_A_GPIO_Port, CS_A_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(CS_B_GPIO_Port, CS_B_Pin, GPIO_PIN_SET);

  /* Start clock signal */
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);

  System_DelayUs(10);

  /* Reset AMC */
  HAL_GPIO_WritePin(SYNCnRESET_GPIO_Port, SYNCnRESET_Pin, GPIO_PIN_RESET);
  System_DelayUs(1000);
  HAL_GPIO_WritePin(SYNCnRESET_GPIO_Port, SYNCnRESET_Pin, GPIO_PIN_SET);
  System_DelayUs(1000);

  /* Read chip ID */

  return ret;
}


uint16_t Amc_ReadRegister(uint16_t address)
{
  uint16_t frame[4];
  uint16_t resp[4];

  /* Compose frame */
  frame[0] = AMC_OP_RREG + (address << 7);
  frame[1] = 0;
  frame[2] = 0;
  frame[3] = 0;

  HAL_GPIO_WritePin(CS_B_GPIO_Port, CS_B_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)frame, (uint8_t*)resp, 4, 20);
  HAL_GPIO_WritePin(CS_B_GPIO_Port, CS_B_Pin, GPIO_PIN_SET);

  System_DelayUs(10);

  /* Compose frame */
  frame[0] = AMC_OP_NULL;

  HAL_GPIO_WritePin(CS_B_GPIO_Port, CS_B_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)frame, (uint8_t*)resp, 4, 20);
  HAL_GPIO_WritePin(CS_B_GPIO_Port, CS_B_Pin, GPIO_PIN_SET);

  return resp[0];
}




void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == ADC_DRDY_Pin)
  {
    amc.irqFlag++;
  }
}

/* Private Functions ---------------------------------------------------------*/

/** @} */
