#include <iostream>
#include <math.h>
#include "NB_Default_Channel_Config.h"

#define LOG(...) printf(__VA_ARGS__)
using namespace std;

int Auto_Filter_NB_SIB2Config(double **passSetting, double **indicateSIB)
{
	int rachperiod[8]={40,80,160,240,320,640,1280,2560};
	int rachstart[8]={8,16,32,64,128,256,512,1024};
	int rachrepeat[8]={1,2,4,8,16,32,64,128};
	int rawindow[8]={2,3,4,5,6,7,8,10};// Not include in MAC_Command in shell script
	int rmax[12]={1,2,4,8,16,32,64,128,256,512,1024,2048};
	double gvalue[8]={1.5,2,4,8,16,32,48,64};
	int candidate[4]={1,2,4,8};

	/*Not include shell script*/
	double pdcchoffset[4]={0,0.125,0.25,0.375};// Not include in MAC_Command in shell script

	/*Not include shell script*/
	int dlcqi[13]={0,1,2,3,4,5,6,7,8,9,10,11,12};
	int dlrepeat[16]={1,2,4,8,16,32,64,128,192,256,384,512,768,1024,1536,2048};
	int rachscofst[7]={0,12,24,36,2,18,34};
	int rachnumsc[4]={12,24,36,48};

	//limitCondition1
	int L_rachperiod=sizeof(rachperiod)/sizeof(int);
	int L_rawindow=sizeof(rawindow)/sizeof(int);
	int L_rmax=sizeof(rmax)/sizeof(int);
	double L_gvalue=sizeof(gvalue)/sizeof(double);
	int L_rachrepeat=sizeof(rachrepeat)/sizeof(int);

	//limitCondition2
	int L_candidate=sizeof(candidate)/sizeof(int);
	int L_rachstart=sizeof(rachstart)/sizeof(int);

	//limitCondition3
	int L_dlcqi=sizeof(dlcqi)/sizeof(int);
	int L_dlrepeat=sizeof(dlrepeat)/sizeof(int);

	double limitCondition1[12000][5];
	int cnt=0;
	for (int i = 0; i < L_rachperiod; ++i)
	{
		for (int j = 0; j < L_rachrepeat; ++j)
			for (int k = 0; k < L_rmax; ++k)
				for (int l = 0; l < L_gvalue; ++l)
					for (int m = 0; m < L_rawindow; ++m)
					{
						if(rachperiod[i]>1.4*4*rachrepeat[j]+3+rmax[k]*gvalue[l]*rawindow[m])
						{
							limitCondition1[cnt][0]=rachperiod[i];
							limitCondition1[cnt][1]=rachrepeat[j];
							limitCondition1[cnt][2]=rmax[k];
							limitCondition1[cnt][3]=gvalue[l];
							limitCondition1[cnt][4]=rawindow[m];
							// cout << limitCondition1[cnt][3] <<endl;
							// cout << limitCondition1[cnt][0] <<endl;
							++cnt;
						}
					}
	}
	// cout << "cnt: " << cnt << endl;

	// double **passSetting;
	// double **indicateSIB;
	// int Ans=number_of_target_SNR;
	// int size=400000;
	// passSetting = new double*[size];
	// indicateSIB = new double*[Ans];
	int setdone=0;
	// for (int i = 0; i < size; ++i)	passSetting[i] = new double [7];
	// for (int i = 0; i < number_of_target_SNR; ++i)	indicateSIB[i] = new double [7];
	int cnt1=0;
	for (int i = 0; i < cnt; ++i)
	{
		for (int j = 0; j < L_rachstart; ++j)
			for (int k = 0; k < L_candidate; ++k)
				if(limitCondition1[i][0]>rachstart[j]&&candidate[k]<=limitCondition1[i][2])
					{
						if(setdone==3)	return 1;
						passSetting[cnt1][0]=limitCondition1[i][0];
						passSetting[cnt1][1]=limitCondition1[i][1];
						passSetting[cnt1][2]=limitCondition1[i][2];
						passSetting[cnt1][3]=limitCondition1[i][3];
						passSetting[cnt1][4]=limitCondition1[i][4];
						passSetting[cnt1][5]=rachstart[j];
						passSetting[cnt1][6]=candidate[k];
						if(passSetting[cnt1][0]==80&&passSetting[cnt1][1]==1&&passSetting[cnt1][2]==2&&passSetting[cnt1][3]==2&&passSetting[cnt1][4]==2&&passSetting[cnt1][5]==8&&passSetting[cnt1][6]==2)
						{
							indicateSIB[0][0]=passSetting[cnt1][0];
							indicateSIB[0][1]=passSetting[cnt1][1];
							indicateSIB[0][2]=passSetting[cnt1][2];
							indicateSIB[0][3]=passSetting[cnt1][3];
							indicateSIB[0][4]=passSetting[cnt1][4];
							indicateSIB[0][5]=passSetting[cnt1][5];
							indicateSIB[0][6]=passSetting[cnt1][6];
							setdone++;
							// cout<<passSetting[cnt1][0]<<','<<passSetting[cnt1][1]<<','<<passSetting[cnt1][2]<<','<<passSetting[cnt1][3]<<','<<passSetting[cnt1][4]<<','<<passSetting[cnt1][5]<<','<<passSetting[cnt1][6]<<endl;
							// getchar();
						}
						else if(passSetting[cnt1][0]==80&&passSetting[cnt1][1]==2&&passSetting[cnt1][2]==4&&passSetting[cnt1][3]==2&&passSetting[cnt1][4]==2&&passSetting[cnt1][5]==8&&passSetting[cnt1][6]==4)
						{
							indicateSIB[1][0]=passSetting[cnt1][0];
							indicateSIB[1][1]=passSetting[cnt1][1];
							indicateSIB[1][2]=passSetting[cnt1][2];
							indicateSIB[1][3]=passSetting[cnt1][3];
							indicateSIB[1][4]=passSetting[cnt1][4];
							indicateSIB[1][5]=passSetting[cnt1][5];
							indicateSIB[1][6]=passSetting[cnt1][6];
							setdone++;
							// cout<<passSetting[cnt1][0]<<','<<passSetting[cnt1][1]<<','<<passSetting[cnt1][2]<<','<<passSetting[cnt1][3]<<','<<passSetting[cnt1][4]<<','<<passSetting[cnt1][5]<<','<<passSetting[cnt1][6]<<endl;
							// getchar();
						}
						else if(passSetting[cnt1][0]==80&&passSetting[cnt1][1]==4&&passSetting[cnt1][2]==8&&passSetting[cnt1][3]==2&&passSetting[cnt1][4]==2&&passSetting[cnt1][5]==8&&passSetting[cnt1][6]==8)
						{
							indicateSIB[2][0]=passSetting[cnt1][0];
							indicateSIB[2][1]=passSetting[cnt1][1];
							indicateSIB[2][2]=passSetting[cnt1][2];
							indicateSIB[2][3]=passSetting[cnt1][3];
							indicateSIB[2][4]=passSetting[cnt1][4];
							indicateSIB[2][5]=passSetting[cnt1][5];
							indicateSIB[2][6]=passSetting[cnt1][6];
							setdone++;
							// cout<<passSetting[cnt1][0]<<','<<passSetting[cnt1][1]<<','<<passSetting[cnt1][2]<<','<<passSetting[cnt1][3]<<','<<passSetting[cnt1][4]<<','<<passSetting[cnt1][5]<<','<<passSetting[cnt1][6]<<endl;
							// getchar();
						}
						// cout<<passSetting[cnt1][0]<<','<<passSetting[cnt1][1]<<','<<passSetting[cnt1][2]<<','<<passSetting[cnt1][3]<<','<<passSetting[cnt1][4]<<','<<passSetting[cnt1][5]<<','<<passSetting[cnt1][6]<<endl;
						// getchar();
						++cnt1;
					}
	}
	cout << "# of SIB2 Config Pass limitation : " << cnt1 << endl;
	return 0;
}

