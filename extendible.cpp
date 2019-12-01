#include "extendible.h"
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<cmath>
// #include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include<vector>
#include"readfile.h"
using namespace std;

//const int BUCKETSIZE = sizeof(Bucket);
std::vector <bool> bits_from_int(int integer)    // discern which bits of PLC codes are true
{
    std::vector <bool> bool_bits;

    // continously divide the integer by 2, if there is no remainder, the bit is 1, else it's 0
    for (int i = 0; i < 8; i++)
    {
        bool_bits.push_back (integer%2);    // remainder of dividing by 2
        integer /= 2;    // integer equals itself divided by 2
    }

    return bool_bits;
}

int ExtendibleHashing::writeDirectoryOffset(int offset, int value){
	// TODO
}

void ExtendibleHashing::redistribute(Bucket &original, Bucket &newBucket, int localDepth){
	//for every record in the original bucket we will check whether it will stay in the same bucket or not
	for(int i=0;i<RECORDSPERBUCKET;i++){
		int index=hashCode(original.dataItem[i].key,directory.globalDepth);
		if(extractNbits(index,localDepth,pow(2,localDepth))%2==1){
			original.dataItem[i].valid=0;
			newBucket.dataItem[i].valid=1;
			newBucket.dataItem[i].key=original.dataItem[i].key;
			newBucket.dataItem[i].data=original.dataItem[i].data;
		}
	}
	//pwrite(BucketsFHandler,&original,sizeof(Bucket),originalOffset);
	//pwrite(BucketsFHandler,&newBucket,sizeof(Bucket),newOffset);
	return;
}
int ExtendibleHashing:: extractNbits(int number,int k,int p){
	cout<<endl<<"anding "<<((1 << k) - 1)<<" with "<<(number>>(p-1));
	return (((1 << k) - 1) &(number>>(p-1)));
}

void ExtendibleHashing::updateBucket(int offset, Bucket newValue){
	pwrite(BucketsFHandler,&newValue,sizeof(Bucket),offset*sizeof(Bucket));

}

Bucket ExtendibleHashing::readFromBuckets(int offset){
	Bucket bucket;
	pread(BucketsFHandler,&bucket,sizeof(bucket),offset);
	return bucket;
}

int ExtendibleHashing::readFromDirectory(int pointerOffset){
	// TODO

}

int ExtendibleHashing::appendBucket(Bucket bucket){
	//returns -1 on failure
	//returns bucket address on success
	//assuming max buckets is 32
	for(int i=0;i<32;i++){
		Bucket temp;
		pread(BucketsFHandler,&temp,sizeof(Bucket),i*sizeof(Bucket));
		//cout<<"reading Bucket number "<<i<<" at offset "<<i*sizeof(Bucket)<<endl;
		for (int j=0;j<RECORDSPERBUCKET;j++){
			//cout<<"record number "<<j<<" valid attribute is: "<<temp.dataItem[j].valid<<"and key "<<temp.dataItem[j].key<<endl;
			if(temp.dataItem[j].valid==0){
			//found an empty bucket
			//pwrite(BucketsFHandler,&bucket,sizeof(Bucket),i*sizeof(Bucket));
			cout<<"bucket will be added at address "<<i*sizeof(Bucket);
			return i*sizeof(Bucket);
			}

		}
	}
	return -1;
}

int ExtendibleHashing::readGlobalDepth(){
	// TODO

}

void ExtendibleHashing::writeWholeDirector(Directory d){
	pwrite(directoryFHandler,&d.globalDepth,sizeof(int),0);
	for(int i=0;i<pow(2,d.globalDepth);i++){
		pwrite(directoryFHandler,&d.bucketsOffsets[i],sizeof(int),sizeof(int)*(i+1));
	}
	return;
}
void ExtendibleHashing:: updateDirectoryPointers(int hashIndex, int bucketOffset,int localDepth, int insertedBucketOffset){
	int startingIndex;
	for(int i=0;i<pow(2,directory.globalDepth);i++ ){
		if(directory.bucketsOffsets[i]==bucketOffset){
			startingIndex=i;
			break;
		}
	}
	for(int i=startingIndex+(pow(2,localDepth)/2);i<pow(2,localDepth);i++){
		directory.bucketsOffsets[i]=insertedBucketOffset;
	}
}

