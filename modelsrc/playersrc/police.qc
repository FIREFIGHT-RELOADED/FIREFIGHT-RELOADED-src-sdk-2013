// Created by Crowbar 0.57

$modelname "player/playermodels/police.mdl"

$model "police" "police_reference.smd" {

	flexfile "police_01.vta" 
	{
		defaultflex frame 0
	}

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
}
$bodygroup "manhack"
{
	blank
	studio "police_manhack_reference.smd"
}

$lod 12
{
	replacemodel "police_reference.smd" "police_reference_lod1.smd"
	replacemodel "police_manhack_reference.smd" "police_manhack_reference_lod1.smd"
	replacebone "ValveBiped.forward" "ValveBiped.Bip01_Head1"
	replacebone "ValveBiped.Anim_Attachment_RH" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Anim_Attachment_LH" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.baton_parent" "ValveBiped.Bip01_R_Hand"
	nofacial
}
$lod 18
{
	replacemodel "police_reference.smd" "police_reference_lod2.smd"
	replacemodel "police_manhack_reference.smd" "police_manhack_reference_lod2.smd"
	replacebone "ValveBiped.forward" "ValveBiped.Bip01_Head1"
	replacebone "ValveBiped.Anim_Attachment_RH" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Anim_Attachment_LH" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.baton_parent" "ValveBiped.Bip01_R_Hand"
	nofacial
}
$lod 42
{
	replacemodel "police_reference.smd" "police_reference_lod3.smd"
	replacemodel "police_manhack_reference.smd" "police_manhack_reference_lod3.smd"
	replacebone "ValveBiped.forward" "ValveBiped.Bip01_Head1"
	replacebone "ValveBiped.Anim_Attachment_RH" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Anim_Attachment_LH" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.baton_parent" "ValveBiped.Bip01_R_Hand"
	nofacial
}
$lod 55
{
	replacemodel "police_reference.smd" "police_reference_lod4.smd"
	replacemodel "police_manhack_reference.smd" "police_manhack_reference_lod4.smd"
	replacebone "ValveBiped.forward" "ValveBiped.Bip01_Head1"
	replacebone "ValveBiped.Anim_Attachment_RH" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Anim_Attachment_LH" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.baton_parent" "ValveBiped.Bip01_R_Hand"
	nofacial
}
$lod 80
{
	replacemodel "police_reference.smd" "police_reference_lod5.smd"
	replacemodel "police_manhack_reference.smd" "police_manhack_reference_lod5.smd"
	replacebone "ValveBiped.forward" "ValveBiped.Bip01_Head1"
	replacebone "ValveBiped.Anim_Attachment_RH" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Anim_Attachment_LH" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_R_Finger01" "ValveBiped.Bip01_R_Finger0"
	replacebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01"
	replacebone "ValveBiped.baton_parent" "ValveBiped.Bip01_R_Hand"
	nofacial
}
$shadowlod
{
	replacemodel "police_reference.smd" "police_reference_lod6.smd"
	replacemodel "police_manhack_reference.smd" "police_manhack_reference_lod6.smd"
	replacebone "ValveBiped.forward" "ValveBiped.Bip01_Head1"
	replacebone "ValveBiped.Anim_Attachment_RH" "ValveBiped.Bip01_R_Hand"
	replacebone "ValveBiped.Anim_Attachment_LH" "ValveBiped.Bip01_L_Hand"
	replacebone "ValveBiped.Bip01_R_Finger01" "ValveBiped.Bip01_R_Finger0"
	replacebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01"
	replacebone "ValveBiped.baton_parent" "ValveBiped.Bip01_R_Hand"
	nofacial
}

$surfaceprop "flesh"

$jointsurfaceprop "ValveBiped.Bip01_Head1" "no_decal"
$jointsurfaceprop "ValveBiped.forward" "no_decal"

$contents "solid"

$illumposition -0.006 1.083 34.855

$eyeposition 0 0 72


$cdmaterials "models\Police\"
$cdmaterials "models\manhack\"

$attachment "eyes" "ValveBiped.Bip01_Head1" 3.34 -5.28 0 rotate 0 -80.1 -90
$attachment "Center" "ValveBiped.Bip01_Spine1" -6.2 2.09 0 rotate 0 96.62 90
$attachment "Chest" "ValveBiped.Bip01_Spine4" -2.7 3.88 0 rotate 0 79.75 90
$attachment "LHand" "ValveBiped.Bip01_L_Hand" 3 0 0 rotate 0 0 0
$attachment "RHand" "ValveBiped.Bip01_R_Hand" 3 0 0 rotate 0 0 0
$attachment "zipline" "ValveBiped.Bip01_Spine4" 2 -2 0 rotate 0 -17 0
$attachment "manhack" "ValveBiped.Bip01_Spine" 18 -6 3 rotate -25 50 0
$attachment "forward" "ValveBiped.forward" 0 0 0 rotate 0 0 0
$attachment "anim_attachment_RH" "ValveBiped.Anim_Attachment_RH" 0 0 0 rotate -90 -90 0
$attachment "anim_attachment_LH" "ValveBiped.Anim_Attachment_LH" 0 0 0 rotate -90 -90 0
$attachment "anim_attachment_head" "ValveBiped.Bip01_Head1" 0 0 0 rotate -90 -90 0

