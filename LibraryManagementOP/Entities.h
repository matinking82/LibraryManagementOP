#pragma once
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct User
{
	int Id;
	string Username;
	string Password;
	string FullName;
	string SignDate;
	bool IsManager;
};

struct Book
{
	int Id;
	string Name;
	string Author;
	string Genre;
	string AddDate;
	bool IsAvailable;
};

struct Comment
{
	int Id;
	int Star;//1-5
	string Text;
	int UserId;
	int BookId;
};

struct BookCart
{
	int UserId;
	int BookId;
	string StartDate;
	string EndDate;
	bool IsGivenBack;
};

struct Transaction
{
	int UserId;
	int Amount;
	bool InCome;
	string Date;
};

struct Penalty
{
	int Id;
	int UserId;
	int BookId;
	int Amount;
	bool IsPayed;
	string PayDate;
};

class UserServices
{
	string path;

public:

	UserServices(string dbPath)
	{
		path = dbPath;
	}

	vector<User> AllUsers()
	{
		ifstream reader(path);
		string line;
		vector<User> result;
		int step = 0;
		User user;
		while (getline(reader, line))
		{
			switch (step)
			{
			case 0:
				user.Id = stoi(line);
				break;

			case 1:
				user.Username = line;
				break;

			case 2:
				user.Password = line;
				break;

			case 3:
				user.FullName = line;
				break;

			case 4:
				user.SignDate = line;
				break;

			case 5:
				user.IsManager = stoi(line);
				break;
			default:
				break;
			}


			step++;
			if (step == 6)
			{
				result.emplace_back(user);
				step = 0;
			}
		}
		reader.close();
		return result;
	}

	vector<User> GetUsersPaged(vector<User> users, int count, int page = 1)
	{
		vector<User> Result;
		int start = 0;
		start += (count * (page - 1));
		for (; start < users.size() && count > 0; start++)
		{
			Result.emplace_back(users[start]);
			count--;
		}

		return Result;
	}

	void Remove(string userName)
	{
		vector<User> users = AllUsers();

		ofstream writer(path);
		for (User user : users)
		{
			if (user.Username == userName)
			{
				continue;
			}

			writer << to_string(user.Id) << endl;
			writer << user.Username << endl;
			writer << user.Password << endl;
			writer << user.FullName << endl;
			writer << user.SignDate << endl;
			writer << to_string(user.IsManager) << endl;

		}
		writer.close();
	}

	void Add(User user)
	{
		ofstream writer(path, ios::app);

		writer << user.Id << endl;
		writer << user.Username << endl;
		writer << user.Password << endl;
		writer << user.FullName << endl;
		writer << user.SignDate << endl;
		writer << user.IsManager << endl;

		writer.close();
	}

	void Update(User user)
	{
		ifstream reader(path);
		string txt = "";
		string line;
		User Fuser;
		int step = 0;
		while (getline(reader, line))
		{
			switch (step)
			{

			case 0:
				Fuser.Id = stoi(line);
				break;

			case 1:
				Fuser.Username = line;
				break;

			case 2:
				Fuser.Password = line;
				break;

			case 3:
				Fuser.FullName = line;
				break;

			case 4:
				Fuser.SignDate = line;
				break;

			case 5:
				Fuser.IsManager = stoi(line);
				break;
			}

			step++;
			if (step == 6)
			{
				if (user.Id == Fuser.Id)
				{
					Fuser.Username = user.Username;
					Fuser.FullName = user.FullName;
					Fuser.Password = user.Password;
					Fuser.SignDate = user.SignDate;
					Fuser.IsManager = user.IsManager;
				}

				txt += to_string(Fuser.Id) + "\n";
				txt += Fuser.Username + "\n";
				txt += Fuser.Password + "\n";
				txt += Fuser.FullName + "\n";
				txt += Fuser.SignDate + "\n";
				txt += to_string(Fuser.IsManager) + "\n";
				step = 0;
			}
		}
		reader.close();

		ofstream writer(path);
		writer << txt;
		writer.close();
	}

