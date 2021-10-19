//
//  main.cpp
//  homework1
//
//  Created by Ilknur Bas on 25.09.2020.
//  Copyright © 2020 Ilknur Bas. All rights reserved.
//

#include <stdio.h>
#include <typeinfo>
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

#include <string>

string* getDNA(const string fileName);
string medianStringSearch(string * dna, int kLength,int noOfSequence);
int kLength;
int noOfSequence;
int hammingDistance(string dna,string leaf,int kLength);
void nextVertex(int &ith,string &v,int kLength,int noOfNucleotide);
void byPass(int &ith,string &v, int kLength,int noOfNucleotide);

int main (int argc, const char * argv[]) {
	
	string fileName;
	if (argc > 1) {
		fileName = argv[1];
		cout << "read from command line argument" << endl;
	}
	else {
		fileName = "sample.txt";
		cout << "not be read from command line, fixed file name will be used" << endl;
	}
	
	//const string fileName="input.txt";
	string * dna = getDNA(fileName);
	
	
	//k must be equals or less than 16, pattern must be of a length equal or less than 500 and the number of DNA sequences must be equal or less than 100.
	if (kLength<=16 && noOfSequence<=100 ){
		string bestPattern = medianStringSearch(dna,kLength,noOfSequence);
		cout<<"Best Pattern is:"<<bestPattern <<endl;
	}else{
		cout<<"wrong kLength or noOfSequence" <<endl;
	}
	
}
	

	
string* getDNA(const string fileName){

	ifstream file;
	file.open(fileName);
	bool a=true;
	string str;
	int sequenceCounter=-1;
	
	
	noOfSequence=0;
	if (file.is_open())
	{
		while (getline(file,str,'\n' )) {
			if(a){
				a=false;
			}else{
				noOfSequence++;
			}
		}
	}
	file.close();
	cout<<"Number of sequences in DNA: "<<noOfSequence<<endl;
	file.open(fileName);
	a=true;
	string* dna=new string [noOfSequence];
	if (file.is_open())
	{
		while (getline(file,str,'\n' )) {
			if(a){ //k- mer pattern's length
				kLength = stoi(str);
				cout<<"K- mer pattern's length is: "<<kLength<<endl;
				a=false;
		
			}else{
				sequenceCounter++;
				dna[sequenceCounter]=str;
			}
		}
	}
	file.close();
	
	//cout<<"sequenceCounter: "<<sequenceCounter<<endl;
	/*for (int i=0;i<noOfSequence;i++){
		cout<<"DNA[" << i<<"]is: "<< dna[i]<<endl;
	}*/
	
	return dna;
}



