#include "SFMLPlot.h"

int main()
{
	// Create window
	SFMLPlot fig("Title");

	std::vector<float> x;
	int num = 1000;
	x.reserve(num);
	for (float i = 0; i < num; i++)
	{
		x.emplace_back(i / 4.0f);
	}

	std::vector<float> y;
	y.reserve(num);
	for (auto i = 0; i < num; i++)
	{
		y.push_back(x[i] * x[i]);
	}

	std::vector<float> y_;
	y_.reserve(num);
	for (auto i = 0; i < num; i++)
	{
		y_.push_back(x[i] * x[i] / 2.0f);
	}

	fig.plot(x, y, sf::Color::Red, 5.0f);
	fig.plot(x, y_, sf::Color::Green);

	SFMLPlot fig2("Title2");
	std::vector<int> x2{ 0,100, 100, 200 };//,200,300,400 };
	std::vector<int> y2{ 0,100, 200, 200 };//,200,300,400 };

	fig2.plot(x2, y2);

	SFMLPlot::show();
	return 0;

}