	bool IsExist(string userName)
	{
		ifstream reader(path);
		string line;
		int step = 0;
		while (getline(reader, line))
		{
			if (step == 1)
			{
				if (line.compare(userName) == 0)
				{
					reader.close();
					return true;
				}
			}

			step++;
			if (step == 6)
			{
				step = 0;
			}
		}
		reader.close();
		return false;
	}

	bool IsExist(int UserId)
	{
		ifstream reader(path);
		string line;
		int step = 0;
		while (getline(reader, line))
		{
			if (step == 0)
			{
				if (stoi(line) == UserId)
				{
					reader.close();
					return true;
				}
			}

			step++;
			if (step == 6)
			{
				step = 0;
			}
		}
		reader.close();
		return false;
	}

	User Find(string userName)
	{
		ifstream reader(path);
		string line;
		User user;
		int step = 0;
		while (getline(reader, line))
		{
			switch (step)
			{
			case 0:
				user.Id = stoi(line);
				break;

			case 1:
				user.Username = line;
				break;

			case 2:
				user.Password = line;
				break;

			case 3:
				user.FullName = line;
				break;

			case 4:
				user.SignDate = line;
				break;

			case 5:
				user.IsManager = stoi(line);
				break;
			default:
				break;
			}


			step++;
			if (step == 6)
			{
				if (user.Username.compare(userName) == 0)
				{
					reader.close();
					return user;
				}
				step = 0;
			}
		}
		reader.close();
	}

	User Find(int UserId)
	{
		ifstream reader(path);
		string line;
		User user;
		int step = 0;
		while (getline(reader, line))
		{
			switch (step)
			{
			case 0:
				user.Id = stoi(line);
				break;

			case 1:
				user.Username = line;
				break;

			case 2:
				user.Password = line;
				break;

			case 3:
				user.FullName = line;
				break;

			case 4:
				user.SignDate = line;
				break;

			case 5:
				user.IsManager = stoi(line);
				break;
			default:
				break;
			}


			step++;
			if (step == 6)
			{
				if (user.Id == UserId)
				{
					reader.close();
					return user;
				}
				step = 0;
			}
		}
		reader.close();
	}

	int LastId()
	{
		ifstream reader(path);
		string line;
		int Id = 0;
		int step = 0;
		while (getline(reader, line))
		{
			if (step == 0)
			{
				Id = stoi(line);
			}

			step++;
			if (step == 6)
			{
				step = 0;
			}
		}
		reader.close();

		return Id;
	}
};

class BookServices
{
	string path;
private:
	bool IsMatch(string s1, string s2)
	{
		int len1 = s1.length();
		int len2 = s2.length();

		if (len2 > len1)
		{
			return false;
		}

		int s = len1 - len2;
		for (; s >= 0; s--)
		{
			string temp = s1.substr(s, len2);
			for (int i = 0; i < len2; i++)
			{
				temp[i] = tolower(temp[i]);
				s2[i] = tolower(s2[i]);
			}
			if (temp == s2)
			{
				return true;
			}
		}

		return false;
	}
public:
	BookServices(string DbPath)
	{
		path = DbPath;
	}

	vector<Book> AllBooks()
	{
		ifstream reader(path);
		string line;
		int step = 0;
		vector<Book> result;
		Book book;
		while (getline(reader, line))
		{
			switch (step)
			{
			case 0:
				book.Id = stoi(line);
				break;

			case 1:
				book.Name = line;
				break;

			case 2:
				book.Author = line;
				break;

			case 3:
				book.Genre = line;
				break;

			case 4:
				book.AddDate = line;
				break;
			case 5:
				book.IsAvailable = stoi(line);
				break;

			default:
				break;
			}


			step++;
			if (step == 6)
			{
				result.emplace_back(book);
				step = 0;
			}
		}
		reader.close();
		return result;
	}

