// Created by Crowbar 0.72

$modelname "player/playermodels/resis_sniper_female.mdl"

$model "resis_sniper_female" "resis_sniper_female_reference.smd" {

	eyeball "eye_right" "ValveBiped.Bip01_Head1" -1.230000 -3.560002 65.120032 "eyeball_r" 1 4 "iris_unused" 0.58
	eyeball "eye_left" "ValveBiped.Bip01_Head1" 1.150000 -3.560001 65.120032 "eyeball_l" 1 -4 "iris_unused" 0.58

	eyelid upper_right "resis_sniper_female_01.vta" lowerer 1 -0.22 neutral 0 0.2 raiser 2 0.27 split 1 eyeball "eye_right"
	eyelid lower_right "resis_sniper_female_01.vta" lowerer 3 -0.22 neutral 0 -0.17 raiser 4 -0.01 split 1 eyeball "eye_right"
	eyelid upper_left "resis_sniper_female_01.vta" lowerer 5 -0.22 neutral 0 0.2 raiser 6 0.27 split -1 eyeball "eye_left"
	eyelid lower_left "resis_sniper_female_01.vta" lowerer 7 -0.22 neutral 0 -0.17 raiser 8 -0.01 split -1 eyeball "eye_left"

	mouth 0 "mouth" "ValveBiped.Bip01_Head1" 0 1 0

	flexfile "resis_sniper_female_01.vta" 
	{
		defaultflex frame 0
		// Already in eyelid lines: flex "upper_right" frame 1
		// Already in eyelid lines: flex "upper_right" frame 2
		// Already in eyelid lines: flex "lower_right" frame 3
		// Already in eyelid lines: flex "lower_right" frame 4
		// Already in eyelid lines: flex "upper_left" frame 5
		// Already in eyelid lines: flex "upper_left" frame 6
		// Already in eyelid lines: flex "lower_left" frame 7
		// Already in eyelid lines: flex "lower_left" frame 8
		flex "AU42" frame 9
		flexpair "AU1" 1 frame 10
		flexpair "AU2" 1 frame 11
		flexpair "AU4" 1 frame 12
		flexpair "AU1AU2" 1 frame 13
	}

	flexcontroller eyelid range 0 1 "right_lid_raiser"
	flexcontroller eyelid range 0 1 "left_lid_raiser"
	flexcontroller eyelid range 0 1 "right_lid_tightener"
	flexcontroller eyelid range 0 1 "left_lid_tightener"
	flexcontroller eyelid range 0 1 "right_lid_droop"
	flexcontroller eyelid range 0 1 "left_lid_droop"
	flexcontroller eyelid range 0 1 "right_lid_closer"
	flexcontroller eyelid range 0 1 "left_lid_closer"
	flexcontroller eyelid range 0 1 "half_closed"
	flexcontroller eyelid range 0 1 "blink"
	flexcontroller brow range 0 1 "right_inner_raiser"
	flexcontroller brow range 0 1 "left_inner_raiser"
	flexcontroller brow range 0 1 "right_outer_raiser"
	flexcontroller brow range 0 1 "left_outer_raiser"
	flexcontroller brow range 0 1 "right_lowerer"
	flexcontroller brow range 0 1 "left_lowerer"
	flexcontroller nose range 0 1 "right_cheek_raiser"
	flexcontroller nose range 0 1 "left_cheek_raiser"
	flexcontroller nose range 0 1 "wrinkler"
	flexcontroller nose range 0 1 "dilator"
	flexcontroller mouth range 0 1 "right_upper_raiser"
	flexcontroller mouth range 0 1 "left_upper_raiser"
	flexcontroller mouth range 0 1 "right_corner_puller"
	flexcontroller mouth range 0 1 "left_corner_puller"
	flexcontroller mouth range 0 1 "right_corner_depressor"
	flexcontroller mouth range 0 1 "left_corner_depressor"
	flexcontroller mouth range 0 1 "chin_raiser"
	flexcontroller phoneme range 0 1 "right_part"
	flexcontroller phoneme range 0 1 "left_part"
	flexcontroller phoneme range 0 1 "right_puckerer"
	flexcontroller phoneme range 0 1 "left_puckerer"
	flexcontroller phoneme range 0 1 "right_funneler"
	flexcontroller phoneme range 0 1 "left_funneler"
	flexcontroller phoneme range 0 1 "right_stretcher"
	flexcontroller phoneme range 0 1 "left_stretcher"
	flexcontroller phoneme range 0 1 "bite"
	flexcontroller phoneme range 0 1 "presser"
	flexcontroller phoneme range 0 1 "tightener"
	flexcontroller phoneme range 0 1 "jaw_clencher"
	flexcontroller phoneme range 0 1 "jaw_drop"
	flexcontroller phoneme range 0 1 "right_mouth_drop"
	flexcontroller phoneme range 0 1 "left_mouth_drop"
	flexcontroller mouth range 0 1 "smile"
	flexcontroller mouth range 0 1 "lower_lip"
	flexcontroller head range -30 30 "head_rightleft"
	flexcontroller head range -15 15 "head_updown"
	flexcontroller head range -15 15 "head_tilt"
	flexcontroller eyes range -30 30 "eyes_updown"
	flexcontroller eyes range -30 30 "eyes_rightleft"
	flexcontroller body range -30 30 "body_rightleft"
	flexcontroller chest range -30 30 "chest_rightleft"
	flexcontroller head range -0.2 0.2 "head_forwardback"
	flexcontroller gesture range -1 1 "gesture_updown"
	flexcontroller gesture range -1 1 "gesture_rightleft"

	localvar AU12R
	localvar AU12L
	localvar AU12AU25
	localvar AU15R
	localvar AU15L
	localvar AU17R
	localvar AU17L
	localvar AU10R
	localvar AU10L
	localvar AU16
	localvar AU6R
	localvar AU6L
	localvar AU9R
	localvar AU9L
	localvar AU25R
	localvar AU25L
	localvar AU18R
	localvar AU18L
	localvar AU22R
	localvar AU22L
	localvar AU20R
	localvar AU20L
	localvar AU32
	localvar AU24
	localvar AU38
	localvar AU31
	localvar AU26R
	localvar AU26L
	localvar AU27R
	localvar AU27L
	localvar right_open
	localvar left_open
	localvar right_lip_suppressor
	localvar left_lip_suppressor
	localvar right_depressor_suppressor
	localvar left_depressor_suppressor
	localvar right_corner_suppressor
	localvar left_corner_suppressor
	localvar right_drop_suppressor
	localvar left_drop_suppressor
	localvar right_drop
	localvar left_drop
	%upper_right_raiser = right_lid_raiser * (1 - right_lid_droop * 0.8) * (1 - right_lid_closer) * (1 - blink)
	%upper_right_neutral = (1 - right_lid_droop * 0.8) * (1 - right_lid_raiser) * (1 - right_lid_closer) * (1 - blink)
	%upper_right_lowerer = right_lid_closer + blink * (1 - right_lid_closer)
	%upper_left_raiser = left_lid_raiser * (1 - left_lid_droop * 0.8) * (1 - left_lid_closer) * (1 - blink)
	%upper_left_neutral = (1 - left_lid_droop * 0.8) * (1 - left_lid_raiser) * (1 - left_lid_closer) * (1 - blink)
	%upper_left_lowerer = left_lid_closer + blink * (1 - left_lid_closer)
	%lower_right_raiser = right_lid_closer + blink * 0.5 * (1 - right_lid_closer)
	%lower_right_neutral = (1 - right_lid_closer) * (1 - 0.5 * blink) * (1 - right_lid_tightener * 0.5) * (1 - right_cheek_raiser * 0.25)
	%lower_right_lowerer = 0
	%lower_left_raiser = left_lid_closer + blink * 0.5 * (1 - left_lid_closer)
	%lower_left_neutral = (1 - left_lid_closer) * (1 - 0.5 * blink) * (1 - left_lid_tightener * 0.5) * (1 - left_cheek_raiser * 0.25)
	%lower_left_lowerer = 0
	%AU1R = right_inner_raiser * (1 - right_outer_raiser) * (1 - right_lowerer)
	%AU1L = left_inner_raiser * (1 - left_outer_raiser) * (1 - left_lowerer)
	%AU2R = right_outer_raiser * (1 - right_inner_raiser) * (1 - right_lowerer)
	%AU2L = left_outer_raiser * (1 - left_inner_raiser) * (1 - left_lowerer)
	%AU1AU2R = right_inner_raiser * right_outer_raiser * (1 - right_lowerer)
	%AU1AU2L = left_inner_raiser * left_outer_raiser * (1 - left_lowerer)
	%AU4R = right_lowerer
	%AU4L = left_lowerer
	%AU6R = right_cheek_raiser
	%AU6L = left_cheek_raiser
	%AU9R = wrinkler
	%AU9L = wrinkler
	%AU38 = dilator
	%right_open = smile + right_part * (1 - smile) + right_puckerer + right_funneler + right_upper_raiser * 0.5 + lower_lip * 0.5
	%left_open = smile + left_part * (1 - smile) + left_puckerer + left_funneler + left_upper_raiser * 0.5 + lower_lip * 0.5
	%right_lip_suppressor = 1 / %right_open * (1 - presser) * (1 - bite)
	%left_lip_suppressor = 1 / %left_open * (1 - presser) * (1 - bite)
	%AU10R = right_upper_raiser * right_upper_raiser * 0.5 * %right_lip_suppressor
	%AU10L = left_upper_raiser * left_upper_raiser * 0.5 * %left_lip_suppressor
	%AU25R = right_part * right_part * %right_lip_suppressor * (1 - smile)
	%AU25L = left_part * left_part * %left_lip_suppressor * (1 - smile)
	%AU12AU25 = smile * smile * 0.5 * (%right_lip_suppressor + %left_lip_suppressor)
	%AU18R = right_puckerer * right_puckerer * %right_lip_suppressor
	%AU18L = left_puckerer * left_puckerer * %left_lip_suppressor
	%AU22R = right_funneler * right_funneler * %right_lip_suppressor
	%AU22L = left_funneler * left_funneler * %left_lip_suppressor
	%AU17R = chin_raiser * (1 - jaw_drop * 0.5) * (1 - bite) * (1 - 0.6 * (%AU25R + %AU18R + %AU22R)) * (1 - smile)
	%AU17L = chin_raiser * (1 - jaw_drop * 0.5) * (1 - bite) * (1 - 0.6 * (%AU25L + %AU18L + %AU22L)) * (1 - smile)
	%right_depressor_suppressor = 1 / (%AU18R * 0.8 + %AU22R * 0.8 + %AU17R + bite + right_corner_puller + right_stretcher + right_corner_depressor) * (1 - jaw_drop * 0.5) * (1 - smile * 0.5)
	%left_depressor_suppressor = 1 / (%AU18L * 0.8 + %AU22L * 0.8 + %AU17L + bite + left_corner_puller + left_stretcher + left_corner_depressor) * (1 - jaw_drop * 0.5) * (1 - smile * 0.5)
	%AU15R = right_corner_depressor * right_corner_depressor * %right_depressor_suppressor
	%AU15L = left_corner_depressor * left_corner_depressor * %left_depressor_suppressor
	%right_corner_suppressor = 1 / (%AU18R * 0.8 + %AU22R * 0.8 + %AU17R + bite + right_corner_puller + right_stretcher + right_corner_depressor) * (1 - jaw_drop * 0.5) * (1 - smile)
	%left_corner_suppressor = 1 / (%AU18L * 0.8 + %AU22L * 0.8 + %AU17L + bite + left_corner_puller + left_stretcher + left_corner_depressor) * (1 - jaw_drop * 0.5) * (1 - smile)
	%AU12R = right_corner_puller * right_corner_puller * %right_corner_suppressor
	%AU12L = left_corner_puller * left_corner_puller * %left_corner_suppressor
	%AU20R = right_stretcher * right_stretcher * %right_corner_suppressor
	%AU20L = left_stretcher * left_stretcher * %left_corner_suppressor
	%AU32 = bite
	%AU24 = presser + (1 - presser) * tightener
	%AU31 = jaw_clencher * jaw_clencher / (jaw_clencher + jaw_drop)
	%right_drop_suppressor = 1 / (0.5 * %right_open + right_mouth_drop) * (1 - presser) * (1 - bite)
	%left_drop_suppressor = 1 / (0.5 * %left_open + left_mouth_drop) * (1 - presser) * (1 - bite)
	%right_drop = right_mouth_drop * right_mouth_drop * %right_drop_suppressor
	%left_drop = left_mouth_drop * left_mouth_drop * %left_drop_suppressor
	%AU26R = jaw_drop * (1 - %right_drop) * jaw_drop / (jaw_clencher + jaw_drop)
	%AU26L = jaw_drop * (1 - %left_drop) * jaw_drop / (jaw_clencher + jaw_drop)
	%AU27R = jaw_drop * %right_drop * jaw_drop / (jaw_clencher + jaw_drop)
	%AU27L = jaw_drop * %left_drop * jaw_drop / (jaw_clencher + jaw_drop)
	%AU16 = lower_lip * lower_lip * 0.25 * (%right_lip_suppressor + %left_lip_suppressor) * (1 - 0.5 * (%AU27R + %AU27L))
	%mouth = %AU27R * 0.5 + %AU27L * 0.5 + %AU22R * 0.35 + %AU22L * 0.35 + %AU18R * 0.25 + %AU18L * 0.25 + %AU25R * 0.35 + %AU25L * 0.35 + %AU32 * 0.5 + %AU12AU25 + %AU16 * 0.4
}

