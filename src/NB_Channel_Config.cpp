#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <iostream>
#include <algorithm>
#include "NB_Default_Channel_Config.h"

#define LOG(...) printf(__VA_ARGS__)

void UE_Init(UL_UE_list &UL_Sche_UE)//Call by reference is the same as Call by pointer
{
	//UL
	UL_Sche_UE.UE_id=-1;
	UL_Sche_UE.Priority=-1;
	UL_Sche_UE.CE_Level=-1;
	UL_Sche_UE.BSR=-1;
	UL_Sche_UE.DV=-1;
	UL_Sche_UE.PHR=-1;
	UL_Sche_UE.multi_tone=-1;
	UL_Sche_UE.num_subcarrier_one_RU=-1;
	UL_Sche_UE.num_UL_Slot=-1;
	UL_Sche_UE.freq_Space=0;
	UL_Sche_UE.mcs=-1;
	UL_Sche_UE.remaining_Buffer_Sizes=0;
	UL_Sche_UE.round=0;
	//DL
	// DL_Sche_UE.UE_id=-1;
	// DL_Sche_UE.Priority=-1;
	// DL_Sche_UE.CE_Level=-1;
	// DL_Sche_UE.Buffer_Size=-1;
}
int do_NPUSCH_Resource_Allocation(UL_UE_list **UL_Sche_UE_List)
{
	// int Init_UL_Remaining_PRB = *x;
	// int Add_UE_Num=6;
	// UL_Sche_UE_List->num_UEs=UL_Sche_UE_List->num_UEs+Add_UE_Num;
	// for (int i = 0; i < 10; ++i)
	// {
	// 	// UE_Init(&(UL_Sche_UE_List[i]),(NULL));
	// 	LOG("%d\n",UL_Sche_UE_List[i]->UE_id);
	// }



	return 0;

}
int get_UL_Data_Bytes(int DV)
{
	int UL_Data_Bytes=0;
	switch(DV)
	{
		case 0:
			UL_Data_Bytes=0;
			break;
		case 1:
			UL_Data_Bytes=rand()%10+1;//0 < DV <= 10
			break;
		case 2:
			UL_Data_Bytes=11+rand()%4;//10 < DV <= 14
			break;
		case 3:
			UL_Data_Bytes=15+rand()%5;//14 < DV <= 19
			break;
		case 4:
			UL_Data_Bytes=20+rand()%7;//19 < DV <= 26
			break;
		case 5:
			UL_Data_Bytes=27+rand()%10;//26 < DV <= 36
			break;
		case 6:
			UL_Data_Bytes=37+rand()%13;//36 < DV <= 49
			break;
		case 7:
			UL_Data_Bytes=50+rand()%18;//49 < DV <= 67
			break;
		case 8:
			UL_Data_Bytes=68+rand()%24;//67 < DV <= 91
			break;
		case 9:
			UL_Data_Bytes=92+rand()%34;//91 < DV <= 125
			break;
		case 10:
			UL_Data_Bytes=126+rand()%46;//125 < DV <= 171
			break;
		case 11:
			UL_Data_Bytes=172+rand()%63;//171 < DV <= 234
			break;
		case 12:
			UL_Data_Bytes=235+rand()%87;//234 < DV <= 321
			break;
		case 13:
			UL_Data_Bytes=322+rand()%447;//321 < DV <= 768
			break;
		case 14:
			UL_Data_Bytes=268+rand()%733;//768 < DV <= 1500-->fixed to no more 1000
			break;
		case 15:
			UL_Data_Bytes=1501;//DV > 1500
			break;
	}
	return UL_Data_Bytes;
}
int UL_TBS_Table[13][8]={{16,2,56,88,120,152,208,256},
{24,56,88,144,176,208,256,344},
{32,72,144,176,208,256,328,424},
{40,104,176,208,256,328,440,568},
{56,120,208,256,328,408,552,680},
{72,144,224,328,424,504,680,872},
{88,176,256,392,504,600,808,1000},
{104,224,328,472,584,712,1000,0},
{120,256,392,536,680,808,0,0},
{136,296,456,616,776,936,0,0},
{144,328,504,680,872,1000,0,0},
{176,376,584,776,1000,0,0,0},
{208,440,680,1000,0,0,0,0}};
bool compareMyType (const UL_UE_list &a, const UL_UE_list &b)
{
	return b.UL_Buffer_Sizes < a.UL_Buffer_Sizes;
	// if(a2->round!=b2->round)	return b2->round - a2->round;
	// else	return b2->DV - a2->DV;
}
int get_num_subcarrier(void)
{
	int x;
	x=rand()%4;
	if(x==0)	return 1;
	else if(x==1)	return 3;
	else if(x==2)	return 6;
	else if(x==3)	return 12;
}
int get_num_Slot(int x)
{
	if(x==1)	return 16;
	else if(x==3)	return 8;
	else if(x==6)	return 4;
	else if(x==12)	return 2;
	else	return -1;
}
int get_I_TBS(int x,int y)
{
	int I_TBS;
	if(y==0) I_TBS=x;
	else if(y==1)
	{
		if(x==1)	I_TBS=2;
		else if(x==2)	I_TBS=1;
		else
		{
			I_TBS=x;
		}
	}
	return I_TBS;
}
int get_I_RU(int x)
{
	int I_RU;
	if(x<=6)	I_RU=x-1;
	else if(x==8)	I_RU=x-2;
	else if(x==10)	I_RU=x-3;
	else return -1;
	return I_RU;
}
int get_TBS_UL(int mcs,int multi_tone,int num_RU)
{
	int TBS,check_I_RU;
	check_I_RU=get_I_RU(num_RU);
	if(check_I_RU==-1)	return -1;
	TBS=UL_TBS_Table[get_I_TBS(mcs,multi_tone)][check_I_RU];
	// LOG("%d\n",TBS);
	return TBS>>3;
}
int do_NPUSCH_Scheduler(int offsetPos,int subframeTTI,int number_of_target_SNR,NPRACH *NPRACH_Struc,UL_Anchor_Channel_Structure *UL_Channle_Struc)
{
	std::list<UL_UE_list> UL_Sche_UE_List;
	std::list<UL_UE_list>::iterator it1,it2;//it1:(UL_UE_list *)
	UL_UE_list UL_Sche_UE;
	int cntP[3]={0,0,0};
	int startScheTTICE0,startScheTTICE1,startScheTTICE2;
	int RBStartPos=NPRACH_Struc->first_end_time;
	int num_UEs_CE0=0,num_UEs_CE1=0,num_UEs_CE2=0;
	for(int subframe=offsetPos;subframe<subframeTTI;++subframe)
	{
		for (int i = 0; i < number_of_target_SNR; ++i)
		{
			if((subframe-offsetPos)%NPRACH_Struc->period[i]==0)
			{
				int startTime=subframe+NPRACH_Struc->start_time[i];
				int occupy_Subframe=NPRACH_Struc->rep[i] * 8;
				int occupy_Freq=NPRACH_Struc->num_Subcarrier[i];
				if(i==0)
				{
					++cntP[i];
					for (int j = startTime; j < (startTime+occupy_Subframe); ++j)
					{
						if(j >= subframeTTI)
						{
							// LOG("NPRACH CE0 Config exceed subframeTTI\n");
							// system("pause");
							break;
						}
						for (int z = NPRACH_Struc->subcarrier_Offset[i]; z < occupy_Freq; ++z)
							UL_Channle_Struc->resourceStruc[z][j]=1;// 1->occupied subcarrier
					}
					num_UEs_CE0=10;//rnad()? Preamble success
					startScheTTICE0=startTime+occupy_Subframe+NPRACH_Struc->I_Preamble_RAR+NPRACH_Struc->rep[i]+2 * NPRACH_Struc->rep[i]+NPRACH_Struc->I_RAR_Msg3+12+rand()%21+3+2 * NPRACH_Struc->rep[i]+NPRACH_Struc->I_Msg3_DCIN0;
					LOG("startScheTTI CE0: %d\n",startScheTTICE0);
				}
				else if(i==1)
				{
					for (int j = startTime; j < (startTime+occupy_Subframe); ++j)
					{
						if(j >= subframeTTI)
						{
							// LOG("NPRACH CE1 Config exceed subframeTTI\n");
							// system("pause");
							break;
						}
						for (int z = NPRACH_Struc->subcarrier_Offset[i]; z < NPRACH_Struc->subcarrier_Offset[i]+occupy_Freq; ++z)
							UL_Channle_Struc->resourceStruc[z][j]=2;// 1->occupied subcarrier
					}
					num_UEs_CE1=10;//rnad()?
					startScheTTICE1=startTime+occupy_Subframe+NPRACH_Struc->I_Preamble_RAR+NPRACH_Struc->rep[i]+2 * NPRACH_Struc->rep[i]+NPRACH_Struc->I_RAR_Msg3+24+rand()%41+3+2 * NPRACH_Struc->rep[i]+NPRACH_Struc->I_Msg3_DCIN0;
					LOG("startScheTTI CE1: %d\n",startScheTTICE1);
				}
				else if(i==2)
				{
					for (int j = startTime; j < (startTime+occupy_Subframe); ++j)
					{
						if(j >= subframeTTI)
						{
							// LOG("NPRACH CE2 Config exceed subframeTTI\n");
							// system("pause");
							break;
						}
						for (int z = NPRACH_Struc->subcarrier_Offset[i]; z < NPRACH_Struc->subcarrier_Offset[i]+occupy_Freq; ++z)
							UL_Channle_Struc->resourceStruc[z][j]=3;// 1->occupied subcarrier
					}
					num_UEs_CE2=10;//rnad()?
					startScheTTICE2=startTime+occupy_Subframe+NPRACH_Struc->I_Preamble_RAR+NPRACH_Struc->rep[i]+2 * NPRACH_Struc->rep[i]+NPRACH_Struc->I_RAR_Msg3+36+rand()%61+3+2 * NPRACH_Struc->rep[i]+NPRACH_Struc->I_Msg3_DCIN0;
					LOG("startScheTTI CE2: %d\n",startScheTTICE2);
				}
			}
		}
		if(subframe==startScheTTICE0)//sche for CE0 UEs
		{
			int UE_id=0;
			for (int i = 0; i < num_UEs_CE0; ++i)	UL_Sche_UE_List.push_back (UL_Sche_UE);
			for (it1=UL_Sche_UE_List.begin(); it1!=UL_Sche_UE_List.end(); ++it1)
			{
				UL_UE_list *i = &*it1;//need to convert from iterator to (UL_UE_list *)
				UE_Init(*i);
				i->UE_id=UE_id;
				i->UE_num_RU=0;
				++UE_id;
				i->CE_Level=rand() % 3; //decided by UE's RSRP
				i->DV=1+rand()%9;	//decided by UE's UL buffer
				i->multi_tone=rand() % 2; //decided by Msg3's content
				i->UL_Buffer_Sizes=get_UL_Data_Bytes(i->DV);
				//http://www.sharetechnote.com/html/Handbook_LTE_CodeRate.html#CodeRate_vs_MCS
				//MCS:2-10 <->Channnel quality/Coding rate
				i->mcs=2+rand()%9;//Qm=2,MCS:2-10 36213 Table 16.5.1.2-1
				std::cout << ' ' << i->UE_id<<','<<i->UL_Buffer_Sizes<<std::endl;
			}
			LOG("Initial/Setting CE0 UEs done!\n");
			UL_Sche_UE_List.sort (compareMyType);
			LOG("Sorting CE0 UEs done!\n");
			for (it1=UL_Sche_UE_List.begin(); it1!=UL_Sche_UE_List.end(); ++it1)
			{
				UL_UE_list *i = &*it1;
				LOG("UE_id:%d,CE_Level:%d,msc:%d,DV:%d,UL_Buffer_Sizes(Bytes):%d,multi_tone:%d\n",i->UE_id,i->CE_Level,i->mcs,i->DV,i->UL_Buffer_Sizes,i->multi_tone);
			}
			system("pause");
			// return 0;
			// for (int i = 0; i < 13; ++i)
			// {
			// 	for (int j = 0; j < 6; ++j)
			// 	{
			// 		LOG("%d",UL_TBS_Table[i][j]);
			// 	}
			// 	LOG("\n");
			// }
			// system("pause");
					//assign_RUs_required
			//To transmit N consecutive UL slot, N=num_Repetition * num_RU * num_UL_Slot
			//Assume num_subcarrier_one_RU transmit the same data size, only affect resource utilization
			// int num_Repetition[num_UEs_CE0];
			int I_TBS[num_UEs_CE0];
			int RU_unit=1;
			int Qm = 2;//modulation order fixed to 2
			//First Round
			for (it1=UL_Sche_UE_List.begin(); it1!=UL_Sche_UE_List.end(); ++it1)
			{
				UL_UE_list *i = &*it1;//need to convert from iterator to (UL_UE_list *)
				int num_RU=RU_unit;
				int TBS=get_TBS_UL(i->mcs,i->multi_tone,num_RU);
				// LOG("%d\n",TBS);
				if(TBS==-1) continue;
				while(TBS<i->UL_Buffer_Sizes)
				{
					num_RU=num_RU+RU_unit;
					if(num_RU>10)
					{
						TBS=get_TBS_UL(i->mcs,i->multi_tone,10);
						num_RU=10;
						i->remaining_Buffer_Sizes=i->UL_Buffer_Sizes-TBS;
						i->round++;
						break;
					}
					// TBS=get_TBS_UL(i->mcs,i->multi_tone,num_RU);
					// LOG("%d\n",TBS);
					// system("pause");
				}
				i->UE_num_RU=num_RU;
				LOG("UE_id:%d,num_RU:%d,UL_Buffer_Sizes:%d,TBS:%d,remaining_Buffer_Sizes:%d,round:%d\n",i->UE_id,i->UE_num_RU,i->UL_Buffer_Sizes,TBS,i->remaining_Buffer_Sizes,i->round);
			}
			system("pause");
			return 0;
		}
		else if(subframe==startScheTTICE1)
		{

		}
		else if(subframe==startScheTTICE2)
		{

		}

	}
	// return 0;
	return 0;
}

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
int Set_NPRACH_Resource(NPRACH *NPRACH_Struc, int subframeTTI, int number_of_target_SNR)
{
	NPRACH_Struc->I_Preamble_RAR=3;//3ms,could modify
	NPRACH_Struc->I_RAR_Msg3=8;//8ms,could modify
	NPRACH_Struc->I_Msg3_DCIN0=8;//8ms,could modify
	double OffsetSNR[3]={0,10,20};
	for (int i = 0; i < number_of_target_SNR; ++i)
	{
		NPRACH_Struc->target_SNR[i]=14.25-OffsetSNR[i];//14.25 dB, 4.25 dB, -5.75 dB
		// LOG("%d,%lf\n",number_of_target_SNR,NPRACH_Struc->target_SNR[i]);
		// system("pause");
		if(NPRACH_Struc->target_SNR[i]==14.25)//MCL:144 DB
		{
			NPRACH_Struc->CE_Level[i]=0;
			NPRACH_Struc->rep[i]=1; //8 ms
			NPRACH_Struc->period[i]=80;
			NPRACH_Struc->start_time[i]=8;//offset from start of NPRACH period
			NPRACH_Struc->num_Subcarrier[i]=12;
			NPRACH_Struc->subcarrier_Offset[i]=0;
			NPRACH_Struc->npdcch_NumRepetitions_RA[i]=1;
		}
		else if(NPRACH_Struc->target_SNR[i]==4.25)//MCL:154 DB
		{
			NPRACH_Struc->CE_Level[i]=1;
			NPRACH_Struc->rep[i]=2; //16 ms
			NPRACH_Struc->period[i]=80;
			NPRACH_Struc->start_time[i]=8;
			NPRACH_Struc->num_Subcarrier[i]=12;
			NPRACH_Struc->subcarrier_Offset[i]=12;
			NPRACH_Struc->npdcch_NumRepetitions_RA[i]=2;
		}
		else if(NPRACH_Struc->target_SNR[i]==-5.75)//MCL:164 DB
		{
			NPRACH_Struc->CE_Level[i]=2;
			NPRACH_Struc->rep[i]=4;	//32 ms
			NPRACH_Struc->period[i]=80;
			NPRACH_Struc->start_time[i]=8;
			NPRACH_Struc->num_Subcarrier[i]=24;
			NPRACH_Struc->subcarrier_Offset[i]=24;
			NPRACH_Struc->npdcch_NumRepetitions_RA[i]=4;
			NPRACH_Struc->first_end_time=NPRACH_Struc->start_time[i]+NPRACH_Struc->rep[i] * 8;//used for estimate start pos of UL data
		}
		else
			return -1;
	}
	return 0;
}