int define_Channel_Structure(UL_Anchor_Channel_Structure &UL_Channle_Struc, DL_Anchor_Channel_Structure &DL_Channle_Struc, int baseSubframeTTI)
{
	// baseSubframeTTI*=2;//Add More UL Channel to finish left UL scheduling
	LOG("Base Subframe TTI:%d\n",baseSubframeTTI);
	// UL_Channle_Struc->resourceStruc=NULL;
	UL_Channle_Struc.resourceStruc = new int*[num_UL_Subcarrier];
	DL_Channle_Struc.resourceStruc = new int*[num_DL_Subcarrier];
	for (int i = 0; i < num_UL_Subcarrier; ++i)	UL_Channle_Struc.resourceStruc[i] = new int [baseSubframeTTI];
	for (int i = 0; i < num_DL_Subcarrier; ++i)	DL_Channle_Struc.resourceStruc[i] = new int [baseSubframeTTI];

	for (int i = 0; i < num_UL_Subcarrier; ++i)
	{
		for (int j = 0; j < baseSubframeTTI; ++j)
		{
			UL_Channle_Struc.resourceStruc[i][j]=0;
			// LOG("%d ",UL_Channle_Struc.resourceStruc[i][j]);
		}
		// system("pause");
		// LOG("\n");
	}
	for (int i = 0; i < num_DL_Subcarrier; ++i)
	{
		for (int j = 0; j < baseSubframeTTI; ++j)
		{
			DL_Channle_Struc.resourceStruc[i][j]=0;
			// LOG("%d ",DL_Channle_Struc.resourceStruc[i][j]);
		}
		// system("pause");
		// LOG("\n");
	}
	return 0;
}
// indicateSIB[0][0]=rachperiod[i];
// indicateSIB[0][1]=rachrepeat[j];
// indicateSIB[0][2]=rmax[k];
// indicateSIB[0][3]=gvalue[l];
// indicateSIB[0][4]=rawindow[m];
// indicateSIB[0][5]=rachstart[j];
// indicateSIB[0][6]=candidate[k];
int Set_NPRACH_Resource(double **indicateSIB, NB_SIB2 &NB_SIB2_Struc, int number_of_target_SNR)
{
	NB_SIB2_Struc.I_Wait_to_NPDCCH=4;//at least 3ms,could modify
	NB_SIB2_Struc.I_NPDCCH_RAR=13;
	NB_SIB2_Struc.I_RAR_Msg3=13;//13ms,could modify
	NB_SIB2_Struc.I_Msg3_DCIN0=8;//8ms,could modify
	double OffsetSNR[3]={0,10,20};
	for (int i = 0; i < number_of_target_SNR; ++i)
	{
		NB_SIB2_Struc.target_SNR[i]=14.25-OffsetSNR[i];//14.25 dB, 4.25 dB, -5.75 dB
		// LOG("%d,%lf\n",number_of_target_SNR,NB_SIB2_Struc.target_SNR[i]);
		// system("pause");
		if(NB_SIB2_Struc.target_SNR[i]==14.25)//MCL:144 DB
		{
			NB_SIB2_Struc.CE_Level[i]=0;
			NB_SIB2_Struc.rep[i]=indicateSIB[0][1]; //1.4*4=5.6ms
			NB_SIB2_Struc.period[i]=indicateSIB[0][0];
			NB_SIB2_Struc.start_time[i]=indicateSIB[0][5];//offset from start of NPRACH period
			NB_SIB2_Struc.num_Subcarrier[i]=12;
			NB_SIB2_Struc.subcarrier_Offset[i]=0;
			NB_SIB2_Struc.npdcch_NumRepetitions_RA[i]=indicateSIB[0][2];
			NB_SIB2_Struc.npdcch_StartSF_CSS_RA[i]=indicateSIB[0][3];
			NB_SIB2_Struc.rawindow[i]=indicateSIB[0][4];
			NB_SIB2_Struc.candidate[i]=indicateSIB[0][6];
		}
		else if(NB_SIB2_Struc.target_SNR[i]==4.25)//MCL:154 DB
		{
			NB_SIB2_Struc.CE_Level[i]=1;
			NB_SIB2_Struc.rep[i]=indicateSIB[1][1]; //16 ms
			NB_SIB2_Struc.period[i]=indicateSIB[1][0];
			NB_SIB2_Struc.start_time[i]=indicateSIB[1][5];
			NB_SIB2_Struc.num_Subcarrier[i]=12;
			NB_SIB2_Struc.subcarrier_Offset[i]=12;
			NB_SIB2_Struc.npdcch_NumRepetitions_RA[i]=indicateSIB[1][2];
			NB_SIB2_Struc.npdcch_StartSF_CSS_RA[i]=indicateSIB[1][3];
			NB_SIB2_Struc.rawindow[i]=indicateSIB[1][4];
			NB_SIB2_Struc.candidate[i]=indicateSIB[1][6];
		}
		else if(NB_SIB2_Struc.target_SNR[i]==-5.75)//MCL:164 DB
		{
			NB_SIB2_Struc.CE_Level[i]=2;
			NB_SIB2_Struc.rep[i]=indicateSIB[2][1];	//32 ms
			NB_SIB2_Struc.period[i]=indicateSIB[2][0];
			NB_SIB2_Struc.start_time[i]=indicateSIB[2][5];
			NB_SIB2_Struc.num_Subcarrier[i]=24;
			NB_SIB2_Struc.subcarrier_Offset[i]=24;
			NB_SIB2_Struc.npdcch_NumRepetitions_RA[i]=indicateSIB[2][2];
			NB_SIB2_Struc.npdcch_StartSF_CSS_RA[i]=indicateSIB[2][3];
			NB_SIB2_Struc.rawindow[i]=indicateSIB[2][4];
			NB_SIB2_Struc.candidate[i]=indicateSIB[2][6];
		}
		else
			return -1;
	}
	checkFrequencyDomain(NB_SIB2_Struc);
	cout<<"NPRACH Config Pass FrequencyDomain limitation"<<endl;
	system("pause");
	return 0;
}

