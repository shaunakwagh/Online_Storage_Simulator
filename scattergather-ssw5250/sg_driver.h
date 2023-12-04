#ifndef SG_DRIVER_INCLUDED
#define SG_DRIVER_INCLUDED

////////////////////////////////////////////////////////////////////////////////
//
//  File           : sg_driver.h
//  Description    : This is the declaration of the interface to the 
//                   ScatterGather driver (student code).
//
//   Author        : Patrick McDaniel
//   Last Modified : Thu 03 Sep 2020 01:26:06 PM PDT
//

// Includes
#include <sg_defs.h>

// Defines 

// Type definitions

// Global interface definitions

SG_Packet_Status serialize_sg_packet( SG_Node_ID loc, SG_Node_ID rem, SG_Block_ID blk, 
        SG_System_OP op, SG_SeqNum sseq, SG_SeqNum rseq, char *data, 
        char *packet, size_t *plen );
    // Serialize a ScatterGather packet (create packet)

SG_Packet_Status deserialize_sg_packet( SG_Node_ID *loc, SG_Node_ID *rem, SG_Block_ID *blk, 
        SG_System_OP *op, SG_SeqNum *sseq, SG_SeqNum *rseq, char *data, 
        char *packet, size_t plen );
    // De-serialize a ScatterGather packet (unpack packet)

#endif