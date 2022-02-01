/*
 * usbd_midi.c
 *
 *  Created on: Jan 31, 2022
 *      Author: fil
 */

#include "usbd_midi.h"
#include "usbd_ctlreq.h"
static uint8_t USBD_MIDI_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t USBD_MIDI_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t USBD_MIDI_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static uint8_t USBD_MIDI_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_MIDI_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_MIDI_EP0_RxReady(USBD_HandleTypeDef *pdev);

static uint8_t *USBD_MIDI_GetFSCfgDesc(uint16_t *length);
static uint8_t *USBD_MIDI_GetHSCfgDesc(uint16_t *length);
static uint8_t *USBD_MIDI_GetOtherSpeedCfgDesc(uint16_t *length);
static uint8_t *USBD_MIDI_GetOtherSpeedCfgDesc(uint16_t *length);
uint8_t *USBD_MIDI_GetDeviceQualifierDescriptor(uint16_t *length);

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_MIDI_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};

/**
  * @}
  */

/** @defgroup USBD_MIDI_Private_Variables
  * @{
  */


/* MIDI interface class callbacks structure */
USBD_ClassTypeDef  USBD_MIDI =
{
  USBD_MIDI_Init,
  USBD_MIDI_DeInit,
  USBD_MIDI_Setup,
  NULL,                 /* EP0_TxSent, */
  USBD_MIDI_EP0_RxReady,
  USBD_MIDI_DataIn,
  USBD_MIDI_DataOut,
  NULL,
  NULL,
  NULL,
  USBD_MIDI_GetHSCfgDesc,
  USBD_MIDI_GetFSCfgDesc,
  USBD_MIDI_GetOtherSpeedCfgDesc,
  USBD_MIDI_GetDeviceQualifierDescriptor,
};

