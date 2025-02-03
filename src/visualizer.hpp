#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP


#include "pathfinder.hpp"

#include <string>

namespace na 
{
     class visualizer 
     {
     public:
          visualizer();
          ~visualizer();
          void run();

     private:
          void update();
          void render();

          pathfinder path_finder;
     };
}

#endif // VISUALIZER_HPP