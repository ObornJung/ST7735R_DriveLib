/**
 Filename:       JMERemoter.h
 Revised:        $Date: 2015-06-18$
 Revision:       $Revision: 01 $
 Author:         Oborn.Jung

 Description:    This file contains the common functions of geometry.

 Copyright 2015 JONMA Inc. All rights reserved.
 */
#ifndef __H__JMEGeometry__H__
#define __H__JMEGeometry__H__

#include "JMEBase.h"
#include "JMEMath.h"

typedef uint8_t JMEGeometryUnit;
#define JMEGeometryUnitEqual(a, b)       ((a) == (b))

/**
 *  Points.
 */
typedef struct JMEPoint {
    JMEGeometryUnit x;
    JMEGeometryUnit y;
} JMEPoint;

/**
 *  Sizes.
 */
typedef struct JMESize {
    JMEGeometryUnit width;
    JMEGeometryUnit height;
} JMESize;

/**
 *  Vectors.
 */
typedef struct JMEVector {
    JMEGeometryUnit dx;
    JMEGeometryUnit dy;
} JMEVector;

/**
 *  Rectangles.
 */
typedef struct JMERect {
    JMEPoint origin;
    JMESize size;
} JMERect;

/**
 *  specify amount to inset (positive) for each of the edges.
 */
typedef struct JMEEdgeInsets {
    JMEGeometryUnit top;
    JMEGeometryUnit left;
    JMEGeometryUnit bottom;
    JMEGeometryUnit right;
} JMEEdgeInsets;

/**
 *  The "zero" point -- equivalent to JMEPointMake(0, 0).
 */
JME_EXTERN const JMEPoint JMEPointZero;

/**
 *  The "zero" size -- equivalent to JMESizeMake(0, 0).
 */
JME_EXTERN const JMESize JMESizeZero;

/**
 *  The "zero" rectangle -- equivalent to JMERectMake(0, 0, 0, 0).
 */
JME_EXTERN const JMERect JMERectZero;

/**
 *  The "empty" rect. This is the rectangle returned when, for example, we
 *  intersect two disjoint rectangles. Note that the null rect is not the
 *  same as the zero rect.
 */
JME_EXTERN const JMERect JMERectNull;

/**
 *  Make a point from `(x, y)'.
 */
inline JMEPoint JMEPointMake(JMEGeometryUnit x, JMEGeometryUnit y) {
    JMEPoint p; p.x = x; p.y = y; return p;
}

/**
 *  Make a size from `(width, height)'.
 */
inline JMESize JMESizeMake(JMEGeometryUnit width, JMEGeometryUnit height) {
    JMESize size; size.width = width; size.height = height; return size;
}

/**
 *  Make a vector from `(dx, dy)'.
 */
inline JMEVector JMEVectorMake(JMEGeometryUnit dx, JMEGeometryUnit dy) {
    JMEVector vector; vector.dx = dx; vector.dy = dy; return vector;
}

/**
 *  Make a rect from `(x, y; width, height)'.
 */
inline JMERect JMERectMake(JMEGeometryUnit x, JMEGeometryUnit y, JMEGeometryUnit width, JMEGeometryUnit height) {
    JMERect rect;
    rect.origin.x = x;
    rect.origin.y = y;
    rect.size.width = width;
    rect.size.height = height;
    return rect;
}

/**
 *  Return the leftmost x-value of `rect'.
 */
inline JMEGeometryUnit JMERectGetMinX(JMERect rect) {
    return JMEMin(rect.origin.x, rect.origin.x + rect.size.width);
}

/**
 *  Return the midpoint x-value of `rect'.
 */
inline JMEGeometryUnit JMERectGetMidX(JMERect rect) {
    return rect.origin.x + rect.size.width / 2;
}

/**
 *  Return the rightmost x-value of `rect'.
 */
inline JMEGeometryUnit JMERectGetMaxX(JMERect rect) {
    return JMEMax(rect.origin.x, rect.origin.x + rect.size.width);
}

/**
 *  Return the bottommost y-value of `rect'.
 */
inline JMEGeometryUnit JMERectGetMinY(JMERect rect) {
    return JMEMin(rect.origin.y, rect.origin.y + rect.size.height);
}

/**
 *  Return the midpoint y-value of `rect'.
 */
inline JMEGeometryUnit JMERectGetMidY(JMERect rect) {
    return rect.origin.y + rect.size.height / 2;
}

/**
 *  Return the topmost y-value of `rect'.
 */