/* USB MIDI device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_MIDI_CfgHSDesc[USB_MIDI_CONFIG_DESC_SIZ] __ALIGN_END =
{
		  // configuration descriptor
		  0x09, 0x02, 0x65, 0x00, 0x02, 0x01, 0x00, 0x80, 0x31,

		  // The Audio Interface Collection
		  0x09, 0x04, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, // Standard AC Interface Descriptor
		  0x09, 0x24, 0x01, 0x00, 0x01, 0x09, 0x00, 0x01, 0x01, // Class-specific AC Interface Descriptor
		  0x09, 0x04, 0x01, 0x00, 0x02, 0x01, 0x03, 0x00, 0x00, // MIDIStreaming Interface Descriptors
		  0x07, 0x24, 0x01, 0x00, 0x01, 0x41, 0x00,             // Class-Specific MS Interface Header Descriptor

		  // MIDI IN JACKS
		  0x06, 0x24, 0x02, 0x01, 0x01, 0x00,
		  0x06, 0x24, 0x02, 0x02, 0x02, 0x00,

		  // MIDI OUT JACKS
		  0x09, 0x24, 0x03, 0x01, 0x03, 0x01, 0x02, 0x01, 0x00,
		  0x09, 0x24, 0x03, 0x02, 0x06, 0x01, 0x01, 0x01, 0x00,

		  // OUT endpoint descriptor
		  0x09, 0x05, MIDI_OUT_EP, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00,
		  0x05, 0x25, 0x01, 0x01, 0x01,

		  // IN endpoint descriptor
		  0x09, 0x05, MIDI_IN_EP, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00,
		  0x05, 0x25, 0x01, 0x01, 0x03,
};


/* USB MIDI device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_MIDI_CfgFSDesc[USB_MIDI_CONFIG_DESC_SIZ] __ALIGN_END =
{
		  // configuration descriptor
		  0x09, 0x02, 0x65, 0x00, 0x02, 0x01, 0x00, 0x80, 0x31,

		  // The Audio Interface Collection
		  0x09, 0x04, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, // Standard AC Interface Descriptor
		  0x09, 0x24, 0x01, 0x00, 0x01, 0x09, 0x00, 0x01, 0x01, // Class-specific AC Interface Descriptor
		  0x09, 0x04, 0x01, 0x00, 0x02, 0x01, 0x03, 0x00, 0x00, // MIDIStreaming Interface Descriptors
		  0x07, 0x24, 0x01, 0x00, 0x01, 0x41, 0x00,             // Class-Specific MS Interface Header Descriptor

		  // MIDI IN JACKS
		  0x06, 0x24, 0x02, 0x01, 0x01, 0x00,
		  0x06, 0x24, 0x02, 0x02, 0x02, 0x00,

		  // MIDI OUT JACKS
		  0x09, 0x24, 0x03, 0x01, 0x03, 0x01, 0x02, 0x01, 0x00,
		  0x09, 0x24, 0x03, 0x02, 0x06, 0x01, 0x01, 0x01, 0x00,

		  // OUT endpoint descriptor
		  0x09, 0x05, MIDI_OUT_EP, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00,
		  0x05, 0x25, 0x01, 0x01, 0x01,

		  // IN endpoint descriptor
		  0x09, 0x05, MIDI_IN_EP, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00,
		  0x05, 0x25, 0x01, 0x01, 0x03,
};

__ALIGN_BEGIN static uint8_t USBD_MIDI_OtherSpeedCfgDesc[USB_MIDI_CONFIG_DESC_SIZ] __ALIGN_END =
{
		  // configuration descriptor
		  0x09, 0x02, 0x65, 0x00, 0x02, 0x01, 0x00, 0x80, 0x31,

		  // The Audio Interface Collection
		  0x09, 0x04, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, // Standard AC Interface Descriptor
		  0x09, 0x24, 0x01, 0x00, 0x01, 0x09, 0x00, 0x01, 0x01, // Class-specific AC Interface Descriptor
		  0x09, 0x04, 0x01, 0x00, 0x02, 0x01, 0x03, 0x00, 0x00, // MIDIStreaming Interface Descriptors
		  0x07, 0x24, 0x01, 0x00, 0x01, 0x41, 0x00,             // Class-Specific MS Interface Header Descriptor

		  // MIDI IN JACKS
		  0x06, 0x24, 0x02, 0x01, 0x01, 0x00,
		  0x06, 0x24, 0x02, 0x02, 0x02, 0x00,

		  // MIDI OUT JACKS
		  0x09, 0x24, 0x03, 0x01, 0x03, 0x01, 0x02, 0x01, 0x00,
		  0x09, 0x24, 0x03, 0x02, 0x06, 0x01, 0x01, 0x01, 0x00,

		  // OUT endpoint descriptor
		  0x09, 0x05, MIDI_OUT_EP, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00,
		  0x05, 0x25, 0x01, 0x01, 0x01,

		  // IN endpoint descriptor
		  0x09, 0x05, MIDI_IN_EP, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00,
		  0x05, 0x25, 0x01, 0x01, 0x03,
};

/**
  * @}
  */

/** @defgroup USBD_MIDI_Private_Functions
  * @{
  */

