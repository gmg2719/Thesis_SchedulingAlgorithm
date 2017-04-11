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
	return a.DV > b.DV; // means The first one always larger than the later one.
	// return a.DV < b.DV; // means The later one always larger than the first one.
	// if(a2->round!=b2->round)	return b2->round - a2->round;
	// else	return b2->DV - a2->DV;
}
int do_NPUSCH_Resource_Allocation(int remaining_UL_resource)
{
	static int x=0;
	static int remainging_subframe=remaining_UL_resource;
	// int x=1;
	// remainging_subframe=remainging_subframe+x;
	std::cout << "remainging_subframe:"<<remainging_subframe<<std::endl;
	++x;
}

int main()
{
	int remaining_UL_resource=10;
	std::list<UL_UE_list> o;
	int index[10] = { 3, 1, 7, 4, 2, 1, 9, 7, 2, 1};
	int cnt = 0;

	for (int i = 0; i < 10; ++i)
	{
		do_NPUSCH_Resource_Allocation(remaining_UL_resource);
		UL_UE_list a;
		a.DV = index[i];
		o.push_back(a);
		++remaining_UL_resource;
	}

	o.sort(compareMyType);

	for (std::list<UL_UE_list>::iterator i = o.begin(); i != o.end(); ++i)
		std::cout << (&*i)->DV << std::endl;

	return 0;
}

//For Linux a kind of way to write Shell Script
// The #define SHELLSCRIPT directive is used in C to define a named constant: SHELLSCRIPT which contains the shell script.

// The back slash \ at the end of each line is used to type the code in next line for better readability.

// The new line \n is used to put the code in next line.

// Double quotes " within the shell script has to be escaped \" to distinguish it between the original opening and closing double quotes of constant value.

// Finally the system function is called by passing the named constant: SHELLSCRIPT as argument which contains the shell script.
// #include <stdio.h>
// #include <stdlib.h>

// #define SHELLSCRIPT "\
// #/bin/bash \n\
// echo \"hello\" \n\
// echo \"how are you\" \n\
// echo \"today\" \n\
// "

// int main()
// {
//     puts("Will execute sh with the following script :");
//     puts(SHELLSCRIPT);
//     puts("Starting now:");
//     system(SHELLSCRIPT);
//     return 0;
// }
