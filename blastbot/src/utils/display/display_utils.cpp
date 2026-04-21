#include "display_utils.h"
namespace blastbot::utils 
{
	void c_display_utils::capture_display ( ) {

	}

	uint32_t c_display_utils::get_color_at_coordinate ( int coord_x, int coord_y ) {
		HDC screen_dc = GetDC ( nullptr );
		if ( !screen_dc ) return 0;

		COLORREF color = GetPixel ( screen_dc, coord_x, coord_y );
		ReleaseDC ( nullptr, screen_dc );

		if ( color == CLR_INVALID ) return 0;

		/*pack as 0xRRGGBB*/
		return ( ( uint32_t )GetRValue ( color ) << 16 ) | ( ( uint32_t )GetGValue ( color ) << 8 ) | ( ( uint32_t )GetBValue ( color ) );
	}

	void c_display_utils::get_cursor_coordinate ( ) {
		POINT cursor_pos = { };
		if ( GetCursorPos ( &cursor_pos ) ) {
			cursor_coord_x = cursor_pos.x;
			cursor_coord_y = cursor_pos.y;
		}
	}
}


