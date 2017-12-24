#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include "gmxcpp/Utils.h"
#include "gmxcpp/Topology.h"
#include "gmxcpp/Trajectory.h"
// #include <mpi.h>

using namespace std;

void GetArea(Trajectory *traj,int filenum);


//void print(int frame, coordinates com);
/** Parses the command line, then prints data to standard out */
int main(int argc, char *argv[])
{
    Index ndx("dat/ndx4096.ndx");
    //Topology top("step7_production.tpr",ndx);
    for(int filenum=10;filenum < 11; filenum++){
    	Trajectory *traj = new Trajectory("dat/md0010_membrane.xtc",ndx);
    	traj->read();
		GetArea(traj,filenum);
		cout << "filenum: " << filenum << "...end" << endl;
	}
	//MPI::Finalize();
    return 0;
}

void GetArea(Trajectory *traj,int filenum){

	cout << "Area" << endl;
	string outfile = "Area/areaperframe"+to_string(filenum)+".csv";
	ofstream ofs;
	ofs.open(outfile.c_str());

	if(!ofs.is_open()){
		cout << "cannot open file" << endl;
		return;
	}

	const int nFrames = traj->GetNFrames();
	cout << nFrames << endl;
	cubicbox box;
	for(int frame=0;frame < nFrames-1; frame++){
		box=traj->GetCubicBox(frame);
		ofs << frame << "," << box[X]*box[Y] << endl;
	}
	ofs.close();
}