	vector<Book> SortBooksByGenre(vector<Book> books)
	{
		bool IsContinue = true;
		while (IsContinue)
		{
			IsContinue = false;

			for (int i = 0; i + 1 < books.size(); i++)
			{
				if (books[i].Genre.compare(books[i + 1].Genre) > 0)
				{
					Book t = books[i];
					books[i] = books[i + 1];
					books[i + 1] = t;
					IsContinue = true;
				}
			}
		}
		return books;
	}

	vector<Book> GetBooksPaged(vector<Book> books, int count, int page = 1)
	{
		vector<Book> Result;
		int start = 0;
		start += (count * (page - 1));
		for (; start < books.size() && count > 0; start++)
		{
			Result.emplace_back(books[start]);
			count--;
		}
		return Result;
	}

	void Remove(int Id)
	{
		vector<Book> books = AllBooks();
		ofstream writer(path);
		for (Book book : books)
		{
			if (book.Id == Id)
			{
				continue;
			}

			writer << to_string(book.Id) << endl;
			writer << book.Name << endl;
			writer << book.Author << endl;
			writer << book.Genre << endl;
			writer << book.AddDate << endl;
			writer << to_string(book.IsAvailable) << endl;
		}
		writer.close();
	}

	void Add(Book book)
	{
		ofstream writer(path, ios::app);

		writer << book.Id << endl;
		writer << book.Name << endl;
		writer << book.Author << endl;
		writer << book.Genre << endl;
		writer << book.AddDate << endl;
		writer << book.IsAvailable << endl;

		writer.close();
	}

	vector<Book> Search(string searchKey, bool name, bool author)
	{
		vector<Book> books = AllBooks();

		vector<Book> result;

		for (Book book : books)
		{
			if ((name && IsMatch(book.Name, searchKey)) || (author && IsMatch(book.Author, searchKey)))
			{
				result.emplace_back(book);
			}
		}

		return result;
	}

	void Update(Book book)
	{
		ifstream reader(path);
		string txt = "";
		string line;
		Book Fbook;
		int step = 0;
		while (getline(reader, line))
		{
			switch (step)
			{

			case 0:
				Fbook.Id = stoi(line);
				break;

			case 1:
				Fbook.Name = line;
				break;

			case 2:
				Fbook.Author = line;
				break;

			case 3:
				Fbook.Genre = line;
				break;

			case 4:
				Fbook.AddDate = line;
				break;

			case 5:
				Fbook.IsAvailable = stoi(line);
				break;
			}

			step++;
			if (step == 6)
			{
				if (book.Id == Fbook.Id)
				{
					Fbook.Name = book.Name;
					Fbook.Author = book.Author;
					Fbook.AddDate = book.AddDate;
					Fbook.Genre = book.Genre;
					Fbook.IsAvailable = book.IsAvailable;
				}

				txt += to_string(Fbook.Id) + "\n";
				txt += Fbook.Name + "\n";
				txt += Fbook.Author + "\n";
				txt += Fbook.Genre + "\n";
				txt += Fbook.AddDate + "\n";
				txt += to_string(Fbook.IsAvailable) + "\n";
				step = 0;
			}
		}
		reader.close();

		ofstream writer(path);
		writer << txt;
		writer.close();
	}

	bool IsExist(int Id)
	{
		ifstream reader(path);
		string line;
		int step = 0;
		while (getline(reader, line))
		{
			if (step == 0)
			{
				if (Id == stoi(line))
				{
					reader.close();
					return true;
				}
			}

			step++;
			if (step == 6)
			{
				step = 0;
			}
		}
		reader.close();
		return false;
	}

	bool IsExist(string name)
	{
		ifstream reader(path);
		string line;
		int step = 0;
		while (getline(reader, line))
		{
			if (step == 1)
			{
				if (line.compare(name) == 0)
				{
					reader.close();
					return true;
				}
			}

			step++;
			if (step == 6)
			{
				step = 0;
			}
		}
		reader.close();
		return false;
	}

