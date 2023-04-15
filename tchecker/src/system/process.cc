/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#include "tchecker/system/process.hh"

namespace tchecker {

namespace system {

/* processes_t */

void processes_t::add_process(std::string const & name, tchecker::system::attributes_t const & attr)
{
  tchecker::process_id_t id = _procs_attr.size();
  _procs_index.add(name, id);
  _procs_attr.emplace_back(attr);
}

tchecker::system::attributes_t const & processes_t::process_attributes(tchecker::process_id_t id) const
{
  if (id >= processes_count())
    throw std::invalid_argument("Unknown process");
  return _procs_attr[id];
}

bool processes_t::is_process(tchecker::process_id_t id) const { return (id < processes_count()); }

bool processes_t::is_process(std::string const & name) const
{
  return _procs_index.find_key(name) != _procs_index.end_key_map();
}

} // end of namespace system

} // end of namespace tchecker
