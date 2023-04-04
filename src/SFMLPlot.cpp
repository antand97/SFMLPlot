#include "SFMLPlot.h"


SFMLPlot::SFMLPlot(const char* title_ = "Plot", bool grid_)
	: m_title{ title_ }
	, m_plot_resize{}
	, m_grid{ grid_ }
	, max_x{ 0 }
	, max_y{ 0 }
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	m_window_width = 1000;
	m_window_height = 1000;

	m_window.create(sf::VideoMode(m_window_width, m_window_height), m_title, sf::Style::Default, settings);
	m_window.clear(sf::Color::White);
	m_renderer_v.push_back(this);
	m_default_view = m_window.getDefaultView();
	index = curr_index;
	curr_index++;
	prevMousePosWindow = sf::Mouse::getPosition(m_window);
	prevMousePosView = m_window.mapPixelToCoords(prevMousePosWindow);


	if (!font_times.loadFromFile("src/fonts/Tinos-Regular.ttf"))
	{
		std::cout << "Font not found" << std::endl;
	}
	text.setFont(font_times);

	text.setFillColor(sf::Color::Black);
	text.setCharacterSize(20);
	text.setOrigin(0, 0);

}

SFMLPlot::~SFMLPlot()
{
	if (m_renderer_v.size() != 0)
	{
		curr_index--;
		m_renderer_v.erase(m_renderer_v.begin() + index);
		for (auto i = index; i < curr_index; i++)
		{
			m_renderer_v[i]->index--;
		}
		std::cout << "Figure " << this->m_title << " has been closed." << std::endl;
		std::cout << "Current number of figure: " << m_renderer_v.size() << std::endl;
	}
}
void SFMLPlot::update_axes() {

	auto resizer = sf::Vector2f(m_window_width / 2.0f / max_x, m_window_height / 2.0f / max_y);

	sf::Vector2f pos0_ax_x = transformer(sf::Vector2f(m_window_width / 2.0f - (m_view.getSize().x / 2.0f - (m_view.getCenter().x - m_window_width)), 0.0f));
	sf::Vector2f pos1_ax_x = pos0_ax_x + sf::Vector2f(m_view.getSize().x, 0.0f);

	sfLine axes = sfLine(pos0_ax_x, pos1_ax_x, 2.0f * (m_zoom_level.x / 2 + m_zoom_level.y / 2));
	m_window.draw(axes);

	sf::CircleShape triangle{ 1.0f };
	triangle.setPointCount(3);
	triangle.setOrigin(1.0f, 0.0f);
	triangle.setPosition((sf::Vector2f(pos1_ax_x.x, m_window_height / 2.0f)));
	triangle.setScale(sf::Vector2f(m_view.getSize().x / 100.0f, m_view.getSize().y / 100.0f));
	triangle.setRotation(90.0f);
	triangle.setFillColor(sf::Color::Black);
	m_window.draw(triangle);

	sf::Vector2f pos0_ax_y = transformer(sf::Vector2f(0.0f, -(m_window_height / 2.0f - (m_view.getSize().y / 2.0f - (m_view.getCenter().y - m_window_height)))));
	sf::Vector2f pos1_ax_y = pos0_ax_y + sf::Vector2f(0.0f, m_view.getSize().y);
	axes = sfLine(pos0_ax_y, pos1_ax_y, 2.0f * (m_zoom_level.x / 2 + m_zoom_level.y / 2));
	m_window.draw(axes);

	triangle.setPosition((sf::Vector2f(m_window_width / 2.0f, pos0_ax_y.y)));
	triangle.setRotation(0.0f);
	m_window.draw(triangle);

	// Draw text
	text.setScale((m_zoom_level.y + m_zoom_level.x) / 2, (m_zoom_level.y + m_zoom_level.x) / 2);
	text.setOrigin(0, 0);
	text.setCharacterSize(10 * m_window_width / 1000);
	int num_amount = 20;

	sf::Vertex line[2];
	line[0].color = sf::Color::Black;
	line[1].color = sf::Color::Black;

	sf::Vector2f position;
	int cur_pos;
	for (int i = -num_amount / 2; i <= num_amount / 2; i++)
	{
		// x_axis
		std::stringstream ss_x;
		cur_pos = (int)((m_view.getCenter().x - m_window_width / 2.0f) / (m_view.getSize().x / (num_amount)));

		ss_x << std::setprecision(2) << std::scientific << ((max_x * m_zoom_level.x) / (num_amount / 2)) * (i + cur_pos);
		text.setString(ss_x.str());
		position = sf::Vector2f(m_window_width / 2.0f + ((m_window_width / 2.0f) / (num_amount / 2)) * (i + cur_pos) * m_zoom_level.x, m_window_height / 2.0f);
		text.setPosition(position);
		float dist_y_up = m_view.getCenter().y - m_view.getSize().y / 2.0f - m_window_height / 2.0f;
		if (dist_y_up > 0.0f) {
			text.setPosition(position + sf::Vector2f( 0.0f, dist_y_up));
		}
		float dist_y_down = m_view.getCenter().y + m_view.getSize().y / 2.0f - text.getLocalBounds().height*2.0f - m_window_height / 2.0f;
		if (dist_y_down < 0.0f) {
			text.setPosition(position + sf::Vector2f(0.0f, dist_y_down));
		}
		m_window.draw(text);

		if (m_grid)
		{
			line[0].position = sf::Vector2f(position.x, pos0_ax_y.y);
			line[1].position = sf::Vector2f(position.x, pos1_ax_y.y);
		}
		else
		{
			line[0].position = position - sf::Vector2f(0.0f, 3.0f) * m_zoom_level;
			line[1].position = position + sf::Vector2f(0.0f, 3.0f) * m_zoom_level;
		}
		m_window.draw(line, 2, sf::Lines);


		// y_axis
		std::stringstream ss_y;
		cur_pos = (int)((m_view.getCenter().y - m_window_height / 2.0f) / (m_view.getSize().y / (num_amount)));
		ss_y << std::setprecision(2) << std::scientific << ((-max_y * m_zoom_level.y) / (num_amount / 2)) * (i + cur_pos);
		text.setString(ss_y.str());
		position = sf::Vector2f(m_window_width / 2.0f, m_window_height / 2.0f + ((m_window_height / 2.0f) / (num_amount / 2)) * (i + cur_pos) * m_zoom_level.y);
		text.setPosition(position);
		float dist_x_left = m_view.getCenter().x - m_view.getSize().x / 2.0f- m_window_width / 2.0f;
		if (dist_x_left > 0.0f) {
			text.setPosition(position +sf::Vector2f(dist_x_left,0.0f));
		}
		float dist_x_right = m_view.getCenter().x + m_view.getSize().x / 2.0f - text.getLocalBounds().width - m_window_width / 2.0f;
		if (dist_x_right < 0.0f) {
			text.setPosition(position + sf::Vector2f(dist_x_right, 0.0f));
		}
		m_window.draw(text);


		if (m_grid)
		{
			line[0].position = sf::Vector2f(pos0_ax_x.x, position.y);
			line[1].position = sf::Vector2f(pos1_ax_x.x, position.y);
		}
		else
		{
			line[0].position = position - sf::Vector2f(3.0f, 0.0f) * m_zoom_level;
			line[1].position = position + sf::Vector2f(3.0f, 0.0f) * m_zoom_level;
		}
		m_window.draw(line, 2, sf::Lines);

	}


}
void SFMLPlot::setThickness(float thickness_, int index)
{
	m_thickness[index] = thickness_;
}

