

#include"AStarHeader.h"

	Node::Node(long x, long y, long node_index)
        {
            this->x = x;
            this->y = y;
            this->node_index = node_index;

            this->f = 0;
            this->g = 0;
            // came_from ou *came_from??????
            this->came_from = NULL;

            this->appearance = "o"; // nó normal
        }
