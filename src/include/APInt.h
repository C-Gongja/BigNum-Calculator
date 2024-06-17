#ifndef APINT_H
#define APINT_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct APInt
{
	int size;
	uint8_t *bytes;
} APInt;

// Constructive methods
APInt *createAPInt(int size);
void dumpAPInt(APInt *src, FILE *output);

void APIntFromUINT64(APInt *arr[], const char *str, int index);
void APIntFromHex(APInt *arr[], const char *str, int index);
void cloneAPInt(APInt *apInt_arr[], int clone_index, int arr_index);

// Common operations
void addAPInt(APInt *arr[], uint64_t *nums);
void multiplyAPInt(APInt *arr[], const APInt *a, const APInt *b, uint64_t *nums);
void multiplyUINT64(APInt *arr[], uint64_t *nums);
void powerAPInt(APInt *arr[], uint64_t *nums, uint64_t k);
void leftShiftAPInt(APInt *arr[], uint64_t *nums);

// Comparison
int compareAPInt(const APInt *a, const APInt *b);

// Destructor
void destroyAPInt(APInt *apInt);

// // Printing method
// void printAPInt(const APInt *a);

#endif
