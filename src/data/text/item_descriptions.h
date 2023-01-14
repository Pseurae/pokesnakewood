static const u8 sDummyDesc[] = _(
    "?????\n");

// Poké Balls
static const u8 sPokeBallDesc[] = _(
    "A tool used for catching wild\n"
    "Pokémon.");

static const u8 sGreatBallDesc[] = _(
    "A good Ball with a higher catch\n"
    "rate than a Poké Ball.");

static const u8 sUltraBallDesc[] = _(
    "A better Ball with a higher catch\n"
    "rate than a Great Ball.");

static const u8 sMasterBallDesc[] = _(
    "The best Ball that catches a\n"
    "Pokémon without fail.");

static const u8 sPremierBallDesc[] = _(
    "A rare Ball made in commemoration\n"
    "of some event.");

static const u8 sHealBallDesc[] = _(
    "A remedial Ball that restores\n"
    "caught Pokémon.");

static const u8 sNetBallDesc[] = _(
    "A Ball that works well on Water-\n"
    "and Bug-type Pokémon.");

static const u8 sNestBallDesc[] = _(
    "A Ball that works better on weaker\n"
    "Pokémon.");

static const u8 sDiveBallDesc[] = _(
    "A Ball that works better on\n"
    "Pokémon on the ocean floor.");

static const u8 sDuskBallDesc[] = _(
    "Works well if used in a dark\n"
    "place.");

static const u8 sTimerBallDesc[] = _(
    "A Ball that gains power in battles\n"
    "taking many turns.");

static const u8 sQuickBallDesc[] = _(
    "Works well if used on the first\n"
    "turn.");

static const u8 sRepeatBallDesc[] = _(
    "A Ball that works better on\n"
    "Pokémon caught before.");

static const u8 sLuxuryBallDesc[] = _(
    "A cozy Ball that makes Pokémon\n"
    "more friendly.");

static const u8 sLevelBallDesc[] = _(
    "A Ball that works well on lower\n"
    "level Pokémon.");

static const u8 sLureBallDesc[] = _(
    "A Ball that works well on fished\n"
    "up Pokémon.");

static const u8 sMoonBallDesc[] = _(
    "A Ball that works well on Moon\n"
    "Stone users.");

static const u8 sFriendBallDesc[] = _(
    "A Ball that makes a Pokémon\n"
    "friendly when caught.");

static const u8 sLoveBallDesc[] = _(
    "Works well on Pokémon of the\n"
    "opposite gender.");

static const u8 sFastBallDesc[] = _(
    "Works well on very fast Pokémon.\n");

static const u8 sHeavyBallDesc[] = _(
    "Works well on very heavy Pokémon.\n");

static const u8 sDreamBallDesc[] = _(
#if B_DREAM_BALL_MODIFIER >= GEN_8
    "A Ball that works well on sleeping\n"
    "Pokémon.");
#else
    "A Poké Ball used in the Entree\n"
    "Forest.");
#endif

static const u8 sSafariBallDesc[] = _(
    "A special Ball that is used only\n"
    "in the Safari Zone.");

static const u8 sSportBallDesc[] = _(
    "A special Ball used in the Bug-\n"
    "Catching Contest.");

static const u8 sParkBallDesc[] = _(
    "A special Ball for the Pal Park.\n");

static const u8 sBeastBallDesc[] = _(
    "A Ball designed to catch Ultra\n"
    "Beasts.");

static const u8 sCherishBallDesc[] = _(
    "A rare Ball made in commemoration\n"
    "of some event.");

// Medicine
static const u8 sPotionDesc[] = _(
    "Restores the HP of a Pokémon by 20\n"
    "points.");

static const u8 sSuperPotionDesc[] = _(
    "Restores the HP of a Pokémon by\n"
#if I_HEALTH_RECOVERY >= GEN_7
    "60 points.");
#else
    "50 points.");
#endif

static const u8 sHyperPotionDesc[] = _(
    "Restores the HP of a Pokémon by\n"
#if I_HEALTH_RECOVERY >= GEN_7
    "120 points.");
#else
    "200 points.");
#endif

static const u8 sMaxPotionDesc[] = _(
    "Fully restores the HP of a\n"
    "Pokémon.");

static const u8 sFullRestoreDesc[] = _(
    "Fully restores the HP and status\n"
    "of a Pokémon.");

static const u8 sReviveDesc[] = _(
    "Revives a fainted Pokémon with\n"
    "half its HP.");

static const u8 sMaxReviveDesc[] = _(
    "Revives a fainted Pokémon with all\n"
    "its HP.");

static const u8 sFreshWaterDesc[] = _(
    "A mineral water that restores HP\n"
#if I_HEALTH_RECOVERY >= GEN_7
    "by 30 points.");
#else
    "by 50 points.");
#endif

static const u8 sSodaPopDesc[] = _(
    "A fizzy soda drink that restores\n"
#if I_HEALTH_RECOVERY >= GEN_7
    "HP by 50 points.");
#else
    "HP by 60 points.");
#endif

static const u8 sLemonadeDesc[] = _(
    "A very sweet drink that restores\n"
#if I_HEALTH_RECOVERY >= GEN_7
    "HP by 70 points.");
#else
    "HP by 80 points.");
#endif

static const u8 sMoomooMilkDesc[] = _(
    "A nutritious milk that restores HP\n"
    "by 100 points.");

static const u8 sEnergyPowderDesc[] = _(
    "A bitter powder that restores HP\n"
#if I_HEALTH_RECOVERY >= GEN_7
    "by 60 points.");
#else
    "by 50 points.");
#endif

static const u8 sEnergyRootDesc[] = _(
    "A bitter root that restores HP\n"
#if I_HEALTH_RECOVERY >= GEN_7
    "by 120 points.");
#else
    "by 200 points.");
#endif

static const u8 sHealPowderDesc[] = _(
    "A bitter powder that heals all\n"
    "status problems.");

static const u8 sRevivalHerbDesc[] = _(
    "A very bitter herb that revives a\n"
    "fainted Pokémon.");

static const u8 sAntidoteDesc[] = _(
    "Heals a poisoned Pokémon.\n");

static const u8 sParalyzeHealDesc[] = _(
    "Heals a paralyzed Pokémon.\n");

static const u8 sBurnHealDesc[] = _(
    "Heals Pokémon of a burn.\n");

static const u8 sIceHealDesc[] = _(
    "Defrosts a frozen Pokémon.\n");

static const u8 sAwakeningDesc[] = _(
    "Awakens a sleeping Pokémon.\n");

static const u8 sFullHealDesc[] = _(
    "Heals all the status problems of\n"
    "one Pokémon.");

static const u8 sEtherDesc[] = _(
    "Restores the PP of a selected move\n"
    "by 10.");

static const u8 sMaxEtherDesc[] = _(
    "Fully restores the PP of a\n"
    "selected move.");

static const u8 sElixirDesc[] = _(
    "Restores the PP of all moves by\n"
    "10.\n");

static const u8 sMaxElixirDesc[] = _(
    "Fully restores the PP of a\n"
    "Pokémon's moves.");

static const u8 sBerryJuiceDesc[] = _(
    "A 100% pure juice that restores HP\n"
    "by 20 points.");

static const u8 sSacredAshDesc[] = _(
    "Fully revives and restores all\n"
    "fainted Pokémon.");

static const u8 sSweetHeartDesc[] = _(
    "A sweet chocolate that restores HP\n"
    "by 20 points.");

static const u8 sMaxHoneyDesc[] = _(
    "Revives a fainted Pokémon with all\n"
    "its HP.");

static const u8 sPewterCrunchiesDesc[] = _(
    "Heals all the status problems of\n"
    "one Pokémon.");

static const u8 sRageCandyBarDesc[] = _(
    "Heals all the status problems of\n"
    "one Pokémon.");

static const u8 sLavaCookieDesc[] = _(
    "A local specialty that heals all\n"
    "status problems.");

static const u8 sOldGateauDesc[] = _(
    "Heals all the status problems of\n"
    "one Pokémon.");

static const u8 sCasteliaconeDesc[] = _(
    "Heals all the status problems of\n"
    "one Pokémon.");

static const u8 sLumioseGaletteDesc[] = _(
    "Heals all the status problems of\n"
    "one Pokémon.");

static const u8 sShalourSableDesc[] = _(
    "Heals all the status problems of\n"
    "one Pokémon.");

static const u8 sBigMalasadaDesc[] = _(
    "Heals all the status problems of\n"
    "one Pokémon.");

// Vitamins
static const u8 sHPUpDesc[] = _(
    "Raises the base HP of one Pokémon.\n");

static const u8 sProteinDesc[] = _(
    "Raises the base Attack stat of one\n"
    "Pokémon.");

static const u8 sIronDesc[] = _(
    "Raises the base Defense stat of\n"
    "one Pokémon.");

static const u8 sCalciumDesc[] = _(
    "Raises the base Sp. Atk stat of\n"
    "one Pokémon.");

static const u8 sZincDesc[] = _(
    "Raises the base Sp. Def stat of\n"
    "one Pokémon.");

static const u8 sCarbosDesc[] = _(
    "Raises the base Speed stat of one\n"
    "Pokémon.");

static const u8 sPPUpDesc[] = _(
    "Raises the maximum PP of a\n"
    "selected move.");

static const u8 sPPMaxDesc[] = _(
    "Raises the PP of a move to its\n"
    "maximum points.");

// EV Feathers
static const u8 sHealthFeatherDesc[] = _(
    "An item that raises the base HP of\n"
    "a Pokémon.");

static const u8 sMuscleFeatherDesc[] = _(
    "An item that raises the base\n"
    "Attack of a Pokémon.");

static const u8 sResistFeatherDesc[] = _(
    "An item that raises the base\n"
    "Defense of a Pokémon.");

static const u8 sGeniusFeatherDesc[] = _(
    "An item that raises the base Sp.\n"
    "Atk. of a Pokémon.");

static const u8 sCleverFeatherDesc[] = _(
    "An item that raises the base Sp.\n"
    "Def. of a Pokémon.");

static const u8 sSwiftFeatherDesc[] = _(
    "An item that raises the base Speed\n"
    "of a Pokémon.");

// Ability Modifiers
static const u8 sAbilityCapsuleDesc[] = _(
    "Switches a Poké- mon's ability.\n");

static const u8 sAbilityPatchDesc[] = _(
    "Turns the ability of a Pokémon\n"
    "into a rare ability.");

