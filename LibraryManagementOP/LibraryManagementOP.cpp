#include <chrono>
#include <iostream>
#include <vector>
#include <thread>
#include "Entities.h"

UserServices userSevices("Data/Users.txt");
BookServices bookServices("Data/Books.txt");
BookCartServices bookCartServices("Data/BookCarts.txt");
CommentServices commentServices("Data/Comments.txt");

bool IsAuthenticated = false;
User AuthUser;

struct MenuInput
{
	string Title;
	vector<string> Items;
};

void ClearConsole();
void ShowMenu(MenuInput menu);
void ShowError(string message);
void MainMenu();
void Login();
void SignUp();
void SignOut();
void Authenticate();
void Start();
bool IsNumber(string s);

int main()
{
	Start();
}

bool IsNumber(string s)
{
	int len = s.length();
	if (len == 0)
	{
		return false;
	}
	for (int i = 0; i < len; i++)
	{
		if (s[i] > 57 || s[i] < 48)
		{
			return false;
		}
	}
	return true;
}

void ClearConsole()
{
	system("cls");
}

void ShowMenu(MenuInput menu)
{
	ClearConsole();
	cout << menu.Title << "\n\n";

	for (string item : menu.Items)
	{
		cout << "\t" << item << endl;
	}
	cout << endl << endl << endl << "\t";
}

void ShowError(string message)
{
	ClearConsole();
	cout << message;
	this_thread::sleep_for(chrono::milliseconds(1000));
}

void MainMenu()
{
	ClearConsole();
	if (!IsAuthenticated)
	{
		Start();
		return;
	}
	MenuInput menu;
	menu.Title = "Welcome " + AuthUser.Username + "!!";
	menu.Items.push_back("0. SignOut");
	ShowMenu(menu);
	int key;
	string k;
	cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		MainMenu();
		return;
	}

	key = stoi(k);
	switch (key)
	{
	case 0:
		SignOut();
		break;

	default:
		ShowError("Invalid Input!!");
		MainMenu();
		break;
	}
}

void Login()
{
	ClearConsole();
	MenuInput menu;
	menu.Title = "Login";
	ShowMenu(menu);
	string Username, Password;

	cout << "\t" << "Username : ";
	cin >> Username;
	cout << "\t" << "Password : ";
	cin >> Password;

	if (!userSevices.IsExist(Username))
	{
		ShowError("Username or Password is incorrect!!");
		Authenticate();
		return;
	}

	User user = userSevices.Find(Username);
	if (user.Password != Password)
	{
		ShowError("Username or Password is incorrect!!");
		Authenticate();
		return;
	}

	AuthUser = user;
	IsAuthenticated = true;

	Start();
}

void SignUp()
{
	ClearConsole();
	MenuInput menu;
	menu.Title = "SignUp";
	ShowMenu(menu);
	string Username, Password;
	char FullName[100];
	cout << "\t" << "Username : ";
	cin >> Username;
	cout << "\t" << "Password : ";
	cin >> Password;
	cout << "\t" << "FullName : ";
	cin.getline(FullName, 100);
	cin.getline(FullName, 100);

	if (userSevices.IsExist(Username))
	{
		ShowError("This Username is taken!!");
		Authenticate();
		return;
	}

	User user;
	user.Username = Username;
	user.Password = Password;
	user.FullName = FullName;
	user.SignDate = "-------";
	user.IsManager = false;

	userSevices.Add(user);

	AuthUser = userSevices.Find(user.Username);
	IsAuthenticated = true;
	Start();
}

void SignOut()
{
	IsAuthenticated = false;
	User t;
	AuthUser = t;
	Start();
}

void Authenticate()
{
	ClearConsole();
	MenuInput menu;
	menu.Title = "Do you have a Subscription?";
	menu.Items.push_back("1. Yes");
	menu.Items.push_back("2. No");
	ShowMenu(menu);
	int key;
	string k;
	cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		Authenticate();
		return;
	}

	key = stoi(k);

	switch (key)
	{
	case 1:
		Login();
		break;

	case 2:
		SignUp();
		break;

	default:
		ShowError("Invalid Input!!");
		Authenticate();
		break;
	}
}

void Start()
{
	ClearConsole();
	if (!IsAuthenticated)
	{
		Authenticate();
	}
	else
	{
		MainMenu();
	}
}

