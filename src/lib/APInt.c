#include "APInt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// added
#include <ctype.h>
#include <math.h>

#define DEST 0
#define SRC1 1
#define SRC2 2

// Function to create an APInt structure
APInt *createAPInt(int size)
{
	APInt *apInt = malloc(sizeof(APInt));
	if (apInt == NULL)
	{
		// Handle memory allocation failure
		free(apInt);
		exit(0);
	}
	apInt->bytes = calloc(size, sizeof(uint8_t));
	if (apInt->bytes == NULL)
	{
		// Handle memory allocation failure
		free(apInt->bytes);
		free(apInt);
		exit(0);
	}
	apInt->size = size;
	return apInt;
}

void decimalToLittleEndianHex(uint64_t decimal, uint8_t *hexArray, int size)
{
	// printf("decimal: %lu\n", decimal);
	// Convert decimal to hexadecimal
	for (int i = 0; i < size; ++i)
	{
		hexArray[i] = decimal & 0xFF;
		decimal >>= 8;
	}
}

uint8_t hexCharToInt(char c)
{
	if (c >= '0' && c <= '9')
	{
		return c - '0';
	}
	else if (c >= 'a' && c <= 'f')
	{
		return c - 'a' + 10;
	}
	else if (c >= 'A' && c <= 'F')
	{
		return c - 'A' + 10;
	}
	return 0; // Invalid character
}

char *uint64ToString(uint64_t value)
{
	// Assuming the maximum length of a uint64_t decimal string is 20 characters
	char *str = malloc(21 * sizeof(char));
	if (str == NULL)
	{
		// Handle memory allocation failure
		free(str);
		return NULL;
	}

	char *p = str;
	uint64_t temp = value;

	// Handle the case of zero
	if (temp == 0)
	{
		*p++ = '0';
	}
	else
	{
		// Extract digits from the least significant to most significant
		while (temp != 0)
		{
			*p++ = '0' + (temp % 10); // Extract the digit
			temp /= 10;				  // Move to the next digit
		}
	}

	// Null-terminate the string
	*p = '\0';

	// Reverse the string
	char *begin = str;
	char *end = p - 1;
	while (begin < end)
	{
		char temp = *begin;
		*begin++ = *end;
		*end-- = temp;
	}

	return str;
}

void APIntFromUINT64(APInt *arr[], const char *str, int index)
{
	// Allocate memory for the APInt struct
	APInt *apInt = malloc(sizeof(APInt));
	if (apInt == NULL)
	{
		// Handle memory allocation error
		free(apInt);
		exit(0);
	}

	// Determine the size of the hexadecimal string
	size_t len = strlen(str);
	uint64_t value = atoi(str);
	// Calculate the size of the uint64_t array
	apInt->size = (int)((len + 1) / 2);
	apInt->bytes = calloc(apInt->size, sizeof(uint8_t));
	// Allocate memory for the byte array
	decimalToLittleEndianHex(value, apInt->bytes, apInt->size);
	// printf("converted: ");
	// for (int i = 0; i < apInt->size; i++)
	// {
	// 	printf("%02X ", apInt->bytes[i]);
	// }
	// printf("\n");

	if (arr[index] != NULL)
	{
		destroyAPInt(arr[index]);
	}
	arr[index] = apInt;
}

void APIntFromHex(APInt *arr[], const char *str, int index)
{
	APInt *apInt = malloc(sizeof(APInt));
	if (apInt == NULL)
	{
		// Handle memory allocation error
		free(apInt);
		exit(0);
	}

	// Determine the length of the hexadecimal string
	int length = 0;
	while (str[length] != '\0')
	{
		length++;
	}

	// Calculate the size of the byte array
	apInt->size = (length + 1) / 2; // Each byte represents two hexadecimal characters

	// Allocate memory for the byte array
	apInt->bytes = (uint8_t *)calloc(apInt->size, sizeof(uint8_t));
	if (apInt->bytes == NULL)
	{
		// Handle memory allocation error
		free(apInt->bytes);
		free(apInt);
		exit(0);
	}

	// Convert hexadecimal string to bytes
	int i = apInt->size - 1;
	if (length % 2 != 0)
	{
		// If the length is odd, handle the first character separately
		apInt->bytes[i] = hexCharToInt(*str);
		i--;
		str++;
	}
	while (*str != '\0')
	{
		apInt->bytes[i] = (hexCharToInt(*str) << 4) | hexCharToInt(*(str + 1));
		i--;
		str += 2;
	}

	if (arr[index] != NULL)
	{
		destroyAPInt(arr[index]);
	}
	arr[index] = apInt;
}

