#define _CRT_SECURE_NO_WARNINGS

#include "vector.h"
#include "map.h"
#include "functions.h"
#include "structures.h"
#include "constants.h"
#include "safeFunctions.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <memory.h>
#include <windows.h>

wchar_t sortedFrequency[SIZE_OF_ALPHABET] = L"îåàèíò¸ðñëâêïìóäÿûüçáãé÷þõøæùöôýú\0";

bool isAlreadySolved(Config* config, wchar_t wch) {
	for (int i = 0; i < config->alreadySolvedIndex; ++i) {
		if (config->alreadySolved[i] == wch) return true;
	}

	return false;
}

void analysisOfFrequencyCharacters(Config* config) {
	Map* map = initMap();

	int count = 0;

	for (int i = 0; i < config->textSize; ++i) {
		if (!isWCharInLowerCase((*config->text)[i]) && !isWCharInUpperCase((*config->text)[i])) continue;

		if (isAlreadySolved(config, (*config->text)[i])) continue;

		MapElement* object = getElementByKey(map, *config->text + i, compareWChar);

		if (object == NULL) {
			ObjectMap key; key.Wchar_t = (*config->text)[i];
			ObjectMap value; value.Int = 1;

			insertMap(map, key, value);

			count++;
		}
		else {
			object->value->Int++;;
		}
	}

	MapElement* iterator = map->top;

	MapElement** toSortArr = (MapElement**) safeCalloc(count, sizeof(MapElement*));
	int indexSort = 0;

	while (iterator != NULL) {
		toSortArr[indexSort++] = iterator;

		iterator = iterator->next;
	}

	bubbleSortByValue(toSortArr, count);

	int biggest = toSortArr[0]->value->Int;
	int indexFrequency = 0;

	for (int i = 0; i < count; ++i) {
		if (biggest != toSortArr[i]->value->Int) {
			biggest = toSortArr[i]->value->Int;
			indexFrequency++;
		}

		while (true) {
			if (indexFrequency > 33) {
				wprintf(L"Out of alphabet\n");

				break;
			}

			if (!isAlreadySolved(config, sortedFrequency[indexFrequency])) break;
			else indexFrequency++;
		}

		if (indexFrequency > 33) {
			wprintf(L"Out of alphabet\n");

			break;
		}

		wprintf(L"%lc can be replaced with %lc, frequency: %.3f\n", toSortArr[i]->key->Wchar_t, sortedFrequency[indexFrequency], (double) biggest / (double) config->textSize);
	}

	free(toSortArr);
	freeMap(&map);
}

void outputCurrentText(Config* config) {
	wprintf(L"%ls\n", *config->text);
}

void markAsSolved(Config* config, wchar_t wch) {
	config->alreadySolved[config->alreadySolvedIndex++] = wch;
}

void markAsNotSolved(Config* config, wchar_t wch) {
	for (int i = 0; i < config->alreadySolvedIndex; ++i) {
		if (config->alreadySolved[i] == wch) {
			for (int j = i + 1; j < config->alreadySolvedIndex; ++j) {
				config->alreadySolved[j - 1] = config->alreadySolved[j];
			}

			config->alreadySolvedIndex--;

			break;
		}
	}
}

void replace(Config* config, wchar_t toReplace, wchar_t replaceWith, bool isUndo) {
	wprintf(L"Replacing %lc with %lc", toReplace, replaceWith);

	bool smthReplaced = false;

	for (int i = 0; i < config->textSize; ++i) {
		if ((*config->text)[i] == toReplace) {
			(*config->text)[i] = replaceWith;

			smthReplaced = true;
		}
	}

	if (smthReplaced && isUndo) markAsNotSolved(config, toReplace);

	if (smthReplaced && !isUndo) {
		if (config->countOfReplaces >= config->replaceCapacity) {
			config->replaceCapacity += MIN_CAPACITY_OF_REPLACMENTS;

			*config->prevReplaces = (wchar_t**) safeRealloc(*config->prevReplaces, config->replaceCapacity * sizeof(wchar_t*));

			for (int i = config->replaceCapacity - MIN_CAPACITY_OF_REPLACMENTS; i < config->replaceCapacity; ++i) {
				*((*config->prevReplaces) + i) = (wchar_t*) safeCalloc(PAIR, sizeof(wchar_t));
			}
		}

		(*((*config->prevReplaces) + config->countOfReplaces))[0] = toReplace;
		(*((*config->prevReplaces) + config->countOfReplaces))[1] = replaceWith;

		config->countOfReplaces++;

		markAsSolved(config, replaceWith);
	}

	wprintf(L"\nReplace ready!\n");
}