// Mints
static const u8 sLonelyMintDesc[] = _(
    "Can be smelled. It ups Attack, but\n"
    "reduces Defense.");

static const u8 sAdamantMintDesc[] = _(
    "Can be smelled. It ups Attack, but\n"
    "reduces Sp. Atk.");

static const u8 sNaughtyMintDesc[] = _(
    "Can be smelled. It ups Attack, but\n"
    "reduces Sp. Def.");

static const u8 sBraveMintDesc[] = _(
    "Can be smelled. It ups Attack, but\n"
    "reduces Speed.");

static const u8 sBoldMintDesc[] = _(
    "Can be smelled. It ups Defense,\n"
    "but reduces Attack.");

static const u8 sImpishMintDesc[] = _(
    "Can be smelled. It ups Defense,\n"
    "but reduces Sp. Atk.");

static const u8 sLaxMintDesc[] = _(
    "Can be smelled. It ups Defense,\n"
    "but reduces Sp. Def.");

static const u8 sRelaxedMintDesc[] = _(
    "Can be smelled. It ups Defense,\n"
    "but reduces Speed.");

static const u8 sModestMintDesc[] = _(
    "Can be smelled. It ups Sp. Atk,\n"
    "but reduces Attack.");

static const u8 sMildMintDesc[] = _(
    "Can be smelled. It ups Sp. Atk,\n"
    "but reduces Defense.");

static const u8 sRashMintDesc[] = _(
    "Can be smelled. It ups Sp. Atk,\n"
    "but reduces Sp. Def.");

static const u8 sQuietMintDesc[] = _(
    "Can be smelled. It ups Sp. Atk,\n"
    "but reduces Speed.");

static const u8 sCalmMintDesc[] = _(
    "Can be smelled. It ups Sp. Def,\n"
    "but reduces Attack.");

static const u8 sGentleMintDesc[] = _(
    "Can be smelled. It ups Sp. Def,\n"
    "but reduces Defense.");

static const u8 sCarefulMintDesc[] = _(
    "Can be smelled. It ups Sp. Def,\n"
    "but reduces Sp. Atk.");

static const u8 sSassyMintDesc[] = _(
    "Can be smelled. It ups Sp. Def,\n"
    "but reduces Speed.");

static const u8 sTimidMintDesc[] = _(
    "Can be smelled. It ups Speed, but\n"
    "reduces Attack.");

static const u8 sHastyMintDesc[] = _(
    "Can be smelled. It ups Speed, but\n"
    "reduces Defense.");

static const u8 sJollyMintDesc[] = _(
    "Can be smelled. It ups Speed, but\n"
    "reduces Sp. Atk.");

static const u8 sNaiveMintDesc[] = _(
    "Can be smelled. It ups Speed, but\n"
    "reduces Sp. Def.");

static const u8 sSeriousMintDesc[] = _(
    "Can be smelled. It ups Speed, but\n"
    "reduces Attack.");

// Candy
static const u8 sRareCandyDesc[] = _(
    "Raises the level of a Pokémon by\n"
    "one.");

static const u8 sExpCandyXSDesc[] = _(
    "Gives a very small amount of Exp.\n"
    "to a single Pokémon.");

static const u8 sExpCandySDesc[] = _(
    "Gives a small amount of Exp. to a\n"
    "single Pokémon.");

static const u8 sExpCandyMDesc[] = _(
    "Gives a moderate amount of Exp. to\n"
    "a single Pokémon.");

static const u8 sExpCandyLDesc[] = _(
    "Gives a large amount of Exp. to a\n"
    "single Pokémon.");

static const u8 sExpCandyXLDesc[] = _(
    "Gives a very large amount of Exp.\n"
    "to a single Pokémon.");

static const u8 sDynamaxCandyDesc[] = _(
    "Raises the Dynamax Level of a\n"
    "single Pokémon by one.");

// Medicinal Flutes
static const u8 sBlueFluteDesc[] = _(
    "A glass flute that awakens\n"
    "sleeping Pokémon.");

static const u8 sYellowFluteDesc[] = _(
    "A glass flute that snaps Pokémon\n"
    "out of confusion.");

static const u8 sRedFluteDesc[] = _(
    "A glass flute that snaps Pokémon\n"
    "out of attraction.");

// Encounter-modifying Flutes
static const u8 sBlackFluteDesc[] = _(
    "A glass flute that keeps away wild\n"
    "Pokémon.");

static const u8 sWhiteFluteDesc[] = _(
    "A glass flute that lures wild\n"
    "Pokémon.");

// Encounter Modifiers
static const u8 sRepelDesc[] = _(
    "Repels weak wild Pokémon for 100\n"
    "steps.");

static const u8 sSuperRepelDesc[] = _(
    "Repels weak wild Pokémon for 200\n"
    "steps.");

static const u8 sMaxRepelDesc[] = _(
    "Repels weak wild Pokémon for 250\n"
    "steps.");

static const u8 sLureDesc[] = _(
    "Makes Pokémon more likely to\n"
    "appear for 100 steps.");

static const u8 sSuperLureDesc[] = _(
    "Makes Pokémon more likely to\n"
    "appear for 200 steps.");

static const u8 sMaxLureDesc[] = _(
    "Makes Pokémon more likely to\n"
    "appear for 250 steps.");

static const u8 sEscapeRopeDesc[] = _(
    "Use to escape instantly from a\n"
    "cave or a dungeon.");

// Battle items
static const u8 sXAttackDesc[] = _(
#if B_X_ITEMS_BUFF >= GEN_7
    "Sharply raises stat Attack during\n"
    "one battle.");
#else
    "Raises the stat Attack during one\n"
    "battle.");
#endif

static const u8 sXDefenseDesc[] = _(
#if B_X_ITEMS_BUFF >= GEN_7
    "Sharply raises stat Defense during\n"
    "one battle.");
#else
    "Raises the stat Defense during one\n"
    "battle.");
#endif

static const u8 sXSpAtkDesc[] = _(
#if B_X_ITEMS_BUFF >= GEN_7
    "Sharply raises stat Sp. Atk during\n"
    "one battle.");
#else
    "Raises the stat Sp. Atk during one\n"
    "battle.");
#endif

static const u8 sXSpDefDesc[] = _(
#if B_X_ITEMS_BUFF >= GEN_7
    "Sharply raises stat Sp. Def during\n"
    "one battle.");
#else
    "Raises the stat Sp. Def during one\n"
    "battle.");
#endif

static const u8 sXSpeedDesc[] = _(
#if B_X_ITEMS_BUFF >= GEN_7
    "Sharply raises stat Speed during\n"
    "one battle.");
#else
    "Raises the stat Speed during one\n"
    "battle.");
#endif

static const u8 sXAccuracyDesc[] = _(
#if B_X_ITEMS_BUFF >= GEN_7
    "Sharply raises move accuracy\n"
    "during one battle.");
#else
    "Raises accuracy of attack moves\n"
    "during one battle.");
#endif

static const u8 sDireHitDesc[] = _(
    "Raises the critical-hit ratio\n"
    "during one battle.");

static const u8 sGuardSpecDesc[] = _(
    "Prevents stat reduction when used\n"
    "in battle.");

// Escape Items
static const u8 sPokeDollDesc[] = _(
    "Use to flee from any battle with a\n"
    "wild Pokémon.");

static const u8 sFluffyTailDesc[] = _(
    "Use to flee from any battle with a\n"
    "wild Pokémon.");

static const u8 sPokeToyDesc[] = _(
    "Use to flee from any battle with a\n"
    "wild Pokémon.");

static const u8 sMaxMushroomsDesc[] = _(
    "Raises every stat during one\n"
    "battle by one stage.");

// Treasures
static const u8 sBottleCapDesc[] = _(
    "A beautiful bottle cap that gives\n"
    "off a silver gleam.");

static const u8 sGoldBottleCapDesc[] = _(
    "A beautiful bottle cap that gives\n"
    "off a golden gleam.");

static const u8 sNuggetDesc[] = _(
    "A nugget of pure gold. Can be sold\n"
    "at a high price.");

static const u8 sBigNuggetDesc[] = _(
    "A big nugget made of gold,\n"
    "sellable at a high price.");

static const u8 sTinyMushroomDesc[] = _(
    "A plain mushroom that would sell\n"
    "at a cheap price.");

static const u8 sBigMushroomDesc[] = _(
    "A rare mushroom that would sell at\n"
    "a high price.");

static const u8 sBalmMushroomDesc[] = _(
    "A rare mushroom that would sell at\n"
    "a high price.");

static const u8 sPearlDesc[] = _(
    "A pretty pearl that would sell at\n"
    "a cheap price.");

static const u8 sBigPearlDesc[] = _(
    "A lovely large pearl that would\n"
    "sell at a high price.");

static const u8 sPearlStringDesc[] = _(
    "Very large pearls that would sell\n"
    "at a high price.");

static const u8 sStardustDesc[] = _(
    "Beautiful red sand. Can be sold at\n"
    "a high price.");

static const u8 sStarPieceDesc[] = _(
    "A red gem shard. It would sell for\n"
    "a very high price.");

static const u8 sCometShardDesc[] = _(
    "A comet's shard. It would sell for\n"
    "a high price.");

static const u8 sShoalSaltDesc[] = _(
    "Salt obtained from deep inside the\n"
    "Shoal Cave.");

static const u8 sShoalShellDesc[] = _(
    "A seashell found deep inside the\n"
    "Shoal Cave.");

static const u8 sRedShardDesc[] = _(
    "A shard from an ancient item. Can\n"
    "be sold cheaply.");

static const u8 sBlueShardDesc[] = _(
    "A shard from an ancient item. Can\n"
    "be sold cheaply.");

static const u8 sYellowShardDesc[] = _(
    "A shard from an ancient item. Can\n"
    "be sold cheaply.");

static const u8 sGreenShardDesc[] = _(
    "A shard from an ancient item. Can\n"
    "be sold cheaply.");

static const u8 sHeartScaleDesc[] = _(
    "A lovely scale. It is coveted by\n"
    "collectors.");

static const u8 sHoneyDesc[] = _(
    "Sweet honey that attracts wild\n"
    "Pokémon when used.");

static const u8 sRareBoneDesc[] = _(
    "A very rare bone. It can be sold\n"
    "at a high price.");

static const u8 sOddKeystoneDesc[] = _(
    "Voices can be heard from this odd\n"
    "stone occasionally.");

static const u8 sPrettyFeatherDesc[] = _(
    "A beautiful yet plain feather that\n"
    "does nothing.");

