#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>


#include "NB_Default_Channel_Config.h"


#define LOG(...) printf(__VA_ARGS__)


int main(int argc, char const *argv[])
{
	srand(time(NULL));
	UL_Anchor_Channel_Structure UL_Channle_Struc;
	DL_Anchor_Channel_Structure DL_Channle_Struc;
	NPRACH NPRACH_Struc;
	// UL_UE_list UL_Sche_UE_List;
	// DL_UE_list DL_Sche_UE_List;
	if(argc < 2 || *(argv+2)== NULL)
	{
		printf("No Argument Parameters\n");
		system("pause");
		return 0;
		// printf("%s\n", *argv);
		// printf("%s\n", *(argv+1));
    	// perror("perror");
    	// printf("strerror(errno)=%s\n", strerror(errno));
	}
	// UE_Init(&UL_Sche_UE_List,&DL_Sche_UE_List);
	// LOG("Initial UEs done!\n");
	int subframeTTI = atoi(argv[1]);//Convert string to integer,int atoi (const char * str);
	if(define_Channel_Structure(UL_Channle_Struc, DL_Channle_Struc, subframeTTI)==0)
	{
		LOG("Set DL/UL Channel Structure Done\n");
		system("pause");
	}
	int number_of_target_SNR = atoi(argv[2]);
	// LOG("%d",number_of_target_SNR);
	if(Set_NPRACH_Resource(NPRACH_Struc, subframeTTI, number_of_target_SNR)==-1)
	{
		LOG("No NPRACH Configuration: BS Failed!\n");
		return 0;
	}
	LOG("Setting NPRACH Configuration!\n");
	// return 0;
	// int num_UEs=10;
	int offsetPos=0;//Need Time Offset from SIB2 to start pos of NPRACH
	int exitReason_do_NPUSCH_Scheduler=do_NPUSCH_Scheduler(offsetPos,subframeTTI,number_of_target_SNR,NPRACH_Struc,UL_Channle_Struc);
	if(exitReason_do_NPUSCH_Scheduler==-1)
		LOG("BS NPUSCH_Scheduler/Resource_Allocation Failed!\n");
	else if(exitReason_do_NPUSCH_Scheduler==2)
		LOG("NPUSCH Scheduler/Resource Allocation/Mapping Not Fully done!\n");
	else if(exitReason_do_NPUSCH_Scheduler==0)
	{
		LOG("NPUSCH Scheduler/Resource Allocation/Mapping Fully done:\n");
		for (int i = 0; i < subframeTTI*2; ++i)
		{
			for (int j = 0; j < num_UL_Subcarrier; ++j)
			{
				LOG("%d ",UL_Channle_Struc.resourceStruc[j][i]);
			}
			LOG("\n");
		}
	}
	else
	{
		LOG("do_NPUSCH_Scheduler() return 1 for Testing!\n");
		for (int i = 0; i < subframeTTI*2; ++i)
		{
			for (int j = 0; j < num_UL_Subcarrier; ++j)
			{
				if(UL_Channle_Struc.resourceStruc[j][i]<10)	LOG("%d  ",UL_Channle_Struc.resourceStruc[j][i]);
				else LOG("%d ",UL_Channle_Struc.resourceStruc[j][i]);
			}
			LOG("\n");
		}
	}
	delete [] UL_Channle_Struc.resourceStruc;
	delete [] DL_Channle_Struc.resourceStruc;
	system("pause");
	return 0;
}