void cloneAPInt(APInt *apInt_arr[], int clone_index, int arr_index)
{
	APInt *apInt = createAPInt(apInt_arr[clone_index]->size);
	memcpy(apInt->bytes, apInt_arr[clone_index]->bytes, apInt_arr[clone_index]->size);
	if (apInt_arr[arr_index] != NULL)
	{
		destroyAPInt(apInt_arr[arr_index]);
	}
	apInt_arr[arr_index] = apInt;
}

void dumpAPInt(APInt *src, FILE *write)
{
	if (!src)
	{
		return;
	}
	while (src->size > 1 && src->bytes[src->size - 1] == 0)
	{
		src->size--;
	}
	fprintf(write, "0x");
	for (int j = src->size - 1; j >= 0; j--)
	{
		fprintf(write, "%02x", src->bytes[j]); // Print each byte in lowercase hexadecimal format
	}
	fprintf(write, "\n");
}

void addAPInt(APInt *arr[], uint64_t *nums)
{
	int carry = 0;
	int size = arr[nums[1]]->size > arr[nums[2]]->size ? arr[nums[1]]->size : arr[nums[2]]->size;
	APInt *result = malloc(sizeof(APInt));
	if (result == NULL)
	{
		// Handle memory allocation failure
		free(result);
		exit(0);
	}
	result->size = size;
	result->bytes = calloc(size, sizeof(uint8_t));
	if (result == NULL)
	{
		// Handle memory allocation error
		free(result->bytes); // Free bytes first
		free(result);
		exit(0);
	}
	// Perform addition
	for (int i = 0; i < size; i++)
	{
		int sum = (i < arr[nums[1]]->size ? arr[nums[1]]->bytes[i] : 0) + (i < arr[nums[2]]->size ? arr[nums[2]]->bytes[i] : 0) + carry;
		result->bytes[i] = (uint8_t)(sum & 0xFF);
		carry = sum >> 8;
	}
	// If there's a carry left, resize the result
	if (carry > 0)
	{
		result->size = size + 1;
		result->bytes = realloc(result->bytes, (size + 1) * sizeof(uint8_t));
		if (result->bytes == NULL)
		{
			// Handle memory reallocation failure
			free(result->bytes); // Free bytes first
			free(result);
			exit(0);
		}
		result->bytes[size] = carry; // Add the carry to the last position
	}
	else
	{
		result->size = size; // Update size if there's no carry left
	}
	if (arr[nums[0]] != NULL)
	{
		destroyAPInt(arr[nums[0]]);
	}
	arr[nums[0]] = result;

	// return result;
}

void multiplyAPInt(APInt *arr[], const APInt *a, const APInt *b, uint64_t *nums)
{
	// Check for NULL pointers
	if (a == NULL || b == NULL)
	{
		// Handle invalid input
		// printf("Invalid inputs: Please check the inputs\n");
		exit(0);
	}

	// Calculate the size of the result
	int size = a->size + b->size;

	// Allocate memory for the result structure
	APInt *result = malloc(sizeof(APInt));
	if (result == NULL)
	{
		// Handle memory allocation failure
		free(result);
		exit(0);
	}

	// Allocate memory for the byte array
	result->bytes = calloc(size, sizeof(uint8_t)); // Initialize bytes to zero
	if (result->bytes == NULL)
	{
		// Handle memory allocation failure
		destroyAPInt(result);
		exit(0);
	}
	// Multiplication
	for (int i = 0; i < a->size; i++)
	{
		int carry = 0;
		for (int j = 0; j < b->size; j++)
		{
			int product = a->bytes[i] * b->bytes[j] + result->bytes[i + j] + carry;
			carry = product >> 8;				   // Get the carry
			result->bytes[i + j] = product & 0xFF; // Store the least significant byte
		}
		// Propagate the carry to subsequent bytes
		for (int k = b->size; carry != 0; k++)
		{
			result->bytes[i + k] += carry;
			carry = result->bytes[i + k] >> 8;
			result->bytes[i + k] &= 0xFF;
		}
	}

	while (size > 1 && result->bytes[size - 1] == 0)
	{
		size--;
	}
	result->size = size;
	// Update the size of the result
	if (arr[nums[DEST]] != NULL)
	{
		destroyAPInt(arr[nums[DEST]]);
	}
	arr[nums[DEST]] = result;
}

