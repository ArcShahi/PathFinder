#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "grid.hpp"

class Renderer
{
public:
	Renderer();
  
	void Draw(const Grid& grid);
private:

};

Renderer::Renderer()
{}

void Renderer::Draw(const Grid & grid)
{}


#endif // !RENDERER_HPP