$cbox 0 0 0 0 0 0

$bbox -13 -13 0 13 13 72

$definebone "ValveBiped.Bip01_Pelvis" "" -0.000005 -0.533615 38.566917 0 0 89.999983 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine" "ValveBiped.Bip01_Pelvis" 0.000005 3.345127 -2.981901 4.944225 89.999983 89.999983 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine1" "ValveBiped.Bip01_Spine" 4.01833 0 0 0 -1.675443 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine2" "ValveBiped.Bip01_Spine1" 3.518562 0 0 0 5.74883 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine4" "ValveBiped.Bip01_Spine2" 8.942638 0 0 0 11.120882 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Neck1" "ValveBiped.Bip01_Spine4" 3.30727 0 0 0 22.9457 179.999856 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Head1" "ValveBiped.Bip01_Neck1" 3.593712 -0.000004 0 -0.000057 23.295721 0 0 0 0 0 0 0
$definebone "ValveBiped.forward" "ValveBiped.Bip01_Head1" 2.000004 -3.000001 0 0 -75.999987 -89.999983 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Clavicle" "ValveBiped.Bip01_Spine4" 2.033356 1.000772 -1.93761 73.712051 168.618794 -95.596517 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_UpperArm" "ValveBiped.Bip01_R_Clavicle" 6.028142 0.000011 0 -0.46209 -33.623806 93.935237 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_R_UpperArm" 11.692547 0 0.000004 -0.000012 -3.45906 0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Hand" "ValveBiped.Bip01_R_Forearm" 11.481695 0 0.000004 6.108065 2.532073 -89.664286 0 0 0 0 0 0
$definebone "ValveBiped.Anim_Attachment_RH" "ValveBiped.Bip01_R_Hand" 2.676083 -1.71244 0 -0.000001 -89.999969 -89.999983 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Clavicle" "ValveBiped.Bip01_Spine4" 2.033348 1.000767 1.93766 -73.711873 168.619245 91.480563 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_UpperArm" "ValveBiped.Bip01_L_Clavicle" 6.028142 0 0 2.738852 -33.527948 -90.505157 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_L_UpperArm" 11.692556 0 0.000004 -0.000002 -3.459061 0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Hand" "ValveBiped.Bip01_L_Forearm" 11.481672 0 -0.000027 -6.108475 2.532071 90.136744 0 0 0 0 0 0
$definebone "ValveBiped.Anim_Attachment_LH" "ValveBiped.Bip01_L_Hand" 2.676088 -1.712444 0 0.000001 89.999983 90.000037 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Thigh" "ValveBiped.Bip01_Pelvis" -3.890452 0.000004 0.000007 2.9705 -90.29935 -89.999983 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Calf" "ValveBiped.Bip01_R_Thigh" 17.848167 0 0 0 1.958771 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Foot" "ValveBiped.Bip01_R_Calf" 16.525251 0 0 2.336464 -61.4 2.212247 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Toe0" "ValveBiped.Bip01_R_Foot" 6.879452 0 0 -0.16908 -33.482114 -4.660038 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Thigh" "ValveBiped.Bip01_Pelvis" 3.890452 -0.000004 -0.000003 2.970156 -89.700793 -89.999983 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Calf" "ValveBiped.Bip01_L_Thigh" 17.848169 0 0 0 1.959516 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Foot" "ValveBiped.Bip01_L_Calf" 16.525246 0 0 -2.336637 -61.400334 -1.599412 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Toe0" "ValveBiped.Bip01_L_Foot" 6.879447 -0.000001 0.000001 -0.16908 -33.48211 -0.418202 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger2" "ValveBiped.Bip01_L_Hand" 3.883694 -0.046772 0.431443 1.250766 -33.682286 4.921821 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger21" "ValveBiped.Bip01_L_Finger2" 1.719578 0 0 0.316044 -20.997627 0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger22" "ValveBiped.Bip01_L_Finger21" 1.209187 0 0 0.176701 -12.39879 0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger1" "ValveBiped.Bip01_L_Hand" 3.859861 -0.137951 1.332467 -2.449396 -30.465185 20.308434 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger11" "ValveBiped.Bip01_L_Finger1" 1.719429 0.000004 -0.000001 0.233426 -20.798869 0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger12" "ValveBiped.Bip01_L_Finger11" 1.099667 0.000002 0 0.148743 -13.999123 -0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger0" "ValveBiped.Bip01_L_Hand" 0.806047 -0.348927 1.321259 -38.931853 -45.224008 -69.147647 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger01" "ValveBiped.Bip01_L_Finger0" 1.789783 0 0.000002 0.423179 13.093404 0.000003 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger02" "ValveBiped.Bip01_L_Finger01" 1.206997 0 0.000004 0.645486 20.790348 0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger2" "ValveBiped.Bip01_R_Hand" 3.883654 -0.050327 -0.431043 -1.512839 -33.672454 -4.921994 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger21" "ValveBiped.Bip01_R_Finger2" 1.719576 0 0 -0.456878 -20.99524 -0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger22" "ValveBiped.Bip01_R_Finger21" 1.209183 -0.000002 0 -0.255598 -12.397429 -0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger1" "ValveBiped.Bip01_R_Hand" 3.859829 -0.148933 -1.331281 2.209725 -30.481424 -20.306718 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger11" "ValveBiped.Bip01_R_Finger1" 1.719421 0 0 -0.378899 -20.796646 -0.000007 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger12" "ValveBiped.Bip01_R_Finger11" 1.099667 0.000004 0 -0.241277 -13.998101 -0.000007 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger0" "ValveBiped.Bip01_R_Hand" 0.806013 -0.359833 -1.318333 38.59547 -45.490331 68.99558 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger01" "ValveBiped.Bip01_R_Finger0" 1.789782 -0.000003 0.000002 -0.361025 13.095238 0.000007 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01" 1.207001 0 0.000004 -0.551078 20.792928 0.000005 0 0 0 0 0 0
$definebone "ValveBiped.baton_parent" "ValveBiped.Bip01_R_Hand" 2.338451 -1.535736 -6.391509 -5.407577 -114.703329 87.103621 0 0 0 0 0 0

