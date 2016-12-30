#include <algorithm>
#include <iostream>
#include <list>
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
	int mcs;//mcs Index
	int round;
	int remaining_Buffer_Sizes;
	//NPUSCH RU Table
	double freq_Space;
	int num_subcarrier_one_RU;
	int num_UL_Slot;
	int UE_num_RU;
};

bool compareMyType (const UL_UE_list &a, const UL_UE_list &b)
{
	return b.DV < a.DV;
	// if(a2->round!=b2->round)	return b2->round - a2->round;
	// else	return b2->DV - a2->DV;
}

int main()
{
	std::list<UL_UE_list> o;
	int index[10] = { 3, 1, 7, 4, 2, 1, 9, 7, 2, 1};
	int cnt = 0;

	for (int i = 0; i < 10; ++i)
	{
		UL_UE_list a;
		a.DV = index[i];
		o.push_back(a);
	}

	o.sort(compareMyType);

	for (std::list<UL_UE_list>::iterator i = o.begin(); i != o.end(); ++i)
		std::cout << (&*i)->DV << std::endl;

	return 0;
}
