/**
 * @file       config_app.c
 * @brief      Application-specific configuration callback implementation
 * @addtogroup grConfig
 * @{
 */

/* Includes ------------------------------------------------------------------*/

#include "common.h"
#include "configuration.h"

/* Application includes */

/* Private defines -----------------------------------------------------------*/
/* Private macros  -----------------------------------------------------------*/

/* Private typedefs ----------------------------------------------------------*/

typedef struct
{
  uint32_t sn;
  uint32_t temp;
}ConfigApp_Private_t;

/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static ConfigApp_Private_t ca;

/* Public variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions -----------------------------------------------------------------*/

Status_t Config_AppInit(void)
{
  Status_t ret = STATUS_OK;

  UNUSED(ca);

  return ret;
}


Status_t Config_Callback(uint32_t id)
{
  Status_t ret = STATUS_OK;

  /* Check if some value need further action to propagate */
  switch (CONF_BLOCK_ID(id))
  {

    default:
      break;
  }

  return ret;
}


/* Private Functions ---------------------------------------------------------*/


/** @} */