void SFMLPlot::setGrid(bool set)
{
	m_grid = set;
}

bool SFMLPlot::inView(sf::Vector2f point)
{
	if ((point.x >= m_view.getCenter().x - m_view.getSize().x * 1.01 / 2.0f && point.x <= (m_view.getCenter().x + m_view.getSize().x * 1.01 / 2.0f))
		&&
		(point.y >= m_view.getCenter().y - m_view.getSize().y * 1.01 / 2.0f && point.y <= (m_view.getCenter().y + m_view.getSize().y * 1.01 / 2.0f)))
	{
		return true;
	}
	return false;
}

bool SFMLPlot::intersectView(sf::Vector2f pos0, sf::Vector2f pos1)
{
	sf::Vector2f viewBounds[4];
	viewBounds[0] = m_view.getCenter() + (m_view.getSize() / 2.0f) * sf::Vector2f(-1.0f, -1.0f);
	viewBounds[1] = m_view.getCenter() + (m_view.getSize() / 2.0f) * sf::Vector2f(1.0f, -1.0f);
	viewBounds[2] = m_view.getCenter() + (m_view.getSize() / 2.0f) * sf::Vector2f(1.0f, 1.0f);
	viewBounds[3] = m_view.getCenter() + (m_view.getSize() / 2.0f) * sf::Vector2f(-1.0f, 1.0f);

	float ax = pos1.x - pos0.x;     // direction of line a
	float ay = pos1.y - pos0.y;     // ax and ay as above

	bool result = false;
	float bx, by, dx, dy, det;

	for (auto i = 0; i < 4; i++) {
		if (i != 3)
		{
			bx = viewBounds[i].x - viewBounds[i + 1].x;     // direction of line b, reversed
			by = viewBounds[i].y - viewBounds[i + 1].y;     // really -by and -by as above
		}
		else
		{
			bx = viewBounds[3].x - viewBounds[0].x;     // direction of line b, reversed
			by = viewBounds[3].y - viewBounds[0].y;     // really -by and -by as above
		}

		dx = viewBounds[i].x - pos0.x;   // right-hand side
		dy = viewBounds[i].y - pos0.y;

		det = ax * by - ay * bx;

		if (det != 0)
		{
			float r = (dx * by - dy * bx) / det;
			float s = (ax * dy - ay * dx) / det;

			result = result || !(r < 0 || r > 1 || s < 0 || s > 1);
		}
	}
	return result;
}