static const u8 sRelicCopperDesc[] = _(
    "A copper coin used long ago. It\n"
    "sells at a high price.");

static const u8 sRelicSilverDesc[] = _(
    "A silver coin used long ago. It\n"
    "sells at a high price.");

static const u8 sRelicGoldDesc[] = _(
    "A gold coin used long ago. It\n"
    "sells at a high price.");

static const u8 sRelicVaseDesc[] = _(
    "A vase made long ago. It sells at\n"
    "a high price.");

static const u8 sRelicBandDesc[] = _(
    "An old bracelet. It sells at a\n"
    "high price.");

static const u8 sRelicStatueDesc[] = _(
    "An old statue. It sells at a high\n"
    "price.");

static const u8 sRelicCrownDesc[] = _(
    "An old crown. It sells at a high\n"
    "price.");

static const u8 sStrangeSouvenirDesc[] = _(
    "An ornament that depicts a Pokémon\n"
    "from Alola.");

// Fossils
static const u8 sHelixFossilDesc[] = _(
    "A piece of an ancient marine\n"
    "Pokémon's seashell.");

static const u8 sDomeFossilDesc[] = _(
    "A piece of an ancient marine\n"
    "Pokémon's shell.");

static const u8 sOldAmberDesc[] = _(
    "A stone containing the genes of an\n"
    "ancient Pokémon.");

static const u8 sRootFossilDesc[] = _(
    "A fossil of an ancient, seafloor-\n"
    "dwelling Pokémon.");

static const u8 sClawFossilDesc[] = _(
    "A fossil of an ancient, seafloor-\n"
    "dwelling Pokémon.");

static const u8 sArmorFossilDesc[] = _(
    "A piece of a prehistoric Poké-\n"
    "mon's head.");

static const u8 sSkullFossilDesc[] = _(
    "A piece of a prehistoric Poké-\n"
    "mon's head.");

static const u8 sCoverFossilDesc[] = _(
    "A piece of a prehistoric Poké-\n"
    "mon's back.");

static const u8 sPlumeFossilDesc[] = _(
    "A piece of a prehistoric Poké-\n"
    "mon's wing.");

static const u8 sJawFossilDesc[] = _(
    "A piece of a prehis- toric\n"
    "Pokémon's large jaw.");

static const u8 sSailFossilDesc[] = _(
    "A piece of a prehis- toric\n"
    "Pokémon's skin sail.");

static const u8 sFossilizedBirdDesc[] = _(
    "A fossil of an ancient, sky-\n"
    "soaring Pokémon.");

static const u8 sFossilizedFishDesc[] = _(
    "A fossil of an ancient, sea-\n"
    "dwelling Pokémon.");

static const u8 sFossilizedDrakeDesc[] = _(
    "A fossil of an ancient, land-\n"
    "roaming Pokémon.");

static const u8 sFossilizedDinoDesc[] = _(
    "A fossil of an ancient, sea-\n"
    "dwelling Pokémon.");

// Mulch
static const u8 sGrowthMulchDesc[] = _(
    "A fertilizer that accelerates the\n"
    "growth of Berries.");

static const u8 sDampMulchDesc[] = _(
    "A fertilizer that decelerates the\n"
    "growth of Berries.");

static const u8 sStableMulchDesc[] = _(
    "A fertilizer that ups the life\n"
    "time of Berry trees.");

static const u8 sGooeyMulchDesc[] = _(
    "A fertilizer that makes more\n"
    "Berries regrow after fall.");

static const u8 sRichMulchDesc[] = _(
    "A fertilizer that ups the number\n"
    "of Berries harvested.");

static const u8 sSurpriseMulchDesc[] = _(
    "A fertilizer that ups the chance\n"
    "of Berry mutations.");

static const u8 sBoostMulchDesc[] = _(
    "A fertilizer that ups the dry\n"
    "speed of soft soil.");

static const u8 sAmazeMulchDesc[] = _(
    "A fertilizer Rich Surprising and\n"
    "Boosting as well.");

// Apricorns
static const u8 sRedApricornDesc[] = _(
    "A red apricorn. It assails your\n"
    "nostrils.");

static const u8 sBlueApricornDesc[] = _(
    "A blue apricorn. It smells a bit\n"
    "like grass.");

static const u8 sYellowApricornDesc[] = _(
    "A yellow apricorn. It has an\n"
    "invigor- ating scent.");

static const u8 sGreenApricornDesc[] = _(
    "A green apricorn. It has a\n"
    "strange, aromatic scent.");

static const u8 sPinkApricornDesc[] = _(
    "A pink apricorn. It has a nice,\n"
    "sweet scent.");

static const u8 sWhiteApricornDesc[] = _(
    "A white apricorn. It doesn't smell\n"
    "like anything.");

static const u8 sBlackApricornDesc[] = _(
    "A black apricorn. It has an inde-\n"
    "scribable scent.");

static const u8 sWishingPieceDesc[] = _(
    "Throw into a {PKMN} Den to attract\n"
    "Dynamax Pokémon.");

static const u8 sGalaricaTwigDesc[] = _(
    "A twig from a tree in Galar called\n"
    "Galarica.");

static const u8 sArmoriteOreDesc[] = _(
    "A rare ore. Can be found in the\n"
    "Isle of Armor at Galar.");

static const u8 sDyniteOreDesc[] = _(
    "A mysterious ore. It can be found\n"
    "in Galar's Max Lair.");

// Mail
static const u8 sOrangeMailDesc[] = _(
    "A Zigzagoon-print Mail to be held\n"
    "by a Pokémon.");

static const u8 sHarborMailDesc[] = _(
    "A Wingull-print Mail to be held by\n"
    "a Pokémon.");

static const u8 sGlitterMailDesc[] = _(
    "A Pikachu-print Mail to be held by\n"
    "a Pokémon.");

static const u8 sMechMailDesc[] = _(
    "A Magnemite-print Mail to be held\n"
    "by a Pokémon.");

static const u8 sWoodMailDesc[] = _(
    "A Slakoth-print Mail to be held by\n"
    "a Pokémon.");

static const u8 sWaveMailDesc[] = _(
    "A Wailmer-print Mail to be held by\n"
    "a Pokémon.");

static const u8 sBeadMailDesc[] = _(
    "Mail featuring a sketch of the\n"
    "holding Pokémon.");

static const u8 sShadowMailDesc[] = _(
    "A Duskull-print Mail to be held by\n"
    "a Pokémon.");

static const u8 sTropicMailDesc[] = _(
    "A Bellossom-print Mail to be held\n"
    "by a Pokémon.");

static const u8 sDreamMailDesc[] = _(
    "Mail featuring a sketch of the\n"
    "holding Pokémon.");

static const u8 sFabMailDesc[] = _(
    "A gorgeous-print Mail to be held\n"
    "by a Pokémon.");

static const u8 sRetroMailDesc[] = _(
    "Mail featuring the drawings of\n"
    "three Pokémon.");

// Evolution Items
static const u8 sFireStoneDesc[] = _(
    "Makes certain species of Pokémon\n"
    "evolve.");

static const u8 sWaterStoneDesc[] = _(
    "Makes certain species of Pokémon\n"
    "evolve.");

static const u8 sThunderStoneDesc[] = _(
    "Makes certain species of Pokémon\n"
    "evolve.");

static const u8 sLeafStoneDesc[] = _(
    "Makes certain species of Pokémon\n"
    "evolve.");

static const u8 sIceStoneDesc[] = _(
    "Makes certain species of Pokémon\n"
    "evolve.");

static const u8 sSunStoneDesc[] = _(
    "Makes certain species of Pokémon\n"
    "evolve.");

static const u8 sMoonStoneDesc[] = _(
    "Makes certain species of Pokémon\n"
    "evolve.");

static const u8 sShinyStoneDesc[] = _(
    "Makes certain species of Pokémon\n"
    "evolve.");

static const u8 sDuskStoneDesc[] = _(
    "Makes certain species of Pokémon\n"
    "evolve.");

static const u8 sDawnStoneDesc[] = _(
    "Makes certain species of Pokémon\n"
    "evolve.");

static const u8 sSweetAppleDesc[] = _(
    "A very sweet apple that makes\n"
    "certain Pokémon evolve.");

static const u8 sTartAppleDesc[] = _(
    "A very tart apple that makes\n"
    "certain Pokémon evolve.");

static const u8 sCrackedPotDesc[] = _(
    "A cracked teapot that makes\n"
    "certain Pokémon evolve.");

static const u8 sChippedPotDesc[] = _(
    "A chipped teapot that makes\n"
    "certain Pokémon evolve.");

static const u8 sGalaricaCuffDesc[] = _(
    "A cuff from Galar that makes\n"
    "certain Pokémon evolve.");

static const u8 sGalaricaWreathDesc[] = _(
    "A wreath made in Galar. Makes some\n"
    "Pokémon evolve.");

static const u8 sDragonScaleDesc[] = _(
    "A strange scale held by Dragon-\n"
    "type Pokémon.");

static const u8 sUpgradeDesc[] = _(
    "A peculiar box made by Silph Co.\n");

static const u8 sProtectorDesc[] = _(
    "Loved by a certain Pokémon. It's\n"
    "stiff and heavy.");

static const u8 sElectirizerDesc[] = _(
    "Loved by a certain Pokémon. It's\n"
    "full of electric energy.");

static const u8 sMagmarizerDesc[] = _(
    "Loved by a certain Pokémon. It's\n"
    "full of magma energy.");

static const u8 sDubiousDiscDesc[] = _(
    "A transparent device overflowing\n"
    "with dubious data.");

static const u8 sReaperClothDesc[] = _(
    "Loved by a certain Pokémon. Imbued\n"
    "with spiritual energy.");

static const u8 sPrismScaleDesc[] = _(
    "A mysterious scale that evolves\n"
    "certain Pokémon. It shines.");

static const u8 sWhippedDreamDesc[] = _(
    "A soft and sweet treat loved by a\n"
    "certain Pokémon.");

static const u8 sSachetDesc[] = _(
    "A sachet filled with perfumes\n"
    "loved by a certain Pokémon.");

static const u8 sOvalStoneDesc[] = _(
    "Makes a certain Pokémon evolve.\n"
    "It's shaped like an egg.");

static const u8 sStrawberrySweetDesc[] = _(
    "Strawberry-shaped sweet loved by\n"
    "Milcery.");

static const u8 sLoveSweetDesc[] = _(
    "A heart-shaped sweet loved by\n"
    "Milcery.");

