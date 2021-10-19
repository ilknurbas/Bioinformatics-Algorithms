//
//  main.cpp
//  getOpt
//
//  Created by Ilknur Bas on 14.10.2020.
//  Copyright © 2020 Ilknur Bas. All rights reserved.
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
 /* for getopt_long; POSIX standard getopt is in unistd.h */

int bruteForceSearch(string text,string pattern,int lengthText,int lengthPattern);
int rabinKarpAlgorithm(string text,string pattern,int lengthText,int lengthPattern);
int knuth_morris_pratt(string text,string pattern,int lengthText,int lengthPattern);
void failureFunction(string pattern,int lengthPattern,int *&F);
int findPrime(int q) ;
bool isPrime(int q);
std::chrono::duration< double, std::micro > minimum( std::chrono::duration< double, std::micro > a, std::chrono::duration< double, std::micro > b, std::chrono::duration< double, std::micro > c );
std::chrono::duration< double, std::micro > minimum2( std::chrono::duration< double, std::micro > a, std::chrono::duration< double, std::micro > b );

//long option 2 dashes
int main(int argc,  char * argv[]) { //int main(int argc, const char * argv[]) {
	
	/*enum modes {
		NONE,
		INDEX,
		SEARCH
	};*/
	
	cout << "Hello, World!\n";
	//enum modes mode;
	//mode=NONE;
	int option=0;
	int index;
	
	const struct option long_options[] =
	{
		{"input",required_argument,0,'i'},
		{"pattern",required_argument,0,'p'},
		{"query",required_argument,0,'q'},
		{0,0,0,0}
	};
	
	
	string textFile;
	string patternFile;
	
	//output will be store in integer option
	option = getopt_long( argc, argv, "i:p:q", long_options , &index);
	
	//while(( option=getopt_long( argc, argv, "i:s:q", long_options , &index))!= -1){
	while( option!= -1){
		switch(option){
			case 'i':{
				//copy optional argument to the filename
				//strcpy(filename, optarg);
				textFile=string(optarg);
				cout<< "1."<<textFile<<"\n";
				//cout<< "1.stringI."<<strI<<"\n";
				option=getopt_long( argc, argv, "i:p:q", long_options , &index);
				break;
			}
			case 'p':{
				//mode=SEARCH;
				//copy optional argument to the filename
				//strcpy(filename, optarg);
				patternFile=string(optarg);
				//std::string strS(filename);
				cout<< "2."<<patternFile<<"\n";
				//cout<< "2.stringS."<<strS<<"\n";
				option=getopt_long( argc, argv, "i:p:q", long_options , &index);
				break;
			}
	
			default:{
				option=-1;
			}
			
		}
	}
	
	cout<< "OPTION -1:"<<option<<"\n";
	

	//burada text file oku
	string text;
	string str;
	ifstream file;
	file.open(textFile);
	if (file.is_open()){
		while (getline(file,str,'\n' )) {
			//cout<<"str[0]"<<str[0]<<endl;
			if(str[0]!='>'){
				text=text+str;
			}
		}
	}
	file.close();
	
	//burada pattern file oku
	string pattern;
	file.open(patternFile);
	if (file.is_open()){
		while (getline(file,str,'\n' )) {
			if(str[0]!='>'){
				pattern=pattern+str;
			}
		}
	}
	file.close();
	
	if (text==""&&pattern==""){
		  text="TAAGTCTATACCATCGTAGTCTAATTAACGTTATGGTAGGATATCAAGGACGGAATGACCGCAGAGGCGACGTTAATGCGCCGTCAGAGACGCCCTAAAGATTGCGGTAGGGTCCCGTTGTTAAAGAGACTTGAGTGGGTGCTTGATGGGAGTGTATTAAGGGCATGTATAAGTGTTGCTGGGTCTAAGGCATTAAAGCTGAGTCAATAGTTACATTGCAGATTAACGAGATCTGAAATTAAGGGAGAGATTCCCAGAGTGGCCTAGTACTTAAGGGCACCCACGCCGCAGGCGGCCCTACGCCCGTTAATGGTTCGAGTGCTATTCACTAACACATTAACGGACGTTTAGTGTGGATTATAGGTGAAGGGTCTGCGCCACTCCAAGGCAGGGAACATATGTGTTGTTACTATCTTAACG";
		
		  pattern="TGGGTCTAAGGCATTAAAGCTGAGTCAATAGT";
		
		
	}
	
	cout<<"Text is:"<<text<<endl;
	cout<<"Pattern is:"<<pattern<<endl;
	
	int lengthText=text.length();
	int lengthPattern=pattern.length();
	std::chrono::time_point< std::chrono::system_clock > startTime;
	std::chrono::duration< double, std::micro >elapsedTimeBrute,elapsedTimeRabin,elapsedTimeKnuth;
	
	cout<<"********Program starts*******************"<<endl;
	cout<<"Length of Text: "<<lengthText<<endl;
	cout<<"Length of Pattern: "<<lengthPattern<<endl;
	
	cout<<"--------Brute Force Search---------------"<<endl;

	startTime = std::chrono::system_clock::now();
	int brute = bruteForceSearch(text,pattern,lengthText,lengthPattern);
	elapsedTimeBrute = std::chrono::system_clock::now() - startTime;
	std::cout << "Run time was:"<<elapsedTimeBrute.count()  <<endl;
	cout<<"Performed "<<brute<<" comparisons."<<endl;
	
	cout<<"--------Rabin Karp Algorithm-------------"<<endl;
	startTime = std::chrono::system_clock::now();
	int rabin = rabinKarpAlgorithm(text,pattern,lengthText,lengthPattern);
	elapsedTimeRabin = std::chrono::system_clock::now() - startTime;
	std::cout << "Run time was:"<<elapsedTimeRabin.count()  <<endl;
	cout<<"Performed "<<rabin<<" comparisons."<<endl;
	
	cout<<"--------Knuth-Morris-Pratt---------------"<<endl;
	startTime = std::chrono::system_clock::now();
	int knuth =knuth_morris_pratt(text,pattern,lengthText,lengthPattern);
	elapsedTimeKnuth = std::chrono::system_clock::now() - startTime;
	std::cout << "Run time was:"<<elapsedTimeKnuth.count()  <<endl;
	cout<<"Performed "<<knuth<<" comparisons."<<endl;
	
	cout<<"--------Best Algorithm-------------------(smaller run time)"<<endl;
	std::chrono::duration< double, std::micro > bestAlgo=minimum(elapsedTimeBrute, elapsedTimeRabin, elapsedTimeKnuth );
	if (bestAlgo==elapsedTimeBrute){
		cout<<"Brute Force Search"<<endl;
	}else if(bestAlgo==elapsedTimeRabin){
		cout<<"Rabin Karp Algorithm"<<endl;
	}else if(bestAlgo==elapsedTimeKnuth){
		cout<<"Knuth-Morris-Pratt"<<endl;
	}
	
	return 0;
}
std::chrono::duration< double, std::micro > minimum( std::chrono::duration< double, std::micro > a, std::chrono::duration< double, std::micro > b, std::chrono::duration< double, std::micro > c ){
	return minimum2( minimum2(a,b), c ) ;
}
std::chrono::duration< double, std::micro > minimum2( std::chrono::duration< double, std::micro > a, std::chrono::duration< double, std::micro > b ){
	if (a<b){
		return a;
	}
	return b;
}

