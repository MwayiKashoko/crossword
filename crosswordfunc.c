#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "crosswordfunc.h"

//This function copies the file inputted by the user into the words array, while making sure there aren't any imporoper characters
int copyFile(FILE *file, char arr[initialListLength][wordLength]) {
	int i = 0;
	int j = 0;

	while (1 && i < initialListLength) {
		char c = getc(file);
		
		if (c == '\n') {
			if (j < 2) {
				printf("The word is too short!");
				
				for (int k = 0; k < wordLength; k++) {
					arr[i][k] = '\0';
				}

				j = 0;
				continue;
			}

			arr[i][j] = '\0';
			i++;
			j = 0;
			continue;
		} else if ((c < 65 || (c > 90 && c < 97) || c > 122 || j > wordLength-1) && c != EOF) {
				printf("Improper formatting of the word!\n");
				
				for (int k = 0; k < wordLength; k++) {
					arr[i][k] = '\0';
				}

				while (getc(file) != '\n') {

				}

				j = 0;
				continue;
		} else if (c == EOF) {
			break;
		}	
	
		arr[i][j] = c;
		j++;
	}

	return i;
}

//Makes every valid word uppercase
void setUppercase(char arr[]) {
	for (int i = 0; i < strlen(arr); i++) {
		arr[i] = toupper(arr[i]);
	}
}

//Sorts each array in descending order of length
void sortArray(char words[initialListLength][wordLength], int length) {
	for (int i = 0; i < length; i++) {
		char tempString[wordLength];
		strcpy(tempString, words[i]);
		int index = i;

		for (int j = i; j < length; j++) {
			if (strlen(words[j]) > strlen(words[index])) {
				index = j;
			}
		}
		
		strcpy(words[i], words[index]);
		strcpy(words[index], tempString);
	}
}

//It first sets up the puzzle board with all # and solution board with . while also finding the longest word and making a struct to put in the wordInformation array.
void setBoard(char board[wordLength][wordLength], char solutionBoard[wordLength][wordLength], char words[initialListLength][wordLength], int length, wordInformation wordList[]) {
	for (int i = 0; i < wordLength; i++) {
		for (int j = 0; j < wordLength; j++) {
			board[i][j] = '#';
			solutionBoard[i][j] = '.';
		}
	}

	char longestWord[wordLength];
	strcpy(longestWord, words[0]);

	int spaces = (wordLength-strlen(longestWord))/2;

	strcpy(wordList[0].word, longestWord);
	wordList[0].location[0] = spaces;
	wordList[0].location[1] = (1+wordLength)/2-1;
	wordList[0].direction = 0;
	wordList[0].length = strlen(longestWord);

	for (int i = 0; i < strlen(longestWord); i++) {
		board[(1+wordLength)/2-1][i+spaces] = ' ';
		solutionBoard[(1+wordLength)/2-1][i+spaces] = longestWord[i];
	}

}

//If the condition is met in the setWords function, this code will add a new word to the solution board and spaces to the puzzle board
int addWord(char board[wordLength][wordLength], char solutionBoard[wordLength][wordLength], char word[], int size, int positionX, int positionY, int index, wordInformation wordList[], int place) {
	//0 for horizontal, 1 for vertical, direction of the intersecting word already on the board, automatically initialized to horizontal
	int direction = 0;

	//Checking to see if the intersecting word is vertical by seeing if the position above or below it at the intersection is not a .
	if (solutionBoard[positionY-1][positionX] != '.' || solutionBoard[positionY+1][positionX] != '.') {
		direction = 1;
	}

	///Checking the bounds of where the word will begin and end
	if (!direction) {
		if (positionY-index < 0 || positionY-index+size-1 > wordLength-1) {
			return 0;
		}
	} else {
		if (positionX-index < 0 || positionX-index+size-1 > wordLength-1) {
			return 0;
		}
	}

	//Horizontal intersecting word so my program will place a vertical word if all conditions are met
	if (!direction) {
		for (int i = 0; i < size; i++) {
			//Checking to make sure that the word does not run into any other letters so it is always spaced apart from a word by one space. 
			if (((solutionBoard[positionY-index+i][positionX-1] != '.' || solutionBoard[positionY-index+i][positionX+1] != '.') && positionY-index+i != positionY) || (solutionBoard[positionY-index-1][positionX] != '.' && positionY-index-1 > 0) || (positionY-index+size < wordLength && solutionBoard[positionY-index+size][positionX] != '.')) {
				return 0;
			}
		}
	
		//Now it will place the word	
		for (int i = 0; i < size; i++) {
			board[positionY-index+i][positionX] = ' ';
			solutionBoard[positionY-index+i][positionX] = word[i];
		}

		//Adding the information about the word to the struct array for later use
		strcpy(wordList[place].word, word);
		wordList[place].location[0] = positionX;
		wordList[place].location[1] = positionY-index;
		wordList[place].direction = 1;
		wordList[place].length = size;;

		return 1;
	} else {
		//Same process for horizontal word placing as vertical word placing
		for (int i = 0; i < size; i++) {
			if (((solutionBoard[positionY-1][positionX-index+i] != '.' || solutionBoard[positionY+1][positionX-index+i] != '.') && positionX-index+i != positionX) || (solutionBoard[positionY][positionX-index-1] != '.' && positionX-index-1 > 0) || (positionY-index+size < wordLength && solutionBoard[positionY][positionX-index+size] != '.')) {
				return 0;
			}
		}

		for (int i = 0; i < size; i++) {
			board[positionY][positionX-index+i] = ' ';
			solutionBoard[positionY][positionX-index+i] = word[i];
		}

		strcpy(wordList[place].word, word);
		wordList[place].location[0] = positionX-index;
		wordList[place].location[1] = positionY;
		wordList[place].direction = 0;
		wordList[place].length = size;

		return 1;
	}

	return 0;

	//If it returns 1 it was a successfull placing, if it returns 0 there was a problem
}