/**
  * @brief  USBD_MIDI_Init
  *         Initialize the MIDI interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t USBD_MIDI_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);
  USBD_MIDI_HandleTypeDef *hcdc;

  hcdc = USBD_malloc(sizeof(USBD_MIDI_HandleTypeDef));

  if (hcdc == NULL)
  {
    pdev->pClassData = NULL;
    return (uint8_t)USBD_EMEM;
  }

  pdev->pClassData = (void *)hcdc;

  if (pdev->dev_speed == USBD_SPEED_HIGH)
  {
    /* Open EP IN */
    (void)USBD_LL_OpenEP(pdev, MIDI_IN_EP, USBD_EP_TYPE_BULK,
                         MIDI_DATA_HS_IN_PACKET_SIZE);

    pdev->ep_in[MIDI_IN_EP & 0xFU].is_used = 1U;

    /* Open EP OUT */
    (void)USBD_LL_OpenEP(pdev, MIDI_OUT_EP, USBD_EP_TYPE_BULK,
                         MIDI_DATA_HS_OUT_PACKET_SIZE);

    pdev->ep_out[MIDI_OUT_EP & 0xFU].is_used = 1U;

    /* Set bInterval for MIDI CMD Endpoint */
    pdev->ep_in[MIDI_CMD_EP & 0xFU].bInterval = MIDI_HS_BINTERVAL;
  }
  else
  {
    /* Open EP IN */
    (void)USBD_LL_OpenEP(pdev, MIDI_IN_EP, USBD_EP_TYPE_BULK,
                         MIDI_DATA_FS_IN_PACKET_SIZE);

    pdev->ep_in[MIDI_IN_EP & 0xFU].is_used = 1U;

    /* Open EP OUT */
    (void)USBD_LL_OpenEP(pdev, MIDI_OUT_EP, USBD_EP_TYPE_BULK,
                         MIDI_DATA_FS_OUT_PACKET_SIZE);

    pdev->ep_out[MIDI_OUT_EP & 0xFU].is_used = 1U;

    /* Set bInterval for CMD Endpoint */
    pdev->ep_in[MIDI_CMD_EP & 0xFU].bInterval = MIDI_FS_BINTERVAL;
  }

  /* Open Command IN EP */
  (void)USBD_LL_OpenEP(pdev, MIDI_CMD_EP, USBD_EP_TYPE_INTR, MIDI_CMD_PACKET_SIZE);
  pdev->ep_in[MIDI_CMD_EP & 0xFU].is_used = 1U;

  /* Init  physical Interface components */
  ((USBD_MIDI_ItfTypeDef *)pdev->pUserData)->Init();

  /* Init Xfer states */
  hcdc->TxState = 0U;
  hcdc->RxState = 0U;

  if (pdev->dev_speed == USBD_SPEED_HIGH)
  {
    /* Prepare Out endpoint to receive next packet */
    (void)USBD_LL_PrepareReceive(pdev, MIDI_OUT_EP, hcdc->RxBuffer,
                                 MIDI_DATA_HS_OUT_PACKET_SIZE);
  }
  else
  {
    /* Prepare Out endpoint to receive next packet */
    (void)USBD_LL_PrepareReceive(pdev, MIDI_OUT_EP, hcdc->RxBuffer,
                                 MIDI_DATA_FS_OUT_PACKET_SIZE);
  }

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_MIDI_Init
  *         DeInitialize the MIDI layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t USBD_MIDI_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);

  /* Close EP IN */
  (void)USBD_LL_CloseEP(pdev, MIDI_IN_EP);
  pdev->ep_in[MIDI_IN_EP & 0xFU].is_used = 0U;

  /* Close EP OUT */
  (void)USBD_LL_CloseEP(pdev, MIDI_OUT_EP);
  pdev->ep_out[MIDI_OUT_EP & 0xFU].is_used = 0U;

  /* Close Command IN EP */
  (void)USBD_LL_CloseEP(pdev, MIDI_CMD_EP);
  pdev->ep_in[MIDI_CMD_EP & 0xFU].is_used = 0U;
  pdev->ep_in[MIDI_CMD_EP & 0xFU].bInterval = 0U;

  /* DeInit  physical Interface components */
  if (pdev->pClassData != NULL)
  {
    ((USBD_MIDI_ItfTypeDef *)pdev->pUserData)->DeInit();
    (void)USBD_free(pdev->pClassData);
    pdev->pClassData = NULL;
  }

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_MIDI_Setup
  *         Handle the MIDI specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t USBD_MIDI_Setup(USBD_HandleTypeDef *pdev,
                              USBD_SetupReqTypedef *req)
{
  USBD_MIDI_HandleTypeDef *hcdc = (USBD_MIDI_HandleTypeDef *)pdev->pClassData;
  uint16_t len;
  uint8_t ifalt = 0U;
  uint16_t status_info = 0U;
  USBD_StatusTypeDef ret = USBD_OK;

  if (hcdc == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
    case USB_REQ_TYPE_CLASS:
      if (req->wLength != 0U)
      {
        if ((req->bmRequest & 0x80U) != 0U)
        {
          ((USBD_MIDI_ItfTypeDef *)pdev->pUserData)->Control(req->bRequest,
                                                            (uint8_t *)hcdc->data,
                                                            req->wLength);

          len = MIN(MIDI_REQ_MAX_DATA_SIZE, req->wLength);
          (void)USBD_CtlSendData(pdev, (uint8_t *)hcdc->data, len);
        }
        else
        {
          hcdc->CmdOpCode = req->bRequest;
          hcdc->CmdLength = (uint8_t)req->wLength;

          (void)USBD_CtlPrepareRx(pdev, (uint8_t *)hcdc->data, req->wLength);
        }
      }
      else
      {
        ((USBD_MIDI_ItfTypeDef *)pdev->pUserData)->Control(req->bRequest,
                                                          (uint8_t *)req, 0U);
      }
      break;

    case USB_REQ_TYPE_STANDARD:
      switch (req->bRequest)
      {
        case USB_REQ_GET_STATUS:
          if (pdev->dev_state == USBD_STATE_CONFIGURED)
          {
            (void)USBD_CtlSendData(pdev, (uint8_t *)&status_info, 2U);
          }
          else
          {
            USBD_CtlError(pdev, req);
            ret = USBD_FAIL;
          }
          break;

        case USB_REQ_GET_INTERFACE:
          if (pdev->dev_state == USBD_STATE_CONFIGURED)
          {
            (void)USBD_CtlSendData(pdev, &ifalt, 1U);
          }
          else
          {
            USBD_CtlError(pdev, req);
            ret = USBD_FAIL;
          }
          break;

        case USB_REQ_SET_INTERFACE:
          if (pdev->dev_state != USBD_STATE_CONFIGURED)
          {
            USBD_CtlError(pdev, req);
            ret = USBD_FAIL;
          }
          break;

        case USB_REQ_CLEAR_FEATURE:
          break;

        default:
          USBD_CtlError(pdev, req);
          ret = USBD_FAIL;
          break;
      }
      break;

    default:
      USBD_CtlError(pdev, req);
      ret = USBD_FAIL;
      break;
  }

  return (uint8_t)ret;
}

