#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 100

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct RecordType* record;
	struct HashType* next;
};

// Compute the hash function
int hash(int x)
{
	return (x % TABLE_SIZE);
}

struct HashType* newNode(struct RecordType* record)
{
	struct HashType* node = (struct HashType*)malloc(sizeof(struct HashType));
	node->record = record;
	node->next = NULL;
	return node;
}

void insert(struct RecordType* record, struct HashType** hashtable)
{
	int index = hash(record->id);
	
	//Insertion code

	//If the hashtable's list doesn't exist at that location, make it the node
	if (hashtable[index] == NULL)
	{
		hashtable[index] = newNode(record);
		return;
	}

	//Otherwise, we're gonna add to the front
	struct HashType* temp = hashtable[index];
	hashtable[index] = newNode(record);
	hashtable[index]->next = temp;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType **pHashArray, int hashSz)
{
	int i;

	for (i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all
		struct HashType* curr = pHashArray[i];
		while (curr != NULL)
		{
			printf("\t%d %c %d\n", curr->record->id, curr->record->name, curr->record->order);
			struct HashType* toDestroy = curr;
			curr = curr->next;
			free(toDestroy);
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input_lab_9.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
	struct HashType** hashtable = (struct HashType**)calloc(TABLE_SIZE, sizeof(struct HashType*));
	for (int i = 0; i < recordSz; i++)
	{
		insert(pRecords + i, hashtable);
	}
	displayRecordsInHash(hashtable, TABLE_SIZE);
	free(pRecords);
	free(hashtable);
}