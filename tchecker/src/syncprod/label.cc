/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#include "tchecker/syncprod/label.hh"

namespace tchecker {

namespace syncprod {

void labels_t::add_label(std::string const & name) { _labels_index.add(name, _labels_index.size()); }

bool labels_t::is_label(tchecker::label_id_t id) const { return _labels_index.find_value(id) != _labels_index.end_value_map(); }

bool labels_t::is_label(std::string const & name) const { return _labels_index.find_key(name) != _labels_index.end_key_map(); }

} // end of namespace syncprod

} // end of namespace tchecker
