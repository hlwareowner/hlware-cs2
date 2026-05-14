#pragma once

namespace Attributes {
	namespace Ragebot {
		namespace Ragebot {
			inline bool IsEnabled = true;
			inline bool IsWallbangEnabled = true;
			inline bool IsSilentAimEnabled = true;
			inline bool IsAutomaticFireEnabled = true;
			inline bool IsAutomaticStopEnabled = false;

			inline int SelectedWeaponIndex = 0;
		}
		namespace Antiaim {
			inline bool IsEnabled = false;
		}
	}

	namespace Legitbot {
		inline bool IsTriggerbotEnabled = false;
		inline bool IsWallbangEnabled = false;
		inline int TriggerbotMinimumDamage = 100;
		inline int TriggerbotDelay = 20;
	}

	namespace ESP {
		namespace ESP {
			inline bool IsBoxEnabled = true;
			inline bool IsHealthBarEnabled = true;
			inline bool IsNameEnabled = true;
			inline bool IsSkeletonEnabled = true;
			inline bool IsGlowEnabled = true;
			inline bool IsFlagsEnabled = false;

			inline ColorRGBAStruct BoxColor = { 255.0f, 255.0f, 255.0f, 255.0f };
			inline ColorRGBAStruct HealthBarColor1 = { 0.0f, 255.0f, 0.0f, 255.0f };
			inline ColorRGBAStruct HealthBarColor2 = { 255.0f, 0.0f, 0.0f, 255.0f };
			inline ColorRGBAStruct NameColor = { 255.0f, 255.0f, 255.0f, 255.0f };
			inline ColorRGBAStruct SkeletonColor = { 255.0f, 255.0f, 255.0f, 255.0f };
			inline ColorRGBAStruct GlowColor = { 255.0f, 125.0f, 0.0f, 255.0f };

			inline bool IsViewmodelChangerEnabled;
		}
		namespace Visuals {
			inline bool RemoveFlashbang = true;
			inline bool RemoveSmoke = true;
		}
		namespace World {
			inline bool IsEnabled = true;

		}
	}

	namespace Miscellaneous {
		inline bool IsBunnyhopEnabled = false;
	}
}
