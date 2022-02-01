/*
 * usbd_midi.h
 *
 *  Created on: Jan 31, 2022
 *      Author: fil
 */

#ifndef ST_STM32_USB_DEVICE_LIBRARY_CLASS_MIDI_INC_USBD_MIDI_H_
#define ST_STM32_USB_DEVICE_LIBRARY_CLASS_MIDI_INC_USBD_MIDI_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup usbd_cdc
  * @brief This file is the Header file for usbd_cdc.c
  * @{
  */


/** @defgroup usbd_cdc_Exported_Defines
  * @{
  */
#define MIDI_IN_EP                                   0x81U  /* EP1 for data IN */
#define MIDI_OUT_EP                                  0x01U  /* EP1 for data OUT */
#define MIDI_CMD_EP                                  0x82U  /* EP2 for MIDI commands */

#ifndef MIDI_HS_BINTERVAL
#define MIDI_HS_BINTERVAL                            0x10U
#endif /* MIDI_HS_BINTERVAL */

#ifndef MIDI_FS_BINTERVAL
#define MIDI_FS_BINTERVAL                            0x10U
#endif /* MIDI_FS_BINTERVAL */

/* MIDI Endpoints parameters: you can fine tune these values depending on the needed baudrates and performance. */
#define MIDI_DATA_HS_MAX_PACKET_SIZE                 512U  /* Endpoint IN & OUT Packet size */
#define MIDI_DATA_FS_MAX_PACKET_SIZE                 64U  /* Endpoint IN & OUT Packet size */
#define MIDI_CMD_PACKET_SIZE                         8U  /* Control Endpoint Packet size */

#define USB_MIDI_CONFIG_DESC_SIZ                     0x6DU
#define MIDI_DATA_HS_IN_PACKET_SIZE                  MIDI_DATA_HS_MAX_PACKET_SIZE
#define MIDI_DATA_HS_OUT_PACKET_SIZE                 MIDI_DATA_HS_MAX_PACKET_SIZE

#define MIDI_DATA_FS_IN_PACKET_SIZE                  MIDI_DATA_FS_MAX_PACKET_SIZE
#define MIDI_DATA_FS_OUT_PACKET_SIZE                 MIDI_DATA_FS_MAX_PACKET_SIZE

#define MIDI_REQ_MAX_DATA_SIZE                       0x7U
/*---------------------------------------------------------------------*/
/*  MIDI definitions                                                    */
/*---------------------------------------------------------------------*/
#define MIDI_SEND_ENCAPSULATED_COMMAND               0x00U
#define MIDI_GET_ENCAPSULATED_RESPONSE               0x01U
#define MIDI_SET_COMM_FEATURE                        0x02U
#define MIDI_GET_COMM_FEATURE                        0x03U
#define MIDI_CLEAR_COMM_FEATURE                      0x04U
#define MIDI_SET_LINE_CODING                         0x20U
#define MIDI_GET_LINE_CODING                         0x21U
#define MIDI_SET_CONTROL_LINE_STATE                  0x22U
#define MIDI_SEND_BREAK                              0x23U

/**
  * @}
  */


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */

/**
  * @}
  */
typedef struct
{
  uint32_t bitrate;
  uint8_t  format;
  uint8_t  paritytype;
  uint8_t  datatype;
} USBD_MIDI_LineCodingTypeDef;

typedef struct _USBD_MIDI_Itf
{
  int8_t (* Init)(void);
  int8_t (* DeInit)(void);
  int8_t (* Control)(uint8_t cmd, uint8_t *pbuf, uint16_t length);
  int8_t (* Receive)(uint8_t *Buf, uint32_t *Len);
  int8_t (* TransmitCplt)(uint8_t *Buf, uint32_t *Len, uint8_t epnum);
} USBD_MIDI_ItfTypeDef;


typedef struct
{
  uint32_t data[MIDI_DATA_HS_MAX_PACKET_SIZE / 4U];      /* Force 32bits alignment */
  uint8_t  CmdOpCode;
  uint8_t  CmdLength;
  uint8_t  *RxBuffer;
  uint8_t  *TxBuffer;
  uint32_t RxLength;
  uint32_t TxLength;

  __IO uint32_t TxState;
  __IO uint32_t RxState;
} USBD_MIDI_HandleTypeDef;



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */

extern USBD_ClassTypeDef USBD_MIDI;
#define USBD_MIDI_CLASS &USBD_MIDI
/**
  * @}
  */

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
uint8_t USBD_MIDI_RegisterInterface(USBD_HandleTypeDef *pdev,
                                   USBD_MIDI_ItfTypeDef *fops);

uint8_t USBD_MIDI_SetTxBuffer(USBD_HandleTypeDef *pdev, uint8_t *pbuff,
                             uint32_t length);

uint8_t USBD_MIDI_SetRxBuffer(USBD_HandleTypeDef *pdev, uint8_t *pbuff);
uint8_t USBD_MIDI_ReceivePacket(USBD_HandleTypeDef *pdev);
uint8_t USBD_MIDI_TransmitPacket(USBD_HandleTypeDef *pdev);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif


#endif /* ST_STM32_USB_DEVICE_LIBRARY_CLASS_MIDI_INC_USBD_MIDI_H_ */
