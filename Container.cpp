#include "Container.h"

ostream& operator<<(ostream& os, DoubleContainer& a) {
    for (int i = 0; i < a.currentSize; i++) {
        cout << a.content[i] << endl;
    }
    return os;
}

DoubleContainer DoubleContainer::operator+(const DoubleContainer& b) {
    DoubleContainer c;
    int s;
    bool isTrue = false;
    if (currentSize <= b.currentSize) {
        c.currentSize = b.currentSize;
        s = currentSize;
        isTrue = true;
    } 
    else {
        c.currentSize = currentSize;
        s = b.currentSize;
    }
    for (int i = 0; i < s; i++){
        c.content[i] = content[i] + b.content[i];
    }
    if (isTrue) {
        for (int i = s; i < b.currentSize; i++) {
            c.content[i] = b.content[i];
        }
    } 
    else {
        for (int i = s; i < currentSize; i++) {
            c.content[i] = content[i];
        }
    }
    c.Size();
    return c;
}

DoubleContainer DoubleContainer::operator-(const DoubleContainer& dc) {
    DoubleContainer f;
    int s;
    bool isTrue = false;
    if (currentSize <= dc.currentSize) {
        f.currentSize = dc.currentSize;
        s = currentSize;
        isTrue = true;
    } 
    else {
        f.currentSize = currentSize;
        s = dc.currentSize;
    }
    for (int i = 0; i < s; i++){
        f.content[i] = content[i] - dc.content[i];
    }

    if (isTrue) {
        for (int i = s; i < currentSize; i++) {
            f.content[i] = dc.content[i];
        }
    } 
    else {
        for (int i = s; i < currentSize; i++) {
            f.content[i] = content[i];
        }
    }
    f.Size();
    return f;
}

double DoubleContainer::operator*(const DoubleContainer& g) {
    double h = 0;
    int s;
    bool isTrue = true;
    if (currentSize <= g.currentSize) {
        s = currentSize;
    } 
    else {
        s = g.currentSize;
        isTrue = false;
    }
    for (int i = 0; i < s; i++) {
        h += content[i] * g.content[i];
    }
    if (isTrue) {
        for (int i = s; i < g.currentSize; i++) {
            h += g.content[i];
        }
    } 
    else {
        for (int i = s; i < currentSize; i++) {
            h += content[i];
        }
    }
    return h;
}

void DoubleContainer::operator-() {
    for (int i = 0; i < currentSize; i++) {
        content[i] *= -1;
    }
}

void DoubleContainer::operator*(const double e) {
    for (int i = 0; i < currentSize; i++) {
        content[i] *= e;
    }
}

void DoubleContainer::operator=(const DoubleContainer& j) { 
    delete content;
    maxSize = j.maxSize;
    currentSize = j.currentSize;
    content = new double [currentSize];
    for (int i = 0; i < currentSize; i++) {
        content[i] = j.content[i];
    }
}

DoubleContainer::DoubleContainer() {
    content = new double[5];
    currentSize = 5;
    Size();
}

DoubleContainer::DoubleContainer(const double ac[], int dd) {
    currentSize = dd;
    content = new double[dd];

    for (int i = 0; i < dd; i++) {
        content[i] = ac[i];
    }
    Size();
}

DoubleContainer::DoubleContainer(const DoubleContainer& da) {
    maxSize = da.maxSize;
    currentSize = da.currentSize;
    content = new double[currentSize];
    for (int i = 0; i < currentSize; i++) {
        content[i] = da.content[i];
    }
}

void DoubleContainer::Size() {
    if (currentSize >= maxSize) {
        maxSize = currentSize + 5;
    }
}

void DoubleContainer::Add(double add) {
    currentSize++;
    double *NextContent = new double[currentSize];
    for (int i = 0; i < currentSize - 1; i++) {
        NextContent[i] = content[i];
    }
    delete content;
    content = new double[currentSize];
    for (int i = 0; i < currentSize; i++) {
        content[i] = NextContent[i];
    }
    content[currentSize - 1] = add;
    delete NextContent;
    Size();
}

double DoubleContainer::Remove() {
    double rem;
    currentSize--;
    double *NextContent = new double [currentSize];
    rem = content[currentSize];
    for (int i = 0; i < currentSize; i++) {
        NextContent[i] = content[i];
    }
    delete content;
    content = new double[currentSize];
    for (int i = 0; i < currentSize; i++) {
        content[i] = NextContent[i];
    }
    delete NextContent;
    Size();
    return rem;
}

void DoubleContainer::Clear() {
    delete content;
    currentSize = 0;
    content = new double[currentSize];
    Size();
}

DoubleContainer::~DoubleContainer() {
    delete content;
}