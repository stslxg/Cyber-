#define MAXROW 60
#define MAXCOL 90
using namespace std;

#include <fstream>
#include <sstream>
#include <iostream>
#include "image-v0.1.2.h"

#include "para.h"
#include <cstdlib>
const int fileNum = 22;
unsigned short image[MAXROW][MAXCOL];

int main(){
  char c[8];

  paraData.paraTable[1] = 2;
  paraData.paraTable[2] = 90;
  paraData.paraTable[3] = 10;
  paraData.paraTable[4] = 50;
  paraData.paraTable[5] = 50;
  paraData.paraTable[6] = 180;
  paraData.paraTable[7] = 20;
  paraData.paraTable[8] = 140;
  paraData.paraTable[9] = 60;
  paraData.paraTable[10] = 30;
  paraData.paraTable[11] = 60;
  paraData.paraTable[12] = 20;
  paraData.paraTable[13] = 60;



  ofstream fout("out.txt");
  ifstream fin;
  for(int i = 0;i<fileNum;i++){
    InitLine();
    
    sprintf(c,"c%d.txt",i);     
    fin.open(c);
    for(int j = 0;j<MAXROW;j++)
      for(int k = 0;k<MAXCOL;k++)
	fin >> image[j][k];
    fin.close();
    GetLine();
    GetSituation();
    if(hasStartLine == True) cout << i << endl;
    fout << forwardDistance << ' ';
    for(int j = 0;j<forwardDistance;j++)
      fout << blackLine[j] << ' ';
    fout << endl;
  };
};
