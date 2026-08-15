#include <windows.h>

#include <cctype>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>
#include <algorithm>
#include <filesystem>
#include <map>

// ============================================================
// Data
// ============================================================

struct Config
{
    std::string fowmPath;
    std::string focwmPath;
    std::string groupsPath;
};

struct GroupDefinition
{
    std::string name;
    int id;
};

struct EncounterAssignment
{
    std::string groupIdText;
    int groupId;
    std::string groupName;
    int quantity;
};

struct WorldCell
{
    std::string coordinate;

    // Original complete line from worldmap.fowm.
    std::string originalLine;

    // Parsed encounter assignments.
    std::vector<EncounterAssignment> encounters;
};

// ============================================================
// Globals
// ============================================================

Config g_Config;

std::vector<GroupDefinition> g_Groups;
std::vector<WorldCell> g_Cells;

HWND g_GroupList = nullptr;
HWND g_CellList = nullptr;
HWND g_EncounterList = nullptr;

HWND g_GroupSearch = nullptr;

HWND g_GroupInfo = nullptr;
HWND g_CellInfo = nullptr;
HWND g_Status = nullptr;

HWND g_AddButton = nullptr;
HWND g_RemoveButton = nullptr;
HWND g_MinusButton = nullptr;
HWND g_PlusButton = nullptr;
HWND g_ReloadButton = nullptr;
HWND g_SaveButton = nullptr;

HWND g_AddEncounterWindow = nullptr;
HWND g_AddEncounterList = nullptr;
HWND g_hMainWnd = nullptr;

int g_AddEncounterCell = -1;

int g_SelectedGroup = -1;
int g_SelectedCell = -1;
int g_SelectedEncounter = -1;

bool g_UnsavedChanges = false;

// ============================================================
// Forward declarations
// ============================================================

void MarkModified();
void RefreshCurrentCell();

// ============================================================
// Save helpers
// ============================================================

std::string BuildEncounterField(
    const WorldCell& cell)
{
    std::string result;

    for (size_t i = 0;
        i < cell.encounters.size();
        i++)
    {
        const EncounterAssignment& encounter =
            cell.encounters[i];

        if (i > 0)
            result += ",";

        result += encounter.groupName;
        result += ":";
        result += std::to_string(
            encounter.quantity
        );
    }

    return result;
}

std::string ReplaceEncounterField(
    const std::string& originalLine,
    const std::string& encounterField)
{
    // .fowm cell format:
    //
    // Coordinate|Field1|Field2|EncounterField

    size_t pipe1 =
        originalLine.find('|');

    if (pipe1 == std::string::npos)
        return originalLine;

    size_t pipe2 =
        originalLine.find('|', pipe1 + 1);

    if (pipe2 == std::string::npos)
        return originalLine;

    size_t pipe3 =
        originalLine.find('|', pipe2 + 1);

    if (pipe3 == std::string::npos)
        return originalLine;

    return originalLine.substr(
        0,
        pipe3 + 1
    ) + encounterField;
}

bool SaveWorldMap()
{
    std::filesystem::path sourcePath =
        g_Config.fowmPath;

    std::filesystem::path tempPath =
        sourcePath;

    tempPath += ".tmp";

    std::ifstream input(
        sourcePath
    );

    if (!input.is_open())
        return false;

    std::ofstream output(
        tempPath,
        std::ios::trunc
    );

    if (!output.is_open())
        return false;

    std::string line;

    while (std::getline(input, line))
    {
        std::string outputLine =
            line;

        // Find the coordinate.
        size_t pipe =
            line.find('|');

        if (pipe != std::string::npos)
        {
            std::string coordinate =
                line.substr(
                    0,
                    pipe
                );

            // Look for a cell with this coordinate.
            for (WorldCell& cell :
                g_Cells)
            {
                if (cell.coordinate != coordinate)
                    continue;

                std::string encounterField =
                    BuildEncounterField(cell);

                outputLine =
                    ReplaceEncounterField(
                        line,
                        encounterField
                    );

                // Keep our in-memory original line
                // synchronized with what was saved.
                cell.originalLine =
                    outputLine;

                break;
            }
        }

        output << outputLine << "\n";
    }

    input.close();
    output.close();

    if (!output)
    {
        std::error_code error;
        std::filesystem::remove(
            tempPath,
            error
        );

        return false;
    }

    // Replace the original file.
    std::error_code error;

    std::filesystem::remove(
        sourcePath,
        error
    );

    error.clear();

    std::filesystem::rename(
        tempPath,
        sourcePath,
        error
    );

    if (error)
    {
        // Try to clean up the temporary file.
        std::error_code cleanupError;

        std::filesystem::remove(
            tempPath,
            cleanupError
        );

        return false;
    }

    return true;
}

// ============================================================
// Helpers
// ============================================================

std::string Trim(const std::string& value)
{
    size_t start =
        value.find_first_not_of(" \t\r\n");

    if (start == std::string::npos)
        return "";

    size_t end =
        value.find_last_not_of(" \t\r\n");

    return value.substr(
        start,
        end - start + 1
    );
}

std::vector<std::string> Split(
    const std::string& value,
    char delimiter)
{
    std::vector<std::string> result;

    std::stringstream ss(value);
    std::string item;

    while (std::getline(ss, item, delimiter))
        result.push_back(item);

    return result;
}

std::string GroupNameFromId(int id)
{
    for (const GroupDefinition& group : g_Groups)
    {
        if (group.id == id)
            return group.name;
    }

    return "GROUP_UNKNOWN";
}

