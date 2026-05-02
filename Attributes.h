#pragma once

namespace Attributes {
	namespace Ragebot {
		namespace Ragebot {
			inline bool IsEnabled = true;
			inline int SelectedWeaponIndex = 0;
		}
		namespace Antiaim {
			inline bool IsEnabled = false;
		}
	}

	namespace Legitbot {
		inline bool IsTriggerbotEnabled = false;
		inline bool IsTriggerbotTeamCheckEnabled = true;
		inline int TriggerbotDelay = 20;
	}

	namespace ESP {
		namespace ESP {
			inline bool IsEnemyEnabled = true;
			inline bool isTeammateEnabled = false;
			//inline bool VisibleOnly = false;
		}
		namespace Visuals {
			inline bool RemoveFlashbang = true;
			inline bool RemoveSmoke = true;
		}
		namespace World {
			inline bool IsEnabled = true;

		}
	}

	inline bool IsESPEnabled = true;
	inline bool IsESPEnemyEnabled = true;
	inline bool IsESPTeammateEnabled = false;

	inline bool IsESPEnemyBoxEnabled = false;
	inline bool IsESPEnemyHealthBarEnabled = false;
	inline bool IsESPEnemyGlowEnabled = false;

	inline bool IsESPTeammateBoxEnabled = false;
	inline bool IsESPTeammateHealthBarEnabled = false;
	inline bool IsESPTeammateGlowEnabled = false;

	inline ColorRGBAStruct ESPEnemyBoxColor = { 255.0f, 255.0f, 255.0f, 255.0f };
	inline ColorRGBAStruct ESPEnemyHealthBarColor1 = { 0.0f, 255.0f, 0.0f, 255.0f };
	inline ColorRGBAStruct ESPEnemyHealthBarColor2 = { 255.0f, 0.0f, 0.0f, 255.0f };
	inline ColorRGBAStruct ESPEnemyGlowColor = { 255.0f, 125.0f, 0.0f, 255.0f };

	inline ColorRGBAStruct ESPTeammateBoxColor = { 255.0f, 255.0f, 255.0f, 255.0f };
	inline ColorRGBAStruct ESPTeammateHealthBarColor1 = { 0.0f, 255.0f, 0.0f, 255.0f };
	inline ColorRGBAStruct ESPTeammateHealthBarColor2 = { 255.0f, 0.0f, 0.0f, 255.0f };
	inline ColorRGBAStruct ESPTeammateGlowColor = { 255.0f, 125.0f, 0.0f, 255.0f };

	inline bool IsBunnyhopEnabled = false;
}

//namespace Attributes {
//	inline bool IsRagebotEnabled = true;
//
//	inline bool IsLegitbotEnabled = true;
//	inline bool IsTriggerbotEnabled = false;
//	inline bool IsTriggerbotTeamCheckEnabled = true;
//	inline int TriggerbotDelay = 20;
//
//	inline bool IsESPEnabled = true;
//	inline bool IsESPEnemyEnabled = true;
//	inline bool IsESPTeammateEnabled = false;
//
//	inline bool IsESPEnemyBoxEnabled = false;
//	inline bool IsESPEnemyHealthBarEnabled = false;
//	inline bool IsESPEnemyGlowEnabled = false;
//
//	inline bool IsESPTeammateBoxEnabled = false;
//	inline bool IsESPTeammateHealthBarEnabled = false;
//	inline bool IsESPTeammateGlowEnabled = false;
//
//	inline ColorRGBAStruct ESPEnemyBoxColor = { 255.0f, 255.0f, 255.0f, 255.0f };
//	inline ColorRGBAStruct ESPEnemyHealthBarColor1 = { 0.0f, 255.0f, 0.0f, 255.0f };
//	inline ColorRGBAStruct ESPEnemyHealthBarColor2 = { 255.0f, 0.0f, 0.0f, 255.0f };
//	inline ColorRGBAStruct ESPEnemyGlowColor = { 255.0f, 125.0f, 0.0f, 255.0f };
//
//	inline ColorRGBAStruct ESPTeammateBoxColor = { 255.0f, 255.0f, 255.0f, 255.0f };
//	inline ColorRGBAStruct ESPTeammateHealthBarColor1 = { 0.0f, 255.0f, 0.0f, 255.0f };
//	inline ColorRGBAStruct ESPTeammateHealthBarColor2 = { 255.0f, 0.0f, 0.0f, 255.0f };
//	inline ColorRGBAStruct ESPTeammateGlowColor = { 255.0f, 125.0f, 0.0f, 255.0f };
//
//	inline bool IsBunnyhopEnabled = false;
//}