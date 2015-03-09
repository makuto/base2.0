#ifndef COLLISION_CPP
#define COLLISION_CPP
#include <cmath>
#include <cstdlib> //For rand() in preventCollision()
#include "collision.hpp"
#include <iostream>
//180/pi. Useful for converting radians to degrees
#define RAD_TO_DEG 57.29577

/////////////////////////////////////
//AABB////////////////////////////
/////////////////////////////////////
aabb::aabb()
{
	x=y=w=h=0;
}
aabb::aabb(float newX, float newY, float newW, float newH)
{
    x=newX;
    y=newY;
    w=newW;
    h=newH;
}
void aabb::setPosition(float newX, float newY)
{
    x=newX;
    y=newY;
}
void aabb::resize(float newW, float newH)
{
    w=newW;
    h=newH;
}
float aabb::getX()
{
	return x;
}
float aabb::getY()
{
	return y;
}
float aabb::getWidth()
{
	return w;
}
float aabb::getHeight()
{
	return h;
}
bool isColliding(aabb* a, aabb* b)
{
    float aMaxX=a->x+a->w;
    float aMaxY=a->y+a->h;
    float bMaxX=b->x+b->w;
    float bMaxY=b->y+b->h;
    //They aren't overlapping on the X axis if the statement is true
    if (aMaxX<b->x || a->x > bMaxX) return false;
    //They aren't overlapping on the Y axis if the statement is true
    if (aMaxY<b->y || a->y > bMaxY) return false;
    return true;
}
//preventCollision takes two bounding boxes and a velocity vector and modifies
//the velocity in order to prevent the two bounding boxes from colliding. The
//new velocity is stored in newVelX and newVelY.
void preventCollision(aabb& movingObj, aabb& staticObj, float velX, float velY, float& newVelX, float& newVelY, const float TOUCHING_TOLERANCE, bool debug)
{
    float xDiff;
    float yDiff;
    //Debug ( 1 = x0 < x1 2 = x0 > x1 0 = x0 = x1)
    int xDiffState = -1;
    int yDiffState = -1;
    //For convenience
    float x0 = movingObj.x;
    float y0 = movingObj.y;
    float w0 = movingObj.w;
    float h0 = movingObj.w;
    float x1 = staticObj.x;
    float y1 = staticObj.y;
    float w1 = staticObj.w;
    float h1 = staticObj.w;

    if (debug) std::cout << "  x0 " << x0 << " y0 " << y0 << " w0 " << w0 << " h0 " << h0 << "\n";
    if (debug) std::cout << "  x1 " << x1 << " y1 " << y1 << " w1 " << w1 << " h1 " << h1 << "\n";
    if (debug) std::cout << "  xvel " << velX << " yvel " << velY << "\n";

    //Get differences between the two closest sides of the bounds (per axis)
    bool isXseparate = false;
    bool isYseparate = false;
    bool isXtouching = false;
    bool isYtouching = false;
    if (x0 < x1)
    {
        xDiff = x1 - (x0 + w0);
        xDiffState = 1;
        if (xDiff >= 0) isXseparate = true;
        if (xDiff == 0) isXtouching = true;
        if (fabs(xDiff)<=TOUCHING_TOLERANCE)
        {
            isXseparate = true;
            isXtouching = true;
        }
    }
    else if (x0 > x1)
    {
        xDiff = (x1 + w1) - x0;
        xDiffState = 2;
        if (xDiff <= 0) isXseparate = true;
        if (xDiff == 0) isXtouching = true;
        if (fabs(xDiff)<=TOUCHING_TOLERANCE)
        {
            isXseparate = true;
            isXtouching = true;
        }
    }
    else
    {
        xDiff = 0;
        xDiffState = -1;
    }
    if (debug) std::cout << "  xDiff " << xDiff << " diffState " << xDiffState << " separate " << isXseparate << " touching " << isXtouching << "\n";
    if (y0 < y1)
    {
        yDiff = y1 - (y0 + h0);
        yDiffState = 1;
        if (yDiff >= 0) isYseparate = true;
        if (yDiff == 0) isYtouching = true;
        if (fabs(yDiff)<=TOUCHING_TOLERANCE)
        {
            isYseparate = true;
            isYtouching = true;
        }
    }
    else if (y0 > y1)
    {
        yDiff = (y1 + h1) - y0;
        yDiffState = 2;
        if (yDiff <= 0) isYseparate = true;
        if (yDiff == 0) isYtouching = true;
        if (fabs(yDiff)<=TOUCHING_TOLERANCE)
        {
            isYseparate = true;
            isYtouching = true;
        }
    }
    else
    {
        yDiff = 0;
        yDiffState = -1;
    }
    if (debug) std::cout << "  yDiff " << yDiff << " diffState " << yDiffState << " separate " << isYseparate << " touching " << isYtouching << "\n";

    float xVel = 0;
    float yVel = 0;
    if (isXseparate && isYseparate)
    {
        //Handle corners
        if (xDiff==0 && yDiff==0)
        {
            //Pick either axis and slide that way (perfect corner)
            //xVel = 0; 
            //yVel = 0;
            if (x0 < x1)
            {
                if (y0 < y1)
                {
                    bool xReceding = false;
                    bool yReceding = false;
                    if (velX < 0) xReceding = true;
                    if (velY < 0) yReceding = true;
                    if (xReceding || yReceding) //Don't allow if neither are moving away
                    {
                        xVel = velX;
                        yVel = velY;
                    }
                    else
                    {
                        if (rand() % 2 == 0) //Pick a random axis
                        {
                            xVel = velX;
                            yVel = 0;
                        }
                        else
                        {
                            yVel = velY;
                            xVel = 0;
                        }
                    }
                }
                else
                {
                    bool xReceding = false;
                    bool yReceding = false;
                    if (velX < 0) xReceding = true;
                    if (velY > 0) yReceding = true;
                    if (xReceding || yReceding) //Don't allow if neither are moving away
                    {
                        xVel = velX;
                        yVel = velY;
                    }
                    else
                    {
                        if (rand() % 2 == 0) //Pick a random axis
                        {
                            xVel = velX;
                            yVel = 0;
                        }
                        else
                        {
                            yVel = velY;
                            xVel = 0;
                        }
                    }
                }
            }
            else
            {
                if (y0 < y1)
                {
                    bool xReceding = false;
                    bool yReceding = false;
                    if (velX > 0) xReceding = true;
                    if (velY < 0) yReceding = true;
                    if (xReceding || yReceding) //Don't allow if neither are moving away
                    {
                        xVel = velX;
                        yVel = velY;
                    }
                    else
                    {
                        if (rand() % 2 == 0) //Pick a random axis
                        {
                            xVel = velX;
                            yVel = 0;
                        }
                        else
                        {
                            yVel = velY;
                            xVel = 0;
                        }
                    }
                }
                else
                {
                    bool xReceding = false;
                    bool yReceding = false;
                    if (velX > 0) xReceding = true;
                    if (velY > 0) yReceding = true;
                    if (xReceding || yReceding) //Don't allow if neither are moving away
                    {
                        xVel = velX;
                        yVel = velY;
                    }
                    else
                    {
                        if (rand() % 2 == 0) //Pick a random axis
                        {
                            xVel = velX;
                            yVel = 0;
                        }
                        else
                        {
                            yVel = velY;
                            xVel = 0;
                        }
                    }
                }
            }
            
        }
        else
        {
            float toiX = 0;
            if (velX != 0) toiX = xDiff / velX;
            float toiY = 0;
            if (velY != 0) toiY = yDiff / velY;
            if (toiX > 1) xVel = velX;
            else if (toiX < 0) xVel = velX;
            else if (isYseparate) xVel = velX;
            else xVel = (velX) * toiX;
            if (toiY > 1) yVel = velY;
            else if (toiY < 0) yVel = velY;
            else if (isXseparate) yVel = velY;
            else yVel = (velY) * toiY;
            if (isXseparate && velX==0) yVel = velY;
            if (isYseparate && velY==0) xVel = velX;
            if (debug) std::cout << "  toix " << toiX << " toiY " << toiY << "\n";
        }
    }
    else
    {
        //Y axis focused
        if (isXseparate)
        {
            yVel = velY;
        }
        else if (isYseparate && !isYtouching)
        {
            float toiY = yDiff / velY;
            if (toiY > 1) yVel = velY;
            else if (toiY <= 0) yVel = velY;
            else yVel = (velY) * toiY;
            if (debug) std::cout << "  toiy " << toiY << "\n";
            if (debug) std::cout << "  new yVel " << yVel << "\n";
        }
        else if (isYseparate && isYtouching) //Resting against object
        {
            switch(yDiffState)
            {
                case 1: //Moving object is above stationary object
                    if (velY < 0) yVel = velY; //Moving away (up)
                    if (debug) std::cout << "velY allowed (moving away up)\n";
                    break;
                case 2: //Moving object is below stationary object
                    if (velY > 0) yVel = velY; //Moving away (down)
                    if (debug) std::cout << "velY allowed (moving away down)\n";
                    break;
                default:
                    if (debug) std::cout << "Default break\n";
            }
            if (debug) std::cout << "  velX " << velX << " velY " << velY << " [post resting]\n";
            if (debug) std::cout << "  xvel " << xVel << " yvel " << yVel << " [post resting]\n";
        }
        //X axis focused
        if (isYseparate)
        {
            xVel = velX;
            if (debug) std::cout << "Y separate; xVel is full\n";
        }
        else if (isXseparate && !isXtouching)
        {
            float toiX = xDiff / velX;
            if (toiX > 1) xVel = velX;
            else if (toiX <= 0) xVel = velX;
            else xVel = (velX) * toiX;
            if (debug) std::cout << "  toix " << toiX << "\n";
        }
        else if (isXseparate && isXtouching) //Resting against object
        {
            switch(xDiffState)
            {
                case 1: //Moving object is above stationarX object
                    if (velX < 0) xVel = velX; //Moving awaX (up)
                    if (debug) std::cout << "velX allowed (moving away left)\n";
                    break;
                case 2: //Moving object is below stationarX object
                    if (velX > 0) xVel = velX; //Moving awaX (down)
                    if (debug) std::cout << "velX allowed (moving away right)\n";
                    break;
                default:
                    if (debug) std::cout << "Default break\n";
            }
        }
    }
    //Overlapping; only allow movement that lessens the overlap(s)
    if (!isXseparate && !isYseparate) 
    {
        if (xDiffState==1)
        {
            //Moving in the negative X direction will resolve X overlap
            if (velX < 0) xVel = velX;
        }
        if (yDiffState==2)
        {
            //Moving in the positive X direction will resolve X overlap
            if (velX > 0) xVel = velX;
        }
        if (yDiffState==1)
        {
            //Moving in the negative Y direction will resolve Y overlap
            if (velY < 0) yVel = velY;
        }
        if (yDiffState==2)
        {
            //Moving in the positive Y direction will resolve Y overlap
            if (velY > 0) yVel = velY;
        }
    }
    if (debug) std::cout << "  xvel " << xVel << " yvel " << yVel << "\n";
    newVelX = xVel;
    newVelY = yVel;
    /*//Separate
    if (isXseparate && isYseparate && !isXtouching && !isYtouching) return 0;
    //Overlapping
    if (!isXseparate && !isYseparate && (!isXtouching || !isYtouching)) return -1;
    //Touching
    if (isXtouching || isYtouching) return 1;
    //Overlapping*/
    //return -1;
    return;
}
bool resolveCollision(aabb* a, aabb* b, vec2 *velocity)
{
	if (velocity->x==0 && velocity->y==0) return false;
	
	a->setPosition(a->x+velocity->x, a->y+velocity->y);
	if (!isColliding(a, b)) return false;
	else a->setPosition(a->x-velocity->x, a->y-velocity->y);
	/*velocity->x=0;
	velocity->y=0;
	return true;*/
	std::cout << "Given velocity " << velocity->x << " , " << velocity->y << "\n";
	
	//See  http://www.gamasutra.com/view/feature/131790/simple_intersection_tests_for_games.php?print=1
	float aMaxX=a->x+a->w;
    float aMaxY=a->y+a->h;
    float bMaxX=b->x+b->w;
    float bMaxY=b->y+b->h;
    ///float aMinX=a->x + velocity->x;
    ///float aMinY=a->y + velocity->y;
    std::cout << "Positions of each object: " << a->x << " , " << a->y << " and " << b->x << " , " << b->y <<"\n";
    /*//They aren't overlapping on the X axis if the statement is true
    if (aMaxX<b->x || a->x > bMaxX) return false;
    //They aren't overlapping on the Y axis if the statement is true
    if (aMaxY<b->y || a->y > bMaxY) return false;*/
    ///vec2 resolve; 
    ///resolve.x=0;
    ///resolve.y=0;
    
    /*if (a->x < b->x && velocity->x > 0) velocity->x=0; 
    else if (a->x > b->x && velocity->x < 0) velocity->x=0;
    
    if (a->y < b->y && velocity->y > 0) velocity->y=0; 
    else if (a->y > b->y && velocity->y < 0) velocity->y=0;*/
    
    /*//Just do a brute force minimum axis resolution
    if (aMinX + velocity->x < b->x) resolve.x=b->x - aMaxX;
    else resolve.x=bMaxX - aMinX;
    if (aMinY < b->y) resolve.y=b->y - aMaxY;
    else resolve.y=bMaxY - aMinY;
    //Make sure this axis is being moved on
    if (velocity->x==0) resolve.x=0;
    if (velocity->y==0) resolve.y=0;
    
    if (resolve.x < resolve.y) velocity->x+=resolve.x + 0.01;
    else if (resolve.x > resolve.y) velocity->y +=resolve.y + 0.01;
    else //Handle resolution eqaulity
    {
		velocity->x+=resolve.x + 0.01;
		velocity->y+=resolve.y + 0.01;
	}*/
    
	//if (a->x + velocity->x < b->x)
    /*//Get the x sweep distance
    std::cout << "X vals (amin amax bmin bmax): " << a->x << " , " << aMaxX << " " << b->x << " , " << bMaxX << "\n";
    std::cout << "Y vals (amin amax bmin bmax): " << a->y << " , " << aMaxY << " " << b->y << " , " << bMaxY << "\n";
    if (aMaxX<=b->x)
    {
		std::cout << "aMaxX is larger than bminx\n";
		resolve.x=aMaxX - b->x;
	}
    else if (bMaxX<a->x)
    {
		std::cout << "bMaxX is larger than aminx\n";
		resolve.x=a->x - bMaxX;
	}
    //Get the y sweep distance
    if (aMaxY<=b->y)
    {
		std::cout << "aMaxY is larger than bminy\n";
		resolve.y=aMaxY - b->y;
	}
    else if (bMaxY<a->y)
    {
		std::cout << "bMaxY is larger than aminy\n";
		resolve.y=a->y - bMaxY;
	}
    
    std::cout << "Just got sweep, values: " << resolve.x << " , " << resolve.y << "\n";
    
    //Make sure we don't divide by zero
    if (resolve.x !=0 && velocity->x !=0) resolve.x/=velocity->x;
    if (resolve.y !=0 && velocity->y !=0) resolve.y/=velocity->y;
    
    //Need to get the absolute value
    resolve.x=fabs(resolve.x);
    resolve.y=fabs(resolve.y);
    
    std::cout << "Just processed sweep values: " << resolve.x << " , " << resolve.y << "\n";
    
    if (resolve.x > resolve.y)
    {
		velocity->x/=resolve.x + 0.01;
		velocity->y/=resolve.x + 0.01;
	}
    else
    {
		velocity->x/=resolve.y + 0.01;
		velocity->y/=resolve.y + 0.01;
	}*/
    std::cout << "Final velocity " << velocity->x << " , " << velocity->y << "\n";
    //Instead of returning true we will resolve the collision (whichever axis has the least distance)
    float xResolve;
    float yResolve;
    if (aMaxX < bMaxX) xResolve=aMaxX - b->x;
    else xResolve=bMaxX-a->x;
    if (aMaxY < bMaxY) yResolve=aMaxY - b->y;
    else yResolve=bMaxY-a->y;
    std::cout << "Xresolve= "<< xResolve << "Yresolve= " << yResolve << " continue: ";
    //std::cin >> input;
    //Now use the axis with the least distance to resolve
    
    if (xResolve==yResolve)
    {
		std::cout << "Equal resolution axis: " << xResolve << " , " << yResolve << "\n";
		if (a->x - b->x < 0)a->x-=xResolve - 0.01;
		else a->x+=xResolve + 0.01;
		//if (a->y - b->y < 0)a->y-=yResolve;
		//else a->y+=yResolve;
	}
    else 
    {
		std::cout << "Not equal resolution axis: " << xResolve << " , " << yResolve << "\n";
	    if (xResolve<yResolve && yResolve !=0)
	    
			if (a->x - b->x < 0)a->x-=xResolve-0.01;
			else a->x+=xResolve+0.01;
		
		else
	    {
			std::cout << "Y is smaller\n";
			if (a->y - b->y < 0)a->y-=yResolve-0.01;
			else a->y+=yResolve+0.01;
		}
	}
    return true;
}
/////////////////////////////////////////
//Circle to Circle//////////////////////
/////////////////////////////////////////
circleBound::circleBound()
{
	x=y=r=0;
}
circleBound::circleBound(float newX, float newY, float newR)
{
    x=newX;
    y=newY;
    r=newR;
}
void circleBound::setPosition(float newX, float newY)
{
    x=newX;
    y=newY;
}
void circleBound::setRadius(float newRadius)
{
    r=newRadius;
}
bool isColliding(circleBound* a, circleBound* b)
{
    float vecX=a->x-b->x;
    float vecY=a->y-b->y;
    //Get the distance between the two (without square root)
    float distance=(vecX * vecX) + (vecY * vecY);
    //How far away each circle actually could be
    float tolerance=a->r+b->r;
    return distance <= tolerance*tolerance;
}
//////////////////////////////////////////////////
//SAT ///////////////////////////////////////////
//////////////////////////////////////////////////
struct  point
{
    float x;
    float y;
};

