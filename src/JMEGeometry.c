/**
 Filename:       JMERemoter.c
 Revised:        $Date: 2015-06-18$
 Revision:       $Revision: 01 $
 Author:         Oborn.Jung

 Description:    This file contains the common functions of geometry.

 Copyright 2015 JONMA Inc. All rights reserved.
 */

#include "JMEGeometry.h"

/**
 *  The "zero" point -- equivalent to JMEPointMake(0, 0).
 */
const JMEPoint JMEPointZero = {0, 0};

/**
 *  The "zero" size -- equivalent to JMESizeMake(0, 0).
 */
const JMESize JMESizeZero = {0, 0};

/**
 *  The "zero" rectangle -- equivalent to JMERectMake(0, 0, 0, 0).
 */
const JMERect JMERectZero = {{0, 0}, {0, 0}};

/**
 *  The "empty" rect. This is the rectangle returned when, for example, we
 *  intersect two disjoint rectangles. Note that the null rect is not the
 *  same as the zero rect.
 */
const JMERect JMERectNull = {{0, 0}, {0, 0}};
