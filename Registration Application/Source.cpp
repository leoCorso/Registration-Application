#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<stdlib.h>
#include<sstream>
#include <ctime>  
#include <stdio.h>
#include <time.h>
#include<chrono>
#include<Windows.h>
#include<mmsystem.h>
#include <algorithm>

#pragma comment(lib, "Winmm.lib")


static char main_Menu()//Prompts user to post or view message threads.
{
	char choice = ' ';

	std::cout << "\n**********************************\n           MAIN MENU \n**********************************\n\n";
	std::cout << "\n[P] - Post Menu\n[D] - DM Menu.\n[F] - Friends Menu.\n[S] - Settings Menu.\n[C] - Clear Console.\n[L] - Log Out.\n\n>";
	std::cin >> choice;
	choice = toupper(choice);

	while (choice != 'P' && choice != 'V' && choice != 'L' && choice != 'D' && choice != 'F' && choice != 'C') {
		std::cout << "\n**********************************\n           MAIN MENU \n**********************************\n\n";
		std::cout << "\n[Please enter a valid option!]" << std::endl;
		std::cout << "\n[P] - Post Menu\n[D] - DM Menu.\n[A] - Friends Menu.\n[S] - Settings Menu.\n[C] - Clear Console.\n[L] - Log Out.\n\n>";
		std::cin >> choice;
		choice = toupper(choice);
	}
	return choice;

}

bool findUsers(std::string toUser) { //Check if a user with that name exists.
	std::ifstream DM("C:/Users/19097/Desktop/Programs/Registration Application/App files/usernames/usernames.txt"); //File with all usernames.
	std::string compareuser;
	bool foundUser = false;

	std::cin.ignore();
	while (std::getline(DM, compareuser)) {
		if (toUser == compareuser) {
			foundUser = true;
			break;
		}
	}
	return foundUser;
}
void displayUsers() { //Displays a list of all usernames.
	std::ifstream Users("C:/Users/19097/Desktop/Programs/Registration Application/App files/usernames/usernames.txt");
	std::string user;
	std::cout << std::endl;
	int count = 0;

	while (getline(Users, user)) {
		std::cout << "-- [" << user << "]" << std::endl;
		count++;
	}
	std::cout << "\n***" << count << " users found***\n\n";
}

//Things to add for settings(User& currentUser){}
// Delte Friends
// Delete DM's
// Delete DM Contact
//Change UserName

class logAction { //Logs user, time and system actions by overloading action functions.

public:
	logAction(std::string action) { //No user only string information logged.
		std::ofstream log("C:/Users/19097/Desktop/Programs/Registration Application/App files/Logs/Logs.txt", std::ios::app);
		log << "[" << getTime() << "] " << action << std::endl;

	}
	logAction(std::string UserName, std::string action) { //User performed an action.

		std::ofstream log("C:/Users/19097/Desktop/Programs/Registration Application/App files/Logs/Logs.txt", std::ios::app);
		log << "[" << getTime() << "] " << UserName << " " << action << std::endl;
	}
	logAction(std::string action, int UserID) { //Action performed on user.
		std::ofstream log("C:/Users/19097/Desktop/Programs/Registration Application/App files/Logs/Logs.txt", std::ios::app);
		log << "[" << getTime() << "] " << action << " " << UserID << std::endl;
	}
	logAction(std::string fromUser, std::string action, std::string toUser) { //Logs when users message DM each other.
		std::ofstream log("C:/Users/19097/Desktop/Programs/Registration Application/App files/Logs/Logs.txt", std::ios::app);
		log << "[" << getTime() << "] " << fromUser << action << toUser << std::endl;
	}

