#include "CalTarget.hpp"

#include <GL/glut.h>

namespace epilog
{

void CalTarget::draw(void) const
{
	glBegin(GL_TRIANGLES);
		glVertex3f(-0.5,-0.5,0.0);
		glVertex3f(0.5,0.0,0.0);
		glVertex3f(0.0,0.5,0.0);
	glEnd();
}

} // namespace epilog
