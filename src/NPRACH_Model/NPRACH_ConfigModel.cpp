#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <fstream>

#include "file/Csv.hpp"
#include "file/Csv.cpp"

using namespace std;

#define LOG(...) printf(__VA_ARGS__)

// #define NUMBER_OF_UE_MAX 10
#define num_UL_Subcarrier 48
#define num_DL_Subcarrier 12



typedef struct {
	int **resourceStruc;
} UL_Anchor_Channel_Structure;
typedef struct {
	int **resourceStruc;
} DL_Anchor_Channel_Structure;

typedef struct {
	double target_SNR[3];
	int CE_Level[3];
	int rep[3];
	int period[3];//nprach-Periodicity
	int start_time[3];
	int first_end_time;//used for estimate start pos of UL data
	int num_Subcarrier[3];
	int subcarrier_Offset[3];
	double subcarrierMSG3_RangeStart[3];
	//DCI N1 for RAR and Msg4, DCI N0 for Msg3 retransmission
	int npdcch_NumRepetitions_RA[3];//Maximum number of repetitions for NPDCCH common search space (CSS) for RAR, Msg3 retransmission and Msg4
	int npdcch_Offset_RA[3];//Fractional period offset of starting subframe for NPDCCH common search space (CSS Type 2)
	int npdcch_StartSF_CSS_RA[3];//Starting subframe configuration for NPDCCH common search space (CSS), including RAR,
} NPRACH;

int define_Channel_Structure(UL_Anchor_Channel_Structure &, DL_Anchor_Channel_Structure &, int);
int Set_NPRACH_Resource(NPRACH &, int, int);
int nprachResourceMapping(int,int,int,NPRACH &,UL_Anchor_Channel_Structure &);