$lod 5
{
	replacemodel "resis_sniper_female_reference.smd" "resis_sniper_female_reference_lod1.smd"
	replacebone "ValveBiped.forward" "ValveBiped.Bip01_Head1"
	replacebone "ValveBiped.Anim_Attachment_LH" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Anim_Attachment_RH" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger41" "ValveBiped.Bip01_R_Finger4"
	replacebone "ValveBiped.Bip01_R_Finger42" "ValveBiped.Bip01_R_Finger41"
	replacebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31"
	replacebone "ValveBiped.wansyo" "ValveBiped.Bip01_L_UpperArm"
	facial
}
$lod 10
{
	replacemodel "resis_sniper_female_reference.smd" "resis_sniper_female_reference_lod2.smd"
	replacebone "ValveBiped.forward" "ValveBiped.Bip01_Head1"
	replacebone "ValveBiped.Anim_Attachment_LH" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Anim_Attachment_RH" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_L_Finger31" "ValveBiped.Bip01_L_Finger3"
	replacebone "ValveBiped.Bip01_R_Finger41" "ValveBiped.Bip01_R_Finger4"
	replacebone "ValveBiped.Bip01_R_Finger42" "ValveBiped.Bip01_R_Finger41"
	replacebone "ValveBiped.Bip01_R_Finger31" "ValveBiped.Bip01_R_Finger3"
	replacebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31"
	replacebone "ValveBiped.wansyo" "ValveBiped.Bip01_L_UpperArm"
	facial
}
$lod 25
{
	replacemodel "resis_sniper_female_reference.smd" "resis_sniper_female_reference_lod3.smd"
	replacebone "ValveBiped.forward" "ValveBiped.Bip01_Head1"
	replacebone "ValveBiped.Anim_Attachment_LH" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Anim_Attachment_RH" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger41" "ValveBiped.Bip01_R_Finger4"
	replacebone "ValveBiped.Bip01_R_Finger42" "ValveBiped.Bip01_R_Finger41"
	replacebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31"
	replacebone "ValveBiped.wansyo" "ValveBiped.Bip01_L_UpperArm"
	facial
}
$lod 40
{
	replacemodel "resis_sniper_female_reference.smd" "resis_sniper_female_reference_lod4.smd"
	replacebone "ValveBiped.forward" "ValveBiped.Bip01_Head1"
	replacebone "ValveBiped.Anim_Attachment_LH" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Anim_Attachment_RH" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_R_Finger41" "ValveBiped.Bip01_R_Finger4"
	replacebone "ValveBiped.Bip01_R_Finger42" "ValveBiped.Bip01_R_Finger41"
	replacebone "ValveBiped.Bip01_R_Finger31" "ValveBiped.Bip01_R_Finger3"
	replacebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31"
	replacebone "ValveBiped.wansyo" "ValveBiped.Bip01_L_UpperArm"
	facial
}
$lod 60
{
	replacemodel "resis_sniper_female_reference.smd" "resis_sniper_female_reference_lod5.smd"
	replacebone "ValveBiped.forward" "ValveBiped.Bip01_Head1"
	replacebone "ValveBiped.Anim_Attachment_LH" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Anim_Attachment_RH" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_L_Finger02" "ValveBiped.Bip01_L_Finger01"
	replacebone "ValveBiped.Bip01_R_Finger41" "ValveBiped.Bip01_R_Finger4"
	replacebone "ValveBiped.Bip01_R_Finger42" "ValveBiped.Bip01_R_Finger41"
	replacebone "ValveBiped.Bip01_R_Finger31" "ValveBiped.Bip01_R_Finger3"
	replacebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31"
	replacebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01"
	replacebone "ValveBiped.wansyo" "ValveBiped.Bip01_L_UpperArm"
	facial
}
$shadowlod
{
	replacemodel "resis_sniper_female_reference.smd" "resis_sniper_female_reference_lod6.smd"
	replacebone "ValveBiped.forward" "ValveBiped.Bip01_Head1"
	replacebone "ValveBiped.Anim_Attachment_LH" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Anim_Attachment_RH" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Bip01_L_Finger02" "ValveBiped.Bip01_L_Finger01"
	replacebone "ValveBiped.Bip01_R_Finger41" "ValveBiped.Bip01_R_Finger4"
	replacebone "ValveBiped.Bip01_R_Finger42" "ValveBiped.Bip01_R_Finger41"
	replacebone "ValveBiped.Bip01_R_Finger31" "ValveBiped.Bip01_R_Finger3"
	replacebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31"
	replacebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01"
	replacebone "ValveBiped.wansyo" "ValveBiped.Bip01_L_UpperArm"
	nofacial
}

