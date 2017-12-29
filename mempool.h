/***mempool.h***/
template<int ObjectSize,int NumofObjects=20> class MemPool{
private:
    const int MemBlockSize; //每个内存块的大小
    const int ItemSize;    //每个内存节点的大小

    //空闲节点结构体
    struct FreeNode{
        FreeNode* pNext;
        char data[ObjectSize];
    };

    //内存块结构体
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

//分配空闲的节点
template<int ObjectSize,int NumofObjects> 
void* MemPool<ObjectSize,NumofObjects>::malloc(){
    if(freeNodeHeader==NULL){      //无空闲节点
        MemBlock* newBlock=new MemBlock;
        newBlock->data[0].pNext=NULL; //设置内存块的第一个节点为空闲节点链表的最后一个
        for(int i=1; i<NumofObjects;++i)
            newBlock->data[i].pNext=&newBlock->data[i-1];
        freeNodeHeader=&newBlock->data[NumofObjects-1];
        newBlock->pNext=memBlockHeader;
    }
    //返回空节点闲链表的第一个节点
    void* freeNode=freeNodeHeader;
    freeNodeHeader=freeNodeHeader->pNext;
    return freeNode;
}

//释放已经分配的节点
template<int ObjectSize,int NumofObjects> 
void MemPool<ObjectSize,NumofObjects>::free(void* p){
    FreeNode* pNode=(FreeNode*)p;
    pNode->pNext=freeNodeHeader;//将释放的节点插入空闲节点头部
    freeNodeHeader=pNode;
}
/***end mempool.h***/

