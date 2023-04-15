/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#ifndef TCHECKER_SYSTEM_PROCESS_HH
#define TCHECKER_SYSTEM_PROCESS_HH

#include <string>
#include <vector>

#include "tchecker/basictypes.hh"
#include "tchecker/system/attribute.hh"
#include "tchecker/utils/index.hh"

/*!
 \file process.hh
 \brief Processes in systems
 */

namespace tchecker {

namespace system {

/*!
 \class processes_t
 \brief Collection of processes
 */
class processes_t {
public:
  /*!
   \brief Add a process
   \param name : process name
   \param attr : process attributes
   \pre process name is not declared yet
   \post process name has been added with attributes attr
   \throw std::invalid_argument : if process name is already declared
   */
  void add_process(std::string const & name, tchecker::system::attributes_t const & attr = tchecker::system::attributes_t());

  /*!
   \brief Accessor
   \return number of processes
   */
  inline tchecker::process_id_t processes_count() const { return _procs_attr.size(); }

  /*!
   \brief Accessor
   \param name : process name
   \return identifier of process name
   \throw std::invalid_argument : if name is not a process
   */
  inline tchecker::process_id_t process_id(std::string const & name) const { return _procs_index.value(name); }

  /*!
   \brief Accessor
   \param id : process identifier
   \return name of process id
   \throw std::invalid_argument : if id is not a process
   */
  inline std::string const & process_name(tchecker::process_id_t id) const { return _procs_index.key(id); }

  /*!
   \brief Accessor
   \param id : process identifier
   \return attributes of process id
   \throw std::invalid_argument : if id is not a process identifier
   */
  tchecker::system::attributes_t const & process_attributes(tchecker::process_id_t id) const;

  /*!
   \brief Checks validity of process identifier
   \param id : process identifier
   \return true if id is a declared process identifier, false othwerise
   */
  bool is_process(tchecker::process_id_t id) const;

  /*!
   \brief Checks validity of process name
   \param name : process name
   \return true if name is a declared process, false otherwise
   */
  bool is_process(std::string const & name) const;

private:
  std::vector<tchecker::system::attributes_t> _procs_attr;             /*!< Processes attributes */
  tchecker::index_t<std::string, tchecker::process_id_t> _procs_index; /*!< Map process name <-> process identifier */
};

} // end of namespace system

} // end of namespace tchecker

#endif // TCHECKER_SYSTEM_PROCESS_HH
