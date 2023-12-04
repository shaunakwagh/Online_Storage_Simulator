////////////////////////////////////////////////////////////////////////////////
//
//  File           : sg_driver.c
//  Description    : This file contains the driver code to be developed by
//                   the students of the 311 class.  See assignment details
//                   for additional information.
//
//   Author        : Shaunak Wagh 
//   Last Modified : 
//

// Include Files

// Project Includes
#include <sg_driver.h>
#include <sg_service.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// Defines

//
// Global Data

// Driver file entry

// Global data
int sgDriverInitialized = 0; // The flag indicating the driver initialized
SG_Block_ID sgLocalNodeId;   // The local node identifier
SG_SeqNum sgLocalSeqno;      // The local sequence number

struct file
{
    char *path;
    //char *data[20];
    int fp;  //File pointer
    int size; //Actual size of the fileSG_Node_ID loc, rem;
    SG_Block_ID blkid[20];
    SG_Node_ID remid[20];
    int curr_blk; // Current block
};


  

struct file fs[100];
 SgFHandle curr = 0;
 int first=0;




// Driver support functions
int sgInitEndpoint( void ); // Initialize the endpoint

//
// Functions

//
// File system interface implementation

////////////////////////////////////////////////////////////////////////////////
//
// Function     : sgopen
// Description  : Open the file for for reading and writing
//
// Inputs       : path - the path/filename of the file to be read
// Outputs      : file handle if successful test, -1 if failure

