//
//  main.cpp
//  hw5
//
//  Created by Ilknur Bas on 28.11.2020.
//  Copyright © 2020 Ilknur Bas. All rights reserved.
//
 //
#include <iostream>
#include <stdio.h>
using namespace std;
#include <getopt.h>
#include <unistd.h>
#include <fstream>
#include <cmath>
#include <time.h>
#include <chrono>


//./alignSeqToProfile --fasta seq.fasta --aln aligned_sequences.aln --out seq.aln --gap  0 --match 2 --mismatch -2

string readFasta(string fastaName);
void readAln(string alnName);
void frequencyTable();
double calculateScore(char ch, int column,int mismatch_, int match );
void naive(string str1, int match, int mismatch_, int gap);
void outputFile(string outFileName);

int seqNumber=0;
char aln[10][500];
double frequency[5][500]; //A C G T -
int match;
int mismatch_;
int gap;
char outAln[10][500];
string fasta="";
string sequence1="";

int main(int argc, char * argv[]) { //int main(int argc, const char * argv[]) {
	int select=0;
	int index;
	
	string alnName="";
	string fastaName="";
	string outFileName="";
	
	const struct option long_options[] =
	{
		{"aln",required_argument,0,'a'},
		{"fasta",required_argument,0,'f'},
		{"gap",required_argument,0,'g'},
		{"match",required_argument,0,'m'},
		{"mismatch",required_argument,0,'s'},
		{"out",required_argument,0,'o'},
		{0,0,0,0}
	};
	
	
	//select = getopt_long( argc, argv, "i:p:q", long_options , &index);
	select = getopt_long( argc, argv, "a:f:g:m:s:o", long_options , &index);
	
	while( select!= -1){
		switch(select){
			case 'a':{
				alnName=string(optarg);
				cout<< "a:"<<alnName<<"\n";
				select = getopt_long( argc, argv, "a:f:g:m:s:o", long_options , &index);
				break;
			}
			case 'f':{
				fastaName=string(optarg);
				cout<< "f:"<<fastaName<<"\n";
				select = getopt_long( argc, argv, "a:f:g:m:s:o", long_options , &index);
				break;
			}
			case 'g':{
				gap = atoi(optarg);
				cout<< "g:"<<gap<<"\n";
				select = getopt_long( argc, argv, "a:f:g:m:s:o", long_options , &index);
				break;
			}
			case 'm':{
				match = atoi(optarg);
				cout<< "m:"<<match<<"\n";
				select = getopt_long( argc, argv,"a:f:g:m:s:o", long_options , &index);
				break;
			}
			case 's':{
				mismatch_ = atoi(optarg);
				cout<< "s:"<< mismatch_<<"\n";
				select = getopt_long( argc, argv,"a:f:g:m:s:o", long_options , &index);
				break;
			}
			case 'o':{
				outFileName=string(optarg);
				cout<< "o:"<<outFileName<<"\n";
				select = getopt_long( argc, argv, "a:f:g:m:s:o", long_options , &index);
				break;
			}
			default:{
				select=-1;
			}
		}
	}
	
	//Read fasta file
	string fasta=readFasta(fastaName);
	cout<<"fasta in main function:"<<fasta<<endl;
	cout<<endl;
	//Read aln file
	cout<<"aln in main function:"<<endl;
	readAln(alnName);
	for(int k=0;k<seqNumber;k++){
		for(int l=0;l<500 ;l++){
			if(aln[k][l]=='0'){
				break;
			}else{
				cout<<aln[k][l];
			}
		}
		cout<<endl;
	}
	cout<<endl;
	
	//Make frequency table
	cout<<"seqNumber"<<seqNumber<<endl;
	cout<<"Frequency table:"<<endl;
	frequencyTable();
	for(int k=0;k<5;k++){
		for(int l=0;l<500 ;l++){ //500
			if(frequency[k][l]==-1){
				//cout<<"Boşluk in main.'-1'"<< k <<endl;
				break;
			}else{
				cout<<frequency[k][l]<<" ";
			}
		}
		cout<<"freq"<<endl;
	}
	
	cout<<endl;
	//na ̈ıve implementation of Needleman-Wunsch
	naive(fasta, match, mismatch_, gap);
	
	//output file
	outputFile(outFileName);
	
}

