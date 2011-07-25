/****************************************************************************/
/// @file    PositionVector.h
/// @author  Daniel Krajzewicz
/// @date    Sept 2002
/// @version $Id$
///
// A list of 2D-positions
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.sourceforge.net/
// Copyright (C) 2001-2011 DLR (http://www.dlr.de/) and contributors
/****************************************************************************/
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/
#ifndef PositionVector_h
#define PositionVector_h


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <queue>
#include <iostream>
#include <utils/common/VectorHelper.h>
#include "AbstractPoly.h"
#include "Boundary.h"
#include "Position.h"


class Line;

// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class PositionVector
 * @brief A list of 2D-positions
 */
class PositionVector
            : public AbstractPoly {
public:
    /// Definition of the list of points
    typedef std::deque<Position> ContType;

public:
    /** @brief Constructor
     *
     * Creates an empty position vector
     */
    PositionVector() throw();


    /** @brief Constructor
     * @param[in] v The vector to copy
     */
    PositionVector(const std::vector<Position> &v) throw();


    /// @brief Destructor
    ~PositionVector() throw();


    /// @name Adding items to the container
    /// @{

    /** @brief Appends the given position to the list
     * @param[in] p The position to append
     */
    void push_back(const Position &p) throw();


    /** @brief Appends all positions from the given vector
     * @param[in] p The vector from which values shall be appended
     */
    void push_back(const PositionVector &p) throw();
    /// @}



    /// Puts the given position at the begin of the list
    void push_front(const Position &p);

    /** @brief Returns the information whether the position vector describes a polygon lying around the given point
        The optional offset is added to the polygon's bounderies */
    bool around(const Position &p, SUMOReal offset=0) const;

    /** @brief Returns the information whether the given polygon overlaps with this
        Again a boundary may be specified */
    bool overlapsWith(const AbstractPoly &poly, SUMOReal offset=0) const;

    /** Returns the information whether this list of points interesects the given line */
    bool intersects(const Position &p1, const Position &p2) const;

    /** Returns the information whether this list of points interesects one the given lines */
    bool intersects(const PositionVector &v1) const;

    /** Returns the position of the intersection */
    Position intersectsAtPoint(const Position &p1,
                               const Position &p2) const; // !!!

    PositionVector intersectsAtPoints(const Position &p1,
                                      const Position &p2) const; // !!!

    DoubleVector intersectsAtLengths(const PositionVector &s) const; // !!!

    DoubleVector intersectsAtLengths(const Line &s) const; // !!!

    /** Returns the position of the intersection */
    Position intersectsAtPoint(const PositionVector &v1) const; // !!!

    /// Removes all information from this list
    void clear();

    void closePolygon();

    /** @brief returns the position at the given index
        !!! exceptions?*/
    const Position &operator[](int index) const;
    Position &operator[](int index);

    /// returns the number of points making up the line vector
    size_t size() const;

    /// Returns the position at the given length
    Position positionAtLengthPosition(SUMOReal pos) const;

    /// Returns the position at the given length
    SUMOReal rotationDegreeAtLengthPosition(SUMOReal pos) const;

    /// Returns the position between the two given point at the specified position */
    static Position positionAtLengthPosition(const Position &p1,
            const Position &p2, SUMOReal pos);

    /// Returns a boundary enclosing this list of lines
    Boundary getBoxBoundary() const;

    /** @brief Returns the arithmetic of all corner points 
     * @note: this is different from the centroid! */
    Position getPolygonCenter() const;

    /** @brief Returns the centroid (closes the polygon if unclosed) */
    Position getCentroid() const;

    /** @brief enlarges/shrinks the polygon based at the centroid */
    void scaleSize(SUMOReal factor);

    Position getLineCenter() const;

    Position pop_back();
    Position pop_front();

    /// Returns the length
    SUMOReal length() const;


    /// Returns the area (0 for non-closed)
    SUMOReal area() const;

    /// Returns the information whether this polygon lies partially within the given polygon
    bool partialWithin(const AbstractPoly &poly, SUMOReal offset=0) const;

    /// Returns the first position
    const Position &getBegin() const;

    /// Returns the last position
    const Position &getEnd() const;

    /// Returns the two lists made when this list vector is splitted at the given point
    std::pair<PositionVector, PositionVector> splitAt(SUMOReal where) const;

    /// Output operator
    friend std::ostream &operator<<(std::ostream &os,
                                    const PositionVector &geom);

    bool crosses(const Position &p1, const Position &p2) const;

    void reshiftRotate(SUMOReal xoff, SUMOReal yoff, SUMOReal rot);

    PositionVector convexHull() const;

    int appendWithCrossingPoint(const PositionVector &v);


    const ContType &getCont() const {
        return myCont;
    }

    PositionVector getSubpart(SUMOReal begin, SUMOReal end) const;

    void sortAsPolyCWByAngle();

    void sortByIncreasingXY();

    void extrapolate(SUMOReal val);

    PositionVector reverse() const;

    void move2side(SUMOReal amount);
    void move2side(SUMOReal amount, int index);

    Line lineAt(size_t pos) const;

    Line getBegLine() const;

    Line getEndLine() const;


    void insertAt(int index, const Position &p);

    // brief inserts p between the two closest positions
    void insertAtClosest(const Position &p);

    /// comparing operation
    bool operator==(const PositionVector &v2) const;

    class as_poly_cw_sorter {
    public:
        /// constructor
        as_poly_cw_sorter() {};

    public:
        /// comparing operation
        int operator()(const Position &p1, const Position &p2) const;

    };

    class increasing_x_y_sorter {
    public:
        /// constructor
        explicit increasing_x_y_sorter();

    public:
        /// comparing operation
        int operator()(const Position &p1, const Position &p2) const;

    };

    // !!!
    SUMOReal isLeft(const Position &P0, const Position &P1, const Position &P2) const;

    void set(size_t pos, const Position &p);

    void pruneFromBeginAt(const Position &p);
    void pruneFromEndAt(const Position &p);

    SUMOReal beginEndAngle() const;

    void eraseAt(int i);

    SUMOReal nearest_position_on_line_to_point(const Position &p, bool perpendicular=true) const;

    /* @brief index of the closest position to p
     * @note: may only be called for a non-empty vector */
    int indexOfClosest(const Position &p) const;

    // distances of all my points to s and all of s points to myself
    DoubleVector distances(const PositionVector &s) const;

    SUMOReal distance(const Position &p) const;

    void push_back_noDoublePos(const Position &p);
    void push_front_noDoublePos(const Position &p);

    void replaceAt(size_t index, const Position &by);

    bool isClosed() const;

    void removeDoublePoints();

    void removeColinearPoints();

private:

    /// The list of points
    ContType myCont;

};


#endif

/****************************************************************************/