$bonemerge "ValveBiped.Bip01_Pelvis"
$bonemerge "ValveBiped.Bip01_Spine"
$bonemerge "ValveBiped.Bip01_Spine1"
$bonemerge "ValveBiped.Bip01_Spine2"
$bonemerge "ValveBiped.Bip01_Spine4"
$bonemerge "ValveBiped.Bip01_R_Clavicle"
$bonemerge "ValveBiped.Bip01_R_UpperArm"
$bonemerge "ValveBiped.Bip01_R_Forearm"
$bonemerge "ValveBiped.Bip01_R_Hand"
$bonemerge "ValveBiped.Anim_Attachment_RH"
$bonemerge "ValveBiped.baton_parent"


$ikchain "rhand" "ValveBiped.Bip01_R_Hand" knee 0.707 0.707 0
$ikchain "lhand" "ValveBiped.Bip01_L_Hand" knee 0.707 0.707 0
$ikchain "rfoot" "ValveBiped.Bip01_R_Foot" knee 0.707 -0.707 0
$ikchain "lfoot" "ValveBiped.Bip01_L_Foot" knee 0.707 -0.707 0

$ikautoplaylock "rfoot" 1 0.1
$ikautoplaylock "lfoot" 1 0.1

$sequence "ragdoll" {
	"police_anims\ragdoll.smd"
	activity "ACT_DIERAGDOLL" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release
	ikrule "lhand" release
	ikrule "rfoot" release
	ikrule "lfoot" release
}

$includemodel "elitepolice_animations.mdl"
$includemodel "combine_soldier_anims.mdl"
$includemodel "player/Police_animations.mdl"
$includemodel "player/Police_ss.mdl"

