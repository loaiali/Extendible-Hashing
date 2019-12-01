//============================================================================
// Name        : hashskeleton.cpp
// Author      : 
// Version     :
// Copyright   : Code adapted From https://www.tutorialspoint.com/
// Description : Hashing using open addressing
//============================================================================
#include <string>
#include<fstream>
#include <libgen.h>
#include "readfile.h"
#include "extendible.h"
#include<iostream>
using namespace std;

/*void insert(int key,int data);
int deleteItem(int key);
struct DataItem * search(int key);
*/

// int filehandle;   //handler for the database file
/*ExtendibleHashing* extendibleHashing;
fstream resultFile;
*/
int main(int argc, char *argv[]){

  ExtendibleHashing ex1("ex1","data1",32);
  DataItem item;
  item.data=12;
  item.key=2369;
  item.valid=1;
  ex1.insertItem(item);
  item.key=3760;
  ex1.insertItem(item);
  item.key=4692;
  ex1.insertItem(item);
  item.key=4871;
  ex1.insertItem(item);
  item.key=5659;
  ex1.insertItem(item);
  item.key=1821;
  ex1.insertItem(item);
  item.key=1074;
  ex1.insertItem(item);
  item.key=7115;
  ex1.insertItem(item);
  item.key=1620;
  ex1.insertItem(item);
  item.key=2428;
  ex1.insertItem(item);
  item.key=3943;
  ex1.insertItem(item);
  item.key=4750;
  ex1.insertItem(item);
  item.key=6975;
  ex1.insertItem(item);
  item.key=4981;
  ex1.insertItem(item);
  item.key=9208;
  ex1.insertItem(item);




  ex1.DisplayFile();
  /*//1. Create Database file or Open it if it already exists, check readfile.cpp
   char* dbPath = "db/extendibleHashing";
   if (argc >= 3) {
     dbPath = argv[1];
   }

  //  filehandle = createFile(FILESIZE, dbPath);
  extendibleHashing = new ExtendibleHashing(dbPath, 0);
   string writePath = string("tests/res/") + "e_" + string(basename(argv[1]));
   resultFile.open(writePath, fstream::out);
   fstream testfile(argv[1], fstream::in);

  char operation;
  testfile >> operation;
   while (!testfile.eof()){
     int key, value;
     switch (operation)
     {
     case 'V':
     resultFile << "* view file operation\n";
       extendibleHashing->displayFile();
       extendibleHashing->displayFile(resultFile);
       break;
     case 'I':
       testfile >> key >> value;
      resultFile << "* insert operation of " << key << ", " << value << endl;
       insert(key, value);
       break;
     case 'S':
       testfile >> key;
        resultFile << "* search operation of " << key << endl;
       search(key);
       break;
     case 'D':
       testfile >> key;
        resultFile << "* delete operation of " << key << endl;
       deleteItem(key);
       break;
     default:
       break;
     }
     testfile >> operation;
     resultFile << "================================================\n\n";
   }
   testfile.close();
   close(filehandle);
   */
   return 0;



}

/* functionality: insert the (key,data) pair into the database table
                  and print the number of comparisons it needed to find
    Input: key, data
    Output: print statement with the no. of comparisons
*/
/*
void insert(int key,int data){
     struct DataItem item ;
     item.data = data;
     item.key = key;
     item.valid = 1;
     int result= insertItem(filehandle,item);
     resultFile << "Insert: No. of searched records " << abs(result) << endl;
     printf("Insert: No. of searched records:%d\n",abs(result));
}

/* Functionality: search for a data in the table using the key

   Input:  key
   Output: the return data Item
*/
/*
struct DataItem * search(int key)
{
  struct DataItem* item = (struct DataItem *) malloc(sizeof(struct DataItem));
     item->key = key;
     int diff = 0;
     int Offset= searchItem(filehandle,item,&diff); //this function is implemented for you in openAddressing.cpp
     resultFile << "Search: No of records searched is " << diff << endl;
     printf("Search: No of records searched is %d\n",diff);
     if(Offset <0){  //If offset is negative then the key doesn't exists in the table
       printf("Search: Item not found\n");
       resultFile << "Search: Item not found" << endl;
     }
     else
     {
        printf("Item found at Offset: %d,  Data: %d and key: %d\n",Offset,item->data,item->key);
        resultFile << "Item found at Offset: " << Offset 
        << ",  Data: " << item->data 
        << " and key: " << item->key << endl;
     }
  return item;
}

/* Functionality: delete a record with a certain key

   Input:  key
   Output: return 1 on success and -1 on failure
*/
/*
int deleteItem(int key){
   struct DataItem* item = (struct DataItem *) malloc(sizeof(struct DataItem));
   item->key = key;
   int diff = 0;
   int Offset= searchItem(filehandle,item,&diff);
   printf("Delete: No of records searched is %d\n",diff);
   resultFile << "Delete: No of records searched is " << diff << endl;
   if(Offset >=0 )
   {
    return deleteOffset(filehandle,Offset);
   }
   return -1;
}
*/