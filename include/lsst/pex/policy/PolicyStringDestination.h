// -*- lsst-c++ -*-

/*
 * LSST Data Management System
 * Copyright 2008, 2009, 2010 LSST Corporation.
 *
 * This product includes software developed by the
 * LSST Project (http://www.lsst.org/).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the LSST License Statement and
 * the GNU General Public License along with this program.  If not,
 * see <http://www.lsstcorp.org/LegalNotices/>.
 */

/**
 * @file PolicyStringDestination.h
 *
 * @ingroup pex
 * @brief definition of the PolicyStringDestination class
 * @author Ray Plante
 *
 */

#ifndef LSST_PEX_POLICY_STRDEST_H
#define LSST_PEX_POLICY_STRDEST_H

#include <sstream>
#include <memory>

#include "lsst/pex/policy/PolicyStreamDestination.h"

namespace lsst {
namespace pex {
namespace policy {

/**
 * a generic stream destination for policy data
 */
class PolicyStringDestination : public PolicyStreamDestination {
public:

    /**
     * create the destination
     */
    PolicyStringDestination();

    /**
     * create the destination, initialized with the given string
     * @param str   initial contents for the output string
     */
    PolicyStringDestination(const std::string& str);

    /**
     * release resource associated with the destination
     */
    virtual ~PolicyStringDestination();

    /**
     * return the data written so far as a string
     */
    std::string getData() {
        return _sstrm->str();
    }

private:
    std::ostringstream *_sstrm;
};

}}}  // end namespace lsst::pex::policy

#endif // LSST_PEX_POLICY_STRDEST_H
