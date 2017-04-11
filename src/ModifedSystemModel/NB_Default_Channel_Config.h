#ifndef NB_Default_Channel_Config_H


#define NB_Default_Channel_Config_H
#define NUMBER_OF_UE_MAX 10
#define num_UL_Subcarrier 48
#define num_DL_Subcarrier 12
//Msg Identity
// #define preamble 1
// #define Msg2 2
// #define Msg3 3
// #define Msg4 4
// #define DCI 5


typedef struct {
	int **resourceStruc;
} UL_Anchor_Channel_Structure;

typedef struct {
	int **resourceStruc;
} DL_Anchor_Channel_Structure;

// typedef struct {
// 	int UE_id;		//BS identify UE by UE_id
// 	int Priority;	//BS determine priority depends on DV/BSR, more tone be used or CE level
// 	int CE_Level;	//After RA Succeed, UE will know own CE level.
// 	int BSR;	//BS know UE's UL Buffer Data Size.(UP solution)
// 	int DV;	//BS know UE's UL Buffer Data Size.(CP solution)
// 	int UL_Buffer_Sizes;
// 	int PHR;	//BS know UE's Power budget.
// 	int multi_tone;// 0: not support; 1:support
// 	int mcs;//mcs Index
// 	int round;
// 	int remaining_Buffer_Sizes;
// 	//NPUSCH RU Table
// 	double freq_Space;
// 	int num_subcarrier_one_RU;
// 	int num_UL_Slot;
// 	int UE_num_RU;
// }UL_UE_list;

class UL_UE_list
{
public:
	int UE_id;		//BS identify UE by UE_id
	int Priority;	//BS determine priority depends on DV/BSR, more tone be used or CE level
	int CE_Level;	//After RA Succeed, UE will know own CE level.
	int BSR;	//BS know UE's UL Buffer Data Size.(UP solution)
	int DV;	//BS know UE's UL Buffer Data Size.(CP solution)
	int UL_Buffer_Sizes;
	int PHR;	//BS know UE's Power budget.
	int multi_tone;// 0: not support; 1:support
	int multi_tone_Msg3;// 0: not support; 1:support
	int mcs;//mcs Index
	int round;
	int remaining_Buffer_Sizes;
	//NPUSCH RU Table
	int startScheMsg3;
	int startScheULdata;
	double freq_Space;//Not used
	int num_subcarrier_perRU;
	int num_UL_Slot;
	int UE_num_RU;
	int remainging_subframe;
	bool resourceAlloc;
};

typedef struct {
	int UE_id;		//BS identify UE by UE_id
	int Priority;	//BS determine priority depends on Buffer_Size or CE level
	int CE_Level;	//After RA Succeed, UE will know own CE level.
	int Buffer_Size;	//BS know UE's DL Buffer Data Size.(CP/UP solution)
} DL_UE_list;


typedef struct {
	int Flag_N0;//0:N0, 1:N1
	int subcarrierIndication;
	int resourceAssignment;
	int schedulingDelay;
	int mcs;//Fixed
	int RV;//Fixed
	int repetitionNumber;
	int NDI;//Not used
	int DCI_repetitionNumber;//Fixed setting by NPDCCH-Config
} DCI_N0;

typedef struct {
	int Flag_N0;//0:N0, 1:N1
	int NPDCCH_orderIndicator;//(not consider initial RA,set to 0)
	int resourceAssignment;
	int schedulingDelay;
	int mcs;//Fixed
	int repetitionNumber;
	int NDI;//Not used
	int HARQ_ACK_resource;//Not used
	int DCI_repetitionNumber;//Fixed setting by NPDCCH-Config
} DCI_N1;

//NB UL Anchor Carrier for default channel

// typedef struct {
// /*Test Parameters Setting*/
// 	int rachperiod[8]={40,80,160,240,320,640,1280,2560};
// 	int rachstart[8]={8,16,32,64,128,256,512,1024};
// 	int rachrepeat[8]={1,2,4,8,16,32,64,128};
// 	int rawindow[8]={2,3,4,5,6,7,8,10};// Not include in MAC_Command in shell script
// 	int rmax[12]={1,2,4,8,16,32,64,128,256,512,1024,2048};
// 	double gvalue[8]={1.5,2,4,8,16,32,48,64};
// 	int candidate[4]={1,2,4,8};

