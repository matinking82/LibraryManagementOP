#include <chrono>
#include <iostream>
#include <vector>
#include <thread>
#include "Entities.h"
#include <conio.h>

UserServices userSevices("Data/Users.txt");
BookServices bookServices("Data/Books.txt");
BookCartServices bookCartServices("Data/BookCarts.txt");
CommentServices commentServices("Data/Comments.txt");
DateTools dateTools;

bool IsAuthenticated = false;
User AuthUser;

struct MenuInput
{
	string Title;
	vector<string> Items;
};

string GetPassword();
string _get_k();
void ClearConsole();
void print(string n, bool t = true);
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
void ShowBorrowedBooksForMembers(int page = 1);
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
void ShowBooksList(vector<Book> books, bool BorrowedBy = false);
void ShowBookCartsList(vector<BookCart> carts);
void EditProfile();

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

void ClearConsole()
{
	system("cls");
}

void print(string n, bool t)
{
	if (t)
	{
		cout << "\t";
	}
	cout << n;
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

void ShowBookCartsList(vector<BookCart> carts)
{
	for (BookCart cart : carts)
	{
		Book book = bookServices.Find(cart.BookId);


		print("Id: " + to_string(book.Id) + "\n");
		print("Name: " + book.Name + "\n");
		print("Author: " + book.Author + "\n");
		print("Borrowed Date: " + cart.StartDate + "\n");
		if (cart.IsGivenBack)
		{
			print("Status: Returned\n");
			print("Return Date: " + cart.EndDate + "\n");
		}
		else
		{
			print("Status: Not Returned\n");
		}
		cout << "-------------------------------------------------\n";
	}
}

void ShowBooksList(vector<Book> books, bool BorrowedBy)
{
	for (Book book : books)
	{
		print("Id: " + to_string(book.Id) + "\n");
		print("Name: " + book.Name + "\n");
		print("Author: " + book.Author + "\n");
		print("Genre: " + book.Genre + "\n");
		print("Added Date: " + book.AddDate + "\n");
		if (book.IsAvailable)
		{
			print("Status: Available\n");
		}
		else
		{
			print("Status: Not Available\n");
			if (BorrowedBy)
			{
				BookCart cart = bookCartServices.Find(book.Id);
				User user = userSevices.Find(cart.UserId);
				print("Borrowed By: " + user.Username + "(Id:" + to_string(user.Id) + ")" + "\n");
			}
		}
		cout << "-------------------------------------------------\n";
	}
}

void ShowCommentsList(vector<Comment> comments, bool showId = false)
{
	for (Comment comment : comments)
	{
		User user = userSevices.Find(comment.UserId);
		if (showId)
		{
			print(user.FullName + " (star: " + to_string(comment.Star) + ") (CommentId: " + to_string(comment.Id) + "):\n");
		}
		else
		{
			print(user.FullName + " (star: " + to_string(comment.Star) + ") :\n");
		}
		print("\t" + comment.Text);

		cout << "\n\n-------------------------------------------------\n";
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
	print("1. Previous Page | 2. Next Page | 3. Add a Comment | 8. Back\n", false);

	int key;
	string k;
	cin >> k;

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

		print("Star : ");
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
		print("comment : ");
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
	menu.Items.emplace_back("\n----------------------------------------\n");
	menu.Items.emplace_back("1. Borrow");
	menu.Items.emplace_back("2. Comments");
	menu.Items.emplace_back("8. Back");

	ShowMenu(menu);

	int key;
	string k;
	cin >> k;

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
		print("1. Previous Page | 2. Next Page | 3.Select Book | 4. Sort By Date | 8. Back\n", false);
	}
	else
	{
		print("1. Previous Page | 2. Next Page | 3.Select Book | 4. Sort By Genre | 8. Back\n", false);
	}

	int key;
	string k;
	cin >> k;

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
		print("Book Id: ");

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
	cin >> k;

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
		print("Search Key: ");
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
		print("1. Previous Page | 2. Next Page | 3.Select Book | 4. Search | 5. Sort By Date | 8. Back\n", false);
	}
	else
	{
		print("1. Previous Page | 2. Next Page | 3.Select Book | 4. Search | 5. Sort By Genre | 8. Back\n", false);
	}

	int key;
	string k;
	cin >> k;

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
		print("Book Id: ");

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

	ShowError("The Book Returned Successfully!!");
	ShowBorrowedBooksForMembers();
}

