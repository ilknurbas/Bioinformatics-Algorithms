//
//  main.cpp
//  hw3
//
//  Created by Ilknur Bas on 26.10.2020.
//  Copyright © 2020 Ilknur Bas. All rights reserved.
//


#include <vector>
#include <map>
#include <queue>
#include <string>
#include <list>
#include <cstring>
#include <cstdio>
#include <iostream>
using namespace std;
#include <getopt.h>
#include <unistd.h>
#include <fstream>
#include <cmath>


struct Node
{
	//it should have 26 children/next nodes since alphabet consists of 26 letters
	Node *allNext[26];
	bool isWord;
};
void constructTrie(Node *rootTrie, string pattern);
void readFile(int argc,  char * argv[]) ;
string *pattern=new string[100]; //değiştir
string text;

void constructTrie(Node *rootTrie, string pattern){
	cout <<"Trie is constructing..."<<endl;
	Node* current=rootTrie;
	//move from the rootTrie
	for(int j=0;j<pattern.length(); j++){
		//??
		int location=pattern[j] - 'a';
		//if the character is not found in its children
		if( !current->allNext[location]){
			//create new node for current
			struct Node *newNode =  new Node;
			newNode->isWord = false;
			for (int i = 0; i < 26; i++){
				newNode->allNext[i] = NULL;
			}
			current->allNext[location] = newNode;
		}
		current=current->allNext[location];
		
	}
	//end of the pattern
	current->isWord=true;
	
}



void readFile(int argc,  char * argv[]) {
	int option=0;
	int index;
	
	const struct option long_options[] =
	{
		{"input",required_argument,0,'i'},
		{"pattern",required_argument,0,'p'},
		{"query",required_argument,0,'q'},
		{0,0,0,0}
	};
	
	
	string inputFile;
	//output will be store in integer option
	option = getopt_long( argc, argv, "i:p:q", long_options , &index);
	
	//while(( option=getopt_long( argc, argv, "i:s:q", long_options , &index))!= -1){
	while( option!= -1){
		switch(option){
			case 'i':{
				//copy optional argument to the filename
				//strcpy(filename, optarg);
				inputFile=string(optarg);
				cout<< "1."<<inputFile<<"\n";
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
	string str;
	ifstream file;
	string patternTrace="";
	file.open(inputFile);
	int i=0;
	if (file.is_open()){
		while (getline(file,str,'\n' )) {
			if(i==0){
				patternTrace=str;
				i++;
			}else{
				text=str;
			}
		}
	}
	file.close();
	
	string eklenecek="";
	int count=0;
	for(int j=0;j<patternTrace.length();j++){
		if(patternTrace[j]==' '|| j==patternTrace.length()-1){
			if (patternTrace[j]==' '){
				pattern[count]=eklenecek;
				count++;
				eklenecek="";
			}else{
				eklenecek=eklenecek+patternTrace[j];
				pattern[count]=eklenecek;
				count++;
				eklenecek="";
			}
		}else{
			eklenecek=eklenecek+patternTrace[j];
		}
		
	}
	//./hw3 -i input.txt
	cout<<"patterns:";
	for(int z=0;z<pattern->size();z++){
		cout<<pattern[z]<<",";
	}
	cout<<endl;
	
	if (text==""&& pattern==NULL){
	cout<<"no file found."<<endl;
		text="once upon a time there was a cool parrot who loves eating carrot on the carpet";
		
		pattern[0]="carrot";
		pattern[1]="cool";
		pattern[2]="parrot";
		pattern[3]="car";
		pattern[4]="carpet";
		pattern[5]="rotate";
		
		
	}
}


int main(int argc,  char * argv[]) {
	readFile(argc,argv);
	//string patterns[]={"carrot", "cool", "parrot", "car", "carpet", "rotate"};
	//int sizeOfPatterns = sizeof(pattern)/sizeof(pattern[0]);
	int sizeOfPatterns = pattern->size();
	cout<<"Size of Patterns:"<<sizeOfPatterns<<endl;
	
	//create new node for root
	Node *newNode =  new Node;
	newNode->isWord = false;
	for (int i = 0; i < 26; i++){
		//cout <<"i:"<<i<<endl;
		newNode->allNext[i] = NULL;
	}
	Node *rootTrie = newNode;
	
	//create trie for each pattern
	for (int i = 0; i < sizeOfPatterns; i++){
		//cout <<"j:"<<i<<endl;
		constructTrie(rootTrie, pattern[i]);
		
	}
	
	
	
}