int main()
{
	// ofstream NPRACH_Configuration;
	// NPRACH_Configuration.open("NPRACH_Configuration_Result.csv", ios::out);

	// Csv csv("NPRACH_ConfigV1.csv");

	// for (size_t i = 0; i < csv.getLines(); ++i)
	// {
	// 	Csv::Line line = csv.getLine(i);

	// 	for (size_t j = 0; j < line.size(); ++j)
	// 		if (line[j].length() > 0)
	// 			std::cout << line[j] << ", ";
	// 			// std::cout << line[j];
	// 	std::cout << "\n";
	// }

	// return 0;

	UL_Anchor_Channel_Structure UL_Channle_Struc;
	DL_Anchor_Channel_Structure DL_Channle_Struc;
	NPRACH NPRACH_Struc;

	int subframeTTI=100;
	if(define_Channel_Structure(UL_Channle_Struc, DL_Channle_Struc, subframeTTI)==0)
	{
		LOG("Set UL Channel Structure Done\n");
		system("pause");
	}
	int number_of_target_SNR=3;
	// LOG("%d",number_of_target_SNR);
	if(Set_NPRACH_Resource(NPRACH_Struc, subframeTTI, number_of_target_SNR)==-1)
	{
		LOG("No NPRACH Configuration: BS Failed!\n");
		system("pause");
		return 0;
	}
	LOG("Setting NPRACH Configuration Done!\n");
	system("pause");
	int offsetPos=0;//Need Time Offset from SIB2 to start pos of NPRACH
	nprachResourceMapping(offsetPos,subframeTTI,number_of_target_SNR,NPRACH_Struc,UL_Channle_Struc);
	for (int i = 0; i < subframeTTI*2; ++i)
	{
		for (int j = 0; j < num_UL_Subcarrier; ++j)
		{
			LOG("%d ",UL_Channle_Struc.resourceStruc[j][i]);
			// NPRACH_Configuration << UL_Channle_Struc.resourceStruc[j][i] << ",";
		}
		// NPRACH_Configuration<<endl;
		LOG("\n");
	}

	// NPRACH_Configuration.close();

	delete [] UL_Channle_Struc.resourceStruc;
	delete [] DL_Channle_Struc.resourceStruc;
	system("pause");
	return 0;
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
int define_Channel_Structure(UL_Anchor_Channel_Structure &UL_Channle_Struc, DL_Anchor_Channel_Structure &DL_Channle_Struc, int subframeTTI)
{
	subframeTTI*=2;//Add More UL Channel to finish left UL scheduling
	// LOG("Total subframeTTI:%d",subframeTTI);
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

int Set_NPRACH_Resource(NPRACH &NPRACH_Struc, int subframeTTI, int number_of_target_SNR)
{
	// open the input file
    fstream fin;
    //fin.open("12.in", fstream::in);
    fin.open("NPRACH_Configuration.txt", fstream::in);
    if(!fin.is_open())
    {
        cout << "Error: the input file is not opened!!" << endl;
        system("pause");
        exit(1);
    }
    char raConfig[1000];
    fin.getline(raConfig,sizeof(raConfig),'\n');
    // cout << raConfig << endl;
    int CE[3][6]={{0,0,0},{0,0,0},{0,0,0}};

    for (int i = 0; i < 5; ++i)
    {
    	fin.getline(raConfig,sizeof(raConfig),',');
    	CE[0][i]=stoi(raConfig);
    }
    fin.getline(raConfig,sizeof(raConfig),'\n');
    CE[0][5]=stoi(raConfig);

    for (int i = 0; i < 5; ++i)
    {
    	fin.getline(raConfig,sizeof(raConfig),',');
    	CE[1][i]=stoi(raConfig);
    }
    fin.getline(raConfig,sizeof(raConfig),'\n');
    CE[1][5]=stoi(raConfig);

    for (int i = 0; i < 5; ++i)
    {
    	fin.getline(raConfig,sizeof(raConfig),',');
    	CE[2][i]=stoi(raConfig);
    }
    fin.getline(raConfig,sizeof(raConfig),'\n');
    CE[2][5]=stoi(raConfig);

	double OffsetSNR[3]={0,10,20};
	for (int i = 0; i < number_of_target_SNR; ++i)
	{
		NPRACH_Struc.target_SNR[i]=14.25-OffsetSNR[i];//14.25 dB, 4.25 dB, -5.75 dB
		// LOG("%d,%lf\n",number_of_target_SNR,NPRACH_Struc.target_SNR[i]);
		// system("pause");
		if(NPRACH_Struc.target_SNR[i]==14.25)//MCL:144 DB
		{
			NPRACH_Struc.CE_Level[i]=CE[i][0];
			NPRACH_Struc.period[i]=CE[i][1];
			NPRACH_Struc.start_time[i]=CE[i][2];//offset from start of NPRACH period
			NPRACH_Struc.subcarrier_Offset[i]=CE[i][3];
			NPRACH_Struc.num_Subcarrier[i]=CE[i][4];
			NPRACH_Struc.rep[i]=CE[i][5];
			NPRACH_Struc.subcarrierMSG3_RangeStart[i]=2/3;
			NPRACH_Struc.npdcch_NumRepetitions_RA[i]=1;
			//limitation tesing
			if(NPRACH_Struc.period[i]<=(5.6)*NPRACH_Struc.rep[i])
			{
				cout << "Error: the NPRACH-Config(CE0) is exceed TimeDomain resource!!" << endl;
				system("pause");
        		exit(1);
			}
		}
		else if(NPRACH_Struc.target_SNR[i]==4.25)//MCL:154 DB
		{
			NPRACH_Struc.CE_Level[i]=CE[i][0];
			NPRACH_Struc.period[i]=CE[i][1];
			NPRACH_Struc.start_time[i]=CE[i][2];//offset from start of NPRACH period
			NPRACH_Struc.subcarrier_Offset[i]=CE[i][3];
			NPRACH_Struc.num_Subcarrier[i]=CE[i][4];
			NPRACH_Struc.rep[i]=CE[i][5];
			NPRACH_Struc.subcarrierMSG3_RangeStart[i]=2/3;
			NPRACH_Struc.npdcch_NumRepetitions_RA[i]=2;
			//limitation tesing
			if(NPRACH_Struc.period[i]<=(5.6)*NPRACH_Struc.rep[i])
			{
				cout << "Error: the NPRACH-Config(CE1) is exceed TimeDomain resource limitation!!" << endl;
				system("pause");
        		exit(1);
			}
		}
		else if(NPRACH_Struc.target_SNR[i]==-5.75)//MCL:164 DB
		{
			NPRACH_Struc.CE_Level[i]=CE[i][0];
			NPRACH_Struc.period[i]=CE[i][1];
			NPRACH_Struc.start_time[i]=CE[i][2];//offset from start of NPRACH period
			NPRACH_Struc.subcarrier_Offset[i]=CE[i][3];
			NPRACH_Struc.num_Subcarrier[i]=CE[i][4];
			NPRACH_Struc.rep[i]=CE[i][5]; //1.4*4*4 = 5.6*4=24.4 ms
			NPRACH_Struc.subcarrierMSG3_RangeStart[i]=2/3;
			NPRACH_Struc.npdcch_NumRepetitions_RA[i]=4;
			NPRACH_Struc.first_end_time=NPRACH_Struc.start_time[i]+int(NPRACH_Struc.rep[i] * 5.6+0.5);//used for estimate start pos of UL data
			if(NPRACH_Struc.period[i]<=(5.6)*NPRACH_Struc.rep[i])
			{
				cout << "Error: the NPRACH-Config(CE2) is exceed TimeDomain resource!!" << endl;
				system("pause");
        		exit(1);
			}
		}
		else
			return -1;
	}
	int checkFreqArray[48]={0};
	int errorReason=-1;
	for (int i = 0; i < 3; ++i)
	{
		if(i==0)
		{
			for (int j = NPRACH_Struc.subcarrier_Offset[i]; j < NPRACH_Struc.subcarrier_Offset[i]+NPRACH_Struc.num_Subcarrier[i]; ++j)
			{
				if(checkFreqArray[j]==0)	checkFreqArray[j]=1;
				else	errorReason=0;
			}
		}
		else if(i==1)
		{
			for (int j = NPRACH_Struc.subcarrier_Offset[i]; j < NPRACH_Struc.subcarrier_Offset[i]+NPRACH_Struc.num_Subcarrier[i]; ++j)
			{
				if(checkFreqArray[j]==0)	checkFreqArray[j]=2;
				else	errorReason=1;
			}
		}
		else if(i==2)
		{
			for (int j = NPRACH_Struc.subcarrier_Offset[i]; j < NPRACH_Struc.subcarrier_Offset[i]+NPRACH_Struc.num_Subcarrier[i]; ++j)
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
