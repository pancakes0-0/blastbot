#pragma once

namespace blastbot::utils 
{
	class c_display_utils {
	public:
		void capture_display ( );

		uint32_t get_color_at_coordinate ( int coord_x, int coord_y );

		void get_cursor_coordinate ( );

		int cursor_coord_x = { };
		int cursor_coord_y = { };
	};
	inline auto g_display_utils = std::make_unique<c_display_utils> ( );
}