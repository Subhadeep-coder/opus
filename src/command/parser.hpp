#ifndef OPUS_COMMAND_PARSER_HPP
#define OPUS_COMMAND_PARSER_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <optional>
#include <sstream>
#include <stdexcept>

namespace opus
{
    namespace command
    {

        /**
         * @enum OptionType
         * @brief Defines the type of command-line option
         */
        enum class OptionType
        {
            REQUIRED_VALUE,
            FLAG
        };

        /**
         * @struct Option
         * @brief Represents a single command-line option configuration
         */
        struct Option
        {
            std::string name;
            std::string description;
            OptionType type;
            bool required;
            std::vector<std::string> conflicts;

            Option() = default;
            Option(std::string n, std::string d, OptionType t, bool req, std::vector<std::string> conf);
        };

        /**
         * @class CommandParser
         * @brief Main parser class for handling command-line arguments
         */
        class CommandParser
        {
        private:
            std::map<std::string, Option> options;
            std::map<std::string, std::string> parsed_values;
            std::string program_name;
            std::string description;

            // Private helper methods
            bool validate_required_options() const;
            bool validate_conflicts() const;

            /**
             * @brief Helper template for type conversion
             */
            template <typename T>
            T convert_to(const std::string &str) const
            {
                std::istringstream iss(str);
                T value;
                iss >> value;
                if (iss.fail())
                {
                    throw std::invalid_argument("Conversion failed");
                }
                return value;
            }

        public:
            /**
             * @brief Constructs a CommandParser
             * @param prog_name Name of the program
             * @param desc Description of the program
             */
            CommandParser(std::string prog_name, std::string desc);

            /**
             * @brief Adds a command-line option to the parser
             */
            void add_option(std::string flag,
                            std::string desc,
                            OptionType type = OptionType::REQUIRED_VALUE,
                            bool required = false,
                            std::vector<std::string> conflicts = {});

            /**
             * @brief Parses command-line arguments
             * @return true if parsing succeeded, false otherwise
             */
            bool parse(int argc, char *argv[]);

            /**
             * @brief Retrieves the value of a parsed option
             */
            std::optional<std::string> get(const std::string &flag) const;

            /**
             * @brief Checks if an option was provided
             */
            bool has(const std::string &flag) const;

            /**
             * @brief Retrieves a typed value from a parsed option
             * @tparam T The type to convert the value to
             * @param flag The option flag to retrieve
             * @return Optional containing the converted value if present and valid
             *
             * Supports: int, long, float, double, std::string
             */
            template <typename T>
            std::optional<T> get_as(const std::string &flag) const
            {
                auto value = get(flag);
                if (!value.has_value())
                {
                    return std::nullopt;
                }

                try
                {
                    return convert_to<T>(value.value());
                }
                catch (...)
                {
                    return std::nullopt;
                }
            }

            /**
             * @brief Prints usage information to stdout
             */
            void print_usage() const;

            /**
             * @brief Clears all parsed values
             */
            void clear();

            /**
             * @brief Gets the number of parsed options
             */
            size_t parsed_count() const;
        };

    }
}

#endif