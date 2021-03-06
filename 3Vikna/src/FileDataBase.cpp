#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <algorithm>
#include <vector>
#include "../include/FileData.h"
#include "../include/ComputerScientist.h"
// , Nationality, Fields
const string DEFAULT_YEAR = "-2015";
const int NUMBER_OF_COLUMNS = 6;
//Constructor.
//Reads from the database on the file (if exists)
FileData::FileData(string DataBaseFile){
     base = DataBaseFile;
     Load();
}

//Add
//Accepts a ComputerScientist class
//Stores it in memory until ready to write to disk
void FileData::Add(ComputerScientist scientist){
     compsci.push_back(scientist);
}

//Save()
//Writes all stored computer scientists to disk
//Returns true if successful, False otherwise
bool FileData::Save(string filename){
     //Local variables
     ofstream Outstream;
     filename = (filename == "") ? base:filename;
     Outstream.open(filename);
     //Saving
     if(Outstream.is_open()){
          //Friendly Warning message
          //Outstream << "Please do not alter this file manually. \n";
          //Iterates trough the entire set; printing each entry to the file in turn
          for (unsigned int i = 0; i<compsci.size();i++){
               Outstream << compsci[i].ToString(false) << "." << "\n";
          }
          Outstream.close();
          return true;
     }
     else{
          return false; //File is not opened; error.
     }
}
//Load()
//Reads all entries from the database
//Returns true if opening the database worked
//Writes out to Stdout how many entries were loaded for debugging purposes
bool FileData::Load(string filename){
   //local variables
     ifstream InStream;
     filename = (filename == "") ? base:filename;
     InStream.open(filename);
     compsci.clear();
     int N;
     string t;
     char newline[2];
     N = 0;
     //Loading
     if(InStream.is_open()){
          //Reads in all lines in file. getline('\n') mostly to trunicate newlines away
          while(getline(InStream,t,'.') && InStream.getline(newline,2,'\n')){
               //Info: All information except birth and deathyear
               //Born: Birth and death year
               string info[6];
               string born[2];
               t+=" ";
               //Default values if not present
               for(int i = 0; i < 6; i++){
                    info[i] = "";
               }
               for(int i = 0; i < 2; i++){
                    born[i] = DEFAULT_YEAR;
               }
               //Reads in informations and groups them
               vector<string> result;
               result = explode(string(t),',');
               /*[0] = name
                * [1] = gender
                * [2] = born
                * [3] = dead
                * [4] = nationality
                * [5] = fields */
               if(result.size() != NUMBER_OF_COLUMNS){
                    //Bad Data
                    //Too much or too little information in this line
                    //cout << "Something is wrong with " << t <<" : "<< result.size() << endl;
                    continue;
               }
               //Reads in the name and splits it up
               vector<string> name;
               name = explode(result[0],' ');
               //Who just has a first name? Seriously
               if(name.size() == 1){
                    info[0] = name[0];
               }
               else{
                    //Reads in first, middle name if any, and last name
                    info[0] = name[0];
                    for(unsigned int i = 1; i < name.size() - 1; i++){
                         info[1] += name[i] + " ";
                    }
                    info[2] = name[name.size()-1];
               }
               //Gender is located in result[1]
               info[3] = result[1];
               //I became lazy; reads the birth and death year and converts to int

               born[0] = result[2];
               born[1] = result[3];
               if(born[1].length() < 3){
                    born[1] = DEFAULT_YEAR;
               }
               if(born[0].length() < 3){
                    born[0] = DEFAULT_YEAR;
               }
               info[4] = result[4];
                       info[5] = result[5];
               //Trims all spaces from the string values
               for(int i = 0; i < NUMBER_OF_COLUMNS; i++){
                    int first = info[i].find_first_not_of(" ");
                    int last = info[i].find_last_not_of(" ");
                    if(first > 0){
                         info[i] = info[i].substr(first,last);
                    }
                    else{
                        if(last < info[i].size() - 2){
                            info[i] = info[i].substr(0,last + 2);
                        }
                    }
               }
               //Creates entry and adds to the database
               ComputerScientist Entry(info[0] , info[1], info[2], info[3], born[0],born[1],info[4],info[5]);
               compsci.push_back(Entry);
               //Success! Increments
               N++;
          }
     }
     else{
          return false; //Error opening file. first time creation?
     }
     //Done. Report success
     cout << N << " Scientists loaded from database" << endl;
     return true;
}

vector<string> FileData::explode(const string s, char delim){
     vector<string> ret;
     stringstream stream(s);
     string temp;
     while(getline(stream, temp, delim)){
          ret.push_back(temp);
     }
     return ret;
}

vector<ComputerScientist> FileData::DataSet(){
    return compsci;
}
