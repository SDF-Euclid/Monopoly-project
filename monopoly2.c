//Standard libraries to include
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

//Other files to include
#include "boardStruct.h" //structure declaring all variables for the board squares
#include "boardStruct.c" //include the functions regarding all the values for the board squares
#include "card.h" //includes the community chest and chance cards 
#include "pieceArt.c" //piece ascii art included in another file 
//#include "boardArt.c" //board ascii art included in another file

//Global Variables


//structures
typedef struct PlayerProfile_struct { //variables that each player profile will use
	char playerName[51];
	char *propertyID[28];
	char *propertyColor[28];
	int propertyNums[28];	
	int playerNumber;
	int playerOrder;
	int boardPiece;
	int money;
	int propertyNumber;
	int railroadNumber;
	int companyNumber;
	int colorSets;
	int houses;
	int hotels;
	int getOutOfJailFreeCard;
	int boardPosition;
	int jailTime;
	bool isBankrupt;
}PlayerProfile;

//Function Prototypes
void ActionChance ();
void ActionCommunityChest ();
void AssignPregameValues ();
void BuyProperty ();
void CheckProperty ();
void ClearScreen ();
void DrawCard ();
void PayRent ();
void PrintPiece ();
void RollDice ();
void Sell ();
void TakeTurnPlayer ();

//main function
int main(void) {
	
	//variables used throughout main() function
	const int BUFSIZE = 100; //here for fgets() and sscanf()
	const int MAX_PLAYER_SIZE = 4;
	char buffer[BUFSIZE]; //character array fgets() adds to and sscanf() looks through
	char selection[BUFSIZE];	
	int totalPlayers = 0;
	int playerNumber = 0;
	int botNumber = 0;	
	int turnNumber = 0;
	int i;
	bool gameOver = 0;	
	
	//structure variables
	PlayerProfile Player[MAX_PLAYER_SIZE]; //number of allowed players
	
	//pointer variables
	PlayerProfile* PlayerPtr = NULL;
	Property* BoardPtr = NULL;	
	
	//dynamic memory allocation for pointer(s)
	PlayerPtr = (PlayerProfile*)malloc(sizeof(PlayerProfile));
	BoardPtr = (Property*)malloc(sizeof(Property));
	
	AssignBoardValues (); //assigning the values for each board square

	printf("Welcome to \e[1;33mMonopoly,\e[0m made by Eli Johnson\n\n");
	printf("Would you like to play? Enter 'Y' or 'y' for yes, and enter anything else for no\n\n");
	fgets(buffer, BUFSIZE, stdin); //using fgets() to put the entire user line input into the buf character array
	sscanf(buffer, "%s", &selection); //searches through the buf character array and assigns the first character it finds to the selection
	printf("\n"); //newline is for console readability	
	
	if (((selection[0] != 'Y') && (selection[0] != 'y')) || (strlen(selection) > 1)) { //checks to see if the user didn't enter a yes
		printf("Maybe next time. Have a nice day :D");
		return 0; //ends program because they don't want to play
	} 
	
	selection[0] = '\0';
	
	printf("How many people will be playing? Up to 4 people can play\n\n");
	fgets(buffer, BUFSIZE, stdin); //using fgets() to put the entire user line input into the buf character array
	sscanf(buffer, "%1d", &playerNumber); //searches through the buf character array and assigns all the integers it finds to the player number
	printf("\n"); //newline is for console readability
	
	while ((playerNumber > 4) || (playerNumber <= 0)) { //here to make sure that the user only selects a number of players between 1 and 4
		printf("Please enter a number between 1 and 4\n\n");
		fgets(buffer, BUFSIZE, stdin); 
		sscanf(buffer, "%d", &playerNumber); 
		printf("\n"); //newline is for console readability
	}	
	
	//section to determine how many bot players to add
	if (playerNumber == 1) { //logic to determine how many bots a single player wants to add
		printf("How many bots would you like to have play? ");
		printf("Please enter a number of bots so that the total number of players is at least 2 and at most 4\n\n");
		fgets(buffer, BUFSIZE, stdin); 
		sscanf(buffer, "%d", &botNumber);
		printf("\n");
		
		while (((playerNumber + botNumber) > 4) || (botNumber <= 0)) { //while loop to make sure they pick a valid ammount
			printf("Please enter a number of bots so that the total number of players is at least 2 and at most 4\n\n");
			fgets(buffer, BUFSIZE, stdin); 
			sscanf(buffer, "%d", &botNumber);
			printf("\n"); 
		}	
	} 
	//logic to determine how many bots the players want to add
	else if ((playerNumber >= 2) && (playerNumber < 4)) { //this section only iterates if the number of players isn't 4
		printf("Would you like to have any bots playing? Press 'Y' or 'y' for yes, and anything else for no\n\n");
		fgets(buffer, BUFSIZE, stdin); 
		sscanf(buffer, "%s", &selection);
		printf("\n");
		
		if (((selection[0] == 'Y') || (selection[0] == 'y')) && ((strlen(selection) == 1) && (selection[0] != '\0'))) { //letting the player decide the number of bots
			printf("Please enter a number of bots so that the total number of players is 4 or less\n\n");
			fgets(buffer, BUFSIZE, stdin); 
			sscanf(buffer, "%d", &botNumber);
			printf("\n");
			
			while (((playerNumber + botNumber) > 4) || (botNumber < 0)) { //while loop to make sure they pick a valid ammount
				printf("Please enter a number of bots so that the total number of players is 4 or less\n\n");
				fgets(buffer, BUFSIZE, stdin); 
				sscanf(buffer, "%d", &botNumber);
				printf("\n"); 
			}
		}
		
	}	
	
	selection[0] = '\0';	
	
	totalPlayers = playerNumber + botNumber;
	printf("Total number of players %d: %d players and %d bot(s)\n\n", totalPlayers, playerNumber, botNumber);
	
	printf("Please enter the names of the people who are playing. Names should be 50 characters or less\n\n");
	printf("Play will go in order of names entered\n\n");
	for (i = 0; i < totalPlayers; i++) {
		if (i < playerNumber) {
			printf("Player %d: ", (i + 1));
			fgets(Player[i].playerName, 50, stdin);
			printf("\n");
			while (Player[i].playerName[0] == '\n') { //logic to make sure the player actually enters a name
				printf("Please enter your name\n\n");
				fgets(Player[i].playerName, 50, stdin);
				printf("\n");				
			}
			if (Player[i].playerName[strlen(Player[i].playerName) - 1] == '\n') { //checking to see if the last character in the player's name is a null, and if it isn't, it gets replaced with one
				Player[i].playerName[strlen(Player[i].playerName) - 1] = '\0';
			}
			Player[i].playerOrder = (i + 1);
		}
		else {
			char temp[BUFSIZE]; //temparary character array
			printf("(Bot) Player %d: ", (i + 1));
			strcpy(Player[i].playerName, "(Bot) ");			
			fgets(buffer, BUFSIZE, stdin);
			sscanf(buffer, "%s", temp);
			printf("\n");
			while (Player[i].playerName[0] == '\n') {
				printf("Please enter your name\n\n");
				fgets(Player[i].playerName, 50, stdin);
				printf("\n");				
			}			
			if (temp[strlen(Player[i].playerName) - 1] == '\n') { //checking to see if the last character in the player's name is a null, and if it isn't, it gets replaced with one
				temp[strlen(Player[i].playerName) - 1] = '\0';
			}
			Player[i].playerOrder = (i + 1);
			strcat(Player[i].playerName, temp);
		}
	}	
	
	ClearScreen(); //clear screen function called to make room for the ascii art
	
	for (i = 0; i < totalPlayers; i++) { //flag to make sure that memory has been properly allocated for the players
		if (PlayerPtr == NULL) {
			printf("No Memory allocated\n\n");
		}
		else {
			printf("Memory successfully allocated for %s\n", Player[i].playerName);
		}
	}
	printf("\n");
	
	printf("Please select the board piece you would like to play as\n\n");
	printf("Enter 1 for the boat\n\n");
	PrintBoat(); //function call to print the ascii art
	printf("\n\n");
	printf("Enter 2 for the airplane\n\n");
	PrintAirplane();
	printf("\n\n");
	printf("Enter 3 for the train\n\n");
	PrintTrain();
	printf("\n\n");
	printf("Enter 4 for the helicopter\n\n");
	PrintHelicopter();
	printf("\n\n");	
	
	for (i = 0; i < totalPlayers; i++) { //for loop for the players to pick their board piece
		printf("%s, which board piece do you pick?\n\n", Player[i].playerName);
		fgets(buffer, BUFSIZE, stdin);
		sscanf(buffer, "%1d", &Player[i].boardPiece);
		printf("\n");
		while ((Player[i].boardPiece > 4) || (Player[i].boardPiece <= 0)) { //logic to make sure they pick a valid piece
			printf("Please pick a valid board piece\n\n");
			fgets(buffer, BUFSIZE, stdin);
			sscanf(buffer, "%1d", &Player[i].boardPiece);
			printf("\n");
		}
		if (i != 0) { // the first player doesn't need to check if their piece selection is taken because they pick first
			while ((Player[i].boardPiece == Player[i - 1].boardPiece) || 
				(Player[i].boardPiece == Player[i - 2].boardPiece) || 
				(Player[i].boardPiece == Player[i - 3].boardPiece)) { //logic to make sure there are no duplicate pieces
				printf("Someone else has already picked that piece. Please pick again\n\n");
				fgets(buffer, BUFSIZE, stdin);
				sscanf(buffer, "%1d", &Player[i].boardPiece);
				printf("\n");
			}
		}
	}	
		
	ClearScreen();	
	
	for (i = 0; i < totalPlayers; i++) { //assigning pre-game values for all players`
		AssignPregameValues (&Player[i], i);
	}	
	
	printf("List of players: \n\n");
	for (i = 0; i < totalPlayers; i++) {
		if (i < playerNumber) {
			printf("\e[1;32mPlayer %d:\e[0m \e[0;32m%s\e[0m\n\n", i + 1, Player[i].playerName);
		}
		else {
			printf("\e[1;31mPlayer %d:\e[0m \e[0;31m%s\e[0m\n\n", i + 1, Player[i].playerName);
		}
	}	
	
	printf("Enter anything to start\n\n");
	fgets(buffer, BUFSIZE, stdin);
	sscanf(buffer, "%c", &selection); 
	printf("\n"); 
	
	ClearScreen();	
	
	//ACTUAL GAME LOOP STARTS
	
	while (!gameOver) {  
		
		printf("\e[1;33mLet's Play Monopoly!\e[0m\n");	
		
		i = (turnNumber % totalPlayers);
		
		if (!Player[i].isBankrupt) { //checking to make sure the player is still in the game
			TakeTurnPlayer(turnNumber, playerNumber, totalPlayers, &gameOver, &Player, &Board);
		}
		
		turnNumber++;
		
		//gameOver = 0; //only temparary for testing. CHANGE WHEN FINISHED
		
		//break; //CHANGE WHEN DONE
	} 

	
	printf("Thank you for playing my game of \e[1;33mMonopoly,\e[0m I hope you enjoyed it.\n\n"); //ending message before program terminates
	printf("Have a nice day!\n");	
	
	free(PlayerPtr);
	free(BoardPtr);
	
	return 0;
	
}

