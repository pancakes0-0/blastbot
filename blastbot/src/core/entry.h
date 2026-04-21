#pragma once

namespace blastbot::core 
{
	class c_entry {
	public:
		void entry ( );
	};
	inline auto g_entry = std::make_unique<c_entry> ( );
}