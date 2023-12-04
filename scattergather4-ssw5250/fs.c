#include <sg_driver.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>



 SgFHandle curr = 0;
 int first=0;

typedef struct 
{
    char path[100];
    //char *data[20];
    int fp;  //File pointer read
    int size; //Actual size of the fileSG_Node_ID loc, rem;
    SG_Block_ID blkid[20];
    SG_Node_ID remid[20];
    int curr_blk; // Current block
} File;



typedef struct {
  File **file;
  int used;
  int size;
} Files;

Files *fs;

void initFiles(Files *f, size_t initialSize) {
	//pt = malloc( sizeof  *pt * 10 ); 
  f->file = malloc(initialSize * sizeof(*f->file));
  f->used = 0;
  f->size = initialSize;
}

void insertFiles(Files *f, File *file) {
  // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
  // Therefore a->used can go up to a->size 
  printf("=== size is %d used is %d\n", fs->size, fs->used);
  if (f->used == f->size) {
    f->size *= 2;
    printf("XXXX\n");
    f->file = realloc(*f->file, f->size * sizeof(*f->file));
    printf("YYYY\n");
  }
  f->file[f->used++] = file;
  printf("+++ size is %d used is %d\n", fs->size, fs->used);
}

void freeFiles(Files *f) {
  free(f->file);
  f->file = NULL;
  f->used = f->size = 0;
}

File *getFile(Files *f, int i){
	return(f->file[i]);
}

//////////////////
//sgopen   
//////////////////
SgFHandle sgopen(const char *path){
		//FILL IN THE REST OF THE CODE
File *local;
local = malloc(sizeof(File));
  
//Check if file exists. If it exists then return the file handle   
for(int i = 0; i < fs->size && i < curr; i++){
  printf("i is %d size is %d curr is %d used is %d file is %s\n", i, fs->size, curr, fs->used, fs->file[i]->path);
    if ( !(strcmp(fs->file[i]->path, path) ) ) {     
        return(i);
    }
}
//If it is not the first file then increase the current fh subscript by 1
if(first != 0){
  curr++;
  //printf("sgopen curr is %d \n", curr);
}

//If the file does not exist then create a new one

    strcpy(local->path, path);
    local->fp=0;    
    local->size = 0;

   insertFiles(fs,local);
   //fs->file[curr] = local;
   //fs->size += 1;
  
  if(first == 0){
    first++;
  }
 
    // Return the file handle 
  //printf("curr is %d\n", curr);
    return(curr);
}

/////////////////////////
// M A I N
/////////////////////////
void main(){

	
	fs = malloc(sizeof(Files));
  fs->size=0;
	initFiles(fs, 3);

	int a = sgopen("AAAA");
	printf("main File AAAA is %d\n", a);

	int b =  sgopen("BBBB");
	printf("main File BBBB is %d\n", b);
	
	int c = sgopen("CCCC");
	printf("main File CCCC is %d\n", c);
	
	int d = sgopen("DDDD");
	printf("main File DDDD is %d\n", d);
	
  /*
  File *gf;
  for(int i=0;i<4;i++){
    printf("gf i is %d \n", i);
    gf = getFile(fs, i);
    printf("main for lop File %d is %s\n", i, gf->path);
  }
*/
  int x = sgopen("DDDD");
  printf("222 main File DDDD is %d\n", x);
}