int GroupIdFromName(
    const std::string& name)
{
    for (const GroupDefinition& group : g_Groups)
    {
        if (group.name == name)
            return group.id;
    }

    return -1;
}

void AddGroupToCell(int groupIndex, int cellIndex)
{
    if (groupIndex < 0 ||
        groupIndex >= static_cast<int>(g_Groups.size()))
        return;

    if (cellIndex < 0 ||
        cellIndex >= static_cast<int>(g_Cells.size()))
        return;

    GroupDefinition& group =
        g_Groups[groupIndex];

    WorldCell& cell =
        g_Cells[cellIndex];

    // Already present -> increase quantity.
    for (EncounterAssignment& encounter :
        cell.encounters)
    {
        if (encounter.groupName == group.name)
        {
            encounter.quantity++;

            MarkModified();
            RefreshCurrentCell();

            return;
        }
    }

    // New encounter.
    EncounterAssignment encounter;

    encounter.groupName =
        group.name;

    encounter.groupId =
        group.id;

    encounter.groupIdText =
        std::to_string(group.id);

    encounter.quantity = 1;

    cell.encounters.push_back(
        encounter
    );

    MarkModified();
    RefreshCurrentCell();
}

LRESULT CALLBACK AddEncounterWndProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        g_AddEncounterList =
            CreateWindowExA(
                WS_EX_CLIENTEDGE,
                "LISTBOX",
                "",
                WS_CHILD |
                WS_VISIBLE |
                WS_VSCROLL |
                LBS_NOTIFY |
                LBS_NOINTEGRALHEIGHT,
                10,
                10,
                360,
                420,
                hwnd,
                nullptr,
                GetModuleHandleA(nullptr),
                nullptr
            );

        for (const GroupDefinition& group :
            g_Groups)
        {
            std::string text =
                group.name +
                " [" +
                std::to_string(group.id) +
                "]";

            SendMessageA(
                g_AddEncounterList,
                LB_ADDSTRING,
                0,
                reinterpret_cast<LPARAM>(
                    text.c_str()
                    )
            );
        }

        CreateWindowExA(
            0,
            "BUTTON",
            "Add",
            WS_CHILD |
            WS_VISIBLE |
            BS_DEFPUSHBUTTON,
            100,
            445,
            100,
            30,
            hwnd,
            reinterpret_cast<HMENU>(1),
            GetModuleHandleA(nullptr),
            nullptr
        );

        CreateWindowExA(
            0,
            "BUTTON",
            "Cancel",
            WS_CHILD |
            WS_VISIBLE,
            215,
            445,
            100,
            30,
            hwnd,
            reinterpret_cast<HMENU>(2),
            GetModuleHandleA(nullptr),
            nullptr
        );

        return 0;
    }

    case WM_COMMAND:
    {
        if (LOWORD(wParam) == 1)
        {
            int selected =
                static_cast<int>(
                    SendMessageA(
                        g_AddEncounterList,
                        LB_GETCURSEL,
                        0,
                        0
                    )
                    );

            if (selected == LB_ERR)
            {
                MessageBoxA(
                    hwnd,
                    "Select an encounter group.",
                    "Add Encounter",
                    MB_OK | MB_ICONINFORMATION
                );

                return 0;
            }

            AddGroupToCell(
                selected,
                g_AddEncounterCell
            );

            DestroyWindow(hwnd);

            return 0;
        }

        if (LOWORD(wParam) == 2)
        {
            DestroyWindow(hwnd);
            return 0;
        }

        break;
    }

    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        g_AddEncounterWindow = nullptr;
        g_AddEncounterList = nullptr;

        if (g_hMainWnd)
        {
            EnableWindow(
                g_hMainWnd,
                TRUE
            );

            SetForegroundWindow(
                g_hMainWnd
            );
        }

        return 0;
    }

    return DefWindowProcA(
        hwnd,
        msg,
        wParam,
        lParam
    );
}

void OpenAddEncounterWindow()
{
    if (g_SelectedCell < 0 ||
        g_SelectedCell >=
        static_cast<int>(g_Cells.size()))
    {
        MessageBoxA(
            g_hMainWnd,
            "Select a cell first.",
            "Add Encounter",
            MB_OK | MB_ICONINFORMATION
        );

        return;
    }

    if (g_AddEncounterWindow)
    {
        SetForegroundWindow(
            g_AddEncounterWindow
        );

        return;
    }

    g_AddEncounterCell =
        g_SelectedCell;

    const char* className =
        "EncounterAddWindowClass";

    WNDCLASSA wc = {};
    wc.lpfnWndProc =
        AddEncounterWndProc;
    wc.hInstance =
        GetModuleHandleA(nullptr);
    wc.hCursor =
        LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground =
        reinterpret_cast<HBRUSH>(
            COLOR_WINDOW + 1
            );
    wc.lpszClassName =
        className;

    RegisterClassA(&wc);

    g_AddEncounterWindow =
        CreateWindowExA(
            WS_EX_DLGMODALFRAME,
            className,
            "Add Encounter",
            WS_CAPTION |
            WS_SYSMENU |
            WS_POPUP,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            400,
            530,
            g_hMainWnd,
            nullptr,
            GetModuleHandleA(nullptr),
            nullptr
        );

    if (!g_AddEncounterWindow)
        return;

    EnableWindow(
        g_hMainWnd,
        FALSE
    );

    ShowWindow(
        g_AddEncounterWindow,
        SW_SHOW
    );

    UpdateWindow(
        g_AddEncounterWindow
    );
}