int bruteForceSearch(string text, string pattern,int lengthText,int lengthPattern){
	
	/*Whether P is in T, and if it is, the location of P within T (1-based coordinate)
	 Number of character comparisons performed.
	 Run time in microseconds.*/
	
	int i =0;
	bool check=false;
	int comparison=0;
	
	while(i<lengthText){
		int checkText=i;
		int j=0;
		for (j=0;j<lengthPattern;j++) {
			comparison++;
			if (pattern[j]==text[checkText]){
				checkText++;
			}else{
				break;
			}
		}
		if (j==lengthPattern) {
			cout<<"Pattern is found, starting index of position is: "<<i+1<<endl;//position
			i=1+lengthText; //in order to exit the while loop
			check=true;
		}else{
			i++;
		}
	}
	
	if (!check){
		cout<<"Pattern is not found."<<endl;
	}
	return comparison;
	
}

bool isPrime(int q) {
	int stop = sqrt(q);
	for(int i = 2; i <= stop; i++){
		if (q % i == 0){
			return false;
		}
	}
	return true;
}

int findPrime(int q) {
	do {
		q++;
	} while (!isPrime(q));
	return q;
}


int rabinKarpAlgorithm(string text,string pattern,int lengthText,int lengthPattern){
	/*Whether P is in T, and if it is, the location of P within T (1-based coordinate)
	 Number of character comparisons performed.
	 Run time in microseconds.*/
	
	int q = findPrime(lengthPattern);
	//int q =997;
	cout<<"q:"<<q<<endl;
	
	//long x= pow(4, (lengthPattern-1));
	//long c=x%q;

	//pow(4, (lengthPattern-1)) %q
	int c=1;
	for (int i = 0; i < lengthPattern - 1; i++) {
		c = (c * 4) % q;
	}
	
	int fingerprintOfPattern = 0;
	int fingerprintOfText = 0;
	int comparison=0;
	
	//preprocessing
	//encode A:0 C:1 G:2 T:3
	int encodePattern;
	int encodeText;
	for (int i = 0; i < lengthPattern; i++) {
		if (pattern[i]=='A') {
			encodePattern=0;
		}else if (pattern[i]=='C') {
			encodePattern=1;
		}else if (pattern[i]=='G') {
			encodePattern=2;
		}else{
			encodePattern=3;
		}
		if ( text[i]=='A') {
			encodeText=0;
		}else if ( text[i]=='C') {
			encodeText=1;
		}else if (text[i]=='G') {
			encodeText=2;
		}else{
			encodeText=3;
		}
		fingerprintOfPattern = (4 * fingerprintOfPattern + encodePattern) % q;
		fingerprintOfText = (4 * fingerprintOfText + encodeText) % q;
		
	}
	
 	bool artik=false;
	int encodeS;
	int encodeSplusM;
	for(int s = 0; s < lengthText-lengthPattern+1; s++) {
		if (text[s]=='A' ) {
			encodeS=0;
		}else if (text[s]=='C' ) {
			encodeS=1;
		}else if (text[s]=='G') {
			encodeS=2;
		}else{
			encodeS=3;
		}
		if ( text[s+lengthPattern]=='A') {
			encodeSplusM=0;
		}else if (  text[s+lengthPattern]=='C') {
			encodeSplusM=1;
		}else if (text[s+lengthPattern]=='G') {
			encodeSplusM=2;
		}else{
			encodeSplusM=3;
		}

		if (fingerprintOfText == fingerprintOfPattern) {
			bool check=true;
			for (int z = 0; z<lengthPattern; z++) {
				comparison++;
				if (pattern[z]==text[s+z]) {
				}else{
					check=false;
					break;
				}
			}
			if (check){
				cout<<"Pattern is found, starting position of pattern is: "<<s+1<<endl;//position
				artik=true;
			}
		}
		//else
		//long x= pow(4, (lengthPattern-1));
		//long c=x%q;
		
		fingerprintOfText = (4*(fingerprintOfText-encodeS*c) + encodeSplusM) % q;
		if (fingerprintOfText < 0) {
			fingerprintOfText = fingerprintOfText + q;
		}
		if(artik)
			return comparison;

	}
	/*if (!check){
		cout<<"Pattern is not found."<<endl;
	}*/
	
	return comparison;
}