//function decleration

void AssignPregameValues (PlayerProfile* PlayerPtr, int i) {
	
	if (PlayerPtr == NULL) {
		return;
	}
	
	PlayerPtr->playerNumber = i;
	PlayerPtr->money = 1500;
	PlayerPtr->boardPosition = 0;
	PlayerPtr->propertyNumber = 0;
	PlayerPtr->railroadNumber = 0;
	PlayerPtr->companyNumber = 0;
	PlayerPtr->colorSets = 0;
	PlayerPtr->houses = 0;
	PlayerPtr->hotels = 0;
	PlayerPtr->getOutOfJailFreeCard = 0;
	PlayerPtr->boardPosition = 0;
	PlayerPtr->isBankrupt = 0; //zero meaning false
	PlayerPtr->propertyID[0] = ""; //initializing the first item in the array to an empty set
	PlayerPtr->propertyColor[0] = "";
	
}

void TakeTurnPlayer (int turnNumber, int numPlayers, int totalPlayers, bool* gameOverPtr, PlayerProfile* PlayerPtr, Property* BoardPtr) {
	
	const int BUFSIZE = 100;
	char buffer[BUFSIZE];	
	char selection[BUFSIZE];	
	
	int turnSelection = 0;
	int currentPlayer = 0;
	int die1Roll;
	int die2Roll;
	int doublesCounter = 0;
	int i;
	
	srand(time(NULL)); //setting random seed for the rand() function	
	
	i = turnNumber % numPlayers;  
	currentPlayer = i;
	
	printf("\e[1;33mCurrent turn: %d\e[0m\n\n", (turnNumber + 1));
	//PrintBoardSquare(playerPointer[i].boardPosition); //function call to print the ascii art of the current tile the player is on
	PrintPiece(PlayerPtr, currentPlayer);
	
	if (currentPlayer < numPlayers) {
		printf("\e[0;32m%s,\e[0m it is your turn. Your current board position is: %s (%d)\n\n", PlayerPtr[i].playerName, monopolyBoard[PlayerPtr[i].boardPosition], PlayerPtr[i].boardPosition);
	}
	else {
		printf("\e[0;31m%s,\e[0m it is your turn. Your current board position is: %s (%d)\n\n", PlayerPtr[i].playerName, monopolyBoard[PlayerPtr[i].boardPosition], PlayerPtr[i].boardPosition);
	}
	
	printf("Current ammount of money: $%d\n\n", PlayerPtr[i].money);
	printf("Which action would you like to take?\n\n");
	
	if (PlayerPtr[i].boardPosition == 40) { //checking to see if the player is in jail
		printf("(1) Roll the dice to try and get doubles to get out of jail\n\n");
		printf("(2) Pay $50 to get out of jail then roll\n\n");
		printf("(3) Use a Get-Out-Of-Jail-Free card\n\n");
		fgets(buffer, BUFSIZE, stdin); 
		sscanf(buffer, "%1d", &turnSelection);		
		
		while ((turnSelection > 3) || (turnSelection <= 0)) { //logic to make sure the player picks a valid option
			printf("Please select a valid option for your turn\n\n");
			fgets(buffer, BUFSIZE, stdin); 
			sscanf(buffer, "%1d", &turnSelection);	
		}
		
	}
	else { //player actions if they are not in jail
		if ((PlayerPtr[i].colorSets == 0) && (PlayerPtr[i].propertyNumber == 0)) { //turn options for a player if they have no property and no color sets
			
			printf("(1) Roll the dice\n\n");
			fgets(buffer, BUFSIZE, stdin); 
			sscanf(buffer, "%1d", &turnSelection);	
			printf("\n");
		
			while (turnSelection != 1) { //logic to make sure the player picks a valid turn option
				printf("Please select a valid option for your turn\n\n");
				fgets(buffer, BUFSIZE, stdin); 
				sscanf(buffer, "%1d", &turnSelection);
				printf("\n");	
			}
			
			if (turnSelection == 1) { //rolling dice function
				
				RollDice(PlayerPtr, currentPlayer, &doublesCounter, &die1Roll, &die2Roll);
				CheckProperty(totalPlayers, currentPlayer, BoardPtr, die1Roll, die2Roll, PlayerPtr); //passing variables for the checking property			
				
				int tempDoublesCounter = 0; //making a temp variable to use for determining if the player rolled doubles again (in the while loop)
				
				while ((doublesCounter - tempDoublesCounter) > 0) { //using the difference between the double counter and the temparary double counter to determine if doubles was rolled
					tempDoublesCounter = doublesCounter; //'incrementing' the temparary doubles counter 
					printf("You rolled doubles! You get to roll again. Enter 1 to do so\n\n");
					fgets(buffer, BUFSIZE, stdin); 
					sscanf(buffer, "%1d", &turnSelection);
					printf("\n");
					
					while (turnSelection != 1) { //making sure the player rolls again
						printf("You have to roll again. You rolled doubles\n\n");
					}
					
					RollDice(PlayerPtr, currentPlayer, &doublesCounter, &die1Roll, &die2Roll);
					CheckProperty(totalPlayers, currentPlayer, BoardPtr, die1Roll, die2Roll, PlayerPtr);
					
					if (doublesCounter == 3) { //the player only gets at most 2 rolls a turn
						printf("You rolled doubles 3 times. You have to go to jail\n\n");
						PlayerPtr[i].boardPosition = 40; //moving the player to the "In Jail" board square
					}
				}
			}
			
		}
		else if (PlayerPtr[i].propertyNumber > 0) { //options for a player if they have at least 1 property
			printf("(1) Roll the dice\n\n");
			
			printf("(2) sell a property\n\n");
			fgets(buffer, BUFSIZE, stdin); 
			sscanf(buffer, "%1d", &turnSelection);	
			printf("\n");
			
			while ((turnSelection > 2) || (turnSelection < 1)) { //logic to make sure the player picks a valid turn option
				printf("Please select a valid option for your turn\n\n");
				fgets(buffer, BUFSIZE, stdin); 
				sscanf(buffer, "%1d", &turnSelection);
				printf("\n");	
			}
			
			if (turnSelection == 1) { //rolling dice function
				
				RollDice(PlayerPtr, currentPlayer, &doublesCounter, &die1Roll, &die2Roll);
				CheckProperty(totalPlayers, currentPlayer, BoardPtr, die1Roll, die2Roll, PlayerPtr); //passing variables for the checking property			
				
				int tempDoublesCounter = 0; //making a temp variable to use for determining if the player rolled doubles again (in the while loop)
				
				while ((doublesCounter - tempDoublesCounter) > 0) { //using the difference between the double counter and the temparary double counter to determine if doubles was rolled
					tempDoublesCounter = doublesCounter; //'incrementing' the temparary doubles counter 
					printf("You rolled doubles! You get to roll again. Enter 1 to do so\n\n");
					fgets(buffer, BUFSIZE, stdin); 
					sscanf(buffer, "%1d", &turnSelection);
					printf("\n");
					
					while (turnSelection != 1) { //making sure the player rolls again
						printf("You have to roll again. You rolled doubles\n\n");
					}
					
					RollDice(PlayerPtr, currentPlayer, &doublesCounter, &die1Roll, &die2Roll);
					CheckProperty(totalPlayers, currentPlayer, BoardPtr, die1Roll, die2Roll, PlayerPtr);
					
					if (doublesCounter == 3) { //the player only gets at most 2 rolls a turn
						printf("You rolled doubles 3 times. You have to go to jail\n\n");
						PlayerPtr[i].boardPosition = 40; //moving the player to the "In Jail" board square
					}
				}
			}
			
			else { //other turn selection
				Sell(PlayerPtr, currentPlayer, BoardPtr); //sell function call
				
				printf("Enter (1) to roll the dice. Enter anything else to end your turn\n\n");
				fgets(buffer, BUFSIZE, stdin); 
				sscanf(buffer, "%c", &selection);
				printf("\n");				
				
				if (selection[0] != '1') { //logic to make sure the player picks a valid turn option
					return;	//ends the function because the user wants to end their turn
				}
				else {
					RollDice(PlayerPtr, currentPlayer, &doublesCounter, &die1Roll, &die2Roll);
					CheckProperty(totalPlayers, currentPlayer, BoardPtr, die1Roll, die2Roll, PlayerPtr); //passing variables for the checking property			
					
					int tempDoublesCounter = 0; //making a temp variable to use for determining if the player rolled doubles again (in the while loop)
					
					while ((doublesCounter - tempDoublesCounter) > 0) { //using the difference between the double counter and the temparary double counter to determine if doubles was rolled
						tempDoublesCounter = doublesCounter; //'incrementing' the temparary doubles counter 
						printf("You rolled doubles! You get to roll again. Enter 1 to do so\n\n");
						fgets(buffer, BUFSIZE, stdin); 
						sscanf(buffer, "%1d", &turnSelection);
						printf("\n");
						
						while (turnSelection != 1) { //making sure the player rolls again
							printf("You have to roll again. You rolled doubles\n\n");
						}
						
						RollDice(PlayerPtr, currentPlayer, &doublesCounter, &die1Roll, &die2Roll);
						CheckProperty(totalPlayers, currentPlayer, BoardPtr, die1Roll, die2Roll, PlayerPtr);
						
						if (doublesCounter == 3) { //the player only gets at most 2 rolls a turn
							printf("You rolled doubles 3 times. You have to go to jail\n\n");
							PlayerPtr[i].boardPosition = 40; //moving the player to the "In Jail" board square
						}
					}					
				}
				
			}
		}
		else if (PlayerPtr[i].colorSets > 0) { //turn options for a player if they have a color set 
			printf("(1) Roll the dice\n\n");
			printf("(2) Buy a house\n\n");
			printf("(3) Buy a hotel\n\n");
			printf("(4) Sell a house\n\n");
			printf("(5) Sell a hotel\n\n");			
			printf("(6) sell a property\n\n");
			fgets(buffer, BUFSIZE, stdin); 
			sscanf(buffer, "%1d", &turnSelection);			
		}
	}
	
	printf("You can't do anything else this turn\n\n");
	printf("Enter 'q' to end your turn\n\n"); //logic to make sure that the player actually ends their turn when they want
	fgets(buffer, BUFSIZE, stdin);
	sscanf(buffer, "%c", &selection);
	printf("\n\n");
	while (selection[0] != 'q') {
		fgets(buffer, BUFSIZE, stdin);
		sscanf(buffer, "%c", &selection);
	}
	
	ClearScreen();
	
}	

