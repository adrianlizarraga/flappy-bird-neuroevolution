#include "Renderable.h"

// -----------------------------------------------------
sf::FloatRect combineBoundingBoxes(const sf::FloatRect& bbox1, const sf::FloatRect& bbox2) {
    float xlt1 = bbox1.left;
    float xlt2 = bbox2.left;
    float ylt1 = bbox1.top;
    float ylt2 = bbox2.top;
    float xrb1 = bbox1.left + bbox1.width;
    float xrb2 = bbox2.left + bbox2.width;
    float yrb1 = bbox1.top + bbox1.height;
    float yrb2 = bbox2.top + bbox2.height;

    float xlt = xlt1 < xlt2 ? xlt1 : xlt2;
    float ylt = ylt1 < ylt2 ? ylt1 : ylt2;
    float xrb = xrb1 > xrb2 ? xrb1 : xrb2;
    float yrb = yrb1 > yrb2 ? yrb1 : yrb2;

    return sf::FloatRect(xlt, ylt, xrb - xlt, yrb - ylt);
}

// -------------------------------------
Renderable::~Renderable() {}

// -------------------------------------
bool Renderable::intersects(const Renderable& other) const {
    return this->boundingBox().intersects(other.boundingBox());
}