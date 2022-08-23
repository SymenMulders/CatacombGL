// Copyright (C) 2022 Arno Ansems
// 
// This program is free software: you can redistribute it and/or modify 
// it under the terms of the GNU General Public License as published by 
// the Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version. 
// 
// This program is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
// GNU General Public License for more details. 
// 
// You should have received a copy of the GNU General Public License 
// along with this program.  If not, see http://www.gnu.org/licenses/ 

#include "SavedGameConverterAbyss.h"
#include "DecorateAll.h"

// obclass definitions as seen in DEF.H
constexpr uint16_t obclassNothing = 0;
constexpr uint16_t obclassPlayer = 1;
constexpr uint16_t obclassBonus = 2;
constexpr uint16_t obclassOrc = 3;
constexpr uint16_t obclassBat = 4;
constexpr uint16_t obclassSkeleton = 5;
constexpr uint16_t obclassTroll = 6;
constexpr uint16_t obclassDemon = 7;
constexpr uint16_t obclassMage = 8;
constexpr uint16_t obclassPlayerShot = 9;
constexpr uint16_t obclassBigPlayerShot = 10;
constexpr uint16_t obclassMageShot = 11;
constexpr uint16_t obclassInert = 12;
constexpr uint16_t obclassBounce = 13;
constexpr uint16_t obclassGrelminar = 14;
constexpr uint16_t obclassGate = 15;
constexpr uint16_t obclassZombie = 16;
constexpr uint16_t obclassSpook = 17;
constexpr uint16_t obclassWet = 18;
constexpr uint16_t obclassExp = 19;
constexpr uint16_t obclassEye = 20;
constexpr uint16_t obclassWallSkeleton = 21;
constexpr uint16_t obclassEyeShot = 22;
constexpr uint16_t obclassGrelminarShot = 23;
constexpr uint16_t obclassRedDemon = 24;
constexpr uint16_t obclassFreeze = 25;
constexpr uint16_t obclassSolid = 26;