	std::string getTime() {//Gets the time for the logs.

		time_t rawtime;
		struct tm* timeinfo;
		char buffer[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
		std::string str(buffer);
		return str;
	}




};

void clear() {// Clears console
	bool flag = false;
	while (flag == false) {
		COORD topLeft = { 0, 0 };
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO screen;
		DWORD written;

		GetConsoleScreenBufferInfo(console, &screen);
		FillConsoleOutputCharacterA(
			console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
		);
		FillConsoleOutputAttribute(
			console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
			screen.dwSize.X * screen.dwSize.Y, topLeft, &written
		);
		SetConsoleCursorPosition(console, topLeft);
		flag = true;
	}
	std::cout << "***************Console cleared****************" << std::endl;
}

class User {
	
private:
	unsigned long long int UserID; //Unique user ID.
	std::string passWord;
	std::string userName;
	std::vector<std::string>users; //Container for users.
	std::vector<std::string>friendsList;
	std::string setUserName() { //Sets the  user class username properties.
		std::cout << "\nEnter your username: ";
		std::cin >> userName;
		return userName;

	}

	std::string setPassWord() { //Sets the user class user password.
		std::string passWord;
		std::cout << "\nEnter your password: ";
		std::cin >> passWord;
		return passWord;

	}

	unsigned long long int generateUserID() { //Generates a userID
		srand(time(NULL));
		UserID = rand() % 18446744073709551614 + 1;
		return UserID;
	}

	unsigned long long int setUserID(User& currentUser) { //Retrives the current userID of user.
		std::string UserIdString;
		std::string userName = currentUser.getUserName();
		std::ifstream user("C:/Users/19097/Desktop/Programs/Registration Application/App files/UserDB/" + userName + "-UserCredentials.txt");

		if (!user) {
			return 0;
		}
		else {
			int count = 0;
			while (count != 3 && std::getline(user, UserIdString)) {//Skips first two lines of UserCredentials.
				count++;
			}

			std::stringstream toInt(UserIdString); //Converts string to integer.
			int userID;
			toInt >> userID;
			UserID = userID;
			return UserID;
		}

	}

	bool uploadUser(std::string userName, std::string passWord, int UserID)//Uploads a new register to file.
	{
		bool flagSuccess = false; //Informs if registration was successful.
		char code = authenticateUser(userName, passWord);
		if (code == 'u' || code == 's')//UserName taken
		{
			std::cout << "\nName Taken!" << std::endl;

			flagSuccess = false;

		}
		else { //UserName Available

			std::ofstream upload("C:/Users/19097/Desktop/Programs/Registration Application/App files/UserDB/" + userName + "-UserCredentials.txt");
			std::ofstream uploadUName{ "C:/Users/19097/Desktop/Programs/Registration Application/App files/usernames/usernames.txt", std::ios::app };
			std::ifstream sortNamesIF("C:/Users/19097/Desktop/Programs/Registration Application/App files/usernames/usernames.txt"); //Reads username contents.
			std::string uName;

			

			uploadUName << userName << std::endl; //Uploads username to Users file.
			upload << userName << std::endl; //Uploads username to file.
			upload << passWord << std::endl; //Uploads password to file
			upload << std::to_string(UserID) << std::endl; //Uploads UserID to file

			users.clear();
			while (std::getline(sortNamesIF, uName)) {//Uploads usernames to vector container.
				users.push_back(uName);
			}

			flagSuccess = true; //Sets user to logged in.
		}
		return flagSuccess;
	}

	void sortNames(std::vector<std::string>& users) { //Sorts usernames in username file.

		std::ofstream sortNamesOut("C:/Users/19097/Desktop/Programs/Registration Application/App files/usernames/usernames.txt", std::ofstream::trunc);//Deletes username contents.
		std::sort(users.begin(), users.end());
		std::ofstream sortNamesIn("C:/Users/19097/Desktop/Programs/Registration Application/App files/usernames/usernames.txt", std::ios::app);//Appends username contents

		for (size_t i = 0; i < users.size(); i++) {//Writes sorted names to file.
			sortNamesIn << users[i] << std::endl;
		}
	}
	std::string getPassWord() //Returns User password.
	{
		return passWord;
	}
	
public:
	std::vector<std::string>DMContact; //Container for public DM Contact. Show off high profile DM chat contacts. Not actual messages contents.
	void createUser() { //Creates a new user.

		bool successfullyCreated = false; //Flag check if user was created successfully.

		while (successfullyCreated == false) {
			userName = setUserName();
			passWord = setPassWord();
			UserID = generateUserID();
			successfullyCreated = uploadUser(userName, passWord, UserID);

		}
		sortNames(users);// Sorts the usernames stored in DB.

		std::cout << "\nWelcome " << userName << "!\n" << std::endl;
		logAction("System Created a new user", UserID);
	}

