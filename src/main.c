#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "vm.h"
#include "lru.h"
#include "debug.h"

static int LOOP;

void vm_init_test(int lru)
{
	vm_init(lru);
	INFO("---init---");
}

void vm_load_data_test()
{
	double cost;
	clock_t begin,end;
	begin=clock();
	vm_load_data();
	end=clock();
	cost=(double)(end-begin);
	printf("---load cost:%lf\n",cost);
}

void vm_put_test()
{
	char* key="key1";
	char* value="val1";
	vm_put(key,value);

	key="key2";
	value="val2";
	vm_put(key,value);
	INFO("---vm put ---");
	
}

void vm_bulk_put_test()
{
	double cost;
	int i;
	char key[256]={0};
	char value[256]={0};
	clock_t begin,end;
	begin=clock();
	for(i=0;i<LOOP;i++)
	{
		sprintf(key,"xxxxxxxxxx%d",i);
		sprintf(value,"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx%d",i);
		vm_bulk_put(key,value);
	}
	vm_bulk_flush();
	end=clock();
    cost=(double)(end-begin);
	printf("put cost:%lf\n",cost);
	INFO("---vm bulk put ---");
}

void vm_get_test()
{
	double cost;
	char key[256]="xxxxxxxxxxxxxxxxx2";
	char value[256]={0};
	int i;
	clock_t begin,end;
	begin=clock();
	for(i=1;i<LOOP;i++)
	{
		sprintf(key,"xxxxxxxxxx%d",rand()%i);
		int  ret=vm_get(key,value);
		if(ret)
			LOG("v is:%s----%s",key,value);
		else
			INFO("nofound!");
	}
	end=clock();
    	cost=(double)(end-begin);
	printf("get cost:%lf\n",cost);
}

void lru_test()
{
	double cost;
	int i;
	char key[256]={0};
	clock_t begin,end;
	begin=clock();
	for(i=0;i<LOOP;i++)
	{
		sprintf(key,"xxxxx%d",i);

		char* v=lru_find(key);
		if(!v)
			lru_add(key,key);
	}
	end=clock();
	cost=(double)(end-begin);
	printf("lru cost:%lf\n",cost);
}

int main(int argc,char**argv)
{
	int lru=0;
	if(argc!=4)
	{
		printf("arg error,format--->loopcount opt lru...\n");
		return 1;
	}
	LOOP=atoi(argv[1]);
	if(strcmp(argv[3],"lru")==0)
		lru=1;
	//lru_test();
	vm_init_test(lru);
	//vm_put_test();
	if(strcmp(argv[2],"put")==0)
		vm_bulk_put_test();
	if(strcmp(argv[2],"load")==0)
		vm_load_data_test();

	vm_get_test();
	return (1);
}
