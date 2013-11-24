/* 
 * File:   main.cpp
 * Author: palmyman
 *
 * Created on November 16, 2013, 10:24 PM
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string.h>

using namespace std;

struct TScore {
    unsigned width, height, x, y, in;
};

class CMatrix {
public:
    CMatrix();
    ~CMatrix();
    friend ostream& operator <<(ostream&, const CMatrix&);
    bool addRow(string);
    bool read();
    bool reallocRows();
    bool reallocCols();
    void evalSubscores();
    unsigned getScore();
private:
    static const unsigned defaultSize = 10;
    unsigned rows, cols;
    unsigned maxHeight, maxWidth;
    unsigned ** data;
    unsigned totalScore;
    TScore * subscores;
    unsigned subscoreRecords;
    unsigned targetDiff;
};

CMatrix::CMatrix() {
    maxHeight = defaultSize;
    maxWidth = defaultSize;
    cols = 0;
    rows = 0;
    totalScore = 0;
    targetDiff = 0;
    data = new unsigned*[maxHeight];
    for (int i = 0; i < maxHeight; i++) {
        data[i] = new unsigned[maxWidth];
    }
    subscores = NULL;
    subscoreRecords = 0;
}

CMatrix::~CMatrix() {
    for (int i = 0; i < maxHeight; i++) {
        delete[] data[i];
    }
    delete[] data;
    data = NULL;
    maxHeight = 0;
    maxWidth = 0;
    rows = 0;
    cols = 0;
    delete [] subscores;
    subscoreRecords = 0;
}

ostream& operator <<(ostream& out, const CMatrix& matrix) {
//MATRIX PRINT
//    if (!matrix.cols || !matrix.rows) {
//        out << "Empty matrix" << endl;
//        return out;
//    }
//    for (int i = 0; i < matrix.rows; i++) {
//        for (int j = 0; j < matrix.cols; j++) {
//            out << setw(4) << matrix.data[i][j];
//        }
//        out << endl;
//    }
    
    for (int i = 0; i < matrix.subscoreRecords; i++) {
        unsigned localDiff = (unsigned) abs((long int)matrix.subscores[i].in - (matrix.totalScore - matrix.subscores[i].in));
        if (localDiff == matrix.targetDiff)
        out << matrix.subscores[i].width << " x " << matrix.subscores[i].height 
            << " @ (" << matrix.subscores[i].x << ", " << matrix.subscores[i].y
            << "): " << matrix.subscores[i].in << ", " << matrix.totalScore - matrix.subscores[i].in
            << endl;        
    }
    return out;
}

bool CMatrix::reallocCols() {
    unsigned ** oldData = data;
    maxWidth *= 2;
    data = new unsigned*[maxHeight];
    for (int i = 0; i < maxHeight; i++) {
        data[i] = new unsigned[maxWidth];
    }
    //copy old data
    for (int i = 0; i < cols; i++) {
        data[0][i] = oldData[0][i];
    }
    //free old memory
    for (int i = 0; i < maxHeight; i++) {
        delete[] oldData[i];
    }
    delete[] oldData;
    return 1;
}

bool CMatrix::reallocRows() {
    unsigned ** oldData = data;
    maxHeight *= 2;
    data = new unsigned*[maxHeight];
    for (int i = 0; i < maxHeight; i++) {
        if (i < maxHeight / 2)
            data[i] = oldData[i];
        else
            data[i] = new unsigned[maxWidth];
    }
    delete[] oldData;
    return 1;
}

bool CMatrix::addRow(string row) {
    unsigned x = 0;
    size_t current, next = -1;
    string colData;
    do {
        current = next + 1;
        next = row.find_first_of(",", current);
        colData = row.substr(current, next - current);
        if (rows && x > cols) return 0;
        if (!rows && x == maxWidth) {
            reallocCols();
        }
        if (!rows) cols++;
        data[rows][x] = atoi(colData.c_str());
        totalScore += data[rows][x];
        x++;
    } while (next != string::npos);
    if (rows && x != cols) return 0;
    rows++;
    return 1;
}

bool CMatrix::read() {
    string row;
    while (!cin.eof()) {
        getline(cin, row);
        if (rows == maxHeight) reallocRows();
        if (!addRow(row)) return 0;
    }
    return 1;
}

void CMatrix::evalSubscores() {
    targetDiff = totalScore - (totalScore - data[0][0]);
    subscores = new TScore[rows * rows * cols * cols];
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            for (int height = 1; y + height - 1 < rows; height++) {
                int rowScore = 0;
                for (int width = 1; x + width - 1 < cols; width++) {
                    unsigned lastRow;
                    if(y) lastRow = subscores[subscoreRecords - cols + x].in;
                    else lastRow = 0;
                    rowScore += data[y + height - 1][x + width - 1];
                    subscores[subscoreRecords].x = x;
                    subscores[subscoreRecords].y = y;
                    subscores[subscoreRecords].height = height;
                    subscores[subscoreRecords].width = width;
                    subscores[subscoreRecords].in = rowScore + lastRow;
                    unsigned localDiff = (unsigned) abs((long int)subscores[subscoreRecords].in - (totalScore - subscores[subscoreRecords].in));
                    if(localDiff < targetDiff)
                        targetDiff = localDiff;                    
                    cout << localDiff << endl;
                    subscoreRecords++;
                }
            }
        }
    }
}

unsigned CMatrix::getScore() {
    return totalScore;
}

/*
 * 
 */
int main(int argc, char** argv) {
    CMatrix testMatrix;
    testMatrix.read();
    testMatrix.evalSubscores();

    cout << testMatrix << endl;
    return 0;
}

