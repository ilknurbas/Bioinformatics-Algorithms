//
//  main.cpp
//  hw6
//
//  Created by Ilknur Bas on 18.12.2020.
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

int rowGlobal=0;
int columnGlobal=0;
string sequence1;
string sequence2;
int rowForDistance,columnForDistance;
int row;
int column;

int counterForSequenceNo;
char sequences [25][500];
char nameOfSequence[25][50]; //50 does matter? no

int affineAndEditDistance(int str1, int str2, int matchScore, int mismatchScore, int gapopen, string option,  int gapExtention);
void readFasta(string fastaName);
double minimumDistance (double **matrixEditDistance,int &row, int &column,int nodeSize);
void outFile(string outFileName, string result );
	
void readFasta(string fastaName){
	
	ifstream file;
	file.open(fastaName);
	string str;
	counterForSequenceNo=0;
	
	if (file.is_open()){
		cout<<"Opened."<<endl;
		while (getline(file,str,'\n')) {

			if(str[0]=='>'){ //get the name of each sequence
				char name_1[str.length()-1];
				int counter=0;
				for (int i=1;i<str.length();i++){
					name_1[counter]=str[i];
					counter++;
				}
				for (int i=0;i<counter;i++){
					nameOfSequence[counterForSequenceNo][i]=name_1[i];
				}
				counterForSequenceNo++;
				
			}else{ //get the each sequence
				int i=0;
				while(i!=str.length()){
					char alphabet=str[i];
					sequences[counterForSequenceNo-1][i]=alphabet;
					i++;
				}
			}
		}
		
	}else{
		cout<<"Closed."<<endl;
	}
	file.close();
	
	cout<<"Sequence No:"<<counterForSequenceNo<<endl;
	
	bool check=true;
	for (int i=0;i<counterForSequenceNo;i++){
		for(int j=0;j<500;j++){
			if (check){
				cout<<"Sequence "<<i<<" in readFasta function: ";
				check=false;
			}
			if(sequences[i][j]!='0'){
				cout<<sequences[i][j];
			}
		}
		cout<<endl;
		check=true;
	}
	
	check=true;
	for (int i=0;i<counterForSequenceNo;i++){
		for(int j=0;j<50;j++){
			if (check){
				cout<<"Name of sequence "<<i<<" in readFasta function: ";
				check=false;
			}
			if(nameOfSequence[i][j]!='0'){
				cout<<nameOfSequence[i][j];
			}
	 	}
		cout<<endl;
		check=true;
	}

}

struct node
{
	double height;
	string name;
	int numOfGenes;
};