void multiplyUINT64(APInt *arr[], uint64_t *nums)
{
	if (arr[nums[SRC1]] == NULL)
	{
		// Handle invalid input
		// printf("Invalid inputs: Please check the inputs\n");
		exit(0);
	}
	APInt *b = createAPInt(256);
	char *str = uint64ToString(nums[SRC2]);
	decimalToLittleEndianHex(atoi(str), b->bytes, b->size);
	// for (int i = 0; i < b->size; i++)
	// {
	// 	printf("converted: %02X ", b->bytes[i]);
	// }
	// printf("\n");
	multiplyAPInt(arr, arr[nums[SRC1]], b, nums);
	free(str);
	destroyAPInt(b);
}

// Function to perform repeated squaring for efficient exponentiation
// odd does not working
void powerAPInt(APInt *arr[], uint64_t *nums, uint64_t k)
{
	// Base case: if k is 0, return 1
	if (k == 0)
	{
		APInt *result = createAPInt(1);
		result->bytes[0] = 1;
		if (arr[nums[DEST]] != NULL)
		{
			destroyAPInt(arr[nums[DEST]]);
		}
		arr[nums[DEST]] = result;
		return;
	}

	// Base case: if k is 1, return a copy of src
	if (k == 1)
	{
		APInt *result = createAPInt(arr[nums[SRC1]]->size);
		memcpy(result->bytes, arr[nums[SRC1]]->bytes, arr[nums[SRC1]]->size);
		if (arr[nums[DEST]] != NULL)
		{
			destroyAPInt(arr[nums[DEST]]);
		}
		arr[nums[DEST]] = result;
		return;
	}
	// If k is even
	if (k % 2 == 0)
	{
		powerAPInt(arr, nums, k / 2);
		multiplyAPInt(arr, arr[nums[DEST]], arr[nums[DEST]], nums);
		// printf("even powerAPInt: ");
		// for (int j = 0; j < arr[nums[DEST]]->size; j++)
		// {
		// 	printf("%02X ", arr[nums[DEST]]->bytes[j]);
		// }
		// printf("\n");
		return;
	}
	else
	{ // If k is odd
		APInt *result = createAPInt(arr[nums[SRC1]]->size);
		memcpy(result->bytes, arr[nums[SRC1]]->bytes, arr[nums[SRC1]]->size);
		powerAPInt(arr, nums, (k - 1) / 2);
		multiplyAPInt(arr, arr[nums[DEST]], arr[nums[DEST]], nums);
		multiplyAPInt(arr, arr[nums[DEST]], result, nums);
		destroyAPInt(result);
		return;
	}
}

void leftShiftAPInt(APInt *arr[], uint64_t *nums)
{
	// Create a new APInt for the result
	APInt *result = malloc(sizeof(APInt));
	if (!result)
	{
		// Handle memory allocation failure
		exit(0);
	}
	int shiftBytes = nums[SRC2] / 8;
	int shiftBits = nums[SRC2] % 8;

	// Calculate size for the result
	result->size = arr[nums[SRC1]]->size + shiftBytes + ((shiftBits > 0) ? 1 : 0);
	result->bytes = calloc(result->size, sizeof(uint8_t));
	if (!result->bytes)
	{
		// Handle memory allocation failure
		free(result);
		exit(0);
	}

	// Perform left shift
	uint8_t carry = 0;
	for (int i = 0; i < arr[nums[SRC1]]->size; i++)
	{
		result->bytes[i + shiftBytes] |= (arr[nums[SRC1]]->bytes[i] << shiftBits) | carry;
		carry = (arr[nums[SRC1]]->bytes[i] >> (8 - shiftBits)) & 0xFF;
	}
	// Handle the last byte
	if (shiftBits > 0)
	{
		result->bytes[arr[nums[SRC1]]->size + shiftBytes] = carry;
	}

	if (arr[nums[DEST]] != NULL)
	{
		destroyAPInt(arr[nums[DEST]]);
	}
	arr[nums[DEST]] = result;
}

int compareAPInt(const APInt *a, const APInt *b)
{
	// If sizes are different, return appropriate result
	if (a->size != b->size)
	{
		return (a->size > b->size) ? 1 : -1;
	}

	// Compare byte by byte
	for (int i = a->size - 1; i >= 0; i--)
	{
		if (a->bytes[i] != b->bytes[i])
		{
			return (a->bytes[i] > b->bytes[i]) ? 1 : -1;
		}
	}

	// If all bytes are equal, return 0
	return 0;
}

// Destructor
void destroyAPInt(APInt *apInt)
{
	if (apInt != NULL)
	{
		free(apInt->bytes);
		free(apInt);
	}
}
