#include<iostream>
#include<cmath>
#include<vector>
#include<string>
#include<queue>
#include<fstream>
#include<ctime>

using namespace std;

enum StudentType{good, average, bad};

class Answer {
public:
	double x1, x2;
	int numOfRoots;

	Answer() : x1(-100), x2(-100), numOfRoots(0) {}
};

class Equation {
public:
	double a, b, c;

	Equation() : a(0), b(0), c(0) {}
	Equation(double a, double b, double c) : a(a), b(b), c(c) {}

	Answer solve()
	{
		vector<double> roots;
		Answer ans;
		double D = b * b - 4 * a * c;
		if (D < 0)
		{
			ans.numOfRoots = 0;
		}
		if (D > 0)
		{
			ans.x1 = ((-b + sqrt(D)) / (2 * a));
			ans.x2 = ((-b - sqrt(D)) / (2 * a));
			ans.numOfRoots = 2;
		}
		else if (D == 0)
		{
		    ans.x1 = ans.x2 = (-b / (2 * a));
			ans.numOfRoots = 1;
		}

		return ans;
	}
};

class Email {
public:
	Equation eq;
	Answer ans;
	string name;

	Email(Equation eq, Answer ans, string name) : eq(eq), ans(ans), name(name) {}
};

class Teacher {
	class Result {
	public:
		string name;
		int solvedEqs;
	};

public:
	vector<Result> results;
	queue<Email> emails;

	bool isCorrectAns(Equation eq, Answer ans)
	{
		Answer correctAns = eq.solve();
		bool res = false;

		if (ans.numOfRoots != correctAns.numOfRoots)
		{
			res = false;
		}
		else
		{
			if ((abs(ans.x1 - correctAns.x1) < 1e-16 && (abs(ans.x2 - correctAns.x2) < 1e-16)) || (abs(ans.x1 - correctAns.x2) < 1e-16 && abs(ans.x2 - correctAns.x1) < 1e-16))
			{
				res = true;
			}
			else
			{
				res = false;
			}
		}

		return res;
	}

	void checkEmails()
	{
		while (!emails.empty())
		{
			bool studentInResult = false;
			for (int i = 0; i < results.size(); ++i)
			{
				if (results[i].name == emails.front().name)
				{
					studentInResult = true;
					if (isCorrectAns(emails.front().eq, emails.front().ans))
					{
						results[i].solvedEqs++;
					}
				}
			}

			if (!studentInResult)
			{
				Result new_res;
				new_res.name = emails.front().name;
				if (isCorrectAns(emails.front().eq, emails.front().ans))
				{
					new_res.solvedEqs = 1;
				}
				else
					new_res.solvedEqs = 0;
				results.push_back(new_res);
			}

			emails.pop();
		}
	}

	void postResults()
	{
		for (int i = 0; i < results.size(); ++i)
		{
			cout << "Student: " << results[i].name << " , Solved equations: " << results[i].solvedEqs << endl;
		}
	}

};

class Student {
public:
	string name;
	StudentType type;

	Student() : name("Неизвестно"), type(StudentType::average) {}
	Student(const string& name, StudentType type) : name(name), type(type) {}

	Answer solveEq(Equation eq)
	{
		Answer ans;

		if (type == good)
		{
			ans = eq.solve();
		}
		else if (type == average)
		{
			if (rand() % 2 == 0)
			{
				ans = eq.solve();
			}
			else
			{
				ans.x1 = ans.x2 = 0;
				ans.numOfRoots = 1;
			}
		}
		else if (type == bad)
		{
			ans.x1 = ans.x2 = 0;
			ans.numOfRoots = 1;
		}

		return ans;
	}

	void sendToTeacher(const Equation& eq, Teacher& tchr)
	{
		Answer ans = solveEq(eq);
		Email email = Email(eq, ans, name);
		tchr.emails.push(email);
	}
};

int countOfEqs = 10;
int countOfStudents = 10;

void generateEquations()
{
	ofstream file;
	file.open("Equations.txt");
	for (int i = 0; i < countOfEqs; ++i)
	{
		file << rand() % 100 + 1 << " " << rand() % 100 + 1 << " " << rand() % 100 + 1 << endl;
	}
	file.close();
}

vector<Student> createStudents()
{
	const string names[10] = { "Вайндрах Елизавета", "Чернорицкая Анастасия", "Иванов Андрей", "Ятманова Анна", "Киселёва Ксения", "Шутова Юлия", "Андреев Ярослав", "Григоьева Анастасия", "Кретова Дарья", "Воронцов Александр" };
	vector<Student> students;

	for (int i = 0; i < countOfStudents; ++i)
	{
		int randomType = rand() % 3;
		Student student;
		if (randomType == 0)
			student = Student(names[i], good);
		else if (randomType == 1)
			student = Student(names[i], average);
		else 
			student = Student(names[i], bad);
		students.push_back(student);
	}

	return students;
}

void studentsSolveEqs(const char* filename, vector<Student>& students, Teacher& tchr)
{
	ifstream file(filename);

	for (int i = 0; i < countOfEqs; ++i)
	{
		Equation eq;
		file >> eq.a >> eq.b >> eq.c;
		for (int j = 0; j < countOfStudents; ++j)
		{
			students[j].sendToTeacher(eq, tchr);
		}
	}
	file.close();
}

int main()
{
	setlocale(LC_ALL, "Russian");
	srand(time(0));
	generateEquations();

	vector<Student> students = createStudents();

	Teacher tchr;
	studentsSolveEqs("Equations.txt", students, tchr);

	tchr.checkEmails();
	tchr.postResults();
}
