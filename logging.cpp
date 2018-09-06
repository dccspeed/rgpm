#include "logging.h"

void init_logging(const boost::log::trivial::severity_level &level) {
   boost::log::core::get()->set_filter(
         boost::log::trivial::severity >= level
    );
}
