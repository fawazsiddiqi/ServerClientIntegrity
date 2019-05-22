//
//  main.cpp
//  CSCI361 Assignment 3

//http://www.zedwood.com/article/cpp-sha256-function

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <bitset>
#include "sha256.h"
using namespace std;

void blockCreate();//server creating hash blocks for the client to recieve
void blockRecieve();//client recieving the file blocks along with the created hash blocks
bool menuRep();//allows the user to go back to main menu
void blockFile(string fileName,vector<bitset<(1024*8)>>blockWritten);//computes the hashes and writes them to a new file
bitset<256> charStringToBin(string text);//changes a string to binary
int blockNumberCheck(string fileName);//counts the number of blocks

vector<string> split(string str,string sep);//splits a string according to the delimiter specified in the sep parameter
string filenameGenerate(string fileName, string addCon);//genereates a new fileName for the hash files
vector<bitset<(1024*8)>> readFileVector(string fileName);// reads the contents of the video file into a vector
bitset<1024*8> hexStringToBin(string);//changes a string from a hex format to binary format

int main(int argc, const char * argv[]) {
    vector<bitset<(1024*8)>>contentFile;
    cout<<"SHA-256 Server-Client Application"<<endl;
    
    cout <<endl;
    
    cout << "INITIATING SERVER SIDE COMPUTATION OF HASHES" <<endl;
    cout << "PLEASE ENTER THE FILE NAME" <<endl;
    blockCreate();
    
    cout << "INITIATING CLIENT SIDE COMPUTATION OF HASHES" <<endl;
    cout << "PLEASE ENTER THE FILE NAME" <<endl;
    blockRecieve();
        
        return 0;
    }


vector<string>split(string str,string sep){
    char* charString=const_cast<char*>(str.c_str());//casting from string to char array
    char* token;
    vector<string> arr;
    token=strtok(charString,sep.c_str());//tokenizing the string according to the sep string in the parameter
    while(token!=NULL){
        arr.push_back(token);//pushing the tokens into a vector
        token=strtok(NULL,sep.c_str());
    }
    return arr;//returning the vector of tokens
}

string filenameGenerate(string fileName, string addCon){
    vector<string> opVectorString;//creating a vector to hold string tokens
    string filenameResult;
    opVectorString=split(fileName, ".");//spliting the fileName using the '.' as the splitting point
    for(int i=0;i<opVectorString.size();i++){
        if(i==opVectorString.size()-2){
            filenameResult=filenameResult+opVectorString[i];
            filenameResult=filenameResult+addCon+".";//adding the string to needs to be added to the file name and returning the '.'
        }
        else{
            filenameResult=filenameResult+opVectorString[i];
            
        }
    }
    return filenameResult;
}

bitset<1024*8> hexStringToBin(string text){
    char hex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};//Hexadecimal representaion
    string binaryArr[16]= {"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111"};//binary representation
    string filenameResult;
    bitset<1024*8>hexResult;
    for(int i=0;i<text.size();i++){
        for(int j=0;j<16;j++){
            if(text[i]==hex[j]){
                filenameResult=filenameResult+binaryArr[j];
                break;
            }
        }
    }
    hexResult=bitset<1024*8>(filenameResult);
    return hexResult;
}

bitset<256> charStringToBin(string text){
    bitset<8>charStr;
    string filenameResult;
    bitset<256> bitResult;
    for(int i=0;i<text.size();i++){
        charStr=bitset<8>((int)(text[i]));//converting the charStr to its ascii form then converting the filenameResult to a bitset of 8 bits
        filenameResult=filenameResult+charStr.to_string();//concatinating the filenameResult with the previous results
    }
    bitResult=bitset<256>(filenameResult);
    return bitResult;
}

vector<bitset<(1024*8)>> readFileVector(string fileName){
    ifstream inputFile;
    int count=0;
    int blocksNum=0;
    char charFile;
    string wordVector;
    vector<bitset<(1024*8)>>contentFile;
    cout<<"READING VIDEO FILE: "<<fileName<<endl;
    inputFile.open(fileName);
    
    while(inputFile>>charFile){//reading characters from the video file
        wordVector=wordVector+charFile;//appending characters from the file to a string
        if(wordVector.size()==2048){//when the string reaches the required capacity
            contentFile.push_back(hexStringToBin(wordVector));//string is converted from hex to bionary then saved into a vector
            wordVector.clear();//string is cleared for the next block
            blocksNum++;
        }
        count++;
    }
    if(!wordVector.empty()){//if more characters in the string
        contentFile.push_back(hexStringToBin(wordVector));//pad the string and add it to the vector
        blocksNum++;
    }
    
    cout<<"READ COMPLETE"<<endl;
    inputFile.close();
    return contentFile;
}

void blockFile(string fileName,vector<bitset<(1024*8)>>blockWritten){
    cout<<"WRITING HASH BLOCKS TO FILE: "<<fileName<<endl;
    ofstream outputFile;
    int count=0;
    string currentHash;
    string prevHash;
    vector<string>inputVector;

    outputFile.open(fileName,ofstream::trunc);

    for(int i=blockWritten.size()-1;i>=0;i--){//for loop traversing the vector of blocks backwards
        if(i==blockWritten.size()-1){//if in last block
            prevHash=sha256(blockWritten[i].to_string());//compute the hash of the block
            inputVector.push_back(prevHash);
            
        }
        else{
            currentHash=sha256(blockWritten[i].to_string()+prevHash);//append the previous hash to the block and hash again
            inputVector.push_back(currentHash);
            prevHash=currentHash;//update the previous has for the next round
        }
        count++;
    }
    for(int i=inputVector.size()-1;i>=0;i--){
        outputFile<<inputVector[i]<<endl;
    }
    cout<<"WRITING OF BLOCKS  "<<count<<" ARE THE BLOCKS WRITTEN" <<endl;
    outputFile.close();
    
}

