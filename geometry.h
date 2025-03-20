#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class Point; // forward declaration

class Shape {
    public:
        // Default constructor, just to make this release version compilable.
        // If your implementation is correct this should be removed
        Shape() = delete; 

        // Constructor specifying the depth of the object.
        // If d is negative, throw a std::invalid_argument exception.
        Shape(int d);

        // Set depth of object to d. If d is negative, return false and
        // do not update depth. Otherwise return true
        bool setDepth(int d);

        // Return the depth of object
        int getDepth() const;

        // Return the dimension of the object (0, 1 or 2)
        int dim() const;

        // Return the X co-ordinate of Point
        virtual float getX() const final;

        // Return the Y co-ordinate of Point
        virtual float getY() const final;

        // Return the X co-ordinate of left end point
        float getXmin() const;

        // Return the Y co-ordinate of left end point
        float getYmin() const;

        // Return the X co-ordinate of right end point
        float getXmax() const;

        // Return the Y co-ordinate of right end point
        float getYmax() const;

        // Return the area of the object
        float area() const;

        // Translate the object horizontally by x and vertically by y
        virtual void translate(float x, float y) final;

        // Rotate the object 90 degrees around its centre
        virtual void rotate();

        // Scale the object by a factor f relative to its centre.
        // If f is zero or negative, throw a std::invalid-argument exception.
        virtual void scale(float f);

        // Return true if the object contains p and false otherwise.
        // Depths are ignored for purpose of comparison
        virtual bool contains(const Point& p) const = 0;

        // the constant pi
        static constexpr double PI = 3.1415926;
        // The constant value for 1D & 2D
        static constexpr char oneDimension = 1;
        static constexpr char twoDimension = 2;

    protected:
        /* shapeDim_ variable can't be initialised with sub class's constructor's
         * initialiser list as it is not the sub-class member rather base.
         * But, this should be intialised when the sub-class's constructor
         * is created, so specifying it as protected seems reasonable than
         * public. Because public destroys the encapsulation. If it is declared
         * private, sub-class can't access it.
         */
        int shapeDim_;
        float xMinCoord_;
        float yMinCoord_;
        float xMaxCoord_;
        float yMaxCoord_;
        float area_;

    private:
        int shapeDepth_;
};

class Point : public Shape {

    public:
        // Point Constructor
        Point(float x, float y, int d = 0);

        // Return true if point contains p and false otherwise.
        // Depths are ignored for purpose of comparison
        bool contains(const Point&) const override;
};

class LineSegment : public Shape {

    public:
        // Constructor.
        // If the two points have different depths, or have the same x- and
        // y-coordinate, or if the line is not axis-aligned, throw a
        // std::invalid_argument exception
        LineSegment(const Point& p, const Point& q);

        // Return the length of the line segment
        float length() const;

        // Return true if line segment contains p and false otherwise.
        // Depths are ignored for purpose of comparison
        bool contains(const Point& p) const override;

        // Scale the object by a factor f relative to its centre.
        // If f is zero or negative, throw a std::invalid-argument exception.
        void scale(float f);

        // Rotate the object 90 degrees around its centre
        void rotate(); 

    private:
        // Length of the line segment
        float lineSegLength_;
};

class TwoDShape : public Shape {

    public:
        // Default constructor.
        // Similar comment to Student default constructor applies
        TwoDShape() = delete;

        // Constructor specifying the depth d
        TwoDShape(int d);

        // Scale the Two Dimensional object by a factor f relative to its centre.
        // If f is zero or negative, throw a std::invalid-argument exception.
        virtual void scale(float f) = 0;
       
};

class Rectangle : public TwoDShape {

    public:
        // Constructor.
        // If the two points have different depths, or have the same x-
        // and/or y-coordinate, throw a std::invalid_argument exception
        Rectangle(const Point& p, const Point& q);

        // Return true if rectangle contains p and false otherwise.
        // Depths are ignored for purpose of comparison
        bool contains(const Point& p) const override;

        // Rotate the object 90 degrees around its centre
        void rotate();

        // Scale the Rectangle by a factor f relative to its centre.
        // If f is zero or negative, throw a std::invalid-argument exception.
        void scale(float f) override;

};

class Circle : public TwoDShape {

    public:
        // Constructor.
        // If r is zero or negative, throw a std::invalid-argument exception.
        Circle(const Point& c, float r);

        // Returns the radius
        float getR() const;

        // Return true if circle contains p and false otherwise.
        // Depths are ignored for purpose of comparison
        bool contains(const Point& p) const override;

        // Scale the Circle by a factor f relative to its centre.
        // If f is zero or negative, throw a std::invalid-argument exception.
        void scale(float f) override;

    private:
        // Circle's Radius
        float radCircle_;
};


class Scene {

    public:
        // Constructor
        Scene();

        // Add the pointer to the collection of pointers stored
        void addObject(std::shared_ptr<Shape> ptr);

        // Set the drawing depth to d
        void setDrawDepth(int d);

        // Constants specifying the size of the drawing area
        static constexpr int WIDTH = 60;
        static constexpr int HEIGHT = 20;

    private:
        // Empty page of WIDTH * HEIGHT
        string scenePage_;
        // Depth of the drawing
        int sceneDepth_;
        // Collection of Shape pointers
        vector<std::shared_ptr<Shape>> shapePtr_;

        // Draw objects as specified in the assignment page
        friend std::ostream& operator<<(std::ostream& out, const Scene& s);
};

#endif /* GEOMETRY_H_ */
