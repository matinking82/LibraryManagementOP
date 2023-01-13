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
void print(string n);
void ShowMenu(MenuInput menu);
void ShowUsersList(vector<User> users);
void ShowError(string message);
void MainMenu();
void ManagerMenu();
void SelectUserByManager(int UserId);
void ShowBooksForMembers();
void Profile();
void ShowBorrowedBooksForMembers();
void ShowBooksForManagers();
void ShowUsersForManagers(int page = 1);
void AddManager();
void Login();
void SignUp();
void SignOut();
void Authenticate();
void Start();
bool IsNumber(string s);
void EditProfile();

int main()
{
	Start();
}

bool IsNumber(string s)
{
	int len = s.length();
	if (len == 0 || len > 9)
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

void print(string n)
{
	cout << "\t" << n;
}

void ShowMenu(MenuInput menu)
{
	ClearConsole();
	cout << menu.Title << "\n\n";

	for (string item : menu.Items)
	{
		cout << "\t" << item << endl;
	}
	cout << endl << endl << endl;
}

void ShowUsersList(vector<User> users)
{
	for (User user : users)
	{
		print("Id: " + to_string(user.Id) + "\n");
		print("Username: " + user.Username + "\n");
		print("FullName: " + user.FullName + "\n");
		print("Register Date: " + user.SignDate + "\n");
		print("Books Borrowed: " + to_string(bookCartServices.GetBorrowingCountForUser(user.Id)) + "\n");
		if (user.IsManager)
		{
			print("Role: Manager\n");
		}
		else
		{
			print("Role: Member\n");
		}
		cout << "-------------------------------------------------\n";
	}
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
	menu.Title = "Main Menu | Welcome " + AuthUser.Username + "!!";
	menu.Items.emplace_back("1. Books");
	menu.Items.emplace_back("2. My Books");
	menu.Items.emplace_back("3. Profile");
	if (AuthUser.IsManager)
	{
		menu.Items.emplace_back("4. Manager Panel");
	}
	menu.Items.emplace_back("0. SignOut");
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

	case 1:
		ShowBooksForMembers();
		break;

	case 2:
		ShowBorrowedBooksForMembers();
		break;

	case 3:
		Profile();
		break;

	case 4:
		if (AuthUser.IsManager)
		{
			ManagerMenu();
			break;
		}
	default:
		ShowError("Invalid Input!!");
		MainMenu();
		break;
	}
}

void ManagerMenu()
{
	ClearConsole();
	if (!IsAuthenticated)
	{
		Start();
		return;
	}

	MenuInput menu;
	menu.Title = "Manager Menu | Welcome " + AuthUser.Username + "!!";
	menu.Items.emplace_back("1. Books");
	menu.Items.emplace_back("2. Users");
	menu.Items.emplace_back("3. Add Manager");
	menu.Items.emplace_back("4. Main Menu");
	menu.Items.emplace_back("0. SignOut");
	ShowMenu(menu);
	int key;
	string k;
	cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		ManagerMenu();
		return;
	}

	key = stoi(k);
	switch (key)
	{
	case 0:
		SignOut();
		break;

	case 1:
		ShowBooksForManagers();
		break;

	case 2:
		ShowUsersForManagers();
		break;

	case 3:
		AddManager();
		break;

	case 4:
		MainMenu();
		break;

	default:
		ShowError("Invalid Input!!");
		MainMenu();
		break;
	}
}

void ShowBooksForMembers()
{

}

void Profile()
{
	ClearConsole();
	if (!IsAuthenticated)
	{
		ShowError("User Is Not Authenticated!!");
		Start();
		return;
	}

	MenuInput menu;
	menu.Title = "Profile";

	menu.Items.emplace_back("Username : " + AuthUser.Username);
	menu.Items.emplace_back("Fullname : " + AuthUser.FullName);
	if (AuthUser.IsManager)
	{
		menu.Items.emplace_back("Role : Manager");
	}
	else
	{
		menu.Items.emplace_back("Role : Member");
	}
	menu.Items.emplace_back("Register Date : " + AuthUser.SignDate);
	menu.Items.emplace_back("\n----------------------------------------\n");
	menu.Items.emplace_back("1. Edit");
	menu.Items.emplace_back("8. Back");

	ShowMenu(menu);

	int key;
	string k;
	cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		Profile();
		return;
	}

	key = stoi(k);
	switch (key)
	{
	case 1:
		EditProfile();
		break;

	case 8:
		MainMenu();
		break;
	default:
		ShowError("Invalid Input!!");
		Profile();
		break;
	}

}