$collisionjoints "police_physics.smd"
{
	$mass 0
	$inertia 2
	$damping 0.01
	$rotdamping 0.4


	$jointconstrain "ValveBiped.Bip01_Spine2" x limit -48 48 0.2
	$jointconstrain "ValveBiped.Bip01_Spine2" y limit -25 25 0.2
	$jointconstrain "ValveBiped.Bip01_Spine2" z limit -25 50 0.2

	$jointconstrain "ValveBiped.Bip01_R_UpperArm" x limit -50 57 0.2
	$jointconstrain "ValveBiped.Bip01_R_UpperArm" y limit -19 102 0.2
	$jointconstrain "ValveBiped.Bip01_R_UpperArm" z limit -93 30 0.2

	$jointconstrain "ValveBiped.Bip01_L_UpperArm" x limit -50 57 0.2
	$jointconstrain "ValveBiped.Bip01_L_UpperArm" y limit -102 19 0.2
	$jointconstrain "ValveBiped.Bip01_L_UpperArm" z limit -93 30 0.2

	$jointmassbias "ValveBiped.Bip01_L_Forearm" 2
	$jointconstrain "ValveBiped.Bip01_L_Forearm" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Forearm" y limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Forearm" z limit -120 4 0.2

	$jointmassbias "ValveBiped.Bip01_L_Hand" 4
	$jointconstrain "ValveBiped.Bip01_L_Hand" x limit -60 60 0.2
	$jointconstrain "ValveBiped.Bip01_L_Hand" y limit -30 30 0.2
	$jointconstrain "ValveBiped.Bip01_L_Hand" z limit -57 70 0.2

	$jointmassbias "ValveBiped.Bip01_R_Forearm" 2
	$jointconstrain "ValveBiped.Bip01_R_Forearm" x limit 0 0 0.2
	$jointconstrain "ValveBiped.Bip01_R_Forearm" y limit 0 0 0.2
	$jointconstrain "ValveBiped.Bip01_R_Forearm" z limit -120 4 0.2

	$jointmassbias "ValveBiped.Bip01_R_Hand" 4
	$jointconstrain "ValveBiped.Bip01_R_Hand" x limit -60 60 0.2
	$jointconstrain "ValveBiped.Bip01_R_Hand" y limit -30 30 0.2
	$jointconstrain "ValveBiped.Bip01_R_Hand" z limit -57 70 0.2

	$jointconstrain "ValveBiped.Bip01_R_Thigh" x limit -25 39 0.2
	$jointconstrain "ValveBiped.Bip01_R_Thigh" y limit -8 75 0.2
	$jointconstrain "ValveBiped.Bip01_R_Thigh" z limit -97 32 0.2

	$jointmassbias "ValveBiped.Bip01_R_Calf" 2
	$jointconstrain "ValveBiped.Bip01_R_Calf" x limit 0 0 0.2
	$jointconstrain "ValveBiped.Bip01_R_Calf" y limit 0 0 0.2
	$jointconstrain "ValveBiped.Bip01_R_Calf" z limit -12 126 0.2

	$jointmassbias "ValveBiped.Bip01_Head1" 2
	$jointconstrain "ValveBiped.Bip01_Head1" x limit -20 20 0.2
	$jointconstrain "ValveBiped.Bip01_Head1" y limit -25 25 0.2
	$jointconstrain "ValveBiped.Bip01_Head1" z limit -13 30 0.2

	$jointconstrain "ValveBiped.Bip01_L_Thigh" x limit -25 20 0.2
	$jointconstrain "ValveBiped.Bip01_L_Thigh" y limit -73 6 0.2
	$jointconstrain "ValveBiped.Bip01_L_Thigh" z limit -93 30 0.2

	$jointmassbias "ValveBiped.Bip01_L_Calf" 2
	$jointconstrain "ValveBiped.Bip01_L_Calf" x limit 0 0 0.2
	$jointconstrain "ValveBiped.Bip01_L_Calf" y limit 0 0 0.2
	$jointconstrain "ValveBiped.Bip01_L_Calf" z limit -8 126 0.2

	$jointmassbias "ValveBiped.Bip01_L_Foot" 2
	$jointconstrain "ValveBiped.Bip01_L_Foot" x limit -1 -1 0.2
	$jointconstrain "ValveBiped.Bip01_L_Foot" y limit -19 19 0.2
	$jointconstrain "ValveBiped.Bip01_L_Foot" z limit -15 35 0.2

	$jointmassbias "ValveBiped.Bip01_R_Foot" 2
	$jointconstrain "ValveBiped.Bip01_R_Foot" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Foot" y limit -21 6 0.2
	$jointconstrain "ValveBiped.Bip01_R_Foot" z limit -15 35 0.2
	$jointcollide "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_R_Thigh"
	$jointcollide "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_L_Thigh"
	$jointcollide "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_R_Thigh"
	$jointcollide "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_L_Thigh"
	$jointcollide "ValveBiped.Bip01_R_Foot" "ValveBiped.Bip01_L_Calf"
	$jointcollide "ValveBiped.Bip01_L_Foot" "ValveBiped.Bip01_R_Calf"
	$jointcollide "ValveBiped.Bip01_L_Foot" "ValveBiped.Bip01_R_Foot"
	$jointcollide "ValveBiped.Bip01_R_Calf" "ValveBiped.Bip01_L_Calf"
	$jointcollide "ValveBiped.Bip01_R_Thigh" "ValveBiped.Bip01_L_Thigh"
	$jointcollide "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_Spine2"
	$jointcollide "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_Spine2"
	$jointcollide "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_Pelvis"
	$jointcollide "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_Pelvis"
}

$collisiontext
{
	animatedfriction 
	{
		"animfrictionmin" "1.000000"
		"animfrictionmax" "1000.000000"
		"animfrictiontimein" "0.800000"
		"animfrictiontimeout" "0.500000"
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
