#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>

char inc = 64;
char dec = -64;

int genhash(char *sz, size_t len)
{
	int r = 0;
	
	for(;len;sz++,len--) r += *sz;
	
	return r;
}

void putall(char *sz, size_t len)
{
	for(;len;sz++,len--) putchar(*sz);
}

int main(int argc, char *argv[])
{
	int Dtgt,Dsrc;
	FILE *Ftgt,*Fsrc;
	struct stat Stgt,Ssrc;
	size_t Gtgt,Gsrc;
	int tgt,src,dif;
	int info;
	char *Btgt,*Bsrc;
	if(argc < 3) {
		puts("usage>rcsmod <target scenario file> <source scenario file>");
		return 1;
	}
	
	Dtgt=open(argv[1],O_RDONLY);
	if(Dtgt == -1) {
		puts("rcsmod: Target scenario file not found.");
		return 2;
	}
	
	Dsrc=open(argv[2],O_RDONLY);
	if(Dsrc == -1) {
		puts("rcsmod: Source scenario file not found.");
		return 2;
	}
	
	Ftgt = fdopen(Dtgt,"rb");
	Fsrc = fdopen(Dsrc,"rb");
	
	fstat(Dtgt, &Stgt);
	fstat(Dsrc, &Ssrc);

	
	Btgt = (char *)malloc(Stgt.st_size);
	Bsrc = (char *)malloc(Ssrc.st_size);
	
	Gtgt = fread(Btgt,1,Stgt.st_size,Ftgt);
	Gsrc = fread(Bsrc,1,Ssrc.st_size,Fsrc);
	
	fclose(Ftgt);
	fclose(Fsrc);
	
	tgt = genhash(Btgt,Gtgt);
	src = genhash(Bsrc,Gsrc);
	
	{
		char sz[512];
		sprintf(sz, "-- Converted by RCSMOD\n-- target scenario code : %X\n-- base scenario code : %X\n\n",tgt,src);
		info = genhash(sz,strlen(sz));
		printf(sz);
	}
	
	dif = tgt - src - '-' * 2 - info;
	
	putall(Bsrc,Gsrc);
	putchar('-');
	putchar('-');
	
	{
		int i;
		int padA = dif > 0 ? *((unsigned char *)&inc) : *((unsigned char *)&dec);
		int padB = dif > 0 ? dif % (int)inc : dif % (int)dec;
		int padN = dif > 0 ? dif / (int)inc : dif / (int)dec;
		
		for(i = 0; i < padN; i++) putchar(padA);
		putchar(padB);
	}
	
	return 0;
}
