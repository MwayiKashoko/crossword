#include <stdio.h>
#include <string.h>
#include "crosswordfunc.h"

int main(int argc, char *argv[]) {
	char words[initialListLength][wordLength];
	//File I'm reading from if the user decides to input it
	FILE *fp;
	//File I'm outputting to in case the user decides to. It creates a new file or reads from a file already stored
	FILE *outputfp;
	//Words the user will input
	char userWords[wordLength];
	//Since I can't change the length of an array in C I have to use this in order to read the size of my words array
	int listLength = 0;
	
	//Puzzle board
	char board[wordLength][wordLength] = {0};
	//Solution board
	char solutionBoard[wordLength][wordLength] = {0};

	//wordInformation struct array which is needed for the anagram
	wordInformation wordList[initialListLength];

	if (argc == 2 || argc == 3) {
		if ((fp = fopen(argv[1], "r")) == NULL) {
			printf("INVALID FILE! THIS PROGRAM WILL TERMINATE!\n");
			return -1;
		}

		//If there are 3 files this then will read all the outputs from the terminal into the file inputted
		if (argc == 3) {
			outputfp = freopen(argv[2], "w+", stdout);
		}

		listLength = copyFile(fp, words);
	} else if (argc == 1) {
		while (listLength < initialListLength) {
			//This might not be the correct way to do it? But since I can't use fgets if the user inputs a string with a length greater than 15 because the program will stop running, I had to use a test word that has a large size to test the size");
			char testWord[500];
			scanf("%s", testWord);
			//So the new line does not get read into the next user input function
			getc(stdin);

			if (strlen(testWord) > wordLength) {
				printf("Error! String is too long\n");
				continue;
			}

			strcpy(userWords, testWord);
	
			if (!strcmp(userWords, ".")) {
				break;
			} else {
				//Used to store if there has been an improper character in the word in the for loop. Since if I use continue in a for loop the scope will not allow it to go to the next iteration of the while loop
				int condition = 0;

				if (strlen(userWords) < 2) {
					printf("Error! String is too short\n");
					continue;
				}

				for (int i = 0; i < strlen(userWords); i++) {
					int c = userWords[i];

					if (c < 65 || (c > 90 && c < 97) || c > 122) {
						printf("Improper word format\n");
						condition = 1;
						break;
					}
				}

				if (condition) {
					continue;
				}

				strcpy(words[listLength], userWords);
				listLength++;
			}
		}
	} else {
		//If the user enters 4 or more inputs in the command line this code will run and break
		printf("TOO MANY COMMAND LINE ARGUMENTS!\n");
		return -1;
	}

	if (listLength < 1) {
		return listLength;
	} else {
		//Making sure every letter in each word is uppercase
		for (int i = 0; i < listLength; i++) {
			setUppercase(words[i]);
		}

		sortArray(words, listLength);
	}

	setBoard(board, solutionBoard, words, listLength, wordList);
	int crosswordListLength = setWords(board, solutionBoard, words, listLength, wordList);
	displayBoard(board, solutionBoard);
	generateAnagrams(wordList, crosswordListLength);

	return 0;
}