/**
  * @brief  USBD_MIDI_DataIn
  *         Data sent on non-control IN endpoint
  * @param  pdev: device instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t USBD_MIDI_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  USBD_MIDI_HandleTypeDef *hcdc;
  PCD_HandleTypeDef *hpcd = pdev->pData;

  if (pdev->pClassData == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  hcdc = (USBD_MIDI_HandleTypeDef *)pdev->pClassData;

  if ((pdev->ep_in[epnum].total_length > 0U) &&
      ((pdev->ep_in[epnum].total_length % hpcd->IN_ep[epnum].maxpacket) == 0U))
  {
    /* Update the packet total length */
    pdev->ep_in[epnum].total_length = 0U;

    /* Send ZLP */
    (void)USBD_LL_Transmit(pdev, epnum, NULL, 0U);
  }
  else
  {
    hcdc->TxState = 0U;

    if (((USBD_MIDI_ItfTypeDef *)pdev->pUserData)->TransmitCplt != NULL)
    {
      ((USBD_MIDI_ItfTypeDef *)pdev->pUserData)->TransmitCplt(hcdc->TxBuffer, &hcdc->TxLength, epnum);
    }
  }

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_MIDI_DataOut
  *         Data received on non-control Out endpoint
  * @param  pdev: device instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t USBD_MIDI_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  USBD_MIDI_HandleTypeDef *hcdc = (USBD_MIDI_HandleTypeDef *)pdev->pClassData;

  if (pdev->pClassData == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  /* Get the received data length */
  hcdc->RxLength = USBD_LL_GetRxDataSize(pdev, epnum);

  /* USB data will be immediately processed, this allow next USB traffic being
  NAKed till the end of the application Xfer */

  ((USBD_MIDI_ItfTypeDef *)pdev->pUserData)->Receive(hcdc->RxBuffer, &hcdc->RxLength);

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_MIDI_EP0_RxReady
  *         Handle EP0 Rx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t USBD_MIDI_EP0_RxReady(USBD_HandleTypeDef *pdev)
{
  USBD_MIDI_HandleTypeDef *hcdc = (USBD_MIDI_HandleTypeDef *)pdev->pClassData;

  if (hcdc == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  if ((pdev->pUserData != NULL) && (hcdc->CmdOpCode != 0xFFU))
  {
    ((USBD_MIDI_ItfTypeDef *)pdev->pUserData)->Control(hcdc->CmdOpCode,
                                                      (uint8_t *)hcdc->data,
                                                      (uint16_t)hcdc->CmdLength);
    hcdc->CmdOpCode = 0xFFU;
  }

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_MIDI_GetFSCfgDesc
  *         Return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t *USBD_MIDI_GetFSCfgDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_MIDI_CfgFSDesc);

  return USBD_MIDI_CfgFSDesc;
}

/**
  * @brief  USBD_MIDI_GetHSCfgDesc
  *         Return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t *USBD_MIDI_GetHSCfgDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_MIDI_CfgHSDesc);

  return USBD_MIDI_CfgHSDesc;
}

/**
  * @brief  USBD_MIDI_GetOtherSpeedCfgDesc
  *         Return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t *USBD_MIDI_GetOtherSpeedCfgDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_MIDI_OtherSpeedCfgDesc);

  return USBD_MIDI_OtherSpeedCfgDesc;
}

/**
  * @brief  USBD_MIDI_GetDeviceQualifierDescriptor
  *         return Device Qualifier descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
uint8_t *USBD_MIDI_GetDeviceQualifierDescriptor(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_MIDI_DeviceQualifierDesc);

  return USBD_MIDI_DeviceQualifierDesc;
}

/**
  * @brief  USBD_MIDI_RegisterInterface
  * @param  pdev: device instance
  * @param  fops: CD  Interface callback
  * @retval status
  */
uint8_t USBD_MIDI_RegisterInterface(USBD_HandleTypeDef *pdev,
                                   USBD_MIDI_ItfTypeDef *fops)
{
  if (fops == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  pdev->pUserData = fops;

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_MIDI_SetTxBuffer
  * @param  pdev: device instance
  * @param  pbuff: Tx Buffer
  * @retval status
  */
uint8_t USBD_MIDI_SetTxBuffer(USBD_HandleTypeDef *pdev,
                             uint8_t *pbuff, uint32_t length)
{
  USBD_MIDI_HandleTypeDef *hcdc = (USBD_MIDI_HandleTypeDef *)pdev->pClassData;

  if (hcdc == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  hcdc->TxBuffer = pbuff;
  hcdc->TxLength = length;

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_MIDI_SetRxBuffer
  * @param  pdev: device instance
  * @param  pbuff: Rx Buffer
  * @retval status
  */
uint8_t USBD_MIDI_SetRxBuffer(USBD_HandleTypeDef *pdev, uint8_t *pbuff)
{
  USBD_MIDI_HandleTypeDef *hcdc = (USBD_MIDI_HandleTypeDef *)pdev->pClassData;

  if (hcdc == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  hcdc->RxBuffer = pbuff;

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_MIDI_TransmitPacket
  *         Transmit packet on IN endpoint
  * @param  pdev: device instance
  * @retval status
  */
uint8_t USBD_MIDI_TransmitPacket(USBD_HandleTypeDef *pdev)
{
  USBD_MIDI_HandleTypeDef *hcdc = (USBD_MIDI_HandleTypeDef *)pdev->pClassData;
  USBD_StatusTypeDef ret = USBD_BUSY;

  if (pdev->pClassData == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  if (hcdc->TxState == 0U)
  {
    /* Tx Transfer in progress */
    hcdc->TxState = 1U;

    /* Update the packet total length */
    pdev->ep_in[MIDI_IN_EP & 0xFU].total_length = hcdc->TxLength;

    /* Transmit next packet */
    (void)USBD_LL_Transmit(pdev, MIDI_IN_EP, hcdc->TxBuffer, hcdc->TxLength);

    ret = USBD_OK;
  }

  return (uint8_t)ret;
}

/**
  * @brief  USBD_MIDI_ReceivePacket
  *         prepare OUT Endpoint for reception
  * @param  pdev: device instance
  * @retval status
  */
uint8_t USBD_MIDI_ReceivePacket(USBD_HandleTypeDef *pdev)
{
  USBD_MIDI_HandleTypeDef *hcdc = (USBD_MIDI_HandleTypeDef *)pdev->pClassData;

  if (pdev->pClassData == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  if (pdev->dev_speed == USBD_SPEED_HIGH)
  {
    /* Prepare Out endpoint to receive next packet */
    (void)USBD_LL_PrepareReceive(pdev, MIDI_OUT_EP, hcdc->RxBuffer,
                                 MIDI_DATA_HS_OUT_PACKET_SIZE);
  }
  else
  {
    /* Prepare Out endpoint to receive next packet */
    (void)USBD_LL_PrepareReceive(pdev, MIDI_OUT_EP, hcdc->RxBuffer,
                                 MIDI_DATA_FS_OUT_PACKET_SIZE);
  }

  return (uint8_t)USBD_OK;
}
