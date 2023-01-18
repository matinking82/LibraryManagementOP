#include <chrono>
#include <iostream>
#include <vector>
#include <thread>
#include "Entities.h"
#include <windows.h>
#include <conio.h>

UserServices userSevices("Data/Users.txt");
BookServices bookServices("Data/Books.txt");
BookCartServices bookCartServices("Data/BookCarts.txt");
CommentServices commentServices("Data/Comments.txt");
TransactionServices transactionServices("Data/Transactions.txt");
PenaltyServices penaltyServices("Data/Penalties.txt");

DateTools dateTools;

bool IsAuthenticated = false;
User AuthUser;

struct MenuInput
{
	string Title;
	vector<string> Items;
};

string GetPassword();
void ClearConsole();
void print(string n, bool end = true);
void ShowMenu(MenuInput menu);
void ShowUsersList(vector<User> users);
void ShowError(string message);
void EditBookByManager(int BookId);
void ShowCommentsForMember(int BookId, int page = 1);
void SelectBookByMember(int BookId);
void MainMenu();
void ManagerMenu();
void SelectUserByManager(int UserId);
void ShowBooksForMembers(int page = 1, bool ByGenre = false);
void Profile();
void ShowBorrowedBooksForMembers(int page = 1, bool FilterNRetuned = false);
void ShowBooksForManagers(int page = 1, bool FilterBorrowed = false);
void GiveBackBook(int BookId);
void ShowUsersForManagers(int page = 1, bool FilterBorrowed = false);
void AddManager();
void AddBookByManager();
void SearchBookResult(string Searchkey, bool name, bool author, int page = 1, bool ByGenre = false);
void SelectBookByManager(int BookId);
void Login();
void ShowCommentsForManager(int bookId, int page = 1);
void SignUp();
void SignOut();
void Authenticate();
void SearchBookForMember();
void Start();
bool IsNumber(string s);
void ShowTransactionsList(vector<Transaction> transactions, bool manager = false);
void ShowBooksList(vector<Book> books, bool BorrowedBy = false);
void ShowBookCartsList(vector<BookCart> carts);
void EditProfile();
void PayDebt();
void ChargeAccountForMember();
void ChargeUserAccountByManager(int UserId);
void PaymentMenu();
void ShowTransactionsForUser(int page = 1);
void ShowAllTransactionsForManager(int page = 1);
void ShowPenaltiesForUser(int page = 1);
void printBorderLine();
void printBorder(bool d = false);

int main()
{
	Start();
}