$surfaceprop "flesh"

$jointsurfaceprop "ValveBiped.Bip01_Head1" "computer"
$jointsurfaceprop "ValveBiped.forward" "computer"

$contents "solid"

$illumposition -1.006 2.672 46.265

$eyeposition 0 0 70

$mostlyopaque

$cdmaterials "models\player\resistance\sniper\"
$cdmaterials "models\humans\male\"

$texturegroup "skinfamilies"
{
	{ "body_sheet_r_sni_f"   "met_sheet_r_sni_f"   }
	{ "body_sheet_r_sni_f_r" "met_sheet_r_sni_f_r" }
	{ "body_sheet_r_sni_f_b" "met_sheet_r_sni_f_b" }
}

$attachment "eyes" "ValveBiped.Bip01_Head1" 2.75 -3.96 0.04 rotate 0 -80.1 -90
$attachment "mouth" "ValveBiped.Bip01_Head1" 0.8 -5.8 -0.15 rotate 0 -80 -90
$attachment "wansyo" "ValveBiped.wansyo" 0 0 0 rotate 0 0 0
$attachment "forward" "ValveBiped.forward" 0 0 0 rotate 0 0 0
$attachment "anim_attachment_RH" "ValveBiped.Anim_Attachment_RH" 0 0 0 rotate -90 -90 0
$attachment "anim_attachment_LH" "ValveBiped.Anim_Attachment_LH" 0 0 0 rotate -90 -90 0