void CheckProperty (int totalPlayers, int currentPlayer, Property* BoardPtr, int die1, int die2, PlayerProfile* PlayerPtr) {
	
	const int BUFSIZE = 100;
	char buffer[BUFSIZE];	
	char selection[BUFSIZE];
	int turnSelection;
	int i;
	int dieTotal = die1 + die2;
	
	if (BoardPtr[PlayerPtr[currentPlayer].boardPosition].isOwned) { //checking to see if the player has to pay rent for the property they landed on
		
		for (i = 0; i < totalPlayers; i++) {
			if (BoardPtr[PlayerPtr[currentPlayer].boardPosition].playerOwner = PlayerPtr[i].playerNumber) {
				printf("You already own this property\n\n");
				break;
			}
			
			else {
				
				if (BoardPtr[PlayerPtr[currentPlayer].boardPosition].company) { //checking to see if where the player landed is a company
				
					for (int j = 0; j < (strlen(*(PlayerPtr[j].propertyID)) - 1); j++) { //loop to iterate through each item in the array of properties owned 
						if (strcmp(PlayerPtr[i].propertyID[j], monopolyBoard[PlayerPtr[currentPlayer].boardPosition])) { //checking each element in the character array to see if it matches the property name
							printf("That property is already owned by %s\n\n", PlayerPtr[i].playerName);
							
							if (PlayerPtr[j].companyNumber == 1) { //checking to see how many companies the player owns
								printf("The rent for %s is $%d\n\n", BoardPtr[PlayerPtr[i].boardPosition].name, (4 * dieTotal));
								PayRent(PlayerPtr, BoardPtr, currentPlayer, (4 * dieTotal));
								break;
							}
							else {
								printf("The rent for %s is $%d\n\n", BoardPtr[PlayerPtr[i].boardPosition].name, (10 * dieTotal));
								PayRent(PlayerPtr, BoardPtr, currentPlayer, (10 * dieTotal));
								break;
							}
						}
						else {
							break;
						}
					}	
				}
				
				else if (BoardPtr[PlayerPtr[currentPlayer].boardPosition].railroad) {
					
					for (int j = 0; j < PlayerPtr[j].propertyNumber; j++) { //loop to iterate through each item in the array of properties owned 
						if (strcmp(PlayerPtr[i].propertyID[j], monopolyBoard[PlayerPtr[currentPlayer].boardPosition])) { 
							printf("That property is already owned by %s\n\n", PlayerPtr[i].playerName);
							
							if (PlayerPtr[i].railroadNumber == 1) {
								printf("The rent for %s is $%d\n\n", BoardPtr[PlayerPtr[i].boardPosition].name, 25);
								PayRent(PlayerPtr, BoardPtr, currentPlayer, 25);
								break;
							}
							else if (PlayerPtr[i].railroadNumber == 2) {
								printf("The rent for %s is $%d\n\n", BoardPtr[PlayerPtr[i].boardPosition].name, 50);
								PayRent(PlayerPtr, BoardPtr, currentPlayer, 50);
								break;
							}
							else if (PlayerPtr[i].railroadNumber == 3) {
								printf("The rent for %s is $%d\n\n", BoardPtr[PlayerPtr[i].boardPosition].name, 100);
								PayRent(PlayerPtr, BoardPtr, currentPlayer, 100);
								break;
							}
							else {
								printf("The rent for %s is $%d\n\n", BoardPtr[PlayerPtr[i].boardPosition].name, 200);
								PayRent(PlayerPtr, BoardPtr, currentPlayer, 200);
								break;
							}
						}
						else {
							break;
						}
					}					
				}
				
				else if (BoardPtr[PlayerPtr[currentPlayer].boardPosition].numHouses == 0) {
					
					for (int j = 0; j < PlayerPtr[i].propertyNumber; j++) { //loop to iterate through each item in the array of properties owned
						if (strcmp(PlayerPtr[i].propertyID[j], monopolyBoard[PlayerPtr[currentPlayer].boardPosition])) { 
							printf("That property is already owned by %s\n\n", PlayerPtr[i].playerName);
							printf("The rent for %s is $%d\n\n", BoardPtr[PlayerPtr[currentPlayer].boardPosition].name, BoardPtr[PlayerPtr[currentPlayer].boardPosition].rent);
							PayRent(PlayerPtr, BoardPtr, currentPlayer, BoardPtr[PlayerPtr[currentPlayer].boardPosition].rent);
							break;
						}
						else {
							break;
						}
					}
				}
				
				else if (BoardPtr[PlayerPtr[currentPlayer].boardPosition].numHouses == 1) {
					
					for (int j = 0; j < PlayerPtr[i].propertyNumber; j++) { //loop to iterate through each item in the array of properties owned
						if (strcmp(PlayerPtr[i].propertyID[j], monopolyBoard[PlayerPtr[currentPlayer].boardPosition])) { 
							printf("That property is already owned by %s\n\n", PlayerPtr[i].playerName);
							printf("The rent for %s is $%d\n\n", BoardPtr[PlayerPtr[currentPlayer].boardPosition].name, BoardPtr[PlayerPtr[currentPlayer].boardPosition].rent1House);
							PayRent(PlayerPtr, BoardPtr, currentPlayer, BoardPtr[PlayerPtr[currentPlayer].boardPosition].rent1House);
							break;							
						}
						else {
							break;
						}
					}
				}
				
				else if (BoardPtr[PlayerPtr[currentPlayer].boardPosition].numHouses == 2) {
					
					for (int j = 0; j < PlayerPtr[i].propertyNumber; j++) { //loop to iterate through each item in the array of properties owned
						if (strcmp(PlayerPtr[i].propertyID[j], monopolyBoard[PlayerPtr[currentPlayer].boardPosition])) { 
							printf("That property is already owned by %s\n\n", PlayerPtr[i].playerName);
							printf("The rent for %s is $%d\n\n", BoardPtr[PlayerPtr[currentPlayer].boardPosition].name, BoardPtr[PlayerPtr[currentPlayer].boardPosition].rent2House);
							PayRent(PlayerPtr, BoardPtr, currentPlayer, BoardPtr[PlayerPtr[currentPlayer].boardPosition].rent2House);
							break;						
						}
						else {
							break;
						}
					}
				}
				
				else if (BoardPtr[PlayerPtr[currentPlayer].boardPosition].numHouses == 3) {
					
					for (int j = 0; j < PlayerPtr[i].propertyNumber; j++) { //loop to iterate through each item in the array of properties owned
						if (strcmp(PlayerPtr[i].propertyID[j], monopolyBoard[PlayerPtr[currentPlayer].boardPosition])) { 
							printf("That property is already owned by %s\n\n", PlayerPtr[i].playerName);
							printf("The rent for %s is $%d\n\n", BoardPtr[PlayerPtr[currentPlayer].boardPosition].name, BoardPtr[PlayerPtr[currentPlayer].boardPosition].rent3House);
							PayRent(PlayerPtr, BoardPtr, currentPlayer, BoardPtr[PlayerPtr[currentPlayer].boardPosition].rent3House);
							break;						
						}
						else {
							break;
						}
					}
				}
				
				else if (BoardPtr[PlayerPtr[currentPlayer].boardPosition].numHouses == 4) {
					
					for (int j = 0; j < PlayerPtr[i].propertyNumber; j++) { //loop to iterate through each item in the array of properties owned
						if (strcmp(PlayerPtr[i].propertyID[j], monopolyBoard[PlayerPtr[currentPlayer].boardPosition])) { 
							printf("That property is already owned by %s\n\n", PlayerPtr[i].playerName);
							printf("The rent for %s is $%d\n\n", BoardPtr[PlayerPtr[currentPlayer].boardPosition].name, BoardPtr[PlayerPtr[currentPlayer].boardPosition].rent4House);
							PayRent(PlayerPtr, BoardPtr, currentPlayer, BoardPtr[PlayerPtr[currentPlayer].boardPosition].rent4House);
							break;						
						}
						else {
							break;
						}
					}
				}
				
				else if (BoardPtr[PlayerPtr[currentPlayer].boardPosition].hasHotel) {
					
					for (int j = 0; j < PlayerPtr[i].propertyNumber; j++) { //loop to iterate through each item in the array of properties owned
						if (strcmp(PlayerPtr[i].propertyID[j], monopolyBoard[PlayerPtr[currentPlayer].boardPosition])) { 
							printf("That property is already owned by %s\n\n", PlayerPtr[i].playerName);
							printf("The rent for %s is $%d\n\n", BoardPtr[PlayerPtr[currentPlayer].boardPosition].name, BoardPtr[PlayerPtr[currentPlayer].boardPosition].rentHotel);
							PayRent(PlayerPtr, BoardPtr, currentPlayer, BoardPtr[PlayerPtr[currentPlayer].boardPosition].rentHotel);
							break;						
						}
						else {
							break;
						}
					}
				}	
			}
			
		}
	}
	else {
		if (BoardPtr[PlayerPtr[currentPlayer].boardPosition].isBuyable) {
			printf("Which action would you like to take?\n\n");
			printf("(1) Buy the property\n\n");
			printf("(2) End your turn\n\n");
			fgets(buffer, BUFSIZE, stdin); 
			sscanf(buffer, "%1d", &turnSelection);
			printf("\n");
			if (turnSelection == 1) {
				BuyProperty(PlayerPtr, currentPlayer, BoardPtr);
			}
		}
	else if (BoardPtr[PlayerPtr[currentPlayer].boardPosition].drawCard) { //drawCard bool being true means they landed on either community chest or chance
			printf("Enter 1 to draw your card\n\n");
			fgets(buffer, BUFSIZE, stdin); 
			sscanf(buffer, "%1d", &turnSelection);
			printf("\n");
			
			while ((turnSelection > 1) || (turnSelection <= 0)) { //logic to make sure the player picks a valid turn option
				printf("Please draw your card\n\n");
				fgets(buffer, BUFSIZE, stdin); 
				sscanf(buffer, "%1d", &turnSelection);
				printf("\n");	
			}
			
			//FIX ME
			DrawCard(totalPlayers, currentPlayer, BoardPtr, die1, die2, PlayerPtr); //total player number needed for some specific chance/community chest cards
		}	
	else if ((BoardPtr[PlayerPtr[currentPlayer].boardPosition].price) < 0) { //price less than 0 means its a tax
			printf("You need to pay $%d\n\n", abs(BoardPtr[PlayerPtr[currentPlayer].boardPosition].price));
			PlayerPtr[currentPlayer].money += BoardPtr[PlayerPtr[currentPlayer].boardPosition].price;
			printf("Your remaining money: $%d\n\n", PlayerPtr[currentPlayer].money);
		}
	else {
			return; //exiting the loop because the player can't do anything
		}
	}
}