string GetPassword()
{
	string pass = "";
	char inp;

	do
	{
		inp = _getch();


		if (inp == '\b')
		{
			int len = pass.length();
			if (len > 0)
			{
				pass = pass.substr(0, len - 1);
				cout << "\b \b";
			}
			continue;
		}
		if (inp == -32)
		{
			_getch();
		}

		if (!((inp >= 48 && inp <= 57) || (inp >= 64 && inp <= 90) || (inp >= 97 && inp <= 122) || inp == 46))
		{
			continue;
		}
		pass += inp;
		cout << "*";

	} while (inp != '\r');
	cout << '\n';
	return pass;
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

void ResetColors()
{
	HANDLE cout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(cout_handle, 100 | BACKGROUND_INTENSITY + 49);
}

void BorderColor()
{
	HANDLE cout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(cout_handle, BACKGROUND_RED + 100 | BACKGROUND_INTENSITY + 49);
}

void ClearConsole()
{
	ResetColors();
	system("cls");
}

void print(string n, bool end)
{
	printBorder();
	n = "    " + n;
	cout << n;
	if (end)
	{
		for (int i = 0; i < 116 - n.length(); i++)
		{
			cout << " ";
		}
		printBorder();
		cout << "\n";
	}
}

void printBorderLine()
{
	BorderColor();
	cout << "************************************************************************************************************************\n";
	ResetColors();
}

void printBorder(bool d)
{
	BorderColor();
	cout << "**";
	ResetColors();
	if (d)
	{
		cout << "                                                                                                                    ";
		BorderColor();
		cout << "**";
		ResetColors();
		cout << "\n";
	}
}

void ShowMenu(MenuInput menu)
{
	ClearConsole();
	printBorderLine();
	printBorder(true);
	printBorder();
	int widthSpace = 116 - menu.Title.length();
	int left = widthSpace / 2;
	int right = widthSpace - left;
	for (int i = 0; i < left; i++)
	{
		cout << " ";
	}
	cout << menu.Title;
	for (int i = 0; i < right; i++)
	{
		cout << " ";
	}
	printBorder();
	cout << "\n";
	printBorder(true);
	printBorderLine();
	if (menu.Items.size() > 0)
	{
		printBorder(true);
	}
	printBorder(true);

	for (string item : menu.Items)
	{
		if (item == "<Line>")
		{
			printBorder(true);
			printBorderLine();
			printBorder(true);
		}
		else
		{
			printBorder();
			cout << "\t" << item;
			for (int i = 0; i < 110 - item.length(); i++)
			{
				cout << " ";
			}
			printBorder();
			cout << "\n";
		}
	}
	if (menu.Items.size() > 0)
	{
		printBorder(true);
		printBorder(true);
	}
	printBorder(true);
}

void ShowUsersList(vector<User> users)
{
	for (User user : users)
	{
		printBorder(true);
		print("Id: " + to_string(user.Id));
		print("Username: " + user.Username);
		print("FullName: " + user.FullName);
		print("Register Date: " + user.SignDate);
		print("Books Borrowed: " + to_string(bookCartServices.GetBorrowingCountForUser(user.Id)));
		if (user.IsManager)
		{
			print("Role: Manager");
		}
		else
		{
			print("Role: Member");
		}
		printBorder(true);
		printBorderLine();
	}
}

void ShowBookCartsList(vector<BookCart> carts)
{
	for (BookCart cart : carts)
	{
		Book book = bookServices.Find(cart.BookId);

		printBorder(true);

		print("Id: " + to_string(book.Id));
		print("Name: " + book.Name);
		print("Author: " + book.Author);
		print("Borrowed Date: " + cart.StartDate);
		if (cart.IsGivenBack)
		{
			print("Status: Returned");
			print("Return Date: " + cart.EndDate);
		}
		else
		{
			print("Status: Not Returned");
		}
		printBorder(true);
		printBorderLine();
	}
}

void ShowBooksList(vector<Book> books, bool BorrowedBy)
{
	for (Book book : books)
	{
		printBorder(true);
		print("Id: " + to_string(book.Id));
		print("Name: " + book.Name);
		print("Author: " + book.Author);
		print("Genre: " + book.Genre);
		print("Added Date: " + book.AddDate);
		if (book.IsAvailable)
		{
			print("Status: Available");
		}
		else
		{
			print("Status: Not Available");
			if (BorrowedBy)
			{
				BookCart cart = bookCartServices.Find(book.Id);
				User user = userSevices.Find(cart.UserId);
				print("Borrowed By: " + user.Username + "(Id:" + to_string(user.Id) + ")");
			}
		}
		printBorder(true);
		printBorderLine();
	}
}

void ShowTransactionsList(vector<Transaction> transactions, bool manager)
{
	for (Transaction transaction : transactions)
	{
		printBorder(true);
		if (manager)
		{
			if (userSevices.IsExist(transaction.UserId))
			{
				User user = userSevices.Find(transaction.UserId);
				print(user.Username + ":");
			}
			else
			{
				print("Deleted User:");
			}
		}
		if (transaction.InCome)
		{
			print("+ Charged:");
		}
		else
		{
			print("- Payed:");
		}
		print("Amount: " + to_string(transaction.Amount) + "000 Tomans");
		print("Date: " + transaction.Date);
		printBorder(true);
		printBorderLine();
	}
}

void ShowPenaltiesList(vector<Penalty> penalties)
{
	for (Penalty penalty : penalties)
	{
		printBorder(true);
		Book book = bookServices.Find(penalty.BookId);
		print("Penalty For Book :'" + book.Name);
		print("Amount: " + to_string(penalty.Amount) + "000 Tomans");
		if (penalty.IsPayed)
		{
			print("Status: Payed");
			print("Pay Date: " + penalty.PayDate);
		}
		else
		{
			print("Status: Not Payed");
		}
		printBorder(true);
		printBorderLine();
	}
}

void ShowCommentsList(vector<Comment> comments, bool showId = false)
{
	for (Comment comment : comments)
	{
		printBorder(true);
		User user = userSevices.Find(comment.UserId);
		if (showId)
		{
			print(user.FullName + " (star: " + to_string(comment.Star) + ") (CommentId: " + to_string(comment.Id) + "):");
		}
		else
		{
			print(user.FullName + " (star: " + to_string(comment.Star) + ") :");
		}
		print("    " + comment.Text);

		printBorder(true);
		printBorderLine();
	}
}

void ShowError(string message)
{
	ClearConsole();
	cout << message;
	this_thread::sleep_for(chrono::milliseconds(1000));
}

void PayDebt()
{
	int balance = transactionServices.GetBalanceForUser(AuthUser.Id);
	int debt = penaltyServices.GetDebtForUser(AuthUser.Id);

	if (debt == 0)
	{
		ShowError("You Don't Have a Debt To Pay!!");
		PaymentMenu();
		return;
	}

	if (balance >= debt)
	{
		Transaction tr;
		tr.Amount = debt;
		tr.Date = dateTools.Now();
		tr.InCome = false;
		tr.UserId = AuthUser.Id;

		transactionServices.Add(tr);

		penaltyServices.PayAllDebtsForUser(AuthUser.Id);

		ShowError("Debt Payed Successfully!!");
		PaymentMenu();
	}
	else
	{
		ShowError("You don't have enough money. Please charge your account!!");
		PaymentMenu();
	}
}

void ChargeUserAccountByManager(int UserId)
{
	print("how much Do You Want To charge?");
	print("1.10000 Tomans");
	print("2.25000 Tomans");
	print("3.50000 Tomans");
	print("4.100000 Tomans");

	int key;
	string k;
	print("Key : ", false); cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		SelectUserByManager(UserId);
		return;
	}

	Transaction transaction;
	transaction.Date = dateTools.Now();
	transaction.InCome = true;
	transaction.UserId = UserId;

	key = stoi(k);
	switch (key)
	{
	case 1:
		transaction.Amount = 10;
		break;

	case 2:
		transaction.Amount = 25;

		break;

	case 3:
		transaction.Amount = 50;

		break;

	case 4:
		transaction.Amount = 100;
		break;

	default:
		ShowError("Invalid Input!!");
		SelectUserByManager(UserId);
		break;
	}

	transactionServices.Add(transaction);
	ShowError("Account Charged Successfully!!");
	SelectUserByManager(UserId);
}

