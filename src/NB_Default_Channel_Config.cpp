#include "NB_Default_Channel_Config.h"

#define LOG(...) printf(__VA_ARGS__)

int define_Channel_Structure(UL_Anchor_Channel_Structure *UL_Channle_Struc, DL_Anchor_Channel_Structure *DL_Channle_Struc, int subframeTTI)
{
	// UL_Channle_Struc->resourceStruc=NULL;
	UL_Channle_Struc->resourceStruc = new int*[num_UL_Subcarrier];
	DL_Channle_Struc->resourceStruc = new int*[num_DL_Subcarrier];
	for (int i = 0; i < num_UL_Subcarrier; ++i)	UL_Channle_Struc->resourceStruc[i] = new int [subframeTTI];
	for (int i = 0; i < num_DL_Subcarrier; ++i)	DL_Channle_Struc->resourceStruc[i] = new int [subframeTTI];

	for (int i = 0; i < num_UL_Subcarrier; ++i)
	{
		for (int j = 0; j < subframeTTI; ++j)
		{
			UL_Channle_Struc->resourceStruc[i][j]=0;
			// LOG("%d ",UL_Channle_Struc->resourceStruc[i][j]);
		}
		// system("pause");
		// LOG("\n");
	}
	for (int i = 0; i < num_DL_Subcarrier; ++i)
	{
		for (int j = 0; j < subframeTTI; ++j)
		{
			DL_Channle_Struc->resourceStruc[i][j]=0;
			// LOG("%d ",DL_Channle_Struc->resourceStruc[i][j]);
		}
		// system("pause");
		// LOG("\n");
	}
	return 0;
}

//UL Anchor Carrier Setting
int Set_NPRACH_Resource(UL_Anchor_Channel_Structure *UL_Channle_Struc, NPRACH *NPRACH_Struc, int subframeTTI, int number_of_target_SNR)
{
	double OffsetSNR[3]={0,10,20};
	for (int i = 0; i < number_of_target_SNR; ++i)
	{
		NPRACH_Struc->target_SNR[i]=14.25-OffsetSNR[i];//14.25 dB, 4.25 dB, -5.75 dB
		switch(NPRACH_Struc->target_SNR[i])
		{
			case 14.25:
				NPRACH_Struc->CE_Level[i]=0;
				NPRACH_Struc->rep[i]=1; //8 ms
				NPRACH_Struc->period[i]=160;
				NPRACH_Struc->start_time[i]=0;
				NPRACH_Struc->num_Subcarrier[i]=12;
				NPRACH_Struc->subcarrier_Offset[i]=0;
				break;
			case 25:
				NPRACH_Struc->CE_Level[i]=1;
				NPRACH_Struc->rep[i]=2; //16 ms
				NPRACH_Struc->period[i]=80;
				NPRACH_Struc->start_time[i]=0;
				NPRACH_Struc->num_Subcarrier[i]=12;
				NPRACH_Struc->subcarrier_Offset[i]=0;
				break;
			case -5.75:
				NPRACH_Struc->CE_Level[i]=2;
				NPRACH_Struc->rep[i]=4;	//32 ms
				NPRACH_Struc->period[i]=40;
				NPRACH_Struc->start_time[i]=0;
				NPRACH_Struc->num_Subcarrier[i]=12;
				NPRACH_Struc->subcarrier_Offset[i]=0;
			default:
				return -1;
		}
	}
	int occupy_Freq=NPRACH_Struc->num_Subcarrier[i];
	for(int subframe=0;subframe<subframeTTI;++subframe)
	{
		for (int i = 0; i < number_of_target_SNR; ++i)
		{
			if(subframe%NPRACH_Struc->period[i]==0)
			{
				NPRACH_Struc->start_time[i]=subframe;
				int occupy_Subframe=NPRACH_Struc->period[i] * 8;
				if(i==0)
				{
					for (int j = NPRACH_Struc->start_time[i]; j < (NPRACH_Struc->start_time[i]+occupy_Subframe); ++j)
					{
						for (int z = 0; z < occupy_Freq; ++z)
							UL_Channle_Struc->resourceStruc[z][j]=1;// 1->occupied subcarrier
					}
				}
				else if(i==1)
				{
					for (int j = NPRACH_Struc->start_time[i]; j < (NPRACH_Struc->start_time[i]+occupy_Subframe); ++j)
					{
						for (int z = 12; z < (12+occupy_Freq); ++z)
							UL_Channle_Struc->resourceStruc[z][j]=1;// 1->occupied subcarrier
					}
				}
				else if(i==2)
				{
					for (int j = NPRACH_Struc->start_time[i]; j < (NPRACH_Struc->start_time[i]+occupy_Subframe); ++j)
					{
						for (int z = 24; z < (24+occupy_Freq); ++z)
							UL_Channle_Struc->resourceStruc[z][j]=1;// 1->occupied subcarrier
					}
				}

			}
		}
	}
	return 0;
}