void BuyProperty (PlayerProfile* PlayerPtr, int currentPlayer, Property* BoardPtr) {

	const int BUFSIZE = 100;
	char buffer[BUFSIZE];	
	char selection[BUFSIZE];	
	char confirm[BUFSIZE];

	printf("%s costs $%d, and you have $%d\n\n", BoardPtr[PlayerPtr[currentPlayer].boardPosition].name, BoardPtr[PlayerPtr[currentPlayer].boardPosition].price, PlayerPtr[currentPlayer].money);
	printf("Are you sure you want to purchase the property?\n\n");
	printf("Enter: Yes\n\n");
	printf("Anything else: No\n\n");
	
	fgets(buffer, BUFSIZE, stdin);
	sscanf(buffer, "%c", &confirm);
	printf("\n");
	
	if ((confirm[0] != '\n') && (strlen(confirm) > 1)) {
		printf("You have elected to not buy the property\n\n");
		return;
	}
	else {

		if (PlayerPtr[currentPlayer].money < BoardPtr[PlayerPtr[currentPlayer].boardPosition].price) {
			printf("You don't have enough money to buy this property\n\n");
		}
		else {
			PlayerPtr[currentPlayer].money -= BoardPtr[PlayerPtr[currentPlayer].boardPosition].price; //subtracting the price of the property from the player's money
			BoardPtr[PlayerPtr[currentPlayer].boardPosition].isOwned = 1; //setting the status of owned to true
			BoardPtr[PlayerPtr[currentPlayer].boardPosition].playerOwner = PlayerPtr[currentPlayer].playerNumber; //setting the number of board owner equal to the player number of the player that bought it
			PlayerPtr[currentPlayer].propertyID[PlayerPtr[currentPlayer].propertyNumber] = BoardPtr[PlayerPtr[currentPlayer].boardPosition].name; //adding the name of the property to the player's array 
			PlayerPtr[currentPlayer].propertyNums[PlayerPtr[currentPlayer].propertyNumber] = BoardPtr[PlayerPtr[currentPlayer].boardPosition].boardNum;
			
			if (BoardPtr[PlayerPtr[currentPlayer].boardPosition].company) { //checking to see if the property is a company or railroad
				++PlayerPtr[currentPlayer].companyNumber;
			}
			else if (BoardPtr[PlayerPtr[currentPlayer].boardPosition].railroad) {
				++PlayerPtr[currentPlayer].railroadNumber;
			}
			else {
				PlayerPtr[currentPlayer].propertyColor[PlayerPtr[currentPlayer].propertyNumber] = BoardPtr[PlayerPtr[currentPlayer].boardPosition].color;
			}
			
			printf("You have purchased %s\n\n", BoardPtr[PlayerPtr[currentPlayer].boardPosition].name);
			printf("Your remaining money: $%d\n\n", PlayerPtr[currentPlayer].money);
			++PlayerPtr[currentPlayer].propertyNumber;
		}
	}
	
}