	Book Find(int Id)
	{
		ifstream reader(path);
		string line;
		Book book;
		int step = 0;
		while (getline(reader, line))
		{
			switch (step)
			{
			case 0:
				book.Id = stoi(line);
				break;

			case 1:
				book.Name = line;
				break;

			case 2:
				book.Author = line;
				break;

			case 3:
				book.Genre = line;
				break;

			case 4:
				book.AddDate = line;
				break;
			case 5:
				book.IsAvailable = stoi(line);
				break;

			default:
				break;
			}


			step++;
			if (step == 6)
			{
				if (book.Id == Id)
				{
					reader.close();
					return book;
				}
				step = 0;
			}
		}
		reader.close();
	}

	int LastId()
	{
		ifstream reader(path);
		string line;
		int Id = 0;
		int step = 0;
		while (getline(reader, line))
		{
			if (step == 0)
			{
				Id = stoi(line);
			}

			step++;
			if (step == 6)
			{
				step = 0;
			}
		}
		reader.close();

		return Id;
	}
};

class CommentServices
{
	string path;
public:
	CommentServices(string DbPath)
	{
		path = DbPath;
	}

	vector<Comment> AllComments()
	{
		ifstream reader(path);
		string line;
		vector<Comment> result;
		int step = 0;
		Comment comment;
		while (getline(reader, line))
		{
			switch (step)
			{

			case 0:
				comment.Id = stoi(line);
				break;

			case 1:
				comment.Star = stoi(line);
				break;

			case 2:
				comment.Text = line;
				break;

			case 3:
				comment.UserId = stoi(line);
				break;

			case 4:
				comment.BookId = stoi(line);
				break;
			default:
				break;
			}

			step++;
			if (step == 5)
			{
				result.emplace_back(comment);
				step = 0;
			}
		}
		reader.close();
		return result;
	}

	vector<Comment> GetCommentsForBook(int bookId)
	{
		vector<Comment> comments = AllComments();
		vector<Comment> result;
		for (Comment comment : comments)
		{
			if (comment.BookId == bookId)
			{
				result.emplace_back(comment);
			}
		}
		return result;
	}

	void DeleteAllCommentsForUser(int userId)
	{
		vector<Comment> comments = AllComments();

		for (Comment comment : comments)
		{
			if (comment.UserId == userId)
			{
				Remove(comment.Id);
			}
		}
	}

	void DeleteAllCommentsForBook(int bookId)
	{
		vector<Comment> comments = AllComments();

		for (Comment comment : comments)
		{
			if (comment.BookId == bookId)
			{
				Remove(comment.Id);
			}
		}
	}

	vector<Comment> GetCommentsPaged(vector<Comment> comments, int count, int page = 1)
	{
		vector<Comment> Result;
		int start = 0;
		start += (count * (page - 1));
		for (; start < comments.size() && count > 0; start++)
		{
			Result.emplace_back(comments[start]);
			count--;
		}
		return Result;
	}

	string GetAverageStarForBook(int bookId)
	{
		vector<Comment> comments = AllComments();
		double sum = 0;
		int count = 0;
		for (Comment comment : comments)
		{
			if (comment.BookId == bookId)
			{
				sum += double(comment.Star);
				count++;
			}
		}
		if (count == 0)
		{
			return "0";
		}

		string res = to_string(sum / double(count));
		return res.substr(0, 3);
	}

	void Remove(int Id)
	{
		vector<Comment> comments = AllComments();
		ofstream writer(path);
		for (Comment comment : comments)
		{
			if (comment.Id == Id)
			{
				continue;
			}
			writer << comment.Id << endl;
			writer << comment.Star << endl;
			writer << comment.Text << endl;
			writer << comment.UserId << endl;
			writer << comment.BookId << endl;
		}
		writer.close();
	}

	Comment Find(int Id)
	{
		vector<Comment> comments = AllComments();
		for (Comment comment : comments)
		{
			if (comment.Id == Id)
			{
				return comment;
			}
		}
	}

	void Add(Comment comment)
	{
		ofstream writer(path, ios::app);

		writer << comment.Id << endl;
		writer << comment.Star << endl;
		writer << comment.Text << endl;
		writer << comment.UserId << endl;
		writer << comment.BookId << endl;

		writer.close();
	}