void ShowBorrowedBooksForMembers(int page)
{
	ClearConsole();
	MenuInput menu;
	menu.Title = "My Borrowed Books";
	ShowMenu(menu);

	vector<BookCart> CartsUser = bookCartServices.GetAllCartsForUser(AuthUser.Id, true);

	CartsUser = bookCartServices.GetPaged(CartsUser, 10, page);

	ShowBookCartsList(CartsUser);
	print("1. Previous Page | 2. Next Page | 3.Return Book | 8. Back\n", false);

	int key;
	string k;
	cin >> k;

	if (!IsNumber(k))
	{
		ShowError("Invalid Input!!");
		ShowBorrowedBooksForMembers(page);
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
		ShowBorrowedBooksForMembers(page);
		break;

	case 2:
		page++;
		ShowBorrowedBooksForMembers(page);
		break;

	case 3:
		print("Book Id: ");

		cin >> temp;
		if (!IsNumber(temp))
		{
			ShowError("Invalid Input!!");
			ShowBorrowedBooksForMembers(page);
			break;
		}
		l = stoi(temp);

		if (!bookServices.IsExist(l) || bookServices.Find(l).IsAvailable)
		{
			ShowError("Book not found!!");
			ShowBorrowedBooksForMembers(page);
			break;
		}

		GiveBackBook(l);
		break;

	case 8:
		MainMenu();
		break;

	default:
		ShowError("Invalid Input!!");
		ShowBorrowedBooksForMembers(page);
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
	print("Name : ");
	cin.getline(name, 150);
	print("Author : ");
	cin.getline(author, 150);
	print("Genre : ");
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
		print("1. Previous Page | 2. Next Page | 3.Select Book | 4. Add a Book | 5. Remove Filter | 8. Back\n", false);
	}
	else
	{
		print("1. Previous Page | 2. Next Page | 3.Select Book | 4. Add a Book | 5. Filter Borrowed Books | 8. Back\n", false);
	}

	int key;
	string k;
	cin >> k;


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
		print("Book Id: ");

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
		menu.Items.emplace_back("\n----------------------------------------");
		menu.Items.emplace_back("\nBorrowed Books:");
		vector<BookCart> carts = bookCartServices.GetAllCartsForUser(UserId);
		int i = 1;
		for (BookCart cart : carts)
		{
			if (cart.IsGivenBack)
			{
				continue;
			}
			Book book = bookServices.Find(cart.BookId);
			menu.Items.emplace_back(to_string(i) + ". Name: " + book.Name + " | Author: " + book.Author + " | Id: " + to_string(book.Id) + "\n");
			i++;
		}
	}
	else
	{
		menu.Items.emplace_back("Borrowed Books : " + to_string(BorrowCount));
	}

	menu.Items.emplace_back("\n----------------------------------------\n");
	if (!(user.Id == AuthUser.Id))
	{
		menu.Items.emplace_back("1. Delete User");
	}
	menu.Items.emplace_back("8. Back");

	ShowMenu(menu);

	int key;
	string k;
	cin >> k;

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
	menu.Items.emplace_back("What Do You Want To Change?\n");
	menu.Items.emplace_back("1. Name");
	menu.Items.emplace_back("2. Author");
	menu.Items.emplace_back("3. Genre");
	menu.Items.emplace_back("");
	menu.Items.emplace_back("8. Back");

	ShowMenu(menu);

	int key;
	string k;
	cin >> k;

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
		print("New Name : ");
		cin.getline(newName, 100);

		book.Name = newName;
		bookServices.Update(book);
		ShowError("Name Changed Successfully!!");
		SelectBookByManager(BookId);
		break;

	case 2:
		print("New Author : ");
		cin.getline(newAuthor, 100);

		book.Author = newAuthor;

		bookServices.Update(book);

		ShowError("Author Changed Successfully!!");
		SelectBookByManager(BookId);
		break;

	case 3:
		print("New Genre : ");

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
	print("1. Previous Page | 2. Next Page | 3. Delete a Comment | 8. Back\n", false);

	int key;
	string k;
	cin >> k;

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

		print("Comment Id: ");
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
		menu.Items.emplace_back("\n----------------------------------------");
		BookCart cart = bookCartServices.Find(book.Id);
		User user = userSevices.Find(cart.UserId);
		menu.Items.emplace_back("Currently Borrowed By\n\n");
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


	menu.Items.emplace_back("\n----------------------------------------\n");
	menu.Items.emplace_back("1. Delete");
	menu.Items.emplace_back("2. Edit");
	menu.Items.emplace_back("3. Manage Comments");
	menu.Items.emplace_back("8. Back");

	ShowMenu(menu);

	int key;
	string k;
	cin >> k;

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
		print("1. Previous Page | 2. Next Page | 3.Select User | 4. Remove Filter | 8. Back\n", false);
	}
	else
	{
		print("1. Previous Page | 2. Next Page | 3.Select User | 4. Filter Borrowed Users | 8. Back\n", false);
	}
	int key;
	string k;
	cin >> k;

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
		print("User Id: ");

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
	print("Username : ");
	cin >> user.Username;
	print("Password : ");
	user.Password = GetPassword();
	print("FullName : ");
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

	print("Username : ");
	cin >> Username;
	print("Password : ");
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
	print("Username : ");
	cin >> Username;
	print("Password : ");
	string Password = GetPassword();
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
	user.SignDate = dateTools.Now();
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
	menu.Items.emplace_back("0. Exit");
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
