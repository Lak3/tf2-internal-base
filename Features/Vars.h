#pragma once

namespace Vars
{
	namespace ESP
	{
		inline bool Enabled = true;

		namespace Players
		{
			inline bool Enabled = true;
			inline bool IgnoreTeam = true;
			inline bool ActiveWeapon = true;
			inline bool HealthText = true;
			inline bool Name = true;
		}

		namespace Buildings
		{
			inline bool Enabled = true;
			inline bool IgnoreTeam = false;
			inline bool HealthText = true;
			inline bool Name = true;
			inline bool Info = true;
		}
	}

	namespace Visual
	{
		inline bool RemoveVisualRecoil = true;
		inline bool Tracers = false;
	}

	namespace Misc
	{
		inline bool Bunnyhop = true;
		inline bool BypassPure = true;
	}
}