static const u8 sBerrySweetDesc[] = _(
    "A berry-shaped sweet loved by\n"
    "Milcery.");

static const u8 sCloverSweetDesc[] = _(
    "A clover-shaped sweet loved by\n"
    "Milcery.");

static const u8 sFlowerSweetDesc[] = _(
    "A flower-shaped sweet loved by\n"
    "Milcery.");

static const u8 sStarSweetDesc[] = _(
    "A star-shaped sweet loved by\n"
    "Milcery.");

static const u8 sRibbonSweetDesc[] = _(
    "A ribbon-shaped sweet loved by\n"
    "Milcery.");

static const u8 sEverstoneDesc[] = _(
    "A wondrous hold item that prevents\n"
    "evolution.");

// Nectars
static const u8 sRedNectarDesc[] = _(
    "Flower nectar that changes the\n"
    "form of certain Pokémon.");

static const u8 sYellowNectarDesc[] = _(
    "Flower nectar that changes the\n"
    "form of certain Pokémon.");

static const u8 sPinkNectarDesc[] = _(
    "Flower nectar that changes the\n"
    "form of certain Pokémon.");

static const u8 sPurpleNectarDesc[] = _(
    "Flower nectar that changes the\n"
    "form of certain Pokémon.");

// Plates
static const u8 sFlamePlateDesc[] = _(
    "A tablet that ups the power of\n"
    "Fire-type moves.");

static const u8 sSplashPlateDesc[] = _(
    "A tablet that ups the power of\n"
    "Water-type moves.");

static const u8 sZapPlateDesc[] = _(
    "A tablet that ups the power of\n"
    "Elec- tric-type moves.");

static const u8 sMeadowPlateDesc[] = _(
    "A tablet that ups the power of\n"
    "Grass-type moves.");

static const u8 sIciclePlateDesc[] = _(
    "A tablet that ups the power of\n"
    "Ice-type moves.");

static const u8 sFistPlateDesc[] = _(
    "A tablet that ups the power of\n"
    "Fight- ing-type moves.");

static const u8 sToxicPlateDesc[] = _(
    "A tablet that ups the power of\n"
    "Poison-type moves.");

static const u8 sEarthPlateDesc[] = _(
    "A tablet that ups the power of\n"
    "Ground-type moves.");

static const u8 sSkyPlateDesc[] = _(
    "A tablet that ups the power of\n"
    "Flying-type moves.");

static const u8 sMindPlateDesc[] = _(
    "A tablet that ups the power of Psy\n"
    "chic-type moves.");

static const u8 sInsectPlateDesc[] = _(
    "A tablet that ups the power of\n"
    "Bug-type moves.");

static const u8 sStonePlateDesc[] = _(
    "A tablet that ups the power of\n"
    "Rock-type moves.");

static const u8 sSpookyPlateDesc[] = _(
    "A tablet that ups the power of\n"
    "Ghost-type moves.");

static const u8 sDracoPlateDesc[] = _(
    "A tablet that ups the power of\n"
    "Dragon-type moves.");

static const u8 sDreadPlateDesc[] = _(
    "A tablet that ups the power of\n"
    "Dark-type moves.");

static const u8 sIronPlateDesc[] = _(
    "A tablet that ups the power of\n"
    "Steel-type moves.");

static const u8 sPixiePlateDesc[] = _(
    "A stone tablet that boosts the\n"
    "power of Fairy-type moves.");

// Drives
static const u8 sDouseDriveDesc[] = _(
    "Changes Genesect's Techno Blast to\n"
    "Water-type.");

static const u8 sShockDriveDesc[] = _(
    "Changes Genesect's Techno Blast to\n"
    "Electric-type.");

static const u8 sBurnDriveDesc[] = _(
    "Changes Genesect's Techno Blast to\n"
    "Fire-type.");

static const u8 sChillDriveDesc[] = _(
    "Changes Genesect's Techno Blast to\n"
    "Ice-type.");

// Memories
static const u8 sFireMemoryDesc[] = _(
    "A disc with Fire type data. It\n"
    "swaps Silvally's type.");

static const u8 sWaterMemoryDesc[] = _(
    "A disc with Water type data. It\n"
    "swaps Silvally's type.");

static const u8 sElectricMemoryDesc[] = _(
    "A disc with Electric type data. It\n"
    "swaps Silvally's type.");

static const u8 sGrassMemoryDesc[] = _(
    "A disc with Grass type data. It\n"
    "swaps Silvally's type.");

static const u8 sIceMemoryDesc[] = _(
    "A disc with Ice type data. It\n"
    "swaps Silvally's type.");

static const u8 sFightingMemoryDesc[] = _(
    "A disc with Fighting type data. It\n"
    "swaps Silvally's type.");

static const u8 sPoisonMemoryDesc[] = _(
    "A disc with Poison type data. It\n"
    "swaps Silvally's type.");

static const u8 sGroundMemoryDesc[] = _(
    "A disc with Ground type data. It\n"
    "swaps Silvally's type.");

static const u8 sFlyingMemoryDesc[] = _(
    "A disc with Flying type data. It\n"
    "swaps Silvally's type.");

static const u8 sPsychicMemoryDesc[] = _(
    "A disc with Psychic type data. It\n"
    "swaps Silvally's type.");

static const u8 sBugMemoryDesc[] = _(
    "A disc with Bug type data. It\n"
    "swaps Silvally's type.");

static const u8 sRockMemoryDesc[] = _(
    "A disc with Rock type data. It\n"
    "swaps Silvally's type.");

static const u8 sGhostMemoryDesc[] = _(
    "A disc with Ghost type data. It\n"
    "swaps Silvally's type.");

static const u8 sDragonMemoryDesc[] = _(
    "A disc with Dragon type data. It\n"
    "swaps Silvally's type.");

static const u8 sDarkMemoryDesc[] = _(
    "A disc with Dark type data. It\n"
    "swaps Silvally's type.");

static const u8 sSteelMemoryDesc[] = _(
    "A disc with Steel type data. It\n"
    "swaps Silvally's type.");

static const u8 sFairyMemoryDesc[] = _(
    "A disc with Fairy type data. It\n"
    "swaps Silvally's type.");

static const u8 sRustedSwordDesc[] = _(
    "A rusty sword. A hero used it to\n"
    "halt a disaster.");

static const u8 sRustedShieldDesc[] = _(
    "A rusty shield. A hero used it to\n"
    "halt a disaster.");

// Colored Orbs
static const u8 sRedOrbDesc[] = _(
    "A red, glowing orb said to contain\n"
    "an ancient power.");

static const u8 sBlueOrbDesc[] = _(
    "A blue, glowing orb said to\n"
    "contain an ancient power.");

// Mega Stones
static const u8 sVenusauriteDesc[] = _(
    "This stone enables Venusaur to\n"
    "Mega Evolve in battle.");

static const u8 sCharizarditeDesc[] = _(
    "This stone enables Charizard to\n"
    "Mega Evolve in battle.");

static const u8 sBlastoisiniteDesc[] = _(
    "This stone enables Blastoise to\n"
    "Mega Evolve in battle.");

static const u8 sBeedrilliteDesc[] = _(
    "This stone enables Beedrill to\n"
    "Mega Evolve in battle.");

static const u8 sPidgeotiteDesc[] = _(
    "This stone enables Pidgeot to Mega\n"
    "Evolve in battle.");

static const u8 sAlakaziteDesc[] = _(
    "This stone enables Alakazam to\n"
    "Mega Evolve in battle.");

static const u8 sSlowbroniteDesc[] = _(
    "This stone enables Slowbro to Mega\n"
    "Evolve in battle.");

static const u8 sGengariteDesc[] = _(
    "This stone enables Gengar to Mega\n"
    "Evolve in battle.");

static const u8 sKangaskhaniteDesc[] = _(
    "This stone enables Kangaskhan to\n"
    "Mega Evolve in battle.");

static const u8 sPinsiriteDesc[] = _(
    "This stone enables Pinsir to Mega\n"
    "Evolve in battle.");

static const u8 sGyaradositeDesc[] = _(
    "This stone enables Gyarados to\n"
    "Mega Evolve in battle.");

static const u8 sAerodactyliteDesc[] = _(
    "This stone enables Aerodactyl to\n"
    "Mega Evolve in battle.");

static const u8 sMewtwoniteDesc[] = _(
    "This stone enables Mewtwo to Mega\n"
    "Evolve in battle.");

static const u8 sAmpharositeDesc[] = _(
    "This stone enables Ampharos to\n"
    "Mega Evolve in battle.");

static const u8 sSteelixiteDesc[] = _(
    "This stone enables Steelix to Mega\n"
    "Evolve in battle.");

static const u8 sScizoriteDesc[] = _(
    "This stone enables Scizor to Mega\n"
    "Evolve in battle.");

static const u8 sHeracroniteDesc[] = _(
    "This stone enables Heracross to\n"
    "Mega Evolve in battle.");

static const u8 sHoundoominiteDesc[] = _(
    "This stone enables Houndoom to\n"
    "Mega Evolve in battle.");

static const u8 sTyranitariteDesc[] = _(
    "This stone enables Tyranitar to\n"
    "Mega Evolve in battle.");

static const u8 sSceptiliteDesc[] = _(
    "This stone enables Sceptile to\n"
    "Mega Evolve in battle.");

static const u8 sBlazikeniteDesc[] = _(
    "This stone enables Blaziken to\n"
    "Mega Evolve in battle.");

static const u8 sSwampertiteDesc[] = _(
    "This stone enables Swampert to\n"
    "Mega Evolve in battle.");

static const u8 sGardevoiriteDesc[] = _(
    "This stone enables Gardevoir to\n"
    "Mega Evolve in battle.");

static const u8 sSableniteDesc[] = _(
    "This stone enables Sableye to Mega\n"
    "Evolve in battle.");

static const u8 sMawiliteDesc[] = _(
    "This stone enables Mawile to Mega\n"
    "Evolve in battle.");

static const u8 sAggroniteDesc[] = _(
    "This stone enables Aggron to Mega\n"
    "Evolve in battle.");

static const u8 sMedichamiteDesc[] = _(
    "This stone enables Medicham to\n"
    "Mega Evolve in battle.");

static const u8 sManectiteDesc[] = _(
    "This stone enables Manectric to\n"
    "Mega Evolve in battle.");

static const u8 sSharpedoniteDesc[] = _(
    "This stone enables Sharpedo to\n"
    "Mega Evolve in battle.");