// State pointers as conveniently documented in Reflection Keen (statetype_ptr_conversion.c).
// State pointers are stored as an array, as different versions of Catacomb Abyss use different pointers.
// The arrays contain { v1.13, v1.24 }.
constexpr uint16_t s_pshot1[] = { 0x1F0A, 0x1EC4 };
constexpr uint16_t s_pshot2[] = { 0x1F14, 0x1ECE };
constexpr uint16_t s_pshot_exp1[] = { 0x1F1E, 0x1ED8 };
constexpr uint16_t s_pshot_exp2[] = { 0x1F28, 0x1EE2 };
constexpr uint16_t s_pshot_exp3[] = { 0x1F32, 0x1EEC };
constexpr uint16_t s_player[] = { 0x1F48, 0x1EEC };
constexpr uint16_t s_explode[] = { 0x1F52, 0x1F0C };
constexpr uint16_t s_boltbonus[] = { 0x2060, 0x201A };
constexpr uint16_t s_boltbonus2[] = { 0x206A, 0x2024 };
constexpr uint16_t s_boltbonus3[] = { 0x2074, 0x202E };
constexpr uint16_t s_nukebonus[] = { 0x207E, 0x2038 };
constexpr uint16_t s_nukebonus2[] = { 0x2088, 0x2042 };
constexpr uint16_t s_nukebonus3[] = { 0x2092, 0x204C };
constexpr uint16_t s_potionbonus[] = { 0x209C, 0x2056 };
constexpr uint16_t s_rkey2bonus[] = { 0x20A6, 0x2060 };
constexpr uint16_t s_rkeybonus[] = { 0x20B0, 0x206A };
constexpr uint16_t s_ykeybonus[] = { 0x20BA, 0x2074 };
constexpr uint16_t s_gkeybonus[] = { 0x20C4, 0x207E };
constexpr uint16_t s_bkeybonus[] = { 0x20CE, 0x2088 };
constexpr uint16_t s_scrollbonus[] = { 0x20D8, 0x2092 };
constexpr uint16_t s_chestbonus[] = { 0x20E2, 0x209C };
constexpr uint16_t s_waterchestbonus1[] = { 0x20EC, 0x20A6 };
constexpr uint16_t s_waterchestbonus2[] = { 0x20F6, 0x20B0 };
constexpr uint16_t s_rgem1bonus[] = { 0x2100, 0x20BA };
constexpr uint16_t s_ygem1bonus[] = { 0x210A, 0x20C4 };
constexpr uint16_t s_ggem1bonus[] = { 0x2114, 0x20CE };
constexpr uint16_t s_bgem1bonus[] = { 0x211E, 0x20D8 };
constexpr uint16_t s_pgem1bonus[] = { 0x2128, 0x20E2 };
constexpr uint16_t s_rgem2bonus[] = { 0x2132, 0x20EC };
constexpr uint16_t s_ygem2bonus[] = { 0x213C, 0x20F6 };
constexpr uint16_t s_ggem2bonus[] = { 0x2146, 0x2100 };
constexpr uint16_t s_bgem2bonus[] = { 0x2150, 0x210A };
constexpr uint16_t s_pgem2bonus[] = { 0x215A, 0x2114 };
constexpr uint16_t s_bonus_die[] = { 0x2164, 0x211E };
constexpr uint16_t s_tombs0[] = { 0x216E, 0x2128 };
constexpr uint16_t s_tombs1[] = { 0x2178, 0x2132 };
constexpr uint16_t s_tombs2[] = { 0x2182, 0x213C };
constexpr uint16_t s_ftimebonus[] = { 0x218C, 0x2146 };
constexpr uint16_t s_ftimebonus2[] = { 0x2196, 0x2150 };
constexpr uint16_t s_walldie1[] = { 0x21A0, 0x215A };
constexpr uint16_t s_walldie2[] = { 0x21AA, 0x2164 };
constexpr uint16_t s_walldie3[] = { 0x21B4, 0x216E };
constexpr uint16_t s_walldie4[] = { 0x21BE, 0x2178 };
constexpr uint16_t s_walldie5[] = { 0x21C8, 0x2182 };
constexpr uint16_t s_walldie6[] = { 0x21D2, 0x218C };
constexpr uint16_t s_obj_gate1[] = { 0x21DC, 0x2196 };
constexpr uint16_t s_obj_gate2[] = { 0x21E6, 0x21A0 };
constexpr uint16_t s_obj_gate3[] = { 0x21F0, 0x21AA };
constexpr uint16_t s_obj_gate4[] = { 0x21FA, 0x21B4 };
constexpr uint16_t s_pit[] = { 0x2204, 0x21BE };
constexpr uint16_t s_trollpause[] = { 0x220E, 0x21C8 };
constexpr uint16_t s_troll1[] = { 0x2218, 0x21D2 };
constexpr uint16_t s_troll2[] = { 0x2222, 0x21DC };
constexpr uint16_t s_troll3[] = { 0x222C, 0x21E6 };
constexpr uint16_t s_troll4[] = { 0x2236, 0x21F0 };
constexpr uint16_t s_trollattack1[] = { 0x2240, 0x21FA };
constexpr uint16_t s_trollattack2[] = { 0x224A, 0x2204 };
constexpr uint16_t s_trollattack3[] = { 0x2254, 0x220E };
constexpr uint16_t s_trollouch[] = { 0x225E, 0x2218 };
constexpr uint16_t s_trolldie1[] = { 0x2268, 0x2222 };
constexpr uint16_t s_trolldie2[] = { 0x2272, 0x222C };
constexpr uint16_t s_trolldie3[] = { 0x227C, 0x2236 };
constexpr uint16_t s_wet_bubbles1[] = { 0x2286, 0x2240 };
constexpr uint16_t s_wet_bubbles2[] = { 0x2290, 0x224A };
constexpr uint16_t s_wet_bubbles3[] = { 0x229A, 0x2254 };
constexpr uint16_t s_wet_peek[] = { 0x22A4, 0x225E };
constexpr uint16_t s_wet_rise1[] = { 0x22AE, 0x2268 };
constexpr uint16_t s_wet_rise3[] = { 0x22B8, 0x2272 };
constexpr uint16_t s_wet_rise4[] = { 0x22C2, 0x227C };
constexpr uint16_t s_wet_rise5[] = { 0x22CC, 0x2286 };
constexpr uint16_t s_wet_sink1[] = { 0x22D6, 0x2290 };
constexpr uint16_t s_wet_sink2[] = { 0x22E0, 0x229A };
constexpr uint16_t s_wet_sink3[] = { 0x22EA, 0x22A4 };
constexpr uint16_t s_wet_walk1[] = { 0x22F4, 0x22AE };
constexpr uint16_t s_wet_walk2[] = { 0x22FE, 0x22B8 };
constexpr uint16_t s_wet_walk3[] = { 0x2308, 0x22C2 };
constexpr uint16_t s_wet_walk4[] = { 0x2312, 0x22CC };
constexpr uint16_t s_wet_attack1[] = { 0x231C, 0x22D6 };
constexpr uint16_t s_wet_attack2[] = { 0x2326, 0x22E0 };
constexpr uint16_t s_wet_attack3[] = { 0x2330, 0x22EA };
constexpr uint16_t s_wet_attack4[] = { 0x233A, 0x22F4 };
constexpr uint16_t s_wet_ouch[] = { 0x2344, 0x22FE };
constexpr uint16_t s_wet_die1[] = { 0x234E, 0x2308 };
constexpr uint16_t s_wet_die2[] = { 0x2358, 0x2312 };
constexpr uint16_t s_wet_die3[] = { 0x2362, 0x231C };
constexpr uint16_t s_wet_die4[] = { 0x236C, 0x2326 };
constexpr uint16_t s_wet_die5[] = { 0x2376, 0x2330 };
constexpr uint16_t s_zombie_risen[] = { 0x2380, 0x233A };
constexpr uint16_t s_zombie_pause[] = { 0x238A, 0x2344 };
constexpr uint16_t s_zombie_inground[] = { 0x2394, 0x234E };
constexpr uint16_t s_zombie_rise1[] = { 0x239E, 0x2358 };
constexpr uint16_t s_zombie_rise2[] = { 0x23A8, 0x2362 };
constexpr uint16_t s_zombie_rise3[] = { 0x23B2, 0x236C };
constexpr uint16_t s_zombie_rise4[] = { 0x23BC, 0x2376 };
constexpr uint16_t s_zombie_alive1[] = { 0x23C6, 0x2380 };
constexpr uint16_t s_zombie_alive2[] = { 0x23D0, 0x238A };
constexpr uint16_t s_zombie_alive3[] = { 0x23DA, 0x2394 };
constexpr uint16_t s_zombie_death1[] = { 0x23E4, 0x239E };
constexpr uint16_t s_zombie_death2[] = { 0x23EE, 0x23A8 };
constexpr uint16_t s_zombie_death3[] = { 0x23F8, 0x23B2 };
constexpr uint16_t s_zombie_attack[] = { 0x2402, 0x23BC };
constexpr uint16_t s_zombie_ouch[] = { 0x240C, 0x23C6 };
constexpr uint16_t s_spook_wait[] = { 0x2416, 0x23D0 };
constexpr uint16_t s_spook0[] = { 0x2420, 0x23DA };
constexpr uint16_t s_spook0_1[] = { 0x242A, 0x23E4 };
constexpr uint16_t s_spook0_2[] = { 0x2434, 0x23EE };
constexpr uint16_t s_spook1[] = { 0x243E, 0x23F8 };
constexpr uint16_t s_spook2[] = { 0x2448, 0x2402 };
constexpr uint16_t s_spook3[] = { 0x2452, 0x240C };
constexpr uint16_t s_spook4[] = { 0x245C, 0x2416 };
constexpr uint16_t s_spook5[] = { 0x2466, 0x2420 };
constexpr uint16_t s_spook6[] = { 0x2470, 0x242A };
constexpr uint16_t s_spook_attack1[] = { 0x247A, 0x2434 };
constexpr uint16_t s_spook_attack3[] = { 0x2484, 0x243E };
constexpr uint16_t s_spook_pause[] = { 0x248E, 0x2448 };
constexpr uint16_t s_spookouch[] = { 0x2498, 0x2452 };
constexpr uint16_t s_spookdie[] = { 0x24A2, 0x245C };
constexpr uint16_t s_spookdie1[] = { 0x24AC, 0x2466 };
constexpr uint16_t s_spookdie2[] = { 0x24B6, 0x2470 };
constexpr uint16_t s_spookdie3[] = { 0x24C0, 0x247A };
constexpr uint16_t s_spookdie4[] = { 0x24CA, 0x2484 };
constexpr uint16_t s_spookdie5[] = { 0x24D4, 0x248E };
constexpr uint16_t s_wallskel[] = { 0x24DE, 0x2498 };
constexpr uint16_t s_wallskel2[] = { 0x24E8, 0x24A2 };
constexpr uint16_t s_skel_pause[] = { 0x24FA, 0x24B4 };
constexpr uint16_t s_skel_1[] = { 0x2504, 0x24BE };
constexpr uint16_t s_skel_2[] = { 0x250E, 0x24C8 };
constexpr uint16_t s_skel_3[] = { 0x2518, 0x24D2 };
constexpr uint16_t s_skel_4[] = { 0x2522, 0x24DC };
constexpr uint16_t s_skel_attack1[] = { 0x252C, 0x24E6 };
constexpr uint16_t s_skel_attack2[] = { 0x2536, 0x24F0 };
constexpr uint16_t s_skel_attack3[] = { 0x2540, 0x24FA };
constexpr uint16_t s_skel_ouch[] = { 0x254A, 0x2504 };
constexpr uint16_t s_skel_die1[] = { 0x2554, 0x250E };
constexpr uint16_t s_skel_die2[] = { 0x255E, 0x2518 };
constexpr uint16_t s_skel_die3[] = { 0x2568, 0x2522 };
constexpr uint16_t s_eye_pause[] = { 0x2572, 0x252C };
constexpr uint16_t s_eye_1[] = { 0x257C, 0x2536 };
constexpr uint16_t s_eye_2[] = { 0x2586, 0x2540 };
constexpr uint16_t s_eye_3[] = { 0x2590, 0x254A };
constexpr uint16_t s_eye_4[] = { 0x259A, 0x2554 };
constexpr uint16_t s_eye_ouch[] = { 0x25A4, 0x255E };
constexpr uint16_t s_eye_ouch2[] = { 0x25AE, 0x2568 };
constexpr uint16_t s_eye_die1[] = { 0x25B8, 0x2572 };
constexpr uint16_t s_eye_die2[] = { 0x25C2, 0x257C };
constexpr uint16_t s_eye_die3[] = { 0x25CC, 0x2586 };
constexpr uint16_t s_eshot1[] = { 0x25D6, 0x2590 };
constexpr uint16_t s_eshot2[] = { 0x25E0, 0x259A };
constexpr uint16_t s_orcpause[] = { 0x25EA, 0x25A4 };
constexpr uint16_t s_orc1[] = { 0x25F4, 0x25AE };
constexpr uint16_t s_orc2[] = { 0x25FE, 0x25B8 };
constexpr uint16_t s_orc3[] = { 0x2608, 0x25C2 };
constexpr uint16_t s_orc4[] = { 0x2612, 0x25CC };
constexpr uint16_t s_orcattack1[] = { 0x261C, 0x25D6 };
constexpr uint16_t s_orcattack2[] = { 0x2626, 0x25E0 };
constexpr uint16_t s_orcattack3[] = { 0x2630, 0x25EA };
constexpr uint16_t s_orcouch[] = { 0x263A, 0x25F4 };
constexpr uint16_t s_orcdie1[] = { 0x2644, 0x25FE };
constexpr uint16_t s_orcdie2[] = { 0x264E, 0x2608 };
constexpr uint16_t s_orcdie3[] = { 0x2658, 0x2612 };
constexpr uint16_t s_demonpause[] = { 0x2662, 0x261C };
constexpr uint16_t s_demon1[] = { 0x266C, 0x2626 };
constexpr uint16_t s_demon2[] = { 0x2676, 0x2630 };
constexpr uint16_t s_demon3[] = { 0x2680, 0x263A };
constexpr uint16_t s_demon4[] = { 0x268A, 0x2644 };
constexpr uint16_t s_demonattack1[] = { 0x2694, 0x264E };
constexpr uint16_t s_demonattack2[] = { 0x269E, 0x2658 };
constexpr uint16_t s_demonattack3[] = { 0x26A8, 0x2662 };
constexpr uint16_t s_demonouch[] = { 0x26B2, 0x266C };
constexpr uint16_t s_demondie1[] = { 0x26BC, 0x2676 };
constexpr uint16_t s_demondie2[] = { 0x26C6, 0x2680 };
constexpr uint16_t s_demondie3[] = { 0x26D0, 0x268A };
constexpr uint16_t s_magepause[] = { 0x26DA, 0x2694 };
constexpr uint16_t s_mage1[] = { 0x26E4, 0x269E };
constexpr uint16_t s_mage2[] = { 0x26EE, 0x26A8 };
constexpr uint16_t s_mageattack3[] = { 0x26F8, 0x26B2 };
constexpr uint16_t s_mageouch[] = { 0x2702, 0x26BC };
constexpr uint16_t s_magedie1[] = { 0x270C, 0x26C6 };
constexpr uint16_t s_magedie2[] = { 0x2716, 0x26D0 };
constexpr uint16_t s_mshot1[] = { 0x2720, 0x26DA };
constexpr uint16_t s_mshot2[] = { 0x272A, 0x26E4 };
constexpr uint16_t s_red_demonpause[] = { 0x2734, 0x26EE };
constexpr uint16_t s_red_demon1[] = { 0x273E, 0x26F8 };
constexpr uint16_t s_red_demon2[] = { 0x2748, 0x2702 };
constexpr uint16_t s_red_demon3[] = { 0x2752, 0x270C };
constexpr uint16_t s_red_demon4[] = { 0x275C, 0x2716 };
constexpr uint16_t s_red_demonattack1[] = { 0x2766, 0x2720 };
constexpr uint16_t s_red_demonattack2[] = { 0x2770, 0x272A };
constexpr uint16_t s_red_demonattack3[] = { 0x277A, 0x2734 };
constexpr uint16_t s_red_demonouch[] = { 0x2784, 0x273E };
constexpr uint16_t s_red_demondie1[] = { 0x278E, 0x2748 };
constexpr uint16_t s_red_demondie2[] = { 0x2798, 0x2752 };
constexpr uint16_t s_red_demondie3[] = { 0x27A2, 0x275C };
constexpr uint16_t s_grelpause[] = { 0x27AC, 0x2766 };
constexpr uint16_t s_grel1[] = { 0x27B6, 0x2770 };
constexpr uint16_t s_grel2[] = { 0x27C0, 0x277A };
constexpr uint16_t s_grelattack3[] = { 0x27CA, 0x2784 };
constexpr uint16_t s_grelouch[] = { 0x27D4, 0x278E };
constexpr uint16_t s_greldie1[] = { 0x27DE, 0x2798 };
constexpr uint16_t s_greldie2[] = { 0x27E8, 0x27A2 };
constexpr uint16_t s_greldie3[] = { 0x27F2, 0x27AC };
constexpr uint16_t s_greldie4[] = { 0x27FC, 0x27B6 };
constexpr uint16_t s_greldie5[] = { 0x2806, 0x27C0 };
constexpr uint16_t s_greldie5a[] = { 0x2810, 0x27CA };
constexpr uint16_t s_greldie6[] = { 0x281A, 0x27D4 };
constexpr uint16_t s_gshot1[] = { 0x2824, 0x27DE };
constexpr uint16_t s_bat1[] = { 0x282E, 0x27E8 };
constexpr uint16_t s_bat2[] = { 0x2838, 0x27F2 };
constexpr uint16_t s_bat3[] = { 0x2842, 0x27FC };
constexpr uint16_t s_bat4[] = { 0x284C, 0x2806 };
constexpr uint16_t s_batpast[] = { 0x2856, 0x2810 };
constexpr uint16_t s_batdie1[] = { 0x2860, 0x281A };
constexpr uint16_t s_batdie2[] = { 0x286A, 0x2824 };