void ChargeAccountForMember()
{
	print("how much Do You Want To charge?");
	print("1.10000 Tomans");
	print("2.25000 Tomans");
	print("3.50000 Tomans");
	print("4.100000 Tomans");

	int key;
	string k;
	print("Key : ", false); cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		PaymentMenu();
		return;
	}

	Transaction transaction;
	transaction.Date = dateTools.Now();
	transaction.InCome = true;
	transaction.UserId = AuthUser.Id;

	key = stoi(k);
	switch (key)
	{
	case 1:
		transaction.Amount = 10;
		break;

	case 2:
		transaction.Amount = 25;

		break;

	case 3:
		transaction.Amount = 50;

		break;

	case 4:
		transaction.Amount = 100;
		break;

	default:
		ShowError("Invalid Input!!");
		PaymentMenu();
		break;
	}

	transactionServices.Add(transaction);
	ShowError("Account Charged Successfully!!");
	PaymentMenu();
}

void ShowTransactionsForUser(int page)
{
	ClearConsole();
	vector<Transaction> transactions = transactionServices.GetTransactionsForUser(AuthUser.Id);
	transactions = transactionServices.GetPaged(transactions, 10, page);
	MenuInput menu;
	menu.Title = "My Transactions | Page " + to_string(page);
	ShowMenu(menu);
	ShowTransactionsList(transactions);
	print("1. Previous Page | 2. Next Page | 8. Back");
	printBorderLine();

	int key;
	string k;
	print("Key : ", false); cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		ShowTransactionsForUser(page);
		return;
	}

	key = stoi(k);
	switch (key)
	{
	case 1:
		page--;
		if (page < 1)
		{
			page = 1;
		}
		ShowTransactionsForUser(page);

		break;

	case 2:
		page++;
		ShowTransactionsForUser(page);
		break;

	case 8:
		PaymentMenu();
		break;

	default:
		ShowError("Invalid Input!!");
		ShowTransactionsForUser(page);
		break;
	}
}

void ShowPenaltiesForUser(int page)
{
	vector<Penalty> penalties = penaltyServices.GetAllPenaltiesForUser(AuthUser.Id);
	penalties = penaltyServices.GetPaged(penalties, 10, page);
	MenuInput menu;
	menu.Title = "My Penalties | Page " + to_string(page);
	ShowMenu(menu);
	ShowPenaltiesList(penalties);
	print("1. Previous Page | 2. Next Page | 8. Back");
	printBorderLine();

	int key;
	string k;
	print("Key : ", false); cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		ShowPenaltiesForUser(page);
		return;
	}

	key = stoi(k);
	switch (key)
	{
	case 1:
		page--;
		if (page < 1)
		{
			page = 1;
		}
		ShowPenaltiesForUser(page);

		break;

	case 2:
		page++;
		ShowPenaltiesForUser(page);
		break;

	case 8:
		PaymentMenu();
		break;

	default:
		ShowError("Invalid Input!!");
		ShowPenaltiesForUser(page);
		break;
	}
}

void PaymentMenu()
{
	MenuInput menu;

	menu.Title = "Payment";
	int Balance = transactionServices.GetBalanceForUser(AuthUser.Id);
	menu.Items.emplace_back("Your Balance :" + to_string(Balance) + "000 Tomans");
	menu.Items.emplace_back("Penalty:");
	int debt = penaltyServices.GetDebtForUser(AuthUser.Id);
	menu.Items.emplace_back("    You Have to Pay " + to_string(debt) + "000 Tomans");
	menu.Items.emplace_back("<Line>");
	menu.Items.emplace_back("1. Pay My Debt");
	menu.Items.emplace_back("2. Charge My Account");
	menu.Items.emplace_back("3. My Transactions");
	menu.Items.emplace_back("4. My Penalties");
	menu.Items.emplace_back("8. Back");

	ShowMenu(menu);
	int key;
	string k;
	print("Key : ", false); cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		PaymentMenu();
		return;
	}

	key = stoi(k);
	switch (key)
	{
	case 1:
		PayDebt();
		break;

	case 2:
		ChargeAccountForMember();
		break;

	case 3:
		ShowTransactionsForUser();
		break;

	case 4:
		ShowPenaltiesForUser();
		break;

	case 8:
		MainMenu();
		break;

	default:
		ShowError("Invalid Input!!");
		PaymentMenu();
		break;
	}
}

void ShowAllTransactionsForManager(int page)
{

	vector<Transaction> transactions = transactionServices.AllTransactions();
	transactions = transactionServices.GetPaged(transactions, 10, page);

	MenuInput menu;
	menu.Title = "All Transactions | Page " + to_string(page);
	ShowMenu(menu);
	ShowTransactionsList(transactions, true);
	print("1. Previous Page | 2. Next Page | 8. Back");
	printBorderLine();

	int key;
	string k;
	print("Key : ", false); cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		ShowAllTransactionsForManager(page);
		return;
	}

	key = stoi(k);
	switch (key)
	{
	case 1:
		page--;
		if (page < 1)
		{
			page = 1;
		}
		ShowAllTransactionsForManager(page);

		break;

	case 2:
		page++;
		ShowAllTransactionsForManager(page);
		break;

	case 8:
		ManagerMenu();
		break;

	default:
		ShowError("Invalid Input!!");
		ShowAllTransactionsForManager(page);
		break;
	}
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
	menu.Items.emplace_back("4. Payment");
	if (AuthUser.IsManager)
	{
		menu.Items.emplace_back("5. Manager Panel");
	}
	menu.Items.emplace_back("0. SignOut");
	ShowMenu(menu);
	int key;
	string k;
	print("Key : ", false); cin >> k;

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
		PaymentMenu();
		break;

	case 5:
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
	menu.Items.emplace_back("4. Finance");
	menu.Items.emplace_back("5. Main Menu");
	menu.Items.emplace_back("0. SignOut");
	ShowMenu(menu);
	int key;
	string k;
	print("Key : ", false); cin >> k;

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
		ShowAllTransactionsForManager();
		break;

	case 5:
		MainMenu();
		break;

	default:
		ShowError("Invalid Input!!");
		ManagerMenu();
		break;
	}
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
	menu.Items.emplace_back("<Line>");
	menu.Items.emplace_back("1. Edit");
	menu.Items.emplace_back("8. Back");

	ShowMenu(menu);

	int key;
	string k;
	print("Key : ", false); cin >> k;

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
	menu.Items.emplace_back("What Do You Want To Change?");
	menu.Items.emplace_back("");
	menu.Items.emplace_back("1. Username");
	menu.Items.emplace_back("2. Password");
	menu.Items.emplace_back("3. FullName");
	menu.Items.emplace_back("");
	menu.Items.emplace_back("8. Back");

	ShowMenu(menu);

	int key;
	string k;
	print("Key : ", false); cin >> k;

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
		print("New Username : ", false);
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
		oldPass = GetPassword();
		print("New Password : ");
		newPass = GetPassword();

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
		print("New FullName : ", false);
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

