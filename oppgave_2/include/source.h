#ifndef SOURCE_H
#define SOURCE_H

typedef struct _NUMBERTYPES{
	int number;
	bool isFibonacci;
    bool isPrime;
    bool isSquare;
    bool isPerfect;
    bool isAbundant;
    bool isDeficient;
    bool isOdd;
}NUMBERTYPES;

// Funksjoner
void displayMenu();
void resetFile(FILE *fp);
bool isFibonacci(int n);
bool isPrime(int n);
bool isSquareNumber(int n);
bool isPerfectNumber(int n);
bool isAbundantNumber(int n);
bool isDeficientNumber(int n);
bool isOdd(int n);

#endif //SOURCE_H