static const u8 sCameruptiteDesc[] = _(
    "This stone enables Camerupt to\n"
    "Mega Evolve in battle.");

static const u8 sAltarianiteDesc[] = _(
    "This stone enables Altaria to Mega\n"
    "Evolve in battle.");

static const u8 sBanettiteDesc[] = _(
    "This stone enables Banette to Mega\n"
    "Evolve in battle.");

static const u8 sAbsoliteDesc[] = _(
    "This stone enables Absol to Mega\n"
    "Evolve in battle.");

static const u8 sGlalititeDesc[] = _(
    "This stone enables Glalie to Mega\n"
    "Evolve in battle.");

static const u8 sSalamenciteDesc[] = _(
    "This stone enables Salamence to\n"
    "Mega Evolve in battle.");

static const u8 sMetagrossiteDesc[] = _(
    "This stone enables Metagross to\n"
    "Mega Evolve in battle.");

static const u8 sLatiasiteDesc[] = _(
    "This stone enables Latias to Mega\n"
    "Evolve in battle.");

static const u8 sLatiositeDesc[] = _(
    "This stone enables Latios to Mega\n"
    "Evolve in battle.");

static const u8 sLopunniteDesc[] = _(
    "This stone enables Lopunny to Mega\n"
    "Evolve in battle.");

static const u8 sGarchompiteDesc[] = _(
    "This stone enables Garchomp to\n"
    "Mega Evolve in battle.");

static const u8 sLucarioniteDesc[] = _(
    "This stone enables Lucario to Mega\n"
    "Evolve in battle.");

static const u8 sAbomasiteDesc[] = _(
    "This stone enables Abomasnow to\n"
    "Mega Evolve in battle.");

static const u8 sGalladiteDesc[] = _(
    "This stone enables Gallade to Mega\n"
    "Evolve in battle.");

static const u8 sAudiniteDesc[] = _(
    "This stone enables Audino to Mega\n"
    "Evolve in battle.");

static const u8 sDianciteDesc[] = _(
    "This stone enables Diancie to Mega\n"
    "Evolve in battle.");

// Gems
static const u8 sNormalGemDesc[] = _(
    "Increases the power of Normal Type\n"
    "moves.");

static const u8 sFireGemDesc[] = _(
    "Increases the power of Fire Type\n"
    "moves.");

static const u8 sWaterGemDesc[] = _(
    "Increases the power of Water Type\n"
    "moves.");

static const u8 sElectricGemDesc[] = _(
    "Increases the power of Electric\n"
    "Type moves.");

static const u8 sGrassGemDesc[] = _(
    "Increases the power of Grass Type\n"
    "moves.");

static const u8 sIceGemDesc[] = _(
    "Increases the power of Ice Type\n"
    "moves.");

static const u8 sFightingGemDesc[] = _(
    "Increases the power of Fighting\n"
    "Type moves.");

static const u8 sPoisonGemDesc[] = _(
    "Increases the power of Poison Type\n"
    "moves.");

static const u8 sGroundGemDesc[] = _(
    "Increases the power of Ground Type\n"
    "moves.");

static const u8 sFlyingGemDesc[] = _(
    "Increases the power of Flying Type\n"
    "moves.");

static const u8 sPsychicGemDesc[] = _(
    "Increases the power of Psychic\n"
    "Type moves.");

static const u8 sBugGemDesc[] = _(
    "Increases the power of Bug Type\n"
    "moves.");

static const u8 sRockGemDesc[] = _(
    "Increases the power of Rock Type\n"
    "moves.");

static const u8 sGhostGemDesc[] = _(
    "Increases the power of Ghost Type\n"
    "moves.");

static const u8 sDragonGemDesc[] = _(
    "Increases the power of Dragon Type\n"
    "moves.");

static const u8 sDarkGemDesc[] = _(
    "Increases the power of Dark Type\n"
    "moves.");

static const u8 sSteelGemDesc[] = _(
    "Increases the power of Steel Type\n"
    "moves.");

static const u8 sFairyGemDesc[] = _(
    "Increases the power of Fairy Type\n"
    "moves.");

// Z-Crystals
static const u8 sNormaliumZDesc[] = _(
    "Upgrade Normal- type moves into\n"
    "Z-Moves.");

static const u8 sFiriumZDesc[] = _(
    "Upgrade Fire- type moves into\n"
    "Z-Moves.");

static const u8 sWateriumZDesc[] = _(
    "Upgrade Water- type moves into\n"
    "Z-Moves.");

static const u8 sElectriumZDesc[] = _(
    "Upgrade Electric- type moves into\n"
    "Z-Moves.");

static const u8 sGrassiumZDesc[] = _(
    "Upgrade Grass- type moves into\n"
    "Z-Moves.");

static const u8 sIciumZDesc[] = _(
    "Upgrade Ice- type moves into\n"
    "Z-Moves.");

static const u8 sFightiniumZDesc[] = _(
    "Upgrade Fighting- type moves into\n"
    "Z-Moves.");

static const u8 sPoisoniumZDesc[] = _(
    "Upgrade Poison- type moves into\n"
    "Z-Moves.");

static const u8 sGroundiumZDesc[] = _(
    "Upgrade Ground- type moves into\n"
    "Z-Moves.");

static const u8 sFlyiniumZDesc[] = _(
    "Upgrade Flying- type moves into\n"
    "Z-Moves.");

static const u8 sPsychiumZDesc[] = _(
    "Upgrade Psychic- type moves into\n"
    "Z-Moves.");

static const u8 sBuginiumZDesc[] = _(
    "Upgrade Bug- type moves into\n"
    "Z-Moves.");

static const u8 sRockiumZDesc[] = _(
    "Upgrade Rock- type moves into\n"
    "Z-Moves.");

static const u8 sGhostiumZDesc[] = _(
    "Upgrade Ghost- type moves into\n"
    "Z-Moves.");

static const u8 sDragoniumZDesc[] = _(
    "Upgrade Dragon- type moves into\n"
    "Z-Moves.");

static const u8 sDarkiniumZDesc[] = _(
    "Upgrade Dark- type moves into\n"
    "Z-Moves.");

static const u8 sSteeliumZDesc[] = _(
    "Upgrade Steel- type moves into\n"
    "Z-Moves.");

static const u8 sFairiumZDesc[] = _(
    "Upgrade Fairy- type moves into\n"
    "Z-Moves.");

static const u8 sPikaniumZDesc[] = _(
    "Upgrade Pikachu's Volt Tackle into\n"
    "a Z-Move.");

static const u8 sEeviumZDesc[] = _(
    "Upgrade Eevee's Last Resort into a\n"
    "Z-Move.");

static const u8 sSnorliumZDesc[] = _(
    "Upgrade Snorlax's Giga Impact into\n"
    "a Z-Move.");

static const u8 sMewniumZDesc[] = _(
    "Upgrade Mew's Psychic into a\n"
    "Z-Move.");

static const u8 sDecidiumZDesc[] = _(
    "Upgrade Decidu- eye's Spirit Sha-\n"
    "ckle into a Z-Move.");

static const u8 sInciniumZDesc[] = _(
    "Upgrade Incine- roar's Darkest La-\n"
    "riat into a Z-Move.");

static const u8 sPrimariumZDesc[] = _(
    "Upgrade Primarina's Sparkling Aria\n"
    "into a Z-Move.");

static const u8 sLycaniumZDesc[] = _(
    "Upgrade Lycanroc's Stone Edge into\n"
    "a Z-Move.");

static const u8 sMimikiumZDesc[] = _(
    "Upgrade Mimikyu's Play Rough into\n"
    "a Z-Move.");

static const u8 sKommoniumZDesc[] = _(
    "Upgrade Kommo-o's Clanging Scales\n"
    "into a Z-Move.");

static const u8 sTapuniumZDesc[] = _(
    "Upgrade the tapu's Nature's\n"
    "Madness into a Z-Move.");

static const u8 sSolganiumZDesc[] = _(
    "Upgrade Solgaleo's Sunsteel Strike\n"
    "into a Z-Move.");

static const u8 sLunaliumZDesc[] = _(
    "Upgrade Lunala's Moongeist Beam\n"
    "into a Z-Move.");

static const u8 sMarshadiumZDesc[] = _(
    "Upgrade Marsha- dow's Spectral\n"
    "Thi- ef into a Z-Move.");

static const u8 sAloraichiumZDesc[] = _(
    "Upgrade Alolan Raichu's Thunder-\n"
    "bolt into a Z-Move.");

static const u8 sPikashuniumZDesc[] = _(
    "Upgrade Pikachu w/ a cap's\n"
    "Thunderbolt into a Z-Move.");

static const u8 sUltranecroziumZDesc[] = _(
    "A crystal to turn fused Necrozma\n"
    "into a new form.");

// Species-specific Held Items
static const u8 sLightBallDesc[] = _(
    "A hold item that raises the Atk\n"
    "and Sp. Atk of Pikachu.");

static const u8 sLeekDesc[] = _(
    "A hold item that raises\n"
    "Farfetch'd's critical-hit ratio.");

static const u8 sThickClubDesc[] = _(
    "A hold item that  raises Cubone or\n"
    "Marowak's Attack.");

static const u8 sLuckyPunchDesc[] = _(
    "A hold item that raises Chansey's\n"
    "critical-hit rate.");

static const u8 sMetalPowderDesc[] = _(
    "A hold item that raises Ditto's\n"
    "Defense.");

static const u8 sQuickPowderDesc[] = _(
    "An item to be held by Ditto. This\n"
    "odd powder boosts Speed.");

static const u8 sDeepSeaScaleDesc[] = _(
    "A hold item that raises the Sp.\n"
    "Def of Clamperl.");

static const u8 sDeepSeaToothDesc[] = _(
    "A hold item that raises the Sp.\n"
    "Atk of Clamperl.");

static const u8 sSoulDewDesc[] = _(
#if B_SOUL_DEW_BOOST >= GEN_7
    "Powers up Latios' & Latias'\n"
    "Psychic and Dragon-type moves.");
#else
    "Hold item: raises Sp. Atk & Sp.\n"
    "Def of Latios & Latias.");
#endif

static const u8 sAdamantOrbDesc[] = _(
    "Boosts the power of Dialga's\n"
    "Dragon and Steel-type moves.");

static const u8 sLustrousOrbDesc[] = _(
    "Boosts the power of Palkia's\n"
    "Dragon and Water-type moves.");

static const u8 sGriseousOrbDesc[] = _(
    "Powers up Giratina's Dragon and\n"
    "Ghost- type moves.");

