/**
 * \file PolicyWriter.cc
 */
#include "lsst/pex/policy/PolicyWriter.h"
#include "lsst/pex/policy/Policy.h"
#include "lsst/pex/policy/PolicyFile.h"

#include <fstream>
#include <sstream>

namespace lsst {
namespace pex {
namespace policy {

using lsst::daf::base::PropertySet;
using lsst::daf::base::Persistable;

/*
 * create a writer attached to an output stream
 * @param out     the output stream to write data to
 */
PolicyWriter::PolicyWriter(std::ostream *out) : _myos(0), _os(out) {
    if (! out) {
        _myos = new std::ostringstream();
        _os = _myos;
    }
}

/*
 * create a writer attached to a file.  The file will be immediately 
 * opened for writing.
 * @param file     the path to the output file
 */
PolicyWriter::PolicyWriter(const std::string& file, bool append) 
    : _myos(new std::ofstream(file.c_str(), append ? std::ios_base::app 
                                                   : std::ios_base::trunc)),
      _os(0)
{
    _os = _myos;
}

PolicyWriter::~PolicyWriter() {
    if (_myos) delete _myos;
}

/*
 * return the written data as a string.  This string will be non-empty 
 * only if this class was was instantiated without an attached stream.
 */
std::string PolicyWriter::toString() {
    std::ostringstream *ss = dynamic_cast<std::ostringstream*>(_os);
    if (ss) return ss->str();
    return std::string();
}

/*
 * close the output stream.  This has no effect if the attached 
 * stream is not a file stream.  
 */
void PolicyWriter::close() {  
    std::ofstream *fs = dynamic_cast<std::ofstream*>(_os);
    if (fs) fs->close();
}

/*
 * write the contents of a policy the attached stream.  Each top-level
 * parameter will be recursively printed.
 * @param policy     the policy data to write
 */
void PolicyWriter::write(const Policy& policy, bool doDecl) {
    if (doDecl) 
        (*_os) << "#<?cfg paf policy ?>" << std::endl;

    Policy::StringArray names = policy.names(true);
    Policy::StringArray::const_iterator ni;
    for(ni=names.begin(); ni != names.end(); ++ni) {
        try {
            const std::type_info& tp = policy.typeOf(*ni);
            if (tp == typeid(bool)) {
                write(*ni, policy.getBoolArray(*ni));
            }
            else if (tp == typeid(int)) {
                write(*ni, policy.getIntArray(*ni));
            }
            else if (tp == typeid(double)) {
                write(*ni, policy.getDoubleArray(*ni));
            }
            else if (tp == typeid(std::string)) {
                write(*ni, policy.getStringArray(*ni));
            }
            else if (tp == typeid(PropertySet::Ptr)) {
                write(*ni, policy.getPolicyArray(*ni));
            }
            else if (tp == typeid(Persistable::Ptr)) {
                write(*ni, policy.getFileArray(*ni));
            }
            else {
                throw LSST_EXCEPT(pexExcept::LogicErrorException, "Policy: unexpected type for name=" + *ni);
            }
        }
        catch (NameNotFound&) {
            // shouldn't happen
            write(*ni, "<missing data>");
        }
    }

}

#define PW_WRITE_PRIMITIVE(vtype) \
void PolicyWriter::write(const std::string& name, vtype value) {    \
    std::vector<vtype> vals;                                        \
    vals.push_back(value); \
    write(name, vals); \
}

PW_WRITE_PRIMITIVE(int)
PW_WRITE_PRIMITIVE(double)
PW_WRITE_PRIMITIVE(bool)

void PolicyWriter::write(const std::string& name, const std::string& value) {
    std::vector<std::string> vals; 
    vals.push_back(value); 
    write(name, vals); 
}

void PolicyWriter::write(const std::string& name, const Policy& value) {   
    std::vector<Policy::Ptr> vals; 
    vals.push_back(Policy::Ptr(new Policy(value))); 
    write(name, vals); 
}

void PolicyWriter::write(const std::string& name, const PolicyFile& value) {   
    std::vector<Policy::FilePtr> vals; 
    vals.push_back(Policy::FilePtr(new PolicyFile(value))); 
    write(name, vals); 
}




}}}  // end lsst::pex::policy