int main(int argc, char * argv[]) {
	std::cout << "Hello, World!\n";
	
	int select=0;
	int index;
	string fastaName="";
	int match=0;
	int mismatch=0;
	int gapopen=0;
	int gapext=0;
	string outFileName="";
	
	const struct option long_options[] =
	{
		{"fasta",required_argument,0,'f'},
		{"match",required_argument,0,'m'},
		{"mismatch",required_argument,0,'s'},
		{"gapopen",required_argument,0,'o'},
		{"gapext",required_argument,0,'e'},
		{"out",required_argument,0,'h'},
		{0,0,0,0}
	};
	select = getopt_long( argc, argv, "f:m:s:o:e:h", long_options , &index);
	
	while( select!= -1){
		switch(select){
			case 'f':{
				fastaName=string(optarg);
				cout<< "f:"<<fastaName<<"\n";
				select = getopt_long( argc, argv, "f:m:s:o:e:h", long_options , &index);
				break;
			}
			case 'm':{
				match = atoi(optarg);
				cout<< "m:"<<match<<"\n";
				select = getopt_long( argc, argv, "f:m:s:o:e:h", long_options , &index);
				break;
			}
			case 's':{
				mismatch = atoi(optarg);
				cout<< "s:"<<mismatch<<"\n";
				select = getopt_long( argc, argv, "f:m:s:o:e:h", long_options , &index);
				break;
			}
			case 'o':{
				gapopen = atoi(optarg);
				cout<< "o:"<<gapopen<<"\n";
				select = getopt_long( argc, argv, "f:m:s:o:e:h", long_options , &index);
				break;
			}
			case 'e':{
				gapext = atoi(optarg);
				cout<< "e:"<< gapext<<"\n";
				select = getopt_long( argc, argv, "f:m:s:o:e:h", long_options , &index);
				break;
			}
			case 'h':{
				outFileName=string(optarg);
				cout<< "h:"<<outFileName<<"\n";
				select = getopt_long( argc, argv, "f:m:s:o:e:h", long_options , &index);
				break;
			}
			default:{
				select=-1;
			}
		}
	}
	//dont forgetttttttttt
	//fastaName ="seq.fasta";
	
	// Initialization
	for (int i=0;i<25;i++){
		for(int j=0;j<500;j++){
			sequences[i][j]='0';
		}
	}
	for (int i=0;i<25;i++){
		for(int j=0;j<50;j++){
			nameOfSequence[i][j]='0';
		}
	}
	
	//Read fasta file
	readFasta(fastaName);
	cout << "-------------------------------------\n";
	
	double ** matrixEditDistance= new double *[counterForSequenceNo];
	for( int i = 0; i < counterForSequenceNo; i++){
		matrixEditDistance[i]= new double [counterForSequenceNo];
	}
	for (int i = 0; i < counterForSequenceNo; i++){
		for (int j = 0; j < i; j++){
			matrixEditDistance[i][j]=affineAndEditDistance(i,j, match, mismatch, gapopen, "global", gapext);
			//matrixEditDistance[i][j]=affineAndEditDistance(i,j, 5, -3, -8, "global", -1);
		}
	}
	cout << "-------------------------------------\n";
	cout << "Edit matrix:"<<endl;
	for (int i = 0; i < counterForSequenceNo; i++) {
		for (int j = 0; j < i; j++) {
			cout << matrixEditDistance[i][j] << " ";
		}
		cout<<endl;
	}
	
	//find min in matrix
/*	double minDistance= minimumDistance (matrixEditDistance,row,column,counterForSequenceNo);
	cout <<"MinimumDistance:" << minDistance<<endl;
	cout <<"row:" << row<<endl;
	cout <<"column:" << column<<endl;
	*/
	
	//NAMES
	string names[counterForSequenceNo];
	string eachName="";
	for (int i = 0; i < counterForSequenceNo; i++) {
		for(int j=0;j<50;j++){
			if(nameOfSequence[i][j]!='0'){
				eachName=eachName+nameOfSequence[i][j];
			}
		}
		names[i]=eachName;
		eachName="";
	}
	cout<<"-------------------------------------"<<endl;
	cout <<"Names array:" <<endl;
	for (int i = 0; i < counterForSequenceNo; i++) {
		cout<< names[i]<<" ";
	}
	cout<<endl;
	
	//upgma
	
	//Create all nodes and to the node tree
	node allNodes [25]; //At most 25
	for(int i=0;i<counterForSequenceNo;i++){
		node newNode;
		newNode.height = 0;
		newNode.name=names[i];
		newNode.numOfGenes = 1;
		allNodes[i]=newNode;
	}

	string netwickArray[counterForSequenceNo];
	for(int i=0;i<counterForSequenceNo;i++){
		netwickArray[i]=names[i];
	}
	cout<<"NetwickArray:"<<endl;
	for(int i=0;i<counterForSequenceNo;i++){
		cout<<netwickArray[i]<<" ";
	}
	cout<<endl;
	cout<<"-------------------------------------"<<endl;
	
	int nodeSize=counterForSequenceNo; //Değiştirdirr
	double heightOfMerged[counterForSequenceNo];  //Değiştirdirr
	for(int i=0;i<counterForSequenceNo;i++){
		heightOfMerged[i]=0;
	}
	
	
	for ( int a=nodeSize; a>1;a--){
		
		nodeSize=a;
		cout<< 	"Edit Matrix inside for loop when node size:"<<nodeSize<<endl;
		for (int i = 0; i <nodeSize; i++) {
			for (int j = 0; j<i; j++) {
				cout << matrixEditDistance[i][j]<<" " ;
			}
			printf("\n");
		}


		double minDistance= minimumDistance(matrixEditDistance,row,column ,nodeSize);
		cout <<"MinimumDistance:" << minDistance<<endl;
		cout <<"row:" << row<<endl;
		cout <<"column:" << column<<endl;
		
		node mergedNode;
		mergedNode.height = (double)matrixEditDistance[row ][column ]/2; //These are
		cout <<"Merged Node height:" << mergedNode.height<<endl;
		mergedNode.name= allNodes[row].name+allNodes[column].name;
		cout <<"Merged Node name:" << mergedNode.name<<endl;
		mergedNode.numOfGenes = allNodes[row].numOfGenes + allNodes[column].numOfGenes;
		cout <<"Merged Node numOfGenes:" << mergedNode.numOfGenes<<endl;
		cout << "-------------UPDATE MATRIX------------------------\n";
		//update matrix and allNodes array
		if (column < row) {
			int temp = row;
			row = column;
			column = temp;
		}
		
		double add[row];
		bool check=false;
		for (int i = 0; i < row; i++) {

			double first=(matrixEditDistance[row][i]* allNodes[row].numOfGenes+ matrixEditDistance[column][i]* allNodes[column].numOfGenes );
			double second=allNodes[row].numOfGenes+allNodes[column].numOfGenes;
			add[i]=(double)first/second;
			cout<<"2.:"<<endl;
			check=true;
		}
		for(int i=0;i<row;i++){
			matrixEditDistance[row][i]=add[i];
		}
		//matrixEditDistance[row]=add;
	
//		cout << "AFTER 0.:"<<endl;
//		for (int i = 0; i <nodeSize; i++) {
//			for (int j = 0; j < i; j++) {
//				cout << matrixEditDistance[i][j]<<" " ;
//			}
//			printf("\n");
//		}
		
		for (int i = row + 1; i < column; i++) {
		
			double first=(matrixEditDistance[i][row]* allNodes[row].numOfGenes+ matrixEditDistance[column][i]* allNodes[column].numOfGenes );
			double second =allNodes[row].numOfGenes+allNodes[column].numOfGenes;
			matrixEditDistance[i][row]=(double)first/second;
			cout<<"1.:"<<endl;
		}
		
//		cout << "AFTER 1.:"<<endl;
//		for (int i = 0; i <nodeSize; i++) {
//			for (int j = 0; j < i; j++) {
//				cout << matrixEditDistance[i][j]<<" " ;
//			}
//			printf("\n");
//		}
		cout<<"nodeSize:"<< nodeSize<<endl;
		for (int i = column + 1; i < nodeSize; i++) {
			
			double first=(matrixEditDistance[i][row]* allNodes[row].numOfGenes+ matrixEditDistance[i][column]* allNodes[column].numOfGenes );
			double second =allNodes[row].numOfGenes+allNodes[column].numOfGenes;
			matrixEditDistance[i][row]=(double)first/second;
			cout<<"2.:"<<endl;

			for(int k=column;k<nodeSize-1; k++){
				matrixEditDistance[i][k]=matrixEditDistance[i][k+1];
			}
			
		}
		for(int k=column;k<nodeSize-1; k++){
			matrixEditDistance[k]=matrixEditDistance[k+1];
		}
//		cout << "AFTER 2.:"<<endl;
//		for (int i = 0; i <nodeSize; i++) {
//			for (int j = 0; j < i; j++) {
//				cout << matrixEditDistance[i][j]<<" " ;
//			}
//			printf("\n");
//		}
		cout << "-------------UPDATED MATRIX------------------------\n";
		
		cout<< "Updated matrix" << endl;
		for (int i = 0; i <nodeSize-1; i++) {
			for (int j = 0; j < i; j++) {
				cout << matrixEditDistance[i][j] <<" ";
			}
			printf("\n");
		}

		//Netwick
		string netwick="("+netwickArray[row]+":";
		//netwick=netwick+ to_string(mergedNode.height);
		netwick=netwick+ to_string(mergedNode.height-heightOfMerged[row]);
		netwick=netwick+", "+netwickArray[column]+":";
		//netwick=netwick+ to_string(mergedNode.height);
		netwick=netwick+ to_string(mergedNode.height-heightOfMerged[column]) + ")";
		cout << "Netwick: " << netwick<<endl;
		
		netwickArray[row]=netwick;
		for(int k=column;k<nodeSize-1; k++){
			netwickArray[k]=netwickArray[k+1];
		}
		cout << "Netwick Array Until Now: " << netwickArray[0]<<endl;
		
		//TREE HEİGHTLERİNİ STORELAMANN LAZIM PRİNTLEMEK ICIN
		//update allNodes array
		allNodes[row]=mergedNode; //??
		for(int k=column;k<nodeSize-1; k++){
			allNodes[k]=allNodes[k+1];
		}
		cout<< "allNodes[i].name"<<endl;
		cout<<"	";
		for (int i = 0; i < nodeSize-1; i++) {
			cout<<allNodes[i].name<<" ";
		}
		cout<<endl;
		//updates names array
		names[row]=mergedNode.name;
		for(int k=column;k<nodeSize-1; k++){
			names[k]=names[k+1];
		}
		cout<< "names[i]"<<endl;
		cout<<"	";
		for (int i = 0; i < nodeSize-1; i++) { //size of names array you should store
			cout<< names[i]<<" ";;
		}
		cout<<endl;
		
		//tree heightları ?? error?? check again
		heightOfMerged[row]=mergedNode.height;
		
		nodeSize--;

		
	
	
	}
	
	//print output file
	outFile(outFileName,netwickArray[0]);
	
	return 0;
}