void BorrowBook(int BookId)
{
	BookCart cart;
	cart.BookId = BookId;
	cart.UserId = AuthUser.Id;
	cart.IsGivenBack = false;
	cart.StartDate = dateTools.Now();
	bookCartServices.Add(cart);
	Book book = bookServices.Find(BookId);
	book.IsAvailable = false;
	bookServices.Update(book);
}

void ShowCommentsForMember(int BookId, int page)
{
	ClearConsole();
	Book book = bookServices.Find(BookId);
	MenuInput menu;
	menu.Title = "Page " + to_string(page) + " | Comments For '" + book.Name + "'(star :" + commentServices.GetAverageStarForBook(BookId) + ") : ";
	ShowMenu(menu);

	vector<Comment> comments = commentServices.GetCommentsForBook(BookId);

	comments = commentServices.GetCommentsPaged(comments, 10, page);
	ShowCommentsList(comments);
	print("1. Previous Page | 2. Next Page | 3. Add a Comment | 8. Back");
	printBorderLine();

	int key;
	string k;
	print("Key : ", false); cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		ShowCommentsForMember(BookId, page);
		return;
	}

	key = stoi(k);
	string temp;
	Comment newComment;
	int star;
	switch (key)
	{
	case 1:
		page--;
		if (page < 1)
		{
			page = 1;
		}
		ShowCommentsForMember(BookId, page);
		break;

	case 2:
		page++;
		ShowCommentsForMember(BookId, page);
		break;

	case 3:
		newComment.BookId = BookId;
		newComment.Id = commentServices.LastId() + 1;
		newComment.UserId = AuthUser.Id;

		print("Star : ", false);
		cin >> temp;
		if (!IsNumber(temp))
		{
			ShowError("Invalid Input!!");
			ShowCommentsForMember(BookId, page);
			break;
		}
		star = stoi(temp);
		if (star > 5 || star < 0)
		{
			ShowError("Invalid Input!!");
			ShowCommentsForMember(BookId, page);
			break;
		}

		char t[250];
		print("comment : ", false);
		cin.getline(t, 250);
		cin.getline(t, 250);

		newComment.Star = star;
		newComment.Text = t;


		commentServices.Add(newComment);
		ShowError("Comment Added Successfully!!");
		ShowCommentsForMember(BookId, page);
		break;

	case 8:
		SelectBookByMember(BookId);
		break;

	default:
		ShowError("Invalid Input!!");
		ShowCommentsForMember(BookId, page);
		break;
	}
}

void SelectBookByMember(int BookId)
{
	ClearConsole();
	if (!IsAuthenticated)
	{
		ShowError("User Is Not Authenticated!!");
		Start();
		return;
	}
	Book book = bookServices.Find(BookId);
	MenuInput menu;
	menu.Title = "Book Detail";

	menu.Items.emplace_back("Name : " + book.Name);
	menu.Items.emplace_back("Author : " + book.Author);
	menu.Items.emplace_back("Genre : " + book.Genre);
	menu.Items.emplace_back("Added Date : " + book.AddDate);
	if (book.IsAvailable)
	{
		menu.Items.emplace_back("Status : Available");
	}
	else
	{
		menu.Items.emplace_back("Status : Not Available");
	}
	menu.Items.emplace_back("<Line>");
	menu.Items.emplace_back("1. Borrow");
	menu.Items.emplace_back("2. Comments");
	menu.Items.emplace_back("8. Back");

	ShowMenu(menu);

	int key;
	string k;
	print("Key : ", false); cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		SelectBookByMember(BookId);
		return;
	}

	key = stoi(k);
	switch (key)
	{
	case 1:
		BorrowBook(BookId);
		ShowError("Book Borrowed Successfully!!");
		ShowBooksForMembers();
		break;

	case 2:
		ShowCommentsForMember(book.Id);
		break;

	case 8:
		ShowBooksForMembers();
		break;

	default:
		ShowError("Invalid Input!!");
		SelectBookByMember(BookId);
		break;
	}
}