void SFMLPlot::update_plot()
{

	sfLine line;


	sf::Vector2f pos0, pos1;
	sf::ConvexShape fill_triangle;
	fill_triangle.setPointCount(3);
	fill_triangle.setFillColor(sf::Color::Black);
	float prevAngle;
	int plot_index = 0;

	auto resizer = sf::Vector2f(m_window_width / 2.0f / max_x, m_window_height / 2.0f / max_y);
	pos0 = transformer(sf::Vector2f((float)x_ax[0], (float)y_ax[0]) * resizer);

	for (uint16_t i = 1; i < x_ax.size(); i++)
	{
		if (x_ax[i] == std::numeric_limits<float>::max())
		{
			i++;
			plot_index++;
			if (i < x_ax.size())
				pos0 = transformer(sf::Vector2f((float)x_ax[i], (float)y_ax[i]) * resizer);
			continue;
		}


		prevAngle = line.getAngle();

		
		pos1 = transformer(sf::Vector2f((float)x_ax[i], (float)y_ax[i]) * resizer);

		
		if (inView(pos0) || inView(pos1) || intersectView(pos0, pos1))
		{
			if (pointsDistance(pos0, pos1) > (m_view.getSize().x + m_view.getSize().y) / 200.0f
				|| x_ax[i + 1] == std::numeric_limits<float>::max())
			{

				line = sfLine(pos0, pos1, m_thickness[plot_index] * (m_zoom_level.x + m_zoom_level.y) / 2);
				fill_triangle.setFillColor(m_colors[plot_index]);
				line.setFillColor(m_colors[plot_index]);

				fill_triangle.setPoint(0, pos0);
				fill_triangle.setPoint(1, pos0 + vectorRotate(sf::Vector2f(0.0f, -line.getThickness() / 2.0f), prevAngle));
				fill_triangle.setPoint(2, pos0 + vectorRotate(sf::Vector2f(0.0f, -line.getThickness() / 2.0f), line.getAngle()));
				if (i != 1) m_window.draw(fill_triangle);
				fill_triangle.setPoint(0, pos0);
				fill_triangle.setPoint(1, pos0 + vectorRotate(sf::Vector2f(0.0f, line.getThickness() / 2.0f), prevAngle));
				fill_triangle.setPoint(2, pos0 + vectorRotate(sf::Vector2f(0.0f, line.getThickness() / 2.0f), line.getAngle()));
				if (i != 1) m_window.draw(fill_triangle);

				m_window.draw(line);

				pos0 = pos1;
			}
		}
		else
		{
			pos0 = pos1;
		}

	}



}
enum action_type {
	still, resize, move, rectResize
};
void SFMLPlot::winupdate(bool first_init)
{
	//m_window.clear(sf::Color::White);
	sf::Event ev;
	sf::Vector2i mousePosWindow = sf::Mouse::getPosition(m_window);
	sf::Vector2f mousePosView = m_window.mapPixelToCoords(mousePosWindow);

	action_type action = still;

	while (m_window.pollEvent(ev))
	{
		if (ev.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			m_window.close();
			this->~SFMLPlot();
			break;
		}
		if (ev.type == sf::Event::Resized)
		{
			action = resize;
			m_window_width = ev.size.width;
			m_window_height = ev.size.height;
			m_view.setSize((float)ev.size.width * m_zoom_level.x, (float)ev.size.height * m_zoom_level.y);
			m_default_view.setSize((float)ev.size.width, (float)ev.size.height);
			m_default_view.setCenter((float)ev.size.width / 2.0f, (float)ev.size.height / 2.0f);
			m_window.setView(m_view);


		}
		if (ev.type == sf::Event::MouseWheelMoved)
		{
			action = move;
			m_zoom_level -= m_zoom_level * (ev.mouseWheel.delta * 0.1f);
			m_view.zoom(1 - ev.mouseWheel.delta * 0.1f);
			m_view.move(((sf::Vector2f(m_window.mapPixelToCoords((mousePosWindow)) - sf::Vector2f(m_view.getCenter()))).x * ev.mouseWheel.delta * (ev.mouseWheel.delta * 0.1f)),
				((sf::Vector2f(m_window.mapPixelToCoords((mousePosWindow)) - sf::Vector2f(m_view.getCenter()))).y) * ev.mouseWheel.delta * (ev.mouseWheel.delta * 0.1f));
			m_window.setView(m_view);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			action = resize;
			m_view = m_default_view;
			m_window.setView(m_view);
			m_zoom_level = { 1.0f,1.0f };
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			action = move;
			m_view.move(sf::Vector2f((prevMousePosWindow - mousePosWindow)) * m_zoom_level);
			m_window.setView(m_view);
		}

		static bool rightMouseHeld = false;
		sf::Vector2f delta_mouse;
		static sf::Vector2f initial_pos;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			m_window.clear(sf::Color::White);
			action = rectResize;
			if (!rightMouseHeld)
			{
				initial_pos = mousePosView;
				rightMouseHeld = true;
			}

			delta_mouse = mousePosView - initial_pos;
			sf::Vertex select_rectangle[2];

			select_rectangle[0].color = sf::Color::Black;
			select_rectangle[1].color = sf::Color::Black;

			select_rectangle[0].position = initial_pos;
			select_rectangle[1].position = initial_pos + delta_mouse * sf::Vector2f(1.0f, 0.0f);
			m_window.draw(select_rectangle, 2, sf::Lines);
			select_rectangle[0].position = select_rectangle[1].position;
			select_rectangle[1].position = mousePosView;
			m_window.draw(select_rectangle, 2, sf::Lines);
			select_rectangle[0].position = select_rectangle[1].position;
			select_rectangle[1].position = initial_pos + delta_mouse * sf::Vector2f(0.0f, 1.0f);
			m_window.draw(select_rectangle, 2, sf::Lines);
			select_rectangle[0].position = select_rectangle[1].position;
			select_rectangle[1].position = initial_pos;
			m_window.draw(select_rectangle, 2, sf::Lines);
		}
		else if (rightMouseHeld)
		{
			action = resize;
			rightMouseHeld = false;
			sf::Vector2f delta_mouse = mousePosView - initial_pos;
			m_view.setCenter(delta_mouse / 2.0f + initial_pos);
			float delta_zoom = std::min(abs(delta_mouse.x) / m_view.getSize().x, abs(delta_mouse.y) / m_view.getSize().y);
			//m_zoom_level = m_zoom_level * sf::Vector2f(abs(delta_mouse.x / m_view.getSize().x), abs(delta_mouse.x / m_view.getSize().x));
			//m_view.setSize(abs(delta_mouse.x), abs(delta_mouse.y));
			m_zoom_level = m_zoom_level * delta_zoom;
			m_view.setSize(m_view.getSize() * delta_zoom);
			m_window.setView(m_view);
		}
	}
	prevMousePosView = mousePosView;
	prevMousePosWindow = mousePosWindow;



	if (m_window.isOpen())
	{	
		
		if (action == move || action == resize|| first_init)
		{
			m_window.clear(sf::Color::White);
			update_axes();
			
			update_plot();
		}
		else if (action == rectResize)
		{
			update_axes();

			update_plot();
		}
		m_window.display();
	}

}

