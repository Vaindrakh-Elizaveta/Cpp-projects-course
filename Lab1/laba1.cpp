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

int main()
{
	setlocale(LC_ALL, "Russian");
	double a, b, c, root1, root2;
	bool HasRealRoots = true;

	get_coefficients(a, b, c);
	find_roots(a, b, c, root1, root2, HasRealRoots);

	return 0;
}
