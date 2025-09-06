#define initialListLength 20
#define wordLength 15

//This struct is used in the generateAnagram function
typedef struct {
	char word[wordLength];
	int location[2];
	int direction;
	int length;
	
} wordInformation;

int copyFile(FILE *file, char arr[initialListLength][wordLength]);
void setUppercase(char arr[]);
void sortArray(char words[initialListLength][wordLength], int length);
void setBoard(char board[wordLength][wordLength], char solutionBoard[wordLength][wordLength], char words[initialListLength][wordLength], int length, wordInformation wordList[]);
int addWord(char board[wordLength][wordLength], char solutionBoard[wordLength][wordLength], char word[], int size, int positionX, int positionY, int index, wordInformation wordList[], int place);
int setWords(char board[wordLength][wordLength], char solutionBoard[wordLength][wordLength], char words[initialListLength][wordLength], int length, wordInformation wordList[]);
void displayBoard(char board[wordLength][wordLength], char solutionBoard[wordLength][wordLength]);
void generateAnagrams(wordInformation words[], int length);