int knuth_morris_pratt(string text,string pattern,int lengthText,int lengthPattern){
	
	int *F=new int[lengthPattern];
	failureFunction(pattern,lengthPattern,F);
	/*
	for(int i=0;i<lengthPattern;i++){
		cout<<"F["<<i<<"]: "<<F[i]<<" ";
	}
	cout<<endl;*/

	int i=0;
	int j=0;
	int check=-1;
	int comparison=0;
	
	while(i<lengthText){
		comparison=comparison+1;
		if (text[i]==pattern[j]) {
			if(j==lengthPattern-1){
				cout<<"Pattern is found, starting position of pattern is: "<<i-j+1<<endl; //position
				check=i-j;
				i=lengthText; //in order to exit the for loop
				//return;//??
			}else{
				i=i+1;
				j=j+1;
			}
			
		}else{
			if(j>0){
				j=F[j-1];
				//comparison=comparison+j;
				
			}else{
				i=i+1;
				j=0;
			}
			/*cout<<"i: "<<i<<endl;
			 cout<<"j: "<<j<<endl;*/
	 	}
		
	}
	
	if (check==-1){
		cout<<"Pattern is not found."<<endl;
	}
	return comparison;

}

void failureFunction(string pattern,int lengthPattern, int *&F){
	F[0]=0;
	int i=1;
	int j=0;
	
	while(i<lengthPattern){
		/*cout<<"i:"<<i<<endl;
		 cout<<"j:"<<j<<endl;
		 cout<<"pattern[i]:"<<pattern[i]<<endl;
		 cout<<"F[i]:"<<F[i]<<endl;*/
		if(pattern[i]==pattern[j]){
			F[i]=j+1;
			i=i+1;
			j=j+1;
		}else if(j>0){
			j=F[j-1];
		}else{
			F[i]=0;
			i=i+1;
		}
	}
	
	
}