	static char authenticateUser(std::string userAttempt, std::string passAtempt) { //Authenticates if user credentials exists.

		std::ifstream readData("C:/Users/19097/Desktop/Programs/Registration Application/App files/UserDB/" + userAttempt + "-UserCredentials.txt"); //User Credentials files.
		char flag = ' ';

		if (!readData)
		{
			flag = 'n';//No username or pass

		}
		else//Username found
		{
			std::string un;
			std::string pw;
			std::string uID;
			std::getline(readData, un); //Reads Username
			std::getline(readData, pw); //Reads Password.
			std::getline(readData, uID); //Reads UserID

			if (un == userAttempt && pw == passAtempt)
			{
				flag = 's';//Success found username and pass

			}
			else {
				flag = 'u';//Found only username
			}
		}
		return flag;
	}

	static bool loginUser(User& currentUser) { //Logs user in

		bool loggedIn = false; //Flag check to see if login was success.
		std::string userAttempt;
		std::string passAttempt;
		std::vector<int>friendsList;
		std::cout << "\nEnter your Username: ";
		std::cin >> userAttempt;
		std::cout << "\nEnter your Password: ";
		std::cin >> passAttempt;

		char check = authenticateUser(userAttempt, passAttempt); //Checks if the username is taken or available.

		if (check == 's') {//Code S = Success (User was authenticated)

			currentUser.userName = userAttempt;//Sets the current user UserName.
			currentUser.passWord = passAttempt; //Sets the current user Password
			currentUser.setUserID(currentUser); //Sets the current user UserID
			loggedIn = true; //Logs user in.
			std::cout << "\nLogin successful.\n" << std::endl;
			logAction(currentUser.userName, "Made a successful login.");
		}
		else {

			std::cout << "\nIncorrect Login!" << std::endl;
			logAction("Incorrect Login Attempt");
		}

		return loggedIn;

	}

	bool logOffUser(User& currentUser) {
		bool loggedIn;
		std::string Temp = currentUser.getUserName();
		currentUser.userName = "";
		currentUser.passWord = "";
		currentUser.UserID = 0;
		loggedIn = false;
		std::cout << "\nLog off successful!\n\n";
		logAction::logAction(Temp, "Logged off successfuly");
		return loggedIn; // We return false to set the loggedIn status as offline.
	}

	static char userLoginRegisterPrompt()//Prompts User to register or login.
	{
		std::cout <<"\n**********************************\n        LOGIN OR REGISTER \n**********************************\n\n";

		char choice = ' ';
		while (choice != 'L' && choice != 'R')
		{
			std::cout << "\t\n[L]-LOGIN \t\n[R]-REGISTER\n\n>";
			std::cin >> choice;
			choice = toupper(choice);
		}

		return choice;

	}

	 int countUsers() {
		 return users.size();
	}

