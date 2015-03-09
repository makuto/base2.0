#ifndef COLLISION_HPP
#define COLLISION_HPP
#include <vector>
#include "../math/math.hpp"
//////////////////////////////////////////////////////////////////////////////////
//Axis aligned bounding boxes //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//NOTE: X & Y are assumed to be in the top left, so if there's offset you must adjust
struct vec2
{
	float x;
	float y;
};
class aabb
{
    public:
        float x, y, w, h;
        aabb();
        //Takes x, y, w, h
        aabb(float, float, float, float);
        void setPosition(float, float);
        void resize(float, float);
        float getX();
        float getY();
        float getWidth();
        float getHeight();
};
bool isColliding(aabb *, aabb*);
//preventCollision takes two bounding boxes and a velocity vector and modifies
//the velocity in order to prevent the two bounding boxes from colliding. The
//new velocity is stored in newVelX and newVelY. It is assumed that the boxes
//never overlap (so they shouldn't start when overlapping). Rand is used when
//on a corner to prevent hitching. TOUCHING_TOLERANCE sets the tolerance of
//what is considered touching and what is considered overlapping (useful if
//there are hitches when sliding).
//Note that movingObj should have the bounds it would have BEFORE the translation.
//When you adjust the velocity, do not change the bounds - leave them until
//all collision checks are complete.
void preventCollision(aabb& movingObj, aabb& staticObj, float velX, float velY, float& newVelX, float& newVelY, const float TOUCHING_TOLERANCE, bool debug);
bool resolveCollision(aabb*, aabb*, vec2* velocity);
//////////////////////////////////////////////////////////////////////////////////
//Simple sphere-to-sphere testing //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
class circleBound
{
    public:
        float x, y, r;
        circleBound();
        circleBound(float, float, float);
        void setPosition(float, float);
        void setRadius(float);
};
bool isColliding(circleBound *, circleBound*);

//Returns the manhattan distance from the first pair of coordinates to the second
//float manhattanTo(float, float, float, float); //See math
//////////////////////////////////////////////////////////////////////////////////
//SAT collision detection (ANY convex shape) /////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/**POTENTIAL IMPROVEMENTS
 * -Optimize the resolve code, it's quite large
 * -Clean up the function calls to the resolve code
 * -Make a file load bounding boxes with XML or something**/
struct point;
class satBound
{
    public:
    std::vector< point> vertices;
    std::vector< point> normals;
    int totalVertices;
    float x;
    float y;
    satBound();
    //Pass in the number of vertices the bound needs (ex 3 for a triangle)
    satBound(int);
    //Change the number of vertices in the bound (the previous vertices will stay)
    bool resetSize(unsigned int);
    //Pass in x,y,index , where index starts at 0
    void setVertex(float, float, int);
    //Generates the normals for the bounding box. This is a heavier function so call sparingly
    void generateNormals();
    //Sets the bound's position to 0
    void resetPosition();
    //Translates the bound. It has to loop through & manipulate every vertex, so try to reduce usage as much as necessary
    void setPosition(float, float);
};
/*True=colliding 
 * tarPos is assumed to be the same as tar->x and tar->y. If there was a collision tarPos will contain resolving position.
 * resolveAlignedCollision() is when you want to only test against the X and Y axes
 * resolveComplexCollision() is for testing non-axis-aligned bounds. It will test against all axes.*/
bool  resolveAlignedCollision(satBound *tar, satBound *stat, point *tarPos, point *statPos);
bool  resolveComplexCollision(satBound *tar, satBound *stat, point *tarPos, point *statPos);
#endif
