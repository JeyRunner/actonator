#pragma once

HAL_StatusTypeDef s;

void canSetup() {
  uint16_t ownCanID = 0x010; // own 11 bit id

  // configure filter-banks
  // -> filter incoming can package by ID
  CAN_FilterTypeDef canFilter;
  canFilter.FilterActivation = CAN_FILTER_ENABLE;
  canFilter.FilterBank = 0;
  canFilter.FilterFIFOAssignment = CAN_RX_FIFO0;
  canFilter.FilterIdHigh = ownCanID<<5; // the 11 bit incoming ID to filter (first 5 bits would belong to extended ID)
  canFilter.FilterIdLow = 0;
  canFilter.FilterMaskIdHigh = 0xFFFF<<5; // the 11 bit fiter mask for the incoming ID to filter
  canFilter.FilterMaskIdLow = 0;
  canFilter.FilterMode = CAN_FILTERMODE_IDMASK;
  canFilter.FilterScale = CAN_FILTERSCALE_32BIT;
  canFilter.SlaveStartFilterBank = 0;
  s = HAL_CAN_ConfigFilter(&hcan, &canFilter);

  // enable can receive interrupt
  s = HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO0_OVERRUN | CAN_IT_TX_MAILBOX_EMPTY);

  // start can
  HAL_CAN_MspInit(&hcan); // needed ??
  s = HAL_CAN_Start(&hcan);
}




uint32_t txMailbox;
CAN_TxHeaderTypeDef  txHeader;
uint8_t txData[2];

void canSendDataTest() {
  txData[0] = 0xFA;
  txData[1] = 0x2D;

  txHeader.DLC = 2;
  txHeader.ExtId = 0;
  txHeader.IDE = CAN_ID_STD;
  txHeader.RTR = CAN_RTR_DATA;
  txHeader.StdId = 0x010; // id is 11 bits long
  txHeader.TransmitGlobalTime = DISABLE;

  int freeMailboxes = HAL_CAN_GetTxMailboxesFreeLevel(&hcan);
  HAL_StatusTypeDef status = HAL_CAN_AddTxMessage(&hcan, &txHeader, txData, &txMailbox);
  s = status;
  //CAN_Error_Code
}


bool canSendQueueNotFull() {
  uint32_t freeMailboxes = HAL_CAN_GetTxMailboxesFreeLevel(&hcan);
  return freeMailboxes > 0;
}

bool canSendQueueEmpty() {
  uint32_t freeMailboxes = HAL_CAN_GetTxMailboxesFreeLevel(&hcan);
  return freeMailboxes >= 3;
}


bool canSendData(uint32_t destAddress, uint8_t *data, uint8_t dataLength) {
  txHeader.DLC = dataLength;
  txHeader.ExtId = 0;
  txHeader.IDE = CAN_ID_STD;
  txHeader.RTR = CAN_RTR_DATA;
  txHeader.StdId = destAddress; // id is 11 bits long
  txHeader.TransmitGlobalTime = DISABLE;

  HAL_StatusTypeDef status = HAL_CAN_AddTxMessage(&hcan, &txHeader, data, &txMailbox);
  return status == HAL_OK;
}


void onCanTxReadyInterruptEnable() {
  HAL_NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
}
void onCanTxReadyInterruptDisable() {
  HAL_NVIC_DisableIRQ(USB_HP_CAN1_TX_IRQn);
}

void onCanRxInterruptEnable() {
  HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
  HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
}
void onCanRxInterruptDisable() {
  HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
  HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
}

/**
 * react when a tx mailbox is available again
 */
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan) {
  CanCommunication::onCanTxReady();
}
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan) {
  CanCommunication::onCanTxReady();
}
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan) {
  CanCommunication::onCanTxReady();
}
void HAL_CAN_TxMailbox0AbortCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_TxMailbox1AbortCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_TxMailbox2AbortCallback(CAN_HandleTypeDef *hcan);


static int receivedC = 0;

/**
 * Handle that can package is received.
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
  // disable this interrupt
  onCanRxInterruptDisable();

  CAN_RxHeaderTypeDef  rxHeader;
  uint8_t rxData[8];
  HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, rxData);

  CanCommunication::onCanPackagedReceived(rxHeader.StdId, rxData, rxHeader.DLC);

  // re-enable this interrupt
  onCanRxInterruptEnable();
}



void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan) {
  // if rx overflow
  if (hcan->ErrorCode & HAL_CAN_ERROR_RX_FOV0) {
    CanCommunication::onCanRxFifoOverflow();
  }
}