//
//  main.cpp
//  hw3
//
//  Created by Ilknur Bas on 16.11.2020.
//  Copyright © 2020 Ilknur Bas. All rights reserved.
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

int rowGlobal=0;
int columnGlobal=0;
int naviveGlobal(string str1, string str2, int matchScore, int mismatchScore, int gapopen,string option);
int affine(string str1, string str2, int matchScore, int mismatchScore, int gapopen, string option,  int gapExtention);



int main(int argc,  char * argv[]) {
	int option=0;
	const struct option long_options[] =
	{
		{"mode",required_argument,0,'m'},
		{"input",required_argument,0,'i'},
		{"gapopen",required_argument,0,'o'},
		{"gapext",required_argument,0,'e'},
		{0,0,0,0}
	};
	
	option = getopt_long( argc, argv, "m:i:o:e", long_options , &index);
	
	//while(( option=getopt_long( argc, argv, "i:s:q", long_options , &index))!= -1){
	while( option!= -1){
		switch(option){
			case 'm':{
				
				textFile=string(optarg);
				cout<< "1."<<textFile<<"\n";
				//cout<< "1.stringI."<<strI<<"\n";
				option=getopt_long( argc, argv, "i:p:q", long_options , &index);
				break;
			}
			case 'p':{
				//mode=SEARCH;
				//copy optional argument to the filename
				//strcpy(filename, optarg);
				patternFile=string(optarg);
				//std::string strS(filename);
				cout<< "2."<<patternFile<<"\n";
				//cout<< "2.stringS."<<strS<<"\n";
				option=getopt_long( argc, argv, "i:p:q", long_options , &index);
				break;
			}
				
			default:{
				option=-1;
			}
				
		}
		
		
		// insert code here...
		std::cout << "Hello, World!\n";
		
		//global
		/*string str1="TCGACCCAAGTAGGGAAAGAATATCAACACAAAGGCTCGAGAAGAGCCACCCCATGAGCCACCGCATCTACCCCGTGCCCCAGCAAATTAAGAATAG";
		 string str2="TCGACCCATGTAGGGAAAGCATATCAATTTCACAAAGGCTCGAGAAGAGCCACATGAGCCACCGCATCTACCCCAGCAAATTAAGAAAAG";
		 
		 string str1="AAAC";
		 string str2="AGC";
		 
		 string str1="ACGCGA";
		 string str2="CGAGTGA";
		 
		 */
		
		
		string str1="ATATTGC";
		string str2="ATAGC";
		
		
		string option="global";
		//string option="local";
		//check mismatch match values
		//int score = naviveGlobal(str1, str2, -5, 2, -3,option);
		int score = naviveGlobal(str1, str2, 1, -1, -2,option);
		cout<<"score:"<<score<<endl;
		//int scoreAffine = affine(str1, str2, 1, -1, -2,option,-2);
		int scoreAffine = affine(str1, str2, 5, -4, -16,option,-4);
		cout<<"scoreAffine:"<<scoreAffine<<endl;
		return 0;
	}
	
	int affine(string str1, string str2, int matchScore, int mismatchScore, int gapopen, string option,  int gapExtention){
		int row=str1.length()+1;
		rowGlobal=str1.length()+1;
		int column=str2.length()+1;
		columnGlobal==str2.length()+1;
		
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
		E[0][0] = INT_MIN;
		F[0][0] = INT_MIN;
		G[0][0] = INT_MIN;
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
					cout<<"G[i][j] is largest."<<G[i][j]<<endl;
					backtrack[i][j] = 0;
				}
				else if(E[i][j] > F[i][j])
				{
					V[i][j]=E[i][j];
					cout<<"E[i][j] is largest."<<E[i][j]<<endl;
					backtrack[i][j] = 1;
				}
				else
				{
					V[i][j]=F[i][j];
					cout<<"F[i][j] is largest."<<F[i][j]<<endl;
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
		
		//backtrack
		//backtrace and print the alignment with best score
		//global da son row ve columndan başlanır local da ise biggest element bul
		int rowTraverse=row-1;
		int columnTraverse=column-1;
		
		if(option=="local"){
			int biggest =V[rowTraverse][columnTraverse];
			cout<<"biggest:"<<biggest<<"6,3:"<<V[6][3] <<endl;
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
		string sequence1="";
		string sequence2="";
		
		while(i>=0 || j>=0){
			int traverse=backtrack[i][j];
			cout<<"backtrack[2][3];"<<backtrack[2][3]<<endl;
			if(traverse==0){ //diagonal
				cout<<"gir3"<<endl;
				
				sequence1= str1[i-1]+sequence1;
				sequence2= str2[j-1]+sequence2;
				i--;
				j--;
			}else if(traverse==1){ //right
				cout<<"gir1"<<endl;
				sequence1= '-'+sequence1;
				sequence2= str2[j-1]+sequence2;
				j--;
			}else if(traverse==2){ //down
				cout<<"gir2"<<endl;
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
		
		return 1;
	}
	
	
	
	//returns score
	int naviveGlobal(string str1, string str2, int matchScore, int mismatchScore, int gapopen, string option){
		
		int row=str1.length()+1;
		int column=str2.length()+1;
		rowGlobal=str1.length()+1;
		columnGlobal==str2.length()+1;
		
		cout<<"row,column:"<<row<<","<<column<<endl;
		int scoreTable[row][column];
		
		//for trackbacking purposes
		int backtrack[row][column];
		
		//??
		backtrack[0][0]=0; //only for this cell otherwise diagonal:0 coming left:1 coming up:2
		scoreTable[0][0] = 0;
		
		//initalize 1st row and column according to gap penalties
		for (int j = 1; j < column; j++) {
			if(option=="local"){
				scoreTable[0][j] = 0;
				backtrack[j][0] = 1; //coming fromleftup
			}else{
				scoreTable[0][j] = gapopen * j;
				backtrack[j][0] = 1; //coming fromleftup
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
				cout<<"J:"<<j<<endl;
				//you have 2 option(match/mismatch) as a diagonal
				int diagonal=0;
				if(str1[i-1] == str2[j-1]){
					diagonal= scoreTable[i-1][j-1] + matchScore;
				}else{
					diagonal= scoreTable[i-1][j-1] + mismatchScore;
				}
				
				//right
				int right=0;
				right=scoreTable[i-1][j]+gapopen;
				
				//down
				int down=0;
				down=scoreTable[i][j-1]+gapopen;
				
				//compare and decide
				if ( diagonal > right && diagonal > down ){
					cout<<"diagonal is largest:"<<diagonal<<endl;
					scoreTable[i][j]=diagonal;
					backtrack[i][j] = 0;
				}
				else if ( down >= diagonal && down > right ){
					cout<<"down is largest:"<<down<<endl;
					scoreTable[i][j]=down;
					backtrack[i][j] = 2;
				}
				
				else if ( right >=  diagonal && right >=   down )
				{
					cout<<"right is largest:"<<right<<endl;
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
			cout<<"biggest:"<<biggest<<"6,3:"<<scoreTable[6][3] <<endl;
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
		for(int i = 0; i<row; i++)
		{
			for(int j = 0; j<column; j++)
			{
				cout<<scoreTable[i][j]<<"-"<<backtrack[i][j]<<" ";
				//cout<<scoreTable[i][j]<<" ";
			}
			cout<<endl;
		}
		
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
		string sequence1="";
		string sequence2="";
		
		while(i>=0 || j>=0){
			int traverse=backtrack[i][j];
			cout<<"backtrack[2][3];"<<backtrack[2][3]<<endl;
			if(traverse==0){ //diagonal
				cout<<"gir3"<<endl;
				
				sequence1= str1[i-1]+sequence1;
				sequence2= str2[j-1]+sequence2;
				i--;
				j--;
			}else if(traverse==1){ //right
				cout<<"gir1"<<endl;
				sequence1= '-'+sequence1;
				sequence2= str2[j-1]+sequence2;
				j--;
			}else if(traverse==2){ //down
				cout<<"gir2"<<endl;
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
		//print sequence
		cout<<"sequence1:"<<sequence1<<endl;
		cout<<"sequence2:"<<sequence2<<endl;
		
		
		
		
		return 1;
	}
	
	
	
	
	
	
	
	
	
	
	
	
	








