//
//  allalign.cpp
//  hw3
//
//  Created by Ilknur Bas on 19.11.2020.
//  Copyright © 2020 Ilknur Bas. All rights reserved.
//
//./allalign --mode aglobal --input sequences.fasta --gapopen -5 --gapext -2
//./allalign --mode alocal --input sequences.fasta --gapopen -5 --gapext -2

//!!
//./allalign --mode local --input sequences.fasta --gapopen -5
//./allalign --mode global --input sequences.fasta --gapopen -5


#include <iostream>
#include <stdio.h>
using namespace std;
#include <getopt.h>
#include <unistd.h>
#include <fstream>
#include <cmath>
#include <time.h>
#include <chrono>
/* for getopt_long; POSIX standard getopt is in unistd.h */

int rowGlobal=0;
int columnGlobal=0;
string sequence1;
string sequence2;

int navive(string str1, string str2, int matchScore, int mismatchScore, int gapopen,string option);
int affine(string str1, string str2, int matchScore, int mismatchScore, int gapopen, string option,  int gapExtention);

int main(int argc,  char * argv[]) { //int main(int argc, const char * argv[]) {
	int select=0;
	int index;
	int gapOpen = 0;
	int gapExtention = -500;
	string fileName;
	string mode;
	
	const struct option long_options[] =
	{
		{"mode",required_argument,0,'m'},
		{"input",required_argument,0,'i'},
		{"gapopen",required_argument,0,'o'},
		{"gapext",required_argument,0,'e'},
		{0,0,0,0}
	};
	
	//select = getopt_long( argc, argv, "i:p:q", long_options , &index);
	select = getopt_long( argc, argv, "m:i:o:e", long_options , &index);
	
	while( select!= -1){
		switch(select){
			case 'm':{
				mode=string(optarg);
				cout<< "m:"<<mode<<"\n";
				select = getopt_long( argc, argv, "m:i:o:e", long_options , &index);
				break;
			}
			case 'i':{
				//copy optional argument to the filename
				//strcpy(filename, optarg);
				fileName=string(optarg);
				cout<< "i:"<<fileName<<"\n";
				select = getopt_long( argc, argv, "m:i:o:e", long_options , &index);
				break;
			}
			case 'o':{
				gapOpen = atoi(optarg);
				cout<< "o:"<<gapOpen<<"\n";
				select = getopt_long( argc, argv, "m:i:o:e", long_options , &index);
				break;
			}
			case 'e':{
				gapExtention = atoi(optarg);
				cout<< "e:"<<gapExtention<<"\n";
				select = getopt_long( argc, argv, "m:i:o:e", long_options , &index);
				break;
			}
			default:{
				select=-1;
			}
		}
	}
	
	//Read input file
	string str1;
	string str2;
	int outputScore=0;
	
	string text;
	string str;
	ifstream file;
	file.open(fileName);
	int find=0;
	if (file.is_open()){
		cout<<"is open?:"<<endl;
		while (getline(file,str,'\n')) {
			//cout<<"str[0]"<<str[0]<<endl;
			if(str[0]!='>' && find==1 ){
				str1=str1+str;
			}else if(find==2){
				str2=str2+str;
			}else{
				find++;
			}
		}
	}else{
		cout<<"is close?:"<<endl;
	}
	file.close();
	cout<<"str1:"<<str1<<endl;
	cout<<"str2:"<<str2<<endl;
	if(str1=="" && str2==""){
		//global
		/* str1="TCGACCCAAGTAGGGAAAGAATATCAACACAAAGGCTCGAGAAGAGCCACCCCATGAGCCACCGCATCTACCCCGTGCCCCAGCAAATTAAGAATAG";
		 str2="TCGACCCATGTAGGGAAAGCATATCAATTTCACAAAGGCTCGAGAAGAGCCACATGAGCCACCGCATCTACCCCAGCAAATTAAGAAAAG";
		 
		 str1="AAAC";
		 str2="AGC";
		 
		 
		 str1="ATATTGC";
		 str2="ATAGC";
		 
		 
		 str1="TCGACCCAAGTAGGGAAAGAATATCAACACAAAGGCTCGAGAAGAGCCACCCCATGAGCCACCGCATCTACCCCGTGCCCCAGCAAATTAAGAATAG";
		 str2="TCGACCCATGTAGGGAAAGCATATCAATTTCACAAAGGCTCGAGAAGAGCCACATGAGCCACCGCATCTACCCCAGCAAATTAAGAAAAG";
		 */
		str1="ACGCGA";
		str2="CGAGTGA";
		
		
		//check mismatch match values
		//int score = naviveGlobal(str1, str2, -5, 2, -3,mode);
		/*mode="global";
		 int score =navive( str1, str2, 1, -1, gapOpen, mode);
		 cout<<"score/navive/global:"<<score<<endl;*/
		//int scoreAffine = affine(str1, str2, 1, -1, -2,mode,-2);
		//int scoreAffine = affine(str1, str2, 5, -4, -16,mode,-4);
		//cout<<"scoreAffine:"<<scoreAffine<<endl;
		mode="local";
		gapOpen=-2;
		int score2 = navive( str1, str2, 1, -1, gapOpen, mode);
		cout<<"score/navive/local:"<<score2<<endl;
		
	}else{
		if ( mode=="global") {
			outputScore=navive( str1, str2, 2, -3, gapOpen, mode);
			//output file format
			ofstream myfile;
			myfile.open ("global-naiveGap.aln");
			myfile << "Score = "<<outputScore <<" \n";
			
			
			int len = sequence1.length();
			int count = len / 60;
			for (int j = 0; j <= count; j++) {
				myfile << "my_first_sequence  ";
				int small=0;
				if(60<len){
					small=60;
				}else{
					small=len;
				}
				for (int i = 0; i < small; i++) {
					if(mode=="local"&&gapExtention==-500){
						myfile<<sequence1[i+60*j];
					}else{
						myfile<<sequence1[i+60*j+1];
					}
					
					//myfile<<sequence1[i+60*j+1];
				}
				myfile  <<"\n";
				myfile << "another_sequence   ";
				if(60<len){
					small=60;
				}else{
					small=len;
				}
				for (int i = 0; i < small; i++) {
					if(mode=="local"&&gapExtention==-500){
						myfile<<sequence2[i+60*j];
					}else{
						myfile<<sequence2[i+60*j+1];
					}
					
					//myfile<<sequence2[i+60*j+1];
				}
				len=len-60;
				myfile  <<" \n";
			}
			
			
			myfile.close();
		}
		else if (mode=="local") {
			outputScore=navive( str1, str2, 2, -3, gapOpen, mode);
			//output file format
			ofstream myfile;
			myfile.open ("local-naiveGap.aln");
			myfile << "Score = "<<outputScore <<" \n";
		
			int len = sequence1.length();
			int count = len / 60;
			for (int j = 0; j <= count; j++) {
				myfile << "my_first_sequence  ";
				int small=0;
				if(60<len){
					small=60;
				}else{
					small=len;
				}
				for (int i = 0; i < small; i++) {
					
					if(mode=="local"&&gapExtention==-500){
						myfile<<sequence1[i+60*j];
					}else{
						myfile<<sequence1[i+60*j+1];
					}
					
					//myfile<<sequence1[i+60*j+1];
				}
				myfile  <<"\n";
				myfile << "another_sequence   ";
				if(60<len){
					small=60;
				}else{
					small=len;
				}
				for (int i = 0; i < small; i++) {
					if(mode=="local"&&gapExtention==-500){
						myfile<<sequence2[i+60*j];
					}else{
						myfile<<sequence2[i+60*j+1];
					}
					
					//myfile<<sequence2[i+60*j+1];
				}
				len=len-60;
				myfile  <<" \n";
			}
			
			
			myfile.close();
		}
		else if (mode=="aglobal") {
			outputScore=affine(str1, str2, 2, -3,  gapOpen, mode, gapExtention);
			//output file format
			ofstream myfile;
			myfile.open ("global-affineGap.aln");
			myfile << "Score = "<<outputScore <<" \n";
			
			
			int len = sequence1.length();
			int count = len / 60;
			for (int j = 0; j <= count; j++) {
				myfile << "my_first_sequence  ";
				int small=0;
				if(60<len){
					small=60;
				}else{
					small=len;
				}
				for (int i = 0; i < small; i++) {
					
					if(mode=="local"&&gapExtention==-500){
						myfile<<sequence1[i+60*j];
					}else{
						myfile<<sequence1[i+60*j+1];
					}
					
					//myfile<<sequence1[i+60*j+1];
				}
				myfile  <<"\n";
				myfile << "another_sequence   ";
				if(60<len){
					small=60;
				}else{
					small=len;
				}
				for (int i = 0; i < small; i++) {
					if(mode=="local"&&gapExtention==-500){
						myfile<<sequence2[i+60*j];
					}else{
						myfile<<sequence2[i+60*j+1];
					}
					
					//myfile<<sequence2[i+60*j+1];
				}
				len=len-60;
				myfile  <<" \n";
			}
			
			
			myfile.close();
		}
		else if (mode=="alocal") {
			outputScore=affine(str1, str2, 2, -3,  gapOpen, mode, gapExtention);
			//output file format
			ofstream myfile;
			myfile.open ("local-affineGap.aln");
			myfile << "Score = "<<outputScore <<" \n";
			
			int len = sequence1.length();
			int count = len / 60;
			for (int j = 0; j <= count; j++) {
				myfile << "my_first_sequence  ";
				int small=0;
				if(60<len){
					small=60;
				}else{
					small=len;
				}
				for (int i = 0; i < small; i++) {
					
					if(mode=="local"&&gapExtention==-500){
						myfile<<sequence1[i+60*j];
					}else{
						myfile<<sequence1[i+60*j+1];
					}
					
					//myfile<<sequence1[i+60*j+1];
				}
				myfile  <<"\n";
				myfile << "another_sequence   ";
				if(60<len){
					small=60;
				}else{
					small=len;
				}
				for (int i = 0; i < small; i++) {
					if(mode=="local"&&gapExtention==-500){
						myfile<<sequence2[i+60*j];
					}else{
						myfile<<sequence2[i+60*j+1];
					}
					
					//myfile<<sequence2[i+60*j+1];
				}
				len=len-60;
				myfile  <<" \n";
			}
			
			
			myfile.close();
		}
		else {
			printf("Mode is wrong.\n");
		}
		
		
	}
	
	
}