	 bool addRemoveFriend(std::string userName) {

		 std::cout << "\n**********************************\n         FRIENDS MENU \n**********************************\n\n";

		 char choice = ' ';//Choice of adding friend or removing frine.
		 std::ifstream Infriends;
		 Infriends.open("C:/Users/19097/Desktop/Programs/Registration Application/App files/friendsList/" + userName + "-friendslist.txt");//Read friends list.
		 std::vector<std::string>tempVec;
		 std::string toUser;//User to add or remove
		 std::string userFriend; //Used to read each friend from friends list.

		 while (choice != 'A' && choice != 'R' && choice != 'V' && choice != 'E')
 {
			 choice = ' ';
			 std::cout << "\nPress [F] - Add Friend\nPress [R] - Remove Friend\nPress [V] - View friends\nPress [E] - Exit main menu.\n\n>";
			 std::cin >> choice;
			 choice = toupper(choice);
			 bool found = false; //Used for finding if a user exists and if a user is in your friends list.
			 bool sucess;//If function return successfully
			 toUser.clear();
			 if (choice == 'A') {//Add friend
				 while (found == false) {
					 displayUsers(); //Displays all users.
					 std::cout << "Enter a username to add: ";
					 std::cin >> toUser;
					 found = findUsers(toUser);//True if a user is found.
					 if (found == false) {
						 std::cout << "No username " << toUser << " exists!" << std::endl; //New text!!!
						 return false;//No uesr was found.
					 }
				 }
				 std::ofstream friends("C:/Users/19097/Desktop/Programs/Registration Application/App files/friendsList/" + userName + "-friendslist.txt", std::ios::app); //Output to friends list.
				 friends << toUser << std::endl;
				 friendsList.push_back(toUser);
				 std::cout << "\n\n" << toUser << " has been added to your friends list." << std::endl;
				 friends.close();
				 logAction(userName, " friended ", toUser);
				 sucess = true;

				 return sucess;
			 }
			 else if (choice == 'R') {//Remove friend.
				 tempVec.clear();
				 std::ofstream temp;
				 bool hasFriends; //Checks if friends list exists.
				 hasFriends = displayFriends(userName);//True if user is found.
				 if (hasFriends == false) {
					 return false;
				 }
				std::cout << "\nEnter a username to remove from your friends list: ";
				std::cin >> toUser;
				found = findUsers(toUser);
				if (found == false) {
					std::cout << "\nNo username " << toUser << " exists in your friends list!" << std::endl;
					return false;//No user was found.
				}

				while (std::getline(Infriends, userFriend)) {//Reads from friends list.
					if (userFriend != toUser) {
							tempVec.push_back(userFriend);
					}
				}
				temp.open("C:/Users/19097/Desktop/Programs/Registration Application/App files/friendsList/-temp.txt");//Read friends list.
				Infriends.close();
				remove(("C:/Users/19097/Desktop/Programs/Registration Application/App files/friendsList/" + userName + "-friendslist.txt").c_str());
				for (int i = 0; i < tempVec.size(); i++) {
					temp << tempVec[i] << std::endl;
				}
				temp.close();
				rename("C:/Users/19097/Desktop/Programs/Registration Application/App files/friendsList/-temp.txt", ("C:/Users/19097/Desktop/Programs/Registration Application/App files/friendsList/" + userName + "-friendslist.txt").c_str());
				std::cout << "\n\nUser " << toUser << " has been removed from your friends list." << std::endl;
				logAction(userName, "Removed ", toUser);

			 }
			 
			 else if (choice == 'V') {
				 displayFriends(userName);
			 }
			 else if (choice == 'E') {
				 std::cout << "\nExiting..." << std::endl;
				 return 0;
			 }
			 else {
				 std::cout << "Please enter a valid choice!" << std::endl;
			 }
		 }
	 }

	 bool displayFriends(std::string userName) {

		 std::ifstream friends("C:/Users/19097/Desktop/Programs/Registration Application/App files/friendsList/" + userName + "-friendslist.txt"); //Used to read friends list.
		 std::string userFriend;//Individual friend.

		 if (friends.fail() == true) {//If not friends list exists.
			 std::cout << "You do not have any friends!" << std::endl;
			 return false; //False- User has no friends.
		 }
		 else { //Friends list does exist.
			 std::cout << "\n***" << userName << "'s friends list ***\n" << std::endl;
			 while(std::getline(friends, userFriend)) {
				 std::cout << "-- [" << userFriend << "]" << std::endl;
			 }
			 return true; //True User has friends.

		 }
	 }