	int LastId()
	{
		ifstream reader(path);
		string line;
		int Id = 0;
		int step = 0;
		while (getline(reader, line))
		{
			if (step == 0)
			{
				Id = stoi(line);
			}

			step++;
			if (step == 5)
			{
				step = 0;
			}
		}
		reader.close();

		return Id;
	}

	bool IsExist(int Id)
	{
		vector<Comment> comments = AllComments();
		for (Comment comment : comments)
		{
			if (comment.Id == Id)
			{
				return true;
			}
		}
		return false;
	}

	vector<Comment> CommentsForBook(int BookId)
	{
		ifstream reader(path);
		string line;
		vector<Comment> result;
		int step = 0;
		while (getline(reader, line))
		{
			Comment comment;
			switch (step)
			{

			case 0:
				comment.Id = stoi(line);
				break;

			case 1:
				comment.Star = stoi(line);
				break;

			case 2:
				comment.Text = line;
				break;

			case 3:
				comment.UserId = stoi(line);
				break;

			case 4:
				comment.BookId = stoi(line);
				break;
			default:
				break;
			}

			step++;
			if (step == 5)
			{
				if (comment.BookId == BookId)
				{
					result.emplace_back(comment);
				}
				step = 0;
			}
		}
		reader.close();

		return result;
	}
};

class BookCartServices
{
	string path;
public:
	BookCartServices(string DbPath)
	{
		path = DbPath;
	}

	vector<BookCart> AllBookCarts()
	{
		ifstream reader(path);
		string line;
		vector<BookCart> result;
		int step = 0;
		BookCart bookCart;
		while (getline(reader, line))
		{
			switch (step)
			{

			case 0:
				bookCart.UserId = stoi(line);
				break;

			case 1:
				bookCart.BookId = stoi(line);
				break;

			case 2:
				bookCart.StartDate = line;
				break;

			case 3:
				bookCart.EndDate = line;
				break;

			case 4:
				bookCart.IsGivenBack = stoi(line);
				break;

			default:
				break;
			}

			step++;
			if (step == 5)
			{
				result.emplace_back(bookCart);
				step = 0;
			}
		}
		reader.close();


		return result;
	}

	int GetBorrowingCountForUser(int userId)
	{
		int count = 0;
		vector<BookCart> carts = AllBookCarts();
		for (BookCart cart : carts)
		{
			if (cart.IsGivenBack)
			{
				continue;
			}
			if (cart.UserId == userId && !cart.IsGivenBack)
			{
				count++;
			}
		}
		return count;
	}

	vector<BookCart> GetAllCartsForUser(int userId, bool reverse = false)
	{
		vector<BookCart> carts = AllBookCarts();
		vector<BookCart> result;
		for (BookCart cart : carts)
		{
			/*if (cart.IsGivenBack)
			{
				continue;
			}*/
			if (cart.UserId == userId)
			{
				result.emplace_back(cart);
			}
		}

		if (reverse)
		{
			vector<BookCart> temp;

			for (int i = result.size(); i >= 1; i--)
			{
				temp.emplace_back(result[i - 1]);
			}

			result = temp;

		}

		return result;
	}

	void Update(BookCart bookCart)
	{
		vector<BookCart> carts = AllBookCarts();

		ofstream writer(path);
		writer.close();

		for (BookCart cart : carts)
		{
			if (cart.BookId == bookCart.BookId && cart.StartDate == bookCart.StartDate)
			{
				cart = bookCart;
			}
			Add(cart);
		}
	}

	vector<	BookCart> GetPaged(vector<BookCart> carts, int count, int page)
	{
		vector<BookCart> Result;
		int start = 0;
		start += (count * (page - 1));
		for (; start < carts.size() && count > 0; start++)
		{
			Result.emplace_back(carts[start]);
			count--;
		}
		return Result;
	}

