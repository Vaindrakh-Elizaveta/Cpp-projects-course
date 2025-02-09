#include<iostream>
#include<cmath>


void get_coefficients(double &a, double &b, double &c)
{
	std::cout << "Введите коэффициенты a, b и с: ";
	std::cin >> a >> b >> c;
}

void find_roots(double a, double b, double c, double& root1, double& root2, bool& HasRealRoots)
{
	double D = b * b - 4 * a * c;

	if (D > 0)
	{
		root1 = (-b + std::sqrt(D)) / (2 * a);
		root2 = (-b - std::sqrt(D)) / (2 * a);
		HasRealRoots = true;
	}
	else if (D == 0)
	{
		root1 = root2 = -b / (2 * a);
		HasRealRoots = true;
	}
	else
	{
		HasRealRoots = false;
	}
}

void print_results(double root1, double root2, bool HasRealRoots)
{
	if (HasRealRoots == true)
	{
		if (root1 == root2)
		{
			std::cout << "Единственный корень: " << root1 << std::endl;
		}
		else
		{
			std::cout << "Первый корень: " << root1 << std::endl;
			std::cout << "Второй корень: " << root2 << std::endl;
		}
	}
	else
	{
		std::cout << "Уравнение не имеет вещественных корней.";
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");
	double a, b, c, root1, root2;
	bool HasRealRoots = true;

	get_coefficients(a, b, c);
	find_roots(a, b, c, root1, root2, HasRealRoots);
	print_results(root1, root2, HasRealRoots);

	return 0;
}