void EditProfile()
{
	ClearConsole();
	if (!IsAuthenticated)
	{
		ShowError("User Is Not Authenticated!!");
		Start();
		return;
	}

	MenuInput menu;
	menu.Title = "Edit Profile";
	menu.Items.emplace_back("What Do You Want To Change?\n");
	menu.Items.emplace_back("1. Username");
	menu.Items.emplace_back("2. Password");
	menu.Items.emplace_back("3. FullName");
	menu.Items.emplace_back("");
	menu.Items.emplace_back("8. Back");

	ShowMenu(menu);

	int key;
	string k;
	cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		EditProfile();
		return;
	}

	string oldPass, newPass, newUsername, newFullName;
	char fl[100];
	key = stoi(k);
	switch (key)
	{
	case 1:
		print("New Username : ");
		cin >> newUsername;

		if (userSevices.IsExist(newUsername))
		{
			ShowError("This Username is taken!!");
			EditProfile();
			return;
		}
		AuthUser.Username = newUsername;
		userSevices.Update(AuthUser);
		ShowError("Username Changed Successfully!!");
		EditProfile();
		break;

	case 2:
		print("Old Password : ");
		cin >> oldPass;
		print("New Password : ");
		cin >> newPass;

		if (AuthUser.Password != oldPass)
		{
			ShowError("Password Is Incorrect!!");
			EditProfile();
			return;
		}

		AuthUser.Password = newPass;

		userSevices.Update(AuthUser);

		ShowError("Password Changed Successfully!!");
		EditProfile();
		break;

	case 3:
		print("New FullName : ");
		cin.getline(fl, 100);
		cin.getline(fl, 100);

		AuthUser.FullName = fl;
		userSevices.Update(AuthUser);

		ShowError("FullName Changed Successfully!!");
		EditProfile();
		break;

	case 8:
		Profile();
		break;
	default:
		ShowError("Invalid Input!!");
		EditProfile();
		break;
	}
}

void ShowBorrowedBooksForMembers()
{

}

// /////////////

void ShowBooksForManagers()
{

}

void SelectUserByManager(int UserId)
{
	ClearConsole();
	if (!IsAuthenticated)
	{
		ShowError("User Is Not Authenticated!!");
		Start();
		return;
	}
	User user = userSevices.Find(UserId);
	MenuInput menu;
	menu.Title = "Profile";

	menu.Items.emplace_back("Username : " + user.Username);
	menu.Items.emplace_back("Fullname : " + user.FullName);
	if (user.IsManager)
	{
		menu.Items.emplace_back("Role : Manager");
	}
	else
	{
		menu.Items.emplace_back("Role : Member");
	}
	menu.Items.emplace_back("Register Date : " + user.SignDate);
	menu.Items.emplace_back("Borrowed Books : " + bookCartServices.GetBorrowingCountForUser(UserId));
	menu.Items.emplace_back("\n----------------------------------------\n");
	menu.Items.emplace_back("1. Delete User");
	menu.Items.emplace_back("8. Back");

	ShowMenu(menu);

	int key;
	string k;
	cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		Profile();
		return;
	}

	key = stoi(k);
	switch (key)
	{
	case 1:

		userSevices.Remove(user.Username);
		ShowError("User With Id " + to_string(user.Id) + " Deleted Successfully!!");
		ShowUsersForManagers();
		break;
	case 8:
		ShowUsersForManagers();
		break;
	default:
		ShowError("Invalid Input!!");
		SelectUserByManager(UserId);
		break;
	}

}

void ShowUsersForManagers(int page)
{
	ClearConsole();
	MenuInput menu;
	menu.Title = "Users | Page " + to_string(page);
	ShowMenu(menu);
	vector<User> users = userSevices.GetUsersPaged(userSevices.AllUsers(), 10, page);
	ShowUsersList(users);
	cout << "1. Previous Page | 2. Next Page | 3.Select User | 4. Filter Borrowed Users | 5. Search | 8. Back\n";
	int key;
	string k;
	cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		Profile();
		return;
	}

	key = stoi(k);
	string temp;
	int l;
	switch (key)
	{
	case 1:
		page--;
		if (page < 1)
		{
			page = 1;
		}
		ShowUsersForManagers(page);
		break;

	case 2:
		page++;
		ShowUsersForManagers(page);
		break;

	case 3:
		print("User Id: ");

		cin >> temp;
		if (!IsNumber(temp))
		{
			ShowError("Invalid Input!!");
			ShowUsersForManagers(page);
			break;;
		}
		l = stoi(temp);

		if (!userSevices.IsExist(l))
		{
			ShowError("User not found!!");
			ShowUsersForManagers(page);
			break;
		}

		SelectUserByManager(l);
		break;

	case 4:

		break;

	case 5:

		break;

	case 8:
		ManagerMenu();
		break;
	default:
		ShowError("Invalid Input!!");
		ShowUsersForManagers();
		break;
	}

}

void AddManager()
{

	ClearConsole();
	if (!AuthUser.IsManager)
	{
		ShowError("Access Denied!!");
		Start();
		return;
	}

	MenuInput menu;
	menu.Title = "Add Manager";
	ShowMenu(menu);


	User user;
	user.IsManager = true;
	print("Username : ");
	cin >> user.Username;
	print("Password : ");
	cin >> user.Password;
	print("FullName : ");
	char fl[100];
	cin.getline(fl, 100);
	cin.getline(fl, 100);
	user.FullName = fl;

	if (userSevices.IsExist(user.Username))
	{
		ShowError("This Username is taken!!");
		ManagerMenu();
		return;
	}

	userSevices.Add(user);
	ShowError("Manager Added Successfully!!");
	ManagerMenu();
}

void Login()
{
	ClearConsole();
	MenuInput menu;
	menu.Title = "Login";
	ShowMenu(menu);
	string Username, Password;

	print("Username : ");
	cin >> Username;
	print("Password : ");
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
	print("Username : ");
	cin >> Username;
	print("Password : ");
	cin >> Password;
	print("FullName : ");
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
	menu.Items.emplace_back("1. Yes");
	menu.Items.emplace_back("2. No");
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