void SearchBookResult(string Searchkey, bool name, bool author, int page, bool ByGenre)
{
	ClearConsole();
	MenuInput menu;
	menu.Title = "Search : " + Searchkey + " | Page " + to_string(page);
	ShowMenu(menu);

	vector<Book> books;

	{
		vector<Book> temp = bookServices.Search(Searchkey, name, author);
		for (Book book : temp)
		{
			if (book.IsAvailable)
			{
				books.emplace_back(book);
			}
		}
	}
	if (ByGenre)
	{
		books = bookServices.SortBooksByGenre(books);
	}
	books = bookServices.GetBooksPaged(books, 10, page);

	ShowBooksList(books);
	if (ByGenre)
	{
		print("1. Previous Page | 2. Next Page | 3.Select Book | 4. Sort By Date | 8. Back");
	}
	else
	{
		print("1. Previous Page | 2. Next Page | 3.Select Book | 4. Sort By Genre | 8. Back");
	}
	printBorderLine();

	int key;
	string k;
	print("Key : ", false); cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		SearchBookResult(Searchkey, name, author, page, ByGenre);
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
		SearchBookResult(Searchkey, name, author, page, ByGenre);
		break;

	case 2:
		page++;
		SearchBookResult(Searchkey, name, author, page, ByGenre);
		break;

	case 3:
		print("Book Id: ", false);

		cin >> temp;
		if (!IsNumber(temp))
		{
			ShowError("Invalid Input!!");
			SearchBookResult(Searchkey, name, author, page, ByGenre);
			break;
		}
		l = stoi(temp);

		if (!bookServices.IsExist(l) || !bookServices.Find(l).IsAvailable)
		{
			ShowError("Book not found!!");
			SearchBookResult(Searchkey, name, author, page, ByGenre);
			break;
		}

		SelectBookByMember(l);
		break;
	case 4:
		SearchBookResult(Searchkey, name, author, page, !ByGenre);

		break;
	case 8:
		ShowBooksForMembers();
		break;

	default:
		ShowError("Invalid Input!!");
		SearchBookResult(Searchkey, name, author, page, ByGenre);
		break;
	}
}

void SearchBookForMember()
{
	ClearConsole();
	MenuInput menu;
	menu.Title = "Search Books";

	menu.Items.emplace_back("1. Search by Book Name");
	menu.Items.emplace_back("2. Search by Book Author");
	menu.Items.emplace_back("3. Search by Book Name and Author");
	menu.Items.emplace_back("8. Back");

	ShowMenu(menu);
	int key;
	string k;
	print("Key : ", false); cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		SearchBookForMember();
		return;
	}

	key = stoi(k);
	char temp[100];
	if (key > 0 && key <= 3)
	{
		print("Search Key: ", false);
		cin.getline(temp, 100);
		cin.getline(temp, 100);
	}
	string SearchKey = temp;
	switch (key)
	{
	case 1:
		SearchBookResult(SearchKey, true, false);
		break;

	case 2:
		SearchBookResult(SearchKey, false, true);
		break;

	case 3:
		SearchBookResult(SearchKey, true, true);
		break;

	case 8:
		ShowBooksForMembers();
		break;
	default:
		ShowError("Invalid Input!!");
		SearchBookForMember();
		break;
	}
}

void ShowBooksForMembers(int page, bool ByGenre)
{
	ClearConsole();
	MenuInput menu;
	menu.Title = "Books | Page " + to_string(page);
	ShowMenu(menu);

	vector<Book> books;

	{
		vector<Book> temp = bookServices.AllBooks();
		for (Book book : temp)
		{
			if (book.IsAvailable)
			{
				books.emplace_back(book);
			}
		}
	}
	if (ByGenre)
	{
		books = bookServices.SortBooksByGenre(books);
	}
	books = bookServices.GetBooksPaged(books, 10, page);

	ShowBooksList(books);
	if (ByGenre)
	{
		print("1. Previous Page | 2. Next Page | 3.Select Book | 4. Search | 5. Sort By Date | 8. Back");
	}
	else
	{
		print("1. Previous Page | 2. Next Page | 3.Select Book | 4. Search | 5. Sort By Genre | 8. Back");
	}
	printBorderLine();

	int key;
	string k;
	print("Key : ", false); cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		ShowBooksForMembers(page, ByGenre);
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
		ShowBooksForMembers(page, ByGenre);
		break;

	case 2:
		page++;
		ShowBooksForMembers(page, ByGenre);
		break;

	case 3:
		print("Book Id: ", false);

		cin >> temp;
		if (!IsNumber(temp))
		{
			ShowError("Invalid Input!!");
			ShowBooksForMembers(page, ByGenre);
			break;;
		}
		l = stoi(temp);

		if (!bookServices.IsExist(l) || !bookServices.Find(l).IsAvailable)
		{
			ShowError("Book not found!!");
			ShowBooksForMembers(page, ByGenre);
			break;
		}

		SelectBookByMember(l);
		break;

	case 4:
		SearchBookForMember();
		break;

	case 5:
		ShowBooksForMembers(page, !ByGenre);
		break;
	case 8:
		MainMenu();
		break;

	default:
		ShowError("Invalid Input!!");
		ShowBooksForMembers(page, ByGenre);
		break;
	}

}

void GiveBackBook(int BookId)
{
	BookCart cart = bookCartServices.Find(BookId);
	if (cart.UserId != AuthUser.Id)
	{
		ShowError("Book Not Found!!");
		ShowBorrowedBooksForMembers();
		return;
	}

	Book book = bookServices.Find(BookId);
	book.IsAvailable = true;
	bookServices.Update(book);

	//bookCartServices.Remove(BookId);

	cart.EndDate = dateTools.Now();
	cart.IsGivenBack = true;

	bookCartServices.Update(cart);

	int daysPassed = dateTools.DaysPassed(cart.StartDate, cart.EndDate);

	if (daysPassed > 14)
	{
		int penaltyDays = daysPassed - 14;
		Penalty penalty;
		penalty.BookId = BookId;
		penalty.UserId = AuthUser.Id;
		penalty.Amount = penaltyDays * 20;
		penalty.IsPayed = false;
		penalty.Id = penaltyServices.LastId() + 1;

		penaltyServices.Add(penalty);
		ShowError("You Didn't Return The Book in 14 Days And You have To pay " + to_string(penalty.Amount) + "000 Tomans");
	}

	ShowError("The Book Returned Successfully!!");
	ShowBorrowedBooksForMembers();
}