	BookCart Find(int BookId)
	{
		ifstream reader(path);
		string line;
		BookCart cart;
		int step = 0;
		while (getline(reader, line))
		{
			switch (step)
			{
			case 0:
				cart.UserId = stoi(line);
				break;

			case 1:
				cart.BookId = stoi(line);
				break;

			case 2:
				cart.StartDate = line;
				break;

			case 3:
				cart.EndDate = line;
				break;

			case 4:
				cart.IsGivenBack = stoi(line);
				break;

			default:
				break;
			}


			step++;
			if (step == 5)
			{
				if (!cart.IsGivenBack && cart.BookId == BookId)
				{
					reader.close();
					return cart;
				}
				step = 0;
			}
		}
		reader.close();
	}

	void Remove(int BookId)
	{
		vector<BookCart> bookCarts = AllBookCarts();

		ofstream writer(path);
		writer.close();

		for (BookCart cart : bookCarts)
		{
			if (cart.BookId != BookId)
			{
				Add(cart);
			}
		}
	}

	void Add(BookCart bookCart)
	{
		ofstream writer(path, ios::app);

		writer << bookCart.UserId << endl;
		writer << bookCart.BookId << endl;
		writer << bookCart.StartDate << endl;
		writer << bookCart.EndDate << endl;
		writer << bookCart.IsGivenBack << endl;

		writer.close();
	}
};

class TransactionServices
{
	string path;
public:

	TransactionServices(string DbPath)
	{
		path = DbPath;
	}

	vector<Transaction> AllTransactions()
	{
		ifstream reader(path);
		string line;
		vector<Transaction> result;
		int step = 0;
		Transaction tra;
		while (getline(reader, line))
		{
			switch (step)
			{

			case 0:
				tra.UserId = stoi(line);
				break;

			case 1:
				tra.Amount = stoi(line);
				break;

			case 2:
				tra.InCome = stoi(line);
				break;

			case 3:
				tra.Date = line;
				break;
			default:
				break;
			}

			step++;
			if (step == 4)
			{
				result.emplace_back(tra);
				step = 0;
			}
		}
		reader.close();

		return result;
	}

	void Add(Transaction transaction)
	{
		ofstream writer(path, ios::app);

		writer << transaction.UserId << endl;
		writer << transaction.Amount << endl;
		writer << transaction.InCome << endl;
		writer << transaction.Date << endl;

		writer.close();
	}

	int GetBalanceForUser(int UserId)
	{
		vector<Transaction> transactions = AllTransactions();
		int Result = 0;
		for (Transaction transaction : transactions)
		{
			if (transaction.UserId == UserId)
			{
				if (transaction.InCome)
				{
					Result += transaction.Amount;
				}
				else
				{
					Result -= transaction.Amount;
				}
			}
		}

		return Result;
	}

	vector<Transaction> GetPaged(vector<Transaction> transactions, int count, int page)
	{
		vector<Transaction> Result;
		int start = 0;
		start += (count * (page - 1));
		for (; start < transactions.size() && count > 0; start++)
		{
			Result.emplace_back(transactions[start]);
			count--;
		}
		return Result;
	}

	vector<Transaction> GetTransactionsForUser(int UserId)
	{
		vector<Transaction> transactions = AllTransactions();
		vector<Transaction> Result;
		for (Transaction transaction : transactions)
		{
			if (transaction.UserId == UserId)
			{
				Result.emplace_back(transaction);
			}
		}

		return Result;
	}
};

class PenaltyServices
{
	string path;
public:
	PenaltyServices(string DbPath)
	{
		path = DbPath;
	}

	void PayAllDebtsForUser(int UserId)
	{
		vector<Penalty> penalties = AllPenalties();
		for (Penalty penalty : penalties)
		{
			if (!penalty.IsPayed)
			{
				penalty.IsPayed = true;
				Update(penalty);
			}
		}
	}

