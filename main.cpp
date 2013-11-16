/* 
 * File:   main.cpp
 * Author: palmyman
 *
 * Created on November 16, 2013, 10:24 PM
 */

#include <cstdlib>
#include <iostream>

using namespace std;

class CMatrix {
public:
    CMatrix();
    ~CMatrix();
    friend ostream& operator <<(ostream&, const CMatrix&);
    bool addRow(string);
private:
    static const unsigned defaultSize = 10;
    unsigned rows, cols;
    unsigned maxHeight, maxWidth;
    unsigned ** data;
};

CMatrix::CMatrix() {
    maxHeight = defaultSize;
    maxWidth = defaultSize;
    cols = 0;
    rows = 0;
    data = new unsigned*[maxHeight];
    for (int i = 0; i < maxHeight; i++) {
        data[i] = new unsigned[maxWidth];
    }
}

CMatrix::~CMatrix() {
    for (int i = 0; i < maxHeight; i++) {
        delete data[i];
    }
    delete[] data;
    data = NULL;
    maxHeight = 0;
    maxWidth = 0;
    rows = 0;
    cols = 0;
}

ostream& operator <<(ostream& out, const CMatrix& matrix) {
    for (int i = 0; i < matrix.rows; i++) {
        for (int j = 0; j < matrix.cols; j++) {
            out << matrix.data[i][j] << " ";
        }
        out << endl;
    }
    return out;
}

bool CMatrix::addRow(string line) {
    unsigned x = 0;
    size_t current, next = -1;
    string colData;
    do {        
        current = next + 1;
        next = line.find_first_of(" ", current);
        colData = line.substr(current, next - current);
        data[rows][x] = atoi(colData.c_str());
        x++;        
    } while (next != string::npos);
    if(!rows) cols = x;
    else if(x != cols) return 0;
    rows++;
    return 1;
}

/*
 * 
 */
int main(int argc, char** argv) {
    CMatrix testMatrix;
    testMatrix.addRow("1 2 3 4\n");    
    
    cout << testMatrix << endl;
    return 0;
}

