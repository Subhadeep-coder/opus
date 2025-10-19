/*

Commands will be all terminal based.
1. Login: opus -h localhost -U <username>
    Enter your password:

2. Create new user: opus -h localhost -nU <username>
    Enter password:
    Confirm password:


So parse the terminal command. Extract the parameters and it's corresponding values.

for now parameters are:

-h : for mentioning host.
-U : for mentioning the username.
-nU : it's also for mentioning the username but it is used when we register new user.
-p : for mentioning port number (if not found then it will fallback to default port that is mentioned in config.yaml configuration file)

*/

#include "parser.hpp"
#include <sstream>

namespace opus
{
    namespace command
    {
        Option::Option(std::string n, std::string d, OptionType t, bool req, std::vector<std::string> conf)
            : name(std::move(n)), description(std::move(d)), type(t), required(req), conflicts(std::move(conf)) {}

        CommandParser::CommandParser(std::string prog_name, std::string desc)
            : program_name(std::move(prog_name)), description(std::move(desc)) {}

        void CommandParser::add_option(std::string flag, std::string desc, OptionType type, bool required, std::vector<std::string> conflicts)
        {
            // Make a stable copy for the map key before moving from `flag`.
            std::string key = flag;
            options[key] = Option(std::move(flag), std::move(desc), type, required, std::move(conflicts));
        }

        bool CommandParser::parse(int argc, char *argv[])
        {
            // Parse all arguments
            for (int i = 1; i < argc; ++i)
            {
                std::string arg = argv[i];

                // Check if option exists
                auto it = options.find(arg);
                if (it == options.end())
                {
                    std::cerr << "Error: Unknown option '" << arg << "'\n";
                    print_usage();
                    return false;
                }

                const Option &opt = it->second;

                // Handle value-based options
                if (opt.type == OptionType::REQUIRED_VALUE)
                {
                    if (i + 1 >= argc)
                    {
                        std::cerr << "Error: Option '" << arg
                                  << "' requires a value\n";
                        return false;
                    }
                    parsed_values[arg] = argv[++i];
                }
                // Handle flag options
                else
                {
                    parsed_values[arg] = "true";
                }
            }

            // Validate required options
            if (!validate_required_options())
            {
                return false;
            }

            // Check for conflicting options
            if (!validate_conflicts())
            {
                return false;
            }

            return true;
        }

        std::optional<std::string> CommandParser::get(const std::string &flag) const
        {
            auto it = parsed_values.find(flag);
            if (it != parsed_values.end())
            {
                return it->second;
            }
            return std::nullopt;
        }

        bool CommandParser::has(const std::string &flag) const
        {
            return parsed_values.find(flag) != parsed_values.end();
        }

        void CommandParser::print_usage() const
        {
            std::cout << "\nUsage: " << program_name << " [OPTIONS]\n\n";
            std::cout << description << "\n\n";
            std::cout << "Options:\n";

            for (const auto &[flag, opt] : options)
            {
                std::cout << "  " << flag;

                if (opt.type == OptionType::REQUIRED_VALUE)
                {
                    std::cout << " <value>";
                }

                std::cout << "\n      " << opt.description;

                if (opt.required)
                {
                    std::cout << " (required)";
                }

                if (!opt.conflicts.empty())
                {
                    std::cout << "\n      Conflicts with: ";
                    for (size_t i = 0; i < opt.conflicts.size(); ++i)
                    {
                        std::cout << opt.conflicts[i];
                        if (i < opt.conflicts.size() - 1)
                        {
                            std::cout << ", ";
                        }
                    }
                }

                std::cout << "\n\n";
            }
        }

        void CommandParser::clear()
        {
            parsed_values.clear();
        }

        size_t CommandParser::parsed_count() const
        {
            return parsed_values.size();
        }

        // Private validation methods
        bool CommandParser::validate_required_options() const
        {
            for (const auto &[flag, opt] : options)
            {
                if (opt.required && !has(flag))
                {
                    std::cerr << "Error: Required option '" << flag
                              << "' is missing\n";
                    print_usage();
                    return false;
                }
            }
            return true;
        }

        bool CommandParser::validate_conflicts() const
        {
            for (const auto &[flag, value] : parsed_values)
            {
                const auto &opt_it = options.find(flag);
                if (opt_it == options.end())
                    continue;

                const auto &conflicts = opt_it->second.conflicts;
                for (const auto &conflict : conflicts)
                {
                    if (has(conflict))
                    {
                        std::cerr << "Error: Options '" << flag << "' and '"
                                  << conflict << "' cannot be used together\n";
                        return false;
                    }
                }
            }
            return true;
        }

    }
}