// ============================================================
// Config
// ============================================================

bool LoadConfig(
    const std::string& configPath,
    Config& config)
{
    std::ifstream file(configPath);

    if (!file.is_open())
        return false;

    std::string line;

    while (std::getline(file, line))
    {
        line = Trim(line);

        if (line.empty())
            continue;

        if (line[0] == '#')
            continue;

        size_t equals =
            line.find('=');

        if (equals == std::string::npos)
            continue;

        std::string key =
            Trim(line.substr(0, equals));

        std::string value =
            Trim(line.substr(equals + 1));

        if (key == "FOWM_PATH")
            config.fowmPath = value;
        else if (key == "FOCWM_PATH")
            config.focwmPath = value;
        else if (key == "GROUPS_PATH")
            config.groupsPath = value;
    }

    return
        !config.fowmPath.empty() &&
        !config.groupsPath.empty();
}

// ============================================================
// Load group definitions
// ============================================================

bool LoadGroups()
{
    g_Groups.clear();

    std::ifstream file(
        g_Config.groupsPath
    );

    if (!file.is_open())
        return false;

    std::regex groupRegex(
        R"(#define\s+(GROUP_[A-Za-z0-9_]+)\s+\((\d+)\))"
    );

    std::string line;

    while (std::getline(file, line))
    {
        std::smatch match;

        if (std::regex_search(
            line,
            match,
            groupRegex))
        {
            GroupDefinition group;

            group.name =
                match[1].str();

            group.id =
                std::stoi(match[2].str());

            g_Groups.push_back(group);
        }
    }

    std::sort(
        g_Groups.begin(),
        g_Groups.end(),
        [](const GroupDefinition& a,
            const GroupDefinition& b)
        {
            return a.name < b.name;
        }
    );

    return !g_Groups.empty();
}

// ============================================================
// Parse encounter field
//
// worldmap.fowm:
// ...
// |2:1,4:1,42:4,43:4|
//
// We convert IDs to names for the GUI.
// ============================================================

std::vector<EncounterAssignment>
ParseEncounters(const std::string& field)
{
    std::vector<EncounterAssignment> encounters;

    std::stringstream ss(field);
    std::string entry;

    while (std::getline(ss, entry, ','))
    {
        size_t colon = entry.find(':');

        if (colon == std::string::npos)
            continue;

        std::string groupName =
            Trim(entry.substr(0, colon));

        std::string quantityText =
            Trim(entry.substr(colon + 1));

        if (groupName.empty())
            continue;

        try
        {
            int quantity =
                std::stoi(quantityText);

            EncounterAssignment encounter;

            encounter.groupName =
                groupName;

            encounter.quantity =
                quantity;

            encounter.groupId =
                GroupIdFromName(groupName);

            encounter.groupIdText =
                std::to_string(
                    encounter.groupId
                );

            encounters.push_back(
                encounter
            );
        }
        catch (...)
        {
            continue;
        }
    }

    return encounters;
}

// ============================================================
// Load worldmap.fowm
// ============================================================

bool LoadWorldMap()
{
    g_Cells.clear();

    std::filesystem::path debugPath =
        std::filesystem::path(g_Config.fowmPath).parent_path()
        / "EncounterEditor_debug.txt";

    std::ofstream debug(debugPath);

    if (!debug.is_open())
        return false;

    debug << "========================================\n";
    debug << "Encounter Editor FOWM Debug\n";
    debug << "========================================\n\n";

    debug << "FOWM path:\n";
    debug << g_Config.fowmPath << "\n\n";

    std::ifstream file(
        g_Config.fowmPath
    );

    if (!file.is_open())
    {
        debug << "ERROR: Could not open FOWM.\n";
        return false;
    }

    debug << "FOWM opened successfully.\n\n";

    std::string line;

    int lineNumber = 0;
    int validLines = 0;
    int encounterLines = 0;
    int totalEncounters = 0;

    while (std::getline(file, line))
    {
        lineNumber++;

        std::vector<std::string> fields =
            Split(line, '|');

        if (fields.size() < 4)
        {
            continue;
        }

        validLines++;

        // Show the first 20 valid FOWM lines.
        if (validLines <= 20)
        {
            debug << "----------------------------------------\n";
            debug << "LINE " << lineNumber << "\n";
            debug << "Coordinate: [" << fields[0] << "]\n";
            debug << "Field 1:   [" << fields[1] << "]\n";
            debug << "Field 2:   [" << fields[2] << "]\n";
            debug << "Field 3:   [" << fields[3] << "]\n";

            std::vector<EncounterAssignment> parsed =
                ParseEncounters(fields[3]);

            debug << "Parsed encounters: "
                << parsed.size()
                << "\n";

            for (const EncounterAssignment& encounter :
                parsed)
            {
                debug
                    << "  ID="
                    << encounter.groupId
                    << " Name="
                    << encounter.groupName
                    << " Quantity="
                    << encounter.quantity
                    << "\n";
            }
        }

        WorldCell cell;

        cell.coordinate =
            Trim(fields[0]);

        cell.originalLine =
            line;

        cell.encounters =
            ParseEncounters(fields[3]);

        if (!cell.encounters.empty())
        {
            encounterLines++;

            totalEncounters +=
                static_cast<int>(
                    cell.encounters.size()
                    );

            g_Cells.push_back(cell);
        }
    }

    debug << "\n========================================\n";
    debug << "SUMMARY\n";
    debug << "========================================\n";

    debug << "Total lines: "
        << lineNumber
        << "\n";

    debug << "Valid lines: "
        << validLines
        << "\n";

    debug << "Cells with encounters: "
        << encounterLines
        << "\n";

    debug << "Total encounter entries: "
        << totalEncounters
        << "\n";

    debug.close();

    return true;
}
// ============================================================
// Listbox helpers
// ============================================================