void autoReplace(Config* config) {
	if (config->alreadySolvedIndex > 0) return;

	Map* map = initMap();

	int count = 0;

	for (int i = 0; i < config->textSize; ++i) {
		if (!isWCharInLowerCase((*config->text)[i]) && !isWCharInUpperCase((*config->text)[i])) continue;

		if (isAlreadySolved(config, (*config->text)[i])) continue;

		MapElement* object = getElementByKey(map, *config->text + i, compareWChar);

		if (object == NULL) {
			ObjectMap key; key.Wchar_t = (*config->text)[i];
			ObjectMap value; value.Int = 1;

			insertMap(map, key, value);

			count++;
		}
		else {
			object->value->Int++;;
		}
	}

	MapElement* iterator = map->top;

	MapElement** toSortArr = (MapElement**)safeCalloc(count, sizeof(MapElement*));
	int indexSort = 0;

	while (iterator != NULL) {
		toSortArr[indexSort++] = iterator;

		iterator = iterator->next;
	}

	bubbleSortByValue(toSortArr, count);

	int biggest = toSortArr[0]->value->Int;
	int indexFrequency = 0;

	for (int i = 0; i < count; ++i) {
		if (indexFrequency > 33) {
			wprintf(L"Out of alphabet\n");

			break;
		}

		replace(config, toSortArr[i]->key->Wchar_t, sortedFrequency[indexFrequency++], false);
	}

	free(toSortArr);
	freeMap(&map);
}

void undo(Config* config) {
	if (config->countOfReplaces > 0) {
		replace(config, (*((*config->prevReplaces) + config->countOfReplaces - 1))[1], (*((*config->prevReplaces) + config->countOfReplaces - 1))[0], true);

		config->countOfReplaces--;
	}
	else wprintf(L"This is the original text!\n");
}

void displayGrouped(Config* config) {
	Map* map = initMap();

	int wordCapacity = MIN_WORD_SIZE;
	wchar_t* word = (wchar_t*) safeCalloc(wordCapacity, sizeof(wchar_t));
	int wordIndex = 0;

	int count = 0;

	for (int i = 0; i < config->textSize; ++i) {
		if (isWCharInLowerCase((*config->text)[i]) || isWCharInUpperCase((*config->text)[i])) {
			safeAddCharToArr(&word, (*config->text)[i], &wordCapacity, &wordIndex);
		}
		else if (wordIndex > 0) {
			safeAddCharToArr(&word, L'\0', &wordCapacity, &wordIndex);

			wordIndex--;

			MapElement* object = getElementByKey(map, &wordIndex, compareInt);

			if (object == NULL) {
				ObjectMap key; key.Int = wordIndex;

				ObjectMap value; value.vector = initVector();

				VectorObject element; element.Word = word;

				pushBack(value.vector, element);

				insertMap(map, key, value);

				count++;
			}
			else {
				VectorObject element; element.Word = word;

				pushBack(object->value->vector, element);
			}

			wordCapacity = MIN_WORD_SIZE;

			word = safeCalloc(wordCapacity, sizeof(wchar_t));

			wordIndex = 0;
		}
	}

	MapElement** toSortArr = (MapElement**) safeCalloc(count, sizeof(MapElement*));
	int indexSort = 0;

	MapElement* mapIterator = map->top;

	while (mapIterator != NULL) {
		toSortArr[indexSort++] = mapIterator;

		mapIterator = mapIterator->next;
	}

	bubbleSortByKey(toSortArr, count);

	for (int i = 0; i < count; ++i) {
		wprintf(L"Words at %d size", toSortArr[i]->key->Int);

		VectorElement* vectorIterator = toSortArr[i]->value->vector->top;

		while (vectorIterator != NULL) {
			wprintf(L", %ls", vectorIterator->element->Word);

			vectorIterator = vectorIterator->next;
		}

		wprintf(L"\n\n");
	}
}

