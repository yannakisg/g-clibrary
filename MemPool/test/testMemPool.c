#include <stdio.h>

#include "../src/mempool.h"

/* Foo struct. Only for educational purposes:P */
typedef struct foo_ {
	struct foo_ *next;
	struct foo_ *prev;
	int k;
	int l;
	void *data;
	struct foo_ *parent;
}Foo;

#define BLOCKS 20

int main() {
	
	printf("Sizeof Foo => %X\n", sizeof(Foo));
	
	Foo *f1,*f2,*f3,*f4,*f5;
	int res = allocMem(BLOCKS ,sizeof(Foo));
	
	if (res != 0) {
		switch(res) {
			case VARERR: fprintf(stderr, "Invalid arguments\n"); break;
			case MEMERRPOOL: fprintf(stderr, "Error while allocating space for MemPool\n");break;
			case MEMERRLPOOL: fprintf(stderr, "Error while allocating space for LMemPool\n");break;
			case MEMERRBLOCK: fprintf(stderr, "Error while allocating space for memBlock\n");break;
			default: fprintf(stderr, "Error\n");
		}
		
		return 1;
	}
	
	f1 = (Foo *)getMemNode();
	if (f1 == NULL) {
		fprintf(stderr, "Memory pool is null or an error occurred while creating a new one\n");
		return 1;
	}
	
	f2 = (Foo *)getMemNode();
	if (f2 == NULL) {
		fprintf(stderr, "Memory pool is null or an error occurred while creating a new one\n");
		return 1;
	}
	
	
	printf("f1 => %p\n",f1);
	printf("f2 => %p\n",f2);
	
	releaseMemNode((void *)f1);
	
	f3 = (Foo *) getMemNode();
	if (f3 == NULL) {
		fprintf(stderr, "Memory pool is null or an error occurred while creating a new one\n");
		return 1;
	}
	f4 = (Foo *) getMemNode();
	if (f4 == NULL) {
		fprintf(stderr, "Memory pool is null or an error occurred while creating a new one\n");
		return 1;
	}
	
	printf("f3 => %p\n",f3);
	printf("f4 => %p\n",f4);
	
	releaseMemNode((void *)f3);
	releaseMemNode((void *)f4);
	
	freeMemory();
	
	res = allocMem(BLOCKS ,sizeof(Foo));	
	if (res != 0) {
		switch(res) {
			case VARERR: fprintf(stderr, "Invalid arguments\n"); break;
			case MEMERRPOOL: fprintf(stderr, "Error while allocating space for MemPool\n");break;
			case MEMERRLPOOL: fprintf(stderr, "Error while allocating space for LMemPool\n");break;
			case MEMERRBLOCK: fprintf(stderr, "Error while allocating space for memBlock\n");break;
			default: fprintf(stderr, "Error\n");
		}
		
		return 1;
	}
	
	f5 = (Foo *)getMemNode();
	if (f5 == NULL) {
		fprintf(stderr, "Memory pool is null or an error occurred while creating a new one\n");
		return 1;
	}
	
	printf("f5 => %p\n",f5);
	
	releaseMemNode((void *)f5);
	
	freeMemory();
	
	return 0;
}