double minimumDistance(double **matrixEditDistanc,int &row, int &column,int nodeSize_){
	
	double minDistance = matrixEditDistanc[1][0]; //its like 1st one to pick
	//cout<<"In minimumDistance function, matrixEditDistance[1][0] is:"<<minDistance<<endl;
	row = 1;
	column = 0;

	for (int i = 0; i < nodeSize_; i++) {
		for (int j = 0; j < i; j++) {
			if (matrixEditDistanc[i][j] < minDistance) {
				minDistance = matrixEditDistanc[i][j];
				row = i;
				column = j;
			}
		}
	}
	return minDistance;
}

int affineAndEditDistance(int str1Length, int str2Length, int matchScore, int mismatchScore, int gapopen, string option,  int gapExtention){
	
	int counter=0;
	string str1="";
	for(int j=0;j<500;j++){
		if(sequences[str1Length][j]!='0'){
			counter++;
			str1=str1+sequences[str1Length][j];
		}
	}

	int counter1=0;
	string str2="";
	for(int j=0;j<500;j++){
		if(sequences[str2Length][j]!='0'){
			counter1++;
			str2=str2+sequences[str2Length][j];
		}
	}

	int row=counter+1;
	rowGlobal=counter+1;
	int column=counter1+1;
	columnGlobal=counter1+1;
	
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
	backtrack[0][0] = 1;
	for (int j = 1; j <= column; j++)
		backtrack[0][j] = 1;
	for (int i = 1; i <= row; i++)
		backtrack[i][0] = 2;
	
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
			
			
			if(G[i][j] >= E[i][j] && G[i][j] >= F[i][j]) //boş
			{
				V[i][j]=G[i][j];
				//cout<<"G[i][j] is largest."<<G[i][j]<<endl;
				backtrack[i][j] = 0;
			}
			else if(E[i][j] >= G[i][j] && E[i][j] >= F[i][j] )
			{
				V[i][j]=E[i][j];
				//cout<<"F[i][j] is largest."<<F[i][j]<<endl;
				backtrack[i][j] = 1;
				
			}
			else
			{
				V[i][j]=F[i][j];
				//cout<<"E[i][j] is largest."<<E[i][j]<<endl;
				backtrack[i][j] = 2;
			}
	
		}
	}
	
	//backtrack
	//backtrace and print the alignment with best score
	//global da son row ve columndan başlanır local da ise biggest element bul
	int rowTraverse=row-1;
	int columnTraverse=column-1;
	
	int l = rowTraverse + columnTraverse;
	int i=rowTraverse;
	int j=columnTraverse;
	sequence1="";
	sequence2="";
	
	
	while(!(i == 0 || j == 0)){
		
		if(backtrack[i][j]==0){ //diagonal
			//cout<<"gir3"<<endl;
			sequence1= str1[i-1]+sequence1;
			sequence2= str2[j-1]+sequence2;
			i--;
			j--;
		}else if(backtrack[i][j]==1){ //right
			//cout<<"gir1"<<endl;
			sequence1= '-'+sequence1;
			sequence2= str2[j-1]+sequence2;
			j--;
		}else if(backtrack[i][j]==2){ //down
			//cout<<"gir2"<<endl;
			sequence1= str1[i-1]+sequence1;
			sequence2= '-'+sequence2;
			i--;
			
		}
		
		
	}
	//print sequence
