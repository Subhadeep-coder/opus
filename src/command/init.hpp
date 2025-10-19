/**
 * @file command/init.hpp
 * @brief Command parser initialization interface
 */

#ifndef OPUS_COMMAND_INIT_HPP
#define OPUS_COMMAND_INIT_HPP

#include <memory>
#include "parser.hpp"

namespace opus
{
    namespace command
    {

        /**
         * @brief Initializes and configures the command parser with all options
         * @return Unique pointer to configured CommandParser instance
         *
         * This function creates a CommandParser and sets up all command-line
         * options for the Opus application including:
         * - Host specification (-h)
         * - Login username (-U)
         * - Registration username (-nU)
         * - Port number (-p)
         * - Verbose flag (--verbose)
         */
        std::unique_ptr<CommandParser> initialize_parser();

    }
}

#endif