/***main.cpp***/
#include <iostream>
#include <stdio.h>
using namespace std;

#include "mempool.h"

class ActualClass{
    static int count;
    int No;

public:
    ActualClass(){
        No=count;
        count++;
    }

    void print(){
        cout<<this<<": ";
        cout<<"the "<<No<<" object"<<endl;
    }

    void* operator new(size_t size);
    void operator delete(void* p);
};

//定义内存池对象
MemPool<sizeof(ActualClass),2> mp;

void* ActualClass::operator new(size_t size){
    return mp.malloc();
}

void ActualClass::operator delete(void* p){
    mp.free(p);
}

int ActualClass::count=0;

int main(){
    ActualClass* p1=NULL;
    p1=new ActualClass;
    p1->print();

    ActualClass* p2=new ActualClass;
    p2->print();
    delete p1;

    p1=new ActualClass;
    p1->print();

    ActualClass* p3=new ActualClass;
    p3->print();

    delete p1;
    delete p2;
    delete p3;

    getchar();
}
/***end main.cpp***/
