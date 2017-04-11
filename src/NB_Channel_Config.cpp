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
	UL_Sche_UE.multi_tone_Msg3=-1;
	UL_Sche_UE.num_subcarrier_perRU=-1;
	UL_Sche_UE.num_UL_Slot=-1;
	UL_Sche_UE.freq_Space=0;
	UL_Sche_UE.mcs=-1;
	UL_Sche_UE.remaining_Buffer_Sizes=0;
	UL_Sche_UE.round=0;
	UL_Sche_UE.startScheMsg3=-1;
	UL_Sche_UE.startScheULdata=-1;
	//DL
	// DL_Sche_UE.UE_id=-1;
	// DL_Sche_UE.Priority=-1;
	// DL_Sche_UE.CE_Level=-1;
	// DL_Sche_UE.Buffer_Size=-1;
}
int fifteenkHz_RU_Format[4][2]={{1,16},{3,8},{6,4},{12,2}};//[i][j]: i: number of subcarrier used in one RU, number of UL slots use in one RU

// int get_resource_allocation(int remaining_UL_resource)
// {
// 	if(remaining_UL_resource>=32)	return	32;//subcarrier44-47,subcarrier36-43,subcarrier24-35,sub0-23
// 	else if(remaining_UL_resource>=8)	return	8;//subcarrier 36-47,subcarrier24-35,sub0-23
// 	else if(remaining_UL_resource>=4)	return	4;//subcarrier 24-35
// 	else if(remaining_UL_resource>=2)	return	2;//subcarrier 0-23
// 	else if(remaining_UL_resource>=1)	return	1;//subcarrier 0-47
// 	else return -1;
// }
int get_remaining_UL_resource(int startScheTTICE0,int &scheTTIOffset,UL_Anchor_Channel_Structure &UL_Channle_Struc)
{
	int remaining_UL_resource=-1;
	//,resource_allocation=0;
	// int scheTTIOffset=0;
	for (int i = startScheTTICE0; i < (startScheTTICE0+48); ++i)
	{
		if(UL_Channle_Struc.resourceStruc[47][i]==3)//check CE2-NPRACH Period to get avaliable resource
		{
			remaining_UL_resource=i-startScheTTICE0;
			scheTTIOffset=0;
		}
		while(remaining_UL_resource==0)
		{
			++i;
			if(UL_Channle_Struc.resourceStruc[47][i]==0)
			{
				remaining_UL_resource=(80-32);//48
				scheTTIOffset=i-startScheTTICE0;
			}
		}
		if(remaining_UL_resource!=-1)	break;
	}
	return	remaining_UL_resource;
}
int resourceAllocPattern0[5]={0,24,36,40,44};//freqOffset for resource mapping, 6 tone/3 tone/1*3 tone
int resourceAllocPattern1[5]={0,4,8,12,24};//freqOffset for resource mapping, 1*3 tone/3 tone/6 tone
int resourceAllocPattern2[8]={0,24,36,40,44,45,46,47};//freqOffset for resource mapping
// int resourceAllocPattern2[5]={0,24,36,40,44};//freqOffset for resource mapping
bool check_ULChannel(UL_Anchor_Channel_Structure &UL_Channle_Struc,int Subframe,int startFreqPos,int occupy_Freq)
{
	for (int i = startFreqPos; i < startFreqPos+occupy_Freq; ++i)
		if(UL_Channle_Struc.resourceStruc[i][Subframe]!=0)	return false;
	return true;
}
int get_num_subcarrier_perRU(int get_startFreqPos)
{
	if(get_startFreqPos==resourceAllocPattern1[4])	return 6;
	else if(get_startFreqPos==resourceAllocPattern1[3])	return 3;
	else if(get_startFreqPos==resourceAllocPattern1[0]||get_startFreqPos==resourceAllocPattern1[1]||get_startFreqPos==resourceAllocPattern1[2])	return 1;
}
int get_num_Slot(int x)
{
	if(x==1)	return 16;
	else if(x==3)	return 8;
	else if(x==6)	return 4;
	else if(x==12)	return 2;
	else	return -1;
}
int get_startFreqPos(UL_Anchor_Channel_Structure &UL_Channle_Struc,int &startTime,int subframeTTI,int multiToneSupport)
{
	int FreqPos=-1;
	int offsetTTI=0;
	if(multiToneSupport!=0)
	{
		for (int i = startTime; i < subframeTTI; ++i)
		{
			for (int j = 12; j < resourceAllocPattern1[4]+24; ++j)//12<=j<48, don't use singleTone resource
			{
				if(UL_Channle_Struc.resourceStruc[j][i]==0)
				{
					FreqPos=j;
					break;
				}
			}
			if(FreqPos!=-1)
			{
				offsetTTI=i-startTime;
				break;
			}
		}
		startTime=startTime+offsetTTI;
		// LOG("startTime%d,FreqPos:%d\n",startTime,FreqPos);
	}
	else if(multiToneSupport==0)
	{
		for (int i = startTime; i < subframeTTI; ++i)
		{
			for (int j = resourceAllocPattern1[0]; j < resourceAllocPattern1[3]; ++j)//0<=j<12, don't use MultiTone resource
				if(UL_Channle_Struc.resourceStruc[j][i]==0)
				{
					FreqPos=j;
					break;
				}
			if(FreqPos!=-1)
			{
				offsetTTI=i-startTime;
				break;
			}
		}
		startTime=startTime+offsetTTI;
		// LOG("startTime%d,FreqPos:%d\n",startTime,FreqPos);
	}
	return FreqPos;
}
bool lock=false;
// int OffsetStartScheTTICE0[10]={0,0,0,0,0,0,0,0,0,0};
int do_NPUSCH_Resource_Allocation(UL_UE_list &UL_Sche_UE,UL_Anchor_Channel_Structure &UL_Channle_Struc,int startScheTTICE0,int &subframeTTI)
{
	int scheTTIRAOffset=0,remaining_UL_RAresource=-1;
	static int cntScheTTICE0=0;
	static int OffsetStartScheTTICE0[3]={startScheTTICE0,startScheTTICE0,startScheTTICE0};
	if(OffsetStartScheTTICE0[cntScheTTICE0]!=startScheTTICE0)
	{
		lock=false;
		++cntScheTTICE0;
		OffsetStartScheTTICE0[cntScheTTICE0]=startScheTTICE0;
	}
	if(!lock)
	{
		for (int i = startScheTTICE0; i < subframeTTI; ++i)
		{
			if(i >= subframeTTI)	return 2;
			for (int j = 0; j < 48; ++j)
			{
				if(UL_Channle_Struc.resourceStruc[j][i]==1||UL_Channle_Struc.resourceStruc[j][i]==2||UL_Channle_Struc.resourceStruc[j][i]==3)//check CE2-NPRACH Period to get avaliable resource
				{
					remaining_UL_RAresource=i-startScheTTICE0;
					scheTTIRAOffset=0;
					break;
				}
			}
			while(remaining_UL_RAresource==0)
			{
				++i;
				int RAcnt=0;
				for (int j = 0; j < 48; ++j)
				{
					if(UL_Channle_Struc.resourceStruc[j][i]!=1||UL_Channle_Struc.resourceStruc[j][i]!=2||UL_Channle_Struc.resourceStruc[j][i]!=3)
					{
						++RAcnt;
					}
				}
				if(RAcnt==48)
				{
					++remaining_UL_RAresource;
					scheTTIRAOffset=i-startScheTTICE0;
				}
				else if(RAcnt!=48&&remaining_UL_RAresource==0)	continue;
				else break;//RAcnt!=48&&remaining_UL_RAresource!=0
			}
			if(remaining_UL_RAresource!=-1)	break;
		}
		startScheTTICE0=startScheTTICE0+scheTTIRAOffset;//Update startScheTTICE0 avoid RA resource
		lock=true;
		// LOG("startScheTTICE0:%d,remaining_UL_resource:%d\n",startScheTTICE0,remaining_UL_RAresource);
	}

	// return 1;//for testing remaining_UL_RAresource
	if(UL_Sche_UE.CE_Level==0)
	{
		int numRepetiiton=1;
		// UL_Sche_UE.num_subcarrier_perRU=num_subcarrier_perRU;
		static int startTimeSixTone=startScheTTICE0;
		static int startTimeThreeTone=startScheTTICE0;
		static int startTimeSingleTone[3]={startScheTTICE0,startScheTTICE0,startScheTTICE0};

		if(UL_Sche_UE.multi_tone!=0)//15 KHz Multi-Tone:0,1,2
		{
			if(startTimeSixTone>startTimeThreeTone)	startScheTTICE0=startTimeThreeTone;
			else	startScheTTICE0=startTimeSixTone;
			int startTime=0;
			// LOG("startScheTTICE0:%d\n",startScheTTICE0);
			int startFreqPos=get_startFreqPos(UL_Channle_Struc,startScheTTICE0,subframeTTI,UL_Sche_UE.multi_tone);
			int num_subcarrier_perRU=get_num_subcarrier_perRU(startFreqPos);
			int occupy_Freq=num_subcarrier_perRU * 4;//(3.75 * 4) ->15 kHz

			if(num_subcarrier_perRU==6)
			{
				// startTimeSixTone=startScheTTICE0;//record MultiTone used time
				startTime=startTimeSixTone;
			}
			else if(num_subcarrier_perRU==3)
			{
				// startTimeThreeTone=startScheTTICE0;//record MultiTone used time
				startTime=startTimeThreeTone;
			}
			// LOG("startScheTTICE0:%d,startTimeSixTone:%d,startTimeThreeTone:%d\n",startScheTTICE0,startTimeSixTone,startTimeThreeTone);

			UL_Sche_UE.remainging_subframe=numRepetiiton * UL_Sche_UE.UE_num_RU * get_num_Slot(num_subcarrier_perRU) * 0.5;//Rep*Num of RU*ULslot(0.5 ms per slot)//ULslot:8,4
			// int remainging_subframe;
			// LOG("UE_id:%d,startTime:%d,#subcarrier:%d,remainging_subframe:%d,\n",UL_Sche_UE.UE_id,startTime,num_subcarrier_perRU,UL_Sche_UE.remainging_subframe);
			// LOG("StartMappingTime%d,FreqPos:%d,#subcarrierPerRU:%d\n",startTime,startFreqPos,num_subcarrier_perRU);
			while(UL_Sche_UE.remainging_subframe!=0)
			{
				for (int i = startTime; i < subframeTTI; ++i)
				{
					// LOG("subframe:%d\n",i);
					if(check_ULChannel(UL_Channle_Struc,i,startFreqPos,occupy_Freq))
					{
						for (int j = startFreqPos; j < startFreqPos+occupy_Freq; ++j)
						{
							UL_Channle_Struc.resourceStruc[j][i]=UL_Sche_UE.UE_id;// Do PHY Resource Mapping subframe-base
						}
						// for (int j = 0; j < 48; ++j)
						// {
						// 	// LOG("%d ",UL_Channle_Struc.resourceStruc[j][i]);
						// }
						// LOG("\n");
						--UL_Sche_UE.remainging_subframe;
						// LOG("\nremainging_subframe:%d\n",UL_Sche_UE.remainging_subframe);
					}
					if(UL_Sche_UE.remainging_subframe==0)
					{
						if(num_subcarrier_perRU==6)	startTimeSixTone=i+1;
						else if(num_subcarrier_perRU==3)	startTimeThreeTone=i+1;
						break;
						// LOG("startTimeSixTone:%d,startTimeThreeTone:%d\n",startTimeSixTone,startTimeThreeTone);
					}
				}
				if(UL_Sche_UE.remainging_subframe!=0)	return 2;
			}
		}
		else//15 KHz single tone:1, not-implenmentation:3.75 KHz single tone
		{
			// int num_subcarrier_perRU=get_num_subcarrier_perRU(startFreqPos);
			int startFreqPos=get_startFreqPos(UL_Channle_Struc,startScheTTICE0,subframeTTI,UL_Sche_UE.multi_tone);
			// LOG("UpdateScheTTI%d,FreqPos:%d,#subcarrierPerRU:1\n",startScheTTICE0,startFreqPos);
			int startTime=0;
			int num_subcarrier_perRU=1;
			int occupy_Freq=num_subcarrier_perRU * 4;//(3.75 * 4) ->15 kHz

			if(startFreqPos==resourceAllocPattern1[0])//resourceAllocPattern1[0]:0
			{
				// startTimeSingleTone[0]=startScheTTICE0;//record SingleTone used time
				startTime=startTimeSingleTone[0];
			}
			else if(startFreqPos==resourceAllocPattern1[1])//resourceAllocPattern1[1]:4
			{
				// startTimeSingleTone[1]=startScheTTICE0;//record SingleTone used time
				startTime=startTimeSingleTone[1];
			}
			else if(startFreqPos==resourceAllocPattern1[2])//resourceAllocPattern1[2]:8
			{
				// startTimeSingleTone[2]=startScheTTICE0;//record SingleTone used time
				startTime=startTimeSingleTone[2];
			}

			UL_Sche_UE.remainging_subframe=numRepetiiton * UL_Sche_UE.UE_num_RU * get_num_Slot(num_subcarrier_perRU) * 0.5;//Rep*Num of RU*ULslot(0.5 ms per slot)//ULslot:8,4
			// int remainging_subframe;
			// LOG("UE_id:%d,startTime:%d,#subcarrier:%d,\n",UL_Sche_UE.UE_id,startTime,num_subcarrier_perRU);
			// LOG("UE_id:%d,startTime:%d,#subcarrier:%d,remainging_subframe:%d,\n",UL_Sche_UE.UE_id,startTime,num_subcarrier_perRU,UL_Sche_UE.remainging_subframe);
			// LOG("StartMappingTime%d,FreqPos:%d,#subcarrierPerRU:%d\n",startTime,startFreqPos,num_subcarrier_perRU);
			while(UL_Sche_UE.remainging_subframe!=0)
			{
				for (int i = startTime; i < subframeTTI; ++i)
				{
					// LOG("subframe:%d\n",i);
					if(check_ULChannel(UL_Channle_Struc,i,startFreqPos,occupy_Freq))
					{
						for (int j = startFreqPos; j < startFreqPos+occupy_Freq; ++j)
						{
							UL_Channle_Struc.resourceStruc[j][i]=UL_Sche_UE.UE_id;// Do PHY Resource Mapping subframe-base
						}
						--UL_Sche_UE.remainging_subframe;
					}
					if(UL_Sche_UE.remainging_subframe==0)
					{
						if(startFreqPos==resourceAllocPattern0[2])
						{
							startTimeSingleTone[0]=i+1;//record SingleTone used time
						}
						else if(startFreqPos==resourceAllocPattern0[3])//resourceAllocPattern0[3]:40
						{
							startTimeSingleTone[1]=i+1;//record SingleTone used time
						}
						else if(startFreqPos==resourceAllocPattern0[4])//resourceAllocPattern0[4]:44
						{
							startTimeSingleTone[2]=i+1;//record SingleTone used time
						}
						break;
					}
				}
				if(UL_Sche_UE.remainging_subframe!=0)	return 2;
			}
		}
	}
	else if(UL_Sche_UE.CE_Level==1)
	{
		// int numRepetiiton=2;
	}
	else if(UL_Sche_UE.CE_Level==2)
	{
		// int numRepetiiton=4;
	}
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
		default:
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
	return a.UL_Buffer_Sizes > b.UL_Buffer_Sizes ;
	// if(a2->round!=b2->round)	return b2->round - a2->round;
	// else	return b2->DV - a2->DV;
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
int nprachResourceMapping(int offsetPos,int subframeTTI,int number_of_target_SNR,NPRACH &NPRACH_Struc,UL_Anchor_Channel_Structure &UL_Channle_Struc)
{
	subframeTTI*=2;
	for(int subframe=offsetPos;subframe<subframeTTI;++subframe)
	{
		for (int i = 0; i < number_of_target_SNR; ++i)
		{
			if((subframe-offsetPos)%NPRACH_Struc.period[i]==0)
			{
				int startTime=subframe+NPRACH_Struc.start_time[i];
				int occupy_Subframe=NPRACH_Struc.rep[i] * 8;
				int occupy_Freq=NPRACH_Struc.num_Subcarrier[i];
				if(i==0)
				{
					for (int j = startTime; j < (startTime+occupy_Subframe); ++j)
					{
						if(j >= subframeTTI)
						{
							// LOG("NPRACH CE0 Config exceed subframeTTI\n");
							// system("pause");
							break;
						}
						for (int z = NPRACH_Struc.subcarrier_Offset[i]; z < occupy_Freq; ++z)
							UL_Channle_Struc.resourceStruc[z][j]=1;// 1.occupied subcarrier
					}
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
						for (int z = NPRACH_Struc.subcarrier_Offset[i]; z < NPRACH_Struc.subcarrier_Offset[i]+occupy_Freq; ++z)
							UL_Channle_Struc.resourceStruc[z][j]=2;// 1.occupied subcarrier
					}
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
						for (int z = NPRACH_Struc.subcarrier_Offset[i]; z < NPRACH_Struc.subcarrier_Offset[i]+occupy_Freq; ++z)
							UL_Channle_Struc.resourceStruc[z][j]=3;// 1.occupied subcarrier
					}
				}
			}
		}
	}
	return 0;
}
int do_NPUSCH_Scheduler(int offsetPos,int &subframeTTI,int number_of_target_SNR,NPRACH &NPRACH_Struc,UL_Anchor_Channel_Structure &UL_Channle_Struc)
{
	std::list<UL_UE_list> UL_Sche_UE_List;
	std::list<UL_UE_list>::iterator it1,it2;//it1:(UL_UE_list *)
	UL_UE_list UL_Sche_UE;
	int startScheTTICE0,startScheTTICE1,startScheTTICE2;
	// int RBStartPos=NPRACH_Struc.first_end_time;// not used for now
	int num_UEs_CE0=0,num_UEs_CE1=0,num_UEs_CE2=0,UE_id=10;
	nprachResourceMapping(offsetPos,subframeTTI,number_of_target_SNR,NPRACH_Struc,UL_Channle_Struc);
	// for (int i = 0; i < subframeTTI; ++i)
	// {
	// 	for (int j = 0; j < num_UL_Subcarrier; ++j)
	// 		LOG("%d ",UL_Channle_Struc.resourceStruc[j][i]);
	// 	LOG("\n");
	// }
	LOG("NPRACH Resource Mapping done!\n");
	// return 0;
	for(int subframe=offsetPos;subframe<subframeTTI;++subframe)
	{
		for (int i = 0; i < number_of_target_SNR; ++i)
		{
			if((subframe-offsetPos)%NPRACH_Struc.period[i]==0)
			{
				int startTime=subframe+NPRACH_Struc.start_time[i];
				int occupy_Subframe=NPRACH_Struc.rep[i] * 8;
				int occupy_Freq=NPRACH_Struc.num_Subcarrier[i];
				if(i==0)
				{
					// int successPreambleCE0=3;//rand()? Preamble success
					// for (int i = 0; i < successPreambleCE0; ++i)	UL_Sche_UE_List.push_back (UL_Sche_UE);
					// int Pos=UL_Sche_UE_List.size()-1;
					for (it1=UL_Sche_UE_List.begin(); it1!=UL_Sche_UE_List.end(); ++it1)
					{
						UL_UE_list *i = &*it1;//need to convert from iterator to (UL_UE_list *)
						UE_Init(*i);
						i->UE_id=UE_id;
						i->UE_num_RU=0;
						i->remainging_subframe=0;
						++UE_id;
						// i->freq_Space=15;//15 kHz
						i->CE_Level=0; //decided by UE's RSRP in RA procedure
						// i->DV=1+rand()%8;	//decided by UE's UL buffer
						i->multi_tone_Msg3=rand() % 3; //decided by Msg3's content
						// i->UL_Buffer_Sizes=get_UL_Data_Bytes(i->DV);
						//http://www.sharetechnote.com/html/Handbook_LTE_CodeRate.html#CodeRate_vs_MCS
						//MCS:2-10 <->Channnel quality/Coding rate
						i->mcs=2+rand()%9;//Qm=2,MCS:2-10 36213 Table 16.5.1.2-1
						// int repDCIRAR=NPRACH_Struc.rep[i]* (rand()%successPreambleCE0);
						// int I_RAR=NPRACH_Struc.rep[i]=* (rand()%successPreambleCE0);
						// i->startScheMsg3=startTime+occupy_Subframe+NPRACH_Struc.I_Preamble_RAR+repDCIRAR+I_RAR;
						// i->startScheULdata=-1;
						// i->resourceAlloc=false;
						// std::cout << ' ' << i->UE_id<<','<<i->UL_Buffer_Sizes<<std::endl;
					}

					startScheTTICE0=startTime+occupy_Subframe+NPRACH_Struc.I_Preamble_RAR+NPRACH_Struc.I_RAR_Msg3+12+rand()%21+3+2 * NPRACH_Struc.rep[i]+NPRACH_Struc.I_Msg3_DCIN0;
					LOG("startScheTTI CE0: %d\n",startScheTTICE0);
					// Msg3deltaMappingTime=time_SuccessPreamble+4ms+DCI_Rep(RAR)+5ms+ ko_RAR+RAR_Rep+ko_Msg3

				}
				else if(i==1)
				{
					int successPreambleCE1=3;//rand()? Preamble success
					num_UEs_CE1=10;//rand()?
					startScheTTICE1=startTime+occupy_Subframe+NPRACH_Struc.I_Preamble_RAR+NPRACH_Struc.rep[i]+2 * NPRACH_Struc.rep[i]+NPRACH_Struc.I_RAR_Msg3+24+rand()%41+3+2 * NPRACH_Struc.rep[i]+NPRACH_Struc.I_Msg3_DCIN0;
					LOG("startScheTTI CE1: %d\n",startScheTTICE1);
				}
				else if(i==2)
				{
					int successPreambleCE3=3;//rand()? Preamble success
					num_UEs_CE2=10;//rand()?
					startScheTTICE2=startTime+occupy_Subframe+NPRACH_Struc.I_Preamble_RAR+NPRACH_Struc.rep[i]+2 * NPRACH_Struc.rep[i]+NPRACH_Struc.I_RAR_Msg3+36+rand()%61+3+2 * NPRACH_Struc.rep[i]+NPRACH_Struc.I_Msg3_DCIN0;
					LOG("startScheTTI CE2: %d\n",startScheTTICE2);
				}
			}
		}//end... number_of_target_SNR
		// for (it1=UL_Sche_UE_List.begin(); it1!=UL_Sche_UE_List.end(); ++it1)
		// {
		// 	UL_UE_list *i = &*it1;
		// 	if(subframe==i->startScheMsg3)
		// 	{
		// 		int Msg3bits=91;
		// 		int RU_unit=1;
		// 		i->DV=1+rand()%8;	//decided by UE's UL buffer
		// 		i->UL_Buffer_Sizes=get_UL_Data_Bytes(Msg3bits);
		// 		i->multi_tone=rand() % 3; //decided by Msg3's content
		// 		int num_RU=RU_unit;
		// 		int TBS=get_TBS_UL(i->mcs,i->multi_tone,num_RU);
		// 		// LOG("InitialTBS:%d\n",TBS);
		// 		if(TBS==-1) continue;
		// 		while(TBS<i->UL_Buffer_Sizes)
		// 		{
		// 			num_RU=num_RU+RU_unit;
		// 			if(num_RU>10)
		// 			{
		// 				TBS=get_TBS_UL(i->mcs,i->multi_tone,10);
		// 				num_RU=10;
		// 				i->remaining_Buffer_Sizes=i->UL_Buffer_Sizes-TBS;
		// 				i->round++;
		// 				break;
		// 			}
		// 			TBS=get_TBS_UL(i->mcs,i->multi_tone,num_RU);
		// 			// LOG("TBS:%d\n",TBS);
		// 			// system("pause");
		// 		}
		// 		i->UE_num_RU=num_RU;
		// 		// while (!i->empty())
		// 		// {

		// 		// }
		// 	}
		// }
		if(subframe==startScheTTICE0)//sche for CE0 UEs
		{
			num_UEs_CE0=10;
			static int cntRnd=0;
			++cntRnd;
			LOG("%d round startScheTTICE0:%d\n",cntRnd,startScheTTICE0);
			// static int UE_id=10;//10~
			// int successProb=0.36;
			int UE_id=10;
			for (int i = 0; i < num_UEs_CE0; ++i)	UL_Sche_UE_List.push_back (UL_Sche_UE);
			for (it1=UL_Sche_UE_List.begin(); it1!=UL_Sche_UE_List.end(); ++it1)
			{
				UL_UE_list *i = &*it1;//need to convert from iterator to (UL_UE_list *)
				UE_Init(*i);
				i->UE_id=UE_id;
				i->UE_num_RU=0;
				i->remainging_subframe=0;
				++UE_id;
				// i->freq_Space=15;//15 kHz
				i->CE_Level=0; //decided by UE's RSRP in RA procedure
				i->DV=1+rand()%8;	//decided by UE's UL buffer
				i->multi_tone=rand() % 3; //decided by Msg3's content
				i->UL_Buffer_Sizes=get_UL_Data_Bytes(i->DV);
				//http://www.sharetechnote.com/html/Handbook_LTE_CodeRate.html#CodeRate_vs_MCS
				//MCS:2-10 <->Channnel quality/Coding rate
				i->mcs=2+rand()%9;//Qm=2,MCS:2-10 36213 Table 16.5.1.2-1
				i->resourceAlloc=false;
				// std::cout << ' ' << i->UE_id<<','<<i->UL_Buffer_Sizes<<std::endl;
			}
			// UE_id=UE_id+10;
			LOG("Initial/Setting CE0 UEs done!\n");
			UL_Sche_UE_List.sort (compareMyType);
			LOG("Sorting CE0 UEs done!\n");
			// for (it1=UL_Sche_UE_List.begin(); it1!=UL_Sche_UE_List.end(); ++it1)
			// {
			// 	UL_UE_list *i = &*it1;
			// 	LOG("UE_id:%d,CE_Level:%d,msc:%d,DV:%d,UL_Buffer_Sizes(Bytes):%d,multi_tone:%d\n",i->UE_id,i->CE_Level,i->mcs,i->DV,i->UL_Buffer_Sizes,i->multi_tone);
			// }
			// system("pause");
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
			// assign_RUs_required
			//To transmit N consecutive UL slot, N=num_Repetition * num_RU * num_UL_Slot
			//Assume num_subcarrier_one_RU transmit the same data size, only affect resource utilization
			//int num_Repetition[num_UEs_CE0];
			// int I_TBS[num_UEs_CE0];
			int RU_unit=1;
			int Qm = 2;//modulation order fixed to 2
			//First Round to calculate RU required by each UEs
			for (it1=UL_Sche_UE_List.begin(); it1!=UL_Sche_UE_List.end(); ++it1)
			{
				UL_UE_list *i = &*it1;//need to convert from iterator to (UL_UE_list *)
				int num_RU=RU_unit;
				int TBS=get_TBS_UL(i->mcs,i->multi_tone,num_RU);
				// LOG("InitialTBS:%d\n",TBS);
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
					TBS=get_TBS_UL(i->mcs,i->multi_tone,num_RU);
					// LOG("TBS:%d\n",TBS);
					// system("pause");
				}
				i->UE_num_RU=num_RU;
				LOG("UE_id:%d,num_RU:%d,UL_Buffer_Sizes:%d,TBS:%d,multi_tone:%d,round:%d\n",i->UE_id,i->UE_num_RU,i->UL_Buffer_Sizes,TBS,i->multi_tone,i->round);
			}
			// int checkMapping=0;//if(UL_Channle_Struc.resourceStruc[47][startTime]==3)	return -1;
			// int scheTTIOffset=0;
			// int remaining_UL_resource=get_remaining_UL_resource(startScheTTICE0,scheTTIOffset,UL_Channle_Struc);//:First remaing resource need fixed->consider UE has mapped done UL data
			// startScheTTICE0=startScheTTICE0+scheTTIOffset;//Update startScheTTICE0:First ScheTTI
			int checkMapping=0;
			while (!UL_Sche_UE_List.empty())
			{
				it1=UL_Sche_UE_List.begin();
				UL_UE_list *i = &*it1;//need to convert from iterator to (UL_UE_list *)
				checkMapping=do_NPUSCH_Resource_Allocation(*i,UL_Channle_Struc,startScheTTICE0,subframeTTI);//handle from 1st UE to 10 UE...
				if(checkMapping==-1)
				{
					LOG("UE:%d,ResourceMapping has error\n",i->UE_id);
					return -1;
				}
				else if(checkMapping==2)
				{
					LOG("UE:%d,Schduler Range Exceed subframeTTI:%d\n",i->UE_id,subframeTTI);
					return 2;
				}
				else if(checkMapping==1)	return 1;

				if(i->remainging_subframe==0)//check if UE has resource mapping done
				{
					// std::cout << "Erase UE:" << i->UE_id;
	    			it1 = UL_Sche_UE_List.erase (it1);
	    			// system("pause");
				}
				else
				{
					LOG("UE:%d.remainging_subframe!=0\n",i->UE_id);
					system("pause");
				}
			}
			LOG("1st round CE0 UEs ResourceMapping has done\n");
			return 1;
			// for (it1=UL_Sche_UE_List.begin(); it1!=UL_Sche_UE_List.end(); ++it1)
			// {
			// 	UL_UE_list *i = &*it1;//need to convert from iterator to (UL_UE_list *)
			// 	checkMapping=do_NPUSCH_Resource_Allocation(*i,UL_Channle_Struc,startScheTTICE0,remaining_UL_resource);
			// 	if(checkMapping==-1)	LOG("ResourceMapping has error\n");
			// 	system("pause");
			// }
			// for (int i = 0; i < subframeTTI; ++i)
			// {
			// 	for (int j = 0; j < num_UL_Subcarrier; ++j)
			// 	{
			// 		LOG("%d ",UL_Channle_Struc.resourceStruc[j][i]);
			// 	}
			// 	LOG("\n");
			// }
			// LOG("NPUSCH Scheduler/Resource Allocation done!\n");
			// system("pause");
			// return 0;
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

int define_Channel_Structure(UL_Anchor_Channel_Structure &UL_Channle_Struc, DL_Anchor_Channel_Structure &DL_Channle_Struc, int subframeTTI)
{
	subframeTTI*=2;//Add More UL Channel to finish left UL scheduling
	LOG("%d",subframeTTI);
	// UL_Channle_Struc->resourceStruc=NULL;
	UL_Channle_Struc.resourceStruc = new int*[num_UL_Subcarrier];
	DL_Channle_Struc.resourceStruc = new int*[num_DL_Subcarrier];
	for (int i = 0; i < num_UL_Subcarrier; ++i)	UL_Channle_Struc.resourceStruc[i] = new int [subframeTTI];
	for (int i = 0; i < num_DL_Subcarrier; ++i)	DL_Channle_Struc.resourceStruc[i] = new int [subframeTTI];

	for (int i = 0; i < num_UL_Subcarrier; ++i)
	{
		for (int j = 0; j < subframeTTI; ++j)
		{
			UL_Channle_Struc.resourceStruc[i][j]=0;
			// LOG("%d ",UL_Channle_Struc.resourceStruc[i][j]);
		}
		// system("pause");
		// LOG("\n");
	}
	for (int i = 0; i < num_DL_Subcarrier; ++i)
	{
		for (int j = 0; j < subframeTTI; ++j)
		{
			DL_Channle_Struc.resourceStruc[i][j]=0;
			// LOG("%d ",DL_Channle_Struc.resourceStruc[i][j]);
		}
		// system("pause");
		// LOG("\n");
	}
	return 0;
}
//UL Anchor Carrier Setting
int Set_NPRACH_Resource(NPRACH &NPRACH_Struc, int subframeTTI, int number_of_target_SNR)
{
	NPRACH_Struc.I_Preamble_RAR=4;//3ms,could modify
	NPRACH_Struc.I_RAR_Msg3=13;//13ms,could modify
	NPRACH_Struc.I_Msg3_DCIN0=8;//8ms,could modify
	double OffsetSNR[3]={0,10,20};
	for (int i = 0; i < number_of_target_SNR; ++i)
	{
		NPRACH_Struc.target_SNR[i]=14.25-OffsetSNR[i];//14.25 dB, 4.25 dB, -5.75 dB
		// LOG("%d,%lf\n",number_of_target_SNR,NPRACH_Struc.target_SNR[i]);
		// system("pause");
		if(NPRACH_Struc.target_SNR[i]==14.25)//MCL:144 DB
		{
			NPRACH_Struc.CE_Level[i]=0;
			NPRACH_Struc.rep[i]=1; //8 ms
			NPRACH_Struc.period[i]=80;
			NPRACH_Struc.start_time[i]=8;//offset from start of NPRACH period
			NPRACH_Struc.num_Subcarrier[i]=12;
			NPRACH_Struc.subcarrier_Offset[i]=0;
			NPRACH_Struc.npdcch_NumRepetitions_RA[i]=1;
		}
		else if(NPRACH_Struc.target_SNR[i]==4.25)//MCL:154 DB
		{
			NPRACH_Struc.CE_Level[i]=1;
			NPRACH_Struc.rep[i]=2; //16 ms
			NPRACH_Struc.period[i]=80;
			NPRACH_Struc.start_time[i]=8;
			NPRACH_Struc.num_Subcarrier[i]=12;
			NPRACH_Struc.subcarrier_Offset[i]=12;
			NPRACH_Struc.npdcch_NumRepetitions_RA[i]=2;
		}
		else if(NPRACH_Struc.target_SNR[i]==-5.75)//MCL:164 DB
		{
			NPRACH_Struc.CE_Level[i]=2;
			NPRACH_Struc.rep[i]=4;	//32 ms
			NPRACH_Struc.period[i]=80;
			NPRACH_Struc.start_time[i]=8;
			NPRACH_Struc.num_Subcarrier[i]=24;
			NPRACH_Struc.subcarrier_Offset[i]=24;
			NPRACH_Struc.npdcch_NumRepetitions_RA[i]=4;
			NPRACH_Struc.first_end_time=NPRACH_Struc.start_time[i]+NPRACH_Struc.rep[i] * 8;//used for estimate start pos of UL data
		}
		else
			return -1;
	}
	return 0;
}