void SFMLPlot::show()
{
	static bool first_init = true;
	while (!m_renderer_v.empty())
	{
		for (auto ren : m_renderer_v)
		{
			if (ren->m_window.hasFocus() || first_init) {
				ren->winupdate(first_init);
			}
			else
			{
				ren->prevMousePosWindow = sf::Mouse::getPosition(ren->m_window);
				ren->prevMousePosView = ren->m_window.mapPixelToCoords(ren->prevMousePosWindow);
				// Prevent unresponsive window
				sf::Event ev;
				while (ren->m_window.pollEvent(ev))
				{
				}
			}
		}
		first_init = false;
	}

}

sf::Vector2f SFMLPlot::transformer(sf::Vector2f v)
{
	v = v * sf::Vector2f(1.0f, -1.0f);
	v += sf::Vector2f(m_window_width / 2.0f, m_window_height / 2.0f);
	
	return v;
}

sf::Vector2f operator*(sf::Vector2f a, sf::Vector2f b)
{
	return sf::Vector2f(a.x*b.x,a.y*b.y);
}

std::ostream& operator<<(std::ostream& os, sf::Vector2f vec)
{
	os << vec.x << ' ' << vec.y;
	return os;
}

float pointsDistance(sf::Vector2f pos0, sf::Vector2f pos1)
{	
	return sqrt((pos0.x - pos1.x) * (pos0.x - pos1.x) + (pos0.y - pos1.y) * (pos0.y - pos1.y));
}