void outputFile(string outFileName){
	
	/*for (int i = 0; i < seqNumber; i++) {
		cout<<"sequence"<<i+1<<" "<<outAln[i]<<endl;
	}
	cout<<"sequence"<<" "<<sequence1<<endl;*/
	
	//output file format
	ofstream myfile;
	myfile.open ("seq.aln");
	for (int i = 0; i < seqNumber; i++) {
		myfile << "sequence" <<i+1<<" "<<outAln[i] <<" \n";
	}
	myfile << "sequence " <<sequence1 <<" \n";
	myfile.close();
	
	
	
}
void naive(string str1, int match, int mismatch_, int gap){
	
	int row=str1.length()+1;
	int column = 35; //Değişşşş
	cout<<"row and column in navive function: "<<row<<","<<column<<endl;
	
	double scoreTable[row][column];
	int backtrack[row][column]; //for trackbacking purposes
	
	//??
	backtrack[0][0]=0; //only for this cell otherwise diagonal:0 coming left:1 coming up:2
	scoreTable[0][0]=0;
	
	for (int i = 1; i < row; i++) {
		scoreTable[i][0] = gap * i;
		backtrack[i][0] = 2; //coming from up
	}
	for (int j = 1; j < column; j++) {
		for (int a = 0; a <= j; a++){
			scoreTable[0][j] += calculateScore('-', a, mismatch_, match );
		}
		backtrack[0][j] = 1; //coming fromleftup
	}
	
	//Create table
	for (int i = 1; i < row; i++) {
		for (int j = 1; j < column; j++) {
			
			double diagonal=0;
			diagonal= scoreTable[i-1][j-1] + calculateScore(str1[i-1], j-1, mismatch_, match ); //i-1leri kontrol et
			
			//down ??
			double down=0;
			down=scoreTable[i-1][j]+gap;
			
			
			//right ??
			double right=0;
			right=scoreTable[i][j-1]+calculateScore('-', j-1, mismatch_, match); //alignGap
			
			//compare and decide
			//Değiştirilebilir bu emin de değilim.
			if ( diagonal >= right && diagonal >= down ){
				//cout<<"diagonal is largest:"<<diagonal<<endl;
				scoreTable[i][j]=diagonal;
				backtrack[i][j] = 0;
			}
			else if ( down > diagonal && down >= right ){
				//else if ( down > right ){
				//cout<<"down is largest:"<<down<<endl;
				scoreTable[i][j]=down;
				backtrack[i][j] = 2;
			}
			else {
				//if ( right >= diagonal && right >=  down ){
				//cout<<"right is largest:"<<right<<endl;
				scoreTable[i][j]=right;
				backtrack[i][j] = 1;
			}
		
			
			
		}
	}
	
	//Print score table
	for(int i = 0; i<row; i++){
		for(int j = 0; j<column; j++){
			//cout<<scoreTable[i][j]<<"-"<<backtrack[i][j]<<" ";
			cout<<scoreTable[i][j]<<" ";
		}
		cout<<endl;
	}
	
	
	//backtrace and print the alignment with best score
	//global da son row ve columndan başlanır
	int rowTraverse=row-1;
	int columnTraverse=column-1;
	
	cout<<"Global score:"<<scoreTable[rowTraverse][columnTraverse]<<endl;
	
	int i=rowTraverse;
	int j=columnTraverse;
	

	while(i>=0 || j>=0){
		int traverse=backtrack[i][j];
		if(traverse==0){ //diagonal
			sequence1=str1[i-1]+sequence1;//sil
			for (int k = 0; k < seqNumber; k++){
				outAln[k][j-1] = aln[k][j-1];
			}
			i--;
			j--;
		}else if(traverse==1){ //right
			sequence1= '-'+sequence1;//sil
			for (int k = 0; k < seqNumber; k++){
				outAln[k][j-1] = aln[k][j-1];
			}
			j--;
		}else  if(traverse==2) { //down
			sequence1= str1[i-1]+sequence1; //sil
			for (int k = 0; k < seqNumber; k++){
				outAln[k][j-1] = '-';
			}
			i--;
		}
	}
	
	//print sequence on console
	//cout<<"sequence1 in naive:"<<sequence1<<endl;
	
}


