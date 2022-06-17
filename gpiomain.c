#include "common.h"

//eric
#include <conio.h>

#include <stdio.h>


//enables bypass SUSI Init (1, 0)
int DRYRUN = 0;


#ifdef _RISC
#define SUSIDEMO_BANK_MAX 40
#else
#define SUSIDEMO_BANK_MAX 4
#endif


typedef struct
{
	uint8_t supPinNum;	/* 0 ~ 31 */
	uint32_t supInput;
	uint32_t supOutput;
} GPIOInfo, * PGPIOInfo;
static GPIOInfo info[SUSIDEMO_BANK_MAX];

typedef struct {
	uint32_t directionVal;
	uint32_t levelVal;
} GPIOConfig, * PGPIOConfig;

static char GPIOName[NAME_MAXIMUM_LENGTH];


enum PINConfig {
	PININPUT,
	PINOUTPUT,
	PINOFF
};

//DEFINE PINS - PIN MODES
#define P1 PININPUT
#define P2 PINOUTPUT
#define P3 PINOFF
#define P4 PINOFF
#define P5 PINOFF
#define P6 PINOFF 
#define P7 PINOFF
#define P8 PINOFF






uint8_t gpio_init1(void)
{
	uint32_t status, supportPin, id;
	uint8_t index, i, j;



	index = 0;
	for (i = 0; i < SUSIDEMO_BANK_MAX; i++)
	{
		id = SUSI_ID_GPIO_BANK(i);

		status = SusiGPIOGetCaps(id, SUSI_ID_GPIO_INPUT_SUPPORT, &info[i].supInput);
		if (status != SUSI_STATUS_SUCCESS)
			continue;

		status = SusiGPIOGetCaps(id, SUSI_ID_GPIO_OUTPUT_SUPPORT, &info[i].supOutput);
		if (status != SUSI_STATUS_SUCCESS)
			continue;

		supportPin = info[i].supInput | info[i].supOutput;

		if (supportPin > 0)
		{
			for (j = 32; (supportPin & (1 << (j - 1))) == 0; j--);
			info[i].supPinNum = j;
		}
		else
		{
			info[i].supPinNum = 0;
		}

		index++;
	}

	if (DRYRUN != 1)
	{

		if (index == 0)
			return SUSIDEMO_DEVICE_UNAVAILALBE;
	}

	return SUSIDEMO_DEVICE_AVAILALBE;
}

static uint8_t gpio_get_name(SusiId_t based_id, char* pName)
{
	uint32_t length = NAME_MAXIMUM_LENGTH, tmpLength = length;	/* include '\0': strlen + 1 */
	SusiId_t mapped_id = SUSI_ID_MAPPING_GET_NAME_GPIO(based_id);
	SusiStatus_t status = SusiBoardGetStringA(mapped_id, pName, &tmpLength);

	switch (status)
	{
	case SUSI_STATUS_MORE_DATA:
		pName[length - 2] = '*';
		pName[length - 1] = '\0';
		break;

	case SUSI_STATUS_ERROR:
		STRING_CP(pName, "Unknown_Name");
		break;

	case SUSI_STATUS_SUCCESS:
		break;

	default:
		STRING_CP(pName, "Unknown_Name");
		/*printf("hwm_get_name() failed. (0x%08X)\n", status);*/
		return 1;
	}

	return 0;
}

static void show_information(uint8_t iPin)
{
	uint8_t iBank;

	iBank = iPin >> 5;

	printf("support input : %d\n", (info[iBank].supInput >> (iPin & 0x1F)) & 0x1);
	printf("support output: %d\n", (info[iBank].supOutput >> (iPin & 0x1F)) & 0x1);

}



static uint8_t get_direction(uint8_t iPin, uint32_t* pvalue)
{
	uint32_t status, id, mask;


	id = SUSI_ID_GPIO(iPin);
	mask = 1;



	status = SusiGPIOGetDirection(id, mask, pvalue);
	if (status != SUSI_STATUS_SUCCESS)
	{
		printf("SusiGPIOGetDirection() failed. (0x%08X)\n", status);
		return 1;
	}

	return 0;
}