void AddListItem(
    HWND list,
    const std::string& text)
{
    SendMessageA(
        list,
        LB_ADDSTRING,
        0,
        reinterpret_cast<LPARAM>(
            text.c_str()
            )
    );
}

// ============================================================
// Group statistics
// ============================================================

void UpdateGroupInfo()
{
    if (g_SelectedGroup < 0 ||
        g_SelectedGroup >=
        static_cast<int>(g_Groups.size()))
    {
        SetWindowTextA(
            g_GroupInfo,
            "No group selected"
        );

        return;
    }

    const GroupDefinition& group =
        g_Groups[g_SelectedGroup];

    int cellCount = 0;
    int totalQuantity = 0;

    int minQuantity = 0;
    int maxQuantity = 0;

    for (const WorldCell& cell : g_Cells)
    {
        for (const EncounterAssignment& encounter :
            cell.encounters)
        {
            if (encounter.groupId != group.id)
                continue;

            cellCount++;

            totalQuantity +=
                encounter.quantity;

            if (minQuantity == 0 ||
                encounter.quantity < minQuantity)
            {
                minQuantity =
                    encounter.quantity;
            }

            if (encounter.quantity > maxQuantity)
            {
                maxQuantity =
                    encounter.quantity;
            }

            break;
        }
    }

    std::string text =
        group.name +
        "    ID: " +
        std::to_string(group.id) +
        "    Cells: " +
        std::to_string(cellCount) +
        "    Total quantity: " +
        std::to_string(totalQuantity) +
        "    Min: " +
        std::to_string(minQuantity) +
        "    Max: " +
        std::to_string(maxQuantity);

    SetWindowTextA(
        g_GroupInfo,
        text.c_str()
    );
}

// ============================================================
// Find cells containing group
// ============================================================

std::vector<int> FindCellsForGroup(
    int groupId)
{
    std::vector<int> result;

    for (size_t i = 0;
        i < g_Cells.size();
        i++)
    {
        for (const EncounterAssignment& encounter :
            g_Cells[i].encounters)
        {
            if (encounter.groupId == groupId)
            {
                result.push_back(
                    static_cast<int>(i)
                );

                break;
            }
        }
    }

    return result;
}

// ============================================================
// Populate group list
// ============================================================

void PopulateGroups()
{
    SendMessageA(
        g_GroupList,
        LB_RESETCONTENT,
        0,
        0
    );

    std::string search;

    char buffer[256];

    GetWindowTextA(
        g_GroupSearch,
        buffer,
        sizeof(buffer)
    );

    search = buffer;

    std::transform(
        search.begin(),
        search.end(),
        search.begin(),
        [](unsigned char c)
        {
            return static_cast<char>(
                std::tolower(c)
                );
        }
    );

    for (const GroupDefinition& group :
        g_Groups)
    {
        std::string lowerName =
            group.name;

        std::transform(
            lowerName.begin(),
            lowerName.end(),
            lowerName.begin(),
            [](unsigned char c)
            {
                return static_cast<char>(
                    std::tolower(c)
                    );
            }
        );

        if (!search.empty() &&
            lowerName.find(search) ==
            std::string::npos)
        {
            continue;
        }

        std::string text =
            group.name +
            "    [" +
            std::to_string(group.id) +
            "]";

        AddListItem(
            g_GroupList,
            text
        );
    }
}

// ============================================================
// Populate cells
// ============================================================

void PopulateCellsForGroup(
    int groupId)
{
    SendMessageA(
        g_CellList,
        LB_RESETCONTENT,
        0,
        0
    );

    SendMessageA(
        g_EncounterList,
        LB_RESETCONTENT,
        0,
        0
    );

    g_SelectedCell = -1;
    g_SelectedEncounter = -1;

    std::vector<int> cells =
        FindCellsForGroup(groupId);

    for (int cellIndex : cells)
    {
        const WorldCell& cell =
            g_Cells[cellIndex];

        int quantity = 0;

        for (const EncounterAssignment& encounter :
            cell.encounters)
        {
            if (encounter.groupId == groupId)
            {
                quantity =
                    encounter.quantity;

                break;
            }
        }

        std::string text =
            cell.coordinate +
            "    x" +
            std::to_string(quantity);

        AddListItem(
            g_CellList,
            text
        );
    }

    UpdateGroupInfo();
}

// ============================================================
// Populate cell encounters
// ============================================================

void PopulateEncounters(
    int cellIndex)
{
    SendMessageA(
        g_EncounterList,
        LB_RESETCONTENT,
        0,
        0
    );

    g_SelectedEncounter = -1;

    if (cellIndex < 0 ||
        cellIndex >=
        static_cast<int>(g_Cells.size()))
    {
        return;
    }

    const WorldCell& cell =
        g_Cells[cellIndex];

    std::string title =
        "CELL " +
        cell.coordinate;

    SetWindowTextA(
        g_CellInfo,
        title.c_str()
    );

    for (const EncounterAssignment& encounter :
        cell.encounters)
    {
        std::string text =
            encounter.groupName +
            "    x" +
            std::to_string(
                encounter.quantity
            );

        AddListItem(
            g_EncounterList,
            text
        );
    }
}