double calculateScore(char ch, int column,int mismatch_, int match ){

	double score=0;
	
	if(ch=='A'){ //y=A
		score=score+ frequency[0][column] * match;
	}else{
		score=score+ frequency[0][column] * mismatch_;
	}
	
	if(ch=='G'){
		score=score+ frequency[2][column] * match;
	}else{
		score=score+ frequency[2][column] * mismatch_;
	}
	
	if(ch=='C'){
		score=score+ frequency[1][column] * match;
	}else{
		score=score+ frequency[1][column] * mismatch_;
	}
	
	if(ch=='T'){
		score=score+ frequency[3][column] * match;
	}else{
		score=score+ frequency[3][column] * mismatch_;
	}
	
	if(ch=='-'){ //GAP SCORE????? match
		//score=score+ frequency[4][column] * match;
		score=score+ frequency[4][column] * match;
	}else{
		//score=score+ frequency[4][column] * mismatch_;
		score=score+ frequency[4][column] * mismatch_;
	}
		
	//cout<<"Score in calculateScore fucntion:"<<score<<endl;
	return score;
}


void frequencyTable(){
	
	double a=0;
	double cc=0;
	double g=0;
	double t=0;
	double gapp=0;
	for(int k=0;k<5;k++){
		for(int l=0;l<500 ;l++){
			frequency[k][l]=-1;
		}
	}
	
	bool check=true;
	for (int i = 0; i < 500; i++) {
		for (int j = 0; j < seqNumber; j++) {
			if(aln[j][i]=='0'){
				//cout<<"Boşluk in frequency tablewhen i,j:"<<i<<" "<<j<<endl;
				check=false;
			}else{
				//cout<<" aln[j][i] "<<aln[j][i];
				if(aln[j][i]=='A'){
					a++;
					
				}else if(aln[j][i]=='C'){
					cc++;
					
				}else if(aln[j][i]=='G'){
					g++;
					
				}else if(aln[j][i]=='T'){
					t++;
					
				}else if(aln[j][i]=='-'){
					gapp++;
				}
			}
		}
		if(!check){
			break;
		}else{
			//For A
			frequency[0][i]= a/seqNumber;
			//cout<<"frequency[0][i] for A "<<frequency[0][i] <<"i "<<i<<endl;
			//For C
			frequency[1][i]= cc/ seqNumber;
			//For G
			frequency[2][i]= g/ seqNumber;
			//For T
			frequency[3][i]=  t/ seqNumber;
			//For -
			frequency[4][i]= gapp/ seqNumber;
			a=0;
			cc=0;
			g=0;
			t=0;
			gapp=0;
			
		}
		
	}
	
	
}

void readAln(string alnName){
	for(int k=0;k<10;k++){
		for(int l=0;l<500 ;l++){
			aln[k][l]='0';
		}
	}
	ifstream file;
	file.open(alnName);
	string str;
	bool check=true;
	int j=0;
	
	if (file.is_open()){
		cout<<"Opened in readAln."<<endl;
		int k=0;
		while (getline(file,str,'\n')) {
			for(int i=0;i<str.length();i++){
				if(str[i]==' '){
					i++;
					check=false;
					k=0;
				}
				if(!check){
					aln[j][k]=str[i];
					k++;
				}
			}
			j++;
			check=true;
		}
	}else{
		cout<<"Close."<<endl;
	}
	file.close();
	
	seqNumber=j;
	
}


string readFasta(string fastaName){
	
	ifstream file;
	file.open(fastaName);
	fasta="";
	string str;
	
	if (file.is_open()){
		cout<<"Opened."<<endl;
		while (getline(file,str,'\n')) {
			if(str[0]=='>'){
			}else{
				fasta=fasta+str;
			}
		}
	}else{
		cout<<"Close."<<endl;
	}
	file.close();
	cout<<"fasta in readFasta function:"<<fasta<<endl;
	return fasta;
}









