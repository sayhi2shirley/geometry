#include "Geometry.h"
#include "math.h"
#include "sstream"

// ============ Shape class =================

Shape::Shape(int d) : xMinCoord_(0), yMinCoord_(0),
    xMaxCoord_(0), yMaxCoord_(0),
    area_(0), shapeDim_(0) {

        bool ret = setDepth(d);
        if (ret == false) {
            throw invalid_argument("Negative depth is not accepted.");
        }
}

bool Shape::setDepth(int d) {

    if (d >= 0) {
        shapeDepth_ = d;
        return true;
    }
    else {
        return false;
    }
}

int Shape::getDepth() const {
    return shapeDepth_;
}

int Shape::dim() const {
    return shapeDim_;
}

float Shape::getX() const {
    return xMinCoord_;
}

float Shape::getY() const {
    return yMinCoord_;
}

float Shape::getXmin() const {
    return xMinCoord_;
}

float Shape::getYmin() const {
    return yMinCoord_;
}

float Shape::getXmax() const {
    return xMaxCoord_;
}

float Shape::getYmax() const {
    return yMaxCoord_;
}

void Shape::translate(float x, float y) {
    xMinCoord_ += x;
    xMaxCoord_ += x;
    yMinCoord_ += y;
    yMaxCoord_ += y;
    return;
}
float Shape::area() const {
    return area_;
}

void Shape::rotate() {
    return;
}

void Shape::scale(float f) {
    if (f <= 0) {
        throw invalid_argument("zero or negative factor."); 
    }
    return;
}

// =============== Point class ================
Point::Point(float x, float y, int d) : Shape(d) {
    xMinCoord_ = x;
    yMinCoord_ = y;
    shapeDim_ = 0;
}

bool Point::contains(const Point& p) const {
    if (p.xMinCoord_ == getX() && p.yMinCoord_ == getY()) {
        // If both X & Y Co-ordinates are same as Point
        return true;
    } else {
        return false;
    }
}

// =========== LineSegment class ==============

LineSegment::LineSegment(const Point& p, const Point& q) : Shape(p.getDepth()) {

    shapeDim_ = oneDimension;

    if (p.getY() == q.getY()) {
        /* Horizontal Line */
        if (p.getX() < q.getX()) {
            xMinCoord_ = p.getX();
            xMaxCoord_ = q.getX();
        } else {
            xMinCoord_ = q.getX();
            xMaxCoord_ = p.getX();
        }

        lineSegLength_ = xMaxCoord_ - xMinCoord_;
        yMinCoord_ = yMaxCoord_ = p.getY();

    } else if (p.getX() == q.getX()) {
        /* Vertical Line */
        if (p.getY() < q.getY()) {
            yMinCoord_ = p.getY();
            yMaxCoord_ = q.getY();
        } else {
            yMinCoord_ = q.getY();
            yMaxCoord_ = p.getY();
        }

        lineSegLength_ = yMaxCoord_ - yMinCoord_;
        xMinCoord_ = xMaxCoord_ = p.getX();

    } else {
        /* Neither Horizontal or Vertical Line */
        throw invalid_argument("Line is not orthogonal");
    }

    if (p.getDepth() != q.getDepth()) {
        throw invalid_argument("Both depths are not same.");
    }

    if (p.contains(q)) {
        throw invalid_argument("Two endpoints are same."); 
    }
}

float LineSegment::length() const {
    return lineSegLength_;
}

void LineSegment::rotate() {

    if (getYmin() == getYmax()) {
        /* Horizontal Line */
        xMinCoord_ += lineSegLength_/2;
        xMaxCoord_ = xMinCoord_;
        yMaxCoord_ += lineSegLength_/2;
        yMinCoord_ -= lineSegLength_/2;
    } else if (getXmin() == getXmax()) {
        /* Vertical Line */
        yMinCoord_ += lineSegLength_/2;
        yMaxCoord_ = yMinCoord_;
        xMaxCoord_ += lineSegLength_/2;
        xMinCoord_ -= lineSegLength_/2;
    }
    return;
}

bool LineSegment::contains(const Point& p) const {

    if (getYmin()== p.getY() && getYmax() == p.getY()) {
        /* Horizontal Line */
        if (getXmin() <= p.getX() && getXmax() >= p.getX()) {
            return true;
        }
        return false;
    } else if (getXmin() == p.getX() && getXmax() == p.getX()) {
        /* Vertical Line */
        if (getYmin() <= p.getY() && getYmax() >= p.getY()) {
            return true;
        }
        return false;
    } else {
        /* Neither Horizontal nor Veritcal Line */
        return false;
    }
}

