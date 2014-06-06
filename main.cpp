#include <stdio.h>
#include "MRI_FIRST.h"

struct Student
{
	int age;
	char name[10];
	int sex;
};

struct Teacher
{
	int age;
	char name[10];
	int sex;
};

class Calculate
{

};

int main()
{
	ClientCalculate < Calculate > client;
	if ( !client.Connect("127.0.0.1", 4141) )
	{
		printf("Can not connect to server.\n");
		return 0;
	}
	else
	{
		printf("Connect to server successfully.\n");
	}

	int nResult = client.sum(2, 33);
	printf("The result of the int sum is %d", nResult);
	
	client.stopConnect();
	return 0;
}
