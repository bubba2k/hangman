#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <ctype.h>

// Defined in src/words.cpp
std::string getRandomWord();

std::vector<std::string> sprites =
{
R"d(
_____                
|   |
|   
|   
|       )d",
R"d(
_____                
|   |
|   O
|   
|       )d",
R"d(
_____                
|   |
|   O
|   |
|       )d",
R"d(
_____                
|   |
|   O
|  /|
|       )d",
R"d(
_____                
|   |
|   O
|  /|\
|       )d",
R"d(
_____                
|   |
|   O
|  /|\
|  /    )d",
R"d(
_____                
|   |
|   O
|  /|\
|  / \   )d"
};

class SecretWord {
	std::string strWord;

	public:
	void reset() {
		strWord = getRandomWord();
	}

	std::string getString() {
		return strWord;
	}

	int getLength() {
		return strWord.size();
	}

	bool charIsMember(char ch) {
		return ( strWord.find(ch) != std::string::npos );
	}

	std::vector<int> getIndices(char ch) {
		std::vector<int> indices;
		int index = 0;

		while( strWord.find(ch, index) != std::string::npos )
		{
			index = strWord.find(ch, index);
			indices.push_back(index);
			++index;
		}
		
		return indices;
	}
};

class TriedCharacters {
	std::string strTriedCharacters;
	std::string strMistakes;

	public:
	void reset() {
		strTriedCharacters.clear();
		strMistakes.clear();
	}

	std::string getTriedCharacters() {
		return strTriedCharacters;
	}

	std::string getMistakes() {
		return strMistakes;
	}

	bool charHasBeenTried(char ch) {
		return ( strTriedCharacters.find(ch) != std::string::npos );
	}

	void add(char ch) {
		strTriedCharacters += ch;
	}

	void addMistake(char ch) {
		strMistakes += ch;
	}
};

char getInputLetter() {
	std::string input;
	char ch = 0;
	
	bool invalidChar = true;
	while(invalidChar)
	{
		while(std::getline(std::cin, input), input.size() != 1)
		{
			std::cout << "Please input only one character."
					  << std::endl;
		}
		
		ch = input.at(0);
		if(!isalpha(ch))
		{
			std::cout << "Not a valid alphabetic character."
					  << std::endl;
		}
		else
		{
			invalidChar = false;
		}
	}

	if(isupper(ch))
		ch = tolower(ch);

	return ch;
}

class Game {
	SecretWord secretWord;
	TriedCharacters triedLetters;
	std::string strProgress;
	int mistakes;

	void reset() {
		srand(time(NULL));

		secretWord.reset();
		triedLetters.reset();

		strProgress.clear();
		for(int i = 0; i < secretWord.getLength(); i++)
			strProgress += '_';

		mistakes = 0;
	}

	void tryLetter(char letter) {
		if(secretWord.charIsMember(letter)) 
		{
			std::vector<int> indices = secretWord.getIndices(letter);
			for(auto index : indices)
				strProgress.replace(index, 1, &letter, 1);
		}
		else 
		{
			triedLetters.addMistake(letter);
			mistakes++;
		}

		triedLetters.add(letter);
	}

	void drawUI() {
		
		std::cout << std::endl << std::endl << std::endl;
		for(unsigned i = 0; i < strProgress.size(); i++)
			std::cout << strProgress.at(i) << ' ';
		std::cout << std::endl;

		std::cout << sprites.at(mistakes);	

		for(unsigned i = 0; i < triedLetters.getMistakes().size(); i++)
			std::cout << triedLetters.getMistakes().at(i) << ' ';
		std::cout << std::endl;

		std::cout << std::endl;
	}

	public:
	bool gameLoop() {
		char letter;
		reset();

		for(;;)
		{
			drawUI();

			if(mistakes >= 6)
			{
				std::cout << "You lose :(" << std::endl
					      << "The word was " << secretWord.getString()
						  << std::endl;
				return false;
			}
			else if(strProgress == secretWord.getString())
				return true;

			while(triedLetters.charHasBeenTried(letter = getInputLetter()))
				std::cout << "You already tried this character." << std::endl;

			tryLetter(letter);
		}
	}
};

int main()
{
	Game game;

	bool playing = true;
	while(playing)
	{
		char input;

		game.gameLoop();
		std::cout << "Play again? y/n" << std::endl;

		bool awaitInput = true;
		while(awaitInput)
		{
			std::cin >> input;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), 
							'\n');
		
			switch(input)
			{
				default:
					break;

				case 'N':
				case 'n':
					playing = false;

				case 'Y':
				case 'y':
					awaitInput = false;
					break;
			}
		}
	}
}
