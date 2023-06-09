.macro def_special ptr
.global SPECIAL_\ptr
.set SPECIAL_\ptr, __special__
.set __special__, __special__ + 1
	.4byte \ptr
	.endm

.set __special__, 0
		.align 2
gSpecials::
	def_special HealPlayerParty
	def_special DoSecretBasePCTurnOffEffect
	def_special SavePlayerParty
	def_special LoadPlayerParty
	def_special ObjectEventInteractionGetBerryTreeData
	def_special ObjectEventInteractionGetBerryName
	def_special ObjectEventInteractionGetBerryCountString
	def_special Bag_ChooseBerry
	def_special ObjectEventInteractionPlantBerryTree
	def_special ObjectEventInteractionPickBerryTree
	def_special ObjectEventInteractionRemoveBerryTree
	def_special ObjectEventInteractionWaterBerryTree
	def_special PlayerHasBerries
	def_special IsEnigmaBerryValid
	def_special GetTrainerBattleMode
	def_special ShowTrainerIntroSpeech
	def_special ShowTrainerCantBattleSpeech
	def_special GetTrainerFlag
	def_special DoTrainerApproach
	def_special PlayTrainerEncounterMusic
	def_special ShouldTryRematchBattle
	def_special IsTrainerReadyForRematch
	def_special BattleSetup_StartRematchBattle
	def_special ShowPokemonStorageSystemPC
	def_special HasEnoughMonsForDoubleBattle
	def_special SaveGame
	def_special DoWateringBerryTreeAnim
	def_special GetSeedotSizeRecordInfo
	def_special CompareSeedotSize
	def_special GetLotadSizeRecordInfo
	def_special CompareLotadSize
	def_special BufferMonNickname
	def_special IsMonOTIDNotPlayers
	def_special SwapRegisteredBike
	def_special CalculatePlayerPartyCount
	def_special CountPartyNonEggMons
	def_special CountPartyAliveNonEggMons_IgnoreVar0x8004Slot
	def_special MauvilleGymSetDefaultBarriers
	def_special MauvilleGymPressSwitch
	def_special ShowFieldMessageStringVar4
	def_special DrawWholeMapView
	def_special StorePlayerCoordsInVars
	def_special MauvilleGymDeactivatePuzzle
	def_special PetalburgGymSlideOpenRoomDoors
	def_special PetalburgGymUnlockRoomDoors
	def_special GetPlayerTrainerIdOnesDigit
	def_special GetPlayerBigGuyGirlString
	def_special GetRivalSonDaughterString
	def_special SetHiddenItemFlag
	def_special CableCarWarp
	def_special CableCar
	def_special Overworld_PlaySpecialMapMusic
	def_special StartWallClock
	def_special Special_ViewWallClock
	def_special ChooseStarter
	def_special StartWallyTutorialBattle
	def_special ChangePokemonNickname
	def_special ChoosePartyMon
	def_special IsFanClubMemberFanOfPlayer
	def_special GetNumFansOfPlayerInTrainerFanClub
	def_special BufferFanClubTrainerName
	def_special TryLoseFansFromPlayTimeAfterLinkBattle
	def_special TryLoseFansFromPlayTime
	def_special SetPlayerGotFirstFans
	def_special UpdateTrainerFanClubGameClear
	def_special Script_TryGainNewFanFromCounter
	def_special RockSmashWildEncounter
	def_special GetBattleOutcome
	def_special GetDaycareMonNicknames
	def_special GetDaycareState
	def_special RejectEggFromDayCare
	def_special GiveEggFromDaycare
	def_special SetDaycareCompatibilityString
	def_special GetSelectedMonNicknameAndSpecies
	def_special StoreSelectedPokemonInDaycare
	def_special ChooseSendDaycareMon
	def_special ShowDaycareLevelMenu
	def_special GetNumLevelsGainedFromDaycare
	def_special GetDaycareCost
	def_special TakePokemonFromDaycare
	def_special ScriptHatchMon
	def_special EggHatch
	def_special IsEnoughForCostInVar0x8005
	def_special SubtractMoneyFromVar0x8005
	def_special TryFieldPoisonWhiteOut
	def_special SetCB2WhiteOut
	def_special RotatingGate_InitPuzzle
	def_special RotatingGate_InitPuzzleAndGraphics
	def_special SetSSTidalFlag
	def_special ResetSSTidalFlag
	def_special IsMirageIslandPresent
	def_special UpdateShoalTideFlag
	def_special InitBirchState
	def_special ScriptGetPokedexInfo
	def_special ShowPokedexRatingMessage
	def_special DoPCTurnOnEffect
	def_special DoPCTurnOffEffect
	def_special SetDeptStoreFloor
	def_special DoLotteryCornerComputerEffect
	def_special EndLotteryCornerComputerEffect
	def_special ChooseMonForMoveRelearner
	def_special MoveDeleterChooseMoveToForget
	def_special MoveDeleterForgetMove
	def_special BufferMoveDeleterNicknameAndMove
	def_special GetNumMovesSelectedMonHas
	def_special TeachMoveRelearnerMove
	def_special GetRecordedCyclingRoadResults
	def_special Special_BeginCyclingRoadChallenge
	def_special GetPlayerAvatarBike
	def_special FinishCyclingRoadChallenge
	def_special UpdateCyclingRoadState
	def_special GetLeadMonFriendshipScore
	def_special DoSpecialTrainerBattle
	def_special StopMapMusic
	def_special BedroomPC
	def_special PlayerPC
	def_special FieldShowRegionMap
	def_special GetWeekCount
	def_special ResetTrickHouseNuggetFlag
	def_special SetTrickHouseNuggetFlag
	def_special ScriptMenu_CreatePCMultichoice
	def_special AccessHallOfFamePC
	def_special Special_ShowDiploma
	def_special CheckLeadMonCool
	def_special CheckLeadMonBeauty
	def_special CheckLeadMonCute
	def_special CheckLeadMonSmart
	def_special CheckLeadMonTough
	def_special LookThroughPorthole
	def_special DoSoftReset
	def_special GameClear
	def_special MoveElevator
	def_special ShowGlassWorkshopMenu
	def_special SpawnCameraObject
	def_special RemoveCameraObject
	def_special TeleportCamera
	def_special ReturnCameraToPlayer
	def_special GetSecretBaseNearbyMapName
	def_special CheckRelicanthWailord
	def_special ShouldDoBrailleRegirockEffectOld
	def_special DoOrbEffect
	def_special FadeOutOrbEffect
	def_special WaitWeather
	def_special GetPlayerFacingDirection
	def_special FoundAbandonedShipRoom1Key
	def_special FoundAbandonedShipRoom2Key
	def_special FoundAbandonedShipRoom4Key
	def_special FoundAbandonedShipRoom6Key
	def_special LeadMonHasEffortRibbon
	def_special GiveLeadMonEffortRibbon
	def_special Special_AreLeadMonEVsMaxedOut
	def_special Script_FacePlayer
	def_special Script_ClearHeldMovement
	def_special InitRoamer
	def_special TryUpdateRusturfTunnelState
	def_special IsGrassTypeInParty
	def_special DoContestHallWarp
	def_special PutZigzagoonInPlayerParty
	def_special IsStarterInParty
	def_special ScriptCheckFreePokemonStorageSpace
	def_special DoSealedChamberShakingEffect_Long
	def_special ShowDeptStoreElevatorFloorSelect
	def_special InteractWithShieldOrTVDecoration
	def_special IsPokerusInParty
	def_special SetSootopolisGymCrackedIceMetatiles
	def_special ShakeCamera
	def_special StartGroudonKyogreBattle
	def_special BattleSetup_StartLegendaryBattle
	def_special StartRegiBattle
	def_special SetTrainerFacingDirection
	def_special DoSealedChamberShakingEffect_Short
	def_special FoundBlackGlasses
	def_special StartDroughtWeatherBlend
	def_special DoDiveWarp
	def_special DoFallWarp
	def_special BattleSetup_StartLatiBattle
	def_special SetRoute119Weather
	def_special SetRoute123Weather
	def_special ScriptGetPartyMonSpecies
	def_special IsSelectedMonEgg
	def_special TryInitBattleTowerAwardManObjectEvent
	def_special LoadPlayerBag
	def_special Script_FadeOutMapMusic
	def_special SetPacifidlogTMReceivedDay
	def_special GetDaysUntilPacifidlogTMAvailable
	def_special HasAllHoennMons
	def_special MonOTNameNotPlayer
	def_special DoesPartyHaveEnigmaBerry
	def_special SetChampionSaveWarp
	def_special ShowGlassWorkshopMenu
	def_special ShouldTryGetTrainerScript
	def_special ShowMapNamePopup
	def_special ShowMapNamePopup
	def_special DoMirageTowerCeilingCrumble
	def_special SetMirageTowerVisibility
	def_special StartPlayerDescendMirageTower
	def_special BufferTMHMMoveName
	def_special ChooseMonForWirelessMinigame
	def_special IsBadEggInParty
	def_special HasAtLeastOneBerry
	def_special OffsetCameraForBattle
	def_special GetDeptStoreDefaultFloorChoice
	def_special BufferVarsForIVRater
	def_special ShowScrollableMultichoice
	def_special ScrollableMultichoice_TryReturnToList
	def_special DoBattlePyramidMonsHaveHeldItem
	def_special BattlePyramidChooseMonHeldItems
	def_special UpdateBattlePointsWindow
	def_special CloseBattlePointsWindow
	def_special Script_DoRayquazaScene
	def_special OpenPokenavForTutorial
	def_special ScriptMenu_CreateStartMenuForPokenavTutorial
	def_special CountPlayerTrainerStars
	def_special ScrollableMultichoice_RedrawPersistentMenu
	def_special ChooseMonForMoveTutor
	def_special ScrollableMultichoice_ClosePersistentMenu
	def_special CreateEventLegalEnemyMon
	def_special StartMirageTowerDisintegration
	def_special StartMirageTowerShake
	def_special StartMirageTowerFossilFallAndSink
	def_special ChangeBoxPokemonNickname
	def_special GetPCBoxToSendMon
	def_special ShouldShowBoxWasFullMessage
	def_special SetMatchCallRegisteredFlag
	def_special DoDomeConfetti
	def_special CreateAbnormalWeatherEvent
	def_special GetAbnormalWeatherMapNameAndType
	def_special GetMartEmployeeObjectEventId
	def_special Unused_SetWeatherSunny
	def_special SetUnlockedPokedexFlags
	def_special IsTrainerRegistered
	def_special ShouldDoBrailleRegicePuzzle
	def_special EnableNationalPokedex
	def_special ScriptMenu_CreateLilycoveSSTidalMultichoice
	def_special GetLilycoveSSTidalSelection
	def_special ShouldDistributeEonTicket
	def_special LoopWingFlapSE
	def_special TryBufferWaldaPhrase
	def_special DoWaldaNamingScreen
	def_special TryGetWallpaperWithWaldaPhrase
	def_special GetBattlePyramidHint
	def_special PlayerFaceTrainerAfterBattle
	def_special ResetHealLocationFromDewford
	def_special IsLastMonThatKnowsSurf
	def_special CountPartyAliveNonEggMons
	def_special TryPrepareSecondApproachingTrainer
	def_special CloseDeptStoreElevatorWindow
	def_special AddObtainedItemIconSprite
	def_special RemoveObtainedItemIconSprite
	def_special EnableWildEncounters
	def_special DisableWildEncounters
	def_special IsWildEncountersDisabled
	def_special SetTotemBoost
	def_special TrySpecialOverworldEvo
