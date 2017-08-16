/*
*	22.09.2016
*	Консольный генератор паролей.
*	github.com/TyUser/hxPasswordGenerator
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void HXpassword(char *sPassword, unsigned int iQuality, unsigned int iType)
{
	FILE *fileTXT;
	fileTXT = fopen("password.txt", "a");
	if (fileTXT != NULL)
	{
		char sTime[32];
		time_t t = time(NULL);
		struct tm *aTm = localtime(&t);
		strftime(sTime, sizeof(sTime)-1, "%H:%M:%S %d_%m_%Y", aTm);

		if (iType == 3)
		{
			fprintf(fileTXT, "%s PIN code:		%s\n", sTime, sPassword);
		}
		else
		{
			fprintf(fileTXT, "%s Password %d:	%s\n", sTime, iQuality, sPassword);
		}
		fclose(fileTXT);
	}
}

void HXrand(const char *sBuf, unsigned int iQuality, unsigned int iType, unsigned int iNumber)
{
	char sBuf1[32] = {0};
	unsigned int iBuf1[12000] = {0};
	unsigned int iLen;
	unsigned int iRand1;
	unsigned int iRand2;
	unsigned int i;

	iLen = (unsigned int)strlen(sBuf);
	if (iLen > 1)
	{
		iRand1 = 0;
		iRand2 = 0;
		i = 0;

		iNumber += 1000;
		if (iNumber > 11000)
		{
			iNumber = 11000;
		}

		while (i < iNumber)
		{
			iBuf1[i] = (unsigned int)rand() % iLen;
			i += 1;
		}

		i = 0;
		while (i < iQuality)
		{
			iRand1 = (unsigned int)rand() % (iNumber - 1);
			iRand2 = iBuf1[iRand1];
			sBuf1[i] = sBuf[iRand2];
			i += 1;
		}
		sBuf1[i] = '\0';

		if (iType == 3)
		{
			printf("PIN code:	%s\n", sBuf1);
		}
		else
		{
			printf("Password:	%s\n", sBuf1);
		}
		HXpassword(sBuf1, iQuality, iType);
	}
}

void HXfileW(unsigned int iQuality, unsigned int iType, unsigned int iNumber)
{
	FILE *fileW;
	fileW = fopen("settings.ini", "w+");
	if (fileW)
	{
		fprintf(fileW, "[Options]\n");
		fprintf(fileW, "# v 2.0\n");
		fprintf(fileW, "# github.com/TyUser/hxPasswordGenerator\n\n");
		fprintf(fileW, "# качество пароля\n");
		fprintf(fileW, "# от 12 до 20\n");
		fprintf(fileW, "quality = %d\n\n", iQuality);
		fprintf(fileW, "# тип пароля\n");
		fprintf(fileW, "# от 1 до 4\n");
		fprintf(fileW, "type = %d\n\n", iType);
		fprintf(fileW, "# для индивидуального\n");
		fprintf(fileW, "# рандома\n");
		fprintf(fileW, "# от 1 до 10000\n");
		fprintf(fileW, "number = %d\n\n", iNumber);
		fclose(fileW);
	}
}

void HXtype(unsigned int iQuality, unsigned int iType, unsigned int iNumber)
{
	switch(iType)
	{
		case 1:
		{
			HXrand("1234567890AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz", iQuality, iType, iNumber);
			break;
		}
		case 2:
		{
			HXrand("123456789AaBbCcDdEeFfGgHhIiJjKkLlMmNnPpQqRrSsTtUuVvWwXxYyZz", iQuality, iType, iNumber);
			break;
		}
		case 3:
		{
			HXrand("1234567890", 3, iType, iNumber);
			break;
		}
		case 4:
		{
			HXrand("1234567890AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz!#$%&()*+,-:;<=>?_.", iQuality, iType, iNumber);
		}
	}
}

void HXfilter(char *sOld, char *sNew)
{
	unsigned int iBuf1 = 0;
	unsigned int iBuf2 = 0;

	while (sOld[iBuf1] != '\0')
	{
		if ((sOld[iBuf1] >= '0') && (sOld[iBuf1] <= '9'))
		{
			sNew[iBuf2] = sOld[iBuf1];
			iBuf2 += 1;
		}
		if ((sOld[iBuf1] >= 'a') && (sOld[iBuf1] <= 'z'))
		{
			sNew[iBuf2] = sOld[iBuf1];
			iBuf2 += 1;
		}
		if ((sOld[iBuf1] >= 'A') && (sOld[iBuf1] <= 'Z'))
		{
			sNew[iBuf2] = sOld[iBuf1];
			iBuf2 += 1;
		}
		if ((sOld[iBuf1] == '=') || (sOld[iBuf1] == '[') || (sOld[iBuf1] == ']'))
		{
			sNew[iBuf2] = sOld[iBuf1];
			iBuf2 += 1;
		}
		if ((sOld[iBuf1] == '#') || (sOld[iBuf1] == ';') || (sOld[iBuf1] == '/'))
		{
			break;
		}
		iBuf1 += 1;
	}
	sNew[iBuf2] = '\0';
}

void HXvalidate(unsigned int iQuality, unsigned int iType, unsigned int iNumber)
{
	unsigned int iRand;
	unsigned int iError;

	srand((unsigned int)time(NULL));
	iRand = 1 + (unsigned int)rand() % 10000;
	iError = 0;

	if (iQuality < 12)
	{
		iQuality = 16;
		iError = 1;
	}
	if (iQuality > 20)
	{
		iQuality = 20;
		iError = 1;
	}

	if (iType < 1)
	{
		iType = 2;
		iError = 1;
	}
	if (iType > 4)
	{
		iType = 4;
		iError = 1;
	}

	if (iNumber < 1)
	{
		iNumber = iRand;
		iError = 1;
	}
	if (iNumber > 10000)
	{
		iNumber = iRand;
		iError = 1;
	}

	if (iError)
	{
		HXfileW(iQuality, iType, iNumber);
	}

	HXtype(iQuality, iType, iNumber);
}

void HXfileR(void)
{
	FILE *fileR;
	char sBuf1[1024] = {0};
	char sBuf2[1024] = {0};
	char sKey1[16] = {0};
	char sKey2[8] = {0};

	unsigned int iQuality = 0;
	unsigned int iNumber = 0;
	unsigned int iType = 0;
	unsigned int i = 0;

	fileR = fopen("settings.ini", "r");
	if (fileR)
	{
		while (i <= 17)
		{
			sBuf1[0] = '\0';
			sBuf2[0] = '\0';
			sKey1[0] = '\0';
			sKey2[0] = '\0';

			if (!fgets(sBuf1, sizeof(sBuf1)-10, fileR))
			{
				break;
			}

			HXfilter(sBuf1, sBuf2);
			if (sscanf(sBuf2, "%8[^=]=%4[0-9]", sKey1, sKey2) == 2)
			{
				if (!strcmp(sKey1, "quality"))
				{
					iQuality = (unsigned int)atoi(sKey2);
				}
				if (!strcmp(sKey1, "type"))
				{
					iType = (unsigned int)atoi(sKey2);
				}
				if (!strcmp(sKey1, "number"))
				{
					iNumber = (unsigned int)atoi(sKey2);
				}
			}
			i += 1;
		}
		fclose(fileR);
	}

	HXvalidate(iQuality, iType, iNumber);
}

int main(int argc, char * argv[])
{
	unsigned int iQuality = 0;
	unsigned int iNumber = 0;
	unsigned int iType = 0;

	if (argc > 1)
	{
		if (argv[1] != NULL)
		{
			iQuality = (unsigned int)atoi(argv[1]);
		}
		if (argv[2] != NULL)
		{
			iType = (unsigned int)atoi(argv[2]);
		}
		if (argv[3] != NULL)
		{
			iNumber = (unsigned int)atoi(argv[3]);
		}

		HXvalidate(iQuality, iType, iNumber);
	}
	else
	{
		HXfileR();
	}
	return 0;
}