static uint8_t set_direction(uint8_t iPin)
{
	uint32_t status, id, mask, setVal;


	show_information(iPin);

	printf("\nSet Direction:\n\n");

	id = SUSI_ID_GPIO(iPin);
	mask = 1;

	do
	{
		printf("\nSet Value (0 or 1): ");
	} while (input_uint(&setVal, 10, 1, 0) != 0);



	status = SusiGPIOSetDirection(id, mask, setVal);

	if (status != SUSI_STATUS_SUCCESS)
	{
		printf("SusiGPIOSetDirection() failed. (0x%08X)\n", status);
		return 1;
	}

	printf("SusiGPIOSetDirection() succeed.\n");
	return 0;
}

static uint8_t get_level(uint8_t iPin, uint32_t* pvalue)
{
	uint32_t status, id, mask;



	id = SUSI_ID_GPIO(iPin);
	mask = 1;


	status = SusiGPIOGetLevel(id, mask, pvalue);
	if (status != SUSI_STATUS_SUCCESS)
	{
		printf("SusiGPIOGetLevel() failed. (0x%08X)\n", status);
		return 1;
	}

	return 0;
}

static uint8_t set_level(uint8_t iPin, uint32_t idTypeVal)
{
	uint32_t status, id, mask, setVal;


	show_information(iPin);

	printf("\nSet Level:\n\n");

	id = SUSI_ID_GPIO(iPin);
	mask = 1;

	do
	{
		printf("\nSet Value (0 or 1): ");
	} while (input_uint(&setVal, 10, 1, 0) != 0);

	status = SusiGPIOSetLevel(id, mask, setVal);
	if (status != SUSI_STATUS_SUCCESS)
	{
		printf("SusiGPIOSetLevel() failed. (0x%08X)\n", status);
		return 1;
	}

	printf("SusiGPIOSetLevel() succeed.\n");
	return 0;
}


