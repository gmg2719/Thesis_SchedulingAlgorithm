#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

const int user = 100;
string *Split2(string ,char);
double exp_arrival_time(double);
void define_system_arrival();


int timeslot;
double gamma ;
double lambda ;
double packet_delay_time[user];
double packetArrivalTime[user];
double temparrival;
double packetArrivalPos=0;
// #include <boost/random/uniform_real_distribution.hpp>
// #include <boost/random/mersenne_twister.hpp>
// #include <boost/random/variate_generator.hpp>
// #include <boost/math/distributions/pareto.hpp>


int main()
{

	// boost::mt19937 randGen(15); //held constant for repeatability
	// boost:math::pareto_distribution<> dist;
	// boost::variate_generator<boost::mt19937&,boost::random::uniform_real_distribution<> > generator(randGen, dist);

	// open the input file
    fstream fin;
    //fin.open("12.in", fstream::in);
    fin.open("NPRACH_Configuration.in", fstream::in);
    if(!fin.is_open())
    {
        cout << "Error: the input file is not opened!!" << endl;
        exit(1);
    }
    char title[1000];
    fin.getline(title,sizeof(title),'\n');
    cout << title << endl;
    for (int i = 0; i < 5; ++i)
    {
    	fin.getline(title,sizeof(title),',');
    	int a=stoi(title);
    	cout << a << endl;
    }
    fin.getline(title,sizeof(title),'\n');
    int b=stoi(title);
    cout << b << endl;
    for (int i = 0; i < 5; ++i)
    {
    	fin.getline(title,sizeof(title),',');
    	cout << title << endl;
    }
    fin.getline(title,sizeof(title),'\n');
    cout << title << endl;
    for (int i = 0; i < 5; ++i)
    {
    	fin.getline(title,sizeof(title),',');
    	cout << title << endl;
    }
    fin.getline(title,sizeof(title),'\n');
    cout << title << endl;
	system("pause");
	return 0;
}

double exp_arrival_time(double lambda){
	double t = (-1 / lambda)*log(1 - ((rand() / (double)RAND_MAX)));
	printf("%lf\n",t);
	system("pause");
	return t;
}

void define_system_arrival(){
	for (int i = 0; i < user; i++){
		packetArrivalTime[i] = 0;
	}
	for (int j = 0; j < user; j++){
		int u = rand() % user ; // randomly choose 0 ~ user-1
		if (packetArrivalTime[u] == 0){
			//temparrival = temparrival + exp_arrival_time(lambda);
			packetArrivalTime[u] = exp_arrival_time(lambda) ; //rivised
			packetArrivalPos= packetArrivalPos + packetArrivalTime[u] ;
		}
		else{
			j = j - 1;
		}
		//cout<< "packet_arrival_pos"<< j + 1 <<"="<<packet_arrival_pos<<endl;
		//cout <<"user " << j + 1<<" arrival time = " << packet_arrival_time[u] <<end1 ;
		//system("pause");
	}
	timeslot = ceil(packetArrivalPos);
	//cout<<"Final_Packet_pos"<<"=" << packet_arrival_pos<<endl;
	//system("pause");
	packetArrivalPos=0;
}