string medianStringSearch(string * dna,int kLength,int noOfSequence){
	
	//create v; the one will be compared; 1=A, 2=C, 3=G, 4=T; also used for the functions, nextVertex and byPass
	string v="";
	string bestPattern="";
	for(int i=0;i<kLength;i++){
		bestPattern+="A";
		v+="1";
	}
	//cout<<"best pattern: "<< bestPattern<<endl;
	
	int bestDistance=INT_MAX; //not sure
	int ith=1;
	
	//in order to test
	//ith=6;
	
	while(ith > 0) {
		
		if( ith<kLength ) {

			string prefix = v.substr (0,ith);
			//cout<<"prefix:"<<prefix<<endl;
			for (int i=0;i<ith;i++) {
				if (prefix[i]=='1'){
					prefix[i]='A';
				}else if(prefix[i]=='2'){
					prefix[i]='C';
				}else if(prefix[i]=='3'){
					prefix[i]='G';
				}else if(prefix[i]=='4'){
					prefix[i]='T';
				}
			}
			//cout<<"prefix:"<<prefix<<endl;
			
			
			int totalDistance=0;
			//calculate hamming distance for each sequence in dna; add them and find totalDistance
			for (int i=0;i<noOfSequence;i++) {
				//find smallest distance with given leaf in given sequence
				//hamming distance will return smallest distance
				//totalDistance+=hammingDistance(dna[i],prefix,kLength);
				totalDistance+=hammingDistance(dna[i],prefix,ith);
			}
			//cout<<"totalDistance in dna:"<<totalDistance<<" for prefix:"<<prefix<<endl;
			
			int branchAndBoundDistance =totalDistance;
			totalDistance=0;
			if (branchAndBoundDistance > bestDistance){
				//bypass;no need to look its children
				byPass(ith,v,kLength,4);
				//after this ith and v sould be change
				//cout<<"byPass ith: "<<ith<<endl;
				//cout<<"byPass v: " <<v<<endl;
				//(v, i) ← ByPass(v, i, l, 4)
			}else{
				nextVertex(ith,v,kLength,4);
				//after this ith and v sould be change
				//cout<<"ith: "<<ith<<endl;
				//cout<<"v: " <<v<<endl;
				
			}
			//test
			//ith=0;
			
			
		}else{ //we are calculating for score for the leaf.
			string leaf = v;
			for (int i=0;i<kLength;i++) {
				if (v[i]=='1'){
					leaf[i]='A';
				}else if(v[i]=='2'){
					leaf[i]='C';
				}else if(v[i]=='3'){
					leaf[i]='G';
				}else if(v[i]=='4'){
					leaf[i]='T';
				}
			}
			
			int totalDistance=0;
			
			//calculate hamming distance for each sequence in dna; add them and find totalDistance
			for (int i=0;i<noOfSequence;i++) {
				//find smallest distance with given leaf in given sequence
				//hamming distance will return smallest distance
				totalDistance+=hammingDistance(dna[i],leaf,kLength);
			}
			//cout<<"totalDistance in dna:"<<totalDistance<<" for v:"<<v<<" for leaf:"<<leaf<<endl;
			if (totalDistance<bestDistance) {
				bestDistance=totalDistance;
				bestPattern=leaf;
			}
			totalDistance=0;
			//i değişecek, v değişecek; ikisini return etmek lazım; &
			
			//NORMALDE BESTPATTERN YERİNE V KULLNAMIŞTIK!!!!!!!
			nextVertex(ith,v,kLength,4);
			//after this ith and v sould be change
			//cout<<"ith: "<<ith<<endl;
			//cout<<"v: " <<v<<endl;
			
			
		}
		
	}

	return bestPattern;
}
void byPass(int &ith,string &v, int kLength,int noOfNucleotide){
	
	for(int j=ith-1;j>=0;j--){
		if(v[j]!='4'){
			//cout<<"bypass:o position 4e eşit değil:" <<v[j]<<endl;
			int b=v[j]-'0';
			b++;
			//cout<<"b is int:" <<b<<endl;
			if (b==1){
				v[j] ='1';
			}else if(b==2){
				v[j] ='2';
			}else if(b==3){
				v[j] ='3';
			}else if(b==4){
				v[j] ='4';
			}
			//ith aynı kalacak ilk buraya girer ve çıkarsa
			return;
			
		}
		//ith değişmeli
		ith--;
	}
	//4,- deyiz
	//cout<<"no bypass move to consider" <<v<<endl;
	ith=0; //this is for exiting from forever while loop;actually it is already zero if we are here
	v="v= no bypass/vertex left to consider "; //do we need to write that?
}


void nextVertex(int &ith,string &v, int kLength,int noOfNucleotide){
	
	if (ith<kLength) { //in a case when 2_ where we are looking for 2-mer
		v[ith]='1';
		ith++;
		//cout<<"		before v with ith:"<< v<< " - after nextVertex:" <<endl;
		//cout<<"		after nextVertex ith:" <<ith<<endl;
		return;
		
	}else{ //in that case we are in the leaf and moving next vertices
		//cout<<"else part ith>=kLength:" <<ith<<endl;
		//yani v nin hepsi kullanılacak
		for (int j=kLength-1;j>=0;j--){
			if( v[j] !='4'){
				//cout<<"o position 4e eşit değil:" <<v[j]<<endl;
				int b=v[j]-'0';
				b++;
				//cout<<"b is int:" <<b<<endl;
				if (b==1){
					v[j] ='1';
				}else if(b==2){
					v[j] ='2';
				}else if(b==3){
					v[j] ='3';
				}else if(b==4){
					v[j] ='4';
				}
				//ith aynı kalacak ilk buraya girer ve çıkarsa
				return;
			}
			
			//ith değişmeli
			ith--;
		}
		
	}
	//4,4 deyiz
	cout<<"no vertex left to consider" <<v<<endl;
	ith=0; //this is for exiting from forever while loop;actually it is already zero if we are here
	v="v= no vertex left to consider "; //do we need to write that?
	
}

int hammingDistance(string dna,string leaf,int kLength){
	int distance=0;
	int minDistance=INT_MAX;
	//cout<<"dna.length()"<<dna.length()<<endl;
	//+1 ekledin????
	for(int i=0; i< dna.length()-kLength+1;i++){ //starting from 0th position until dna.length()-kLength th position
		int z=i;
		for(int j=0;j<kLength;j++){
			//cout<<"dna[z]:"<<typeid(dna[z]).name()  <<"-leafNucleotide[j]:"<<typeid(leaf[j]).name()<<endl;
			if (dna[z]!=leaf[j]){
				distance++;
			}
			z++;
		}
		//cout<<"distance in hamming:"<<distance<<endl;
		if(distance<minDistance){
			minDistance=distance;
		}
		distance=0;
		
	}
	//cout<<"minDistance in hamming:"<<minDistance<<endl;
	return minDistance;
}







