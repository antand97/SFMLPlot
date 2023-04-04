#pragma once

#include <iostream>
#include <iterator>
#include <vector>
#include <ctime>
#include <random>
#include <string>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <stdexcept>
#include <thread>
#include <future>
#include <iomanip>
#include <limits>

#define PI 3.141592654f


sf::Vector2f operator*(sf::Vector2f a, sf::Vector2f b);
std::ostream& operator<<(std::ostream& os, sf::Vector2f vec);

float pointsDistance(sf::Vector2f pos0, sf::Vector2f pos1);

sf::Vector2f vectorRotate(sf::Vector2f v, float angle);


class sfLine : public sf::RectangleShape
{
public:
	sfLine(sf::Vector2f pos0, sf::Vector2f pos1, float thickness_);

	sfLine() = default;

	void setPoints(sf::Vector2f pos0, sf::Vector2f pos1);

	void setThickness(float thickness_);

	float getThickness();

	float getAngle();

private:
	float m_thickness;

	sf::Vector2f m_pos0, m_pos1;

	float m_angle;

};


class SFMLPlot
{
public:

	SFMLPlot(const char* title_,bool grid_ = true);
	~SFMLPlot();


	template <typename X, typename Y>
	void plot(X x_axis, Y y_axis, sf::Color color_ = sf::Color::Black, float thickness_ = 2.0f);

	void update_plot();
	void update_axes();

	void setGrid(bool set = true);

	void setThickness(float thickness_, int index = 0);

	void winupdate(bool first_init);
	static void show();
	sf::Vector2f transformer(sf::Vector2f v);

	bool inView(sf::Vector2f point);

	bool intersectView(sf::Vector2f pos0, sf::Vector2f pos1);

	


private:

	sf::RenderWindow m_window;
	const char* m_title;
	uint8_t index;

	inline static std::vector<SFMLPlot*> m_renderer_v = {};
	inline static uint8_t curr_index = 0;
	sf::View m_view;
	sf::View m_default_view;
	int32_t m_window_width;
	int32_t m_window_height;

	sf::RenderTexture plotted_image;
	
	std::vector<float> x_ax, y_ax;
	sf::Vector2f m_zoom_level = { 1.0f,1.0f };

	sf::Vector2i prevMousePosWindow;
	sf::Vector2f prevMousePosView;

	sf::Font font_times;
	sf::Text text;

	sf::Vector2f m_plot_resize;

	float max_x, max_y;

	std::vector<sf::Color> m_colors;
	std::vector<float> m_thickness;
	bool m_grid;
};




// Template functions
template<typename X, typename Y>
inline void SFMLPlot::plot(X x_axis, Y y_axis, sf::Color color_, float thickness_)
{
	try
	{
		auto x_len = std::size(x_axis);
		auto y_len = std::size(y_axis);
		if (x_len != y_len)
		{
			std::string error_str = "Size of x(" + std::to_string(x_len) + ") does not match size of y(" + std::to_string(y_len) + ")";
			throw std::invalid_argument(error_str);
		}
		m_colors.push_back(color_);
		m_thickness.push_back(thickness_);

		// Draw figure

		for (uint16_t i = 0; i < x_len; i++)
		{
			x_ax.push_back((float)x_axis[i]);
			max_x = max_x < x_axis[i] ? x_axis[i] : max_x;
			max_y = max_y < y_axis[i] ? y_axis[i] : max_y;
			y_ax.push_back((float)y_axis[i]);
		}	
		x_ax.push_back(std::numeric_limits<float>::max());
		y_ax.push_back(std::numeric_limits<float>::max());
		
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << m_title << " cannot be plotted" << std::endl;
		std::cout << e.what() << std::endl;
	}
}