$cbox 0 0 0 0 0 0

$bbox -30.119 -28.559 6.68 24.775 26.546 85.849

$definebone "ValveBiped.Bip01_Pelvis" "" -0.000005 -0.78846 37.913784 0 0 89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine" "ValveBiped.Bip01_Pelvis" 0.000005 4.212784 -1.689857 -1.602964 89.999982 89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine1" "ValveBiped.Bip01_Spine" 3.837406 0 0 0 -6.452307 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine2" "ValveBiped.Bip01_Spine1" 3.617867 0 0 0 -0.723932 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine4" "ValveBiped.Bip01_Spine2" 7.539783 0 0 0 8.927427 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Neck1" "ValveBiped.Bip01_Spine4" 3.178295 0 0 0 12.841531 179.999855 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Head1" "ValveBiped.Bip01_Neck1" 2.970276 0 0 0 6.295659 0 0 0 0 0 0 0
$definebone "ValveBiped.forward" "ValveBiped.Bip01_Head1" 0 -0.000001 0 0 -75.999986 -89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Clavicle" "ValveBiped.Bip01_Spine4" 2.023708 0.90746 0.852579 -76.440363 166.16435 93.870951 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_UpperArm" "ValveBiped.Bip01_L_Clavicle" 4.983669 -0.000004 0 13.809087 -43.179367 -87.04598 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_L_UpperArm" 11.123062 0 -0.000004 0.000009 -11.662212 -0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Hand" "ValveBiped.Bip01_L_Forearm" 11.208307 -0.000001 -0.000004 -2.211894 2.080008 86.253985 0 0 0 0 0 0
$definebone "ValveBiped.Anim_Attachment_LH" "ValveBiped.Bip01_L_Hand" 2.676094 -1.712448 0 -0.000002 90.000043 90.000037 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Clavicle" "ValveBiped.Bip01_Spine4" 2.023712 0.907463 -0.852528 76.440527 166.163777 -97.982479 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_UpperArm" "ValveBiped.Bip01_R_Clavicle" 4.983664 0 0 -10.977357 -43.834945 90.098419 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_R_UpperArm" 11.123055 0.000001 0.000008 -0.000063 -11.660149 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Hand" "ValveBiped.Bip01_R_Forearm" 11.208296 0.000001 0.000008 2.211382 2.08001 -85.770066 0 0 0 0 0 0
$definebone "ValveBiped.Anim_Attachment_RH" "ValveBiped.Bip01_R_Hand" 2.676086 -1.712444 0 0.000001 -89.999921 -89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Thigh" "ValveBiped.Bip01_Pelvis" 3.984014 -0.000004 -0.000003 2.965973 -92.308258 -89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Calf" "ValveBiped.Bip01_L_Thigh" 15.940012 0 0 0 2.959556 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Foot" "ValveBiped.Bip01_L_Calf" 17.709557 0.000001 0 -4.013913 -67.334976 0.980102 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Toe0" "ValveBiped.Bip01_L_Foot" 6.204 0 0 0.053513 -28.677396 -0.638847 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Thigh" "ValveBiped.Bip01_Pelvis" -3.984013 0.000008 0.000007 2.965973 -87.691993 -89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Calf" "ValveBiped.Bip01_R_Thigh" 15.940008 0 0 0 2.959557 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Foot" "ValveBiped.Bip01_R_Calf" 17.709566 0 0 4.012423 -67.504057 -0.366464 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Toe0" "ValveBiped.Bip01_R_Foot" 6.203999 0.000001 0 -0.354375 -28.587333 -4.506313 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger4" "ValveBiped.Bip01_L_Hand" 3.549248 0.198822 -1.48947 21.265154 0.597188 -12.141545 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger3" "ValveBiped.Bip01_L_Hand" 3.698013 0.267555 -0.728979 14.90962 -7.176074 -4.669948 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger31" "ValveBiped.Bip01_L_Finger3" 1.749004 0.000004 0.000001 1.552368 -10.895544 0.012207 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger2" "ValveBiped.Bip01_L_Hand" 3.768507 0.15255 0.071384 1.624106 -0.196873 5.042317 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger21" "ValveBiped.Bip01_L_Finger2" 2.038002 -0.000004 0 -0.250154 -32.972574 0.086747 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger22" "ValveBiped.Bip01_L_Finger21" 1.067001 -0.000002 0 -0.324982 -5.671938 -0.071332 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger1" "ValveBiped.Bip01_L_Hand" 3.783932 -0.017262 0.794555 -12.383851 3.590776 16.236422 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger11" "ValveBiped.Bip01_L_Finger1" 1.629004 -0.000004 0 -3.819795 -26.858732 1.078936 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger12" "ValveBiped.Bip01_L_Finger11" 0.955997 0.000004 0 -3.994662 -9.741897 0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger0" "ValveBiped.Bip01_L_Hand" 1.25897 -0.297985 1.252592 -30.375118 -37.494586 -73.854713 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger01" "ValveBiped.Bip01_L_Finger0" 1.393002 -0.000002 0.000004 0.115971 -2.497458 -0.000004 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger02" "ValveBiped.Bip01_L_Finger01" 1.1 -0.000002 0 0.75224 -16.433276 -0.000006 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger4" "ValveBiped.Bip01_R_Hand" 3.549273 0.211407 1.487671 -21.265162 0.597132 12.141549 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger41" "ValveBiped.Bip01_R_Finger4" 1.219002 -0.000008 0.000001 -5.585311 -21.005782 0.264312 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger42" "ValveBiped.Bip01_R_Finger41" 0.679998 0 0 -4.983475 -9.905296 -0.651044 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger3" "ValveBiped.Bip01_R_Hand" 3.698017 0.273712 0.726624 -14.909625 -7.176126 4.669951 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger31" "ValveBiped.Bip01_R_Finger3" 1.749001 -0.000008 0.000001 -1.552377 -10.895598 -0.012143 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31" 0.959999 -0.000002 0.000001 -2.257574 -13.628779 -0.13315 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger2" "ValveBiped.Bip01_R_Hand" 3.768501 0.151951 -0.072739 -1.624107 -0.196931 -5.042314 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger21" "ValveBiped.Bip01_R_Finger2" 2.038 0 0 0.250153 -32.972636 -0.086746 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger22" "ValveBiped.Bip01_R_Finger21" 1.066998 0.000002 0 0.324981 -5.671992 0.071333 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger1" "ValveBiped.Bip01_R_Hand" 3.783909 -0.023956 -0.794451 12.383854 3.590719 -16.23642 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger11" "ValveBiped.Bip01_R_Finger1" 1.629001 -0.000004 -0.000001 3.819798 -26.858775 -1.078944 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger12" "ValveBiped.Bip01_R_Finger11" 0.956001 0 0 3.994664 -9.741887 -0.000007 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger0" "ValveBiped.Bip01_R_Hand" 1.258938 -0.308548 -1.250053 30.375132 -37.49459 73.854719 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger01" "ValveBiped.Bip01_R_Finger0" 1.392998 0 0 -0.115968 -2.497469 0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01" 1.099998 0.000002 0 -0.752238 -16.433292 0.00006 0 0 0 0 0 0
$definebone "ValveBiped.wansyo" "ValveBiped.Bip01_L_UpperArm" 6.000004 0.750001 1.07 13.000019 -7.999981 -7.500017 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Ankle" "ValveBiped.Bip01_L_Calf" 16.646982 0 0 0 0 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Shin" "ValveBiped.Bip01_L_Calf" 8.32349 0 0 0 0 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Knee" "ValveBiped.Bip01_L_Thigh" 15.940012 0 0 0 1.445572 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Quadricep" "ValveBiped.Bip01_L_Thigh" 6.376007 0 0 0 0 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Ankle" "ValveBiped.Bip01_R_Calf" 16.64699 0 0 0 0 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Shin" "ValveBiped.Bip01_R_Calf" 8.323495 0 0 0 0 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Quadricep" "ValveBiped.Bip01_R_Thigh" 6.376003 0 0 0 0 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Knee" "ValveBiped.Bip01_R_Thigh" 15.940008 0 0 0 1.445573 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Ulna" "ValveBiped.Bip01_L_Forearm" 5.604149 0 0 0.000009 -0.000001 -1.769066 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Shoulder" "ValveBiped.Bip01_L_UpperArm" 1.5 0 0 0.000007 -0.000001 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Elbow" "ValveBiped.Bip01_L_UpperArm" 11.123062 0 -0.000004 0.000005 -5.734219 -0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Bicep" "ValveBiped.Bip01_L_UpperArm" 5.560001 -0.700001 -0.500004 0.000007 -0.000001 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Wrist" "ValveBiped.Bip01_R_Forearm" 11.208296 0 0.000008 -0.000063 0 3.734024 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Ulna" "ValveBiped.Bip01_R_Forearm" 5.604145 0 0.000008 -0.000063 0 2.001342 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Bicep" "ValveBiped.Bip01_R_UpperArm" 5.560005 -0.7 0.500004 -0.000005 0.000001 -0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Shoulder" "ValveBiped.Bip01_R_UpperArm" 1.500004 0 0 -0.000005 0.000001 -0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Elbow" "ValveBiped.Bip01_R_UpperArm" 11.123055 0.000001 0.000008 -0.000005 -5.733187 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Latt" "ValveBiped.Bip01_Spine2" 2.132019 1.532022 5 0 0 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Pectoral" "ValveBiped.Bip01_Spine2" 2 4.938042 -2.578 0 0 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Latt" "ValveBiped.Bip01_Spine2" 2.132023 1.532021 -5 0 0 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Pectoral" "ValveBiped.Bip01_Spine2" 2 4.938046 2.578 0 0 0 0 0 0 0 0 0