Directory ExtendibleHashing::readWholeDirectory(){
	Directory d;
	int gd;
	pread(directoryFHandler, &gd, sizeof(int), 0);
	d.globalDepth=gd;
	for (int i = 0; i < pow(2, gd); i++){
		int temp;
		pread(directoryFHandler,&temp,sizeof(int),(i+1)*sizeof(int));//i+1 because at i=0 we will be reading the global depth
		d.bucketsOffsets.push_back(temp);
	}
	return d;
}

ExtendibleHashing::ExtendibleHashing(char *directoryName,char *datafileName, int maxFileSize )
{
	directoryFHandler = createFile(maxFileSize, directoryName);
	BucketsFHandler = createFile(maxFileSize, datafileName);
	//writeWholeDirector(Directory()); // insertDirectory remove the current directory and insert the passed directory in directoryFile
	directory = readWholeDirectory();
}

int ExtendibleHashing::createFile(int size, char *name)
{
	int fd;
	int result;
	struct stat sb;

	if (stat(name, &sb) == -1)
	{
		printf("file doesn't exist, create it\n");
		fd = open(name, O_RDWR | O_CREAT, (mode_t)0600);
		if (fd == -1)
		{
			perror("Error opening file for writing");
			return 1;
		}
		if (size == 0)
			return fd;

		/* Stretch the file size.
     * Note that the bytes are not actually written,
     * and cause no IO activity.
     * Also, the diskspace is not even used
     * on filesystems that support sparse files.
     * (you can verify this with 'du' command)
     */
		result = lseek(fd, size - 1, SEEK_SET);
		if (result == -1)
		{
			close(fd);
			perror("Error calling lseek() to 'stretch' the file");
			return 1;
		}

		/* write just one byte at the end */
		result = write(fd, "", 1);
		if (result < 0)
		{
			close(fd);
			perror("Error writing a byte at the end of the file");
			return -1;
		}
	}
	else
	{ //file exists
		//| O_EXCL
		fd = open(name, O_RDWR, (mode_t)0600);
		if (fd == -1)
		{
			perror("Error opening file for writing");
			return -1;
		}
	}
	return fd;
}

/*
	return int index of that key in the dictionary given globaldepth
 */
int ExtendibleHashing::hashCode(int key, int globalDepth)
{
	// convert key to bits
	// read first globalDepth bits
	// convert these bits again into integer
	int hash = key % 32;
	//i want to take the left most global depth bits
	int index=extractNbits(hash,globalDepth,5-globalDepth+1);
	/*auto bits = bits_from_int(hash);
	cout << "the vector of " << key << " is ";
	for(bool bit : bits){
		cout << bit << " ";
	}
	cout << endl;
	for(int i = 0; i < globalDepth; i++){
		bool bit = bits.back();
		bits.pop_back();
	}
	return 0;*/
	return index;
}


void ExtendibleHashing::splitDirectory(){
	//Directory currentD = readWholeDirectory();
	/*Directory newD;
	newD.globalDepth = currentD.globalDepth += 1;
	auto currentBuckets = currentD.bucketsOffsets;
	int* newBuckets = new int(pow(2, newD.globalDepth));
	for (int i = 0, j = 0; i < pow(2, currentD.globalDepth); i++)
	{
		newBuckets[j++] = currentBuckets[i];
		newBuckets[j++] = i == splitOffset ? newBucketOffset : currentBuckets[i];
	}
	newD.bucketsOffsets = newBuckets;
	writeWholeDirector(newD);*/
	directory.globalDepth++;
	directory.bucketsOffsets.resize(pow(2,directory.globalDepth));
	vector<int> newDirectory;

	for (int i=0;i<directory.bucketsOffsets.size();i++){
		newDirectory.push_back(directory.bucketsOffsets[i]);
		newDirectory.push_back(directory.bucketsOffsets[i]);
	}
	directory.bucketsOffsets=newDirectory;
}

