#include <iostream>
#include <countingbloomfilter.h>
using std::cout;
using std::endl;


#define CAPACITY 100000
#define ERROR_RATE .05

union data
{
    int index;
    char d;
};

int main()
{
    CountingBloomFilter bloom(CAPACITY,ERROR_RATE);
    data i;
    for(i.index=0;i.index<1000;i.index++)
    {
        bloom.Add(&i.d,4);
        cout<<"Add"<<i.index<<"check "<<bloom.Check(&i.d,4)<<endl;
    }
    for(i.index=0;i.index<2000;i.index++)
    {
        bloom.Add(&i.d,4);
        cout<<"Add"<<i.index<<"check "<<bloom.Check(&i.d,4)<<endl;
    }
    for(i.index=0;i.index<2000;i.index++)
    {
        bloom.Remove(&i.d,4);
        cout<<i.index<<"check "<<bloom.Check(&i.d,4)<<endl;
    }
    cout<<"program ends"<<endl;
}
