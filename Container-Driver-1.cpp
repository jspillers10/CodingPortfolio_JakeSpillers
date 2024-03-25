// In this assignment, I combined DMA and operator overloading to make a container
// that behaves like a vector (in the mathematical sense) of arbitrary size.
// I created a class called DoubleContainer that has a dynamically allocated array of double values called "content".
// My DoubleContainer also has two int values that stores the maximum size of the container as well as the current number of doubles in the container.

#include <iostream>
#include <ostream>
#include <cstring>
#include "Container2.h"
using namespace std;

int main() {
    char input;
    char input2;
    double num;
    double userNum1;
    double userNum2;
    double userNum3;
    const int s = 3;
    double ContentA[s] = {3.2, 4, 4.5};
    double ContentB[s + 2] = {4.5, 5, 6, 6.7, 8};
    DoubleContainer Cont1(ContentA, s);
    DoubleContainer Cont2(ContentB, s + 2);
    DoubleContainer Cont3 = Cont1 + Cont2;
    DoubleContainer Cont4 = Cont3;

    cout << "Double Containers: " << endl;
    cout << "1: " << Cont1 << endl;
    cout << "2: " << Cont3 << endl;
    cout << "3: " << Cont4 << endl << endl;

    cout << "What number do you want to add to Container 1?: ";
    cin >> userNum1;
    Cont1.Add(userNum1);

    cout << "What number do you want to add to Container 2?: ";
    cin >> userNum2;
    Cont3.Add(userNum2);
    
    cout << "What number do you want to add to Container 3?: ";
    cin >> userNum3;
    Cont4.Add(userNum3);

    cout << "Double Containers: " << endl;
    cout << "1: " << Cont1 << endl;
    cout << "2: " << Cont3 << endl;
    cout << "3: " << Cont4 << endl << endl;

    cout << "Which double container would you like to remove from (1, 2, or 3):" << endl;
    cin >> input;
    if (input == '1') {
        Cont1.Remove();
    } 
    else if (input == '2') {
        Cont3.Remove();
    } 
    else if (input == '3') {
        Cont4.Remove();
    }

    cout << "Double Containers: " << endl;
    cout << "1: " << Cont1 << endl;
    cout << "2: " << Cont3 << endl;
    cout << "3: " << Cont4 << endl << endl;

    cout << "What number do you want to multiply to Container 1?: ";
    cin >> userNum1;
    Cont1 * userNum1;

    cout << "What number do you want to multiply to Container 2?: ";
    cin >> userNum2;
    Cont3 * userNum2;

    cout << "What number do you want to multiplu to Container 3?: ";
    cin >> userNum3;
    Cont4 * userNum3;

    cout << "Double Containers: " << endl;
    cout << "1: " << Cont1 << endl;
    cout << "2: " << Cont3 << endl;
    cout << "3: " << Cont4 << endl << endl;

    cout << "Which double container would you like to clear:" << endl;
    cin >> input2;
    if (input2 == '1') {
        Cont1.Clear();
    } 
    else if (input2 == '2') {
        Cont3.Clear();
    } 
    else if (input2 == '3') {
        Cont4.Clear();
    }
}
   