inline JMEGeometryUnit JMERectGetMaxY(JMERect rect) {
    return JMEMax(rect.origin.y, rect.origin.y + rect.size.height);
}

/**
 *  Return true if `point1' and `point2' are the same, false otherwise.
 */
inline BOOL JMEPointEqualToPoint(JMEPoint point1, JMEPoint point2) {
    return JMEGeometryUnitEqual(point1.x, point2.x) && JMEGeometryUnitEqual(point1.y, point2.y);
}

/**
 *  Return true if `size1' and `size2' are the same, false otherwise.
 */
inline BOOL JMESizeEqualToSize(JMESize size1, JMESize size2) {
    return JMEGeometryUnitEqual(size1.width, size2.width) && JMEGeometryUnitEqual(size1.height, size2.height);
}

/**
 *  Return true if `rect1' and `rect2' are the same, false otherwise.
 */
inline BOOL JMERectEqualToRect(JMERect rect1, JMERect rect2) {
    return JMEGeometryUnitEqual(rect1.origin.x, rect2.origin.x) &&
    JMEGeometryUnitEqual(rect1.origin.y, rect2.origin.y) &&
    JMEGeometryUnitEqual(rect1.size.width, rect2.size.width) &&
    JMEGeometryUnitEqual(rect1.size.height, rect2.size.height);
}

/**
 *  Return true if `rect' is empty (that is, if it has zero width or height),
 *  false otherwise. A null rect is defined to be empty.
 */
inline BOOL JMERectIsEmpty(JMERect rect) {
    return JMEGeometryUnitEqual(rect.size.width, 0) || JMEGeometryUnitEqual(rect.size.height, 0);
}

/**
 *  Return true if `rect' is the null rectangle, false otherwise.
 */
inline BOOL JMERectIsNull(JMERect rect) {
    return JMEGeometryUnitEqual(rect.size.width, 0) || JMEGeometryUnitEqual(rect.size.height, 0);
}

/**
 *  Inset `rect' by `(dx, dy)' -- i.e., offset its origin by `(dx, dy)', and
 *  decrease its size by `(2*dx, 2*dy)'.
 */
inline JMERect JMERectInset(JMERect rect, JMEGeometryUnit dx, JMEGeometryUnit dy) {
    rect.origin.x += dx;
    rect.origin.y += dy;
    rect.size.width -= dx + dx;
    rect.size.height -= dy + dy;
    return rect;
}

/**
 *  Return the union of `r1' and `r2'.
 */
inline JMERect JMERectUnion(JMERect r1, JMERect r2) {
    JMERect unionRect;
    JMEGeometryUnit r1MinX = JMEMin(r1.origin.x, r1.origin.x + r1.size.width);
    JMEGeometryUnit r1MaxX = JMEMax(r1.origin.x, r1.origin.x + r1.size.width);
    JMEGeometryUnit r1MinY = JMEMin(r1.origin.y, r1.origin.y + r1.size.height);
    JMEGeometryUnit r1MaxY = JMEMax(r1.origin.y, r1.origin.y + r1.size.height);

    JMEGeometryUnit r2MinX = JMEMin(r2.origin.x, r2.origin.x + r2.size.width);
    JMEGeometryUnit r2MaxX = JMEMax(r2.origin.x, r2.origin.x + r2.size.width);
    JMEGeometryUnit r2MinY = JMEMin(r2.origin.y, r2.origin.y + r2.size.height);
    JMEGeometryUnit r2MaxY = JMEMax(r2.origin.y, r2.origin.y + r2.size.height);

    unionRect.origin.x = JMEMin(r1MinX, r2MinX);
    unionRect.origin.y = JMEMin(r1MinY, r2MinY);
    unionRect.size.width = JMEMax(r1MaxX, r2MaxX) - unionRect.origin.x;
    unionRect.size.height = JMEMax(r1MaxY, r2MaxY) - unionRect.origin.y;
    return unionRect;
}

/**
 *  Return the intersection of `r1' and `r2'. This may return a null rect.
 */
