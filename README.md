# SFMLPlot
Application based on SFML visual library for plotting data 

# Dependencies
The appication is written using SFML. In order to work, one has to download SFML 2.5.1 from https://www.sfml-dev.org/download.php, unzip it in a Dependencies folder and perform all the linker operations.

# How to use it
SFMLPlot is fairly simple. The class SFMLPlot is used to cerate a figure

```C++
SFMLPlot fig("Title");
```

which is currently empty. Is then possible to plot a set of lines defined by their starting and ending points in a vector.

```C++
std::vector<int> x{ 0,100, 100, 200 };
std::vector<int> y{ 0,100, 200, 200 };

fig2.plot(x2, y2);
```

To show the figure at the end of the code one should call

```C++
SFMLPlot::show();
```

Output:

![example1](https://user-images.githubusercontent.com/93544266/229771974-c6e284b1-e54c-42a8-8f36-2983c957d42f.PNG)

It is possible to plot multiple lines on the same figure and open multiple figures.

![example2](https://user-images.githubusercontent.com/93544266/229774387-059cad0b-90e3-49dd-a671-801da12f192f.PNG)

# Commands
- Left click  : move the view
- Mouse wheel : zoom in and out
- Right click : zoom in an area (buggy)
- Space       : reset the view

![commands_gif](https://user-images.githubusercontent.com/93544266/229774137-fcbc08f4-c2fe-4eca-a686-597482500c07.gif)