satBound::satBound()
{
    x=0;
    y=0;
    totalVertices=0;
}
satBound::satBound(int numbOfVertices)
{
    vertices.resize(numbOfVertices);
    normals.resize(numbOfVertices);
    totalVertices=numbOfVertices;
    x=0;
    y=0;
}
//Makes the std::vectors larger to accomodate more vertices/normals; returns false if
//the new size is less than the old size
bool satBound::resetSize(unsigned int newSize)
{
    if (newSize<=vertices.size())
    {
        return false;
    }
    else
    {
        vertices.resize(newSize);
        normals.resize(newSize);
    }
    return true;
}
void satBound::setVertex(float newX, float newY, int position)
{
    if (position<totalVertices)
    {
        vertices[position].x=newX;
        vertices[position].y=newY;
    }

}
//Generates a normal for each side of the bound
void satBound::generateNormals()
{
    float tempXa;
    float tempYa;
    float tempXb;
    float tempYb;
    float rise;
    float run;
    float distance;
    for (int i=0; i<totalVertices-1; i++)
    {
        //get the points to generate normals for
        tempXa=vertices[i].x;
        tempYa=vertices[i].y;
        tempXb=vertices[i+1].x;
        tempYb=vertices[i+1].y;
        //calculate the rise and run
        rise=tempYb-tempYa;
        run=tempXb-tempXa;
        //Now to the normals
        normals[i].x=-rise;
        normals[i].y=run;
        //normalize the normal
        distance=sqrt((normals[i].x*normals[i].x)+(normals[i].y*normals[i].y));
        normals[i].x=normals[i].x/distance;
        normals[i].y=normals[i].y/distance;
    }
    //Get the normal of the last and the first vertices
    int last=totalVertices-1;
    //get the points to generate normals for
    tempXa=vertices[last].x;
    tempYa=vertices[last].y;
    tempXb=vertices[0].x;
    tempYb=vertices[0].y;
    //calculate the rise and run
    rise=tempYb-tempYa;
    run=tempXb-tempXa;
    //Now to the normals
    normals[last].x=-rise;
    normals[last].y=run;
    //normalize the normal
    distance=sqrt((normals[last].x*normals[last].x)+(normals[last].y*normals[last].y));
    normals[last].x=normals[last].x/distance;
    normals[last].y=normals[last].y/distance;
}
//Sets the position to 0
void satBound::resetPosition()
{
    //Loop through each vertex and subtract the previous position
    for (int i=0; i<totalVertices; i++)
    {
        //std::cout << "trying to subtract position x from vertex x..." << std::endl;
        vertices[i].x-=x;
        vertices[i].y-=y;
    }

}
//Used when resolving object collisions; the object's coordinates are added to the bound
void satBound::setPosition(float newX, float newY)
{
    //Reset all of the coordinates back to zero
    //std::cout << "calling resetPosition()"<<std::endl;
    resetPosition();
    //std::cout << "ResetPosition() isnt the problem" << std::endl;
    for (int i=0; i<totalVertices; i++)
    {
        vertices[i].x+=newX;
        vertices[i].y+=newY;
    }
    x=newX;
    y=newY;
}

