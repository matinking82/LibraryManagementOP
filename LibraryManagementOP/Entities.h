#pragma once
#include <string>
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
};

struct Comments
{
	int Id;
	int Start;//1-5
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
