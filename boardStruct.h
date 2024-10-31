//Preprocessor directives
#ifndef boardStruct_h
#define boardStruct_h

//Structure definitions
 typedef struct Property_struct { //variables for each board square
	char name[21];
	char color[11];
	int price;
	int rent;
	int rent1House;
	int rent2House;
	int rent3House;
	int rent4House;
	int rentHotel;	
	int sellValue;
	int boardNum;
	int playerOwner;
	int numHouses;
	int houseBuyValue;
	int houseSellValue;
	bool hasHotel;
	bool company;
	bool railroad;
	bool isBuyable;
	bool isOwned;
	bool drawCard;
}Property;

//structure variables
Property Board[41];	

//Global variables
const char *monopolyBoard[] = {
	"Go Square",
	"Mediterranean Avenue",
	"Commuity Chest",
	"Baltic Avenue",
	"Income Tax",
	"Reading Railroad",
	"Oriental Avenue",
	"Chance",
	"Vermont Avenue",
	"Connecticut Avenue",
	"Just Visiting Jail",
	"St. Charles Place",
	"Electric Company",
	"States Avenue",
	"Virginia Avenue",
	"Pennsylvania Railroad",
	"St. James Place",
	"Community Chest",
	"Tennessee Avenue",
	"New York Avenue",
	"Free Parking",
	"Kentucky Avenue",
	"Chance",
	"Indiana Avenue",
	"Illinois Avenue",
	"B. & O. Railroad",
	"Atlantic Avenue",
	"Ventnor Avenue",
	"Water Works",
	"Marvin Gardens",
	"GO TO JAIL",
	"Pacific Avenue",
	"North Carolina Avenue",
	"Commuity Chest",
	"Pennsylvania Avenue",
	"Short Line Railroad",
	"Chance",
	"Park Place",
	"Luxury Tax",
	"Boardwalk",
	"IN JAIL"
};
	
#endif
