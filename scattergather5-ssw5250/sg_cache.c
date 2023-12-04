////////////////////////////////////////////////////////////////////////////////
//
//  File           : sg_driver.c
//  Description    : This file contains the driver code to be developed by
//                   the students of the 311 class.  See assignment details
//                   for additional information.
//
//   Author        : YOUR NAME
//   Last Modified : 
//

// Include Files
#include <stdlib.h>
#include <cmpsc311_log.h>

// Project Includes
#include <sg_cache.h>

currElement=0;
numHits=0; 
numMisses=0;

// Functional Prototypes

//
// Functions

////////////////////////////////////////////////////////////////////////////////
//
// Function     : initSGCache
// Description  : Initialize the cache of block elements
//
// Inputs       : maxElements - maximum number of elements allowed
// Outputs      : 0 if successful, -1 if failure

int initSGCache( uint16_t maxElements ) {

	if (maxElements > SG_MAX_CACHE_ELEMENTS || maxElements < 0){
		return(-1);
	}
	maxElem = maxElements;
	for(int i = 0; i < maxElem; i++){
		cache[i].node = 0;
		cache[i].block = 0;
		cache[i].data = (char *)malloc(1024);
		cache[i].numUsed = 0;
	}

    // Return successfully
    return( 0 );
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : closeSGCache
// Description  : Close the cache of block elements, clean up remaining data
//
// Inputs       : none
// Outputs      : 0 if successful, -1 if failure

int closeSGCache( void ) {
	printf("In closeSGCache \n");
	for(int i = 0; i < maxElem; i++){
		free(cache[i].data);
	}
	maxElem=0;
	printf("numHits are %d, numMisses are %d HIT RATIO IS : %.2f%\n", numHits, numMisses, ((float)numHits/(float)(numHits + numMisses)) * 100 );
    // Return successfully
    return( 0 );
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : getSGDataBlock
// Description  : Get the data block from the block cache
//
// Inputs       : nde - node ID to find
//                blk - block ID to find
// Outputs      : pointer to block or NULL if not found

char * getSGDataBlock( SG_Node_ID nde, SG_Block_ID blk ) {
	printf("\n IN getSGDataBlock \n");
	for(int i = 0; i < maxElem; i++){
		if (cache[i].node  == nde && cache[i].block == blk){
			//ret_data = &(cache[i].data);
			printf("\n\nret_data is ");
			fwrite(cache[i].data, 1, 1024, stdout);
			printf("\n============\n");
			numHits++;
			cache[i].numUsed++;
			printf("cache[%d].numUsed is %d\n", i, cache[i].numUsed);
			return(cache[i].data);
		}
		else{
			numMisses++;
			return(NULL);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : putSGDataBlock
// Description  : Get the data block from the block cache
//
// Inputs       : nde - node ID to find
//                blk - block ID to find
//                block - block to insert into cache
// Outputs      : 0 if successful, -1 if failure

int putSGDataBlock( SG_Node_ID nde, SG_Block_ID blk, char *block ) {
	printf("currElement is %d maxElem is %d\n",currElement, maxElem);
	if(currElement >= maxElem){
		int min, minSubscript = 0;
		min = cache[0].numUsed;
		for(int i = maxElem -1; i >= 0 ; i--){
			if (cache[i].numUsed < min){
				min = cache[i].numUsed;
				minSubscript=i;
			}
		}
		printf("Putting node %d blk %d at subscript %d\n", nde, blk, minSubscript);
		cache[minSubscript].node = nde;
		cache[minSubscript].block = blk;
		cache[minSubscript].numUsed=1;
		memcpy(cache[minSubscript].data, block, 1024);

	}
	else {
			
			cache[currElement].node = nde;
			cache[currElement].block = blk;
			memcpy(cache[currElement].data, block, 1024);
			printf("### CACHED node %d block %d currElement is %d\n\n", cache[currElement].node, cache[currElement].block, currElement );
			currElement++;
		}
    // Return successfully
    return( 0 );
}

/*void main(){
	initSGCache(10);
	printf("SG_MAX_CACHE_ELEMENTS is %d\n", maxElem);

}*/