void ShowBorrowedBooksForMembers(int page, bool FilterNRetuned)
{
	ClearConsole();
	MenuInput menu;
	menu.Title = "My Borrowed Books";
	ShowMenu(menu);

	vector<BookCart> CartsUser = bookCartServices.GetAllCartsForUser(AuthUser.Id, true);

	if (FilterNRetuned)
	{
		vector<BookCart> temp;
		for (BookCart cart : CartsUser)
		{
			if (!cart.IsGivenBack)
			{
				temp.emplace_back(cart);
			}
		}
		CartsUser = temp;
	}

	CartsUser = bookCartServices.GetPaged(CartsUser, 10, page);

	ShowBookCartsList(CartsUser);

	if (FilterNRetuned)
	{
		print("1. Previous Page | 2. Next Page | 3.Return Book | 4. RemoveFilter | 8. Back");
	}
	else
	{
		print("1. Previous Page | 2. Next Page | 3.Return Book | 4. Filter Not Returned | 8. Back");
	}

	printBorderLine();

	int key;
	string k;
	print("Key : ", false); cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		ShowBorrowedBooksForMembers(page, FilterNRetuned);
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
		ShowBorrowedBooksForMembers(page, FilterNRetuned);
		break;

	case 2:
		page++;
		ShowBorrowedBooksForMembers(page, FilterNRetuned);
		break;

	case 3:
		print("Book Id: ", false);

		cin >> temp;
		if (!IsNumber(temp))
		{
			ShowError("Invalid Input!!");
			ShowBorrowedBooksForMembers(page, FilterNRetuned);
			break;
		}
		l = stoi(temp);

		if (!bookServices.IsExist(l) || bookServices.Find(l).IsAvailable)
		{
			ShowError("Book not found!!");
			ShowBorrowedBooksForMembers(page, FilterNRetuned);
			break;
		}

		GiveBackBook(l);
		break;

	case 4:
		ShowBorrowedBooksForMembers(page, !FilterNRetuned);
		break;

	case 8:
		MainMenu();
		break;

	default:
		ShowError("Invalid Input!!");
		ShowBorrowedBooksForMembers(page, FilterNRetuned);
		break;
	}
}

void AddBookByManager()
{
	ClearConsole();
	MenuInput menu;
	menu.Title = "Add Books";
	ShowMenu(menu);
	Book book;
	book.Id = bookServices.LastId() + 1;
	book.IsAvailable = true;

	char name[150], author[150], genre[150];

	cin.getline(name, 150);
	print("Name : ", false);
	cin.getline(name, 150);
	print("Author : ", false);
	cin.getline(author, 150);
	print("Genre : ", false);
	cin.getline(genre, 150);

	book.Name = name;
	book.Author = author;
	book.Genre = genre;
	book.AddDate = dateTools.Now();

	bookServices.Add(book);
	ShowError("Book Added Successfully");
	ShowBooksForManagers();
}