int affine(string str1, string str2, int matchScore, int mismatchScore, int gapopen, string option,  int gapExtention){
	int row=str1.length()+1;
	rowGlobal=str1.length()+1;
	int column=str2.length()+1;
	columnGlobal=str2.length()+1;
	
	cout<<"row,column:"<<row<<","<<column<<endl;
	int E[row][column]; //gap left
	int F[row][column]; //gap up
	int G[row][column]; // match/mismatch
	int V[row][column];
	int backtrack[row][column]; //diagonal:0 coming left:1 coming up:2
	
	for (int i = 1; i < row; i++) {
		backtrack[i][0] = 2; //coming up
		V[i][0] = gapopen + i*gapExtention;
		E[i][0] = gapopen + i*gapExtention;
		F[i][0] = 0;
		G[i][0] = 0;
		
	}
	for (int j = 1; j < column; j++) {
		backtrack[0][j] = 1;//coming left
		V[0][j] = gapopen + j*gapExtention;
		F[0][j] = gapopen + j*gapExtention;
		E[0][j] = 0;
		G[0][j] = 0;
		
	}
	E[0][0] = -999999;
	F[0][0] = -999999;
	G[0][0] = -999999;
	V[0][0] = 0;
	
	for (int i = 1; i < row; i++) {
		for (int j = 1; j < column; j++) {
			
			//coming left
			if(E[i][j-1]+gapExtention > V[i][j-1]+gapopen+gapExtention ){
				E[i][j]=E[i][j-1]+gapExtention;
			}else{
				E[i][j]=V[i][j-1]+gapopen+gapExtention;
			}
			
			if(E[i][j-1]+gapExtention > V[i][j-1]+gapopen+gapExtention ){
				E[i][j]=E[i][j-1]+gapExtention;
			}else{
				E[i][j]=V[i][j-1]+gapopen+gapExtention;
			}
			
			//coming up
			if(F[i-1][j]+gapExtention > V[i-1][j]+gapopen+gapExtention){
				F[i][j]=F[i-1][j]+gapExtention;
			}else{
				F[i][j]=V[i-1][j]+gapopen+gapExtention;
			}
			
			//diagonal
			if( str1[i-1]==str2[j-1]){
				G[i][j] =V[i-1][j-1] +matchScore;
			}else{
				G[i][j] =V[i-1][j-1] +mismatchScore;
			}
			
			
			if(G[i][j] > E[i][j] && G[i][j] > F[i][j])
			{
				V[i][j]=G[i][j];
				//cout<<"G[i][j] is largest."<<G[i][j]<<endl;
				backtrack[i][j] = 0;
			}
			else if(E[i][j] > F[i][j])
			{
				V[i][j]=E[i][j];
				//cout<<"E[i][j] is largest."<<E[i][j]<<endl;
				backtrack[i][j] = 1;
			}
			else
			{
				V[i][j]=F[i][j];
				//cout<<"F[i][j] is largest."<<F[i][j]<<endl;
				backtrack[i][j] = 2;
			}
			
			
			if(option=="local"){
				if(V[i][j]  < 0){
					V[i][j] = 0;
					backtrack[i][j] = 0;
				}
			}
			
			
		}
	}
	
	/*
	 cout<<"E[i][j]:"<<endl;
	 for(int i = 0; i<row; i++)
	 {
	 for(int j = 0; j<column; j++)
	 {
	 cout<<E[i][j]<<"-"<<" ";
	 //cout<<scoreTable[i][j]<<" ";
	 }
	 cout<<endl;
	 }
	 
	 
	 //print score table
	 cout<<"V[i][j]:"<<endl;
	 for(int i = 0; i<row; i++)
	 {
	 for(int j = 0; j<column; j++)
	 {
	 cout<<V[i][j]<<"-"<<" ";
	 //cout<<scoreTable[i][j]<<" ";
	 }
	 cout<<endl;
	 }
	 */
	//backtrack
	//backtrace and print the alignment with best score
	//global da son row ve columndan başlanır local da ise biggest element bul
	int rowTraverse=row-1;
	int columnTraverse=column-1;
	
	if(option=="local"){
		int biggest =V[rowTraverse][columnTraverse];
		//cout<<"biggest:"<<biggest<<"6,3:"<<V[6][3] <<endl;
		for(int i = row-1 ; i >= 0 ; i--){
			for(int j = column-1 ; j >= 0 ; j--){
				if(V[i][j]>biggest){
					biggest=V[i][j];
					rowTraverse=i;
					columnTraverse=j;
				}
			}
		}
	}
	
	//her türlü scoreları scoreTable[rowTraverse][columnTraverse] print score
	if(option=="local"){
		cout<<"local score i:"<<rowTraverse<<endl;
		cout<<"local score j:"<<columnTraverse<<endl;
		cout<<"local score:"<<V[rowTraverse][columnTraverse]<<endl;
	}else{
		cout<<"global score:"<<V[rowTraverse][columnTraverse]<<endl;
	}
	
	int i=rowTraverse;
	int j=columnTraverse;
	sequence1="";
	sequence2="";
	
	while(i>=0 || j>=0){
		int traverse=backtrack[i][j];
		//cout<<"backtrack[2][3];"<<backtrack[2][3]<<endl;
		if(traverse==0){ //diagonal
			//cout<<"gir3"<<endl;
			sequence1= str1[i-1]+sequence1;
			sequence2= str2[j-1]+sequence2;
			i--;
			j--;
		}else if(traverse==1){ //right
			//cout<<"gir1"<<endl;
			sequence1= '-'+sequence1;
			sequence2= str2[j-1]+sequence2;
			j--;
		}else if(traverse==2){ //down
			//cout<<"gir2"<<endl;
			sequence1= str1[i-1]+sequence1;
			sequence2= '-'+sequence2;
			i--;
		}
		
		//If we reach 0, stop
		if(option=="local"){
			if(V[i][j]==0){
				//çıkıyor mu??
				break;
			}
		}
		
	}
	//print sequence
	cout<<"sequence1:"<<sequence1<<endl;
	cout<<"sequence2:"<<sequence2<<endl;
	
	return V[rowTraverse][columnTraverse];
}



