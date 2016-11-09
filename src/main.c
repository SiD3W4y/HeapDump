#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 256
#define OFFSET_SIZE 13
#define CYCLE_SIZE 512

typedef struct Address{
	long begin;
	long end;
}Address;

Address parse_line(char* line)
{
	Address addr;
	char *buff = malloc(OFFSET_SIZE);
	
	memmove(buff,line,12);
	buff[12] = '\0';
	addr.begin = strtol(buff,NULL,16);

	memmove(buff,line+OFFSET_SIZE,12);
	buff[12] = '\0';
	addr.end = strtol(buff,NULL,16);
	
	free(buff);

	return addr;
}

void dump(char *pid,char *name,char *output)
{
	char* path = malloc(LINE_LENGTH+1);
	char* line = malloc(LINE_LENGTH+1);
	char* buff = malloc(CYCLE_SIZE);
	FILE* map;
	FILE* mem;
	FILE* op;
	int count = 0;
	Address addr;
	
	sprintf(path,"/proc/%s/maps",pid);
	map = fopen(path,"r");

	if(map == NULL){
		perror("Cannot open map file");
		exit(-1);
	}
	
	sprintf(path,"/proc/%s/mem",pid);
	mem = fopen(path,"r");
	
	if(mem == NULL){
		perror("Cannot open process memory, maybe run as root ?");
		exit(-1);
	}


	op = fopen(output,"wb");

	if(op == NULL){
		perror("Cannot open output file");
		exit(-1);
	}
	
	while(fgets(line,LINE_LENGTH,map)){
		if(strstr(line,name)){
			addr = parse_line(line);
			long size = addr.end-addr.begin;
			int cycles = (size-(size%CYCLE_SIZE))/CYCLE_SIZE;

			printf("Dumping segment n°%d\n",count);
			count++;

			fseek(mem,addr.begin,SEEK_SET);

			for(int i=0;i < cycles;i++){
				fread(buff,CYCLE_SIZE,1,mem);
				fwrite(buff,CYCLE_SIZE,1,op);
			}

			fread(buff,size%CYCLE_SIZE,1,mem);
			fwrite(buff,size%CYCLE_SIZE,1,op);
		}
	}
	

	if(mem == NULL){
		perror("Cannot open file");
	}

	fclose(map);
	fclose(op);
	fclose(mem);
	free(buff);
	free(line);
	free(path);
}

int main(int argc,char** argv)
{
	if(argc<4){
		printf("Usage : %s <pid> <section name> <output file>\n",argv[0]);
		exit(-1);
	}
	
	if(strlen(argv[1]) > 5){
		perror("Wrong pid");
		exit(-1);
	}
	dump(argv[1],argv[2],argv[3]);
	return 0;
}

