#include <iostream>
using namespace std;

template <typename T, int count>
void loopIt(T x)
{
   T val[count];

   for(int ii=0; ii<count; ii++)
   {
       // val[ii] = x++;
   		val[ii]=++x;
       cout <<  val[ii] << endl;
   }
};

main()
{
   float xx = 2.1;

   loopIt<float,3>(xx);
}
