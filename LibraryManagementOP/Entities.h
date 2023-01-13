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
		static int id = LastId() + 1;
		user.Id = id;
		ofstream writer(path, ios::app);

		writer << user.Id << endl;
		writer << user.Username << endl;
		writer << user.Password << endl;
		writer << user.FullName << endl;
		writer << user.SignDate << endl;
		writer << user.IsManager << endl;

		writer.close();

		id++;
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

	void Remove(int Id)
	{
		vector<Comment> comments = AllComments();
		ofstream writer(path);
		for (Comment comment : comments)
		{
			writer << comment.Id << endl;
			writer << comment.Star << endl;
			writer << comment.Text << endl;
			writer << comment.UserId << endl;
			writer << comment.BookId << endl;
		}
		writer.close();
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
			if (cart.UserId == userId && !cart.IsGivenBack)
			{
				count++;
			}
		}
		return count;
	}

	void Remove(int Id)
	{
		vector<BookCart> bookCarts = AllBookCarts();

		ofstream writer(path);

		for (BookCart bookCart : bookCarts)
		{
			writer << bookCart.UserId << endl;
			writer << bookCart.BookId << endl;
			writer << bookCart.StartDate << endl;
			writer << bookCart.EndDate << endl;
			writer << bookCart.IsGivenBack << endl;
		}

		writer.close();

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