//returns 1 on success,-1 on failure
int ExtendibleHashing::insertItem(DataItem item)
{
	cout<<"directory.globalDepth "<<directory.globalDepth<<"directory.bucketsOffsets.size() "<<directory.bucketsOffsets.size();
	//in this case we don't have a directory so we need to create it
	if (directory.globalDepth == 0 && directory.bucketsOffsets.size() == 0){		
		Directory newD;
		newD.globalDepth=0;
		Bucket b;
		b.localDepth = 0;
		b.dataItem[0] = item;
		b.dataItem[1].valid = 0;
		int insertedBucketOffset = appendBucket(b);
		newD.bucketsOffsets.push_back(insertedBucketOffset);
		writeWholeDirector(newD);
		return 1;
	}
	else {
		// there is a dictionary with globalDepth gd
		int hashIndex = hashCode(item.key, directory.globalDepth);
		cout<<" Inserting item with key "<<item.key<<" In index "<<hashIndex<<endl;
		int bucketOffset = directory.bucketsOffsets[hashIndex];
		cout<<"bucketOffset "<<bucketOffset<<endl;
		Bucket targetBucket = readFromBuckets(bucketOffset);
		// loop through bucket records and find available space in this bucket
		for (int i = 0; i < RECORDSPERBUCKET; i++){
			if (!targetBucket.dataItem[i].valid){
				cout<<"at iteration "<<i<<"inserted item with key "<<item.key<<endl;
				// empty record, insert the item here
				targetBucket.dataItem[i] = item;
				updateBucket(bucketOffset, targetBucket); // update the dataitems
				return 1; // * success insert
			}
		}
		// the bucket is full, we need to split the bucket, and update directory if needed
		// split the bucket into two buckets and try inserting the item again
		cout<<"Bucket was full !!"<<endl;
		Bucket newBucket;
		targetBucket.localDepth += 1;
		newBucket.localDepth = targetBucket.localDepth;
		int insertedBucketOffset = appendBucket(newBucket);
		
		if(insertedBucketOffset==-1){
			perror("File is full");
			return -1;
		}

		redistribute(targetBucket, newBucket, targetBucket.localDepth); // redistribute data among two buckets
		
		updateBucket(bucketOffset, targetBucket); // update local depth and may be the records also after redistribution


		if (targetBucket.localDepth > directory.globalDepth) splitDirectory();
		// here is we split the bucket but the directory have enough bits, so we need to update the second pointer (that end with 1) point to the new one
		updateDirectoryPointers(hashIndex, bucketOffset, targetBucket.localDepth, insertedBucketOffset);
		// int secondPointerOffset = hashIndex;
		// if (hashIndex % 2 == 0) secondPointerOffset += 1;
		// directory.bucketsOffsets[hashIndex]
		// writeDirectoryOffset(secondPointerOffset, insertedBucketOffset);
		
		insertItem(item); // try to insert again, it will be inserted or it will be splitted again
		return 1;
	}
}

int ExtendibleHashing::searchItem(struct DataItem *item, int *count)
{
}

int ExtendibleHashing::DisplayFile()
{
	Directory d =  readWholeDirectory();
	int gd = d.globalDepth;
	int numPointers = pow(2, gd);
	cout << endl<<"global depth = " << gd << " with " << numPointers << " buckets" << endl;
	for(int i = 0; i < numPointers; i++){
		Bucket currentBucket = readFromBuckets(d.bucketsOffsets[i]);
		for (int j = 0; j < RECORDSPERBUCKET; j++){
			DataItem currentItem = currentBucket.dataItem[j];
			if (currentItem.valid) 
			cout << "bucket["<<i<<"] :"<<"(" << currentItem.key << ", " <<  currentItem.data << ")" << endl;
		}
		cout << endl;
	}
}

int ExtendibleHashing::deleteOffset(int Offset)
{
	
}

int ExtendibleHashing::deleteKey(int key)
{
	/*int hashIndex = hashCode(key, directory.globalDepth);
	int bucketPointer = directory.bucketsOffsets[hashIndex];
	Bucket b = readFromBuckets(bucketPointer);
	for (int i = 0; i< RECORDSPERBUCKET; i++){
		auto dataItem = b.dataItem[i];
		if (dataItem.valid && dataItem.key == key){
			// we found the object, delete and update
			b.dataItem[i].valid = 0;
			updateBucket(bucketPointer, b);

			if (isBucketEmpty(b)){
				// merge and shrink if required
				
			}
		}

	}*/
	return -1;
}