void Sell (PlayerProfile* PlayerPtr, int currentPlayer, Property* BoardPtr) {
	
	const int BUFSIZE = 100;
	char buffer[BUFSIZE];	
	int selection;	
	
	int i;
	int sellPrice;
	
	while (PlayerPtr[currentPlayer].money < 0) {
		printf("Which property would you like to sell?\n\n");
		for (i = 0; i < PlayerPtr[currentPlayer].propertyNumber; i++) { //listing the properties that the current player has
			printf("Enter %d for %s (%d houses). Sell value: $%d\n\n", i, (PlayerPtr[currentPlayer].propertyID[i]), BoardPtr[PlayerPtr[currentPlayer].propertyNums[i]].numHouses, BoardPtr[PlayerPtr[currentPlayer].propertyNums[i]].sellValue);
		} 

		fgets(buffer, BUFSIZE, stdin); 
		sscanf(buffer, "%d", &selection);
		printf("\n");		
			
		if ((selection > i) || (selection < -1)) { //making sure the player pickes a valid option
			printf("Please select a valid option for your turn\n\n");
			fgets(buffer, BUFSIZE, stdin); 
			sscanf(buffer, "%1d", &selection);
			printf("\n");			
		}
		
		if (BoardPtr[PlayerPtr[currentPlayer].propertyNums[selection]].numHouses > 0) { //checking if the board space the player picked has any houses
			printf("How many houses would you like to sell?\n\n");
			
		}
		
		printf("You sold %s\n\n", PlayerPtr[currentPlayer].propertyID[selection]);
		--PlayerPtr[currentPlayer].propertyNumber;
		PlayerPtr[currentPlayer].money += BoardPtr[PlayerPtr[currentPlayer].propertyNums[selection]].sellValue;
		PlayerPtr[currentPlayer].propertyID[selection] = "";
		PlayerPtr[currentPlayer].propertyColor[selection] = "";
		BoardPtr[PlayerPtr[currentPlayer].propertyNums[selection]].isOwned = 0;
		BoardPtr[PlayerPtr[currentPlayer].propertyNums[selection]].playerOwner = -1;
		PlayerPtr[currentPlayer].propertyNums[selection] = -1;
		
		printf("Your new money total: $%d\n\n", PlayerPtr[currentPlayer].money);		
	}		
	
}

