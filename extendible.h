#pragma once
#include <vector>
#include <string>
using namespace std;
#define RECORDSPERBUCKET 2 //No. of records inside each Bucket

//Data Record inside the file
struct DataItem
{
   int valid; //) means invalid record, 1 = valid record
   int data;
   int key;
};

//Each bucket contains number of records
struct Bucket
{
   int localDepth = -1; // means uninitialized
   struct DataItem dataItem[RECORDSPERBUCKET];
};


struct Directory
{
   int globalDepth = 0;
   vector<int> bucketsOffsets; // string is the bits string key and the value is the offset or the pointer to access the bucket
};

class ExtendibleHashing
{
public:
   ExtendibleHashing(char *directoryName,char *datafileName, int maxFileSize);

   int insertItem(DataItem item);
   int searchItem(struct DataItem *item, int *count);
   int DisplayFile();
   int deleteOffset(int Offset);
   int deleteKey(int key);

private:
   int directoryFHandler; // handler for the directory file
   int BucketsFHandler;   // handler for the buckets file
   Directory directory;
   int createFile(int size, char *name);
   void writeWholeDirector(Directory d);
   int hashCode(int key, int globalDepth);
   Directory readWholeDirectory();
   void splitDirectory();
   int readGlobalDepth();
   int appendBucket(Bucket bucket);
   int readFromDirectory(int pointerOffset); // get the pointer value at pointerOffset in the directory file
   Bucket readFromBuckets(int offset);
   void updateBucket(int offset, Bucket newValue);
   void redistribute(Bucket &original, Bucket &newBucket, int localDepth);
   int writeDirectoryOffset(int offset, int value);
   void updateDirectoryPointers(int hashIndex, int bucketOffset,int localDepth, int insertedBucketOffset);
   int extractNbits(int key,int depth,int p);
   void printBucket(Bucket bucket);
};