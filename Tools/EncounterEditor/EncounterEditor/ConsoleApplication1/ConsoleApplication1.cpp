#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <regex>
#include <filesystem>

struct Config
{
    std::string fowmPath;
    std::string focwmPath;
    std::string groupsPath;
};

struct Encounter
{
    std::string groupName;
    int quantity;
};

// ------------------------------------------------------------
// Read configuration
// ------------------------------------------------------------

bool LoadConfig(const std::string& configPath, Config& config)
{
    std::ifstream file(configPath);

    if (!file.is_open())
    {
        std::cout << "ERROR: Could not open config:\n";
        std::cout << configPath << "\n";
        return false;
    }

    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        if (line[0] == '#')
            continue;

        size_t equals = line.find('=');

        if (equals == std::string::npos)
            continue;

        std::string key = line.substr(0, equals);
        std::string value = line.substr(equals + 1);

        if (key == "FOWM_PATH")
            config.fowmPath = value;
        else if (key == "FOCWM_PATH")
            config.focwmPath = value;
        else if (key == "GROUPS_PATH")
            config.groupsPath = value;
    }

    file.close();

    if (config.fowmPath.empty() ||
        config.focwmPath.empty() ||
        config.groupsPath.empty())
    {
        std::cout << "ERROR: Config is missing one or more paths.\n";
        std::cout << "Required:\n";
        std::cout << "FOWM_PATH\n";
        std::cout << "FOCWM_PATH\n";
        std::cout << "GROUPS_PATH\n";
        return false;
    }

    return true;
}

// ------------------------------------------------------------
// Load GROUP_* definitions from worldmap_h.fos
// ------------------------------------------------------------

std::map<std::string, int> LoadGroupDefinitions(
    const std::string& filePath)
{
    std::map<std::string, int> groups;

    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cout << "ERROR: Could not open group definitions:\n";
        std::cout << filePath << "\n";
        return groups;
    }

    std::regex groupRegex(
        R"(#define\s+(GROUP_[A-Za-z0-9_]+)\s+\((\d+)\))"
    );

    std::string line;

    while (std::getline(file, line))
    {
        std::smatch match;

        if (std::regex_search(line, match, groupRegex))
        {
            std::string name = match[1].str();
            int id = std::stoi(match[2].str());

            groups[name] = id;
        }
    }

    file.close();

    return groups;
}

// ------------------------------------------------------------
// Parse GROUP_NAME:QUANTITY
// ------------------------------------------------------------

std::vector<Encounter> ParseEncounters(
    const std::string& field)
{
    std::vector<Encounter> encounters;

    std::stringstream ss(field);
    std::string entry;

    while (std::getline(ss, entry, ','))
    {
        size_t colon = entry.find(':');

        if (colon == std::string::npos)
            continue;

        std::string groupName =
            entry.substr(0, colon);

        try
        {
            int quantity =
                std::stoi(entry.substr(colon + 1));

            encounters.push_back(
                { groupName, quantity }
            );
        }
        catch (...)
        {
            std::cout << "WARNING: Could not parse encounter: "
                << entry << "\n";
        }
    }

    return encounters;
}

// ------------------------------------------------------------
// Rebuild encounter field
// ------------------------------------------------------------

std::string BuildEncounterField(
    const std::vector<Encounter>& encounters)
{
    std::string result;

    for (size_t i = 0; i < encounters.size(); i++)
    {
        if (i > 0)
            result += ",";

        result += encounters[i].groupName;
        result += ":";
        result += std::to_string(
            encounters[i].quantity
        );
    }

    return result;
}

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