//returns score
int navive(string str1, string str2, int matchScore, int mismatchScore, int gapopen, string option){
	
	int row=str1.length()+1;
	int column=str2.length()+1;
	rowGlobal=str1.length()+1;
	columnGlobal=str2.length()+1;
	
	cout<<"row,column:"<<row<<","<<column<<endl;
	int scoreTable[row][column];
	
	//for trackbacking purposes
	int backtrack[row][column];
	
	//??
	backtrack[0][0]=0; //only for this cell otherwise diagonal:0 coming left:1 coming up:2
	scoreTable[0][0] = 0;
	
	//initalize 1st row and column according to gap penalties
	/*for (int j = 1; j < column; j++) {
	 if(option=="local"){
	 scoreTable[0][j] = 0;
	 backtrack[j][0] = 1; //coming fromleftup
	 }else{
	 scoreTable[0][j] = gapopen * j;
	 backtrack[j][0] = 1; //coming fromleftup
	 }
	 }*/
	for (int j = 1; j < column; j++) {
		if(option=="local"){
			scoreTable[0][j] = 0;
			backtrack[0][j] = 1; //coming fromleftup
		}else{
			scoreTable[0][j] = gapopen * j;
			backtrack[0][j] = 1; //coming fromleftup
		}
	}
	scoreTable[0][0] = 0;
	for (int i = 1; i < row; i++) {
		if(option=="local"){
			scoreTable[i][0] =0;
			backtrack[i][0] = 2; //coming from up
		}else{
			scoreTable[i][0] = gapopen * i;
			backtrack[i][0] = 2; //coming from up
		}
		
	}
	
	//create table
	for (int i = 1; i < row; i++) {
		for (int j = 1; j < column; j++) {
			
			
			//cout<<"J:"<<j<<endl;
			//you have 2 option(match/mismatch) as a diagonal
			int diagonal=0;
			if(str1[i-1] == str2[j-1]){
				diagonal= scoreTable[i-1][j-1] + matchScore;
			}else{
				diagonal= scoreTable[i-1][j-1] + mismatchScore;
			}
			
			//right
			int right=0;
			//right=scoreTable[i-1][j]+gapopen;
			right=scoreTable[i][j-1]+gapopen;
			
			//down
			int down=0;
			//down=scoreTable[i][j-1]+gapopen;
			down=scoreTable[i-1][j]+gapopen;
			
			//compare and decide
			if ( diagonal > right && diagonal > down ){
				//cout<<"diagonal is largest:"<<diagonal<<endl;
				scoreTable[i][j]=diagonal;
				backtrack[i][j] = 0;
			}
			else if ( down >= diagonal && down > right ){
				//else if ( down > right ){
				//cout<<"down is largest:"<<down<<endl;
				scoreTable[i][j]=down;
				backtrack[i][j] = 2;
			}
			
			else
				if ( right >= diagonal && right >=  down )
				{
					//cout<<"right is largest:"<<right<<endl;
					scoreTable[i][j]=right;
					backtrack[i][j] = 1;
				}
			
			
			
			
			//In local alignment smallest must be 0
			if(option=="local"){
				if(scoreTable[i][j] < 0){
					scoreTable[i][j] = 0;
					backtrack[i][j] = 0;
				}
			}
			
		}
	}
	
	//backtrace and print the alignment with best score
	//global da son row ve columndan başlanır local da ise biggest element bul
	int rowTraverse=row-1;
	int columnTraverse=column-1;
	
	if(option=="local"){
		int biggest =scoreTable[rowTraverse][columnTraverse];
		//cout<<"biggest:"<<biggest<<"6,3:"<<scoreTable[6][3] <<endl;
		for(int i = row-1 ; i >= 0 ; i--){
			for(int j = column-1 ; j >= 0 ; j--){
				if(scoreTable[i][j]>biggest){
					biggest=scoreTable[i][j];
					rowTraverse=i;
					columnTraverse=j;
				}
			}
		}
	}
	
	//print score table
	/*
	for(int i = 0; i<row; i++)
	{
		for(int j = 0; j<column; j++)
		{
			cout<<scoreTable[i][j]<<"-"<<backtrack[i][j]<<" ";
			//cout<<scoreTable[i][j]<<" ";
		}
		cout<<endl;
	}
	*/
	
	//her türlü scoreları scoreTable[rowTraverse][columnTraverse] print score
	if(option=="local"){
		cout<<"local score i:"<<rowTraverse<<endl;
		cout<<"local score j:"<<columnTraverse<<endl;
		cout<<"local score:"<<scoreTable[rowTraverse][columnTraverse]<<endl;
	}else{
		cout<<"global score:"<<scoreTable[rowTraverse][columnTraverse]<<endl;
	}
	
	int i=rowTraverse;
	int j=columnTraverse;
	sequence1="";
	sequence2="";
	
	while(i>=0 || j>=0){
		int traverse=backtrack[i][j];
		//cout<<"backtrack[2][3];"<<backtrack[2][3]<<endl;
		if(traverse==0){ //diagonal
			//cout<<"gir3"<<endl;
			sequence1= str1[i-1]+sequence1;
			sequence2= str2[j-1]+sequence2;
			i--;
			j--;
		}else if(traverse==1){ //right
			//cout<<"gir1"<<endl;
			sequence1= '-'+sequence1;
			sequence2= str2[j-1]+sequence2;
			j--;
		}else if(traverse==2){ //down
			//cout<<"gir2"<<endl;
			sequence1= str1[i-1]+sequence1;
			sequence2= '-'+sequence2;
			i--;
		}
		
		//If we reach 0, stop
		if(option=="local"){
			if(scoreTable[i][j]==0){
				//çıkıyor mu??
				break;
			}
		}
		
	}
	//print sequence on console
	cout<<"sequence1:"<<sequence1<<endl;
	cout<<"sequence2:"<<sequence2<<endl;
	
	
	return scoreTable[rowTraverse][columnTraverse];
}