	std::string getUserName() {//Returns username
		return userName;
	}
	std::string getUserName(int UserID) {//Returns username
		int user = UserID;
		return userName;
	}
	int getUserID() //Returns UserID
	{
		return UserID;
	}

};

class Post {	//Will hold the contents of a single post.

private:
	int UserID;
	std::string message;
	//time_t date; // Will eventually hold a time value.
public:

	static void postMenu(User& currentUser, bool loggedIn) {
		std::cout << "\n**********************************\n          POST MENU\n**********************************\n\n";

		char postChoice = ' ';
		std::cout << "\n[P] - Post to board.\n[V] - View Board.\n[E] - Exit to main menu.\n\n>";
		std::cin >> postChoice;
		postChoice = toupper(postChoice);

		if (postChoice == 'P') {
			Post currentUserPost;//Initilize current post.
			currentUserPost.loadPosts();
			currentUserPost.postMessage(currentUser, loggedIn);
			currentUserPost.loadPosts();
			//PlaySound(TEXT("C:/Users/19097/Desktop/Programs/Registration Application/App files/Audio/ding.wav"), NULL, SND_FILENAME | SND_ASYNC); // Sound when message is posted
		}
		else if (postChoice == 'V') {
			Post::loadPosts(); //Loads the post wall for viewing
		}
		else if (postChoice == 'E') {
			std::cout << "\nExiting..." << std::endl;
			return;
		}
	}
	char postMessage(User& currentUser, bool loggedIn) {	//Will post a message to the board. Passes a User object.
		if (loggedIn == false) {
			std::cout << "You must be logged in to post!" << std::endl;
			User::userLoginRegisterPrompt();
			return 'F';
		}

		std::string message; //Actual message string

		std::cout << "\nEnter your message: ";
		std::cin.ignore(); //Clears string buffer.
		getline(std::cin, message);
		std::ofstream post("C:/Users/19097/Desktop/Programs/Registration Application/App files/UserPosts/-Posts.txt", std::ios::app); //File with user posts.
		post << currentUser.getUserName() << ": " << message << std::endl;
		post.close();
		logAction(currentUser.getUserName(), "Made a post");
	}
	static bool loadPosts() {	//Will be for viewing the post history.
		std::ifstream post("C:/Users/19097/Desktop/Programs/Registration Application/App files/UserPosts/-Posts.txt");
		std::string line;

		if (!post) {
			std::cout << "No board exists!" << std::endl;
			return false;
		}
		std::cout << "\n**********************************\n          GLOBAL POST\n**********************************\n\n";
		while (getline(post, line)) {
			std::cout << line << std::endl;
		}
		std::cout << "\n\n**********************************\n**********************************\n\n";
	}

};

class DirectMessage { //Will hold the mesages between two users;

private:
	std::string messageContent;
	User fromUser;
	User toUser;

public:
	static char sendViewMessage(User& currentUser) {
		std::cout << "\n**********************************\n            DM MENU\n**********************************\n\n";
		std::string toUser; //UserName that will be messaged
		char choice = ' ';
		bool userFound = false;
		std::string fromUser = currentUser.getUserName();
		char DMCode;
		std::vector<std::string>externalDMContact;
		std::string message;//Will hold current message being sent
		std::string pastMessages; //Will hold line of past message
		std::ofstream DM_HistoryNew; //Declaration of DM
		std::cout << "\n[S] - Send\n[V] - View\n[E] - Exit to main menu\n\n>";
		std::cin >> choice;
		choice = toupper(choice);

		if (choice == 'S') { //Complete Algorithm for sending a DM.

			while ((choice != 'U' && choice != 'F') && (userFound == false)) {//Lets you find username by search

				std::cout << "\nPress [U] - To list all users.\nPress [F] - To find by Username.\n\n>";
				std::cin >> choice;
				choice = toupper(choice);

				if (choice == 'U')
				{
					displayUsers();
					std::cout << "Enter a username to message: ";
					std::cin >> toUser;
					userFound = findUsers(toUser);
					if (userFound == false) {
						std::cout << "\nNo Username [" << toUser << "] found!" << std::endl;
						return 'f'; //Failed to find username.
					}
					//else{
					//displayDM(toUser, fromUser)
				//}

				}
				else if (choice == 'F') {
					std::cout << "\nEnter a username: ";
					std::cin >> toUser;
					userFound = findUsers(toUser);
					if (userFound == false) {
						std::cout << "\nNo Username [" << toUser << "] found!" << std::endl;
						return 'f'; //Failed to find username.
					}

				}

			}

			DMCode = displayDMs(fromUser, toUser);//DM Code will decide if previous messages exist and in what format. I.E (from - to, to - from).

			std::ifstream DM_History("C:/Users/19097/Desktop/Programs/Registration Application/App files/DMessages/" + fromUser + "-" + toUser + ".txt"); //Checks if two users have message history (from - to)
			std::ifstream DM_History2("C:/Users/19097/Desktop/Programs/Registration Application/App files/DMessages/" + toUser + "-" + fromUser + ".txt"); //Checks if two users have message history (to - from)
			std::ofstream DMContactOut("C:/Users/19097/Desktop/Programs/Registration Application/App files/DMessages/DMContact/" + fromUser + "-" + "DMContacts.txt"); //output for a contacts.
			//Using Found one and found two. Because a user can send a message as (Leo to Kim) or (Kim to Leo).
			if (DMCode == '1') {
				std::ofstream DM_History("C:/Users/19097/Desktop/Programs/Registration Application/App files/DMessages/" + fromUser + "-" + toUser + ".txt", std::ios::app); //Initializes DM between two users.
				std::cout << "\nEnter your message: ";
				std::getline(std::cin, message);
				DM_History << fromUser << ": " << message << std::endl;
				logAction(fromUser, " sent a message to: ", toUser);
			}
			else if (DMCode == '2') {
				std::ofstream DM_History2("C:/Users/19097/Desktop/Programs/Registration Application/App files/DMessages/" + toUser + "-" + fromUser + ".txt", std::ios::app); //Initializes DM between two users.
				std::cout << "\nEnter your message: ";
				std::getline(std::cin, message);
				DM_History2 << fromUser << ": " << message << std::endl;
				logAction(fromUser, " sent a message to: ", toUser);
				displayDMs(fromUser, toUser);
			}

			else {//No chat exists so it creates one.
				std::ofstream DM_HistoryNew("C:/Users/19097/Desktop/Programs/Registration Application/App files/DMessages/" + fromUser + "-" + toUser + ".txt", std::ios::app); //Initializes DM between two users.
				std::cout << "\nEnter your message: ";
				std::getline(std::cin, message);
				DM_HistoryNew << fromUser << ": " << message << std::endl;
				addDMContact(fromUser, toUser); //Adds 
				logAction(fromUser, " created a new conversation with: ", toUser);
				displayDMs(fromUser, toUser);

			}
		}
		else if(choice == 'V'){
			std::string contact; // DM Contact for Selection.
			std::string DMContactView; // Contact choice for viewing DM history.
			std::ifstream DMContactIn("C:/Users/19097/Desktop/Programs/Registration Application/App files/DMContact/" + fromUser + "-DMContacts.txt"); //Inputs for DM contacts.

			if (DMContactIn.fail() == true) {//No DM Histroy.
				std::cout << "No DM history." << std::endl;
			}
			else {//Shows DM Contacts.
				std::cout << "\n********** [DM CONTACTS] **********\n\n";
				while (std::getline(DMContactIn, contact)) {

					std::cout << "- [" << contact << "]" << std::endl;
				}
				std::cout << "\nSelect a contact to view DM's\n\n>";
				std::cin >> DMContactView; //Name for DM View
				displayDMs(fromUser, DMContactView); //Actual call to display DM History.
			}

		}
		else if (choice == 'E') {
			std::cout << "\nExiting...\n\n";
			return 'E';
		}
		return 's'; //s for success.
	}