sf::Vector2f vectorRotate(sf::Vector2f v, float angle)
{
	sf::Vector2f r_v{};
	angle = angle * (PI / 180.0f); //Radians
	r_v.x = cos(angle) * v.x - sin(angle) * v.y;
	r_v.y = sin(angle) * v.x + cos(angle) * v.y;
	return r_v;
}

sfLine::sfLine(sf::Vector2f pos0, sf::Vector2f pos1, float thickness_)
	: m_pos0{pos0}
	, m_pos1{pos1}
	, m_angle{0.0f}
{
	m_thickness = thickness_;
	setPoints(pos0, pos1);
	this->setFillColor(sf::Color::Black);

}

void sfLine::setPoints(sf::Vector2f pos0, sf::Vector2f pos1)
{
	this->setOrigin(0.0f, m_thickness / 2.0f);
	this->setPosition(pos0);

	float length = pointsDistance(pos0, pos1);
	this->setSize(sf::Vector2f(length, m_thickness));
	if (length != 0)
	{
		float angle = acos(abs(pos0.x - pos1.x) / length) * (180.0f / PI);
		if (pos0.y > pos1.y) m_angle = -angle;
		else m_angle = angle;
	}

	this->setRotation(m_angle);
}

void sfLine::setThickness(float thickness_)
{
	m_thickness = thickness_;

	setPoints(m_pos0, m_pos1);
}

float sfLine::getThickness()
{
	return m_thickness;
}

float sfLine::getAngle()
{
	return m_angle;
}