// Incenses
static const u8 sSeaIncenseDesc[] = _(
    "A hold item that slightly boosts\n"
    "Water-type moves.");

static const u8 sLaxIncenseDesc[] = _(
    "A hold item that slightly lowers\n"
    "the foe's accuracy.");

static const u8 sOddIncenseDesc[] = _(
    "A hold item that boosts Psychic-\n"
    "type moves.");

static const u8 sRockIncenseDesc[] = _(
    "A hold item that raises the power\n"
    "of Rock-type moves.");

static const u8 sFullIncenseDesc[] = _(
    "A held item that makes the holder\n"
    "move slower.");

static const u8 sWaveIncenseDesc[] = _(
    "A hold item that slightly boosts\n"
    "Water-type moves.");

static const u8 sRoseIncenseDesc[] = _(
    "A hold item that raises the power\n"
    "of Grass-type moves.");

static const u8 sLuckIncenseDesc[] = _(
    "Doubles money in battle if the\n"
    "holder takes part.");

static const u8 sPureIncenseDesc[] = _(
    "A hold item that helps repel wild\n"
    "Pokémon.");

// Contest Scarves
static const u8 sRedScarfDesc[] = _(
    "A hold item that raises Cool in\n"
    "Contests.");

static const u8 sBlueScarfDesc[] = _(
    "A hold item that raises Beauty in\n"
    "Contests.");

static const u8 sPinkScarfDesc[] = _(
    "A hold item that raises Cute in\n"
    "Contests.");

static const u8 sGreenScarfDesc[] = _(
    "A hold item that raises Smart in\n"
    "Contests.");

static const u8 sYellowScarfDesc[] = _(
    "A hold item that raises Tough in\n"
    "Contests.");

// EV Gain Modifiers
static const u8 sMachoBraceDesc[] = _(
    "A hold item that promotes growth,\n"
    "but reduces Speed.");

static const u8 sPowerWeightDesc[] = _(
    "A hold item that promotes HP gain,\n"
    "but reduces Speed.");

static const u8 sPowerBracerDesc[] = _(
    "A hold item that promotes Atk\n"
    "gain, but reduces Speed.");

static const u8 sPowerBeltDesc[] = _(
    "A hold item that promotes Def\n"
    "gain, but reduces Speed.");

static const u8 sPowerLensDesc[] = _(
    "Hold item that pro- motes Sp. Atk\n"
    "gain, but reduces Speed.");

static const u8 sPowerBandDesc[] = _(
    "Hold item that pro- motes Sp. Def\n"
    "gain, but reduces Speed.");

static const u8 sPowerAnkletDesc[] = _(
    "A hold item that promotes Spd\n"
    "gain, but reduces Speed.");

// Type-boosting Held Items
static const u8 sSilkScarfDesc[] = _(
    "A hold item that raises the power\n"
    "of Normal-type moves.");

static const u8 sCharcoalDesc[] = _(
    "A hold item that raises the power\n"
    "of Fire-type moves.");

static const u8 sMysticWaterDesc[] = _(
    "A hold item that raises the power\n"
    "of Water-type moves.");

static const u8 sMagnetDesc[] = _(
    "A hold item that boosts Electric-\n"
    "type moves.");

static const u8 sMiracleSeedDesc[] = _(
    "A hold item that raises the power\n"
    "of Grass-type moves.");

static const u8 sNeverMeltIceDesc[] = _(
    "A hold item that raises the power\n"
    "of Ice-type moves.");

static const u8 sBlackBeltDesc[] = _(
    "A hold item that boosts Fighting-\n"
    "type moves.");

static const u8 sPoisonBarbDesc[] = _(
    "A hold item that raises the power\n"
    "of Poison-type moves.");

static const u8 sSoftSandDesc[] = _(
    "A hold item that raises the power\n"
    "of Ground-type moves.");

static const u8 sSharpBeakDesc[] = _(
    "A hold item that raises the power\n"
    "of Flying-type moves.");

static const u8 sTwistedSpoonDesc[] = _(
    "A hold item that boosts Psychic-\n"
    "type moves.");

static const u8 sSilverPowderDesc[] = _(
    "A hold item that raises the power\n"
    "of Bug-type moves.");

static const u8 sHardStoneDesc[] = _(
    "A hold item that raises the power\n"
    "of Rock-type moves.");

static const u8 sSpellTagDesc[] = _(
    "A hold item that raises the power\n"
    "of Ghost-type moves.");

static const u8 sDragonFangDesc[] = _(
    "A hold item that raises the power\n"
    "of Dragon-type moves.");

static const u8 sBlackGlassesDesc[] = _(
    "A hold item that raises the power\n"
    "of Dark-type moves.");

static const u8 sMetalCoatDesc[] = _(
    "A hold item that raises the power\n"
    "of Steel-type moves.");

// Choice Items
static const u8 sChoiceBandDesc[] = _(
    "Raises a move's power, but permits\n"
    "only that move.");

static const u8 sChoiceSpecsDesc[] = _(
    "Boosts Sp. Atk, but allows the use\n"
    "of only one move.");

static const u8 sChoiceScarfDesc[] = _(
    "Boosts Speed, but allows the use\n"
    "of only one move.");

// Status Orbs
static const u8 sFlameOrbDesc[] = _(
    "A bizarre orb that inflicts a burn\n"
    "on holder in battle.");

static const u8 sToxicOrbDesc[] = _(
    "A bizarre orb that badly poisons\n"
    "the holder in battle.");

// Weather Rocks
static const u8 sDampRockDesc[] = _(
    "Extends the length of Rain Dance\n"
    "if used by the holder.");

static const u8 sHeatRockDesc[] = _(
    "Extends the length of Sunny Day if\n"
    "used by the holder.");

static const u8 sSmoothRockDesc[] = _(
    "Extends the length of Sandstorm if\n"
    "used by the holder.");

static const u8 sIcyRockDesc[] = _(
    "Extends the length of the move\n"
    "Hail used by the holder.");

// Terrain Seeds
static const u8 sElectricSeedDesc[] = _(
    "Boosts Defense on Electric\n"
    "Terrain, but only one time.");

static const u8 sPsychicSeedDesc[] = _(
    "Boosts Sp. Def. on Psychic\n"
    "Terrain, but only one time.");

static const u8 sMistySeedDesc[] = _(
    "Boosts Sp. Def. on Misty Terrain,\n"
    "but only one time.");

static const u8 sGrassySeedDesc[] = _(
    "Boosts Defense on Grassy Terrain,\n"
    "but only one time.");

// Type-activated Stat Modifiers
static const u8 sAbsorbBulbDesc[] = _(
    "Raises Sp. Atk if the holder is\n"
    "hit by a Water-type move.");

static const u8 sCellBatteryDesc[] = _(
    "Raises Atk if the holder is hit by\n"
    "an Electric-type move.");

static const u8 sLuminousMossDesc[] = _(
    "Raises Sp. Def if the holder is\n"
    "hit by a Water-type move.");

static const u8 sSnowballDesc[] = _(
    "Raises Atk if its holder is hit by\n"
    "an Ice-type move.");

// Misc. Held Items
static const u8 sBrightPowderDesc[] = _(
    "A hold item that casts a glare to\n"
    "reduce accuracy.");

static const u8 sWhiteHerbDesc[] = _(
    "A hold item that restores any\n"
    "lowered stat.");

static const u8 sExpShareDesc[] = _(
    "A hold item that gets Exp. points\n"
    "from battles.");

static const u8 sQuickClawDesc[] = _(
    "A hold item that occasionally\n"
    "allows the first strike.");

static const u8 sSootheBellDesc[] = _(
    "A hold item that calms spirits and\n"
    "fosters friendship.");

#if B_MENTAL_HERB >= GEN_5
static const u8 sMentalHerbDesc[] = _(
    "Snaps Pokémon out of move-binding\n"
    "effects.");
#else
static const u8 sMentalHerbDesc[] = _(
    "A hold item that snaps Pokémon out\n"
    "of infatuation.");
#endif

static const u8 sKingsRockDesc[] = _(
    "A hold item that may cause\n"
    "flinching when the foe is hit.");

static const u8 sAmuletCoinDesc[] = _(
    "Doubles money in battle if the\n"
    "holder takes part.");

static const u8 sCleanseTagDesc[] = _(
    "A hold item that helps repel wild\n"
    "Pokémon.");

static const u8 sSmokeBallDesc[] = _(
    "A hold item that assures fleeing\n"
    "from wild Pokémon.");

static const u8 sFocusBandDesc[] = _(
    "A hold item that occasionally\n"
    "prevents fainting.");

static const u8 sLuckyEggDesc[] = _(
    "A hold item that boosts Exp.\n"
    "points earned in battle.");

static const u8 sScopeLensDesc[] = _(
    "A hold item that improves the\n"
    "critical-hit rate.");

static const u8 sLeftoversDesc[] = _(
    "A hold item that gradually\n"
    "restores HP in battle.");

static const u8 sShellBellDesc[] = _(
    "A hold item that restores HP upon\n"
    "striking the foe.");

static const u8 sWideLensDesc[] = _(
    "A magnifying lens that boosts the\n"
    "accuracy of moves.");

static const u8 sMuscleBandDesc[] = _(
    "A headband that boosts the power\n"
    "of physical moves.");

static const u8 sWiseGlassesDesc[] = _(
    "A pair of glasses that ups the\n"
    "power of special moves.");

static const u8 sExpertBeltDesc[] = _(
    "A belt that boosts the power of\n"
    "super effective moves.");

static const u8 sLightClayDesc[] = _(
    "Extends the length of barrier\n"
    "moves used by the holder.");

static const u8 sLifeOrbDesc[] = _(
    "Boosts the power of moves at the\n"
    "cost of some HP per turn.");

static const u8 sPowerHerbDesc[] = _(
    "Allows immediate use of a move\n"
    "that charges first.");

static const u8 sFocusSashDesc[] = _(
    "If the holder has full HP, it\n"
    "endures KO hits with 1 HP.");

static const u8 sZoomLensDesc[] = _(
    "If the holder moves after the foe,\n"
    "it'll boost accuracy.");

static const u8 sMetronomeDesc[] = _(
    "A held item that boosts a move\n"
    "used consecutively.");

static const u8 sIronBallDesc[] = _(
    "Cuts Speed and lets Flying-types\n"
    "be hit by Ground moves.");

static const u8 sLaggingTailDesc[] = _(
    "A held item that makes the holder\n"
    "move slower.");

