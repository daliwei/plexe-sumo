/****************************************************************************/
/// @file    GUIShapeContainer.cpp
/// @author  Daniel Krajzewicz
/// @author  Jakob Erdmann
/// @author  Michael Behrisch
/// @date    08.10.2009
/// @version $Id$
///
// Storage for geometrical objects extended by mutexes
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.sourceforge.net/
// Copyright (C) 2001-2012 DLR (http://www.dlr.de/) and contributors
/****************************************************************************/
//
//   This file is part of SUMO.
//   SUMO is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include "GUIShapeContainer.h"
#include <foreign/rtree/SUMORTree.h>
#include <utils/gui/globjects/GUIPolygon.h>
#include <utils/gui/globjects/GUIPointOfInterest.h>

#ifdef CHECK_MEMORY_LEAKS
#include <foreign/nvwa/debug_new.h>
#endif // CHECK_MEMORY_LEAKS


// ===========================================================================
// method definitions
// ===========================================================================
GUIShapeContainer::GUIShapeContainer(SUMORTree& vis)
    : myVis(vis) {}


GUIShapeContainer::~GUIShapeContainer() {}


bool
GUIShapeContainer::addPoI(const std::string& name, int layer, const std::string& type, const RGBColor& c,
                          const Position& pos, const std::string imgFile, int imgWidth, int imgHeight) {
    GUIPointOfInterest* p = new GUIPointOfInterest(layer, name, type, pos, c, imgFile, imgWidth, imgHeight);
    AbstractMutex::ScopedLocker locker(myLock);
    const bool ret = add(layer, p);
    if (ret) {
        myVis.addAdditionalGLObject(p);
    } else {
        delete p;
    }
    return ret;
}


bool
GUIShapeContainer::addPolygon(const std::string& name, int layer, const std::string& type, const RGBColor& c,
                              bool filled, const PositionVector& shape) {
    GUIPolygon* p = new GUIPolygon(layer, name, type, c, shape, filled);
    AbstractMutex::ScopedLocker locker(myLock);
    const bool ret = add(layer, p);
    if (ret) {
        myVis.addAdditionalGLObject(p);
    } else {
        delete p;
    }
    return ret;
}



bool
GUIShapeContainer::removePoI(int layer, const std::string& id) {
    AbstractMutex::ScopedLocker locker(myLock);
    if (myPOILayers.find(layer) == myPOILayers.end()) {
        myLock.unlock();
        return false;
    }
    NamedObjectCont<PointOfInterest*> &c = myPOILayers.find(layer)->second;
    PointOfInterest* p = c.get(id);
    if (p == 0) {
        return false;
    }
    myVis.removeAdditionalGLObject(static_cast<GUIPointOfInterest*>(p));
    return c.remove(id);
}


bool
GUIShapeContainer::removePolygon(int layer, const std::string& id) {
    AbstractMutex::ScopedLocker locker(myLock);
    if (myPolygonLayers.find(layer) == myPolygonLayers.end()) {
        return false;
    }
    GUIPolygon* p = static_cast<GUIPolygon*>(myPolygonLayers.find(layer)->second.get(id));
    if (p == 0) {
        return false;
    }
    myVis.removeAdditionalGLObject(p);
    return myPolygonLayers.find(layer)->second.remove(id);
}



void
GUIShapeContainer::movePoI(int layer, const std::string& id, const Position& pos) {
    AbstractMutex::ScopedLocker locker(myLock);
    if (myPOILayers.find(layer) != myPOILayers.end()) {
        PointOfInterest* p = myPOILayers.find(layer)->second.get(id);
        if (p != 0) {
            myVis.removeAdditionalGLObject(static_cast<GUIPointOfInterest*>(p));
            static_cast<Position*>(p)->set(pos);
            myVis.addAdditionalGLObject(static_cast<GUIPointOfInterest*>(p));
        }
    }
}


void
GUIShapeContainer::reshapePolygon(int layer, const std::string& id, const PositionVector& shape) {
    AbstractMutex::ScopedLocker locker(myLock);
    if (myPolygonLayers.find(layer) != myPolygonLayers.end()) {
        GUIPolygon* p = static_cast<GUIPolygon*>(myPolygonLayers.find(layer)->second.get(id));
        if (p != 0) {
            myVis.removeAdditionalGLObject(p);
            p->setShape(shape);
            myVis.addAdditionalGLObject(p);
        }
    }
}


std::vector<GUIGlID>
GUIShapeContainer::getShapeIDs() const {
    AbstractMutex::ScopedLocker locker(myLock);
    std::vector<GUIGlID> ret;
    for (int j = myMinLayer; j <= myMaxLayer; ++j) {
        const PolyMap& pol = getPolygonCont(j).getMyMap();
        for (PolyMap::const_iterator i = pol.begin(); i != pol.end(); ++i) {
            ret.push_back(static_cast<GUIPolygon*>((*i).second)->getGlID());
        }
        const std::map<std::string, PointOfInterest*> &poi = getPOICont(j).getMyMap();
        for (std::map<std::string, PointOfInterest*>::const_iterator i = poi.begin(); i != poi.end(); ++i) {
            ret.push_back(static_cast<GUIPointOfInterest*>((*i).second)->getGlID());
        }
    }
    return ret;
}


/****************************************************************************/

