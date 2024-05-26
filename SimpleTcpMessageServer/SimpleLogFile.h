#pragma once

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/log/common.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sources/logger.hpp>

namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;

typedef sinks::asynchronous_sink< sinks::text_file_backend > file_sink;

boost::shared_ptr< file_sink > init_logging();
void stop_logging(boost::shared_ptr< file_sink >& sink);