void ShowBooksForManagers(int page, bool FilterBorrowed)
{
	ClearConsole();
	MenuInput menu;
	menu.Title = "Books | Page " + to_string(page);
	ShowMenu(menu);

	vector<Book> books;

	if (FilterBorrowed)
	{
		vector<Book> temp = bookServices.AllBooks();
		for (Book book : temp)
		{
			if (!book.IsAvailable)
			{
				books.emplace_back(book);
			}
		}
	}
	else
	{
		books = bookServices.AllBooks();
	}

	books = bookServices.GetBooksPaged(books, 10, page);

	ShowBooksList(books, true);
	if (FilterBorrowed)
	{
		print("1. Previous Page | 2. Next Page | 3.Select Book | 4. Add a Book | 5. Remove Filter | 8. Back");
	}
	else
	{
		print("1. Previous Page | 2. Next Page | 3.Select Book | 4. Add a Book | 5. Filter Borrowed Books | 8. Back");
	}
	printBorderLine();

	int key;
	string k;
	print("Key : ", false); cin >> k;


	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		ShowBooksForManagers(page, FilterBorrowed);
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
		ShowBooksForManagers(page, FilterBorrowed);
		break;

	case 2:
		page++;
		ShowBooksForManagers(page, FilterBorrowed);
		break;

	case 3:
		print("Book Id: ", false);

		cin >> temp;
		if (!IsNumber(temp))
		{
			ShowError("Invalid Input!!");
			ShowBooksForManagers(page, FilterBorrowed);
			break;;
		}
		l = stoi(temp);

		if (!bookServices.IsExist(l))
		{
			ShowError("Book not found!!");
			ShowBooksForManagers(page, FilterBorrowed);
			break;
		}

		SelectBookByManager(l);
		break;

	case 4:
		AddBookByManager();
		break;

	case 5:
		ShowBooksForManagers(page, !FilterBorrowed);
		break;

	case 8:
		ManagerMenu();
		break;

	default:
		ShowError("Invalid Input!!");
		ShowBooksForManagers(page, FilterBorrowed);
		break;
	}

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
	menu.Title = "User";

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

	int BorrowCount = bookCartServices.GetBorrowingCountForUser(UserId);
	if (BorrowCount > 0)
	{
		menu.Items.emplace_back("<Line>");
		menu.Items.emplace_back("Borrowed Books:");
		vector<BookCart> carts = bookCartServices.GetAllCartsForUser(UserId);
		int i = 1;
		for (BookCart cart : carts)
		{
			if (cart.IsGivenBack)
			{
				continue;
			}
			Book book = bookServices.Find(cart.BookId);
			menu.Items.emplace_back(to_string(i) + ". Name: " + book.Name + " | Author: " + book.Author + " | Id: " + to_string(book.Id));
			menu.Items.emplace_back("");
			i++;
		}
	}
	else
	{
		menu.Items.emplace_back("Borrowed Books : " + to_string(BorrowCount));
	}
	int balance = transactionServices.GetBalanceForUser(UserId);
	menu.Items.emplace_back("Balance :" + to_string(balance) + "000 Tomans");

	menu.Items.emplace_back("<Line>");
	if (!(user.Id == AuthUser.Id))
	{
		menu.Items.emplace_back("1. Delete User");
	}
	menu.Items.emplace_back("2. Charge User Account");
	menu.Items.emplace_back("8. Back");

	ShowMenu(menu);

	int key;
	string k;
	print("Key : ", false); cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		SelectUserByManager(UserId);
		return;
	}

	key = stoi(k);
	vector<BookCart> carts;
	switch (key)
	{
	case 1:

		if (user.Id == AuthUser.Id)
		{
			ShowError("Invalid Input!!");
			SelectUserByManager(UserId);
		}

		userSevices.Remove(user.Username);

		carts = bookCartServices.GetAllCartsForUser(user.Id);
		for (BookCart cart : carts)
		{
			Book book = bookServices.Find(cart.BookId);
			book.IsAvailable = true;
			bookServices.Update(book);

			bookCartServices.Remove(cart.BookId);
		}
		commentServices.DeleteAllCommentsForUser(user.Id);

		ShowError("User With Id " + to_string(user.Id) + " Deleted Successfully!!");
		ShowUsersForManagers();
		break;

	case 2:
		ChargeUserAccountByManager(UserId);
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

void EditBookByManager(int BookId)
{
	ClearConsole();
	if (!IsAuthenticated)
	{
		ShowError("User Is Not Authenticated!!");
		Start();
		return;
	}
	Book book = bookServices.Find(BookId);
	MenuInput menu;
	menu.Title = "Edit Book";
	menu.Items.emplace_back("What Do You Want To Change?");
	menu.Items.emplace_back("");
	menu.Items.emplace_back("1. Name");
	menu.Items.emplace_back("2. Author");
	menu.Items.emplace_back("3. Genre");
	menu.Items.emplace_back("");
	menu.Items.emplace_back("8. Back");

	ShowMenu(menu);

	int key;
	string k;
	print("Key : ", false); cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		EditBookByManager(BookId);
		return;
	}

	char newGenre[100], newName[100], newAuthor[100];
	char fl[100];
	key = stoi(k);
	cin.getline(newGenre, 100);
	switch (key)
	{
	case 1:
		print("New Name : ", false);
		cin.getline(newName, 100);

		book.Name = newName;
		bookServices.Update(book);
		ShowError("Name Changed Successfully!!");
		SelectBookByManager(BookId);
		break;

	case 2:
		print("New Author : ", false);
		cin.getline(newAuthor, 100);

		book.Author = newAuthor;

		bookServices.Update(book);

		ShowError("Author Changed Successfully!!");
		SelectBookByManager(BookId);
		break;

	case 3:
		print("New Genre : ", false);

		cin.getline(newGenre, 100);

		book.Genre = newGenre;

		bookServices.Update(book);

		ShowError("Genre Changed Successfully!!");
		SelectBookByManager(BookId);
		break;

	case 8:
		SelectBookByManager(BookId);
		break;
	default:
		ShowError("Invalid Input!!");
		EditBookByManager(BookId);
		break;
	}
}

void ShowCommentsForManager(int bookId, int page)
{
	ClearConsole();
	Book book = bookServices.Find(bookId);
	MenuInput menu;
	menu.Title = "Page " + to_string(page) + " | Comments For '" + book.Name + "'(star :" + commentServices.GetAverageStarForBook(bookId) + ") : ";
	ShowMenu(menu);

	vector<Comment> comments = commentServices.GetCommentsForBook(bookId);

	comments = commentServices.GetCommentsPaged(comments, 10, page);
	ShowCommentsList(comments, true);
	print("1. Previous Page | 2. Next Page | 3. Delete a Comment | 8. Back");
	printBorderLine();

	int key;
	string k;
	print("Key : ", false); cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		ShowCommentsForManager(bookId, page);
		return;
	}

	key = stoi(k);
	int l;
	string temp;
	switch (key)
	{
	case 1:
		page--;
		if (page < 1)
		{
			page = 1;
		}
		ShowCommentsForManager(bookId, page);
		break;

	case 2:
		page++;
		ShowCommentsForManager(bookId, page);
		break;

	case 3:

		print("Comment Id: ", false);
		cin >> temp;

		if (!IsNumber(temp))
		{
			ShowError("Invalid Input!!");
			ShowCommentsForManager(bookId, page);
			break;
		}

		l = stoi(temp);
		if (!commentServices.IsExist(l) || commentServices.Find(l).BookId != bookId)
		{
			ShowError("Invalid Input!!");
			ShowCommentsForManager(bookId, page);
			break;
		}

		commentServices.Remove(l);
		ShowError("Comment Deleted Successfully!!");
		ShowCommentsForManager(bookId, page);
		break;

	case 8:
		SelectBookByManager(bookId);
		break;

	default:
		ShowError("Invalid Input!!");
		ShowCommentsForManager(bookId, page);
		break;
	}
}