//Checks the condition for adding words on the board and goes back to words that were skipped
int setWords(char board[wordLength][wordLength], char solutionBoard[wordLength][wordLength], char words[initialListLength][wordLength], int length, wordInformation wordList[]) {
	//Total words placed which will be the length of the array used for anagrams
	int place = 1;
	char skippedWords[length][wordLength];
	//Length of skipped words array
	int skippedWordsPosition = 0;

	for (int i = 1; i < length; i++) {
		//If the condition of setting a word is true, then my code will skip through the j, k, and l loops 
		int condition = 0;
		for (int j = 0; j < wordLength; j++) {
			if (condition) {
				continue;
			}

			for (int k = 0; k < strlen(words[i]); k++) {
				if (condition) {
					continue;
				}

				for (int l = 0; l < wordLength; l++) {
					if (condition) {
						continue;
					}
				
					if (words[i][k] == solutionBoard[j][l]) {
						condition = addWord(board, solutionBoard, words[i], strlen(words[i]), l, j, k, wordList, place);
					}

					if (condition) {
						place++;
					}
				}
			}
		}

		if (!condition) {
			printf("It was not possible to place %s. My program will try to place it later.\n", words[i]);
			strcpy(skippedWords[skippedWordsPosition], words[i]);
			skippedWordsPosition++;
		}
	}

	if (place != length) {
		printf("My program will now try to place the words it skipped\n");

		int newSkippedWordsLength = 0;
		int loopCount = 0;
		int wordsSuccessfullyPlaced = 0;

		//Same process as the above loop
		while (1) {
			for (int i = 0; i < skippedWordsPosition; i++) {
				int condition = 0;
				for (int j = 0; j < wordLength; j++) {
					if (condition) {
						continue;
					}

					for (int k = 0; k < strlen(skippedWords[i]); k++) {
						if (condition) {
							continue;
						}

						for (int l = 0; l < wordLength; l++) {
							if (condition) {
								continue;
							}
				
							if (skippedWords[i][k] == solutionBoard[j][l]) {
								condition = addWord(board, solutionBoard, skippedWords[i], strlen(skippedWords[i]), l, j, k, wordList, place);
							}

							if (condition) {
								place++;
								printf("IT WORKED! IT PLACED %s!\n", skippedWords[i]);
								wordsSuccessfullyPlaced++;
							}
						}
					}
				}

				if (!condition) {
					printf("It was not possible to place %s (At least right now). My program will try to place it later.\n", skippedWords[i]);
					strcpy(skippedWords[newSkippedWordsLength], skippedWords[i]);
					newSkippedWordsLength++;
				}
			}

			if (newSkippedWordsLength == skippedWordsPosition) {
				loopCount++;
			}

			if (loopCount == 2) {
				break;
			}

			skippedWordsPosition = newSkippedWordsLength;
			newSkippedWordsLength = 0;
		}

		if (wordsSuccessfullyPlaced == 0) {
			printf("My program could not place any new words unfortunately\n");
		} else if (skippedWordsPosition == 0) {
			printf("My program was able to successfully place every word!\n");
		}
	} else {
		printf("My program was able to successfully place every word!\n");
	}
	
	return place;
}

//Displays the puzzle board and solution board
void displayBoard(char board[wordLength][wordLength], char solutionBoard[wordLength][wordLength]) {
	printf(" ------------------------------\n");

	for (int i = 0; i < wordLength; i++) {
		printf("|");
		for (int j = 0; j < wordLength; j++) {
			printf("%c ", board[i][j]);
		}

		printf("|\n");
	}
	
	printf(" ------------------------------\n\n");

	printf(" ------------------------------\n");

	for (int i = 0; i < wordLength; i++) {
		printf("|");
		for (int j = 0; j < wordLength; j++) {
			//I personally like the space because it makes the width and length visually equal for the user
			printf("%c ", solutionBoard[i][j]);
		}

		printf("|\n");
	}
	
	printf(" ------------------------------\n\n");

}


//It will generate the anagrams as well as providing the location and diretion of the words on the board
void generateAnagrams(wordInformation words[], int length) {
	printf("CLUES:\nLocation | Direction |  Anagram\n");

	//the srand function gives the random function (rand()) a new seed, while the time function is giving the time elapsed since January 1st 1970 (around 1680000000 seconds). I could have used time(0) as well but time(NULL) is equivalent to it and I think it looks better. I know it is not a very good function for generating random numbers but it will be fine for a program like this.
	srand(time(NULL));

	//Randomize array. strfry but for the struct array
	for (int i = 0; i < length; i++) {
		char tempWord[wordLength];
		strcpy(tempWord, words[i].word);
		int tempLocation1 = words[i].location[0];
		int tempLocation2 = words[i].location[1];
		int tempDirection = words[i].direction;
		int tempLength = strlen(tempWord);

		int ran = rand()%length;

		strcpy(words[i].word, words[ran].word);
		words[i].location[0] = words[ran].location[0];
		words[i].location[1] = words[ran].location[1];
		words[i].direction = words[ran].direction;
		words[i].length = words[ran].length;

		strcpy(words[ran].word, tempWord);
		words[ran].location[0] = tempLocation1;
		words[ran].location[1] = tempLocation2;
		words[ran].direction = tempDirection;
		words[ran].length = tempLength;
	}

	for (int i = 0; i < length; i++) {
		printf("%2d, %2d   | %9s |  %-5s\n", words[i].location[0]+1, words[i].location[1]+1, words[i].direction ? "Down" : "Across", strfry(words[i].word));
	}
}
