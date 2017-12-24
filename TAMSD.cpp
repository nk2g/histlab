#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
//#include <mpi.h>

using namespace std;

int main(int argc, char *argv[]) {
//	MPI_Init(&argc,&argv);
//	int rank; int size;
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
	string topfilepath = "";
	string membranefilepath = "";
	string infilepath = "trajxvg1-50us/";
	string outfilepath = "TAMSD1-50us/";
	
	string topfilename="system.top";
	string grp;
	int endframe = 500000;	//50 us
	int removeframe = 50000; // 5 us

	float *memtime;
	float *memx; float *memy; float *memz;

	float *time;
	float *x; float *y; float *z;
	cout << "start" << endl;
	
//	int calcsize = lipidnum/size;

//read topfile
	ifstream ifstop(topfilepath + topfilename);
	string strtop;
	
	string lipidname[30];
	int lipidnum[30];

	if(ifstop.fail()){
		cout << "cannot open topfile" << endl;
		return -1;
	}
	int i=0;
	int removeline=10;
	while(getline(ifstop, strtop)){
		string token;
		istringstream stream(strtop);
		if(i>=removeline){
			int j=0;
			while(getline(stream, token, ' ')){
				if(j%2){
					lipidnum[i-removeline]=stoi(token.c_str());
				}else{
					lipidname[i-removeline]=token;
				}
				j++;
			}
		}
		i++;
	}

	for(int i=0; i<=17; i++){
		cout << lipidname[i] << endl;
		struct stat st;
		string buf="";
		buf = outfilepath + lipidname[i];
		if(stat(buf.c_str(),&st) != 0){
			mkdir(buf.c_str(),0775);
		}
	}
//read topfile end
//read membranefile

	memtime = (float *)malloc(endframe*sizeof(float)*2);
	memx = (float *)malloc(endframe*sizeof(float)*2);
	memy = (float *)malloc(endframe*sizeof(float)*2);
	memz = (float *)malloc(endframe*sizeof(float)*2);

 
	string membranefilename = membranefilepath + "membrane0-50us.xvg";
	ifstream ifsmem(membranefilename);
	string strmem;
	if(ifsmem.fail()){
		cout << "cannot open membrane file" << endl;
		return -1;
	}
	i=0;
	while(getline(ifsmem, strmem)){
		sscanf(strmem.c_str(), "%f %f %f %f", &memtime[i], &memx[i], &memy[i], &memz[i]);
		i++;
	}
	free(memtime);free(memz);

//read membranefile end

	int buf=0;

	for(int shurui=0; shurui<=17; shurui++){// 0 to 17
		for(int num = 1+buf; num <= buf+lipidnum[shurui]; num++){
		string infilename = "";
		string outfilename = "";	
		
		time = (float *)malloc(endframe*sizeof(float)*2);
		x = (float *)malloc(endframe*sizeof(float)*2);
		y = (float *)malloc(endframe*sizeof(float)*2);
		z = (float *)malloc(endframe*sizeof(float)*2);
//read file
		infilename = infilepath + "r_" + to_string(num) + ".xvg";
		ifstream ifs(infilename);
		string str;
		if (ifs.fail()) {
			cout << "cannot open infile" << endl;
			return -1;
		}

		i=0;
		while (getline(ifs, str)) {
			sscanf(str.c_str(), "%f %f %f %f", &time[i], &x[i], &y[i], &z[i]);
			i++;
		}
		i = 0;
//read file end;
//TAMSD output
		outfilename = outfilepath + lipidname[shurui] + "/r_" + to_string(num) + ".csv";
		ofstream ofs(outfilename);
		if (ofs.fail()) {
			cout << "cannot open outfile" << endl;
			return -1;
		}

		//TAMSD
		long double r2;
		i=0;
		int tmp=0;
		for (long delta = 1; delta <= endframe/2; delta = (int)pow(10,0.1*i)) {
			if(delta != tmp){
				r2 = 0;
				for (int frame = removeframe; frame <= endframe-delta; frame++) {
					r2 += pow((x[frame + delta] - x[frame] - (memx[frame+delta]-memx[frame])), 2) + pow((y[frame + delta] - y[frame]-(memy[frame+delta]-memy[frame])), 2);
				}
				ofs << time[delta] << "," << r2 / (time[endframe] - time[delta]) << endl;
			}else{
			}
			i++;
			tmp = delta;
		}
//TAMSD output end
		cout << "r_" << num << endl;
		free(time); free(x); free(y); free(z);
		ofs.close();				
		}
		buf+=lipidnum[shurui];		
	}
	cout << "end" << endl;
//	MPI_Finalize();
	return 0;
}
