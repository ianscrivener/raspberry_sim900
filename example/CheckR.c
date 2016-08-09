#include "GSM.h"

int i; int call; extern gFD;
int *j[30]; int ppp; char *k; int* m; int a; int b; int ver;int reg;

void main(void)
{
	serialBegin(9600);
	TurnOn(9600);          		//module power on
	InitParam(PARAM_SET_1);		//configure the module  
	Echo(1);               		//enable AT echo 

    reg=CheckRegistration();
    
	switch (reg)
	{
      	case REG_NOT_REGISTERED:
        printf("not registered\n");
        break;
      	case REG_REGISTERED:
        printf("GSM module is registered\n");      
        break;
      	case REG_NO_RESPONSE:
        printf("GSM doesn't response\n");
        break;
      	case REG_COMM_LINE_BUSY:
        printf("comm line is not free\n");
        break;
    }
	
    delay(2000);

	reg=IsRegistered();
	printf("Registration:%d\n",reg);

    delay(5000);
}