$bonemerge "ValveBiped.Bip01_Pelvis"
$bonemerge "ValveBiped.Bip01_Spine"
$bonemerge "ValveBiped.Bip01_Spine1"
$bonemerge "ValveBiped.Bip01_Spine2"
$bonemerge "ValveBiped.Bip01_Spine4"
$bonemerge "ValveBiped.Bip01_L_Clavicle"
$bonemerge "ValveBiped.Bip01_L_UpperArm"
$bonemerge "ValveBiped.Bip01_R_Clavicle"
$bonemerge "ValveBiped.Bip01_R_UpperArm"
$bonemerge "ValveBiped.Bip01_R_Forearm"
$bonemerge "ValveBiped.Bip01_R_Hand"
$bonemerge "ValveBiped.Anim_Attachment_RH"
$bonemerge "ValveBiped.Bip01_R_Finger4"
$bonemerge "ValveBiped.Bip01_R_Finger41"
$bonemerge "ValveBiped.Bip01_R_Finger42"
$bonemerge "ValveBiped.Bip01_R_Finger3"
$bonemerge "ValveBiped.Bip01_R_Finger31"
$bonemerge "ValveBiped.Bip01_R_Finger32"
$bonemerge "ValveBiped.Bip01_R_Finger2"
$bonemerge "ValveBiped.Bip01_R_Finger21"
$bonemerge "ValveBiped.Bip01_R_Finger22"
$bonemerge "ValveBiped.Bip01_R_Finger1"
$bonemerge "ValveBiped.Bip01_R_Finger11"
$bonemerge "ValveBiped.Bip01_R_Finger12"
$bonemerge "ValveBiped.Bip01_R_Finger0"
$bonemerge "ValveBiped.Bip01_R_Finger01"
$bonemerge "ValveBiped.Bip01_R_Finger02"
$bonemerge "ValveBiped.wansyo"