// ============================================================
// Mark modified
// ============================================================

void MarkModified()
{
    g_UnsavedChanges = true;

    SetWindowTextA(
        g_Status,
        "UNSAVED CHANGES"
    );

    EnableWindow(
        g_SaveButton,
        TRUE
    );
}

// ============================================================
// Refresh current cell
// ============================================================

void RefreshCurrentCell()
{
    int cellIndex = g_SelectedCell;
    int encounterIndex = g_SelectedEncounter;
    int groupIndex = g_SelectedGroup;

    if (cellIndex < 0 ||
        cellIndex >= static_cast<int>(g_Cells.size()))
    {
        return;
    }

    // --------------------------------------------------------
    // Refresh the Cells list first.
    // This function clears the encounter list.
    // --------------------------------------------------------

    if (groupIndex >= 0 &&
        groupIndex < static_cast<int>(g_Groups.size()))
    {
        int groupId =
            g_Groups[groupIndex].id;

        PopulateCellsForGroup(groupId);

        // Restore selected group.
        g_SelectedGroup =
            groupIndex;

        // Find the cell again in the refreshed list.
        std::vector<int> cells =
            FindCellsForGroup(groupId);

        for (size_t i = 0;
            i < cells.size();
            i++)
        {
            if (cells[i] == cellIndex)
            {
                g_SelectedCell =
                    cellIndex;

                SendMessageA(
                    g_CellList,
                    LB_SETCURSEL,
                    static_cast<WPARAM>(i),
                    0
                );

                break;
            }
        }
    }

    // --------------------------------------------------------
    // NOW populate the Cell Encounters panel.
    // --------------------------------------------------------

    PopulateEncounters(
        cellIndex
    );

    // --------------------------------------------------------
    // Restore selected encounter.
    // --------------------------------------------------------

    if (encounterIndex >= 0 &&
        encounterIndex <
        static_cast<int>(
            g_Cells[cellIndex].encounters.size()))
    {
        g_SelectedCell =
            cellIndex;

        g_SelectedEncounter =
            encounterIndex;

        SendMessageA(
            g_EncounterList,
            LB_SETCURSEL,
            static_cast<WPARAM>(
                encounterIndex
                ),
            0
        );
    }

    UpdateGroupInfo();
}

// ============================================================
// Change quantity
// ============================================================

void ChangeSelectedQuantity(int delta)
{
    if (g_SelectedCell < 0 ||
        g_SelectedCell >=
        static_cast<int>(g_Cells.size()))
    {
        return;
    }

    WorldCell& cell =
        g_Cells[g_SelectedCell];

    if (g_SelectedEncounter < 0 ||
        g_SelectedEncounter >=
        static_cast<int>(cell.encounters.size()))
    {
        return;
    }

    EncounterAssignment& encounter =
        cell.encounters[g_SelectedEncounter];

    int newQuantity =
        encounter.quantity + delta;

    // Do not allow zero.
    if (newQuantity < 1)
        newQuantity = 1;

    encounter.quantity =
        newQuantity;

    MarkModified();

    RefreshCurrentCell();
}

// ============================================================
// Add selected group to current cell
// ============================================================

void AddSelectedGroupToCell()
{
    if (g_SelectedGroup < 0 ||
        g_SelectedGroup >=
        static_cast<int>(g_Groups.size()))
    {
        return;
    }

    if (g_SelectedCell < 0 ||
        g_SelectedCell >=
        static_cast<int>(g_Cells.size()))
    {
        MessageBoxA(
            nullptr,
            "Select a cell first.",
            "Encounter Editor",
            MB_OK | MB_ICONINFORMATION
        );

        return;
    }

    GroupDefinition& group =
        g_Groups[g_SelectedGroup];

    WorldCell& cell =
        g_Cells[g_SelectedCell];

    // If the group already exists,
    // increase its quantity.
    for (EncounterAssignment& encounter :
        cell.encounters)
    {
        if (encounter.groupName ==
            group.name)
        {
            encounter.quantity++;

            MarkModified();
            RefreshCurrentCell();

            return;
        }
    }

    // Group does not exist in this cell.
    EncounterAssignment encounter;

    encounter.groupName =
        group.name;

    encounter.groupId =
        group.id;

    encounter.groupIdText =
        std::to_string(group.id);

    encounter.quantity =
        1;

    cell.encounters.push_back(
        encounter
    );

    MarkModified();
    RefreshCurrentCell();
}

// ============================================================
// Remove selected encounter
// ============================================================

void RemoveSelectedEncounter()
{
    if (g_SelectedCell < 0 ||
        g_SelectedCell >=
        static_cast<int>(g_Cells.size()))
    {
        return;
    }

    WorldCell& cell =
        g_Cells[g_SelectedCell];

    if (g_SelectedEncounter < 0 ||
        g_SelectedEncounter >=
        static_cast<int>(cell.encounters.size()))
    {
        return;
    }

    cell.encounters.erase(
        cell.encounters.begin() +
        g_SelectedEncounter
    );

    g_SelectedEncounter = -1;

    MarkModified();

    // If the removed encounter was the selected
    // group, the cell may disappear from the
    // group's cell list. RefreshCurrentCell()
    // handles that.
    RefreshCurrentCell();
}

// ============================================================
// Reload
// ============================================================