void displayGroupedUnscrambled(Config* config) {
	Map* map = initMap();

	int wordCapacity = MIN_WORD_SIZE;
	wchar_t* word = (wchar_t*) safeCalloc(wordCapacity, sizeof(wchar_t));
	int wordIndex = 0;
	int unsolvedCount = 0;

	int count = 0;

	for (int i = 0; i < config->textSize; ++i) {
		if (isWCharInLowerCase((*config->text)[i]) || isWCharInUpperCase((*config->text)[i])) {
			if(!isAlreadySolved(config, (*config->text)[i])) unsolvedCount++;

			safeAddCharToArr(&word, (*config->text)[i], &wordCapacity, &wordIndex);
		}
		else if (wordIndex > 0) {
			safeAddCharToArr(&word, L'\0', &wordCapacity, &wordIndex);

			MapElement* object = getElementByKey(map, &unsolvedCount, compareInt);

			if (object == NULL) {
				ObjectMap key; key.Int = unsolvedCount;

				ObjectMap value; value.vector = initVector();

				VectorObject element; element.Word = word;

				pushBack(value.vector, element);

				insertMap(map, key, value);

				count++;
			}
			else {
				VectorObject element; element.Word = word;

				pushBack(object->value->vector, element);
			}

			wordCapacity = MIN_WORD_SIZE;

			word = safeCalloc(wordCapacity, sizeof(wchar_t));

			wordIndex = 0;

			unsolvedCount = 0;
		}
	}

	MapElement** toSortArr = (MapElement**)safeCalloc(count, sizeof(MapElement*));
	int indexSort = 0;

	MapElement* mapIterator = map->top;

	while (mapIterator != NULL) {
		toSortArr[indexSort++] = mapIterator;

		mapIterator = mapIterator->next;
	}

	bubbleSortByKey(toSortArr, count);

	for (int i = 0; i < count; ++i) {
		wprintf(L"Words at %d size", toSortArr[i]->key->Int);

		VectorElement* vectorIterator = toSortArr[i]->value->vector->top;

		while (vectorIterator != NULL) {
			wprintf(L", %ls", vectorIterator->element->Word);

			vectorIterator = vectorIterator->next;
		}

		wprintf(L"\n\n");
	}

	free(toSortArr);
	freeMap(&map);
}

Config* initConfig() {
	Config* config = (Config*) safeCalloc(1, sizeof(Config));

	config->text = (wchar_t**) safeCalloc(1, sizeof(wchar_t*));
	*config->text = NULL;

	config->prevReplaces = (wchar_t***) safeCalloc(1, sizeof(wchar_t**));

	config->replaceCapacity = MIN_CAPACITY_OF_REPLACMENTS;

	*config->prevReplaces = (wchar_t**) safeCalloc(config->replaceCapacity, sizeof(wchar_t*));

	for (int i = 0; i < config->replaceCapacity; ++i) {
		*((*config->prevReplaces) + i) = (wchar_t*) safeCalloc(PAIR, sizeof(wchar_t));
	}

	config->alreadySolvedIndex = 0;

	// config.map <- init this

	return config;
}

void helpOutput() {
	wprintf(
		L"Hello, a man with a passion for cryptanalytics!\n"
		L"Here commands, what you can use:\n"
		L"1 - Display the resulting text on the screen\n" // Ready
		L"2 - Perform frequency analysis\n" // Ready
		L"3 - Replace a character with another character\n" // Ready
		L"4 - Cancel the last substitution\n" // Map with last characters, what replaced // Ready
		// L"5 - Mark the symbol as solved\n" // Map with last characters, what replaced // Ready
		L"5 - Automatic replacement by letter frequency\n" // Ready
		L"6 - Display replacement history\n" // Ready
		L"7 - Display all words grouped by number of letters on the screen\n" // Ready
		L"8 - Display all words grouped by the number of unscrambled letters on the screen\n" // Ready
		L"9 - Clean the console\n" // Ready
		L"0 - Quit the program\n" // Ready
	);
}

