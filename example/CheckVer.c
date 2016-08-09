#include "GSM.h"

int i; int call; extern gFD;
int *j[30]; int ppp; char *k; int* m; int a; int b; int ver;int reg;

void main(void)
{
	serialBegin(9600);
	TurnOn(9600);          		//module power on
	InitParam(PARAM_SET_1);		//configure the module  
	Echo(1);               		//enable AT echo
	
    ver=LibVer();
    printf("Response Ver %d\n",ver);

}