SgFHandle sgopen(const char *path) {
//printf("\n 111 File is %s\n", path);
    // First check to see if we have been initialized

    if (!sgDriverInitialized) {

        // Call the endpoint initialization 
        if ( sgInitEndpoint() ) {
            logMessage( LOG_ERROR_LEVEL, "sgopen: Scatter/Gather endpoint initialization failed." );
            return( -1 );
        }

        // Set to initialized
        sgDriverInitialized = 1;
    }
    

//FILL IN THE REST OF THE CODE
    
   

//printf("\n File is %s\n", path);

struct file local;
  
//Check if file exists. If it exists then return the file handle   
for(int i = 0; i < 100 && i < curr; i++){
    if ( !(strcmp(fs[i].path, path) ) ) {
        fs[i].fp=0;

        return(i);
    }
}
//If it is not the first file then increase the current fh subscript by 1
if(first != 0){
  curr++;
}

//If the file does not exist then create a new one
if (curr <100){ 
    local.path = (char *) malloc(sizeof(path));
    local.fp=0;    
    local.size = 0;
   fs[curr]= local;
}
  
  if(first == 0){
    first++;
  }
 
    // Return the file handle 
    return(curr);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : sgread
// Description  : Read data from the file
//
// Inputs       : fh - file handle for the file to read from
//                buf - place to put the data
//                len - the length of the read
// Outputs      : number of bytes read, -1 if failure

int sgread(SgFHandle fh, char *buf, size_t len) {

size_t pktlen, rpktlen;
pktlen = SG_DATA_PACKET_SIZE;
   
char rbuf[SG_DATA_PACKET_SIZE] , sbuf[SG_DATA_PACKET_SIZE];
    SG_Node_ID loc, rem;
    SG_Block_ID blkid;
    SG_SeqNum sloc, srem;
    SG_Packet_Status ret;
    SG_System_OP op;
    
    sgLocalSeqno = SG_INITIAL_SEQNO;

printf("File handle is %d\n", fh);
if(fs[fh].path == NULL)
{
    return -1;
}

    if ( fs[fh].curr_blk*(SG_BLOCK_SIZE) == fs[fh].fp ){
      printf("FP is at end of file \n");
      return -1;
    }


    printf("read remid is %d\n", fs[fh].remid[fs[fh].fp/SG_BLOCK_SIZE]);
    printf("read blkid is %d\n", fs[fh].blkid[fs[fh].fp/SG_BLOCK_SIZE]);

      if ( (ret = serialize_sg_packet( SG_NODE_UNKNOWN, // Local ID
                                    fs[fh].remid[fs[fh].fp/SG_BLOCK_SIZE],   // Remote ID
                                    fs[fh].blkid[fs[fh].fp/SG_BLOCK_SIZE],  // Block ID
                                    SG_OBTAIN_BLOCK,  // Operation
                                    sgLocalSeqno++,    // Sender sequence number
                                    SG_SEQNO_UNKNOWN,  // Receiver sequence number
                                    NULL, sbuf, &pktlen)) != SG_PACKT_OK ) {
        logMessage( LOG_ERROR_LEVEL, "sgwrite: failed serialization of packet [%d].", ret );
        return( -1 );
    }


       if (sgServicePost(sbuf, &pktlen, rbuf, &rpktlen ) ) {
                logMessage( LOG_ERROR_LEVEL, "sgwrite: failed packet post" );
                return( -1 );
    }


    if ( (ret = deserialize_sg_packet(&sgLocalSeqno, &rem, &blkid,&op, &sloc, 
                                    &srem, buf, rbuf, rpktlen)) != SG_PACKT_OK ) {
        printf("ret is %d\n",ret );
         //printf("SG_PACKT_OK is %d\n",SG_PACKT_OK );
        logMessage( LOG_ERROR_LEVEL, "sgwrite: failed deserialization of packet [%d]", ret );
        return( -1 );
    }

    fs[fh].fp += SG_BLOCK_SIZE;



    // Return the bytes processed
    return( len );
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : sgwrite
// Description  : write data to the file
//
// Inputs       : fh - file handle for the file to write to
//                buf - pointer to data to write
//                len - the length of the write
// Outputs      : number of bytes written if successful test, -1 if failure

int sgwrite(SgFHandle fh, char *buf, size_t len) {

    // Log the write, return bytes written
    //int ret;
    size_t pktlen, rpktlen;
    pktlen = SG_DATA_PACKET_SIZE;
   
    char rbuf[SG_BASE_PACKET_SIZE] , sbuf[SG_DATA_PACKET_SIZE];
    SG_Node_ID loc, rem;
    SG_Block_ID blkid;
    SG_SeqNum sloc, srem;
    SG_Packet_Status ret;
    SG_System_OP op;
    

    //printf("\n len is %d data is %s\n" , len, buf);

    //Check if handel exists
    if(fs[fh].path == NULL){
        return(-1);
    }
    sgLocalSeqno = SG_INITIAL_SEQNO;
    //printf("\n IN HERE \n");
    //printf("SG_BLOCK_SIZE is %d\n", SG_BLOCK_SIZE);
    
    //printf("size of sbuf is %d\n", sizeof(sbuf));
                // Serialize the buffer
    if ( (ret = serialize_sg_packet( SG_NODE_UNKNOWN, // Local ID
                                    SG_NODE_UNKNOWN,   // Remote ID
                                    SG_BLOCK_UNKNOWN,  // Block ID
                                    SG_CREATE_BLOCK,  // Operation
                                    sgLocalSeqno++,    // Sender sequence number
                                    SG_SEQNO_UNKNOWN,  // Receiver sequence number
                                    buf, sbuf, &pktlen)) != SG_PACKT_OK ) {
        logMessage( LOG_ERROR_LEVEL, "sgwrite: failed serialization of packet [%d].", ret );
        return( -1 );
    }

     rpktlen = SG_BASE_PACKET_SIZE;
    
/*
    printf("\nret from serializations is %d\n", ret);
    printf(" buf is %s\n", buf);
    printf("sbuf is %s\n ", sbuf);
    printf(" 111 rbuf is  %s\n rbuf ***** len is %d  \n", rbuf, sizeof(rbuf));
    printf("pktlen is %d\n", pktlen);
    printf("rpktlen is %d\n", rpktlen);


printf("\n## sbuf ###\n");
fwrite(sbuf, 1, 1065, stdout);
printf("\n## END sbuf ###\n\n");

printf("pktlen is %d\n", pktlen);
printf("rpktlen is %d\n", rpktlen);*/




    if (sgServicePost(sbuf, &pktlen, rbuf, &rpktlen ) ) {
                logMessage( LOG_ERROR_LEVEL, "sgwrite: failed packet post" );
                return( -1 );
    }

  /*  printf("AFTER sgServicePost \n");
printf("\n## rbuf ###\n");
fwrite(rbuf, 1, 1065, stdout);
printf("\n## END rbuf ###\n\n");
printf("rpktlen is %d\n", rpktlen);
    */
    


    if ( (ret = deserialize_sg_packet(&sgLocalSeqno, &rem, &blkid,&op, &sloc, 
                                    &srem, NULL, rbuf, rpktlen)) != SG_PACKT_OK ) {
        printf("ret is %d\n",ret );
         //printf("SG_PACKT_OK is %d\n",SG_PACKT_OK );
        logMessage( LOG_ERROR_LEVEL, "sgwrite: failed deserialization of packet [%d]", ret );
        return( -1 );
    }
  //  printf("222 rbuf is %s\n", rbuf);
  printf("\nrem is %d\n", rem);
  printf("\nblkid %d\n", blkid);
  printf("Locid is %d\n", loc);
    // Sanity check the return value
    if ( loc == SG_NODE_UNKNOWN ) {
        logMessage( LOG_ERROR_LEVEL, "sgwrite: bad local ID returned [%ul]", loc );
        return( -1 );
    }

            fs[fh].remid[fs[fh].fp/SG_BLOCK_SIZE] = rem;  //Remote Node ID
            fs[fh].blkid[fs[fh].fp/SG_BLOCK_SIZE] = blkid;  //Block Id
            fs[fh].fp += SG_BLOCK_SIZE; // File pointer
            fs[fh].size += len;     //File size (Actual # of characters)
            fs[fh].curr_blk += 1;  // Current Blk
  


    return(len);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : sgseek
// Description  : Seek to a specific place in the file
//
// Inputs       : fh - the file handle of the file to seek in
//                off - offset within the file to seek to
// Outputs      : new position if successful, -1 if failure

int sgseek(SgFHandle fh, size_t off) {

    if(fs[fh].path == NULL)
    {
      return -1;
    }

    if (off > fs[fh].size){
      return -1;
    }

    fs[fh].fp = off;

    // Return new position
    return( off );
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : sgclose
// Description  : Close the file
//
// Inputs       : fh - the file handle of the file to close
// Outputs      : 0 if successful test, -1 if failure

int sgclose(SgFHandle fh) {

    if(fs[fh].path == NULL)
    {
      return -1;
    }
    fs[fh].fp=0;



    // Return successfully
    return( 0 );
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : sgshutdown
// Description  : Shut down the filesystem
//
// Inputs       : none
// Outputs      : 0 if successful test, -1 if failure

int sgshutdown(void) {
  // Local variables
    char initPacket[SG_BASE_PACKET_SIZE], recvPacket[SG_BASE_PACKET_SIZE];
    size_t pktlen, rpktlen;
    SG_Node_ID loc, rem;
    SG_Block_ID blkid;
    SG_SeqNum sloc, srem;
    SG_System_OP op;
    SG_Packet_Status ret;

    // Local and do some initial setup
    logMessage( LOG_INFO_LEVEL, "sgshutdown local endpoint ..." );
    sgLocalSeqno = SG_INITIAL_SEQNO;

    // Setup the packet
    pktlen = SG_BASE_PACKET_SIZE;
   // printf("IN   sgInitEndpoint    \n");
    //printf("\n sgInitEndpoint pktlen is %d\n",pktlen );
    if ( (ret = serialize_sg_packet( SG_NODE_UNKNOWN, // Local ID
                                    SG_NODE_UNKNOWN,   // Remote ID
                                    SG_BLOCK_UNKNOWN,  // Block ID
                                    SG_STOP_ENDPOINT,  // Operation
                                    sgLocalSeqno++,    // Sender sequence number
                                    SG_SEQNO_UNKNOWN,  // Receiver sequence number
                                    NULL, initPacket, &pktlen)) != SG_PACKT_OK ) {
        logMessage( LOG_ERROR_LEVEL, "sgshutdown: failed serialization of packet [%d].", ret );
        return( -1 );
    }

    // Send the packet
    rpktlen = SG_BASE_PACKET_SIZE;
    /*
     printf("\n sgInitEndpoint rpktlen is %d\n",rpktlen );
     printf("\n sgInitEndpoint pktlen is %d\n", pktlen );
     printf("\n sgInitEndpoint initPacket is %s\n",initPacket );
     printf("\n sgInitEndpoint recvPacket is %s\n", recvPacket );
     */
    if ( sgServicePost(initPacket, &pktlen, recvPacket, &rpktlen) ) {
        logMessage( LOG_ERROR_LEVEL, "sgshutdown: failed packet post" );
        return( -1 );
    }

     //printf("\n 222 sgInitEndpoint rpktlen is %d\n",rpktlen );
     //printf("\n 222 sgInitEndpoint pktlen is %d\n", pktlen );
     //printf("\n 222 sgInitEndpoint initPacket is %s\n",initPacket );
     //printf("\n 222 sgInitEndpoint recvPacket is %s\n", recvPacket );
    

    // Unpack the recieived data
    //printf("sgInitEndpoint initPacket is %s\n", initPacket);
    //printf("sgInitEndpoint recvPacket is %s\n", recvPacket);
    if ( (ret = deserialize_sg_packet(&sgLocalSeqno, &rem, &blkid, &op, &sloc, 
                                    &srem, NULL, recvPacket, rpktlen)) != SG_PACKT_OK ) {
        //printf("\n sgInitEndpoint ret is %d\n",ret );
         //printf("\n sgInitEndpoint SG_PACKT_OK is %d\n",SG_PACKT_OK );
        logMessage( LOG_ERROR_LEVEL, "sgshutdown: failed deserialization of packet [%d]", ret );
        return( -1 );
    }

    // Sanity check the return value
    if ( loc == SG_NODE_UNKNOWN ) {
        logMessage( LOG_ERROR_LEVEL, "sgshutdown: bad local ID returned [%ul]", loc );
        return( -1 );
    }

    // Set the local node ID, log and return successfully
    sgLocalNodeId = loc;
    //printf("loc is %u sgLocalNodeId is %d\n", loc, sgLocalNodeId);
    //printf("blkid is %d\n", blkid);
    //printf("rem is %d\n", rem);
    


    // Log, return successfully
    logMessage( LOG_INFO_LEVEL, "Shut down Scatter/Gather driver." );
    return( 0 );
}

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

//--------------------------Original--------------------------
SG_Packet_Status serialize_sg_packet(SG_Node_ID loc, SG_Node_ID rem,
                                     SG_Block_ID blk, SG_System_OP op,
                                     SG_SeqNum sseq, SG_SeqNum rseq, char *data,
                                     char *packet, size_t *plen) {
/*
  printf("#### plen is %d\n", *plen);
  printf("#### packet is %s\n", packet);
  printf("#### data is %s\n", data);
  printf("#### SG_MAGIC_VALUE is %d\n", SG_MAGIC_VALUE);
  */
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
//printf("#### 111 packet is %s\n", &packet[0]);

  memcpy(&packet[4], &loc, 8);
//printf("#### 111.000 packet is %s\n", &packet[0]);
  memcpy(&packet[12], &rem, 8);
//printf("#### 111.000 packet is %s\n", &packet[0]);
  //printf("#### 111.000 packet is %s\n", &packet[0]);
  memcpy(&packet[20], &blk, 8);
//printf("#### 111.000 packet is %s\n", &packet[0]);
  memcpy(&packet[28], &op, 4);
//printf("#### 111.000 packet is %s\n", &packet[0]);
  memcpy(&packet[32], &sseq, 2);

  memcpy(&packet[34], &rseq, 2);

  memcpy(&packet[36], &data_indicator, 1);

//printf("#### 222 packet is %s\n", packet);
  if (data_indicator == 1) {
 //   printf("%d\n", SG_BLOCK_SIZE);
    memcpy(&packet[37], data, SG_BLOCK_SIZE);
 //   printf("#### 222.0000 data is %s\n", data);
 //   printf("#### 222.00000 packet is %s\n", &packet[37]);
    memcpy(&packet[37+SG_BLOCK_SIZE], &magic, 4);
  //  printf("#### 222.00001 packet is %s\n", &packet[1061]);
    *plen = SG_DATA_PACKET_SIZE;
  }
  if (data_indicator == 0) {
    memcpy(&packet[37], &magic, 4);
    *plen = SG_BASE_PACKET_SIZE;
  }

 // printf("\n## PACKET ###\n");
//fwrite(packet, 1, 1061, stdout);
//printf("\n## END PACKET ###\n");
  //packet=packet-SG_DATA_PACKET_SIZE;
 
  return (SG_PACKT_OK);
}


//-----------------------------------Original ^^^^^^------------------------------------------------
/*
SG_Packet_Status xserialize_sg_packet(SG_Node_ID loc, SG_Node_ID rem, SG_Block_ID blk,
                                     SG_System_OP op, SG_SeqNum sseq, SG_SeqNum rseq, char *data,
                                     char *packet, size_t *plen) {

// REPLACE THIS CODE WITH YOU ASSIGNMENT #2

    // Return the system function return value
  //printf("111 \n") ;
    printf("\n in serialise begginning of data %d\n", strlen(data));
    printf("data in serialize %s\n", data);

  int8_t data_indicator = 0;

  if (loc == 0)
    return (SG_PACKT_LOCID_BAD);
  if (rem == 0)
    return (SG_PACKT_REMID_BAD);
  if (blk == 0)
    return (SG_PACKT_BLKID_BAD);

//printf("222 \n") ;

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

//printf("333 \n") ;

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


//printf("444 \n") ;


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


printf("555 packet is %s\n", packet) ;

  if (data_indicator == 1) {
    printf("%d\n", SG_BLOCK_SIZE);

printf("data is %s\n", data);
printf("\n in serialise size of data %d\n", sizeof(data));
    memcpy(packet[37], data, SG_BLOCK_SIZE);
    memcpy(&packet[37 + SG_BLOCK_SIZE], &magic, 4);
  }
printf("\n packet is %s\n", packet);
//printf("666 \n") ;

  if (data_indicator == 0) {
    memcpy(&packet[37], &magic, 4);
  }

//printf(" 777 \n") ;
  printf("\n66666size of packet %d\n", sizeof(packet));
  *plen = SG_BASE_PACKET_SIZE ;
  //printf("plen in serial is %d\n",*plen );


//printf("888 \n") ;

  return (SG_PACKT_OK);
}
*/
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

//--------------------------Original--------------------------

SG_Packet_Status deserialize_sg_packet(SG_Node_ID *loc, SG_Node_ID *rem,
                                       SG_Block_ID *blk, SG_System_OP *op,
                                       SG_SeqNum *sseq, SG_SeqNum *rseq,
                                       char *data, char *packet, size_t plen) {
/*

 printf("\n DESERIALIZED packet ###\n");
fwrite(packet, 1, 41, stdout);
printf("\n## DESERIALIZED END packet ###\n");*/


  uint32_t magic = SG_MAGIC_VALUE;
  int8_t data_indicator;

  memcpy(&magic, &packet[0], 4);
  //printf("\n magic is %d\n",magic);
  memcpy(loc, &packet[4], 8);
   //printf("\n loc is %d\n",loc);
  memcpy(rem, &packet[12], 8);
   //printf("\n rem is %d\n",rem);
  memcpy(blk, &packet[20], 8);
   //printf("\n blk is %d\n",blk);
   //printf("\n op is %d\n",op);
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
/*
  printf("@@@@@ plen is %d\n", plen);*/
  if ( ( plen != SG_BASE_PACKET_SIZE) &&  (plen != SG_DATA_PACKET_SIZE)) {
    return (SG_PACKT_BLKLN_BAD);
  }

  return (SG_PACKT_OK);
}













//-----------------------------------Original ^^^^^^------------------------------------------------

SG_Packet_Status xdeserialize_sg_packet(SG_Node_ID *loc, SG_Node_ID *rem, SG_Block_ID *blk,
                                       SG_System_OP *op, SG_SeqNum *sseq, SG_SeqNum *rseq, char *data,
                                       char *packet, size_t plen) {

// REPLACE THIS CODE WITH YOU ASSIGNMENT #2

    // Return the system function return value

  //printf("\nIn deserialize_sg_packet\n");
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
//printf("\n magic value is %d\n", magic);
  if (magic != SG_MAGIC_VALUE) {
    return (SG_PACKT_PDATA_BAD);
  }

  if (data_indicator == 1 && data == NULL) {
    return (SG_PACKT_BLKDT_BAD);
  }
// printf("\nplen in deserial is  %d\n",plen );
// printf("\nthe total block size %d\n",SG_DATA_PACKET_SIZE);

  if (plen != SG_BASE_PACKET_SIZE ) {
    return (SG_PACKT_BLKLN_BAD);
  }
 // printf("\nplen in deserial2 is  %d\n",plen );

  return (SG_PACKT_OK);
}

//
// Driver support functions

////////////////////////////////////////////////////////////////////////////////
//
// Function     : sgInitEndpoint
// Description  : Initialize the endpoint
//
// Inputs       : none
// Outputs      : 0 if successfull, -1 if failure

int sgInitEndpoint( void ) {

    // Local variables
    char initPacket[SG_BASE_PACKET_SIZE], recvPacket[SG_BASE_PACKET_SIZE];
    size_t pktlen, rpktlen;
    SG_Node_ID loc, rem;
    SG_Block_ID blkid;
    SG_SeqNum sloc, srem;
    SG_System_OP op;
    SG_Packet_Status ret;

    // Local and do some initial setup
    logMessage( LOG_INFO_LEVEL, "Initializing local endpoint ..." );
    sgLocalSeqno = SG_INITIAL_SEQNO;

    // Setup the packet
    pktlen = SG_BASE_PACKET_SIZE;
   // printf("IN   sgInitEndpoint    \n");
    //printf("\n sgInitEndpoint pktlen is %d\n",pktlen );
    if ( (ret = serialize_sg_packet( SG_NODE_UNKNOWN, // Local ID
                                    SG_NODE_UNKNOWN,   // Remote ID
                                    SG_BLOCK_UNKNOWN,  // Block ID
                                    SG_INIT_ENDPOINT,  // Operation
                                    sgLocalSeqno++,    // Sender sequence number
                                    SG_SEQNO_UNKNOWN,  // Receiver sequence number
                                    NULL, initPacket, &pktlen)) != SG_PACKT_OK ) {
        logMessage( LOG_ERROR_LEVEL, "sgInitEndpoint: failed serialization of packet [%d].", ret );
        return( -1 );
    }

    // Send the packet
    rpktlen = SG_BASE_PACKET_SIZE;
    /*
     printf("\n sgInitEndpoint rpktlen is %d\n",rpktlen );
     printf("\n sgInitEndpoint pktlen is %d\n", pktlen );
     printf("\n sgInitEndpoint initPacket is %s\n",initPacket );
     printf("\n sgInitEndpoint recvPacket is %s\n", recvPacket );
     */
    if ( sgServicePost(initPacket, &pktlen, recvPacket, &rpktlen) ) {
        logMessage( LOG_ERROR_LEVEL, "sgInitEndpoint: failed packet post" );
        return( -1 );
    }

     //printf("\n 222 sgInitEndpoint rpktlen is %d\n",rpktlen );
     //printf("\n 222 sgInitEndpoint pktlen is %d\n", pktlen );
     //printf("\n 222 sgInitEndpoint initPacket is %s\n",initPacket );
     //printf("\n 222 sgInitEndpoint recvPacket is %s\n", recvPacket );
    

    // Unpack the recieived data
    //printf("sgInitEndpoint initPacket is %s\n", initPacket);
    //printf("sgInitEndpoint recvPacket is %s\n", recvPacket);
    if ( (ret = deserialize_sg_packet(&loc, &rem, &blkid, &op, &sloc, 
                                    &srem, NULL, recvPacket, rpktlen)) != SG_PACKT_OK ) {
        //printf("\n sgInitEndpoint ret is %d\n",ret );
         //printf("\n sgInitEndpoint SG_PACKT_OK is %d\n",SG_PACKT_OK );
        logMessage( LOG_ERROR_LEVEL, "sgInitEndpoint: failed deserialization of packet [%d]", ret );
        return( -1 );
    }

    // Sanity check the return value
    if ( loc == SG_NODE_UNKNOWN ) {
        logMessage( LOG_ERROR_LEVEL, "sgInitEndpoint: bad local ID returned [%ul]", loc );
        return( -1 );
    }

    // Set the local node ID, log and return successfully
    sgLocalNodeId = loc;
    //printf("loc is %u sgLocalNodeId is %d\n", loc, sgLocalNodeId);
    //printf("blkid is %d\n", blkid);
    //printf("rem is %d\n", rem);
    logMessage( LOG_INFO_LEVEL, "Completed initialization of node (local node ID %lu", sgLocalNodeId );
    //printf("END   sgInitEndpoint    \n");
    return( 0 );
}