void LineSegment::scale(float f) {

    if (f <= 0) {
        throw invalid_argument("zero or negative factor.");
    }

    if (getYmin() == getYmax()) {
        /* Horizontal Line */
        int middle = xMinCoord_ + (lineSegLength_/2);
        xMinCoord_ = middle - (lineSegLength_/2) * f;
        xMaxCoord_ = middle + (lineSegLength_/2) * f;
    } else if (getXmin() == getXmax()) {
        /* Vertical Line */
        int middle = yMinCoord_ + (lineSegLength_/2);
        yMinCoord_ = middle - (lineSegLength_/2) * f;
        yMaxCoord_ = middle + (lineSegLength_/2) * f;
    }
    return;
}

// ============ TwoDShape class ================

TwoDShape::TwoDShape(int d) : Shape(d) {
    shapeDim_ = twoDimension;
}

// ============== Rectangle class ================

Rectangle::Rectangle(const Point& p, const Point& q) : TwoDShape(p.getDepth()) {

    if (p.getX() == q.getX()) {
        throw invalid_argument("Two points of X-coord are same.");
    }
    if (p.getY() == q.getY()) {
        throw invalid_argument("Two points of Y-coord are same.");
    }
    xMinCoord_ = (p.getX() < q.getX()) ? p.getX() : q.getX();
    xMaxCoord_ = (p.getX() > q.getX()) ? p.getX() : q.getX();
    yMinCoord_ = (p.getY() < q.getY()) ? p.getY() : q.getY();
    yMaxCoord_ = (p.getY() > q.getY()) ? p.getY() : q.getY();
    area_ = (xMaxCoord_ - xMinCoord_) * (yMaxCoord_ - yMinCoord_);

    if (p.getDepth() != q.getDepth()) {
        throw invalid_argument("Both depths are not same.");
    }
}

bool Rectangle::contains(const Point& p) const {

    if (getXmin() <= p.getX() && getXmax() >= p.getX() &&
            getYmin() <= p.getY() && getYmax() >= p.getY()) {
        return true;
    }

    return false;
}

void Rectangle::rotate() {

    int xDiff = (xMaxCoord_ - xMinCoord_)/2;
    int xCenter = xMinCoord_ + xDiff;
    int yDiff = (yMaxCoord_ - yMinCoord_)/2;
    int yCenter = yMinCoord_ + yDiff;

    xMinCoord_ = xCenter - yDiff;
    xMaxCoord_ = xCenter + yDiff;
    yMaxCoord_ = yCenter + xDiff;
    yMinCoord_ = yCenter - xDiff;
    return;
}

void Rectangle::scale(float f) {

    if (f <= 0) {
        throw invalid_argument("zero or negative factor.");
    }

    int xDiff = (xMaxCoord_ - xMinCoord_)/2;
    int xCenter = xMinCoord_ + xDiff;
    int yDiff = (yMaxCoord_ - yMinCoord_)/2;
    int yCenter = yMinCoord_ + yDiff;

    xMinCoord_ = xCenter - (xDiff * f);
    xMaxCoord_ = xCenter + (xDiff * f);
    yMinCoord_ = yCenter - (yDiff * f);
    yMaxCoord_ = yCenter + (yDiff * f);
    return;
}

// ================== Circle class ===================

Circle::Circle(const Point& c, float r) : TwoDShape(c.getDepth()) {
    if (r <= 0) {
        throw invalid_argument("Circle of zero or negative radius.");
    }
    xMinCoord_ = c.getX();
    yMinCoord_ = c.getY();
    radCircle_ = r;
    area_ = PI * pow(r, 2);
}

float Circle::getR() const {
    return radCircle_;
}

bool Circle::contains(const Point& p) const {

    float xDiff = pow((getX() - p.getX()), 2);
    float yDiff = pow((getY() - p.getY()), 2);
    float rootAns = sqrt(xDiff + yDiff);

    if (getR() > rootAns) {
        return true;
    } else {
        return false;
    }
}

void Circle::scale(float f) {

    if (f <= 0) {
        throw invalid_argument("zero or negative factor.");
    }
    radCircle_ = radCircle_ * f;
    return;
}

// ================= Scene class ===================

Scene::Scene() : sceneDepth_(0) {
    string blankline(WIDTH, ' ');
    for(int index = 0; index < HEIGHT; index++) {
        scenePage_+= blankline + "\n";
    }
    /* TODO: Initialise the shared_ptr */
}

void Scene::addObject(shared_ptr<Shape> ptr) {
    this->shapePtr_.push_back(shared_ptr<Shape> (ptr));
    return;
}

void Scene::setDrawDepth(int depth) {
    sceneDepth_ = depth;
    return;
}

