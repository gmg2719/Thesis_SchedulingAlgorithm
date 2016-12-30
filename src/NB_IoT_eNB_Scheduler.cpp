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
	int subframeTTI = atoi(argv[1]);
	if(define_Channel_Structure(&UL_Channle_Struc, &DL_Channle_Struc, subframeTTI)==0)
	{
		LOG("Set DL/UL Channel Structure Done\n");
		system("pause");
	}
	int number_of_target_SNR = atoi(argv[2]);
	// LOG("%d",number_of_target_SNR);
	if(Set_NPRACH_Resource(&NPRACH_Struc, subframeTTI, number_of_target_SNR)==-1)
	{
		LOG("No NPRACH Configuration: BS Failed!\n");
		return 0;
	}
	LOG("Set NPRACH Configuration done!\n");
	// return 0;
	// int num_UEs=10;
	int offsetPos=0;//Need Time Offset from SIB2 to start pos of NPRACH
	if(do_NPUSCH_Scheduler(offsetPos,subframeTTI,number_of_target_SNR,&NPRACH_Struc,&UL_Channle_Struc)==-1)
	{
		LOG("BS NPUSCH_Scheduler/Resource_Allocation Failed!\n");
		return 0;
	}
	for (int i = 0; i < subframeTTI; ++i)
	{
		for (int j = 0; j < num_UL_Subcarrier; ++j)
		{
			LOG("%d ",UL_Channle_Struc.resourceStruc[j][i]);
		}
		LOG("\n");
	}
	LOG("NPUSCH Scheduler/Resource Allocation done!\n");




	delete [] UL_Channle_Struc.resourceStruc;
	delete [] DL_Channle_Struc.resourceStruc;
	system("pause");
	return 0;
}