void DrawCard (int totalPlayers, int currentPlayer, Property* BoardPtr, int die1, int die2, PlayerProfile* PlayerPtr) {
	
	srand(time(NULL)); //setting random seed
	
	int randomCardDraw = rand() % 16; //random number between 0 and 15
	
	if ((PlayerPtr[currentPlayer].boardPosition == 2) || (PlayerPtr[currentPlayer].boardPosition == 17) || (PlayerPtr[currentPlayer].boardPosition == 33)) { //checking to see if the player landed on a community chest space
		printf("The card you drew says \"%s\"\n\n", CommunityChestCards[randomCardDraw]);
		
		ActionCommunityChest (PlayerPtr, randomCardDraw, totalPlayers, currentPlayer, BoardPtr);
		
	}
	else if ((PlayerPtr[currentPlayer].boardPosition == 7) || (PlayerPtr[currentPlayer].boardPosition == 22) || (PlayerPtr[currentPlayer].boardPosition == 36)) { //checking to see if the player landed on a chance space
		printf("The card you drew says \"%s\"\n\n", ChanceCards[randomCardDraw]);
		
		//ActionChance (totalPlayers, currentPlayer, BoardPtr, die1Roll, die2Roll, PlayerPtr, randomCardDraw);
		
	}
}

void ActionCommunityChest (PlayerProfile* PlayerPtr, int cardNum, int totalPlayers, int currentPlayer, Property* BoardPtr) {

	if (cardNum == 0) { //advance to go
		PlayerPtr[currentPlayer].boardPosition = 0;
		PlayerPtr[currentPlayer].money += 200;
		printf("Your new money total: $%d\n\n", PlayerPtr[currentPlayer].money);
	}
	else if (cardNum == 1) { //bank error
		PlayerPtr[currentPlayer].money += 200;
		printf("Your new money total: $%d\n\n", PlayerPtr[currentPlayer].money);
	}
	else if (cardNum == 2) { //doctor fee
		PlayerPtr[currentPlayer].money -= 50;
		if ((PlayerPtr[currentPlayer].money) < 0) { //checking to see if the player can afford this action
			printf("You can't afford that action\n\n");
			if (PlayerPtr[currentPlayer].propertyNumber < 0) { //checking to see if the player has anything they can sell
				printf("You don't have any properties to sell. Game Over\n\n");
				PlayerPtr[currentPlayer].isBankrupt = 1; //setting the current player's game over status to true
				return; //exiting the loop
			}
			else { //calling the sell function to let the player sell things
				while (PlayerPtr[currentPlayer].money < 0) { //calling the sell function enough times to let the player (possible) sell enough things to stay in the game
					Sell (PlayerPtr, currentPlayer, BoardPtr);
					if (PlayerPtr[currentPlayer].propertyNumber < 0) { //checking to see if the player has anything they can sell
						printf("You don't have any properties to sell. Game Over\n\n");
						PlayerPtr[currentPlayer].isBankrupt = 1; //setting the current player's game over status to true
						return; //exiting the loop
					}
				}	
			}
		}
		else {
			printf("Your new money total: $%d\n\n", PlayerPtr[currentPlayer].money);
		}
	}
	else if (cardNum == 3) { //stock sale
		PlayerPtr[currentPlayer].money += 50;
		printf("Your new money total: $%d\n\n", PlayerPtr[currentPlayer].money);
	}
	else if (cardNum == 4) { //get out of jail free
		++PlayerPtr[currentPlayer].getOutOfJailFreeCard;
		printf("Your updated get out of jail free card number: %d\n\n", PlayerPtr[currentPlayer].getOutOfJailFreeCard);
	}
	else if (cardNum == 5) { //go to jail
		PlayerPtr[currentPlayer].boardPosition = 40;
		printf("Your new money total: $%d\n\n", PlayerPtr[currentPlayer].money);
	}
	else if (cardNum == 6) { //holiday fund
		PlayerPtr[currentPlayer].money += 100;
		printf("Your new money total: $%d\n\n", PlayerPtr[currentPlayer].money);
	}
	else if (cardNum == 7) { //tax refund
		PlayerPtr[currentPlayer].money += 20;
		printf("Your new money total: $%d\n\n", PlayerPtr[currentPlayer].money);
	}
	else if (cardNum == 8) { //birthday
		PlayerPtr[currentPlayer].money += (10 * (totalPlayers - 1)); //one less than total player number because the player who drew the card isn't paying themself
		printf("Your new money total: $%d\n\n", PlayerPtr[currentPlayer].money);
	}
	else if (cardNum == 9) { //life insurance
		PlayerPtr[currentPlayer].money += 100;
		printf("Your new money total: $%d\n\n", PlayerPtr[currentPlayer].money);
	}
	else if (cardNum == 10) { //hospital fee
		PlayerPtr[currentPlayer].money -= 100;
		if ((PlayerPtr[currentPlayer].money) < 0) { //checking to see if the player can afford this action
			printf("You can't afford that action\n\n");
			if (PlayerPtr[currentPlayer].propertyNumber < 0) { //checking to see if the player has anything they can sell
				printf("You don't have any properties to sell. Game Over\n\n");
				PlayerPtr[currentPlayer].isBankrupt = 1; //setting the current player's game over status to true
				return; //exiting the loop
			}
			else { //calling the sell function to let the player sell things
				while (PlayerPtr[currentPlayer].money < 0) { //calling the sell function enough times to let the player (possible) sell enough things to stay in the game
					Sell (PlayerPtr, currentPlayer, BoardPtr);
					if (PlayerPtr[currentPlayer].propertyNumber < 0) { //checking to see if the player has anything they can sell
						printf("You don't have any properties to sell. Game Over\n\n");
						PlayerPtr[currentPlayer].isBankrupt = 1; //setting the current player's game over status to true
						return; //exiting the loop
					}
				}	
			}
		}
		else {
			printf("Your new money total: $%d\n\n", PlayerPtr[currentPlayer].money);
		}
	}
	else if (cardNum == 11) { //school fee
		PlayerPtr[currentPlayer].money -= 50;
		
		printf("Your new money total: $%d\n\n", PlayerPtr[currentPlayer].money);
	}
	else if (cardNum == 12) { //consultancy fee
		PlayerPtr[currentPlayer].money += 25;
		printf("Your new money total: $%d\n\n", PlayerPtr[currentPlayer].money);
	}
	else if (cardNum == 13) { //street repair
		PlayerPtr[currentPlayer].money -= ((40 * PlayerPtr[currentPlayer].houses) + (115 * PlayerPtr[currentPlayer].hotels));
		if ((PlayerPtr[currentPlayer].money) < 0) { //checking to see if the player can afford this action
			printf("You can't afford that action\n\n");
			if (PlayerPtr[currentPlayer].propertyNumber < 0) { //checking to see if the player has anything they can sell
				printf("You don't have any properties to sell. Game Over\n\n");
				PlayerPtr[currentPlayer].isBankrupt = 1; //setting the current player's game over status to true
				return; //exiting the loop
			}
			else { //calling the sell function to let the player sell things
				while (PlayerPtr[currentPlayer].money < 0) { //calling the sell function enough times to let the player (possible) sell enough things to stay in the game
					Sell (PlayerPtr, currentPlayer, BoardPtr);
					if (PlayerPtr[currentPlayer].propertyNumber < 0) { //checking to see if the player has anything they can sell
						printf("You don't have any properties to sell. Game Over\n\n");
						PlayerPtr[currentPlayer].isBankrupt = 1; //setting the current player's game over status to true
						return; //exiting the loop
					}
				}	
			}
		}
		else {
			printf("Your new money total: $%d\n\n", PlayerPtr[currentPlayer].money);
		} 
	}
	else if (cardNum == 14) { //beauty contest
		PlayerPtr[currentPlayer].money += 10;
		printf("Your new money total: $%d\n\n", PlayerPtr[currentPlayer].money);
	}
	else { //inheritance
		PlayerPtr[currentPlayer].money += 100;
		printf("Your new money total: $%d\n\n", PlayerPtr[currentPlayer].money);
	}
	
}