bool ReloadData()
{
    if (!LoadGroups())
        return false;

    if (!LoadWorldMap())
        return false;

    g_SelectedGroup = -1;
    g_SelectedCell = -1;
    g_SelectedEncounter = -1;
    g_UnsavedChanges = false;

    PopulateGroups();

    SendMessageA(
        g_CellList,
        LB_RESETCONTENT,
        0,
        0
    );

    SendMessageA(
        g_EncounterList,
        LB_RESETCONTENT,
        0,
        0
    );

    SetWindowTextA(
        g_GroupInfo,
        "Select a group"
    );

    SetWindowTextA(
        g_CellInfo,
        "Select a cell"
    );

    SetWindowTextA(
        g_Status,
        "Ready"
    );

    EnableWindow(
        g_SaveButton,
        FALSE
    );

    return true;
}

// ============================================================
// IDs
// ============================================================

#define IDC_GROUP_LIST       1001
#define IDC_CELL_LIST        1002
#define IDC_ENCOUNTER_LIST   1003
#define IDC_GROUP_SEARCH     1004

#define IDC_RELOAD           1005
#define IDC_SAVE             1006

#define IDC_MINUS            1007
#define IDC_PLUS             1008
#define IDC_ADD              1009
#define IDC_REMOVE           1010

#define IDC_GROUP_INFO       1011
#define IDC_CELL_INFO        1012
#define IDC_STATUS           1013

// ============================================================
// Window procedure
// ============================================================