	vector<Penalty> AllPenalties()
	{
		ifstream reader(path);
		string line;
		vector<Penalty> result;
		int step = 0;
		Penalty penalty;
		while (getline(reader, line))
		{
			switch (step)
			{

			case 0:
				penalty.Id = stoi(line);
				break;
			case 1:
				penalty.UserId = stoi(line);
				break;

			case 2:
				penalty.BookId = stoi(line);
				break;

			case 3:
				penalty.Amount = stoi(line);
				break;

			case 4:
				penalty.IsPayed = stoi(line);
				break;

			case 5:
				penalty.PayDate = line;
				break;
			default:
				break;
			}

			step++;
			if (step == 6)
			{
				result.emplace_back(penalty);
				step = 0;
			}
		}
		reader.close();

		return result;
	}

	int GetDebtForUser(int UserId)
	{
		vector<Penalty> penalties = AllPenalties();
		int debt = 0;
		for (Penalty penalty : penalties)
		{
			if (penalty.UserId == UserId && !penalty.IsPayed)
			{
				debt += penalty.Amount;
			}
		}
		return debt;
	}

	vector<Penalty> GetPaged(vector<Penalty> penalties, int count, int page)
	{
		vector<Penalty> Result;
		int start = 0;
		start += (count * (page - 1));
		for (; start < penalties.size() && count > 0; start++)
		{
			Result.emplace_back(penalties[start]);
			count--;
		}
		return Result;
	}

	vector<Penalty> GetAllPenaltiesForUser(int UserId)
	{
		vector<Penalty> penalties = AllPenalties();
		vector<Penalty> result;
		for (Penalty penalty : penalties)
		{
			if (penalty.UserId == UserId)
			{
				result.emplace_back(penalty);
			}
		}
		return result;
	}

	void Update(Penalty p)
	{
		ofstream writer(path);
		writer.close();

		vector<Penalty> penalties = AllPenalties();
		for (Penalty penalty : penalties)
		{
			if (penalty.Id == p.Id)
			{
				penalty = p;
			}

			Add(penalty);
		}
	}

	void Add(Penalty penalty)
	{
		ofstream writer(path, ios::app);

		writer << penalty.Id << endl;
		writer << penalty.UserId << endl;
		writer << penalty.BookId << endl;
		writer << penalty.Amount << endl;
		writer << penalty.IsPayed << endl;
		writer << penalty.PayDate << endl;

		writer.close();
	}

	int LastId()
	{
		int last = 0;
		vector<Penalty>	penalties = AllPenalties();
		for (Penalty penalty : penalties)
		{
			if (penalty.Id > last)
			{
				last = penalty.Id;
			}
		}
		return last;
	}
};

class DateTools//Format "YYYY/MM/DD 00:00"
{
public:
	string Now()
	{
		time_t now = time(0);
		char str[26];
		ctime_s(str, 26, &now);
		string t = str;
		string year = t.substr(20, 4);
		string hour = t.substr(11, 5);
		string day = t.substr(8, 2);
		string month = t.substr(4, 3);

		switch (month[0])
		{
		case 'J':
			if (month[1] == 'u')
			{
				if (month[2] == 'l')
				{
					month = "07";
				}
				else
				{
					month = "06";
				}
			}
			else
			{
				month = "01";
			}
			break;
		case 'F':
			month = "02";
			break;
		case 'M':
			if (month[2] == 'y')
			{
				month = "05";
			}
			else
			{
				month = "03";
			}
			break;
		case 'A':
			if (month[1] == 'p')
			{
				month = "04";
			}
			else
			{
				month = "08";
			}
			break;
		case 'S':
			month = "09";
			break;
		case 'O':
			month = "10";
			break;
		case 'N':
			month = "11";
			break;
		case 'D':
			month = "12";
			break;
		}

		return (year + "/" + month + "/" + day + " " + hour);
	}

	int DaysPassed(string start, string end)
	{
		int year = stoi(end.substr(0, 4)) - stoi(start.substr(0, 4));
		int month = stoi(end.substr(5, 2)) - stoi(start.substr(5, 2));
		int day = stoi(end.substr(8, 2)) - stoi(start.substr(8, 2));

		if (day < 0)
		{
			day = 30 + day;
			month--;
		}

		if (month < 0)
		{
			month = 12 + month;
			year--;
		}

		return (year * 365 + month * 30 + day);
	}
};
