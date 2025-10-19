/**
 * @file command/init.cpp
 * @brief Implementation of command parser initialization
 */

#include "init.hpp"
#include <memory>

namespace opus
{
    namespace command
    {

        std::unique_ptr<CommandParser> initialize_parser()
        {
            auto parser = std::make_unique<CommandParser>("opus", "Opus Terminal Database Client");

            // Host configuration (required)
            parser->add_option(
                "-h",
                "Database host address (e.g., localhost, 192.168.1.100)",
                OptionType::REQUIRED_VALUE,
                true // required
            );

            // Login username (mutually exclusive with -nU)
            parser->add_option(
                "-U",
                "Username for login to existing account",
                OptionType::REQUIRED_VALUE,
                false,  // optional (but either -U or -nU required)
                {"-nU"} // conflicts with -nU
            );

            // Registration username (mutually exclusive with -U)
            parser->add_option(
                "-nU",
                "Username for new user registration",
                OptionType::REQUIRED_VALUE,
                false,
                {"-U"} // conflicts with -U
            );

            // Optional port specification
            parser->add_option(
                "-p",
                "Port number (1-65535). Defaults to config.yaml value if not specified",
                OptionType::REQUIRED_VALUE,
                false // optional
            );

            // Verbose output flag
            parser->add_option(
                "--verbose",
                "Enable verbose output for debugging and detailed information",
                OptionType::FLAG,
                false // optional
            );

            return parser;
        }

    }
}