/* FIX ME 

void ActionChance (int totalPlayers, int currentPlayer, Property* BoardPtr, int die1, int die2, PlayerProfile* PlayerPtr, randomCardDraw) {
	
	if (cardNum == 0) { //boardwalk
		PlayerPtr[currentPlayer].boardPosition = 39;
		printf("Your new board position is %d\n\n", PlayerPtr[currentPlayer].boardPosition);
	}
	else if (cardNum == 1) { //go
		PlayerPtr[currentPlayer].boardPosition = 0;
		PlayerPtr[currentPlayer].money += 200;
		printf("Your new board position is %d\n\n", PlayerPtr[currentPlayer].boardPosition);
	}
	else if (cardNum == 2) { //Illinois
		if (PlayerPtr[currentPlayer].boardPosition > 24) { //checking to see if the player passed go
			PlayerPtr[currentPlayer].money += 200; //the player passed go, so they get $200
			printf("You passed Go. You get $200\n\n");
		}
		PlayerPtr[currentPlayer].boardPosition = 24;
		printf("Your new board position is %d\n\n", PlayerPtr[currentPlayer].boardPosition);
		CheckProperty(totalPlayers, currentPlayer, BoardPtr, die1Roll, die2Roll, PlayerPtr);
	}
	else if (cardNum == 3) { //st. charles place
		if (PlayerPtr[currentPlayer].boardPosition > 11) {
			PlayerPtr[currentPlayer].money += 200;
			printf("You passed Go. You get $200\n\n");
		}
		PlayerPtr[currentPlayer].boardPosition = 11;
		printf("Your new board position is %d\n\n", PlayerPtr[currentPlayer].boardPosition);
		CheckProperty(totalPlayers, currentPlayer, BoardPtr, die1Roll, die2Roll, PlayerPtr);
	}
	else if ((cardNum == 4) || (cardNum == 5)) { //nearest railroad
		int temp1 = (5 - PlayerPtr[currentPlayer].boardPosition); //assigning temparary variables to the railroads for comparison
		int temp2 = (15 - PlayerPtr[currentPlayer].boardPosition);
		int temp3 = (25 - PlayerPtr[currentPlayer].boardPosition);
		int temp4 = (35 - PlayerPtr[currentPlayer].boardPosition);
		
		if (temp1 > 0) { //checking which railroad the player moves to
			PlayerPtr[currentPlayer].boardPosition = 5;
			printf("Your new board position is %d\n\n", PlayerPtr[currentPlayer].boardPosition);
			CheckProperty(totalPlayers, currentPlayer, BoardPtr, die1Roll, die2Roll, PlayerPtr);
		}
		else if (temp2 > 0) {
			PlayerPtr[currentPlayer].boardPosition = 15;
			printf("Your new board position is %d\n\n", PlayerPtr[currentPlayer].boardPosition);
			CheckProperty(totalPlayers, currentPlayer, BoardPtr, die1Roll, die2Roll, PlayerPtr);
		}
		else if (temp3 > 0) {
			PlayerPtr[currentPlayer].boardPosition = 25;
			printf("Your new board position is %d\n\n", PlayerPtr[currentPlayer].boardPosition);
			CheckProperty(totalPlayers, currentPlayer, BoardPtr, die1Roll, die2Roll, PlayerPtr);
		}
		else if (temp4 > 0) {
			playerPtr[currentPlayer].boardPosition = 35;
			printf("Your new board position is %d\n\n", PlayerPtr[currentPlayer].boardPosition);
			CheckProperty(totalPlayers, currentPlayer, BoardPtr, die1Roll, die2Roll, PlayerPtr);
		}
		else { //if all the values are negetive, that means the player passes go
			PlayerPtr[currentPlayer].boardPosition = 5;
			PlayerPtr[currentPlayer].money += 200;
			printf("You passed Go. You get $200\n\n");
			printf("Your new board position is %d\n\n", PlayerPtr[currentPlayer].boardPosition);
			CheckProperty(totalPlayers, currentPlayer, BoardPtr, die1Roll, die2Roll, PlayerPtr);
		}
		
	}
	else if (cardNum == 6) { //nearest utility
		
	}
	else if (cardNum == 7) { bank pays you
		
	}
	else if (cardNum == 8) { // get out of jail free
		
	}
	else if (cardNum == 9) { //go back 3 spaces
		
	}
	else if (cardNum == 10) { //go to jail
		
	}
	else if (cardNum == 11) { //general repairs
		
	}
	else if (cardNum == 12) { //speeding fine
		
	}
	else if (cardNum == 13) { //trip to reading railroad
		PlayerPtr
	}
	else if (cardNum == 14) { //board chairman
		PlayerPtr[currentPlayer].money -= (50 * (totalPlayers - 1));
	}
	else { // building loan matures
		PlayerPtr[currentPlayer].money += 150;
	}	
	
}

*/

