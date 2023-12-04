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
int wcount=0, rcount=0;
int sgDriverInitialized = 0; // The flag indicating the driver initialized
SG_Block_ID sgLocalNodeId;   // The local node identifier
SG_SeqNum sgLocalSeqno;      // The local sequence number

struct file
{
    char *path;
    //char *data[20];
    int fp;  //File pointer read
    int size; //Actual size of the fileSG_Node_ID loc, rem;
    SG_Block_ID blkid[20];
    int halfBlock[20];
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
struct file local;
  
//Check if file exists. If it exists then return the file handle   
for(int i = 0; i < 100 && i < curr; i++){
    if ( !(strcmp(fs[i].path, path) ) ) {     
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
    strcpy(local.path, path);
    local.fp=0;    
    local.size = 0;
    for(int j=0; j <20; j++){
            local.halfBlock[j]=0;
    }
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
char rbuf[SG_DATA_PACKET_SIZE] , sbuf[SG_DATA_PACKET_SIZE], tbuf[SG_DATA_PACKET_SIZE];
char* cbuf;
    SG_Node_ID loc, rem;

    SG_Block_ID blkid;
    SG_SeqNum sloc, srem;
    SG_Packet_Status ret;
    SG_System_OP op;
    
    sgLocalSeqno = SG_INITIAL_SEQNO;

printf("\n\n****   R E A D rcount is %d *****  \n\n", ++rcount);
 printf("\n READ %d chars to node %d and blockid %d . fp is %d size is %d\n", len, rem, blkid, fs[fh].fp, fs[fh].size );
//printf("***** fp is %d file is %d node is %d block is %d name is %s\n",  fs[fh].fp, fh, fs[fh].remid[fs[fh].fp/SG_BLOCK_SIZE], fs[fh].blkid[fs[fh].fp/SG_BLOCK_SIZE], fs[fh].path);
cbuf = getSGDataBlock(fs[fh].remid[fs[fh].fp/SG_BLOCK_SIZE], fs[fh].blkid[fs[fh].fp/SG_BLOCK_SIZE]);

//fwrite(tbuf, 1, 512, stdout);
  //printf("\nfgrrrrgrgrg\n");

/*if (cbuf != NULL){
    printf("\n## CACHE cbuf ###\n");
    printf("xxx %c xx\n", cbuf[0]);
    fwrite(cbuf, 1, 1024, stdout);
    printf("\n\n");
}*/

/*    if ((fs[fh].fp % 1024) ==0)
    {
        memcpy(buf,tbuf,512);
        //printf("\n## READ  first half of buf ###\n");
            //printf("\nhiiii\n");

        //fwrite(buf, 1, 512, stdout);
        //printf("\n\n");
    }
    if ((fs[fh].fp % 1024) ==512)
    {
            //printf("\nhere\n");

        //printf("\n## READ second half of buf ###\n");

        memcpy(buf,tbuf+512,512);
        //fwrite(buf, 513, 1024, stdout);
        //printf("\n\n");
    }
    //printf("\n===== FOUND IT IN CACHE ================\n\n");
    return(512);
}*/
/*else {
    printf("\nAllocating mem to tbuf \n\n");
    tbuf = (char *) malloc(sizeof(SG_DATA_PACKET_SIZE));
}*/
     //printf("\n len is %d buf is %s\n\n",len, buf );
//printf("***** fp is %d file is %d node is %d block is %fname is %s\n",  fs[fh].fp, fh, fs[fh].remid[(fs[fh].fp -len )/SG_BLOCK_SIZE], fs[fh].blkid[(fs[fh].fp - len )/SG_BLOCK_SIZE], fs[fh].path);
//printf("**** fp is %d len is %d fh is %d fname is %s\n\n", fs[fh].fp, len, fh, fs[fh].path );
//printf("node is %d block is %d\n\n", fs[fh].remid[fs[fh].fp/SG_BLOCK_SIZE], fs[fh].blkid[fs[fh].fp/SG_BLOCK_SIZE]);
if(fs[fh].path == NULL)
{
    return -1;
}

    if ( fs[fh].curr_blk*(SG_BLOCK_SIZE) == fs[fh].fp ){
      return -1;
    }
    //printf("r 111\n");
      if ( (ret = serialize_sg_packet( SG_NODE_UNKNOWN, // Local ID
                                    fs[fh].remid[fs[fh].fp/SG_BLOCK_SIZE],   // Remote ID
                                    fs[fh].blkid[fs[fh].fp/SG_BLOCK_SIZE],  // Block ID
                                    SG_OBTAIN_BLOCK,  // Operation
                                    sgLocalSeqno++,    // Sender sequence number
                                    SG_SEQNO_UNKNOWN,  // Receiver sequence number
                                    NULL, sbuf, &pktlen)) != SG_PACKT_OK ) {
        logMessage( LOG_ERROR_LEVEL, "sgread: failed serialization of packet [%d].", ret );
        return( -1 );
    }

//printf("r 222 pktlen is %d\n\n", pktlen);
       if (sgServicePost(sbuf, &pktlen, rbuf, &rpktlen ) ) {
                logMessage( LOG_ERROR_LEVEL, "sgread: failed packet post" );
                return( -1 );
    }


//printf("r 333 pktlen is %d rpktlen is %d\n\n", pktlen, rpktlen);
    if ( (ret = deserialize_sg_packet(&sgLocalSeqno, &rem, &blkid,&op, &sloc, 
                                    &srem, tbuf, rbuf, rpktlen)) != SG_PACKT_OK ) {

        logMessage( LOG_ERROR_LEVEL, "sgread: failed deserialization of packet [%d]", ret );
        return( -1 );
    }
//printf("r rem is %d blkid %d rpktlen is %d \n\n", rem, blkid,rpktlen);

 //printf("\n## READ rbuf ###\n");
//fwrite(rbuf, 1, 1024, stdout);
//printf("X&X&\n\n");

/*printf("\n## READ tbuf ###\n");
fwrite(tbuf, 1, 1024, stdout);
printf("\n\n");*/

if ((fs[fh].fp % 1024) ==0)
{
  memcpy(buf,tbuf,512);
  //printf("\n## READ  first half of buf ###\n");

  //fwrite(buf, 1, 512, stdout);
//printf("\n\n");
}
if ((fs[fh].fp % 1024) ==512)
{
//printf("\n## READ second half of buf ###\n");

 memcpy(buf,tbuf+512,512);
 //fwrite(buf, 513, 1024, stdout);
//printf("\n\n");
}

//memcpy(tbuf, buf, 1024);
    fs[fh].fp += len;
    //printf("$$$$$$$$$$$$$$$  fh is %d, fp is %d $$$$$$ \n\n", fh, fs[fh].fp );
    //fs[fh].fp += SG_BLOCK_SIZE;

//printf("r 444\n");
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

    //printf("buf is %d\n",buf );
    size_t pktlen, rpktlen;

    pktlen = SG_DATA_PACKET_SIZE;
    //pktlen = SG_BASE_PACKET_SIZE + len;
  /*   printf("\nKILL MEE NOW## 2 buf ###\n");
            fwrite(buf, 1, 512, stdout);
            printf("\n\n");
*/


    
   
    char rbuf[SG_DATA_PACKET_SIZE] , sbuf[SG_DATA_PACKET_SIZE], tbuf[pktlen], obbuf[SG_DATA_PACKET_SIZE];
    SG_Node_ID loc, rem;
    SG_Block_ID blkid;
    SG_SeqNum sloc, srem;
    SG_Packet_Status ret;
    SG_System_OP op;
    
printf("\n\n****   W R I T E wcount is %d *****  \n\n", ++wcount);
    //Check if handel exists
    if(fs[fh].path == NULL){
        return(-1);
    }
    sgLocalSeqno = SG_INITIAL_SEQNO;
    //printf("\n halfblock is %d\n", fs[fh].halfBlock[fs[fh].size/SG_BLOCK_SIZE]);
/*
    if (fs[fh].halfBlock[fs[fh].fp/SG_BLOCK_SIZE] == 2){
        fs[fh].halfBlock[fs[fh].fp/SG_BLOCK_SIZE] = 0;
    }*/
    //printf("\n @@@@@ SIZE IS @@@@@ %d\n", fs[fh].size);
    if ((fs[fh].size/512) % 2 == 0){
        //printf("111\n");
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
    
    //printf("\n 111.000 pktlen is %d\n",  pktlen);

    if (sgServicePost(sbuf, &pktlen, rbuf, &rpktlen ) ) {
                logMessage( LOG_ERROR_LEVEL, "sgwrite: failed packet post" );
                return( -1 );
    }
    //printf("\n 111.111 pktlen is %d rpktlen is %d\n",  pktlen, rpktlen);

    if ( (ret = deserialize_sg_packet(&sgLocalSeqno, &rem, &blkid,&op, &sloc, 
                                    &srem, NULL, rbuf, rpktlen)) != SG_PACKT_OK ) {
        //printf("ret is %d\n",ret );
         //printf("SG_PACKT_OK is %d\n",SG_PACKT_OK );
        logMessage( LOG_ERROR_LEVEL, "sgwrite: failed deserialization of packet [%d]", ret );
        return( -1 );
    }
    //printf("\n 111.333 rpktlen is %d\n", rpktlen);
    // Sanity check the return value
    if ( loc == SG_NODE_UNKNOWN ) {
        logMessage( LOG_ERROR_LEVEL, "sgwrite: bad local ID returned [%ul]", loc );
        return( -1 );
    }
    
            fs[fh].remid[fs[fh].fp/SG_BLOCK_SIZE] = rem;  //Remote Node ID
            fs[fh].blkid[fs[fh].fp/SG_BLOCK_SIZE] = blkid;  //Block Id
            fs[fh].halfBlock[fs[fh].size/SG_BLOCK_SIZE] =1;
            fs[fh].fp += len; // File pointer
            fs[fh].size += len;     //File size (Actual # of characters)
            
    /*        printf("222\n");
             printf("\n## WRTIE rbuf ###\n");
    fwrite(rbuf, 41, 1061, stdout);
    printf("\n\n"); */
              printf("\n 1 Written %d chars to node %d and blockid %d . fp is %d size is %d \n", len, rem, blkid, fs[fh].fp, fs[fh].size );
    return(len);

  }

if ((fs[fh].size/512) % 2 != 0){
    //printf("333\n");
    // Serialize the buffer
    //printf("sbuf is size of %d\n",sizeof(sbuf));

    

    if ( (ret = serialize_sg_packet( SG_NODE_UNKNOWN, // Local ID
                                    fs[fh].remid[fs[fh].fp/SG_BLOCK_SIZE],   // Remote ID
                                    fs[fh].blkid[fs[fh].fp/SG_BLOCK_SIZE] ,  // Block ID
                                    SG_OBTAIN_BLOCK,  // Operation
                                    sgLocalSeqno++,    // Sender sequence number
                                    SG_SEQNO_UNKNOWN,  // Receiver sequence number
                                    NULL, sbuf, &pktlen)) != SG_PACKT_OK ) {
        logMessage( LOG_ERROR_LEVEL, "sgwrite: failed serialization of packet [%d].", ret );
        return( -1 );
    }

     pktlen =SG_DATA_PACKET_SIZE;
     rpktlen =SG_DATA_PACKET_SIZE;

  
    

     //printf("333.0000\n");
     //printf("size of sbuf  %d and rbuf is %d pktlen is %d rpktlen is %d\n",sizeof(sbuf),sizeof(rbuf),pktlen,rpktlen);


    if (sgServicePost(sbuf, &pktlen, rbuf, &rpktlen ) ) {
                logMessage( LOG_ERROR_LEVEL, "sgwrite: failed packet post" );
                return( -1 );
    }

    //printf("333.111\n");
    if ( (ret = deserialize_sg_packet(&sgLocalSeqno, &rem, &blkid,&op, &sloc, 
                                    &srem, tbuf, rbuf, rpktlen)) != SG_PACKT_OK ) {
        //printf("ret is %d\n",ret );
         //printf("SG_PACKT_OK is %d\n",SG_PACKT_OK );
        logMessage( LOG_ERROR_LEVEL, "sgwrite: failed deserialization of packet [%d]", ret );
        return( -1 );
    }
    //printf("333.222\n");
    // Sanity check the return value
    if ( loc == SG_NODE_UNKNOWN ) {
        logMessage( LOG_ERROR_LEVEL, "sgwrite: bad local ID returned [%ul]", loc );
        return( -1 );
    }

   /* if ( sgread(fh, &obbuf, 1065) != 1065 ) {
                    logMessage( LOG_ERROR_LEVEL, "\nSG error read failed \n" );
                    return( -1 );
    }


       //printf("aaaaa\n");
             //printf("\n## 2 obbuf ###\n");
            //fwrite(obbuf, 1, 512, stdout);
            //printf("[SHAUN]\n\n");*/

      //fwrite(tbuf, 1, 512, stdout);
        //    printf("\n\n");
        printf("FILE POINTER1 IS %d \n\n",fs[fh].fp);

    if ((fs[fh].fp/512) % 2 != 0)
    {
    printf("FILE POINTER2 IS %d \n\n",fs[fh].fp);


    memcpy(tbuf+512, buf, 512);
      fs[fh].size += len;  
    }
    if((fs[fh].fp/512) % 2 == 0)
    {
         printf("FILE POINTER3 IS %d \n\n",fs[fh].fp);  
        memcpy(tbuf,buf,512);
    }

     /*    printf("444\n");
             printf("\n##----------------------------------------------- 2 rbuf ###\n");
            fwrite(tbuf, 1, 1024, stdout);
            printf("\n\n");
            printf("the size of tbuf  %d and the size of buf is %d\n",sizeof(tbuf),sizeof(buf) );
             printf("\nKILL MEE## 2 buf ###\n");
           */
    


    // UPDATE

    if ( (ret = serialize_sg_packet( SG_NODE_UNKNOWN, // Local ID
                                        fs[fh].remid[fs[fh].fp/SG_BLOCK_SIZE],   // Remote ID
                                        fs[fh].blkid[fs[fh].fp/SG_BLOCK_SIZE] ,  // Block ID
                                        SG_UPDATE_BLOCK,  // Operation
                                        sgLocalSeqno++,    // Sender sequence number
                                        SG_SEQNO_UNKNOWN,  // Receiver sequence number
                                        tbuf, buf, &pktlen)) != SG_PACKT_OK ) {
            logMessage( LOG_ERROR_LEVEL, "sgwrite: failed serialization of packet [%d].", ret );
            return( -1 );
        }

         rpktlen = SG_BASE_PACKET_SIZE;
        
         //printf("555.0000\n");
        if (sgServicePost(buf, &pktlen, rbuf, &rpktlen ) ) {
                    logMessage( LOG_ERROR_LEVEL, "sgwrite: failed packet post" );
                    return( -1 );
        }

        //printf("555.111\n");
             /*   printf("xxxx\n");
                 printf("\n## 3 sbuf ###\n");
                fwrite(sbuf, 0, 1061, stdout);
                printf("\n\n");

                        printf("yyyy\n");
                 printf("\n## 3 rbuf ###\n");
                fwrite(rbuf, 1, 1061, stdout);
                printf("\n\n");
 */
        if ( (ret = deserialize_sg_packet(&sgLocalSeqno, &rem, &blkid,&op, &sloc, 
                                        &srem, NULL, rbuf, rpktlen)) != SG_PACKT_OK ) {
            //printf("ret is %d\n",ret );
             //printf("SG_PACKT_OK is %d\n",SG_PACKT_OK );
            logMessage( LOG_ERROR_LEVEL, "sgwrite: failed deserialization of packet [%d]", ret );
            return( -1 );
        }
        //printf("555.222\n");
        // Sanity check the return value
        if ( loc == SG_NODE_UNKNOWN ) {
            logMessage( LOG_ERROR_LEVEL, "sgwrite: bad local ID returned [%ul]", loc );
            return( -1 );
        }
        putSGDataBlock(rem, blkid, tbuf);



    // END UPDATE






                fs[fh].remid[fs[fh].fp/SG_BLOCK_SIZE] = rem;  //Remote Node ID
                fs[fh].blkid[fs[fh].fp/SG_BLOCK_SIZE] = blkid;  //Block Id
                fs[fh].halfBlock[fs[fh].size/SG_BLOCK_SIZE] =2;
                fs[fh].fp += len; // File pointer
                //fs[fh].size += len;     //File size (Actual # of characters)
                fs[fh].curr_blk += 1;  // Current Blk
            /*    printf("555\n");
                 printf("\n## 3 rbuf ###\n");
                fwrite(rbuf, 41, 1061, stdout);
                printf("\n\n"); */
                  printf("\n 2 Written %d chars to node %d and blockid %d . fp is %d size is %d\n", len, rem, blkid, fs[fh].fp, fs[fh].size );
        return(len);
      }


  
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

    printf("\nSGSEEK file is %d name is %s off is %d size is %d\n", fh, fs[fh].path, off, fs[fh].size );
    if(fs[fh].path == NULL)
    {
      return -1;
    }

    if (off > fs[fh].size){
      return -1;
    }
    printf("\nSGSEEK off is %d file is %s\n", off, fs[fh].path);
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
  
    if ( sgServicePost(initPacket, &pktlen, recvPacket, &rpktlen) ) {
        logMessage( LOG_ERROR_LEVEL, "sgshutdown: failed packet post" );
        return( -1 );
    }


    // Unpack the recieived data
    if ( (ret = deserialize_sg_packet(&sgLocalSeqno, &rem, &blkid, &op, &sloc, 
                                    &srem, NULL, recvPacket, rpktlen)) != SG_PACKT_OK ) {

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
    
    closeSGCache();
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
    memcpy(&packet[37], data, SG_BLOCK_SIZE);
    memcpy(&packet[37+SG_BLOCK_SIZE], &magic, 4);
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
/*

 printf("\n DESERIALIZED packet ###\n");
fwrite(packet, 1, 41, stdout);
printf("\n## DESERIALIZED END packet ###\n");*/


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
  if ( ( plen != SG_BASE_PACKET_SIZE) &&  (plen != SG_DATA_PACKET_SIZE)) {
    return (SG_PACKT_BLKLN_BAD);
  }

  return (SG_PACKT_OK);
}


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
    if ( sgServicePost(initPacket, &pktlen, recvPacket, &rpktlen) ) {
        logMessage( LOG_ERROR_LEVEL, "sgInitEndpoint: failed packet post" );
        return( -1 );
    }

    // Unpack the recieived data
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
    logMessage( LOG_INFO_LEVEL, "Completed initialization of node (local node ID %lu", sgLocalNodeId );

    if (initSGCache(32) == 0) {
        logMessage( LOG_INFO_LEVEL, "Successfully initialized the cache");
    }
    else {
            logMessage( LOG_ERROR_LEVEL, "SG error cache initialization failed, aborting");
            return(-1);
    }
    return( 0 );
}