	static void addDMContact(std::string fromUser, std::string toUser) {
		std::ofstream DMContactWrite("C:/Users/19097/Desktop/Programs/Registration Application/App files/DMContact/" + fromUser + "-DMContacts.txt", std::ios::app);
		DMContactWrite << toUser << std::endl;
		DMContactWrite.close();
	}
	// static bool displayDM(std::string fromUser, std::string toUser) {
	static char displayDMs(std::string fromUser, std::string toUser) {

		std::ifstream DM_History("C:/Users/19097/Desktop/Programs/Registration Application/App files/DMessages/" + fromUser + "-" + toUser + ".txt"); //Checks if two users have message history (from - to)
		std::ifstream DM_History2("C:/Users/19097/Desktop/Programs/Registration Application/App files/DMessages/" + toUser + "-" + fromUser + ".txt"); //Checks if two users have message history (to - from)
		std::string pastMessages;

		if (DM_History.fail() == false) {

			std::cout << "\n**********************************\n**********************************\n\n";
			std::cout << "DM History between " << fromUser << " and " << toUser << "." << std::endl;
			std::cout << "----------------------------------\n\n";


			while (std::getline(DM_History, pastMessages)) {
				std::cout << pastMessages << std::endl;
			}

			std::cout << "\n**********************************\n**********************************\n\n";
			return '1';
		}

		else if (DM_History2.fail() == false) {

			std::cout << "\n**********************************\n**********************************\n\n";
			std::cout << "DM History between " << fromUser << " and " << toUser << ".\n" << std::endl;

			while (std::getline(DM_History2, pastMessages)) {
				std::cout << pastMessages << std::endl;
			}

			std::cout << "\n**********************************\n**********************************\n\n";

			return '2';
		
		}
		else {
			std::cout << "\nStart a new conversation with " << toUser << ".";
			std::cout << "\n-----------------------------------\n\n";

			return '3';
		}


	}

	
	//viewMessages();
	//createGroup();
};



int main()
{
	std::cout << "\n**********************************\n    WELCOME TO QUICK SOCIAL!\n      [By-Leonardo Corso]\n**********************************\n";

	bool quitProgram = false;
	while (quitProgram != true) {
		char Loginchoice = User::userLoginRegisterPrompt(); //Prompt user to login or register.
		User currentUser; //Initilize current user.
		bool loggedIn = false;
		while (loggedIn == false) {
			if (Loginchoice == 'R') //R = Register
			{
				while (Loginchoice == 'R') {
					currentUser.createUser(); //Call to create a user.
					Loginchoice = User::userLoginRegisterPrompt(); //Prompts user to login with account or register another.
				}
				loggedIn = User::loginUser(currentUser); //Sets user to logged in.
				//PlaySound(TEXT("C:/Users/19097/Desktop/Programs/Registration Application/App files/Audio/Windows Information Bar.wav"), NULL, SND_FILENAME | SND_ASYNC); // Sound when user logs in
			}
			else //Choice for user to login in.
			{
				loggedIn = User::loginUser(currentUser); //L = Login

			}
			if (loggedIn == false) {

				Loginchoice = User::userLoginRegisterPrompt();

			}
		}


		char post = ' ';
		while (post != 'L') {
			post = main_Menu(); //Prompts user to post or view or logout.

			if (post == 'P') {
				Post::postMenu(currentUser, loggedIn);

			}
			else if (post == 'L') {
				loggedIn = currentUser.logOffUser(currentUser); //Log off user and delete the attributes of currentUser object from RAM.
				Loginchoice = ' ';

			}
			else if (post == 'D') {

				DirectMessage::sendViewMessage(currentUser);
			}
			else if (post == 'F') {
				currentUser.addRemoveFriend(currentUser.getUserName());
			}
			else if (post == 'S') {
				//loadSettings(currentUser.getUserName());
			}
			else if (post == 'C') {
				clear(); // Clears history.

			}
			else {
				std::cout << "\nPlease enter valid input!\n" << std::endl;
				post = main_Menu();
			}
		}
	}
	//postMessage();


	return 0;
}