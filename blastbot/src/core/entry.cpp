#include "entry.h"
#include <utils/logging/logging.h>
#include <utils/display/display_utils.h>
void blastbot::core::c_entry::entry ( ) {
	blastbot::utils::g_display_utils->get_cursor_coordinate ( );
	int coord_x = blastbot::utils::g_display_utils->cursor_coord_x;
	int coord_y = blastbot::utils::g_display_utils->cursor_coord_y;

#ifdef _DEBUG
	blastbot::utils::g_logging->init ( "Blast Bot" );
	blastbot::utils::g_logging->set_color ( foreground_intense_yellow );

	blastbot::utils::g_logging->log_prefixed ( "[Entry] ", foreground_white, "Logging Init" );
	blastbot::utils::g_logging->log ( "X {} Y {}", coord_x, coord_y );
	char hex_str [ 9 ];
	uint32_t hex_color = blastbot::utils::g_display_utils->get_color_at_coordinate ( coord_x, coord_y );
	fmt_detail::u32_to_hex8 ( hex_color, hex_str );
	blastbot::utils::g_logging->log ( "Color at ({}, {}) -> #{}", coord_x, coord_y, ( const char* )hex_str + 2 );
	blastbot::utils::g_logging->pop_color ( );
#endif



}
