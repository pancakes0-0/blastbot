#include "logging.h"

bool blastbot::utils::c_logging::init ( const char* title ) {
    AllocConsole ( );
    SetConsoleTitleA ( title );

    handle = GetStdHandle ( STD_OUTPUT_HANDLE );
    return handle != INVALID_HANDLE_VALUE;
}

void blastbot::utils::c_logging::shutdown ( ) {
    FreeConsole ( );

    handle = INVALID_HANDLE_VALUE;
}