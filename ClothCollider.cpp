#include "ClothCollider.h"


ClothCollider::ClothCollider(MassSpringMesh* _mesh) :mesh(_mesh)
{
}

void
ClothCollider::initCollider()
{
	n = mesh->getNoMassPoints();
	forces = (float*)malloc(n * 3 * sizeof(float));
}

float*
ClothCollider::applyCollisionConstraint(float* cur_state, float* prev_state, float* prev_prev_state)
{
	for (int i = 0; i < n * 3; i++)
		forces[i] = 0;

	hashtable.update();

	for (int i = 0; i < mesh->getNoMassPoints(); i++)
	{
		Cell& cell = hashtable.getCell(mesh->getMassPoint(i).vertex);
		//printf("whuuut0");
		if (cell.size() > 0)
		{
			//printf("cellsize: %d\n", cell.size());
			vector3d p0 = vector3d(cur_state[i * 3], cur_state[i * 3 + 1], cur_state[i * 3 + 2]),
					p1 = vector3d(prev_state[i * 3], prev_state[i * 3 + 1], prev_state[i * 3 + 2]),
					p2 = vector3d(prev_prev_state[i * 3], prev_prev_state[i * 3 + 1], prev_prev_state[i * 3 + 2]);
				     
			Sphere sphere = Sphere(p0, SPHERE_RADIUS);

			for (int j = 0; j < cell.size(); j++)
			{
				Triangle* t = (Triangle*)cell[j];

				if (&(t->p0) == &(mesh->getMassPoint(i).vertex) ||
					&(t->p1) == &(mesh->getMassPoint(i).vertex) ||
					&(t->p2) == &(mesh->getMassPoint(i).vertex))
					continue;

				CollisionInfo info;

				if (Collision::testTriangleSphere(*t, sphere, &info))
				{
					vector3d dir = info.norm * 1.001;
					cur_state[i * 3] += dir.x;
					cur_state[i * 3 + 1] += dir.y;
					cur_state[i * 3 + 2] += dir.z;

					prev_state[i * 3] = prev_prev_state[i * 3];
					prev_state[i * 3 + 1] = prev_prev_state[i * 3 + 1];
					prev_state[i * 3 + 2] = prev_prev_state[i * 3 + 2];
				}
			}

		}
	}

	return forces;
}


void
ClothCollider::render()
{
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0, 1.0, 0.0);
	glPushMatrix();
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < triangles.size(); i++)
	{
		glVertex3fv(triangles[i]->p0.coords);
		glVertex3fv(triangles[i]->p2.coords);
		glVertex3fv(triangles[i]->p1.coords);
		
	}
	glEnd(); 
	glPopMatrix();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(1.0, 0.0, 0.0);
	glEnable(GL_LIGHTING);
}

ClothCollider::~ClothCollider()
{
	free(forces);
	for (int i = 0; i < triangles.size(); i++)
		delete triangles[i];
}
