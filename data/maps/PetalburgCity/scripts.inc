PetalburgCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PetalburgCity_MapScripts_MAP_SCRIPT_ON_TRANSITION
	.byte 0

PetalburgCity_MapScripts_MAP_SCRIPT_ON_TRANSITION:
	setflag FLAG_VISITED_PETALBURG_CITY
	end


PetalburgCity_EventScript_WallyHouseSign::
	msgbox PetalburgCity_EventScript_WallyHouseSign_Text_0, MSGBOX_SIGN
	end


PetalburgCity_EventScript_GymSign::
	msgbox PetalburgCity_EventScript_GymSign_Text_0, MSGBOX_SIGN
	end


PetalburgCity_EventScript_GymIsLocked::
	msgbox PetalburgCity_EventScript_GymIsLocked_Text_0, MSGBOX_NPC
	end


PetalburgCity_EventScript_CitySign::
	msgbox PetalburgCity_EventScript_CitySign_Text_0, MSGBOX_SIGN
	end


PetalburgCity_EventScript_WallyHouseSign_Text_0:
	.string "Wally's House$"

PetalburgCity_EventScript_GymSign_Text_0:
	.string "Petalburg City Pokémon Gym\n"
	.string "Leader: Norman\l"
	.string "“A man in pursuit of power!”$"

PetalburgCity_EventScript_GymIsLocked_Text_0:
	.string "The gym is locked from the inside.$"

PetalburgCity_EventScript_CitySign_Text_0:
	.string "Petalburg City\n"
	.string "“Where people mingle with nature.”$"