/*float manhattanTo(float ax, float ay, float bx, float by)
{
    return ((ax * bx) * (ax * bx)) + ((ay * by) * (ay * by)); 
}*/

//This will test the bounds against the X and Y axis only. Tunneling occurs slightly with objects 
//going more than 10 pixels a second (times the frame rate, something like 1000px per second).  Multisample 
//to the optimal case of around 5 px/sec if you really need such a fast object (it looks terrible anyways).
//tarPos is the position of the target object. If there is a collision tarPos will contain the new position to resolve the collision
bool  resolveAlignedCollision(satBound *tar, satBound *stat, point *tarPos, point *statPos)
{
    //The dot product of the current vertex
    float dp=0;
    //The axis we are testing
     point axis;
    //The minimum and maximum points of each object on the current axis
    float minA=5000;
    float maxA=0;
    float minB=5000;
    float maxB=0;
    //The minimum translation distance and the axis that distance is on
     point minAxis;
    float minRes=5000;
    //If there are two *different* axes tested that return an equal resolution distance (ie at a corner),
    //the collision will be resolved by combining the axes and getting the final resolution distance
     point equalAxis;
    float equalRes=5000;
    //The point we are testing and the final translation vector
    float x;
    float y;
    //The current axis minimum translation to resolve
    float tempDist=0;
    //The vector from center to center
     point center;
    //Now we loop through each axis and at each axis, each vertex of both objects
    //Grab the axis we are testing (the X-axis)
    axis.x=1;
    axis.y=0;
    //Test all of the targets points to get min and max
    for (int v=0; v < tar->totalVertices; v++)
    {
        //Make sure we are accessing a true vertex
        if (v < tar->totalVertices)
        {
            //Get the point we are testing
            x=tar->vertices[v].x;
            y=tar->vertices[v].y;
            //std::cout << "The point is " << x << " , " << y << std::endl;
            //Get the dot product of the current point in order to project the vertices onto a single line
            dp=(x * axis.x) + (y * axis .y);
            ///TESTING
            dp=fabs(dp);
            //Check to see of the dot product is the minimum or maximum extent of the object
            if (dp<minA)
            {
                minA=dp;
            }
            else if (dp > maxA)
            {
                maxA=dp;
            }
        }
    }//End of the loop for target points
    //Test all of the static points and get the minimum and maximum extents of the object
    for (int g=0; g < stat->totalVertices; g++)
    {
        ///Get rid of this if statement
        //Make sure we aren't out of range
        if (g < stat->totalVertices)
        {
            //Grab the coordinates for the current point we are testing
            x= stat->vertices[g].x;
            y= stat->vertices[g].y;
            //Grab the dot product of these points and the axis
            dp=(x * axis.x) + (y * axis.y);
            dp=fabs(dp); ///TESTING
            //See if this is the largest or smallest point of the object
            if (dp < minB)
            {
                minB=dp;
            }
            else if (dp > maxB)
            {
                maxB=dp;
            }
        }
    }//End of the loop for static points
        
    //Now we are going to grab how far the object needs to travel in order to resolve the collision
    minA=fabs(minA);
    maxA=fabs(maxA);
    minB=fabs(minB);	///TESTING (NOT SURE IF THIS WORKS!)
    maxB=fabs(maxB);
    if (minA<minB)
    {
        tempDist=minB-maxA;
    }
    else
    {
        tempDist=minA-maxB;
    }
    //If this value is greater than zero, there is no collision
    if (tempDist > 0)
    {
        //std::cout << tempDist << std::endl;
        return false;
    }	
    //Now we see if this axis has the smallest required translation.
    if (fabs(tempDist)<minRes)
    {
        //Set the axis this translation distance is on
        minAxis = axis;
        //Set the translation distance
        minRes=fabs(tempDist);
    }
    ///The new stuff (hopefully this will fix the corner issue)
    //If this is a different axis and the res is the same as the current minimum, we take note for future use
    if (fabs(tempDist)==minRes && (fabs(axis.x) != fabs(minAxis.x) || fabs(axis.y) != fabs(minAxis.y)))
    {
        equalAxis=axis;
        equalRes=fabs(tempDist);
    }	
    ///Will there be a problem if there are two equal resolutions, then a smaller one is found?
    //We are going to reset the mins and maxes just in case
    minA=minB=5000;
    maxA=maxB=0;
    ///Grab the axis we are testing (the Y-axis) _______________________________________________________
    axis.x=0;
    axis.y=1;
    //Test all of the targets points to get min and max
    for (int v=0; v < tar->totalVertices; v++)
    {
        //Get the point we are testing
        x=tar->vertices[v].x;
        y=tar->vertices[v].y;
        //std::cout << "The point is " << x << " , " << y << std::endl;
        //Get the dot product of the current point in order to project the vertices onto a single line
        dp=(x * axis.x) + (y * axis .y);
        ///TESTING
        dp=fabs(dp);
        //Check to see of the dot product is the minimum or maximum extent of the object
        if (dp<minA)
        {
            minA=dp;
        }
        else if (dp > maxA)
        {
            maxA=dp;
        }
    }//End of the loop for target points
    //Test all of the static points and get the minimum and maximum extents of the object
    for (int g=0; g < stat->totalVertices; g++)
    {
        //Grab the coordinates for the current point we are testing
        x= stat->vertices[g].x;
        y= stat->vertices[g].y;
        //Grab the dot product of these points and the axis
        dp=(x * axis.x) + (y * axis.y);
        dp=fabs(dp); ///TESTING
        //See if this is the largest or smallest point of the object
        if (dp < minB)
        {
            minB=dp;
        }
        else if (dp > maxB)
        {
            maxB=dp;
        }
    }//End of the loop for static points
    //Now we are going to grab how far the object needs to travel in order to resolve the collision
    minA=fabs(minA);
    maxA=fabs(maxA);
    minB=fabs(minB);	///TESTING (NOT SURE IF THIS WORKS!)
    maxB=fabs(maxB);
    if (minA<minB)
    {
        tempDist=minB-maxA;
    }
    else
    {
        tempDist=minA-maxB;
    }
    //If this value is greater than zero, there is no collision
    if (tempDist > 0)
    {
        //std::cout << tempDist << std::endl;
        return false;
    }	
    //Now we see if this axis has the smallest required translation.
    if (fabs(tempDist)<minRes)
    {
        //Set the axis this translation distance is on
        minAxis = axis;
        //Set the translation distance
        minRes=fabs(tempDist);
    }
    ///The new stuff (hopefully this will fix the corner issue)
    //If this is a different axis and the res is the same as the current minimum, we take note for future use
    if (fabs(tempDist)==minRes && (fabs(axis.x) != fabs(minAxis.x) || fabs(axis.y) != fabs(minAxis.y)))
    {
        equalAxis=axis;
        equalRes=fabs(tempDist);
    }	
    ///Will there be a problem if there are two equal resolutions, then a smaller one is found? Nope
    //We are going to reset the mins and maxes just in case
    minA=minB=5000;
    maxA=maxB=0;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///This here is the main difference.  Here we will take the two equal resolution axes, combine them, 
    ///project them, and send them on to the actual resolution code.
    //Check to see if there was an equal axis; if so, we will project on the combination of the axes, then find the 
    //minimum resolution, which will then be used to finally resolve the collision (note that it will replace the old
    //minimums)
    if (fabs(equalRes)==fabs(minRes)) //For sure there is an equal resolution on another axis; let's prepare that axis
    {
        //std::cout << "There is an equivalent resolution axis" << std::endl;
        //Good 'ol cut n' paste
        //Grab the axis we are testing (In this case, it's the sum of equal and minimum)
        axis.x=minAxis.x + equalAxis.x; ///NORMALIZE? (Please no)
        axis.y=minAxis.y + equalAxis.y;
        //Test all of the targets points to get min and max
        for (int v=0; v < tar->totalVertices; v++)
        {
            //Make sure we are accessing a true vertex
            if (v < tar->totalVertices)
            {
                //Get the point we are testing
                x=tar->vertices[v].x;
                y=tar->vertices[v].y;
                //std::cout << "The point is " << x << " , " << y << std::endl;
                //Get the dot product of the current point in order to project the vertices onto a single line
                dp=(x * axis.x) + (y * axis .y);
                ///TESTING
                dp=fabs(dp);
                //Check to see of the dot product is the minimum or maximum extent of the object
                if (dp<minA)
                {
                    minA=dp;
                }
                else if (dp > maxA)
                {
                    maxA=dp;
                }
            }
        }//End of the loop for target points
        //Test all of the static points and get the minimum and maximum extents of the object
        for (int g=0; g < stat->totalVertices; g++)
        {
            ///Get rid of this if statement
            //Make sure we aren't out of range
            if (g < stat->totalVertices)
            {
                //Grab the coordinates for the current point we are testing
                x= stat->vertices[g].x;
                y= stat->vertices[g].y;
                //Grab the dot product of these points and the axis
                dp=(x * axis.x) + (y * axis.y);
                dp=fabs(dp); ///TESTING
                //See if this is the largest or smallest point of the object
                if (dp < minB)
                {
                    minB=dp;
                }
                else if (dp > maxB)
                {
                    maxB=dp;
                }
            }
        }//End of the loop for static points
        
        //Now we are going to grab how far the object needs to travel in order to resolve the collision
        minA=fabs(minA);
        maxA=fabs(maxA);
        minB=fabs(minB);	///TESTING (NOT SURE IF THIS WORKS!)
        maxB=fabs(maxB);
        if (minA<minB)
        {
            tempDist=minB-maxA;
        }
        else
        {
            tempDist=minA-maxB;
        }
        ///Here's where the actual change comes in. 
        minRes=tempDist;
        minAxis.x=axis.x; 
        minAxis.y=axis.y;
    }
    ///Now we will actually resolve the collision
    //This is to make sure the translation is in the right direction
    center.x=tarPos->x - statPos->x;
    center.y=tarPos->y - statPos->y;
    if ((center.x*minAxis.x)+(center.y*minAxis.y)<0)
    {
        minAxis.x=-minAxis.x;
        minAxis.y=-minAxis.y;
    }
    //Add a one pixel offset (because the resolution so finely displaces it that drift makes it seem true on the
    //next frame
    minRes+=0.01;
    //Apparently there is, in fact, a collision.  We will now obtain the translation vector
    //We will use x and y just because they aren't being used now
    x=minAxis.x * minRes;
    y=minAxis.y * minRes;
    tarPos->x+=x;
    tarPos->y+=y;
    //Done!
    return true;
}
//This will resolve convex to convex shape collisions. It costs a lot more than aligned resolution because we
//have to test on all axes, but it is useful for pretty much anything.
bool  resolveComplexCollision(satBound *tar, satBound *stat, point *tarPos, point *statPos)
{
    //The dot product of the current vertex
    float dp=0;
    //The axis we are testing
    point axis;
    //The minimum and maximum points of each object on the current axis
    float minA=5000;
    float maxA=0;
    float minB=5000;
    float maxB=0;
    //The minimum translation distance and the axis that distance is on
    point minAxis;
		///NOTE: added these in without testing in order to get rid of the warning
    minAxis.x=0;
    minAxis.y=0;
    float minRes=5000;
    //If there are two *different* axes tested that return an equal resolution distance (ie at a corner),
    //the collision will be resolved by combining the axes and getting the final resolution distance
    point equalAxis;
		///NOTE: added these in without testing in order to get rid of the warning
    equalAxis.x=0;
    equalAxis.y=0;
    float equalRes=5000;
    //The point we are testing and the final translation vector
    float x;
    float y;
    //The current axis minimum translation to resolve
    float tempDist=0;
    //The vector from center to center
    point center;
    ///Eventually code to also test static axes (for "complex" non AABB version)
    //Now we loop through each axis and at each axis, each vertex of both objects
    for (int i=0;  i < tar->totalVertices; i++)
    {
        //Grab the axis we are testing
        axis.x=tar->normals[i].x; 
        axis.y=tar->normals[i].y;
        //Test all of the targets points to get min and max
        for (int v=0; v < tar->totalVertices; v++)
        {
            //Make sure we are accessing a true vertex
            if (v < tar->totalVertices)
            {
                //Get the point we are testing
                x=tar->vertices[v].x;
                y=tar->vertices[v].y;
                //std::cout << "The point is " << x << " , " << y << std::endl;
                //Get the dot product of the current point in order to project the vertices onto a single line
                dp=(x * axis.x) + (y * axis .y);
                ///TESTING
                dp=fabs(dp);
                //Check to see of the dot product is the minimum or maximum extent of the object
                if (dp<minA)
                {
                    minA=dp;
                }
                else if (dp > maxA)
                {
                    maxA=dp;
                }
            }
        }//End of the loop for target points
        //Test all of the static points and get the minimum and maximum extents of the object
        for (int g=0; g < stat->totalVertices; g++)
        {
            ///Get rid of this if statement
            //Make sure we aren't out of range
            if (g < stat->totalVertices)
            {
                //Grab the coordinates for the current point we are testing
                x= stat->vertices[g].x;
                y= stat->vertices[g].y;
                //Grab the dot product of these points and the axis
                dp=(x * axis.x) + (y * axis.y);
                dp=fabs(dp); ///TESTING
                //See if this is the largest or smallest point of the object
                if (dp < minB)
                {
                    minB=dp;
                }
                else if (dp > maxB)
                {
                    maxB=dp;
                }
            }
        }//End of the loop for static points
         
        //Now we are going to grab how far the object needs to travel in order to resolve the collision
        minA=fabs(minA);
        maxA=fabs(maxA);
        minB=fabs(minB);    ///TESTING (NOT SURE IF THIS WORKS!)
        maxB=fabs(maxB);
        if (minA<minB)
        {
            tempDist=minB-maxA;
        }
        else
        {
            tempDist=minA-maxB;
        }
        //If this value is greater than zero, there is no collision
        if (tempDist > 0)
        {
            //std::cout << tempDist << std::endl;
            return false;
        }     
        //Now we see if this axis has the smallest required translation.
        if (fabs(tempDist)<minRes)
        {
            //Set the axis this translation distance is on
            minAxis = axis;
            //Set the translation distance
            minRes=fabs(tempDist);
        }
        ///The new stuff (hopefully this will fix the corner issue)
        //If this is a different axis and the res is the same as the current minimum, we take note for future use
        if (fabs(tempDist)==minRes && (fabs(axis.x) != fabs(minAxis.x) || fabs(axis.y) != fabs(minAxis.y)))
        {
            equalAxis=axis;
            equalRes=fabs(tempDist);
        }     
        ///Will there be a problem if there are two equal resolutions, then a smaller one is found?
        //We are going to reset the mins and maxes just in case
        minA=minB=5000;
        maxA=maxB=0;
    }//End of the loop for target axes
    ///This here is the main difference.  Here we will take the two equal resolution axes, combine them, 
    ///project them, and send them on to the actual resolution code.
    //Check to see if there was an equal axis; if so, we will project on the combination of the axes, then find the 
    //minimum resolution, which will then be used to finally resolve the collision (note that it will replace the old
    //minimums)
    if (fabs(equalRes)==fabs(minRes)) //For sure there is an equal resolution on another axis; let's prepare that axis
    {
        //Good 'ol cut n' paste
        //Grab the axis we are testing (In this case, it's the sum of equal and minimum)
        axis.x=minAxis.x + equalAxis.x; ///NORMALIZE? (Please no)
        axis.y=minAxis.y + equalAxis.y;
        //Test all of the targets points to get min and max
        for (int v=0; v < tar->totalVertices; v++)
        {
            //Make sure we are accessing a true vertex
            if (v < tar->totalVertices)
            {
                //Get the point we are testing
                x=tar->vertices[v].x;
                y=tar->vertices[v].y;
                //std::cout << "The point is " << x << " , " << y << std::endl;
                //Get the dot product of the current point in order to project the vertices onto a single line
                dp=(x * axis.x) + (y * axis .y);
                ///TESTING
                dp=fabs(dp);
                //Check to see of the dot product is the minimum or maximum extent of the object
                if (dp<minA)
                {
                    minA=dp;
                }
                else if (dp > maxA)
                {
                    maxA=dp;
                }
            }
        }//End of the loop for target points
        //Test all of the static points and get the minimum and maximum extents of the object
        for (int g=0; g < stat->totalVertices; g++)
        {
            ///Get rid of this if statement
            //Make sure we aren't out of range
            if (g < stat->totalVertices)
            {
                //Grab the coordinates for the current point we are testing
                x= stat->vertices[g].x;
                y= stat->vertices[g].y;
                //Grab the dot product of these points and the axis
                dp=(x * axis.x) + (y * axis.y);
                dp=fabs(dp); ///TESTING
                //See if this is the largest or smallest point of the object
                if (dp < minB)
                {
                    minB=dp;
                }
                else if (dp > maxB)
                {
                    maxB=dp;
                }
            }
        }//End of the loop for static points
         
        //Now we are going to grab how far the object needs to travel in order to resolve the collision
        minA=fabs(minA);
        maxA=fabs(maxA);
        minB=fabs(minB);    ///TESTING (NOT SURE IF THIS WORKS!)
        maxB=fabs(maxB);
        if (minA<minB)
        {
            tempDist=minB-maxA;
        }
        else
        {
            tempDist=minA-maxB;
        }
        ///Here's where the actual change comes in. 
        minRes=tempDist;
        minAxis.x=axis.x; 
        minAxis.y=axis.y;
    }
    ///Time to test the staticObj's sides------------------------------------------------------------------------
    //Now we loop through each axis and at each axis, each vertex of both objects
    for (int i=0;  i < stat->totalVertices; i++)
    {
        //Grab the axis we are testing
        axis.x=stat->normals[i].x; 
        axis.y=stat->normals[i].y;
        //Test all of the targets points to get min and max
        for (int v=0; v < tar->totalVertices; v++)
        {
              //Get the point we are testing
              x=tar->vertices[v].x;
              y=tar->vertices[v].y;
              //std::cout << "The point is " << x << " , " << y << std::endl;
               //Get the dot product of the current point in order to project the vertices onto a single line
               dp=(x * axis.x) + (y * axis .y);
               ///TESTING
               dp=fabs(dp);
               //Check to see of the dot product is the minimum or maximum extent of the object
               if (dp<minA)
               {
                   minA=dp;
               }
               else if (dp > maxA)
               {
                   maxA=dp;
               }
        }//End of the loop for target points
        //Test all of the static points and get the minimum and maximum extents of the object
        for (int g=0; g < stat->totalVertices; g++)
        {
            ///Get rid of this if statement
            //Make sure we aren't out of range
            if (g < stat->totalVertices)
            {
                //Grab the coordinates for the current point we are testing
                x= stat->vertices[g].x;
                y= stat->vertices[g].y;
                //Grab the dot product of these points and the axis
                dp=(x * axis.x) + (y * axis.y);
                dp=fabs(dp); ///TESTING
                //See if this is the largest or smallest point of the object
                if (dp < minB)
                {
                    minB=dp;
                }
                else if (dp > maxB)
                {
                    maxB=dp;
                }
            }
        }//End of the loop for static points
         
        //Now we are going to grab how far the object needs to travel in order to resolve the collision
        minA=fabs(minA);
        maxA=fabs(maxA);
        minB=fabs(minB);    ///TESTING (NOT SURE IF THIS WORKS!)
        maxB=fabs(maxB);
        if (minA<minB)
        {
            tempDist=minB-maxA;
        }
        else
        {
            tempDist=minA-maxB;
        }
        //If this value is greater than zero, there is no collision
        if (tempDist > 0)
        {
            //std::cout << tempDist << std::endl;
            return false;
        }     
        //Now we see if this axis has the smallest required translation.
        if (fabs(tempDist)<minRes)
        {
            //Set the axis this translation distance is on
            minAxis = axis;
            //Set the translation distance
            minRes=fabs(tempDist);
        }
        ///The new stuff (hopefully this will fix the corner issue)
        //If this is a different axis and the res is the same as the current minimum, we take note for future use
        if (fabs(tempDist)==minRes && (fabs(axis.x) != fabs(minAxis.x) || fabs(axis.y) != fabs(minAxis.y)))
        {
            equalAxis=axis;
            equalRes=fabs(tempDist);
        }     
        //We are going to reset the mins and maxes just in case
        minA=minB=5000;
        maxA=maxB=0;
    }//End of the loop for static axes
    
    
    
    ///This here is the main difference.  Here we will take the two equal resolution axes, combine them, 
    ///project them, and send them on to the actual resolution code.
    //Check to see if there was an equal axis; if so, we will project on the combination of the axes, then find the 
    //minimum resolution, which will then be used to finally resolve the collision (note that it will replace the old
    //minimums)
    if (fabs(equalRes)==fabs(minRes)) //For sure there is an equal resolution on another axis; let's prepare that axis
    {
        //std::cout << "There is an equivalent resolution axis" << std::endl;
        //Good 'ol cut n' paste
        //Grab the axis we are testing (In this case, it's the sum of equal and minimum)
        axis.x=minAxis.x + equalAxis.x; ///NORMALIZE? (Please no)
        axis.y=minAxis.y + equalAxis.y;
        //Test all of the targets points to get min and max
        for (int v=0; v < tar->totalVertices; v++)
        {
            //Make sure we are accessing a true vertex
            if (v < tar->totalVertices)
            {
                //Get the point we are testing
                x=tar->vertices[v].x;
                y=tar->vertices[v].y;
                //std::cout << "The point is " << x << " , " << y << std::endl;
                //Get the dot product of the current point in order to project the vertices onto a single line
                dp=(x * axis.x) + (y * axis .y);
                ///TESTING
                dp=fabs(dp);
                //Check to see of the dot product is the minimum or maximum extent of the object
                if (dp<minA)
                {
                    minA=dp;
                }
                else if (dp > maxA)
                {
                    maxA=dp;
                }
            }
        }//End of the loop for target points
        //Test all of the static points and get the minimum and maximum extents of the object
        for (int g=0; g < stat->totalVertices; g++)
        {
            ///Get rid of this if statement
            //Make sure we aren't out of range
            if (g < stat->totalVertices)
            {
                //Grab the coordinates for the current point we are testing
                x= stat->vertices[g].x;
                y= stat->vertices[g].y;
                //Grab the dot product of these points and the axis
                dp=(x * axis.x) + (y * axis.y);
                dp=fabs(dp); ///TESTING
                //See if this is the largest or smallest point of the object
                if (dp < minB)
                {
                    minB=dp;
                }
                else if (dp > maxB)
                {
                    maxB=dp;
                }
            }
        }//End of the loop for static points
         
        //Now we are going to grab how far the object needs to travel in order to resolve the collision
        minA=fabs(minA);
        maxA=fabs(maxA);
        minB=fabs(minB);    ///TESTING (NOT SURE IF THIS WORKS!)
        maxB=fabs(maxB);
        if (minA<minB)
        {
            tempDist=minB-maxA;
        }
        else
        {
            tempDist=minA-maxB;
        }
        ///Here's where the actual change comes in. 
        minRes=tempDist;
        minAxis.x=axis.x; 
        minAxis.y=axis.y;
    }
    ///Now we will actually resolve the collision
    //This is to make sure the translation is in the right direction
    center.x=tarPos->x - statPos->x;
    center.y=tarPos->y - statPos->y;
    if ((center.x*minAxis.x)+(center.y*minAxis.y)<0)
    {
        minAxis.x=-minAxis.x;
        minAxis.y=-minAxis.y;
    }
    //Add a one pixel offset (because the resolution so finely displaces it that drift makes it seem true on the
    //next frame
    minRes+=0.01;
    //Apparently there is, in fact, a collision.  We will now obtain the translation vector
    //We will use x and y just because they aren't being used now
    //std::cout << "==MinAxis - minRes: " << minAxis.x << " , " << minAxis.y << ">>" << minRes << std::endl;
    x=minAxis.x * minRes;
    y=minAxis.y * minRes;
    tarPos->x+=x;
    tarPos->y+=y;
    //Done!
    return true;
}

#endif