void blockCreate(){
    vector<bitset<(1024*8)>> contentFile;//vector to hold file contents
    string inputFileName;
    cin >> inputFileName;//getting file name from the user
    string outputFileName=filenameGenerate(inputFileName, "HashFile");//creating another file to hold hash blocks
    contentFile=readFileVector(inputFileName);//reading the contents of the video file
    blockFile(outputFileName,contentFile);//hashing the video file and writing the blocks into the hash file
    cout<<"**********File Written to Directory*********"<<endl;
    
    
}

int blockNumberCheck(string fileName){
    cout<<"OBTAINING SERVER SIDE CALCULATED NUMBER OF HASHES FROM "<<fileName<<endl;
    ifstream inputFile;
    char charFile;
    string wordCheck;
    int blocksNum=0;
    inputFile.open(fileName);
    while(inputFile>>charFile){//reading the file charStr by charStr
        wordCheck=wordCheck+charFile;
        if(wordCheck.size()==2048){//when block size is reached
            wordCheck.clear();//clear the word for the next round
            blocksNum++;//increment block count
        }
    }
    if(!wordCheck.empty()){//if characters are left over it is considered as an another block
        blocksNum++;
    }
    inputFile.close();
    cout<<"SERVER SIDE BLOCK COUNT COMPLETE "<<blocksNum<<" BLOCKS FOUND"<<endl;
    return blocksNum;
}

void blockRecieve(){
    
    //basically reverses the operation of the blockFile function
    string fileName;
    cin >> fileName;//getting the name of the file to be recieved from the user
    string hashFileName=filenameGenerate(fileName, "HashFile");//determining the hash file of the chosen file
    ifstream inputHashFile;
    ifstream inputFile;
    string wordString;
    int numberBlocks;
    char charFile;
    vector<pair<string, bitset<256>>> hashStore;
    string stringHash;
    bitset<256>binaryHash;
    bool complete=true;
    
    numberBlocks=blockNumberCheck(fileName);//counting the size of the file chosen
    
    inputFile.open(fileName);//opening chosen file
    inputHashFile.open(hashFileName);//opening hash file of the chosen file
    
    inputHashFile>>stringHash;//getting H0 the first hash block
    binaryHash=bitset<256>(charStringToBin(stringHash));//converting the hashblock to binary
    hashStore.push_back(pair<string, bitset<256>>(stringHash,binaryHash));//hash is pushed to the vector of hashes
    cout<<"SERVER HASH0: "<<get<0>(hashStore[hashStore.size()-1])<<" SENT"<<endl;
    for(int i=0;i<numberBlocks;i++){
        while(wordString.size()<2048){//getting the block charStr by charStr from the server
            inputFile>>charFile;
            wordString=wordString+charFile;
        }
        wordString=hexStringToBin(wordString).to_string();//converting the recieved data to binary
        if(i!=numberBlocks-1){//if not the last block
            inputHashFile>>stringHash;//get hash block
            binaryHash=bitset<256>(charStringToBin(stringHash)); //convert hash blcok to binary
            cout<<"SERVER SIDE HASH "<<i+1<<": "<<stringHash<<"+ Block"<<i<<" TRASNMITTED"<<endl;
            hashStore.push_back(pair<string, bitset<256>>(stringHash,binaryHash));//send binary hash blcok
            stringHash=sha256(wordString+get<0>(hashStore[hashStore.size()-1])); //compute hash of file block+hash block
            cout<<"CLIENT SIDE HASH "<<i<<": "<<stringHash<<endl;
            binaryHash=bitset<256>(charStringToBin(stringHash));//convert the computed hash to binary
            if(binaryHash.to_string()==get<1>(hashStore[hashStore.size()-2]).to_string()){//compare the computed hash with recieved hash
                cout<<"CLIENT COMPUTED HASH "<<i<<" AND TRASMITTED HASH"<<i<<" ARE EQUAL"<<endl;
            }
            else{
                cout<<"CLIENT SIDE ERROR: COMPUTED HASH "<<i<<" TRASMITTED HASH"<<i<<" ARE NOT EQUAL"<<endl;
                complete=false;
                break;
            }
        }
        else{// if last block is reached the client only computes the hash of the file block and doesnot request a hash block
            hashStore.push_back(pair<string, bitset<256>>(stringHash,binaryHash));
            stringHash=sha256(wordString);
            cout<<"CLIENT SIDE HASH"<<i<<": "<<stringHash<<endl;
            binaryHash=bitset<256>(charStringToBin(stringHash));
            if(binaryHash.to_string()==get<1>(hashStore[hashStore.size()-2]).to_string()){
                cout<<"CLIENT SIDE HASH"<<i<<" AND TRASMITTED HASH"<<i<<" ARE EQUAL"<<endl;
            }
            else{
                cout<<"CLIENT SIDE ERROR: COMPUTED "<<i<<" AND TRASMITTED HASH"<<i<<" ARE NOT EQUAL"<<endl;
                complete=false;
                break;
            }
        }
        wordString.clear();
    }
    if(complete){
        cout<<"VIDEO PLAYED COMPLETELY"<<endl;
    }
    else{
        cout<<"VIDEO STOPPED"<<endl;
    }
    inputHashFile.close();
    inputFile.close();
}
