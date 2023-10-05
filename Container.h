#include <iostream>
#include <ostream>
#include <cstring>
using namespace std;

class DoubleContainer{

    friend ostream& operator<<(ostream&, DoubleContainer&);

    public:

        DoubleContainer();

        DoubleContainer(const double[], int); 

        DoubleContainer(const DoubleContainer&); 

        ~DoubleContainer(); 

        DoubleContainer operator+(const DoubleContainer&);
        DoubleContainer operator-(const DoubleContainer&);
        
        double operator*(const DoubleContainer&);
        void operator-();
        void operator*(const double);
        void operator=(const DoubleContainer&);

        double Remove();
        void Size();
        void Add(double);
        void Clear();


    private:

        double* content;
        int maxSize;
        int currentSize;
};