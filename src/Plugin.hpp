// This file is included separately for each engine version

namespace GOTHIC_NAMESPACE 
{
	std::vector<zVEC3> fireplaces;

	bool CanSave()
	{
		for(auto fireplace : fireplaces)
		{
			if(static_cast<int>(player->GetPositionWorld().Distance(fireplace)) <= 250)
			{
				return true;
			}
		}
		return false;
	}

	void Game_Init()
	{
		
	}

	void Game_Exit()
	{
		fireplaces.clear();
	}

	void Game_Loop()
	{
		if(!ogame || !player || ogame->singleStep)
		{
			return;
		}

		auto canSave = CanSave();

		zSTRING saveMenu = gameMan->menu_save_savegame ? gameMan->menu_save_savegame->name : "MENU_SAVEGAME_SAVE";

		for (int i = 0; i < zCMenuItem::itemList.GetNum(); i++)
		{
			zCMenuItem* item = zCMenuItem::itemList[i];
			if (item->m_parOnSelAction_S[0] == saveMenu)
			{
				if((item->m_parItemFlags & IT_DISABLED) != IT_DISABLED && !canSave)
				{
					item->m_parItemFlags = item->m_parItemFlags | IT_DISABLED;
				}
				else if ((item->m_parItemFlags & IT_DISABLED) == IT_DISABLED && canSave)
				{
					item->m_parItemFlags = item->m_parItemFlags &~IT_DISABLED;
				}
			}
		}
	}

	void LoadEnd()
	{
		fireplaces.clear();
		zCArray<zCVob*> voblist;
		ogame->GetGameWorld()->SearchVobListByBaseClass(zCVob::classDef, voblist, nullptr);

		for(int i = 0; i< voblist.GetNumInList(); i++)
		{
			auto vob = voblist[i];

			if(!vob)
			{
				continue;
			}

			if(!vob->visual)
			{
				continue;
			}

			if(vob->visual->GetVisualName().IsEmpty())
			{
				continue;
			}

			auto visualName = vob->visual->GetVisualName();
			visualName.Upper();
			
			if(visualName.Search("NW_MISC_FIREPLACE", 1U) == -1 
				&& visualName.Search("FIREPLACE_GROUND", 1U) == -1 
			)
			{
				continue;
			}

			fireplaces.push_back(vob->GetPositionWorld());
		}
	}

	void Game_LoadEnd_NewGame()
	{
		LoadEnd();
	}

	void Game_LoadEnd_SaveGame()
	{
		LoadEnd();
	}

	void Game_LoadEnd_ChangeLevel()
	{
		LoadEnd();
	}

	void Game_LoadEnd_Trigger()
	{
		LoadEnd();
	}

	class oCPluginPerFrameCallback : public zCWorldPerFrameCallback
	{
		virtual void DoWorldPerFrameCallback(zCWorld* world, zCCamera* cam)
		{
			Game_Loop();
		}
	} pluginPerFrameCallback;

	void __fastcall Game_Init_Hook()
	{
		Game_Init();
		ogame->GetWorld()->RegisterPerFrameCallback(&pluginPerFrameCallback);
	}
	auto PartialHook__Game_Init = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x00637BB6, 0x0065DFFA, 0x00665112, 0x006C1BFF)), &Game_Init_Hook);
#if ENGINE >= Engine_G2
	auto PartialHook__Game_Init2 = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x00000000, 0x00000000, 0x006650F6, 0x006C1C1B)), &Game_Init_Hook);
#endif

	void __fastcall Game_Exit_Hook()
	{
		Game_Exit();
	}
	auto PartialHook__Game_Exit = ::Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x00424850 + 7, 0x00427310 + 7, 0x004251A0 + 7, 0x004254E0 + 7)), &Game_Exit_Hook);

	void __fastcall Game_LoadEnd_NewGame_Hook()
	{
		Game_LoadEnd_NewGame();
	}
	auto PartialHook_Game_LoadEnd_NewGame = ::Union::CreatePartialHook((void*)(zSwitch(0x0063C28C, 0x00662D5A, 0x00669B8C, 0x006C67BC)), &Game_LoadEnd_NewGame_Hook);

	void __fastcall Game_LoadEnd_SaveGame_Hook()
	{
		Game_LoadEnd_SaveGame();
	}
	auto PartialHook_Game_LoadEnd_SaveGame = ::Union::CreatePartialHook((void*)(zSwitch(0x0063CD50, 0x0066393C, 0x0066A653, 0x006C7283)), &Game_LoadEnd_SaveGame_Hook);

	void __fastcall Game_LoadEnd_ChangeLevel_Hook()
	{
		Game_LoadEnd_ChangeLevel();
	}
	auto PartialHook_Game_LoadEnd_ChangeLevel = ::Union::CreatePartialHook((void*)(zSwitch(0x0063D46B, 0x006640F0, 0x0066AD6B, 0x006C7ADD)), &Game_LoadEnd_ChangeLevel_Hook);

	void __fastcall Game_LoadEnd_Trigger_Hook()
	{
		Game_LoadEnd_Trigger();
	}
	auto PartialHook_Game_LoadEnd_Trigger = ::Union::CreatePartialHook((void*)(zSwitch(0x0063D6BE, 0x0066433E, 0x0066AFBE, 0x006C7D2E)), &Game_LoadEnd_Trigger_Hook);
}