void PayRent (PlayerProfile* PlayerPtr, Property* BoardPtr, int currentPlayer, int rent) {
	
	const int BUFSIZE = 100;
	char buffer[BUFSIZE];	
	char selection[BUFSIZE];	
	
	printf("Enter 1 to pay rent\n\n");
	fgets(buffer, BUFSIZE, stdin);
	sscanf(buffer, "%c", &selection);
	
	while (selection[0] != '1') {
		
	}
	
	if (rent > PlayerPtr[currentPlayer].money) {
		printf("You don't have enough money to pay rent\n\n");
		Sell (PlayerPtr, currentPlayer, BoardPtr);
	}
	else {
		PlayerPtr[currentPlayer].money -= rent;
		printf("You paid $%d in rent. Your remaining money is $%d\n\n", rent, PlayerPtr[currentPlayer].money);
	}
	
}

void RollDice (PlayerProfile* PlayerPtr, int i, int* doublesCounter, int* die1Ptr, int* die2Ptr) {
	
	int die1Roll = 0;
	int die2Roll = 0;
	
	printf("\e[0;32m%s\e[0m is rolling the dice\n\n", PlayerPtr[i].playerName);
	die1Roll = (rand() % 6) + 1; //%6 gives a random number from 0-5, so you need to add 1 to get 1-6 
	die2Roll = (rand() % 6) + 1;
	
	die1Ptr = &die1Roll; //assigning pointer values for later calculations
	die2Ptr = &die2Roll;
	
	printf("\e[0;32m%s\e[0m rolled a %d. (%d + %d)\n\n", PlayerPtr[i].playerName, (die1Roll + die2Roll), die1Roll, die2Roll);
	
	if ((PlayerPtr[i].boardPosition + (die1Roll + die2Roll)) > 39) {
		PlayerPtr[i].boardPosition += (die1Roll + die2Roll);
		printf("You passed Go! you get $200\n\n");
		printf("Your new board position is %s\n\n", monopolyBoard[PlayerPtr[i].boardPosition - 40]);
		PlayerPtr[i].money += 200;
		PlayerPtr[i].boardPosition = ((PlayerPtr[i].boardPosition + (die1Roll + die2Roll)) - 40);
	}
	else {
		PlayerPtr[i].boardPosition += (*die1Ptr + *die2Ptr); //updating player position
		printf("Your new board position is %s (%d)\n\n", monopolyBoard[PlayerPtr[i].boardPosition], PlayerPtr[i].boardPosition);
	}
	
	if (die1Roll == die2Roll) {
		++*doublesCounter;
	}

}

void PrintPiece (PlayerProfile* PlayerPtr, int currentPlayer) {
	if (PlayerPtr[currentPlayer].boardPiece == 1) {
		PrintBoat();
		printf("\n\n");
	}
	else if (PlayerPtr[currentPlayer].boardPiece == 2) {
		PrintAirplane();
		printf("\n\n");
	}
	else if (PlayerPtr[currentPlayer].boardPiece == 3) {
		PrintTrain();
		printf("\n\n");
	}
	else {
		PrintHelicopter();
		printf("\n\n");
	}
}

void ClearScreen () {
	printf("\ec"); //escape character for clearing the terminal
}