void SelectBookByManager(int BookId)
{
	ClearConsole();
	if (!IsAuthenticated)
	{
		ShowError("User Is Not Authenticated!!");
		Start();
		return;
	}
	Book book = bookServices.Find(BookId);
	MenuInput menu;
	menu.Title = "Book Detail";

	menu.Items.emplace_back("Name : " + book.Name);
	menu.Items.emplace_back("Author : " + book.Author);
	menu.Items.emplace_back("Genre : " + book.Genre);
	menu.Items.emplace_back("Added Date : " + book.AddDate);
	if (book.IsAvailable)
	{
		menu.Items.emplace_back("Status : Available");
	}
	else
	{
		menu.Items.emplace_back("Status : Not Available");
		menu.Items.emplace_back("<Line>");
		BookCart cart = bookCartServices.Find(book.Id);
		User user = userSevices.Find(cart.UserId);
		menu.Items.emplace_back("Currently Borrowed By");
		menu.Items.emplace_back("");
		menu.Items.emplace_back("");
		menu.Items.emplace_back("Username : " + user.Username);
		menu.Items.emplace_back("FullName : " + user.FullName);
		if (user.IsManager)
		{
			menu.Items.emplace_back("Role : Manager");
		}
		else
		{
			menu.Items.emplace_back("Role : Member");
		}
	}


	menu.Items.emplace_back("<Line>");
	menu.Items.emplace_back("1. Delete");
	menu.Items.emplace_back("2. Edit");
	menu.Items.emplace_back("3. Manage Comments");
	menu.Items.emplace_back("8. Back");

	ShowMenu(menu);

	int key;
	string k;
	print("Key : ", false); cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		SelectBookByManager(BookId);
		return;
	}

	key = stoi(k);
	switch (key)
	{
	case 1:

		bookServices.Remove(book.Id);
		if (!book.IsAvailable)
		{
			bookCartServices.Remove(book.Id);
		}
		commentServices.DeleteAllCommentsForBook(book.Id);
		ShowError("Book With Id " + to_string(book.Id) + " Deleted Successfully!!");
		ShowBooksForManagers();
		break;

	case 2:
		EditBookByManager(BookId);
		break;

	case 3:
		ShowCommentsForManager(BookId);
		break;

	case 8:
		ShowBooksForManagers();
		break;
	default:
		ShowError("Invalid Input!!");
		SelectBookByManager(BookId);
		break;
	}
}

void ShowUsersForManagers(int page, bool FilterBorrowed)
{
	ClearConsole();
	MenuInput menu;
	menu.Title = "Users | Page " + to_string(page);
	ShowMenu(menu);

	vector<User> users;
	if (FilterBorrowed)
	{
		vector<User> temp = userSevices.AllUsers();

		for (User user : temp)
		{
			if (bookCartServices.GetBorrowingCountForUser(user.Id) != 0)
			{
				users.emplace_back(user);
			}
		}
	}
	else
	{
		users = userSevices.AllUsers();
	}

	users = userSevices.GetUsersPaged(users, 10, page);

	ShowUsersList(users);
	if (FilterBorrowed)
	{
		print("1. Previous Page | 2. Next Page | 3.Select User | 4. Remove Filter | 8. Back");
	}
	else
	{
		print("1. Previous Page | 2. Next Page | 3.Select User | 4. Filter Borrowed Users | 8. Back");
	}
	printBorderLine();
	int key;
	string k;
	print("Key : ", false); cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		ShowUsersForManagers(page, FilterBorrowed);
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
		ShowUsersForManagers(page, FilterBorrowed);
		break;

	case 2:
		page++;
		ShowUsersForManagers(page, FilterBorrowed);
		break;

	case 3:
		print("User Id: ", false);

		cin >> temp;
		if (!IsNumber(temp))
		{
			ShowError("Invalid Input!!");
			ShowUsersForManagers(page, FilterBorrowed);
			break;;
		}
		l = stoi(temp);

		if (!userSevices.IsExist(l))
		{
			ShowError("User not found!!");
			ShowUsersForManagers(page, FilterBorrowed);
			break;
		}

		SelectUserByManager(l);
		break;

	case 4:
		ShowUsersForManagers(page, !FilterBorrowed);
		break;

	case 8:
		ManagerMenu();
		break;

	default:
		ShowError("Invalid Input!!");
		ShowUsersForManagers(page, FilterBorrowed);
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
	print("Username : ", false);
	cin >> user.Username;
	print("Password : ", false);
	user.Password = GetPassword();
	print("FullName : ", false);
	char fl[100];
	cin.getline(fl, 100);
	cin.getline(fl, 100);
	user.FullName = fl;
	user.SignDate = dateTools.Now();
	if (userSevices.IsExist(user.Username))
	{
		ShowError("This Username is taken!!");
		ManagerMenu();
		return;
	}
	user.Id = userSevices.LastId() + 1;
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
	string Username;

	print("Username : ", false);
	cin >> Username;
	print("Password : ", false);
	string Password = GetPassword();

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
	string Username;
	char FullName[100];
	print("Username : ", false);
	cin >> Username;
	print("Password : ", false);
	string Password = GetPassword();
	print("FullName : ", false);
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
	user.SignDate = dateTools.Now();
	user.IsManager = false;
	user.Id = userSevices.LastId() + 1;

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
	menu.Items.emplace_back("0. Exit");
	ShowMenu(menu);
	int key;
	string k;
	print("Key : ", false); cin >> k;

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
	case 0:
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
