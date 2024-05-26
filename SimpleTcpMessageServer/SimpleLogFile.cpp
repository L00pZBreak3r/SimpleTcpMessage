#include "pch.h"

#include <string>
#include <iostream>

#include <boost/log/expressions.hpp>

#include "SimpleLogFile.h"

namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;


boost::shared_ptr< file_sink > init_logging()
{
    // Create a text file sink
    boost::shared_ptr< file_sink > sink(new file_sink(
        keywords::file_name = "file.log",
        keywords::open_mode = std::ios_base::out | std::ios_base::app,
        keywords::enable_final_rotation = false
    ));

    // Add the sink to the core
    logging::core::get()->add_sink(sink);
    return sink;
}


void stop_logging(boost::shared_ptr< file_sink >& sink)
{
    boost::shared_ptr< logging::core > core = logging::core::get();

    // Remove the sink from the core, so that no records are passed to it
    core->remove_sink(sink);

    // Break the feeding loop
    sink->stop();

    // Flush all log records that may have left buffered
    sink->flush();

    sink.reset();
}