static const u8 sDestinyKnotDesc[] = _(
    "If the holder falls in love, the\n"
    "foe does too.");

static const u8 sBlackSludgeDesc[] = _(
    "Gradually restores HP of Poison-\n"
    "types. Damages others.");

static const u8 sGripClawDesc[] = _(
    "Makes binding moves used by the\n"
    "holder go on for 7 turns.");

static const u8 sStickyBarbDesc[] = _(
    "Damages the holder each turn. May\n"
    "latch on to foes.");

static const u8 sShedShellDesc[] = _(
    "Enables the holder to switch out\n"
    "of battle without fail.");

static const u8 sBigRootDesc[] = _(
    "A held item that boosts the power\n"
    "of HP-stealing moves.");

static const u8 sRazorClawDesc[] = _(
    "A hooked claw that ups the\n"
    "holder's critical-hit ratio.");

static const u8 sRazorFangDesc[] = _(
    "A hold item that may cause\n"
    "flinching when the foe is hit.");

static const u8 sEvioliteDesc[] = _(
    "Raises the Def and Sp. Def of\n"
    "Pokémon that can evolve.");

static const u8 sFloatStoneDesc[] = _(
    "It's so light that when held, it\n"
    "halves a Pokémon's weight.");

static const u8 sRockyHelmetDesc[] = _(
    "Hurts the foe if they touch its\n"
    "holder.");

static const u8 sAirBalloonDesc[] = _(
    "Elevates the holder in the air. If\n"
    "hit, this item will burst.");

static const u8 sRedCardDesc[] = _(
    "Switches out the foe if they hit\n"
    "the holder.");

static const u8 sRingTargetDesc[] = _(
    "Moves that wouldn't have effect\n"
    "will land on its holder.");

static const u8 sBindingBandDesc[] = _(
    "Increases the power of binding\n"
    "moves when held.");

static const u8 sEjectButtonDesc[] = _(
    "Switches out the user if they're\n"
    "hit by the foe.");

static const u8 sWeaknessPolicyDesc[] = _(
    "If hit by a Super Effective move,\n"
    "ups Atk and Sp. Atk.");

static const u8 sAssaultVestDesc[] = _(
    "Raises Sp. Def but prevents the\n"
    "use of status moves.");

static const u8 sSafetyGogglesDesc[] = _(
    "Protect from weather damage and\n"
    "powder moves.");

static const u8 sAdrenalineOrbDesc[] = _(
    "Boosts Speed if the user is\n"
    "intimidated, but only one time.");

static const u8 sTerrainExtenderDesc[] = _(
    "Extends the length of the active\n"
    "battle terrain.");

static const u8 sProtectivePadsDesc[] = _(
    "Guard the holder from contact move\n"
    "effects.");

static const u8 sThroatSprayDesc[] = _(
    "Raises Sp. Atk. if a Pokémon is\n"
    "hit by a sound-based move.");

static const u8 sEjectPackDesc[] = _(
    "Forces the user to switch if its\n"
    "stats are lowered.");

static const u8 sHeavyDutyBootsDesc[] = _(
    "Boots that prevent effects of\n"
    "traps set in the field.");

static const u8 sBlunderPolicyDesc[] = _(
    "Raises Speed if the user misses\n"
    "due to Accuracy.");

static const u8 sRoomServiceDesc[] = _(
    "Lowers Speed if Trick Room is\n"
    "active.");

static const u8 sUtilityUmbrellaDesc[] = _(
    "An umbrella that protects from\n"
    "weather effects.");

// Berries
static const u8 sCheriBerryDesc[] = _(
    "A hold item that heals paralysis\n"
    "in battle.");

static const u8 sChestoBerryDesc[] = _(
    "A hold item that awakens Pokémon\n"
    "in battle.");

static const u8 sPechaBerryDesc[] = _(
    "A hold item that heals poisoning\n"
    "in battle.");

static const u8 sRawstBerryDesc[] = _(
    "A hold item that heals a burn in\n"
    "battle.");

static const u8 sAspearBerryDesc[] = _(
    "A hold item that defrosts Pokémon\n"
    "in battle.");

static const u8 sLeppaBerryDesc[] = _(
    "A hold item that restores 10 PP in\n"
    "battle.");

static const u8 sOranBerryDesc[] = _(
    "A hold item that restores 10 HP in\n"
    "battle.");

static const u8 sPersimBerryDesc[] = _(
    "A hold item that heals confusion\n"
    "in battle.");

static const u8 sLumBerryDesc[] = _(
    "A hold item that heals any status\n"
    "problem in battle.");

static const u8 sSitrusBerryDesc[] = _(
#if I_SITRUS_BERRY_HEAL >= GEN_4
    "A hold item that restores the\n"
    "user's HP a little.");
#else
    "A hold item that restores 30 HP in\n"
    "battle.");
#endif

static const u8 sFigyBerryDesc[] = _(
    "A hold item that restores HP but\n"
    "may confuse.");

static const u8 sWikiBerryDesc[] = _(
    "A hold item that restores HP but\n"
    "may confuse.");

static const u8 sMagoBerryDesc[] = _(
    "A hold item that restores HP but\n"
    "may confuse.");

static const u8 sAguavBerryDesc[] = _(
    "A hold item that restores HP but\n"
    "may confuse.");

static const u8 sIapapaBerryDesc[] = _(
    "A hold item that restores HP but\n"
    "may confuse.");

static const u8 sRazzBerryDesc[] = _(
    "{POKEBLOCK} ingredient. Plant in\n"
    "loamy soil to grow Razz.");

static const u8 sBlukBerryDesc[] = _(
    "{POKEBLOCK} ingredient. Plant in\n"
    "loamy soil to grow Bluk.");

static const u8 sNanabBerryDesc[] = _(
    "{POKEBLOCK} ingredient. Plant in\n"
    "loamy soil to grow Nanab.");

static const u8 sWepearBerryDesc[] = _(
    "{POKEBLOCK} ingredient. Plant in\n"
    "loamy soil to grow Wepear.");

static const u8 sPinapBerryDesc[] = _(
    "{POKEBLOCK} ingredient. Plant in\n"
    "loamy soil to grow Pinap.");

static const u8 sPomegBerryDesc[] = _(
    "Makes a Pokémon friendly but\n"
    "lowers base HP.");

static const u8 sKelpsyBerryDesc[] = _(
    "Makes a Pokémon friendly but\n"
    "lowers base Attack.");

static const u8 sQualotBerryDesc[] = _(
    "Makes a Pokémon friendly but\n"
    "lowers base Defense.");

static const u8 sHondewBerryDesc[] = _(
    "Makes a Pokémon friendly but\n"
    "lowers base Sp. Atk.");

static const u8 sGrepaBerryDesc[] = _(
    "Makes a Pokémon friendly but\n"
    "lowers base Sp. Def.");

static const u8 sTamatoBerryDesc[] = _(
    "Makes a Pokémon friendly but\n"
    "lowers base Speed.");

static const u8 sCornnBerryDesc[] = _(
    "{POKEBLOCK} ingredient. Plant in\n"
    "loamy soil to grow Cornn.");

static const u8 sMagostBerryDesc[] = _(
    "{POKEBLOCK} ingredient. Plant in\n"
    "loamy soil to grow Magost.");

static const u8 sRabutaBerryDesc[] = _(
    "{POKEBLOCK} ingredient. Plant in\n"
    "loamy soil to grow Rabuta.");

static const u8 sNomelBerryDesc[] = _(
    "{POKEBLOCK} ingredient. Plant in\n"
    "loamy soil to grow Nomel.");

static const u8 sSpelonBerryDesc[] = _(
    "{POKEBLOCK} ingredient. Plant in\n"
    "loamy soil to grow Spelon.");

static const u8 sPamtreBerryDesc[] = _(
    "{POKEBLOCK} ingredient. Plant in\n"
    "loamy soil to grow Pamtre.");

static const u8 sWatmelBerryDesc[] = _(
    "{POKEBLOCK} ingredient. Plant in\n"
    "loamy soil to grow Watmel.");

static const u8 sDurinBerryDesc[] = _(
    "{POKEBLOCK} ingredient. Plant in\n"
    "loamy soil to grow Durin.");

static const u8 sBelueBerryDesc[] = _(
    "{POKEBLOCK} ingredient. Plant in\n"
    "loamy soil to grow Belue.");

static const u8 sChilanBerryDesc[] = _(
    "A hold item that weakens a Normal\n"
    "move.");

static const u8 sOccaBerryDesc[] = _(
    "A hold item that weakens a Fire\n"
    "move if weak to it.");

static const u8 sPasshoBerryDesc[] = _(
    "A hold item that weakens a Water\n"
    "move if weak to it.");

static const u8 sWacanBerryDesc[] = _(
    "A hold item that weakens a\n"
    "Electric move if weak to it.");

static const u8 sRindoBerryDesc[] = _(
    "A hold item that weakens a Grass\n"
    "move if weak to it.");

static const u8 sYacheBerryDesc[] = _(
    "A hold item that weakens a Ice\n"
    "move if weak to it.");

static const u8 sChopleBerryDesc[] = _(
    "A hold item that weakens a\n"
    "Fighting move if weak to it.");

static const u8 sKebiaBerryDesc[] = _(
    "A hold item that weakens a Poison\n"
    "move if weak to it.");

static const u8 sShucaBerryDesc[] = _(
    "A hold item that weakens a Ground\n"
    "move if weak to it.");

static const u8 sCobaBerryDesc[] = _(
    "A hold item that weakens a Flying\n"
    "move if weak to it.");

static const u8 sPayapaBerryDesc[] = _(
    "A hold item that weakens a Psychic\n"
    "move if weak to it.");

static const u8 sTangaBerryDesc[] = _(
    "A hold item that weakens a Bug\n"
    "move if weak to it.");

static const u8 sChartiBerryDesc[] = _(
    "A hold item that weakens a Rock\n"
    "move if weak to it.");

static const u8 sKasibBerryDesc[] = _(
    "A hold item that weakens a Ghost\n"
    "move if weak to it.");

static const u8 sHabanBerryDesc[] = _(
    "A hold item that weakens a Dragon\n"
    "move if weak to it.");

static const u8 sColburBerryDesc[] = _(
    "A hold item that weakens a Dark\n"
    "move if weak to it.");

static const u8 sBabiriBerryDesc[] = _(
    "A hold item that weakens a Steel\n"
    "move if weak to it.");