LRESULT CALLBACK WindowProc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        HFONT font =
            (HFONT)GetStockObject(
                DEFAULT_GUI_FONT
            );

        // ----------------------------------------------------
        // Groups
        // ----------------------------------------------------

        CreateWindowA(
            "STATIC",
            "Groups",
            WS_CHILD | WS_VISIBLE,
            10, 10,
            280, 25,
            hwnd,
            nullptr,
            nullptr,
            nullptr
        );

        g_GroupSearch =
            CreateWindowExA(
                WS_EX_CLIENTEDGE,
                "EDIT",
                "",
                WS_CHILD | WS_VISIBLE |
                ES_AUTOHSCROLL,
                10, 35,
                280, 25,
                hwnd,
                (HMENU)IDC_GROUP_SEARCH,
                nullptr,
                nullptr
            );

        g_GroupList =
            CreateWindowExA(
                WS_EX_CLIENTEDGE,
                "LISTBOX",
                "",
                WS_CHILD | WS_VISIBLE |
                LBS_NOTIFY |
                WS_VSCROLL,
                10, 65,
                300, 510,
                hwnd,
                (HMENU)IDC_GROUP_LIST,
                nullptr,
                nullptr
            );

        // ----------------------------------------------------
        // Cells
        // ----------------------------------------------------

        CreateWindowA(
            "STATIC",
            "Cells",
            WS_CHILD | WS_VISIBLE,
            325, 10,
            280, 25,
            hwnd,
            nullptr,
            nullptr,
            nullptr
        );

        g_CellList =
            CreateWindowExA(
                WS_EX_CLIENTEDGE,
                "LISTBOX",
                "",
                WS_CHILD | WS_VISIBLE |
                LBS_NOTIFY |
                WS_VSCROLL,
                325, 35,
                300, 540,
                hwnd,
                (HMENU)IDC_CELL_LIST,
                nullptr,
                nullptr
            );

        // ----------------------------------------------------
        // Encounters
        // ----------------------------------------------------

        CreateWindowA(
            "STATIC",
            "Cell Encounters",
            WS_CHILD | WS_VISIBLE,
            640, 10,
            350, 25,
            hwnd,
            nullptr,
            nullptr,
            nullptr
        );

        g_CellInfo =
            CreateWindowA(
                "STATIC",
                "Select a cell",
                WS_CHILD | WS_VISIBLE,
                640, 35,
                350, 25,
                hwnd,
                (HMENU)IDC_CELL_INFO,
                nullptr,
                nullptr
            );

        g_EncounterList =
            CreateWindowExA(
                WS_EX_CLIENTEDGE,
                "LISTBOX",
                "",
                WS_CHILD | WS_VISIBLE |
                LBS_NOTIFY |
                WS_VSCROLL,
                640, 65,
                350, 400,
                hwnd,
                (HMENU)IDC_ENCOUNTER_LIST,
                nullptr,
                nullptr
            );

        // ----------------------------------------------------
        // Quantity buttons
        // ----------------------------------------------------

        g_MinusButton =
            CreateWindowA(
                "BUTTON",
                "-",
                WS_CHILD | WS_VISIBLE |
                BS_PUSHBUTTON,
                640, 480,
                55, 30,
                hwnd,
                (HMENU)IDC_MINUS,
                nullptr,
                nullptr
            );

        g_PlusButton =
            CreateWindowA(
                "BUTTON",
                "+",
                WS_CHILD | WS_VISIBLE |
                BS_PUSHBUTTON,
                705, 480,
                55, 30,
                hwnd,
                (HMENU)IDC_PLUS,
                nullptr,
                nullptr
            );

        g_AddButton =
            CreateWindowA(
                "BUTTON",
                "Add Encounter",
                WS_CHILD | WS_VISIBLE |
                BS_PUSHBUTTON,
                640, 520,
                150, 30,
                hwnd,
                (HMENU)IDC_ADD,
                nullptr,
                nullptr
            );

        g_RemoveButton =
            CreateWindowA(
                "BUTTON",
                "Remove Selected",
                WS_CHILD | WS_VISIBLE |
                BS_PUSHBUTTON,
                800, 520,
                150, 30,
                hwnd,
                (HMENU)IDC_REMOVE,
                nullptr,
                nullptr
            );

        // ----------------------------------------------------
        // Group info
        // ----------------------------------------------------

        g_GroupInfo =
            CreateWindowA(
                "STATIC",
                "Select a group",
                WS_CHILD | WS_VISIBLE,
                10, 585,
                980, 25,
                hwnd,
                (HMENU)IDC_GROUP_INFO,
                nullptr,
                nullptr
            );

        // ----------------------------------------------------
        // Bottom buttons
        // ----------------------------------------------------

        g_ReloadButton =
            CreateWindowA(
                "BUTTON",
                "Reload",
                WS_CHILD | WS_VISIBLE |
                BS_PUSHBUTTON,
                10, 620,
                100, 30,
                hwnd,
                (HMENU)IDC_RELOAD,
                nullptr,
                nullptr
            );

        g_SaveButton =
            CreateWindowA(
                "BUTTON",
                "Save",
                WS_CHILD | WS_VISIBLE |
                BS_PUSHBUTTON,
                120, 620,
                100, 30,
                hwnd,
                (HMENU)IDC_SAVE,
                nullptr,
                nullptr
            );

        g_Status =
            CreateWindowA(
                "STATIC",
                "Loading...",
                WS_CHILD | WS_VISIBLE,
                240, 625,
                750, 25,
                hwnd,
                (HMENU)IDC_STATUS,
                nullptr,
                nullptr
            );

        // ----------------------------------------------------
        // Font
        // ----------------------------------------------------

        HWND controls[] =
        {
            g_GroupSearch,
            g_GroupList,
            g_CellList,
            g_CellInfo,
            g_EncounterList,
            g_MinusButton,
            g_PlusButton,
            g_AddButton,
            g_RemoveButton,
            g_GroupInfo,
            g_ReloadButton,
            g_SaveButton,
            g_Status
        };

        for (HWND control : controls)
        {
            SendMessage(
                control,
                WM_SETFONT,
                (WPARAM)font,
                TRUE
            );
        }

        // ----------------------------------------------------
        // Initial state
        // ----------------------------------------------------

        EnableWindow(
            g_SaveButton,
            FALSE
        );

        if (!ReloadData())
        {
            MessageBoxA(
                hwnd,
                "Could not load the configured FOnline files.",
                "Encounter Editor",
                MB_ICONERROR
            );
        }

        return 0;
    }

    case WM_COMMAND:
    {
        int controlId =
            LOWORD(wParam);

        int notification =
            HIWORD(wParam);

        // ----------------------------------------------------
        // Group search
        // ----------------------------------------------------

        if (controlId == IDC_GROUP_SEARCH &&
            notification == EN_CHANGE)
        {
            PopulateGroups();
            return 0;
        }

        // ----------------------------------------------------
        // Group selected
        // ----------------------------------------------------
        if (controlId == IDC_GROUP_LIST &&
            notification == LBN_SELCHANGE)
        {
            int listIndex =
                static_cast<int>(
                    SendMessage(
                        g_GroupList,
                        LB_GETCURSEL,
                        0,
                        0
                    )
                    );

            if (listIndex == LB_ERR)
                return 0;

            // Get the selected text.
            char buffer[512] = {};

            SendMessageA(
                g_GroupList,
                LB_GETTEXT,
                listIndex,
                reinterpret_cast<LPARAM>(buffer)
            );

            std::string text = buffer;

            // Expected:
            //
            // GROUP_Brahmin    [100]
            //
            // Extract the ID from [100].

            size_t openBracket =
                text.rfind('[');

            size_t closeBracket =
                text.rfind(']');

            if (openBracket == std::string::npos ||
                closeBracket == std::string::npos ||
                closeBracket <= openBracket)
            {
                SetWindowTextA(
                    g_Status,
                    "Could not determine group ID."
                );

                return 0;
            }

            std::string idText =
                text.substr(
                    openBracket + 1,
                    closeBracket - openBracket - 1
                );

            int groupId = -1;

            try
            {
                groupId = std::stoi(idText);
            }
            catch (...)
            {
                SetWindowTextA(
                    g_Status,
                    "Invalid group ID."
                );

                return 0;
            }

            // Find the actual group definition.
            g_SelectedGroup = -1;

            for (size_t i = 0;
                i < g_Groups.size();
                i++)
            {
                if (g_Groups[i].id == groupId)
                {
                    g_SelectedGroup =
                        static_cast<int>(i);

                    break;
                }
            }

            if (g_SelectedGroup == -1)
            {
                SetWindowTextA(
                    g_Status,
                    "Group ID was not found."
                );

                return 0;
            }

            // This is the important part:
            // populate the cells containing this group.
            PopulateCellsForGroup(groupId);

            std::string status =
                "Selected " +
                g_Groups[g_SelectedGroup].name +
                " [" +
                std::to_string(groupId) +
                "]";

            SetWindowTextA(
                g_Status,
                status.c_str()
            );

            return 0;
        }

        
        // ----------------------------------------------------
        // Cell selected
        // ----------------------------------------------------

        if (controlId == IDC_CELL_LIST &&
            notification == LBN_SELCHANGE)
        {
            int listIndex =
                static_cast<int>(
                    SendMessage(
                        g_CellList,
                        LB_GETCURSEL,
                        0,
                        0
                    )
                    );

            if (g_SelectedGroup >= 0 &&
                listIndex >= 0)
            {
                std::vector<int> cells =
                    FindCellsForGroup(
                        g_Groups[
                            g_SelectedGroup
                        ].id
                    );

                if (listIndex <
                    static_cast<int>(
                        cells.size()
                        ))
                {
                    g_SelectedCell =
                        cells[listIndex];

                    PopulateEncounters(
                        g_SelectedCell
                    );
                }
            }

            return 0;
        }

        // ----------------------------------------------------
        // Encounter selected
        // ----------------------------------------------------

        if (controlId == IDC_ENCOUNTER_LIST &&
            notification == LBN_SELCHANGE)
        {
            g_SelectedEncounter =
                static_cast<int>(
                    SendMessage(
                        g_EncounterList,
                        LB_GETCURSEL,
                        0,
                        0
                    )
                    );

            return 0;
        }

        // ----------------------------------------------------
        // Minus
        // ----------------------------------------------------

        if (controlId == IDC_MINUS)
        {
            ChangeSelectedQuantity(-1);
            return 0;
        }

        // ----------------------------------------------------
        // Plus
        // ----------------------------------------------------

        if (controlId == IDC_PLUS)
        {
            ChangeSelectedQuantity(1);
            return 0;
        }

        // ----------------------------------------------------
        // Add selected group
        // ----------------------------------------------------

        if (controlId == IDC_ADD)
        {
            OpenAddEncounterWindow();
            return 0;
        }

        // ----------------------------------------------------
        // Remove encounter
        // ----------------------------------------------------

        if (controlId == IDC_REMOVE)
        {
            RemoveSelectedEncounter();
            return 0;
        }

        // ----------------------------------------------------
        // Reload
        // ----------------------------------------------------

        if (controlId == IDC_RELOAD)
        {
            if (g_UnsavedChanges)
            {
                int result =
                    MessageBoxA(
                        hwnd,
                        "You have unsaved changes.\n\n"
                        "Reload anyway and discard them?",
                        "Reload",
                        MB_YESNO | MB_ICONWARNING
                    );

                if (result != IDYES)
                    return 0;
            }

            if (!ReloadData())
            {
                MessageBoxA(
                    hwnd,
                    "Failed to reload the FOnline files.",
                    "Encounter Editor",
                    MB_ICONERROR
                );
            }

            return 0;
        }

        // ----------------------------------------------------
        // Save
        // ----------------------------------------------------

        if (controlId == IDC_SAVE)
        {
            if (!SaveWorldMap())
            {
                MessageBoxA(
                    hwnd,
                    "Failed to save worldmap.fowm.",
                    "Encounter Editor",
                    MB_OK | MB_ICONERROR
                );

                return 0;
            }

            g_UnsavedChanges = false;

            SetWindowTextA(
                g_Status,
                "Saved successfully"
            );

            EnableWindow(
                g_SaveButton,
                FALSE
            );

            return 0;
        }

        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(
        hwnd,
        message,
        wParam,
        lParam
    );
}

