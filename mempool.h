template<int ObjectSize,int NumofObjects=3> class MemPool{
private:
    const int MemBlockSize; //Size of a memory block
    const int ItemSize;    //Size of a memory node

    //Free node
    struct FreeNode{
        FreeNode* pNext;
        char data[ObjectSize];
    };

   	//One memory block can include numbers of nodes.
    struct MemBlock{
        MemBlock *pNext;
        FreeNode data[NumofObjects];
    };

    FreeNode* freeNodeHeader;
    MemBlock* memBlockHeader;

public:
    MemPool():ItemSize(ObjectSize+sizeof(FreeNode*)),MemBlockSize(sizeof(MemBlock*)+NumofObjects*(ObjectSize+sizeof(FreeNode*))){
        freeNodeHeader=NULL;
        memBlockHeader=NULL;
    }

    ~MemPool(){
        MemBlock* ptr;
        while(memBlockHeader){
            ptr=memBlockHeader->pNext;
            delete memBlockHeader;
            memBlockHeader=ptr;
        }
    }
    void* malloc();
    void free(void*);
};

//Allocating a free node
template<int ObjectSize,int NumofObjects> 
void* MemPool<ObjectSize,NumofObjects>::malloc(){
    if(freeNodeHeader==NULL){      //No more free nodes
        MemBlock* newBlock=new MemBlock;
        newBlock->data[0].pNext=NULL; //Initiate a new memory block.
        for(int i=1; i<NumofObjects;++i)
            newBlock->data[i].pNext=&newBlock->data[i-1];
        freeNodeHeader=&newBlock->data[NumofObjects-1];
        newBlock->pNext=memBlockHeader;
    }
    //Return the first free node in the memory block.
    void* freeNode=freeNodeHeader;
    freeNodeHeader=freeNodeHeader->pNext;
    return freeNode;
}

//Free a node
template<int ObjectSize,int NumofObjects> 
void MemPool<ObjectSize,NumofObjects>::free(void* p){
    FreeNode* pNode=(FreeNode*)p;
    pNode->pNext=freeNodeHeader;//Insert node
    freeNodeHeader=pNode;
}

