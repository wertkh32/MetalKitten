#include "Collision.h"

bool
Collision::testLineCCWTriangle(Line& l, Triangle& t,  vector3d *out)
{

	vector3d ll = l.p1 - l.p0;
	vector3d lla = t.p0 - l.p0;
	vector3d llb = t.p1 - l.p0;
	vector3d llc = t.p2 - l.p0;
	
	// Test if pq is inside the edges bc, ca and ab. Done by testing
	// that the signed tetrahedral volumes, computed using scalar triple
	// products, are all positive
	
	float u = ll.dot(llc.cross(llb)); //ScalarTriple(pq, pc, pb);
	if (u < 0.0f) return false;
	float v = ll.dot(lla.cross(llc)); //ScalarTriple(pq, pa, pc);
	if (v < 0.0f) return false;
	float w = ll.dot(llb.cross(lla)); //ScalarTriple(pq, pb, pa);
	if (w < 0.0f) return false;
	
	// Compute the barycentric coordinates (u, v, w) determining the
	// intersection point r, r = u*a + v*b + w*c
	float denom = 1.0f / (u + v + w);
	
	if (out)
	{

		u *= denom;
		v *= denom;
		w *= denom; // w = 1.0f - u - v;
		*out = t.p0 * u + t.p1 * v + t.p2 * w;
	}
	//printf("%f %f %f\n", u, v, w);

	return true;
}

bool
Collision::testLineTriangle(Line& l, Triangle& t, vector3d* out)
{
	vector3d t10 = t.p1 - t.p0;
	vector3d t20 = t.p2 - t.p0;
	vector3d n = t10.cross(t20);

	float d = n.dot(t.p0);
	bool d0 = (n.dot(l.p0) - d) < 0;
	bool d1 = (n.dot(l.p1) - d) < 0;

	if (d0 == d1) return false;


	Triangle tp = Triangle(t.p0, t.p2, t.p1);
	return (testLineCCWTriangle(l, t, out) || testLineCCWTriangle(l, tp,out));
}


bool
Collision::testTriangleTriangle(Triangle& t1, Triangle& t2, vector3d* out)
{
	Line t1l1 = Line(t1.p0, t1.p1);
	Line t1l2 = Line(t1.p1, t1.p2);
	Line t1l3 = Line(t1.p2, t1.p0);

	Line t2l1 = Line(t2.p0, t2.p1);
	Line t2l2 = Line(t2.p1, t2.p2);
	Line t2l3 = Line(t2.p2, t2.p0);

	if (testLineTriangle(t1l1, t2,out) ||
		testLineTriangle(t2l1, t1, out) ||
		testLineTriangle(t1l2, t2, out) ||
		testLineTriangle(t2l2, t1, out) ||
		testLineTriangle(t1l3, t2, out) ||
		testLineTriangle(t2l3, t1, out))
		return true;

	return false;
}


bool
Collision::testAABBAABB(AABB& a, AABB& b)
{
	if (a.max.x < b.min.x || a.min.x > b.max.x) return false;
	if (a.max.y < b.min.y || a.min.y > b.max.y) return false;
	if (a.max.z < b.min.z || a.min.z > b.max.z) return false;
	return true;
}