// ============================================================
// WinMain
// ============================================================

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE,
    LPSTR,
    int nCmdShow)
{
    // --------------------------------------------------------
    // Find executable directory
    // --------------------------------------------------------

    char exePathBuffer[MAX_PATH];

    GetModuleFileNameA(
        nullptr,
        exePathBuffer,
        MAX_PATH
    );

    std::filesystem::path exePath(
        exePathBuffer
    );

    std::filesystem::path configPath =
        exePath.parent_path() /
        "EncounterEditor.cfg";

    // --------------------------------------------------------
    // Load configuration
    // --------------------------------------------------------

    if (!LoadConfig(
        configPath.string(),
        g_Config))
    {
        MessageBoxA(
            nullptr,
            "Could not load EncounterEditor.cfg.",
            "FOnline Encounter Editor",
            MB_ICONERROR
        );

        return 1;
    }

    // --------------------------------------------------------
    // Register window
    // --------------------------------------------------------

    const wchar_t CLASS_NAME[] =
        L"FOnlineEncounterEditor";

    WNDCLASSW wc = {};

    wc.lpfnWndProc =
        WindowProc;

    wc.hInstance =
        hInstance;

    wc.lpszClassName =
        CLASS_NAME;

    wc.hCursor =
        LoadCursor(
            nullptr,
            IDC_ARROW
        );

    wc.hbrBackground =
        (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClassW(&wc))
    {
        MessageBoxW(
            nullptr,
            L"Could not register window class.",
            L"FOnline Encounter Editor",
            MB_ICONERROR
        );

        return 1;
    }

    // --------------------------------------------------------
    // Create window
    // --------------------------------------------------------

    g_hMainWnd =
        CreateWindowExW(
            0,
            CLASS_NAME,
            L"FOnline Encounter Editor",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            1020,
            700,
            nullptr,
            nullptr,
            hInstance,
            nullptr
        );

    if (!g_hMainWnd)
    {
        MessageBoxW(
            nullptr,
            L"Could not create main window.",
            L"FOnline Encounter Editor",
            MB_ICONERROR
        );

        return 1;
    }

    ShowWindow(
        g_hMainWnd,
        nCmdShow
    );

    UpdateWindow(g_hMainWnd);

    // --------------------------------------------------------
    // Message loop
    // --------------------------------------------------------

    MSG msg = {};

    while (GetMessage(
        &msg,
        nullptr,
        0,
        0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}