inline JMERect JMERectIntersection(JMERect r1, JMERect r2) {
    JMERect intersectionRect;
    JMEGeometryUnit r1MinX = JMEMin(r1.origin.x, r1.origin.x + r1.size.width);
    JMEGeometryUnit r1MaxX = JMEMax(r1.origin.x, r1.origin.x + r1.size.width);
    JMEGeometryUnit r1MinY = JMEMin(r1.origin.y, r1.origin.y + r1.size.height);
    JMEGeometryUnit r1MaxY = JMEMax(r1.origin.y, r1.origin.y + r1.size.height);

    JMEGeometryUnit r2MinX = JMEMin(r2.origin.x, r2.origin.x + r2.size.width);
    JMEGeometryUnit r2MaxX = JMEMax(r2.origin.x, r2.origin.x + r2.size.width);
    JMEGeometryUnit r2MinY = JMEMin(r2.origin.y, r2.origin.y + r2.size.height);
    JMEGeometryUnit r2MaxY = JMEMax(r2.origin.y, r2.origin.y + r2.size.height);
    if (r1MinX <= r2MinX && r1MinY <= r2MinY && r1MaxX > r2MinX && r1MaxY > r2MinY) {
        intersectionRect.origin.x = r2MinX;
        intersectionRect.origin.y = r2MinY;
        intersectionRect.size.width = JMEMin(r1MaxX, r2MaxX) - r2MinX;
        intersectionRect.size.height = JMEMin(r1MaxY, r2MaxY) - r2MinY;
    } else if (r2MinX <= r1MinX && r2MinY <= r1MinY && r2MaxX > r1MinX && r2MaxY > r1MinY) {
        intersectionRect.origin.x = r1MinX;
        intersectionRect.origin.y = r1MinY;
        intersectionRect.size.width = JMEMin(r2MaxX, r1MaxX) - r1MinX;
        intersectionRect.size.height = JMEMin(r2MaxY, r1MaxY) - r1MinY;
    }
    return intersectionRect;
}

/**
 *  Offset `rect' by `(dx, dy)'.
 */
inline JMERect JMERectOffset(JMERect rect, JMEGeometryUnit dx, JMEGeometryUnit dy) {
    rect.origin.x -= dx;
    rect.origin.y -= dy;
    return rect;
}

/**
 *  Return true if `point' is contained in `rect', false otherwise.
 */
inline BOOL JMERectContainsPoint(JMERect rect, JMEPoint point) {
    return JMERectGetMinX(rect) <= point.x && JMERectGetMaxX(rect) >= point.x &&
    JMERectGetMinY(rect) <= point.y && JMERectGetMaxY(rect) >= point.y;
}

/**
 *  Return true if `rect2' is contained in `rect1', false otherwise. `rect2'
 *  is contained in `rect1' if the union of `rect1' and `rect2' is equal to
 *  `rect1'.
 */
inline BOOL JMERectContainsRect(JMERect rect1, JMERect rect2) {
    return JMERectGetMinX(rect1) < JMERectGetMinX(rect2) && JMERectGetMaxX(rect1) > JMERectGetMaxX(rect2) &&
    JMERectGetMidY(rect1) < JMERectGetMinY(rect2) && JMERectGetMaxY(rect1) > JMERectGetMaxY(rect2);
}

/**
 *  Return true if `rect1' intersects `rect2', false otherwise. `rect1'
 * intersects `rect2' if the intersection of `rect1' and `rect2' is not the
 * null rect..
 */
inline BOOL JMERectIntersectsRect(JMERect rect1, JMERect rect2) {
    JMEGeometryUnit rect1MinX = JMEMin(rect1.origin.x, rect1.origin.x + rect1.size.width);
    JMEGeometryUnit rect1MaxX = JMEMax(rect1.origin.x, rect1.origin.x + rect1.size.width);
    JMEGeometryUnit rect1MinY = JMEMin(rect1.origin.y, rect1.origin.y + rect1.size.height);
    JMEGeometryUnit rect1MaxY = JMEMax(rect1.origin.y, rect1.origin.y + rect1.size.height);

    JMEGeometryUnit rect2MinX = JMEMin(rect2.origin.x, rect2.origin.x + rect2.size.width);
    JMEGeometryUnit rect2MaxX = JMEMax(rect2.origin.x, rect2.origin.x + rect2.size.width);
    JMEGeometryUnit rect2MinY = JMEMin(rect2.origin.y, rect2.origin.y + rect2.size.height);
    JMEGeometryUnit rect2MaxY = JMEMax(rect2.origin.y, rect2.origin.y + rect2.size.height);

    return (rect1MinX <= rect2MinX && rect1MinY <= rect2MinY && rect1MaxX > rect2MinX && rect1MaxY > rect2MinY) ||
    (rect2MinX <= rect1MinX && rect2MinY <= rect1MinY && rect2MaxX > rect1MinX && rect2MaxY > rect1MinY);
}

#endif /* defined(__H__JMEGeometry__H__) */