//	cout<<"sequence1:"<<sequence1<<endl;
//	cout<<"sequence2:"<<sequence2<<endl;
//	cout<<"sequence1.length(),:"<<sequence1.length()<<endl;
//	cout<<"sequence2.length(),:"<<sequence2.length()<<endl;
//  cout<<"Global score:"<<V[rowTraverse][columnTraverse]<<endl;
	
	int editDistance = 0;
	
	for(int i = rowTraverse, j = columnTraverse; i >= 1 && j >= 1;) {
		if (backtrack[i][j]==0) {
			if (str1[i-1] != str2[j-1]){
				editDistance++;
			}
			i--;
			j--;
		}
		else if (backtrack[i][j]==1) {
			editDistance++;
			j--;
		}
		else {
			editDistance++;
			i--;
		}
	}
	//cout<<"editDistance:"<<editDistance<<endl;
	return (double)editDistance;
}





void outFile(string outFileName, string result ){
	ofstream myfile;
	myfile.open (outFileName);
	myfile << result<<" \n";
	myfile.close();
	
}


/*
 >A
 CTAGATAATTGCCAGATGATCAAATTTATAT
 >B
 CTAGATAATCATGCTAGCTAGTGCACAAATTTATAT
 >C
 CTAGATAATTGGAATGTCGATCGATCG
 
 
 
 >A
 CTAGATAATTGCCAGATGATCAAATTTATATAGTA
 >B
 CTAGATAATCATGCTAGCTAGTGCACAAATTTATATATGGC
 >C
 CTAGATAATTGGAATGTCGATCGATCGGATTTT
 >D
 CTAGAGATATGGAATTTGGAATGTCGATCT
 
 

(((A:6.000000, B:6.000000):0.000000, (C:7.000000, D:7.000000):0.000000):9.125000, D:9.125000)

((A:6.000000, B:6.000000):3.750000, ((C:7.000000, D:7.000000):2.000000, D:9.000000):0.750000)

*/

