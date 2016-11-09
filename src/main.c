#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 256
#define OFFSET_SIZE 13

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

void dump_stack(char *pid,char *output)
{
	char* path = malloc(LINE_LENGTH+1);
	char* line = malloc(LINE_LENGTH+1);
	char* stack;
	FILE* map;
	FILE* mem;
	FILE* op;
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
		if(strstr(line,"[stack]")){
			addr = parse_line(line);
		}
	}
	

	if(mem == NULL){
		perror("Cannot open file");
	}

	fclose(map);
	free(line);
	free(path);
}

int main(int argc,char** argv)
{
	if(argc<3){
		perror("Not enough args");
		exit(-1);
	}
	
	if(strlen(argv[1]) > 5){
		perror("Wrong pid");
		exit(-1);
	}
	dump_stack(argv[1],argv[2]);
	return 0;
}