$proceduralbones "resis_sniper_female.vrd"


$ikchain "rhand" "ValveBiped.Bip01_R_Hand" knee 0.707 0.707 0
$ikchain "lhand" "ValveBiped.Bip01_L_Hand" knee 0.707 0.707 0
$ikchain "rfoot" "ValveBiped.Bip01_R_Foot" knee 0.707 -0.707 0
$ikchain "lfoot" "ValveBiped.Bip01_L_Foot" knee 0.707 -0.707 0

$ikautoplaylock "rfoot" 1 0.1
$ikautoplaylock "lfoot" 1 0.1

$sequence "ragdoll" {
	"resis_sniper_female_anims\ragdoll.smd"
	activity "ACT_DIERAGDOLL" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 3
}

$includemodel "humans/female_shared.mdl" 
$includemodel "humans/female_ss.mdl"
$includemodel "humans/female_gestures.mdl"
$includemodel "humans/female_postures.mdl"
$includemodel "alyx_animations.mdl"
$includemodel "player/female_shared.mdl"
$includemodel "player/female_ss.mdl"
$includemodel "player/female_gestures.mdl"
$includemodel "player/female_postures.mdl"

$collisionjoints "resis_sniper_female_physics.smd"
{
	$mass 0
	$inertia 10
	$damping 0.01
	$rotdamping 1.5
	$rootbone " "


	$jointconstrain "ValveBiped.Bip01_R_Thigh" x limit -12 12 0
	$jointconstrain "ValveBiped.Bip01_R_Thigh" y limit -8 75 0
	$jointconstrain "ValveBiped.Bip01_R_Thigh" z limit -97 32 0

	$jointconstrain "ValveBiped.Bip01_R_Calf" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Calf" y limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Calf" z limit -12 126 0

	$jointconstrain "ValveBiped.Bip01_L_Thigh" x limit -12 12 0
	$jointconstrain "ValveBiped.Bip01_L_Thigh" y limit -73 6 0
	$jointconstrain "ValveBiped.Bip01_L_Thigh" z limit -93 30 0

	$jointconstrain "ValveBiped.Bip01_L_Calf" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Calf" y limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Calf" z limit -8 126 0

	$jointconstrain "ValveBiped.Bip01_L_Foot" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Foot" y limit -19 19 0
	$jointconstrain "ValveBiped.Bip01_L_Foot" z limit -15 35 0

	$jointconstrain "ValveBiped.Bip01_Spine2" x limit -48 48 0
	$jointconstrain "ValveBiped.Bip01_Spine2" y limit -25 25 0
	$jointconstrain "ValveBiped.Bip01_Spine2" z limit -25 50 0

	$jointconstrain "ValveBiped.Bip01_R_UpperArm" x limit -39 39 0
	$jointconstrain "ValveBiped.Bip01_R_UpperArm" y limit -79 95 0
	$jointconstrain "ValveBiped.Bip01_R_UpperArm" z limit -93 23 0

	$jointconstrain "ValveBiped.Bip01_R_Forearm" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Forearm" y limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Forearm" z limit -149 4 0

	$jointconstrain "ValveBiped.Bip01_L_UpperArm" x limit -30 30 0
	$jointconstrain "ValveBiped.Bip01_L_UpperArm" y limit -95 84 0
	$jointconstrain "ValveBiped.Bip01_L_UpperArm" z limit -86 26 0

	$jointconstrain "ValveBiped.Bip01_L_Forearm" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Forearm" y limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Forearm" z limit -149 4 0

	$jointconstrain "ValveBiped.Bip01_L_Hand" x limit -37 37 0
	$jointconstrain "ValveBiped.Bip01_L_Hand" y limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Hand" z limit -57 59 0

	$jointconstrain "ValveBiped.Bip01_Head1" x limit -20 20 0
	$jointconstrain "ValveBiped.Bip01_Head1" y limit -25 25 0
	$jointconstrain "ValveBiped.Bip01_Head1" z limit -13 30 0

	$jointconstrain "ValveBiped.Bip01_R_Hand" x limit -60 60 0
	$jointconstrain "ValveBiped.Bip01_R_Hand" y limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Hand" z limit -57 70 0

	$jointconstrain "ValveBiped.Bip01_R_Foot" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Foot" y limit -25 6 0
	$jointconstrain "ValveBiped.Bip01_R_Foot" z limit -15 35 0
}

$collisiontext
{
	animatedfriction 
	{
		"animfrictionmin" "1.000000"
		"animfrictionmax" "400.000000"
		"animfrictiontimein" "0.500000"
		"animfrictiontimeout" "0.300000"
		"animfrictiontimehold" "0.000000"
	}
	editparams 
	{
		"rootname" "valvebiped.bip01_pelvis"
		"totalmass" "90.000000"
		"jointmerge" "valvebiped.bip01_pelvis,valvebiped.bip01"
		"jointmerge" "valvebiped.bip01_pelvis,valvebiped.bip01_spine1"
	}
}
