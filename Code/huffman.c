#include "huffman.h"
#include "queue.h"

//---------------------------------------------------------------------------------------- 
// Creates a priority queue.
// Create a leaf node for each symbol from frequency table and add it to the priority queue
// Input:
//        pArrayPtrToSymbols - pointer to frequency table
//        size_arraySymbols  - size of frequency table
// Output:
//        pointer to priority queue
//---------------------------------------------------------------------------------------- 
pPriorityQueue createAndBuildTreeArray(pSymbol pArrayPtrToSymbols, int size_arraySymbols)
{
	pPriorityQueue pq = Init_PQ(size_arraySymbols);
    int i;
    for (i = 0; i < size_arraySymbols; i++)
        Enqueue(pq, newNode(pArrayPtrToSymbols[i].value, pArrayPtrToSymbols[i].counter), pArrayPtrToSymbols[i].counter);
	return pq;
}
//---------------------------------------------------------------------------------------- 
// Creates a priority queue and inserts all character of frequency table in tree array.
// Input:
//        pArrayPtrToSymbols - pointer to frequency table
//        size_arraySymbols  - size of frequency table
// Output:
//        pointer to huufman tree root
//---------------------------------------------------------------------------------------- 
//---------------------------------------------------------------------------------------- 
pTreeNode BuildTree(pSymbol pArrayPtrToSymbols, int size_arraySymbols)
{
	pTreeNode min1, min2, top;
	// Step 1: Create a leaf node for each symbol and add it to the priority queue
	pPriorityQueue  priorityQueue = createAndBuildTreeArray(pArrayPtrToSymbols, size_arraySymbols);
     
    // While there is more than one node in the priority queue
    while ( priorityQueue->size > 1)
    {
        // Step 2: Remove the two nodes of lowest probability from the priority queue.( Extract the two minimum freq items from priority queue )
        min1  = Dequeue(priorityQueue);
        min2 = Dequeue(priorityQueue); 
 
        // Step 3:  Create a new internal node ( '\0' is a special value for internal nodes, not used ) with frequency equal to the
        // sum of the two nodes frequencies. Make the two extracted node as
        // left and right children of this new node. 
		//  Add this node to the priority queue.
        top = newNode('\0', min1->freq + min2->freq);
        top->left = min1;
        top->right = min2;
		Enqueue(priorityQueue, top, top->freq);
    } 
    // Step 4: The remaining node is the root node and the tree is complete.
    top = Dequeue(priorityQueue); 
	free(priorityQueue);
	return top;
}

//------------------------------  M E T H O D S  for  C O M P R E S S ----------------------------------------------- 
//
//  Input:
//          pArraySymbols   - pointer to pointer to first synmbol in array
//---------------------------------------------------------------------------------------- 
void insert_to_arraySymbols(pSymbol* pArraySymbols, int* pSize_arraySymbols, BYTE value)
{
	if ( *pArraySymbols == NULL )
	{   //  If frequency table is empty
		*pArraySymbols = (pSymbol) malloc(sizeof(Symbol));    //  Allocation memory for array with 1 node
		(*pArraySymbols)->value = value;
		(*pArraySymbols)->counter = 1;
		*pSize_arraySymbols = 1;
	}
	else
	{
		pSymbol pNode = *pArraySymbols;
		// Loop for search for new value in table
		while( pNode < *pArraySymbols + *pSize_arraySymbols)
		{	
			if ( pNode->value == value )
			{   //  Found
				pNode->counter++;
				return;
			}
  		    pNode++;              //  To next element
		}
		// Add new value to frequency table
        *pArraySymbols = (pSymbol) realloc(*pArraySymbols, ++(*pSize_arraySymbols)*sizeof(Symbol));    //  Reallocation memory for array 
		(*pArraySymbols+*pSize_arraySymbols-1)->value = value;
		(*pArraySymbols+*pSize_arraySymbols-1)->counter = 1;
	}
}
//---------------------------------------------------------------------------------------- 
//  Search code for value in table symbol codes
// Input :
//        value              - search value in table
//        arrayPtrToSymbols  - pointer to table
//        size_arraySymbols  - size of table
// Output:
//        pointer to element in table or NULL if not found
//---------------------------------------------------------------------------------------- 
pSymbol getElementByValue(BYTE value, pSymbol arrayPtrToSymbols, int size_arraySymbols )
{
	int i;
	for(i=0; i<size_arraySymbols; i++)
	{
		if ( arrayPtrToSymbols[i].value == value )
			return arrayPtrToSymbols+i;
	}
	return NULL;
}
//---------------------------------------------------------------------------------------- 
// Fill array of huffman codes from the root of Huffman Tree.
// Input :
//        root       - current node in Huffman Tree
//        pArrCodes  - pointer to array of huffman codes : value and string of code
//        code       - pointer to current code
// Output:
//        None 
//---------------------------------------------------------------------------------------- 
void fillCodes(pTreeNode root, pSymbol arrayPtrToSymbols, int size_arraySymbols, char* code)
{
	char* codeHelp ;
	int len ;
	
    // Assign 0 to left edge and recur
    if (root->left)
    {	
		codeHelp = strdup(code);
		len = strlen(codeHelp) + 2;
        codeHelp = (char*) realloc(codeHelp, len);
		codeHelp[len-2] = '0';
		codeHelp[len-1] = '\0';
        fillCodes(root->left, arrayPtrToSymbols, size_arraySymbols, codeHelp);
    }
 
    // Assign 1 to right edge and recur
    if (root->right)
    {
		codeHelp = strdup(code);
		len = strlen(codeHelp) + 2;
        codeHelp = (char*) realloc(codeHelp, len);
		codeHelp[len-2] = '1';
		codeHelp[len-1] = '\0';
        fillCodes(root->right, arrayPtrToSymbols, size_arraySymbols, codeHelp);
    }
 
    // If this is a leaf node, then it contains one of the input
    // characters, print the character and its code from arr[]
    if (isLeaf(root))
    {
		pSymbol p = getElementByValue(root->value, arrayPtrToSymbols, size_arraySymbols);
		p->code = strdup(code);
    }
}

