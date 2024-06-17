#include "APInt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 256
#define OPERPARAMS 3

// commands
#define UINT64 "UINT64"
#define HEX "HEX_STRING"
#define CLONE "CLONE"
#define DUMP "DUMP"
#define END "END"

// operations
#define ADD "ADD"
#define MULT_64 "MUL_UINT64"
#define MULT_AP "MUL_APINT"
#define POWER "POW"
#define COMPARE "CMP"
#define SHIFTL "SHL"

uint64_t *readNumsInLine(char *line)
{
	uint64_t *nums = malloc((OPERPARAMS + 1) * sizeof(uint64_t));
	char *token = strtok(line, " ");
	int i = 0;
	while (token != NULL)
	{
		nums[i++] = atoi(token);
		token = strtok(NULL, " ");
	}
	nums[i] = '\0';
	// printf("nums: %i %i %i\n", nums[0], nums[1], nums[2]);
	return nums;
}

void getLine(FILE *input, char **line)
{
	//*line = malloc(MAX_LEN);

	if (fgets(*line, 100000000, input) != NULL)
	{
		// Remove '\n' at the end of the line if it exists
		(*line)[strcspn(*line, "\n")] = '\0';
	}
	else
	{
		if (feof(input))
		{
			// Check if end of file was reached but the line is empty
			if (strlen(*line) == 0)
			{
				free(*line);  // Free the memory allocated for the empty line
				*line = NULL; // Set line to NULL to indicate end of file
			}
			else
			{
				printf("End of file reached.\n");
			}
		}
		else
		{
			perror("Error reading line");
		}
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char const *argv[])
{

	// Open the input and output files, use stdin and stdout if not configured.
	FILE *output = NULL;
	int outputGiven = (argc >= 3);
	if (!outputGiven)
		output = stdout;
	else
		output = fopen(argv[2], "w");

	FILE *input = NULL;
	// char filepath[MAX_LEN] = "/workspaces/ECS153-sq-hw0/input/";
	// strcat(filepath, argv[1]);
	// printf("Opening file: %s\n", filepath);
	int inputGiven = (argc >= 2);
	if (!inputGiven)
		input = stdin;
	else
	{
		input = fopen(argv[1], "r");
		if (input == NULL)
		{
			fprintf(stderr, "Error: Unable to open the input file.\n");
			return 1;
		}
	}

	/* Your code to init APInt array, and operate on them here. */
	char *line = malloc(100000000);
	int i = 0;
	int numAPInts = 0;
	// the first line is the number of APInts
	getLine(input, &line);
	numAPInts = atoi(line);
	// printf("numAPInts: %i\n", numAPInts);

	// Allocate memory for an array of APInt pointers
	APInt *apInt_arr[numAPInts];
	for (int i = 0; i < numAPInts; i++)
	{
		apInt_arr[i] = NULL;
	}

	// the read the rest of the lines
	while (line != NULL)
	{
		getLine(input, &line);
		if (strcmp(line, UINT64) == 0)
		{
			getLine(input, &line);
			APIntFromUINT64(apInt_arr, line, i);
			// printf("UINT64s: ");
			// for (int j = 0; j < apInt_arr[i]->size; j++)
			// {
			// 	printf("%02X ", apInt_arr[i]->bytes[j]);
			// }
			// printf("(%d bytes)\n", apInt_arr[i]->size);
			i++;
		}
		else if (strcmp(line, HEX) == 0)
		{
			getLine(input, &line);
			// apInt_arr[i] = APIntFromHex(line);
			APIntFromHex(apInt_arr, line, i);
			// printf("HEX_STR: ");
			// for (int j = 0; j < apInt_arr[i]->size; j++)
			// {
			// 	printf("%02X ", apInt_arr[i]->bytes[j]);
			// }
			// printf("(%d bytes)\n", apInt_arr[i]->size);
			i++;
		}
		else if (strcmp(line, CLONE) == 0)
		{
			getLine(input, &line);
			int index = atoi(line);
			cloneAPInt(apInt_arr, index, i);
			// printf("CLONE: ");
			// for (int j = 0; j < apInt_arr[i]->size; j++)
			// {
			// 	printf("%02X ", apInt_arr[i]->bytes[j]);
			// }
			// printf("(%d bytes)\n", apInt_arr[i]->size);
			i++;
		}
		else if (strcmp(line, DUMP) == 0)
		{
			// printf("DUMP:\n");
			for (int j = 0; j < numAPInts; j++)
			{
				dumpAPInt(apInt_arr[j], output);
			}
			fprintf(output, "\n");
		}
		else if (strcmp(line, ADD) == 0)
		{
			getLine(input, &line);
			uint64_t *nums = readNumsInLine(line);
			addAPInt(apInt_arr, nums);
			// printf("ADD: ");
			// for (int j = 0; j < apInt_arr[nums[0]]->size; j++)
			// {
			// 	printf("%02X ", apInt_arr[nums[0]]->bytes[j]);
			// }
			// printf("(%d bytes)\n", apInt_arr[nums[0]]->size);
			free(nums);
		}
		else if (strcmp(line, MULT_AP) == 0)
		{
			getLine(input, &line);
			uint64_t *nums = readNumsInLine(line);
			// free dest APInt??
			multiplyAPInt(apInt_arr, apInt_arr[nums[1]], apInt_arr[nums[2]], nums);
			// printf("MULTAPINT: ");
			// for (int j = 0; j < apInt_arr[nums[0]]->size; j++)
			// {
			// 	printf("%02X ", apInt_arr[nums[0]]->bytes[j]);
			// }
			// printf("(%d bytes)\n", apInt_arr[nums[0]]->size);
			free(nums);
		}
		else if (strcmp(line, MULT_64) == 0)
		{
			getLine(input, &line);
			uint64_t *nums = readNumsInLine(line);
			// free dest APInt??
			multiplyUINT64(apInt_arr, nums);
			// printf("MULTUINT64: ");
			// for (int j = 0; j < apInt_arr[nums[0]]->size; j++)
			// {
			// 	printf("%02X ", apInt_arr[nums[0]]->bytes[j]);
			// }
			// printf("(%d bytes)\n", apInt_arr[nums[0]]->size);
			free(nums);
		}
		else if (strcmp(line, POWER) == 0)
		{
			getLine(input, &line);
			uint64_t *nums = readNumsInLine(line);
			// free dest APInt??
			powerAPInt(apInt_arr, nums, nums[2]);
			// printf("POWER: ");
			// for (int j = 0; j < apInt_arr[nums[0]]->size; j++)
			// {
			// 	printf("%02X ", apInt_arr[nums[0]]->bytes[j]);
			// }
			// printf("(%d bytes)\n", apInt_arr[nums[0]]->size);
			free(nums);
		}
		else if (strcmp(line, COMPARE) == 0)
		{
			getLine(input, &line);
			uint64_t *nums = readNumsInLine(line);
			// store in array or print?
			int val = 0;
			val = compareAPInt(apInt_arr[nums[0]], apInt_arr[nums[1]]);
			fprintf(output, "%i\n", val);
			// printf("COMPARE: %i\n", val);
			free(nums);
		}
		else if (strcmp(line, SHIFTL) == 0)
		{
			getLine(input, &line);
			uint64_t *nums = readNumsInLine(line);
			// free dest APInt??
			leftShiftAPInt(apInt_arr, nums);
			// printf("SHIFTL: ");
			// for (int j = 0; j < apInt_arr[nums[0]]->size; j++)
			// {
			// 	printf("%02X ", apInt_arr[nums[0]]->bytes[j]);
			// }
			// printf("(%d bytes)\n", apInt_arr[nums[0]]->size);
			free(nums);
		}
		else if (strcmp(line, END) == 0)
		{
			for (int i = 0; i < numAPInts; i++)
			{
				destroyAPInt(apInt_arr[i]);
			}
			// printf("END\n");
			break;
		}
		else
		{
			// printf("Invalid input\n");
			exit(0);
		}
	}

	free(line);
	// Close the files we opened.
	if (outputGiven)
		fclose(output);
	if (inputGiven)
		fclose(input);
	return 0;
}
