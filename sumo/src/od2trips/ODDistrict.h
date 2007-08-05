/****************************************************************************/
/// @file    ODDistrict.h
/// @author  Daniel Krajzewicz
/// @date    Sept 2002
/// @version $Id$
///
// A district (origin/destination)
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.sourceforge.net/
// copyright : (C) 2001-2007
//  by DLR (http://www.dlr.de/) and ZAIK (http://www.zaik.uni-koeln.de/AFS)
/****************************************************************************/
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/
#ifndef ODDistrict_h
#define ODDistrict_h


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <vector>
#include <string>
#include <utility>
#include <utils/common/Named.h>
#include <utils/common/UtilExceptions.h>
#include <utils/helpers/RandomDistributor.h>


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class ODDistrict
 * @brief A district (origin/destination)
 *
 * Class representing a district which has some ingoing and outgoing connections
 *  to the road network which may be weighted.
 */
class ODDistrict : public Named
{
public:
    /** @brief Constructor
	 *
	 * @param[in] id The id of the district
	 */
    ODDistrict(const std::string &id) throw();


    /// Destructor
    ~ODDistrict() throw();


    /** @brief Adds a source connection
     *
     * A source is an edge where vehicles leave the district from to reach
     *  the network. The weight is used when a random source shall be
	 *  chosen.
	 *
	 * BTW, it is possible to add a source twice. In this case it will occure
	 *  twice within the distribution so that the behaviour is as adding
	 *  both given probabilities.
	 *
	 * @param[in] id The id of the source
	 * @param[in] weight The weight (probability to be chosen) of the source
	 */
    void addSource(const std::string &id, SUMOReal weight) throw();


    /** @brief Adds a sink connection
     *
     * A sink connection is an edge which is used by vehicles to leave the
     *  network and reach the district.  The weight is used when a random 
	 *  sink shall be chosen.
	 *
	 * BTW, it is possible to add a sink twice. In this case it will occure
	 *  twice within the distribution so that the behaviour is as adding
	 *  both given probabilities.
	 *
	 * @param[in] id The id of the sink
	 * @param[in] weight The weight (probability to be chosen) of the sink
	 */
    void addSink(const std::string &id, SUMOReal weight) throw();


    /** @brief Returns the id of a source to use
	 *
	 * If the list of this district's sources is empty, an OutOfBoundsException
	 *  -exception is thrown.
	 *
	 * @return One of this district's sources chosen randomly regarding their weights
	 * @exception OutOfBoundsException If this district has no sources
	 */
    std::string getRandomSource() const throw(OutOfBoundsException);


    /** @brief Returns the id of a sink to use
	 *
	 * If the list of this district's sinks is empty, an OutOfBoundsException
	 *  -exception is thrown.
	 *
	 * @return One of this district's sinks chosen randomly regarding their weights
	 * @exception OutOfBoundsException If this district has no sinks
	 */
    std::string getRandomSink() const throw(OutOfBoundsException);


private:
    /// Container of weighted sources
    RandomDistributor<std::string> mySources;

    /// Container of weighted sinks
    RandomDistributor<std::string> mySinks;


};


#endif

/****************************************************************************/