//---------------------------------------------------------------------------------------- 
//  Write to ouput file :
//  Header :  size of frequency table (int)  completeBits (BYTE) frequency table ( element : BYTE and int )
//  Compress bytes
//
void writeData(FILE* fOut,  pSymbol arrayPtrToSymbols, int size_arraySymbols, int completeBits, BYTE* buf, int size)
{
	int i;
    //----------------  Write Header ---------------------------------------	
	fwrite( &size_arraySymbols, sizeof(int), 1, fOut);                       // size of frequency table ( number elements. element : BYTE and int )
	fwrite( &completeBits, sizeof(BYTE), 1, fOut);                           // quntity bits in last byte
	// Write frequency table
	for (i = 0; i < size_arraySymbols; i++)
	{
		fwrite( &arrayPtrToSymbols[i].value, sizeof(BYTE), 1, fOut);        // value of character
		fwrite( &arrayPtrToSymbols[i].counter, sizeof(int), 1, fOut);       // frequency of character
	}
	//-------------------------------------------------------------------
	//  Write content of compress bytes
	for(i=0; i< size; i++)
		fwrite( buf + i, sizeof(BYTE), 1, fOut);
}
//---------------------------------------------------------------------------------------- 
void writeToFile(char* filenameIn, pSymbol arrayPtrToSymbols, int size_arraySymbols )
{
	BYTE* buf = NULL;                        // pointer to dynamic array of compress bytes
	int   size=0;                            // size of dynamic array of compress bytes
	BYTE  value;
	BYTE  byte;
	char* code;
	char  index;
	FILE* fIn, *fOut;
    char* fnameOut = strdup(filenameIn);     //  Build  filename for output compress file
	char* pch = strrchr(fnameOut,'.');
	strcpy(pch+1, "mip");
	fIn = fopen(filenameIn, "rb");
	fOut = fopen(fnameOut,"wb");
	if ( fOut == NULL )
	{
    	printf("Error :  %s ( open output zip file %s ) \n", sys_errlist[errno], fnameOut);
		getch();
		return;
	}
	
	//  Create dynamic array of compress bytes
	byte = 0;
	index = 7;
	while ( fread(&value, sizeof(BYTE), 1, fIn) )
	{
		pSymbol p = getElementByValue(value, arrayPtrToSymbols, size_arraySymbols);
		code = p->code;
		while ( code && *code )
		{
			if ( *code == '1' ) 
				byte |= 1<<index;                 //  1<<index  -  mask for ON '1' in place index
			index--;
			if ( index == -1 )
			{
				buf = (BYTE*) realloc(buf, ++size);
				buf[size-1] = byte;
				byte = 0;
				index = 7;
			}
			++code;
		}
    } 

	index = 8 - (index+1);    // realy size of last byte
	if ( index > 0 )
	{
		buf = (BYTE*) realloc(buf, ++size);
		buf[size-1] = byte;
	}
	else
		index = 8;
	// Write to file
	writeData(fOut, arrayPtrToSymbols, size_arraySymbols, index, buf, size);    
	fclose(fOut);
	fclose(fIn);
	free(buf);
}
//---------------------------------------------------------------------------------------- 
void compress(char* filename)
{
	pTreeNode root;
	BYTE value;
	pSymbol  arrayPtrToSymbols = NULL;       // pointer to first symbol in array of symbols
	int size_arraySymbols = 0;               // size of array of symbols
    FILE* fIn = fopen(filename, "rb");

	if ( fIn == NULL)
	{
		printf("Error :  %s ( open source file %s ) \n", sys_errlist[errno], filename);
		getch();
		exit(-1);
	}
	//  Build frequency table
	while ( fread(&value, sizeof(BYTE), 1, fIn) )
	{
		insert_to_arraySymbols(&arrayPtrToSymbols, &size_arraySymbols, value);
	}
	fclose(fIn);
	// Build tree
	root = BuildTree(arrayPtrToSymbols, size_arraySymbols);
	//  Fill binary codes
	fillCodes(root, arrayPtrToSymbols, size_arraySymbols, "");
	writeToFile(filename, arrayPtrToSymbols, size_arraySymbols);
	free(arrayPtrToSymbols);
	freeTree (root);
}
//------------------------------  D E C O M P R E S S ----------------------------------------------- 
//--------------------------------------------------------------------------------------------------- 
void decompress(char* filenameIn)
{
	pSymbol pArraySymbols = NULL;                   // pointer to first symbol in array of symbols
	int size_arraySymbols;                          // size of array of symbols
	FILE* fIn, *fOut; 
	int bufSize, headerSize;
	int i, j, n;
	BYTE value;
	int freq;
	pTreeNode root, rootStart;
	BYTE curBit, completeBits;
	char* filenameOut = strdup(filenameIn);
	char * pch = strrchr(filenameOut,'.');
	strcpy(pch+1, "dat");

	fIn = fopen(filenameIn, "rb");
	if ( fIn == NULL)
	{
		printf("Error :  %s ( open source file %s ) \n", sys_errlist[errno], filenameIn);
		getch();
		exit(-1);
	}
	fOut = fopen(filenameOut,"wb");
	if ( fOut == NULL )
	{
		printf("Error :  %s ( open output file %s ) \n", sys_errlist[errno], filenameOut);
		getch();
		return;
	}

	//  Read Header
	//  read  realy size of frequency table in bytes
	fread(&size_arraySymbols, sizeof(int), 1, fIn);
	//  read   completeBits
	fread( &completeBits, sizeof(BYTE), 1, fIn);                               
	//  Read frequency table	
	pArraySymbols = (pSymbol) malloc(size_arraySymbols*sizeof(Symbol)); 
	for(i=0; i<size_arraySymbols; i++)
	{
	   fread(&value, sizeof(BYTE), 1, fIn); 
	   fread(&freq , sizeof(int), 1, fIn); 
	   pArraySymbols[i].value = value;
	   pArraySymbols[i].counter = freq;
	}

	// Build tree
	rootStart = root = BuildTree(pArraySymbols, size_arraySymbols);

	//  Read compressed bits
	fseek(fIn, 0, SEEK_END);
	bufSize = ftell(fIn);                           // size of file
	headerSize = sizeof(int) + sizeof(BYTE) + size_arraySymbols*(sizeof(BYTE)+ sizeof(int));
	bufSize -= headerSize;
	fseek(fIn, -bufSize, SEEK_END);
	for(j=0; j<bufSize; j++)
	{
	    fread(&value, sizeof(BYTE), 1, fIn);
		n =  j == bufSize - 1  ? completeBits : sizeof(BYTE)*8;
		for(i=0; i<n; i++)
		{
		   curBit = (value & 0x80) >> 7;
		   value <<= 1;
		   if ( isLeaf(root) )
		   {
				fwrite(&root->value, sizeof(BYTE), 1, fOut);
				root = rootStart;
		   }
           if ( curBit == 0 )
               root =root->left;
		   else
               root =root->right;
		}
		if ( j == bufSize - 1 )
			fwrite(&root->value, sizeof(BYTE), 1, fOut);
	}
	fclose(fOut);
	fclose(fIn);
	free(pArraySymbols);
	freeTree (rootStart);
}
//---------------------------------------------------------------------------------------- 
void usage()
{
	puts("Usage:   huffman <-c|-d> <filename>\n");
}

//--------------------------------   M A I N --------------------------------------------- 
int main(int argc, char** argv)
{  
	  if ( argc != 3 )
		  usage();
	  else
	  {
		   if( strcmp(*(argv+1),"-c") == 0 )
			   compress(*(argv+2));
		   else
		   {
			   if( strcmp(*(argv+1),"-d") == 0 )
			       decompress(*(argv+2));
			   else
                   usage();
		   }
	  }
}