void mainLoop(Config* config) {
	wchar_t lastCommand = '\n';
	short currentCountOfCommands = 0;
	wchar_t wch;

	while (true) {
		wch = getWChar();

		if (wch == WEOF) break;

		if (wch == L'\n' && currentCountOfCommands > 1) {
			currentCountOfCommands = 0;
			wprintf(L"Fail to process more, than one command in one time\n");

			continue;
		}

		currentCountOfCommands++;

		if (wch == L'\n' && lastCommand != L'\n') {
			if (lastCommand == FREQUENCY_ANALYSIS) {
				analysisOfFrequencyCharacters(config);
			}
			else if (lastCommand == OUTPUT_CURRENT_TEXT) {
				outputCurrentText(config);
			}
			else if (lastCommand == MAKE_REPLACE_IN_TEXT) {
				wprintf(L"\nTo replace: ");

				wchar_t toReplace, replaceWith, tmp;

				toReplace = getWChar();

				tmp = getWChar();

				if (tmp != L'\n') {
					wprintf(L"\Please, enter only once symbol!\n");

					return;
				}

				wprintf(L"\nReplace with: ");

				replaceWith = getWChar();

				tmp = getWChar();

				if (tmp != L'\n') {
					wprintf(L"\Please, enter only once symbol!\n");

					return;
				}

				replace(config, toReplace, replaceWith, false);
			}
			else if (lastCommand == DISPLAY_GROUPED) {
				displayGrouped(config);
			}
			else if (lastCommand == DISPLAY_GROUPED_UNSCRAMBLED) {
				displayGroupedUnscrambled(config);
			}
			else if (lastCommand == UNDO) {
				undo(config);
			}
			else if (lastCommand == AUTO_REPLACEMENT) {
				autoReplace(config);
			}
			else if (lastCommand == OUTPUT_LAST_REPLACEMENTS) {
				for (int i = 0; i < config->countOfReplaces; ++i) {
					wprintf(L"%lc - %lc\n", (*((*config->prevReplaces) + i))[0], (*((*config->prevReplaces) + i))[1]);
				}
			}
			else if (lastCommand == FLUSH) {
				system("cls");

				helpOutput();
			}
			else if (lastCommand == EXIT) {
				wprintf(L"Have a nice day!\n");
				exit(0);
			}
			else {
				wprintf(L"Unidentified symbol: %lc\n", lastCommand);
			}

			currentCountOfCommands = 0;
		}
		else if (wch == L'\n' && lastCommand == L'\n') currentCountOfCommands = 0;

		lastCommand = wch;
	}
}

void writeTextToBuffer(FILE* file, Config* config) {
	int currentTextCapacity = MIN_WORD_SIZE;
	wchar_t* text = (wchar_t*)safeCalloc(currentTextCapacity, sizeof(wchar_t));
	int currentTextIndex = 0;

	wchar_t wch;

	while (true) {
		wch = getwc(file);

		if (wch == WEOF) break;

		safeAddCharToArr(&text, wch, &currentTextCapacity, &currentTextIndex);
	}

	safeAddCharToArr(&text, L'\0', &currentTextCapacity, &currentTextIndex);

	*config->text = text;
	config->textSize = currentTextIndex - 1;
}

int main(int argc, char* argv[]) {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");

	if (argc != 2) {
		wprintf(L"Please, enter path to your file in arg of program (without spaces)!\n");

		exit(1);
	}

	FILE* text = fopen(*(argv + 1), "r, ccs=UTF-8");

	if (!text) {
		wprintf(L"Unable to open file!\n");

		exit(1);
	}

	helpOutput();

	Config* config = initConfig();

	writeTextToBuffer(text, config);

	mainLoop(config);

	return EXIT_SUCCESS;
}