SavedGameConverterAbyss::SavedGameConverterAbyss(const uint8_t gameId) :
    m_gameId(gameId)
{
}

SavedGameConverterAbyss::~SavedGameConverterAbyss()
{
}

const uint16_t SavedGameConverterAbyss::GetActorIdOfBonus(const uint16_t state16, const int16_t temp1) const
{
    const uint8_t i = (m_gameId == 0) ? 0 : 1;
    const uint16_t actorId =
        (state16 == s_boltbonus[i] || state16 == s_boltbonus2[i] || state16 == s_boltbonus3[i]) ? actorIdBonusBolt :
        (state16 == s_nukebonus[i] || state16 == s_nukebonus2[i] || state16 == s_nukebonus3[i]) ? actorIdBonusNuke :
        (state16 == s_potionbonus[i]) ? actorIdBonusPotion :
        (state16 == s_rkeybonus[i]) ? actorIdBonusKeyRed :
        (state16 == s_rkey2bonus[i]) ? actorIdBonusKeyRed2 :
        (state16 == s_ykeybonus[i]) ? actorIdBonusKeyYellow :
        (state16 == s_gkeybonus[i]) ? actorIdBonusKeyGreen :
        (state16 == s_bkeybonus[i]) ? actorIdBonusKeyBlue :
        (state16 == s_scrollbonus[i] && temp1 == 7) ? actorIdBonusScroll1 :
        (state16 == s_scrollbonus[i] && temp1 == 8) ? actorIdBonusScroll2 :
        (state16 == s_scrollbonus[i] && temp1 == 9) ? actorIdBonusScroll3 :
        (state16 == s_scrollbonus[i] && temp1 == 10) ? actorIdBonusScroll4 :
        (state16 == s_scrollbonus[i] && temp1 == 11) ? actorIdBonusScroll5 :
        (state16 == s_scrollbonus[i] && temp1 == 12) ? actorIdBonusScroll6 :
        (state16 == s_scrollbonus[i] && temp1 == 13) ? actorIdBonusScroll7 :
        (state16 == s_scrollbonus[i] && temp1 == 14) ? actorIdBonusScroll8 :
        (state16 == s_chestbonus[i]) ? actorIdBonusChest :
        (state16 == s_waterchestbonus1[i] || state16 == s_waterchestbonus2[i]) ? actorIdBonusWaterChest :
        (state16 == s_bonus_die[i]) ? actorIdBonusExplosion :
        (state16 == s_ftimebonus[i] || state16 == s_ftimebonus2[i]) ? actorIdBonusFreezeTime :
        (state16 == s_rgem1bonus[i] || state16 == s_rgem2bonus[i]) ? actorIdBonusGemRed :
        (state16 == s_ygem1bonus[i] || state16 == s_ygem2bonus[i]) ? actorIdBonusGemYellow :
        (state16 == s_ggem1bonus[i] || state16 == s_ggem2bonus[i]) ? actorIdBonusGemGreen :
        (state16 == s_bgem1bonus[i] || state16 == s_bgem2bonus[i]) ? actorIdBonusGemBlue :
        (state16 == s_pgem1bonus[i] || state16 == s_pgem2bonus[i]) ? actorIdBonusGemPurple :
        0;
    return actorId;
}

