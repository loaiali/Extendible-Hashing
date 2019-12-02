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
#include "extendible.h"
#include<iostream>
using namespace std;

void insert(ExtendibleHashing&, int key,int data);
int deleteItem(ExtendibleHashing&, int key);
struct DataItem * search(ExtendibleHashing&, int key);

fstream resultFile;
int main(int argc, char *argv[])
{

  ExtendibleHashing ex("db/extendible_dir", "db/extendible_data", 32 * sizeof(Bucket));

  string writePath = string("tests/res/") + "e_" + string(basename(argv[1]));
  resultFile.open(writePath, fstream::out);
  fstream testfile(argv[1], fstream::in);

  char operation;
  testfile >> operation;
  while (!testfile.eof())
  {
    int key, value;
    switch (operation)
    {
    case 'V':
      resultFile << "* view file operation\n";
      ex.DisplayFile();
      ex.DisplayFile(resultFile);
      break;
    case 'I':
      testfile >> key >> value;
      resultFile << "* insert operation of " << key << ", " << value << endl;
      insert(ex, key, value);
      break;
    case 'S':
      testfile >> key;
      resultFile << "* search operation of " << key << endl;
      search(ex, key);
      break;
    case 'D':
      testfile >> key;
      resultFile << "* delete operation of " << key << endl;
      deleteItem(ex, key);
      break;
    default:
      break;
    }
    testfile >> operation;
    resultFile << "================================================\n\n";
  }
  testfile.close();

  return 0;
}

void insert(ExtendibleHashing &ex, int key, int data)
{
  struct DataItem item;
  item.data = data;
  item.key = key;
  item.valid = 1;
  int result = ex.insertItem(item);
  resultFile << "Insert: No. of searched records " << abs(result) << endl;
  printf("Insert: No. of searched records:%d\n", abs(result));
}

struct DataItem * search(ExtendibleHashing &ex, int key)
{
  struct DataItem* item = (struct DataItem *) malloc(sizeof(struct DataItem));
     item->key = key;
     int diff = 0;
     int Offset= ex.searchItem(item,&diff); //this function is implemented for you in openAddressing.cpp
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


int deleteItem(ExtendibleHashing &ex, int key){
   struct DataItem* item = (struct DataItem *) malloc(sizeof(struct DataItem));
   item->key = key;
   int diff = 0;
   int Offset= ex.searchItem(item, &diff);
   printf("Delete: No of records searched is %d\n",diff);
   resultFile << "Delete: No of records searched is " << diff << endl;
   if(Offset >=0 )
   {
    return ex.deleteOffset(Offset);
   }
   return -1;
}