int nprachResourceMapping(int offsetPos,int baseSubframeTTI,int number_of_target_SNR,NB_SIB2 &NB_SIB2_Struc,UL_Anchor_Channel_Structure &UL_Channle_Struc)
{
	for(int subframe=offsetPos;subframe<baseSubframeTTI;++subframe)
	{
		for (int i = 0; i < number_of_target_SNR; ++i)
		{
			if((subframe-offsetPos)%NB_SIB2_Struc.period[i]==0)
			{
				int startTime=subframe+NB_SIB2_Struc.start_time[i];
				// cout<<NB_SIB2_Struc.rep[i] * 5.6<<endl;
				int occupy_Subframe=ceil(NB_SIB2_Struc.rep[i] * 5.6);
				// cout<<occupy_Subframe<<endl;
				// system("pause");
				int occupy_Freq=NB_SIB2_Struc.num_Subcarrier[i];
				if(i==0)
				{
					for (int j = startTime; j < (startTime+occupy_Subframe); ++j)
					{
						if(j >= baseSubframeTTI)
						{
							// LOG("NPRACH CE0 Config exceed baseSubframeTTI\n");
							// system("pause");
							break;
						}
						for (int z = NB_SIB2_Struc.subcarrier_Offset[i]; z < occupy_Freq; ++z)
							UL_Channle_Struc.resourceStruc[z][j]=1;// 1.occupied subcarrier
					}
				}
				else if(i==1)
				{
					for (int j = startTime; j < (startTime+occupy_Subframe); ++j)
					{
						if(j >= baseSubframeTTI)
						{
							// LOG("NPRACH CE1 Config exceed baseSubframeTTI\n");
							// system("pause");
							break;
						}
						for (int z = NB_SIB2_Struc.subcarrier_Offset[i]; z < NB_SIB2_Struc.subcarrier_Offset[i]+occupy_Freq; ++z)
							UL_Channle_Struc.resourceStruc[z][j]=2;// 1.occupied subcarrier
					}
				}
				else if(i==2)
				{
					for (int j = startTime; j < (startTime+occupy_Subframe); ++j)
					{
						if(j >= baseSubframeTTI)
						{
							// LOG("NPRACH CE2 Config exceed baseSubframeTTI\n");
							// system("pause");
							break;
						}
						for (int z = NB_SIB2_Struc.subcarrier_Offset[i]; z < NB_SIB2_Struc.subcarrier_Offset[i]+occupy_Freq; ++z)
							UL_Channle_Struc.resourceStruc[z][j]=3;// 1.occupied subcarrier
					}
				}
			}
		}
	}
	return 0;
}