int main()
{
    std::cout
        << "========================================\n"
        << " FOnline Encounter Group Editor\n"
        << "========================================\n\n";

    // Find the directory where the EXE is located.
    char exePathBuffer[MAX_PATH];

    GetModuleFileNameA(
        nullptr,
        exePathBuffer,
        MAX_PATH
    );

    std::filesystem::path exePath(exePathBuffer);

    // Look for the CFG next to the EXE.
    std::filesystem::path configPath =
        exePath.parent_path() / "EncounterEditor.cfg";

    std::cout << "Executable:\n";
    std::cout << exePath.string() << "\n\n";

    std::cout << "Looking for config:\n";
    std::cout << configPath.string() << "\n\n";

    Config config;

    // --------------------------------------------------------
    // Load config
    // --------------------------------------------------------

    if (!LoadConfig(configPath.string(), config))
        return 1;

    std::cout << "Configuration loaded.\n\n";

    std::cout << "FOWM:\n"
        << config.fowmPath << "\n\n";

    std::cout << "FOCWM:\n"
        << config.focwmPath << "\n\n";

    std::cout << "GROUPS:\n"
        << config.groupsPath << "\n\n";

    // --------------------------------------------------------
    // Check files
    // --------------------------------------------------------

    if (!std::filesystem::exists(config.fowmPath))
    {
        std::cout << "ERROR: FOWM file does not exist.\n";
        return 1;
    }

    if (!std::filesystem::exists(config.groupsPath))
    {
        std::cout << "ERROR: Groups file does not exist.\n";
        return 1;
    }

    // --------------------------------------------------------
    // Load group definitions
    // --------------------------------------------------------

    std::map<std::string, int> groups =
        LoadGroupDefinitions(config.groupsPath);

    if (groups.empty())
    {
        std::cout << "ERROR: No GROUP definitions found.\n";
        return 1;
    }

    std::cout << "Loaded "
        << groups.size()
        << " group definitions.\n\n";

    // --------------------------------------------------------
    // Ask for group
    // --------------------------------------------------------

    std::string targetGroup;

    std::cout << "Enter GROUP name to remove:\n";
    std::cout << "> ";

    std::getline(std::cin, targetGroup);

    if (groups.find(targetGroup) == groups.end())
    {
        std::cout << "\nERROR: GROUP not found:\n";
        std::cout << targetGroup << "\n";
        return 1;
    }

    int targetId = groups[targetGroup];

    std::cout << "\nSelected group:\n";
    std::cout << "  Name: " << targetGroup << "\n";
    std::cout << "  ID:   " << targetId << "\n";

    // --------------------------------------------------------
    // Read FOWM
    // --------------------------------------------------------

    std::ifstream input(config.fowmPath);

    if (!input.is_open())
    {
        std::cout << "\nERROR: Could not open FOWM.\n";
        return 1;
    }

    std::vector<std::string> lines;

    std::string line;

    int cellsFound = 0;
    int entriesRemoved = 0;

    while (std::getline(input, line))
    {
        std::vector<std::string> fields;

        std::stringstream ss(line);
        std::string field;

        while (std::getline(ss, field, '|'))
            fields.push_back(field);

        // FOWM encounter groups are field 3.
        if (fields.size() >= 4)
        {
            std::vector<Encounter> encounters =
                ParseEncounters(fields[3]);

            std::vector<Encounter> remaining;

            bool found = false;

            for (const Encounter& encounter : encounters)
            {
                if (encounter.groupName == targetGroup)
                {
                    found = true;
                    entriesRemoved++;
                }
                else
                {
                    remaining.push_back(encounter);
                }
            }

            if (found)
            {
                cellsFound++;

                std::cout
                    << "Found in cell: "
                    << fields[0]
                    << "\n";

                fields[3] =
                    BuildEncounterField(remaining);

                line.clear();

                for (size_t i = 0;
                    i < fields.size();
                    i++)
                {
                    if (i > 0)
                        line += "|";

                    line += fields[i];
                }
            }
        }

        lines.push_back(line);
    }

    input.close();

    // --------------------------------------------------------
    // Results
    // --------------------------------------------------------

    std::cout
        << "\n========================================\n"
        << " Search complete\n"
        << "========================================\n";

    std::cout
        << "Cells affected: "
        << cellsFound
        << "\n";

    std::cout
        << "Entries removed: "
        << entriesRemoved
        << "\n";

    if (cellsFound == 0)
    {
        std::cout << "\nNothing to remove.\n";
        return 0;
    }

    // --------------------------------------------------------
    // Confirm
    // --------------------------------------------------------

    std::string answer;

    std::cout
        << "\nRemove "
        << targetGroup
        << " from the FOWM? (y/n): ";

    std::getline(std::cin, answer);

    if (answer != "y" && answer != "Y")
    {
        std::cout << "\nOperation cancelled.\n";
        return 0;
    }

    // --------------------------------------------------------
    // Backup
    // --------------------------------------------------------

    std::string backupPath =
        config.fowmPath + ".backup";

    try
    {
        std::filesystem::copy_file(
            config.fowmPath,
            backupPath,
            std::filesystem::copy_options::overwrite_existing
        );
    }
    catch (const std::exception& e)
    {
        std::cout
            << "\nERROR: Could not create backup.\n";

        std::cout << e.what() << "\n";

        return 1;
    }

    std::cout
        << "\nBackup created:\n"
        << backupPath
        << "\n";

    // --------------------------------------------------------
    // Write modified FOWM
    // --------------------------------------------------------

    std::ofstream output(
        config.fowmPath,
        std::ios::trunc
    );

    if (!output.is_open())
    {
        std::cout
            << "\nERROR: Could not write FOWM.\n";

        return 1;
    }

    for (const std::string& modifiedLine : lines)
    {
        output << modifiedLine << "\n";
    }

    output.close();

    // --------------------------------------------------------
    // Final status
    // --------------------------------------------------------

    std::cout
        << "\n========================================\n"
        << " SUCCESS\n"
        << "========================================\n";

    std::cout
        << "Removed "
        << entriesRemoved
        << " entries from "
        << cellsFound
        << " cells.\n\n";

    std::cout
        << "Modified FOWM:\n"
        << config.fowmPath
        << "\n\n";

    std::cout
        << "The World Editor can now open the modified FOWM.\n";

    std::cout
        << "\nThe FOCWM is expected at:\n"
        << config.focwmPath
        << "\n";

    if (std::filesystem::exists(config.focwmPath))
    {
        std::cout
            << "\nExisting FOCWM detected.\n"
            << "The World Editor should regenerate/update it when\n"
            << "the FOWM is opened/saved.\n";
    }
    else
    {
        std::cout
            << "\nNo FOCWM currently exists at that path.\n";
    }

    return 0;
}