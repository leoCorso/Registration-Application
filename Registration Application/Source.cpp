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
#include<conio.h>
#include <filesystem>
using namespace std;

#pragma comment(lib, "Winmm.lib")


static char main_Menu()//Prompts user to post or view message threads.
{
	char choice = ' ';

	std::cout << "\n**********************************\n           MAIN MENU \n**********************************\n\n";
	std::cout << "\n[P] - Post Menu\n[D] - DM Menu.\n[F] - Friends Menu.\n[S] - Settings Menu.\n[C] - Clear Console.\n[L] - Log Out.\n[E] - Exit\n\n>";
	std::cin >> choice;
	choice = toupper(choice);

	while (choice != 'P' && choice != 'V' && choice != 'L' && choice != 'D' && choice != 'F' && choice != 'C' && choice != 'E') {
		std::cout << "\n**********************************\n           MAIN MENU \n**********************************\n\n";
		std::cout << "\n[Please enter a valid option!]" << std::endl;
		std::cout << "\n[P] - Post Menu\n[D] - DM Menu.\n[A] - Friends Menu.\n[S] - Settings Menu.\n[C] - Clear Console.\n[L] - Log Out.\n\n>";
		std::cin >> choice;
		choice = toupper(choice);
	}
	return choice;

}

bool findUsers(std::string toUser) { //Check if a user with that name exists.
	std::ifstream DM("App files/usernames/usernames.txt"); //File with all usernames.
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
void exitApp(int exitCode) {//Exits the app and dismises user with friendly message.


	std::cout << "\nCya later..." << std::endl;
	exit(exitCode);
}
void displayUsers() { //Displays a list of all usernames.
	std::ifstream Users("App files/usernames/usernames.txt");
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
		std::ofstream log("App files/Logs/Logs.txt", std::ios::app);
		log << "[" << getTime() << "] " << action << std::endl;

	}
	logAction(std::string UserName, std::string action) { //User performed an action.

		std::ofstream log("App files/Logs/Logs.txt", std::ios::app);
		log << "[" << getTime() << "] " << UserName << " " << action << std::endl;
	}
	logAction(std::string action, int UserID) { //Action performed on user.
		std::ofstream log("App files/Logs/Logs.txt", std::ios::app);
		log << "[" << getTime() << "] " << action << " " << UserID << std::endl;
	}
	logAction(std::string fromUser, std::string action, std::string toUser) { //Logs when users message DM each other.
		std::ofstream log("App files/Logs/Logs.txt", std::ios::app);
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

		bool matches = false;
		while (matches == false) {
			std::string password = "";
			std::string passwordMatch = "";
			char charContainer = ' ';
			std::cout << "Enter a password : ";

			while (charContainer != 13) {
				charContainer = _getch();
				if (charContainer == 8 && password.size() > 0) { //Backspace.
					password.pop_back(); //Removes last character from string.
					std::cout << "\b \b";//Removes char from console.
				}
				if (charContainer != 13 && charContainer != 8) {
					password += charContainer;
					cout << "*";
				}

			}
			std::cout << "\nEnter that password again: ";
			charContainer = ' ';
			while (charContainer != 13) {
				charContainer = _getch();
				if (charContainer == 8 && passwordMatch.size() > 0) { //Backspace.
					passwordMatch.pop_back(); //Removes last character from string.
					std::cout << "\b \b";//Removes char from console.
				}
				if (charContainer != 13 && charContainer != 8) {
					passwordMatch += charContainer;
					cout << "*";
				}
			}
			if (password == passwordMatch) {
				matches = true;
				return password;
			}
			else {
				std::cout << "\nPasswords do not match. Try again!" << std::endl;
			}
		}
	}

	unsigned long long int generateUserID() { //Generates a userID
		srand(time(NULL));
		UserID = rand() % 18446744073709551614 + 1;
		return UserID;
	}

	unsigned long long int setUserID(User& currentUser) { //Retrives the current userID of user.
		std::string UserIdString;
		std::string userName = currentUser.getUserName();
		std::ifstream user("App files/UserDB/" + userName + "-UserCredentials.txt");

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

			std::ofstream upload("App files/UserDB/" + userName + "-UserCredentials.txt");
			std::ofstream uploadUName{ "App files/usernames/usernames.txt", std::ios::app };
			std::ifstream sortNamesIF("App files/usernames/usernames.txt"); //Reads username contents.
			std::string uName;

			

			uploadUName << userName << std::endl; //Uploads username to Users file.
			upload << userName << std::endl; //Uploads username to file.
			upload << passWord << std::endl; //Uploads password to file
			upload << std::to_string(UserID) << std::endl; //Uploads UserID to file

			users.clear();
			while (std::getline(sortNamesIF, uName)) {//Uploads usernames to vector container.
				users.push_back(uName);
			}
			upload.close();
			uploadUName.close();

			flagSuccess = true; //Sets user to logged in.
		}
		return flagSuccess;
	}

	void sortNames(std::vector<std::string>& users) { //Sorts usernames in username file.

		std::ofstream sortNamesOut("App files/usernames/usernames.txt", std::ofstream::trunc);//Deletes username contents.
		std::sort(users.begin(), users.end());
		std::ofstream sortNamesIn("App files/usernames/usernames.txt", std::ios::app);//Appends username contents

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

		std::ifstream readData("App files/UserDB/" + userAttempt + "-UserCredentials.txt"); //User Credentials files.
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
		std::string userAttempt; //Usersname login attempt
		std::string passAttempt = "";//User password attempt
		std::vector<int>friendsList;
		std::cout << "\nEnter your Username: ";
		std::cin >> userAttempt;
		char charContainer = ' ';//1 character in the password
		cout << "Enter your password: ";

		while (charContainer != 13) {
			charContainer = _getch();
			if (charContainer == 8 && passAttempt.size() > 0) { //Backspace.
				passAttempt.pop_back(); //Removes last character from string.
				std::cout << "\b \b";//Removes char from console.
			}
			if (charContainer != 13 && charContainer != 8) {
				passAttempt += charContainer;
				cout << "*";
			}

		}


		char check = authenticateUser(userAttempt, passAttempt); //Authenticates the user.

		if (check == 's') {//Code S = Success (User was authenticated)

			currentUser.userName = userAttempt;//Sets the current user UserName.
			currentUser.passWord = passAttempt; //Sets the current user Password
			currentUser.setUserID(currentUser); //Sets the current user UserID
			loggedIn = true; //Logs user in.
			std::cout << "\n\nLogin successful.\n" << std::endl;
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
		clear();
		std::cout << "\nLog off successful!\n\n";
		logAction::logAction(Temp, "Logged off successfuly");
		return loggedIn; // We return false to set the loggedIn status as offline.
	}

	static char userLoginRegisterPrompt()//Prompts User to register or login.
	{
		std::cout <<"\n**********************************\n        LOGIN OR REGISTER \n**********************************\n\n";

		char choice = ' ';
		while (choice != 'L' && choice != 'R' && choice != 'E')
		{
			std::cout << "\t\n[L]-LOGIN \t\n[R]-REGISTER\n[E]-Exit\n\n>";
			std::cin >> choice;
			choice = toupper(choice);
		}

		return choice;

	}

	 int countUsers() {
		 return users.size();
	}

	 bool addRemoveFriend(std::string userName) {


		 char choice = ' ';//Choice of adding friend or removing frine.
		 std::ifstream Infriends;
		 Infriends.open("App files/friendsList/" + userName + "-friendslist.txt");//Read friends list.
		 std::vector<std::string>tempVec;
		 std::string toUser;//User to add or remove
		 std::string userFriend; //Used to read each friend from friends list.
		 std::cout << "\n**********************************\n         FRIENDS MENU \n**********************************\n\n";

		 while (choice != 'E'){
			 choice = ' ';
			 std::cout << "\nPress [A] - Add Friend\nPress [R] - Remove Friend\nPress [V] - View friends\nPress [E] - Exit main menu.\n\n>";
			 std::cin >> choice;
			 choice = toupper(choice);

			 while (choice != 'A' && choice != 'R' && choice != 'V' && choice != 'E') {
				 std::cout << "\n**********************************\n         FRIENDS MENU \n**********************************\n\n";
				 std::cout << "[Please enter a valid choice!]\n\n";
				 choice = ' ';
				 std::cout << "\nPress [A] - Add Friend\nPress [R] - Remove Friend\nPress [V] - View friends\nPress [E] - Exit main menu.\n\n>";
				 std::cin >> choice;
				 choice = toupper(choice);

			 }
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
				 std::ofstream friends("App files/friendsList/" + userName + "-friendslist.txt", std::ios::app); //Output to friends list.
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
				temp.open("App files/friendsList/-temp.txt");//Read friends list.
				Infriends.close();
				remove(("App files/friendsList/" + userName + "-friendslist.txt").c_str());
				for (int i = 0; i < tempVec.size(); i++) {
					temp << tempVec[i] << std::endl;
				}
				temp.close();
				rename("App files/friendsList/-temp.txt", ("App files/friendsList/" + userName + "-friendslist.txt").c_str());
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
		 }
	 }

	 bool displayFriends(std::string userName) {

		 std::ifstream friends("App files/friendsList/" + userName + "-friendslist.txt"); //Used to read friends list.
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
		char postChoice = ' ';
		while (postChoice != 'E') {
			std::cout << "\n**********************************\n          POST MENU\n**********************************\n\n";
			std::cout << "\n[P] - Post to board.\n[V] - View Board.\n[E] - Exit to main menu.\n\n>";
			std::cin >> postChoice;
			postChoice = toupper(postChoice);

			while (postChoice != 'P' && postChoice != 'V' && postChoice != 'E')
			{

				std::cout << "\n**********************************\n          POST MENU\n**********************************\n\n";
				std::cout << "\n[Please enter a valid option!]\n" << std::endl;
				std::cout << "\n[P] - Post to board.\n[V] - View Board.\n[E] - Exit to main menu.\n\n>";
				std::cin >> postChoice;
				postChoice = toupper(postChoice);
			}


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
		std::ofstream post("App files/UserPosts/-Posts.txt", std::ios::app); //File with user posts.
		post << currentUser.getUserName() << ": " << message << std::endl;
		post.close();
		logAction(currentUser.getUserName(), "Made a post");
	}
	static bool loadPosts() {	//Will be for viewing the post history.
		std::ifstream post("App files/UserPosts/-Posts.txt");
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
	static char removeDMContact(std::string fromUser) {
		char DMHistoryCheck;
		std::string toUser;
		std::vector<string>tempVec;
		displayDMContacts(fromUser);
		std::cout << "\nWARNING! Removing a DM Contact removes your DM History.\nType a username from your contacts to drop.: ";
		std::cin >> toUser;
		ifstream ContactList("App files/DMContact/" + fromUser + "-DMContacts.txt");
		ofstream temp;
		std::string contactCheck;

		while (std::getline(ContactList, contactCheck)) {//Reads from friends list.
			if (contactCheck != toUser) {
				tempVec.push_back(contactCheck);
			}
		}
		temp.open("App files/DMContact/-temp.txt");//Read friends list.
		ContactList.close();
		remove(("App files/DMContact/" + fromUser + "-DMContacts.txt").c_str());
		for (int i = 0; i < tempVec.size(); i++) {
			temp << tempVec[i] << std::endl;
		}
		temp.close();
		rename("App files/DMContact/-temp.txt", ("App files/DMContact/" + fromUser + "-DMContacts.txt").c_str());
		remove("App files/DMContact/-temp.txt");
		logAction(fromUser, "Removed DM Contact ", toUser);



		DMHistoryCheck = removeDMHistory(fromUser, toUser);
		return 'S';//Successfully removed DM History and contact.
	}

	static char removeDMHistory(std::string fromUser, std::string toUser) {//Remove DM Convo by with preconfigured Username 

		ifstream DMHistroyFrom("App files/DMessages/" + fromUser + "-" + toUser + ".txt");
		ifstream DMHistroyto("App files/DMessages/" + toUser + "-" + fromUser + ".txt");

		if (DMHistroyFrom.fail() == false) {
			DMHistroyFrom.close();
			remove(("App files/DMessages/" + fromUser + "-" + toUser + ".txt").c_str());
			std::cout << "\n\nConversation with " << toUser << " deleted successfully." << std::endl;
			return 'S';//Success
		}
		else if (DMHistroyto.fail() == false) {
			DMHistroyto.close();
			remove(("App files/DMessages/" + toUser + "-" + fromUser + ".txt").c_str());
			std::cout << "\n\nConversation with " << toUser << " deleted successfully." << std::endl;
			return 'S'; //Success
		}
		else {

			std::cout << "You have no DM History with " << toUser << std::endl;
			return 'F';//Failed to delete.
		}
	}


	static char displayDMContacts(std::string fromUser) {

		std::string contact; // DM Contact for Selection.
		std::ifstream DMContactIn("App files/DMContact/" + fromUser + "-DMContacts.txt"); //Inputs for DM contacts.

		if (DMContactIn.fail() == true) {//No DM Histroy.
			std::cout << "\n[No DM history]" << std::endl;
			return 'F';
		}
		else {//Shows DM Contacts.
			std::cout << "\n********** [DM CONTACTS] **********\n\n";
			while (std::getline(DMContactIn, contact)) {

				std::cout << "- [" << contact << "]" << std::endl;
			}
			return 'S';
		}

	}

	static char sendViewMessage(User& currentUser) {
		char choice = ' ';
		while (choice != 'E') {
			std::string toUser; //UserName that will be messaged
			bool userFound = false;
			std::string fromUser = currentUser.getUserName();
			char DMCode;
			std::vector<std::string>externalDMContact;
			std::string message;//Will hold current message being sent
			std::string pastMessages; //Will hold line of past message
			std::ofstream DM_HistoryNew; //Declaration of DM
			std::cout << "\n**********************************\n            DM MENU\n**********************************\n\n";

			while (choice != 'S' && choice != 'V' && choice != 'E' && choice != 'D') {
				std::cout << "\n[S] - Send\n[V] - View\n[D] - Delete\n[E] - Exit to main menu\n\n>";
				std::cin >> choice;
				choice = toupper(choice);
				int contactExists;

				while (choice != 'S' && choice != 'V' && choice != 'E' && choice != 'D') {
					std::cout << "\n**********************************\n            DM MENU\n**********************************\n\n";
					std::cout << "[Please enter a valid choice!]\n" << std::endl;
					std::cout << "\n[S] - Send\n[V] - View\n[D] - Delete\n[E] - Exit to main menu\n\n>";
					std::cin >> choice;
					choice = toupper(choice);
				}

				if (choice == 'S') { //Complete Algorithm for sending a DM.
					char choicetwo = ' ';
					while ((choicetwo != 'U' && choicetwo != 'F' && choicetwo != 'E') && (userFound == false)) {//Lets you find username by search

						std::cout << "\nPress [U] - To list all users.\nPress [F] - To find by Username.\nPress [E] - To exit\n\n>";
						std::cin >> choicetwo;
						choicetwo = toupper(choicetwo);

						if (choicetwo == 'U')
						{
							displayUsers();
							std::cout << "Enter a username to message: ";
							std::cin >> toUser;
							userFound = findUsers(toUser);
							if (userFound == false) {
								std::cout << "\nNo Username [" << toUser << "] found!" << std::endl;
								return 'F'; //Failed to find username.
							}
							//else{
							//displayDM(toUser, fromUser)
						//}

						}
						else if (choicetwo == 'F') {
							std::cout << "\nEnter a username: ";
							std::cin >> toUser;
							userFound = findUsers(toUser);
							if (userFound == false) {
								std::cout << "\nNo Username [" << toUser << "] found!" << std::endl;
								return 'F'; //Failed to find username.
							}

						}
						else if (choicetwo == 'E') {
							choice = 'Z';//Will change choice to execute loop again. AKA Back arrow.
							sendViewMessage(currentUser);
							return 'E'; //Exit code.
						}
						else {
							std::cout << "\nPlease enter a valid choice!" << std::endl;
						}

					}

					DMCode = displayDMs(fromUser, toUser);//DM Code will decide if previous messages exist and in what format. I.E (from - to, to - from).

					std::ifstream DM_History("App files/DMessages/" + fromUser + "-" + toUser + ".txt"); //Checks if two users have message history (from - to)
					std::ifstream DM_History2("App files/DMessages/" + toUser + "-" + fromUser + ".txt"); //Checks if two users have message history (to - from)
					std::ofstream DMContactOut("App files/DMessages/DMContact/" + fromUser + "-" + "DMContacts.txt"); //output for a contacts.
					//Using Found one and found two. Because a user can send a message as (Leo to Kim) or (Kim to Leo).
					if (DMCode == '1') {
						std::ofstream DM_History("App files/DMessages/" + fromUser + "-" + toUser + ".txt", std::ios::app); //Initializes DM between two users.
						std::cout << "\nEnter your message: ";
						std::getline(std::cin, message);
						DM_History << fromUser << ": " << message << std::endl;
						logAction(fromUser, " sent a message to: ", toUser);
					}
					else if (DMCode == '2') {
						std::ofstream DM_History2("App files/DMessages/" + toUser + "-" + fromUser + ".txt", std::ios::app); //Initializes DM between two users.
						std::cout << "\nEnter your message: ";
						std::getline(std::cin, message);
						DM_History2 << fromUser << ": " << message << std::endl;
						logAction(fromUser, " sent a message to: ", toUser);
						displayDMs(fromUser, toUser);
					}

					else {//No chat exists so it creates one.
						std::cout << "\nStart a new conversation with " << toUser << ".";
						std::cout << "\n-----------------------------------\n\n";
						std::ofstream DM_HistoryNew("App files/DMessages/" + fromUser + "-" + toUser + ".txt", std::ios::app); //Initializes DM between two users.
						std::cout << "\nEnter your message: ";
						std::getline(std::cin, message);
						DM_HistoryNew << fromUser << ": " << message << std::endl;
						addDMContact(fromUser, toUser); //Adds 
						logAction(fromUser, " created a new conversation with: ", toUser);
						displayDMs(fromUser, toUser);

					}
				}
				else if (choice == 'V') {

					char DMFound = displayDMContacts(fromUser);
					std::string DMContactView;
					if (DMFound == 'F') {
						return 'F';//For fail.
					}
					std::cout << "\nSelect a contact to view DM's\n\n>";
					std::cin >> DMContactView; //Name for DM View
					contactExists = displayDMs(fromUser, DMContactView); //Actual call to display DM History.
					if (contactExists == '3') {
						std::cout << "\nNo DM Chat exists betweem " << fromUser << " and " << DMContactView << std::endl;
						return 'F';//Fails.
					}
				}
				else if (choice == 'E') {
					std::cout << "\nExiting...\n\n";
					return 'E';
				}
				else if (choice == 'D') {
					char deleteChoice;
					bool flag = false;
					while (flag == false) {
						std::cout << "\n[D] - Delete DM contacts.\n[E] - Exit back to DM menu.\n\n>";
						std::cin >> deleteChoice;
						deleteChoice = toupper(deleteChoice);

						if (deleteChoice == 'D') {
							removeDMContact(fromUser);
						}
						else if (deleteChoice == 'E'){
							choice = 'F';//False, will trigger as a back button.
							flag = true;

						}
						else {
							std::cout << "Enter a valid option!" << std::endl;
						}
					}

				}
			}
			return 'S'; //s for success.
		}
}

	static void addDMContact(std::string fromUser, std::string toUser){
		std::ofstream DMContactWrite("App files/DMContact/" + fromUser + "-DMContacts.txt", std::ios::app); //Writes the toUser in the fromUsers DM Contact list.
		std::ofstream DMContactWriteTwo("App files/DMContact/" + toUser + "-DMContacts.txt", std::ios::app); //Writes the fromUser in the toUserr DM Contact list.

		DMContactWrite << toUser << std::endl;
		DMContactWriteTwo << fromUser << std::endl;
		DMContactWriteTwo.close();
		DMContactWrite.close();
	}
	// static bool displayDM(std::string fromUser, std::string toUser) {
	static char displayDMs(std::string fromUser, std::string toUser) {

		std::ifstream DM_History("App files/DMessages/" + fromUser + "-" + toUser + ".txt"); //Checks if two users have message history (from - to)
		std::ifstream DM_History2("App files/DMessages/" + toUser + "-" + fromUser + ".txt"); //Checks if two users have message history (to - from)
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
			return '3';
		}


	}
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
			else if (Loginchoice == 'E') {
				exitApp(0);
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
			else if (post == 'E') {

				exitApp(0); //Exits the App.
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