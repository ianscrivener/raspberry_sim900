#include "GSM.h"

char number[]="+39123456789";  	//Destination number
char text[]="hello world";  	//SMS to send
char sms_rx[122]; 				//Received text SMS
char number_incoming[20];
char inSerial[5];

int call;
int error;
int i=0;
int NU=0;

byte type_sms=SMS_UNREAD;      	//Type of SMS
byte del_sms=0;                	//0: No deleting sms - 1: Deleting SMS

void Check_Protocol();
void Check_Call();
void Check_SMS();

void main(void)
{
	serialBegin(9600);
	printf("system startup\n"); 
	TurnOn(9600);          		//module power on
	InitParam(PARAM_SET_1);		//configure the module  
	Echo(0);               		//enable AT echo 

	while(1)
	{
		delay(2000);
		Check_Call(); //Check if there is an incoming call
		Check_SMS();  //Check if there is SMS
		//Check data serial com

		if (serialDataAvail() > 0)
		{
			while (serialDataAvail() > 0)
			{
				inSerial[i]=(serialGetchar()); //read data
				NU = i;
				i++;
			}
			inSerial[i]='\0';
			Check_Protocol();
		}
	}
}

void Check_Protocol()
{
	printf("Command:");
	for(i=0;i<5;i++)
	{
		printf("%c",inSerial[i]);
	}
	printf("\n");

    	switch (inSerial[0])
	{
		case 'a' :  //Answer
			if (CallStatus()==CALL_INCOM_VOICE)
			{
				PickUp();
				printf("Answer\n");
			}
			else
			{
				printf("No incoming call\n");
			}
		break;


		case 'c': // C  //Call
			if (NU<2)  //To call variable 'number'    comand   c
			{
				printf("Calling %s\n",number);
				CallS(number);
			}

			if (NU==2)
			//To call number in phone book position comand cx where x is the SIM position
			{
				error=GetPhoneNumber(inSerial[1],number);
				if (error!=0)
				{
					printf("Calling %s\n",number);
					CallS(number);
				}
				else
				{
					printf("No number in pos %s\n",inSerial[1]);
				}
			}
		break;

		case 'h': //H //HangUp if there is an incoming call
			if (CallStatus()!=CALL_NONE)
			{
				printf("Hang\n");
				HangUp();
			}
			else
			{
				printf("No incoming call\n");
			}
		break;

		case 's': //S //Send SMS
			printf("Send SMS to %s\n",number);
			error=SendSMS(number,text);
			if (error==0)  //Check status
			{
				printf("SMS ERROR \n");
			}
			else
			{
				printf("SMS OK \n");
			}
		break;

		case 'p':  //Read-Write Phone Book
			if (NU==3)
			{
				switch (inSerial[1])
				{
				case 'd':  //Delete number in specified position  pd2
				   error=DelPhoneNumber(inSerial[2]);
				   if (error!=0)
				   {
						printf("Phone number position %s deleted\n",inSerial[2]);
				   }
				break;

				case 'g':  //Read from Phone Book position      pg2
				   error=GetPhoneNumber(inSerial[2],number);
				   if (error!=0)  //Find number in specified position
				   {
						printf("Phone Book position %s : %d\n",inSerial[2],number);
				   }
				   else  //Not find number in specified position
				   {
						printf("No Phone number in position %s\n",inSerial[2]);
				   }
				break;

				case 'w':  //Write from Phone Book Position    pw2
				   error=WritePhoneNumber(inSerial[2],number);
				   if (error!=0)
				   {
						printf("Number:%d writed in Phone Book position:%s\n",number,inSerial[2]);
				   }
				   else printf("Writing error\n");
				break;
				}
			}
		break;
	}
    delay(1500);
    return;
 }

void Check_Call()  //Check status call if this is available
{
	call=CallStatus();
	switch (call)
	{
		case CALL_NONE:
			printf("no call\n");
		break;

		case CALL_INCOM_VOICE:
			CallStatusWithAuth(number_incoming,0,0);
			printf("incoming voice call from %s\n",number_incoming);
		break;

		case CALL_ACTIVE_VOICE:
			printf("active voice call\n");
		break;

		case CALL_NO_RESPONSE:
			printf("no response\n");
		break;
	}
	return;
}

 void Check_SMS()  //Check if there is an sms 'type_sms'
 {
	char pos_sms_rx;  //Received SMS position     
	pos_sms_rx=IsSMSPresent(type_sms);
	if (pos_sms_rx!=0)
	{
		//Read text/number/position of sms
		GetSMS(pos_sms_rx,number_incoming,sms_rx,120);

		printf("Received SMS from:%s(sim position:%d),%d\n",number_incoming,(word)(pos_sms_rx),sms_rx);

		if (del_sms==1)  //If 'del_sms' is 1, i delete sms
		{
			error=DeleteSMS(pos_sms_rx);
			if (error==1)printf("SMS deleted\n");
			else printf("SMS not deleted\n");
		}
	}
	return;
 }
