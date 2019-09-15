#ifndef LOGGING_H
#define LOGGING_H

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#define LOG(level) BOOST_LOG_TRIVIAL(level)

void init_logging(const boost::log::trivial::severity_level &level);

#endif