const uint16_t SavedGameConverterAbyss::GetActorId(const SavedGameInDosFormat::ObjectInDosFormat& dosObject) const
{
    uint16_t actorId = 0;
    switch (dosObject.obclass)
    {
    case obclassPlayer:
        actorId = actorIdPlayer;
        break;
    case obclassBonus:
        actorId = GetActorIdOfBonus(dosObject.state16, dosObject.temp1);
        break;
    case obclassOrc:
        actorId = actorIdMonsterOrc;
        break;
    case obclassBat:
        actorId = actorIdMonsterBat;
        break;
    case obclassSkeleton:
        actorId = actorIdMonsterSkeleton;
        break;
    case obclassTroll:
        actorId = actorIdMonsterSkeleton;
        break;
    case obclassDemon:
        actorId = actorIdMonsterDemon;
        break;
    case obclassMage:
        actorId = actorIdMonsterMage;
        break;
    case obclassPlayerShot:
        actorId = actorIdProjectilePlayerShot;
        break;
    case obclassBigPlayerShot:
        actorId = actorIdProjectilePlayerBigShot;
        break;
    case obclassMageShot:
        actorId = actorIdProjectileMageShot;
        break;
    case obclassInert:
        // TODO
        break;
    case obclassBounce:
        // Not used in Catacomb Abyss
        break;
    case obclassGrelminar:
        actorId = actorIdMonsterNemesis;
        break;
    case obclassGate:
        // TODO
        break;
    case obclassZombie:
        actorId = actorIdMonsterZombie;
        break;
    case obclassSpook:
        actorId = actorIdMonsterSpook;
        break;
    case obclassWet:
        actorId = actorIdMonsterWetMan;
        break;
    case obclassExp:
        // TODO
        break;
    case obclassEye:
        actorId = actorIdMonsterEye;
        break;
    case obclassWallSkeleton:
        actorId = actorIdMonsterWallSkeleton;
        break;
    case obclassEyeShot:
        actorId = actorIdProjectileEyeShot;
        break;
    case obclassGrelminarShot:
        actorId = actorIdProjectileNemesisShot;
        break;
    case obclassRedDemon:
        actorId = actorIdMonsterRedDemon;
        break;
    case obclassFreeze:
        actorId = actorIdBonusFreezeTime;
        break;
    case obclassSolid:
        // TODO
        break;
    }
    return actorId;
}

const DecorateStateId SavedGameConverterAbyss::GetDecorateStateId(const SavedGameInDosFormat::ObjectInDosFormat& /*dosObject*/) const
{
    // TODO
    return StateIdWalk;
}
const uint16_t SavedGameConverterAbyss::GetAnimationFrame(const SavedGameInDosFormat::ObjectInDosFormat& /*dosObject*/) const
{
    // TODO
    return 0;
}

const bool SavedGameConverterAbyss::IsInertObject(const uint16_t obclass) const
{
    // TODO
    return false;
}