// 	/*Not include shell script*/
// 	double pdcchoffset[4]={0,0.125,0.25,0.375};// Not include in MAC_Command in shell script

// 	/*Not include shell script*/
// 	int dlcqi[13]={0,1,2,3,4,5,6,7,8,9,10,11,12};
// 	int dlrepeat[16]={1,2,4,8,16,32,64,128,192,256,384,512,768,1024,1536,2048};
// 	int rachscofst[7]={0,12,24,36,2,18,34};
// 	int rachnumsc[4]={12,24,36,48};
// } NB_SIB2_Combination;


// // NPRACH* RSRP_Threshold = new NPRACH[3];
// // delete[] RSRP_Threshold; //
typedef struct {
	double target_SNR[3];
	int CE_Level[3];
	int rep[3];
	int period[3];//nprach-Periodicity
	int start_time[3];
	int first_end_time;//used for estimate start pos of UL data
	int num_Subcarrier[3];
	int subcarrier_Offset[3];
	//DCI N1 for RAR and Msg4, DCI N0 for Msg3 retransmission
	int npdcch_NumRepetitions_RA[3];//Maximum number of repetitions for NPDCCH common search space (CSS) for RAR, Msg3 retransmission and Msg4
	int npdcch_Offset_RA[3];//Fractional period offset of starting subframe for NPDCCH common search space (CSS Type 2)
	int npdcch_StartSF_CSS_RA[3];//Starting subframe configuration for NPDCCH common search space (CSS), including RAR, Msg3 retransmission, and Msg4
	int rawindow[3];//in RACH-ConfigCommon-NB-r13
	int candidate[3];
	int ContentionResolutionTimer;//in RACH-ConfigCommon-NB-r13
	int I_SI_NPRACH;//44ms, a gap, could be variale
	//The following value depend on NPDCCH config and DCI format
	int I_Wait_to_NPDCCH;//Interval from end pos of preamble to start DCI N1(RAR) and RAR
	int I_NPDCCH_RAR;//DCI_repetition+ko
	int I_RAR_Msg3;//Interval from end of RAR to start of Msg3
	int I_Msg3_DCIN0;//Interval from end pos of Ms3 to start pos of DCI N0
	int I_DCIN0_ULPacket;//Interval from start of DCI0,DCI Rep to start of ULPacket
	int DL_repetiitonLevel;
} NB_SIB2;

typedef struct {
	struct Msg2{
	int Rep;
     };
	struct Msg4{
	int Rep;
     };
} NPDSCH;

//NB DL Anchor Carrier for default channel
// int NPSS(int x,int y);
// int NSSS(int x,int y);
// int PBCH(int x,int y);
// int NPDSCH_SIB(int x,int y);


//Initialization
// bool compareMyType (const UL_UE_list, const UL_UE_list);
void UE_Init(UL_UE_list &);
//NB UL Scheduler Function incldue resource allocation
int Auto_Filter_NB_SIB2Config(double **, double **);
int define_Channel_Structure(UL_Anchor_Channel_Structure &, DL_Anchor_Channel_Structure &, int);
int Set_NPRACH_Resource(double **, NB_SIB2 &, int);
int checkFrequencyDomain(NB_SIB2 &);



int do_NPUSCH_Resource_Allocation(UL_UE_list &,UL_Anchor_Channel_Structure &,int,int &);
// int do_NPUSCH_Scheduler(int,int &,int,NB_SIB2 &,UL_Anchor_Channel_Structure &);
int get_UL_Data_Bytes(int);

int get_remaining_UL_resource(int,int &,UL_Anchor_Channel_Structure &);
// int get_resource_allocation(int);
bool check_ULChannel(UL_Anchor_Channel_Structure &,int,int,int);
int nprachResourceMapping(int,int,int,NB_SIB2 &,UL_Anchor_Channel_Structure &);
int get_startFreqPos(UL_Anchor_Channel_Structure &,int &,int,int);

int get_num_subcarrier_perRU(int);
int get_num_Slot(int);
int get_TBS_UL(int,int);
int get_I_TBS(int);
int get_I_RU(int);
#endif