void gpio_main1(void)
{
	//Input Level  = 1
	//Output Level = 0
	//Power on     = 1
	//Power off    = 0
	int32_t op;
	uint8_t iPin = 0;

	uint32_t iTemp = 0;

	int32_t P1_level_old = 0;
	int32_t P1_level_now = 0;
	int32_t P6_level_old = 0;
	int32_t P6_level_now = 0;

	//PIN INITILIZATION
	if (P1 == PININPUT) {
		SusiGPIOSetDirection(0, 0, 1);
		SusiGPIOSetLevel(0, 0, 0);
	}
	if (P1 == PINOUTPUT || P1 == PINOFF) {
		SusiGPIOSetDirection(0, 0, 0);
		SusiGPIOSetLevel(0, 0, 0);
	}

	if (P2 == PININPUT) {
		SusiGPIOSetDirection(1, 0, 1);
		SusiGPIOSetLevel(1, 0, 0);
	}
	if (P2 == PINOUTPUT || P2 == PINOFF) {
		SusiGPIOSetDirection(1, 0, 0);
		SusiGPIOSetLevel(1, 0, 0);
	}

	if (P3 == PININPUT) {
		SusiGPIOSetDirection(2, 0, 1);
		SusiGPIOSetLevel(2, 0, 0);
	}
	if (P3 == PINOUTPUT || P3 == PINOFF) {
		SusiGPIOSetDirection(2, 0, 0);
		SusiGPIOSetLevel(2, 0, 0);
	}

	if (P4 == PININPUT) {
		SusiGPIOSetDirection(3, 0, 1);
		SusiGPIOSetLevel(3, 0, 0);
	}
	if (P4 == PINOUTPUT || P4 == PINOFF) {
		SusiGPIOSetDirection(3, 0, 0);
		SusiGPIOSetLevel(3, 0, 0);
	}

	if (P5 == PININPUT) {
		SusiGPIOSetDirection(4, 0, 1);
		SusiGPIOSetLevel(4, 0, 0);
	}
	if (P5 == PINOUTPUT || P5 == PINOFF) {
		SusiGPIOSetDirection(4, 0, 0);
		SusiGPIOSetLevel(4, 0, 0);
	}

	if (P6 == PININPUT) {
		SusiGPIOSetDirection(5, 0, 1);
		SusiGPIOSetLevel(5, 0, 0);
	}
	if (P6 == PINOUTPUT || P6 == PINOFF) {
		SusiGPIOSetDirection(5, 0, 0);
		SusiGPIOSetLevel(5, 0, 0);
	}

	if (P7 == PININPUT) {
		SusiGPIOSetDirection(6, 0, 1);
		SusiGPIOSetLevel(6, 0, 0);
	}
	if (P7 == PINOUTPUT || P7 == PINOFF) {
		SusiGPIOSetDirection(6, 0, 0);
		SusiGPIOSetLevel(6, 0, 0);
	}

	if (P8 == PININPUT) {
		SusiGPIOSetDirection(7, 0, 1);
		SusiGPIOSetLevel(7, 0, 0);
	}
	if (P8 == PINOUTPUT || P8 == PINOFF) {
		SusiGPIOSetDirection(7, 0, 0);
		SusiGPIOSetLevel(7, 0, 0);
	}
	printf("PINS HAVE BEEN INITIALIZED");



	printf("\n===============================\n");
	printf("\nGPIO MAIN\n");
	printf("\n===============================\n");



	gpio_get_name((SusiId_t)iPin, GPIOName);



	while (1)
	{
		//printf("\nPress any key to continue.  0 to exit ");

		Sleep(1000);

		// get P1 level 
		get_level(0, &P1_level_now);

		if (DRYRUN) {
			if (iTemp % 5 == 0)
			{
				if (P1_level_now == 0)
					P1_level_now = 1;
				else
					P1_level_now = 0;
			}
			if (iTemp % 5 == 0)
			{
				if (P6_level_now == 0)
					P6_level_now = 1;
				else
					P6_level_now = 0;
			}
		}


		//printf("P1_level_now =%d\n", P1_level_now);
		//printf("P1_level_old =%d\n", P1_level_old);

		// PIN1 DISPLAY
		if (P1_level_old - P1_level_now == 0)
		{
			//printf("\nPin 1 not changed\n");
		}
		else if(P1_level_old - P1_level_now > 0)
		{
			printf("\nPin 1 input changed from High to Low\n");
		}
		else if (P1_level_old - P1_level_now < 0)
		{
			printf("\nPin 1 input changed from Low to High\n");
			// send High to Pin 2

		}
		P1_level_old = P1_level_now;


		// PIN6 DISPLAY
		if (P6 == PININPUT) {
			get_level(5, &P6_level_now);
			if (P6_level_old - P6_level_now == 0)
			{
				//printf("\nPin 6 not changed\n");
			}
			else if (P6_level_old - P6_level_now > 0)
			{
				printf("\nPin 6 input changed from High to Low\n");
			}
			else if (P6_level_old - P6_level_now < 0)
			{
				printf("\nPin 6 input changed from Low to High\n");
				// send High to Pin 2

			}
			P6_level_old = P6_level_now;

		}

		//DETECT KEYPRESS TO SET PIN OUTPUT LEVEL
		//1 for HIGH
		//0 FOR LOW
		if (kbhit())
		{
			char ch = getch();
			printf("USER INPUT:  %c \n ", ch);
			if ((char)(ch) == '1') {
				printf("Pin is set to low \n");
				SusiGPIOSetLevel(1, 0, 0);
			}
			else if ((char)(ch) == '2') {
				printf("Pin is set to high \n");
				SusiGPIOSetLevel(1, 0, 1);
			}
			else {
				printf("Key is invalid: Enter 1 to set pin to low, 2 to set to high \n");
			}
		}
		
		iTemp++;

	}



}



int main(void)
{

	SusiStatus_t status;

	int ret = 0;

	printf("program started\n");

	status = SusiLibInitialize();

	if (DRYRUN == 1)
		status = SUSI_STATUS_INITIALIZED;

	if (status == SUSI_STATUS_ERROR)
	{
		// This error only occurs on Linux distributions
		printf("Your Linux capabilities is not enough, recommond to become ROOT!\n");
		printf("Aborting demo now.\n");
		return -1;
	}

	if (status != SUSI_STATUS_SUCCESS && status != SUSI_STATUS_INITIALIZED)
	{
		printf("SusiLibInitialize() failed. (0x%08X)\n", status);
		printf("Exit the program...\n");
		return -1;
	}

	gpio_init1();


	clr_screen();


	gpio_main1();



	printf("Exit the program...\n");

	status = SusiLibUninitialize();


	if (status != SUSI_STATUS_SUCCESS)
	{
		printf("SusiLibUninitialize() failed. (%8X)\n", status);
		return -1;
	}

	return 0;
}