static const u8 sRoseliBerryDesc[] = _(
    "A hold item that weakens a Fairy\n"
    "move if weak to it.");

static const u8 sLiechiBerryDesc[] = _(
    "A hold item that raises Attack in\n"
    "a pinch.");

static const u8 sGanlonBerryDesc[] = _(
    "A hold item that raises Defense in\n"
    "a pinch.");

static const u8 sSalacBerryDesc[] = _(
    "A hold item that raises Speed in a\n"
    "pinch.");

static const u8 sPetayaBerryDesc[] = _(
    "A hold item that raises Sp. Atk in\n"
    "a pinch.");

static const u8 sApicotBerryDesc[] = _(
    "A hold item that raises Sp. Def in\n"
    "a pinch.");

static const u8 sLansatBerryDesc[] = _(
    "A hold item that ups the critical-\n"
    "hit rate in a pinch.");

static const u8 sStarfBerryDesc[] = _(
    "A hold item that sharply boosts a\n"
    "stat in a pinch.");

static const u8 sEnigmaBerryDesc[] = _(
    "A hold item that heals from super\n"
    "effective moves.");

static const u8 sMicleBerryDesc[] = _(
    "When held, it ups the Accuracy of\n"
    "a move in a pinch.");

static const u8 sCustapBerryDesc[] = _(
    "It allows a Pokémon in a pinch to\n"
    "move first just once.");

static const u8 sJabocaBerryDesc[] = _(
    "If hit by a physical move, it will\n"
    "hurt the attacker a bit.");

static const u8 sRowapBerryDesc[] = _(
    "If hit by a special move, it will\n"
    "hurt the attacker a bit.");

static const u8 sKeeBerryDesc[] = _(
    "If hit by a physical move, it\n"
    "raises the Defense a bit.");

static const u8 sMarangaBerryDesc[] = _(
    "If hit by a special move, it\n"
    "raises the Sp. Def. a bit.");

static const u8 sEnigmaBerryEReaderDesc[] = _(
    "{POKEBLOCK} ingredient. Plant in\n"
    "loamy soil to grow a mystery.");

// Charms
static const u8 sOvalCharmDesc[] = _(
    "Raises the chance of finding eggs\n"
    "at the daycare.");

static const u8 sShinyCharmDesc[] = _(
    "A charm that will raise the chance\n"
    "of Shiny Pokémon.");

static const u8 sCatchingCharmDesc[] = _(
    "A charm that raises the chance of\n"
    "Critical Captures.");

static const u8 sExpCharmDesc[] = _(
    "A charm that raises the amount of\n"
    "Exp. earned in battle.");

// Form-changing Key Items
static const u8 sRotomCatalogDesc[] = _(
    "A catalog full of deviced liked by\n"
    "Rotom.");

static const u8 sGracideaDesc[] = _(
    "Bouquets made with it are offered\n"
    "as a token of gratitude.");

static const u8 sRevealGlassDesc[] = _(
    "This glass returns a Pokémon back\n"
    "to its original form.");

static const u8 sDNASplicersDesc[] = _(
    "Splicer that fuses Kyurem and a\n"
    "certain Pokémon.");

static const u8 sZygardeCubeDesc[] = _(
    "An item to store Zygarde Cores and\n"
    "Cells.");

static const u8 sPrisonBottleDesc[] = _(
    "A bottle used to seal a certain\n"
    "Pokémon long ago.");

static const u8 sNSolarizerDesc[] = _(
    "A device to fuse and split\n"
    "Necrozma using a Solgaleo.");

static const u8 sNLunarizerDesc[] = _(
    "A device to fuse and split\n"
    "Necrozma using a Lunala.");

static const u8 sReinsOfUnityDesc[] = _(
    "Reins that unite Calyrex with its\n"
    "beloved steed.");

// Battle Mechanic Key Items
static const u8 sMegaRingDesc[] = _(
    "Enables {PKMN} holding their Mega\n"
    "Stone to Mega Evolve.");

static const u8 sZPowerRingDesc[] = _(
    "A strange ring that enables Z-Move\n"
    "usage.");

static const u8 sDynamaxBandDesc[] = _(
    "A band carrying a Wishing Star\n"
    "that allows Dynamaxing.");

// Misc. Key Items
static const u8 sBicycleDesc[] = _(
    "A folding bicycle that is faster\n"
    "than the Running Shoes.");

static const u8 sMachBikeDesc[] = _(
    "A folding bicycle that doubles\n"
    "your speed or better.");

static const u8 sAcroBikeDesc[] = _(
    "A folding bicycle capable of jumps\n"
    "and wheelies.");

static const u8 sOldRodDesc[] = _(
    "Use by any body of water to fish\n"
    "for wild Pokémon.");

static const u8 sGoodRodDesc[] = _(
    "A decent fishing rod for catching\n"
    "wild Pokémon.");

static const u8 sSuperRodDesc[] = _(
    "The best fishing rod for catching\n"
    "wild Pokémon.");

static const u8 sDowsingMachineDesc[] = _(
    "A device that signals an invisible\n"
    "item by sound.");

static const u8 sTownMapDesc[] = _(
    "Can be viewed anytime. Shows your\n"
    "present location.");

static const u8 sVsSeekerDesc[] = _(
    "A rechargeable unit that flags\n"
    "battle- ready Trainers.");

static const u8 sTMCaseDesc[] = _(
    "A convenient case  that holds TMs\n"
    "and HMs.");

static const u8 sBerryPouchDesc[] = _(
    "A convenient container that holds\n"
    "Berries.");

static const u8 sPokemonBoxLinkDesc[] = _(
    "This device grants access to the\n"
    "{PKMN} Storage System.");

static const u8 sCoinCaseDesc[] = _(
    "A case that holds up to 9,999\n"
    "Coins.");

static const u8 sPowderJarDesc[] = _(
    "Stores Berry Powder made using a\n"
    "Berry Crusher.");

static const u8 sWailmerPailDesc[] = _(
    "A tool used for watering Berries\n"
    "and plants.");

static const u8 sPokeRadarDesc[] = _(
    "A tool used to search out Pokémon\n"
    "hiding in grass.");

static const u8 sPokeblockCaseDesc[] = _(
    "A case for holding {POKEBLOCK}s\n"
    "made with a Berry Blender.");

static const u8 sSootSackDesc[] = _(
    "A sack used to gather and hold\n"
    "volcanic ash.");

static const u8 sPokeFluteDesc[] = _(
    "A sweet-sounding flute that\n"
    "awakens Pokémon.");

static const u8 sFameCheckerDesc[] = _(
    "Stores information on famous\n"
    "people for instant recall.");

static const u8 sTeachyTVDesc[] = _(
    "A TV set tuned to an advice\n"
    "program for Trainers.");

// Story Key Items
static const u8 sSSTicketDesc[] = _(
    "The ticket required for sailing on\n"
    "a ferry.");

static const u8 sEonTicketDesc[] = _(
    "The ticket for a ferry to a\n"
    "distant southern island.");

static const u8 sMysticTicketDesc[] = _(
    "A ticket required to board the\n"
    "ship to Navel Rock.");

static const u8 sAuroraTicketDesc[] = _(
    "A ticket required to board the\n"
    "ship to Birth Island.");

static const u8 sOldSeaMapDesc[] = _(
    "A faded sea chart that shows the\n"
    "way to a certain island.");

static const u8 sLetterDesc[] = _(
    "A letter to Steven from the\n"
    "President of the Devon Corp.");

static const u8 sDevonPartsDesc[] = _(
    "A package that contains Devon's\n"
    "machine parts.");

static const u8 sGoGogglesDesc[] = _(
    "Nifty goggles that protect eyes\n"
    "from desert sandstorms.");

static const u8 sDevonScopeDesc[] = _(
    "A device by Devon that signals any\n"
    "unseeable Pokémon.");

static const u8 sBasementKeyDesc[] = _(
    "The key for New Mauville beneath\n"
    "Mauville City.");

static const u8 sScannerDesc[] = _(
    "A device found inside the\n"
    "Abandoned Ship.");

static const u8 sStorageKeyDesc[] = _(
    "The key to the storage inside the\n"
    "Abandoned Ship.");

static const u8 sKeyToRoom1Desc[] = _(
    "A key that opens a door inside the\n"
    "Abandoned Ship.");

static const u8 sKeyToRoom2Desc[] = _(
    "A key that opens a door inside the\n"
    "Abandoned Ship.");

static const u8 sKeyToRoom4Desc[] = _(
    "A key that opens a door inside the\n"
    "Abandoned Ship.");

static const u8 sKeyToRoom6Desc[] = _(
    "A key that opens a door inside the\n"
    "Abandoned Ship.");

static const u8 sMeteoriteDesc[] = _(
    "A meteorite found at Meteor Falls.\n");

static const u8 sMagmaEmblemDesc[] = _(
    "A medal-like item in the same\n"
    "shape as Team Magma's mark.");

static const u8 sContestPassDesc[] = _(
    "The pass required for entering\n"
    "Pokémon Contests.");

static const u8 sOaksParcelDesc[] = _(
    "A parcel for Prof. Oak from a\n"
    "Pokémon Mart's clerk.");

static const u8 sSecretKeyDesc[] = _(
    "The key to the Cinnabar Island\n"
    "Gym's entrance.");

static const u8 sBikeVoucherDesc[] = _(
    "A voucher for obtaining a bicycle\n"
    "from the Bike Shop.");

static const u8 sGoldTeethDesc[] = _(
    "Gold dentures lost by the Safari\n"
    "Zone's Warden.");

static const u8 sCardKeyDesc[] = _(
    "A card-type door key used in Silph\n"
    "Co's office.");

static const u8 sLiftKeyDesc[] = _(
    "An elevator key used in Team\n"
    "Rocket's Hideout.");

static const u8 sSilphScopeDesc[] = _(
    "Silph Co's scope makes unseeable\n"
    "POKéMON visible.");

static const u8 sTriPassDesc[] = _(
    "A pass for ferries between One,\n"
    "Two, and Three Island.");

static const u8 sRainbowPassDesc[] = _(
    "For ferries serving Vermilion and\n"
    "the Sevii Islands.");

static const u8 sTeaDesc[] = _(
    "A thirst-quenching tea prepared by\n"
    "an old lady.");

static const u8 sRubyDesc[] = _(
    "An exquisite, red- glowing gem\n"
    "that symbolizes passion.");

static const u8 sSapphireDesc[] = _(
    "A brilliant blue gem that\n"
    "symbolizes honesty.");
