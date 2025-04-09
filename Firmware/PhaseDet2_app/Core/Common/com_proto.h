/**
 * @file       comm_proto.h
 * @brief      LeBin communication protocol communication
 * @addtogroup grComProto
 * @{
 */

#ifndef COM_PROTO_H_
#define COM_PROTO_H_

/* Includes ------------------------------------------------------------------*/

#include "common.h"

/* Definitions ---------------------------------------------------------------*/

//! Protocol header length in Bytes
#define COM_PROTO_HEADER_LEN          4

//! Protocol start byte
#define COM_PROTO_UART_START_BYTE     0x90

//! Protocol minimal message length
#define COM_PROTO_MIN_LENGTH          8

//! Text log buffer length
#define COM_PROTO_TEXT_BUFFER         512

/* Typedefs ------------------------------------------------------------------*/

//! Protocol control character enumeration
typedef enum
{
  COM_PROTO_ERROR          = 1,    ///< Program Error code
  COM_PROTO_READ_REG_RESP  = 5,    ///< Response to ReadRegister command
  COM_PROTO_TIME_SER       = 8,    ///< Time series
  COM_PROTO_EVENT_RESP     = 12,   ///< Response to Event request
  COM_PROTO_TEXT_RESP      = 13,   ///< Response to Text log request
  COM_PROTO_FILE_UP_ACK    = 64,   ///< Acknowledge of file upload packet
  COM_PROTO_FILE_HDR_ACK   = 67,   ///< Acknowledge of uploaded file header
  COM_PROTO_FW_UPG_ACK     = 127,  ///< Acknowledge of FW upgrade packet
  COM_PROTO_WRITE_REG      = 129,  ///< Write register request
  COM_PROTO_READ_REG       = 130,  ///< Read register request
  COM_PROTO_GET_EVENT      = 131,  ///< Get events from local buffer
  COM_PROTO_GET_TEXT       = 132,  ///< Get text log
  COM_PROTO_FILE_UPLOAD    = 200,  ///< File upload
  COM_PROTO_FILE_HEADER    = 203,  ///< File header upload
  COM_PROTO_FW_UPGRADE     = 255,  ///< Read configuration from external flash memory
} ComProto_PacketId_t;

/* Typedefs ------------------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/

/**
 * @brief Initialize module
 * @return STATUS_OK
 */
Status_t ComProto_Init(void);

/**
 * @brief Permanent handle
 * @return STATUS_OK
 */
Status_t ComProto_Handle(void);

/**
 * @brief Get length of received packet
 * @param data Pointer to packet
 * @return Length of packet in bytes
 */
uint16_t ComProto_GetLength(uint8_t *data);

/**
 * @brief Check the length and basic consistency of received packet
 * @param data Pointer to packet
 * @param length Length of received data
 * @return STATUS_OK if we have received the entire packet
 */
Status_t ComProto_CheckProtoLength(uint8_t *data, uint16_t length);

/**
 * @brief Fill series of data into packet for sending
 * @param buffer Pointer to buffer
 * @param type Data type and size of variable
 * @param id Identifier of series
 * @param timestamp Current timestamp
 * @param delta Delta of samples or sampling frequency
 * @param count Number of samples
 * @return STATUS_OK
 */
Status_t ComProto_FillSeries(uint8_t *buffer, uint16_t type, uint16_t id, uint32_t timestamp, uint32_t delta, uint32_t count);

/**
 * @brief Fill data into the log text buffer
 * @param text String to log
 */
Status_t ComProto_LogText(char *text);

/**
 * Insert a character to the text buffer
 * @param text String to log
 */
Status_t ComProto_LogChar(char chr);

/**
 * @brief Process the received packet
 *
 * The received packet is processed according to the type of packet (read, write, fw_upgr)
 * and the response is created.
 * @param data Received packet to process
 * @param txResp Response buffer
 * @return Status
 */
Status_t ComProto_ProcessPacket(uint8_t *data, uint8_t* txResp);

/* ---------------------------------------------------------------------------*/

#endif /* COM_PROTO_H_ */

/** @} */