int checkFrequencyDomain(NB_SIB2 &NB_SIB2_Struc)
{
	int checkFreqArray[48]={0};
	int errorReason=-1;
	for (int i = 0; i < 3; ++i)
	{
		if(i==0)
		{
			for (int j = NB_SIB2_Struc.subcarrier_Offset[i]; j < NB_SIB2_Struc.subcarrier_Offset[i]+NB_SIB2_Struc.num_Subcarrier[i]; ++j)
			{
				if(checkFreqArray[j]==0)	checkFreqArray[j]=1;
				else	errorReason=0;
			}
		}
		else if(i==1)
		{
			for (int j = NB_SIB2_Struc.subcarrier_Offset[i]; j < NB_SIB2_Struc.subcarrier_Offset[i]+NB_SIB2_Struc.num_Subcarrier[i]; ++j)
			{
				if(checkFreqArray[j]==0)	checkFreqArray[j]=2;
				else	errorReason=1;
			}
		}
		else if(i==2)
		{
			for (int j = NB_SIB2_Struc.subcarrier_Offset[i]; j < NB_SIB2_Struc.subcarrier_Offset[i]+NB_SIB2_Struc.num_Subcarrier[i]; ++j)
			{
				if(checkFreqArray[j]==0)	checkFreqArray[j]=3;
				else	errorReason=2;
			}
		}
		if(errorReason!=-1)
		{
			cout << "Error: the NPRACH-Config(CE"<<errorReason<<") is exceed FreqDomain resource!!" << endl;
			system("pause");
			exit(1);
		}
	}
	return 0;
}