ostream& operator<<(ostream& out, const Scene& s) {

    /* Pointer will not point anything initially. */
    if (s.shapePtr_.empty()) {
        out << s.scenePage_;
        return out;
    }
    string buffer(""), eachLine("");
    stringstream blankline(s.scenePage_);

    for (auto vectIter = s.shapePtr_.begin();
            vectIter != s.shapePtr_.end(); vectIter++) {
        /* Clear the eachLine after each iteration. */
        eachLine = "";
        /* Line will be incremented upto s.HEIGHT */
        int line = 0;

        /* Clear the flag to reach the beginning of the stringstream. */
        blankline.clear();
        blankline.seekg(0);

        /* When the drawing depth is less, store the streams into buffer */
        if (s.sceneDepth_ != 0 && s.sceneDepth_ < (*vectIter)->getDepth()) {
            buffer = blankline.str();
            break; 
        }
        /*
         * Buffer will be cleared after storing into stringstream(blankline)
         * in each iteration.
         */
        buffer = "";

        switch ((*vectIter)->dim())
        {
            case 0:
                {
                    while (getline(blankline, eachLine, '\n')) {
                        line++;
                        if (line != ((s.HEIGHT) - (*vectIter)->getYmin())) {
                            buffer += eachLine + '\n';
                            continue;
                        }
                        for(int index = 0; index < s.WIDTH; index++) {
                            if (eachLine[(*vectIter)->getXmin()] == ' ') {
                                eachLine[(*vectIter)->getXmin()] = '*';
                                buffer += eachLine + '\n';
                            }
                        }
                    }
                }
                break;
            case 1:
                {
                    while (getline(blankline, eachLine, '\n')) {
                        line++;
                        if ((*vectIter)->getXmin() == (*vectIter)->getXmax()) {
                            //Vertical line
                            if (line > ((s.HEIGHT) - (*vectIter)->getYmin()) &&
                                    line < ((s.HEIGHT) - (*vectIter)->getYmax())) {
                                buffer += eachLine + '\n';
                                continue;
                            }
                            for(int index = 0; index < s.WIDTH; index++) {
                                if (eachLine[(*vectIter)->getXmin()] == ' ') {
                                    eachLine[(*vectIter)->getXmin()] = '*';
                                }
                            }
                            buffer += eachLine + '\n';
                        } else if ((*vectIter)->getYmin() == (*vectIter)->getYmax()) {
                            //Horizontal line
                            if (line != ((s.HEIGHT) - (*vectIter)->getYmin())) {
                                buffer += eachLine + '\n';
                                continue;
                            }
                            for(int index = (*vectIter)->getXmin();
                                    index <= (*vectIter)->getXmax(); index++) {
                                if (eachLine[index] == ' ') {
                                    eachLine[index] = '*';
                                }
                            }
                            buffer += eachLine + '\n';
                        }  // else if
                    } // while
                } // case 1
                break;
            case 2:
                {
                    if ((*vectIter)->getYmax() != 0 && (*vectIter)->getXmax() != 0) {
                        // Rectangle Class
                        while (getline(blankline, eachLine, '\n')) {
                            line++;
                            if (line > ((s.HEIGHT) - (*vectIter)->getYmin())) {
                                buffer += eachLine + '\n';
                                continue;
                            }
                            for(int index = (*vectIter)->getXmin();
                                    index <= (*vectIter)->getXmax(); index++) {
                                if (eachLine[index] == ' ') {
                                    eachLine[index] = '*';
                                }
                            }
                            buffer += eachLine + '\n';
                        } // while
                    } else if ((*vectIter)->getYmax() == 0 && (*vectIter)->getXmax() == 0) {
                        float radius = sqrt((*vectIter)->area()/(*vectIter)->PI);
                        int plane = s.HEIGHT;

                        while (getline(blankline, eachLine, '\n')) {
                            line++;
                            plane--;
                            if ((line < (s.HEIGHT - (*vectIter)->getYmin() - radius)) ||
                                    (line > (s.HEIGHT - (*vectIter)->getYmin() + radius))) {
                                buffer += eachLine + '\n';
                                continue;
                            }

                            float yDiff = pow(abs(plane - (*vectIter)->getYmin()), 2);
                            for (float index = (*vectIter)->getXmin() - radius;
                                    index <= (*vectIter)->getXmin() + radius; index++) {

                                float xDiff = pow((abs((*vectIter)->getXmin() - index)), 2);
                                float rootAns = sqrt(xDiff + yDiff);

                                if (radius >= rootAns) {
                                    if (eachLine[index] = ' ') {
                                        eachLine[index] = '*';
                                    }
                                }
                            }
                            buffer += eachLine + '\n';
                        } // while
                    } // else if
                } // case 2
                break;
        } // Switch Case
        /* All the collection of pointers should keep adding on the same stringstream */
        blankline.str(buffer);
    } // for loop
    out << buffer;
    return out;
}
