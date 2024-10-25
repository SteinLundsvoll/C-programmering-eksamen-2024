#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "source.h"

int main(int argc, char**argv[]){
	FILE *fp;
	NUMBERTYPES numTypes = {0};
	int n = numTypes.number;
	char inputBuffer[512];
	int choice = 0;
	
	resetFile(fp); // Fjerner innholdet 
	
	while(choice != 2){
	displayMenu(); // Viser tekst for hovedmeny
		
		choice = getchar() - '0'; // Brukers input
		while(getchar() != '\n'); // Fjerner newline
		
		switch(choice){
			case 1:
				printf("Type a number\n");
				
				fgets(inputBuffer, sizeof(inputBuffer), stdin); // Brukers input
				inputBuffer[strcspn(inputBuffer, "\n")] = '\0'; // Fjerner newline
				n = atoi(inputBuffer); // Konverterer til int
				
				// Hvis brukeren skriver noe annet enn et tall (ignorerer 0)
				if (n == 0 && inputBuffer[0] != '0') {
                    printf("Invalid input. Please enter a valid number.\n");
                    break;
                }
				
				// Lagre tallet (n) i strukturmedlemmene
				numTypes.number = n;
                numTypes.isFibonacci = isFibonacci(n);
                numTypes.isPrime = isPrime(n);
                numTypes.isSquare = isSquareNumber(n);
                numTypes.isPerfect = isPerfectNumber(n);
                numTypes.isAbundant = isAbundantNumber(n);
                numTypes.isDeficient = isDeficientNumber(n);
                numTypes.isOdd = isOdd(n);
				
				// Hvis funksjonen(e) returnerer true 
				if(numTypes.isFibonacci) printf("%d is a Fibonacci number\n", numTypes.number);
                if(numTypes.isPrime) printf("%d is a prime number\n", numTypes.number);
                if(numTypes.isSquare) printf("%d is a square number\n", numTypes.number);
                if(numTypes.isPerfect) printf("%d is a perfect number\n", numTypes.number);
                if(numTypes.isAbundant) printf("%d is an abundant number\n", numTypes.number);
                if(numTypes.isDeficient) printf("%d is a deficient number\n", numTypes.number);
                if(numTypes.isOdd) printf("%d is a odd number\n", numTypes.number);
                
                
				fp = fopen("binary.bin", "ab"); // "ab" for å ikke slette innhold mens programmet kjøer
				// Sjekker om filens åpnes riktig
				if(fp == NULL){
					printf("Failed to open file...\n");
					exit(EXIT_FAILURE);
				}
				
				// Skriver strukturen binaert til binary.bin filen
				if(fwrite(&numTypes, sizeof(NUMBERTYPES), 1, fp) != 1){
					printf("An error occured...\n");
					fclose(fp);
					return 1; // Fwrite error håndtering
				}
				
				fclose(fp); // Lukker filen
				break;
			case 2:
				printf("Exiting program...\n");
				exit(0); // Avslutter programmet uten feilkode
				break;
			default:
				printf("Choose between [1 or 2]\n");
		}
	}
	return 0;
}

void displayMenu(){
	printf("Choose one of the following options [1 - 2]\n");
    printf("1. Type a number and find its properties\n");
    printf("2. Quit\n");
}

void resetFile(FILE *fp){
	fp = fopen("binary.bin", "wb"); // wb 
	// Sjekker om filens åpnes riktig
	if(fp == NULL){
		printf("Failed to open file...\n");
		exit(EXIT_FAILURE);
	}
	fclose(fp);
}
