////////////////////////////////////////////////////////////////////////////////
//
//  File           : sg_driver.c
//  Description    : This file contains the driver code to be developed by
//                   the students of the 311 class.  See assignment details
//                   for additional information.
//
//   Author        : Shaunak Wagh
//   Last Modified : 10/07/2020
//

// Include Files
#include <stdlib.h>
#include <string.h>

// Project Includes
#include <sg_defs.h>
#include <sg_driver.h>
// Defines

//
// Global Data

//
// Functions

////////////////////////////////////////////////////////////////////////////////
//
// Function     : serialize_sg_packet
// Description  : Serialize a ScatterGather packet (create packet)
//
// Inputs       : loc - the local node identifier
//                rem - the remote node identifier
//                blk - the block identifier
//                op - the operation performed/to be performed on block
//                sseq - the sender sequence number
//                rseq - the receiver sequence number
//                data - the data block (of size SG_BLOCK_SIZE) or NULL
//                packet - the buffer to place the data
//                plen - the packet length (int bytes)
// Outputs      : 0 if successfully created, -1 if failure

SG_Packet_Status serialize_sg_packet(SG_Node_ID loc, SG_Node_ID rem,
                                     SG_Block_ID blk, SG_System_OP op,
                                     SG_SeqNum sseq, SG_SeqNum rseq, char *data,
                                     char *packet, size_t *plen) {
printf("SERIALIZE\n\n");
printf("loc, is %d\n", loc);
printf("rem is %d\n", rem);
printf("blk is %d\n", blk);
printf("op is %d\n", op);
printf("data is %s\n", data);
printf("packet is %s\n", packet);
printf("plen is %d\n", plen);

  int8_t data_indicator = 0;

  if (loc == 0)
    return (SG_PACKT_LOCID_BAD);
  if (rem == 0)
    return (SG_PACKT_REMID_BAD);
  if (blk == 0)
    return (SG_PACKT_BLKID_BAD);
  switch (op) {
  case SG_INIT_ENDPOINT:
    break;
  case SG_STOP_ENDPOINT:
    break;
  case SG_CREATE_BLOCK:
    break;
  case SG_UPDATE_BLOCK:
    break;
  case SG_OBTAIN_BLOCK:
    break;
  case SG_DELETE_BLOCK:
    break;
  case SG_MAXVAL_OP:
    break;
  default:
    return (SG_PACKT_OPERN_BAD);
  }

  if (sseq == 0)
    return (SG_PACKT_SNDSQ_BAD);
  if (rseq == 0)
    return (SG_PACKT_RCVSQ_BAD);
  if (data != NULL) {
    data_indicator = 1;
  }
  if (data == NULL) {
    data_indicator = 0;

    if (packet == NULL) {
      return (SG_PACKT_PDATA_BAD);
    }
  }

  //packet[SG_DATA_PACKET_SIZE];

  uint32_t magic = SG_MAGIC_VALUE;

  memcpy(&packet[0], &magic, 4);

  memcpy(&packet[4], &loc, 8);

  memcpy(&packet[12], &rem, 8);

  memcpy(&packet[20], &blk, 8);

  memcpy(&packet[28], &op, 4);

  memcpy(&packet[32], &sseq, 2);

  memcpy(&packet[34], &rseq, 2);

  memcpy(&packet[36], &data_indicator, 1);

  if (data_indicator == 1) {
    printf("%d\n", SG_BLOCK_SIZE);
    memcpy(&packet[37], data, SG_BLOCK_SIZE);
    memcpy(&packet[37 + SG_BLOCK_SIZE], &magic, 4);
  }
  if (data_indicator == 0) {
    memcpy(&packet[37], &magic, 4);
  }

  *plen = SG_BASE_PACKET_SIZE;
    printf("\nplen in serial is %d\n",*plen );


printf("222 data is %s\n", data);
printf("222 packet is %s\n", packet);
printf("222 plen is %d\n", plen);
printf("END SERIALIZE\n\n");

  return (SG_PACKT_OK);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : deserialize_sg_packet
// Description  : De-serialize a ScatterGather packet (unpack packet)
//
// Inputs       : loc - the local node identifier
//                rem - the remote node identifier
//                blk - the block identifier
//                op - the operation performed/to be performed on block
//                sseq - the sender sequence number
//                rseq - the receiver sequence number
//                data - the data block (of size SG_BLOCK_SIZE) or NULL
//                packet - the buffer to place the data
//                plen - the packet length (int bytes)
// Outputs      : 0 if successfully created, -1 if failure

SG_Packet_Status deserialize_sg_packet(SG_Node_ID *loc, SG_Node_ID *rem,
                                       SG_Block_ID *blk, SG_System_OP *op,
                                       SG_SeqNum *sseq, SG_SeqNum *rseq,
                                       char *data, char *packet, size_t plen) {
  printf("DE-SERIALIZE\n\n");

  printf("loc, is %d\n", loc);
printf("rem is %d\n", rem);
printf("blk is %d\n", blk);
printf("op is %d\n", op);
printf("data is %s\n", data);
printf("packet is %s\n", packet);
printf("plen is %d\n", plen);

  uint32_t magic = SG_MAGIC_VALUE;
  int8_t data_indicator;

  memcpy(&magic, &packet[0], 4);

  memcpy(loc, &packet[4], 8);

  memcpy(rem, &packet[12], 8);

  memcpy(blk, &packet[20], 8);

  memcpy(op, &packet[28], 4);

  memcpy(sseq, &packet[32], 2);

  memcpy(rseq, &packet[34], 2);

  memcpy(&data_indicator, &packet[36], 1);

  if (data_indicator == 1) {
    memcpy(data, &packet[37], SG_BLOCK_SIZE);
    memcpy(&magic, &packet[37 + SG_BLOCK_SIZE], sizeof(magic));
  }
  if (data_indicator == 0) {
    memcpy(&magic, &packet[37], 4);
  }

  if (packet == NULL) {
    return (SG_PACKT_PDATA_BAD);
  }

  if (*loc == 0)
    return (SG_PACKT_LOCID_BAD);
  if (*rem == 0)
    return (SG_PACKT_REMID_BAD);
  if (*blk == 0)
    return (SG_PACKT_BLKID_BAD);
  switch (*op) {
  case SG_INIT_ENDPOINT:
    break;
  case SG_STOP_ENDPOINT:
    break;
  case SG_CREATE_BLOCK:
    break;
  case SG_UPDATE_BLOCK:
    break;
  case SG_OBTAIN_BLOCK:
    break;
  case SG_DELETE_BLOCK:
    break;
  case SG_MAXVAL_OP:
    break;
  default:
    return (SG_PACKT_OPERN_BAD);
  }

  if (*sseq == 0)
    return (SG_PACKT_SNDSQ_BAD);
  if (*rseq == 0)
    return (SG_PACKT_RCVSQ_BAD);

  if (magic != SG_MAGIC_VALUE) {
    return (SG_PACKT_PDATA_BAD);
  }

  if (data_indicator == 1 && data == NULL) {
    return (SG_PACKT_BLKDT_BAD);
  }
  printf("\nplen in deserial is  %d\n",plen );
printf("\nthe total block size %d\n",SG_DATA_PACKET_SIZE);

  if (plen != SG_BASE_PACKET_SIZE) {
    return (SG_PACKT_BLKLN_BAD);
  }

printf("data is %s\n", data);
printf("packet is %s\n", packet);
printf("plen is %d\n", plen);
printf("END DE-SERIALIZE\n\n");
  return (SG_PACKT_OK);
}
