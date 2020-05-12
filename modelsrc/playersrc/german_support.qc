// Created by Crowbar 0.57

$modelname "player\playermodels\german_support.mdl"

$bodygroup "body"
{
	studio "german_support_dod_german_german_support_reference.smd"
}
$bodygroup "helmet"
{
	studio "german_support_dod_german_german_helmet_reference.smd"
	blank
}


$surfaceprop "flesh"

$contents "solid"

$illumposition -0.01 1.402 34.797

$eyeposition 0 0 70

$mostlyopaque


$cdmaterials "models\player\german\"
$cdmaterials "models\humans\male\"

$attachment "forward" "ValveBiped.forward" 0 0 0 rotate 0 0 0
$attachment "anim_attachment_RH" "ValveBiped.Anim_Attachment_RH" 0 0 0 rotate -90 -90 0
$attachment "anim_attachment_LH" "ValveBiped.Anim_Attachment_LH" 0 0 0 rotate -90 -90 0
$attachment "anim_attachment_head" "ValveBiped.Bip01_Head1" 0 0 0 rotate -90 -90 0
$attachment "eyes" "ValveBiped.Bip01_Head1" 3.43 -3.51 0.05 rotate 0 -80.1 -90
$attachment "mouth" "ValveBiped.Bip01_Head1" 0.8 -5.5 0.1 rotate 0 -80 -90
$attachment "chest" "ValveBiped.Bip01_Spine2" 5 4 0 rotate 0 90 90

$cbox 0 0 0 0 0 0

$bbox -13 -13 0 13 13 72

$definebone "ValveBiped.Bip01_Pelvis" "" -0.000005 -0.533615 38.566917 0 0 89.999983 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine" "ValveBiped.Bip01_Pelvis" 0.000005 3.345127 -2.981901 4.944225 89.999983 89.999983 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine1" "ValveBiped.Bip01_Spine" 4.018322 0 0 0 -1.675443 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine2" "ValveBiped.Bip01_Spine1" 3.518555 0 0 0 5.748831 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine4" "ValveBiped.Bip01_Spine2" 8.942631 0 0 0 11.120882 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Neck1" "ValveBiped.Bip01_Spine4" 3.30727 -0.000002 0 0 22.9457 179.999856 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Head1" "ValveBiped.Bip01_Neck1" 3.593712 -0.000004 0 -0.000057 23.295723 0 0 0 0 0 0 0
$definebone "ValveBiped.forward" "ValveBiped.Bip01_Head1" 2.000019 -3.000001 0 0 -75.999987 -89.999983 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Clavicle" "ValveBiped.Bip01_Spine4" 2.033356 1.000772 -1.93761 73.712051 168.618794 -95.596517 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_UpperArm" "ValveBiped.Bip01_R_Clavicle" 6.028144 0.000008 0 -1.22252 -29.867328 94.075256 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_R_UpperArm" 11.692541 0 0.000011 -0.00001 -4.459791 0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Hand" "ValveBiped.Bip01_R_Forearm" 11.481699 -0.000001 0.000011 6.089154 4.890713 0.586365 0 0 0 0 0 0
$definebone "ValveBiped.Anim_Attachment_RH" "ValveBiped.Bip01_R_Hand" 2.67609 -1.712443 0 0 -89.999983 -89.999969 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Clavicle" "ValveBiped.Bip01_Spine4" 2.033348 1.000767 1.93766 -73.711867 168.619245 91.480563 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_UpperArm" "ValveBiped.Bip01_L_Clavicle" 6.028142 0 0 3.269411 -29.727118 -90.501203 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_L_UpperArm" 11.692574 0 0.000004 0.00001 -4.462026 0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Hand" "ValveBiped.Bip01_L_Forearm" 11.481688 0 -0.000038 -6.108921 4.890882 -0.114304 0 0 0 0 0 0
$definebone "ValveBiped.Anim_Attachment_LH" "ValveBiped.Bip01_L_Hand" 2.676083 -1.712445 -0.000008 -0.000002 90.000037 90.000051 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Thigh" "ValveBiped.Bip01_Pelvis" -3.890452 0.000004 0.000007 3.889982 -96.119914 -89.999983 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Calf" "ValveBiped.Bip01_R_Thigh" 17.848173 0 0 0 0 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Foot" "ValveBiped.Bip01_R_Calf" 16.52524 0 0 13.16663 -58.015204 9.39341 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Toe0" "ValveBiped.Bip01_R_Foot" 6.879447 -0.000002 -0.000002 -0.168221 -33.966032 -4.281998 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Thigh" "ValveBiped.Bip01_Pelvis" 3.890452 -0.000004 -0.000003 3.89004 -83.880277 -89.999983 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Calf" "ValveBiped.Bip01_L_Thigh" 17.848179 0 0 0 0 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Foot" "ValveBiped.Bip01_L_Calf" 16.525251 0 0 -13.166797 -58.015361 -8.7992 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Toe0" "ValveBiped.Bip01_L_Foot" 6.879457 0 -0.000003 -0.163752 -33.966026 -0.43304 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger4" "ValveBiped.Bip01_L_Hand" 3.859707 -0.142405 -1.191971 6.33635 -48.929222 -27.095296 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger41" "ValveBiped.Bip01_L_Finger4" 1.312561 0 -0.000008 0.689394 -6.869135 0.275365 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger42" "ValveBiped.Bip01_L_Finger41" 0.729364 -0.000004 0 0.351518 -2.716044 0.000054 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger3" "ValveBiped.Bip01_L_Hand" 3.942327 0.046771 -0.43145 4.543791 -46.421333 -8.880857 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger31" "ValveBiped.Bip01_L_Finger3" 1.539099 0.000001 0.000004 0.335415 -14.196172 0.00005 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger32" "ValveBiped.Bip01_L_Finger31" 1.196327 -0.000001 0.000004 0.553713 7.890832 -0.00001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger2" "ValveBiped.Bip01_L_Hand" 3.88369 -0.046772 0.43145 1.512851 -33.672457 4.921988 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger21" "ValveBiped.Bip01_L_Finger2" 1.719578 -0.000001 0 0.456827 -20.995192 0.000046 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger22" "ValveBiped.Bip01_L_Finger21" 1.209186 -0.000002 0 0.255547 -12.397379 0.000044 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger1" "ValveBiped.Bip01_L_Hand" 3.859863 -0.137948 1.332474 2.811976 -28.624403 0.1096 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger11" "ValveBiped.Bip01_L_Finger1" 1.719435 0.000002 0.000004 0.378849 -20.796593 0.000051 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger12" "ValveBiped.Bip01_L_Finger11" 1.099669 -0.000004 -0.000004 0.241222 -13.99793 -0.000009 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger0" "ValveBiped.Bip01_L_Hand" 0.806047 -0.348922 1.321259 -38.595501 -45.490338 -68.995478 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger01" "ValveBiped.Bip01_L_Finger0" 1.789785 0.000008 0.000001 0.361031 13.095197 0.000011 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger02" "ValveBiped.Bip01_L_Finger01" 1.207001 0.000004 0.000002 0.551135 20.792882 0.000013 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger4" "ValveBiped.Bip01_R_Hand" 3.859673 -0.132568 1.193108 -6.336345 -48.92917 27.42469 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger41" "ValveBiped.Bip01_R_Finger4" 1.312561 -0.000004 -0.000004 -0.689447 -6.869142 0.275433 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger42" "ValveBiped.Bip01_R_Finger41" 0.729362 -0.000004 0.000008 -0.351513 -2.716047 0.000127 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger3" "ValveBiped.Bip01_R_Hand" 3.942287 0.050328 0.431038 -4.543847 -46.421333 8.880797 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger31" "ValveBiped.Bip01_R_Finger3" 1.539097 -0.000004 0.000008 -0.335414 -14.196295 0.000065 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31" 1.19632 -0.000006 -0.000008 -0.55371 7.890831 0.000123 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger2" "ValveBiped.Bip01_R_Hand" 3.88365 -0.050328 -0.431046 -1.512842 -33.672464 -4.921991 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger21" "ValveBiped.Bip01_R_Finger2" 1.719576 -0.000003 0 -0.456883 -20.995315 0.000003 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger22" "ValveBiped.Bip01_R_Finger21" 1.209178 -0.000006 0.000008 -0.255603 -12.397504 0.000068 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger1" "ValveBiped.Bip01_R_Hand" 3.859821 -0.148935 -1.331287 -2.812026 -28.62435 -0.10966 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger11" "ValveBiped.Bip01_R_Finger1" 1.719425 0.000001 0 -0.378899 -20.796719 0.000003 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger12" "ValveBiped.Bip01_R_Finger11" 1.099666 -0.000005 0 -0.241275 -13.998114 0.000061 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger0" "ValveBiped.Bip01_R_Hand" 0.806017 -0.359833 -1.31834 38.59547 -45.490399 68.995635 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger01" "ValveBiped.Bip01_R_Finger0" 1.78978 0.000008 0.000002 -0.361024 13.095175 0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01" 1.207001 -0.000004 0 -0.551189 20.79292 0 0 0 0 0 0 0

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


$ikchain "rhand" "ValveBiped.Bip01_R_Hand" knee 0.707 0.707 0
$ikchain "lhand" "ValveBiped.Bip01_L_Hand" knee 0.707 0.707 0
$ikchain "rfoot" "ValveBiped.Bip01_R_Foot" knee 0.707 -0.707 0
$ikchain "lfoot" "ValveBiped.Bip01_L_Foot" knee 0.707 -0.707 0

$ikautoplaylock "rfoot" 1 0.1
$ikautoplaylock "lfoot" 1 0.1

$sequence "ragdoll" {
	"german_support_anims\ragdoll.smd"
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
$includemodel "player/male_shared.mdl"
$includemodel "player/male_ss.mdl"
$includemodel "player/male_gestures.mdl"
$includemodel "player/male_postures.mdl"

$collisionjoints "german_support_physics.smd"
{
	$mass 100
	$inertia 10
	$damping 0
	$rotdamping 4
	$rootbone "ValveBiped.Bip01_pelvis"

	$jointrotdamping "ValveBiped.Bip01_Pelvis" 0

	$jointmassbias "ValveBiped.Bip01_L_Thigh" 0.8
	$jointconstrain "ValveBiped.Bip01_L_Thigh" x limit -12 12 0
	$jointconstrain "ValveBiped.Bip01_L_Thigh" y limit -20 12 0
	$jointconstrain "ValveBiped.Bip01_L_Thigh" z limit -79 24 0

	$jointmassbias "ValveBiped.Bip01_L_Calf" 1.2
	$jointrotdamping "ValveBiped.Bip01_L_Calf" 3
	$jointconstrain "ValveBiped.Bip01_L_Calf" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Calf" y limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Calf" z limit 0 116 0

	$jointmassbias "ValveBiped.Bip01_R_Thigh" 0.8
	$jointconstrain "ValveBiped.Bip01_R_Thigh" x limit -12 12 0
	$jointconstrain "ValveBiped.Bip01_R_Thigh" y limit -12 20 0
	$jointconstrain "ValveBiped.Bip01_R_Thigh" z limit -79 24 0

	$jointmassbias "ValveBiped.Bip01_R_Calf" 1.2
	$jointrotdamping "ValveBiped.Bip01_R_Calf" 3
	$jointconstrain "ValveBiped.Bip01_R_Calf" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Calf" y limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Calf" z limit 0 116 0

	$jointmassbias "ValveBiped.Bip01_R_Foot" 0.75
	$jointrotdamping "ValveBiped.Bip01_R_Foot" 7
	$jointconstrain "ValveBiped.Bip01_R_Foot" x limit -10 22 0
	$jointconstrain "ValveBiped.Bip01_R_Foot" y limit -19 25 0
	$jointconstrain "ValveBiped.Bip01_R_Foot" z limit -17 24 0

	$jointrotdamping "ValveBiped.Bip01_Spine1" 18
	$jointconstrain "ValveBiped.Bip01_Spine1" x limit -20 20 0
	$jointconstrain "ValveBiped.Bip01_Spine1" y limit -10 10 0
	$jointconstrain "ValveBiped.Bip01_Spine1" z limit -15 30 0

	$jointmassbias "ValveBiped.Bip01_Spine2" 1.6
	$jointrotdamping "ValveBiped.Bip01_Spine2" 12
	$jointconstrain "ValveBiped.Bip01_Spine2" x limit -30 30 0
	$jointconstrain "ValveBiped.Bip01_Spine2" y limit -16 16 0
	$jointconstrain "ValveBiped.Bip01_Spine2" z limit -20 15 0

	$jointmassbias "ValveBiped.Bip01_L_Clavicle" 0.6
	$jointrotdamping "ValveBiped.Bip01_L_Clavicle" 12.5
	$jointconstrain "ValveBiped.Bip01_L_Clavicle" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Clavicle" y limit -16 16 0
	$jointconstrain "ValveBiped.Bip01_L_Clavicle" z limit 0 20 0

	$jointmassbias "ValveBiped.Bip01_L_UpperArm" 0.8
	$jointrotdamping "ValveBiped.Bip01_L_UpperArm" 16
	$jointconstrain "ValveBiped.Bip01_L_UpperArm" x limit -40 50 0
	$jointconstrain "ValveBiped.Bip01_L_UpperArm" y limit -55 32 0
	$jointconstrain "ValveBiped.Bip01_L_UpperArm" z limit -80 25 0

	$jointmassbias "ValveBiped.Bip01_R_Clavicle" 0.6
	$jointrotdamping "ValveBiped.Bip01_R_Clavicle" 12.5
	$jointconstrain "ValveBiped.Bip01_R_Clavicle" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Clavicle" y limit -16 16 0
	$jointconstrain "ValveBiped.Bip01_R_Clavicle" z limit 0 20 0

	$jointmassbias "ValveBiped.Bip01_R_UpperArm" 0.8
	$jointrotdamping "ValveBiped.Bip01_R_UpperArm" 16
	$jointconstrain "ValveBiped.Bip01_R_UpperArm" x limit -50 40 0
	$jointconstrain "ValveBiped.Bip01_R_UpperArm" y limit -32 55 0
	$jointconstrain "ValveBiped.Bip01_R_UpperArm" z limit -80 25 0

	$jointmassbias "ValveBiped.Bip01_R_Forearm" 1.8
	$jointconstrain "ValveBiped.Bip01_R_Forearm" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Forearm" y limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_R_Forearm" z limit -120 -6 0

	$jointmassbias "ValveBiped.Bip01_R_Hand" 0.6
	$jointrotdamping "ValveBiped.Bip01_R_Hand" 8
	$jointconstrain "ValveBiped.Bip01_R_Hand" x limit -37 30 0
	$jointconstrain "ValveBiped.Bip01_R_Hand" y limit -8 12 0
	$jointconstrain "ValveBiped.Bip01_R_Hand" z limit -35 20 0

	$jointmassbias "ValveBiped.Bip01_L_Forearm" 1.8
	$jointconstrain "ValveBiped.Bip01_L_Forearm" x limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Forearm" y limit 0 0 0
	$jointconstrain "ValveBiped.Bip01_L_Forearm" z limit -120 -6 0

	$jointmassbias "ValveBiped.Bip01_L_Hand" 0.6
	$jointrotdamping "ValveBiped.Bip01_L_Hand" 8
	$jointconstrain "ValveBiped.Bip01_L_Hand" x limit -30 37 0
	$jointconstrain "ValveBiped.Bip01_L_Hand" y limit -12 8 0
	$jointconstrain "ValveBiped.Bip01_L_Hand" z limit -20 35 0

	$jointmassbias "ValveBiped.Bip01_Head1" 1.5
	$jointrotdamping "ValveBiped.Bip01_Head1" 8
	$jointconstrain "ValveBiped.Bip01_Head1" x limit -50 50 0
	$jointconstrain "ValveBiped.Bip01_Head1" y limit -20 20 0
	$jointconstrain "ValveBiped.Bip01_Head1" z limit -26 30 0

	$jointmassbias "ValveBiped.Bip01_L_Foot" 0.75
	$jointrotdamping "ValveBiped.Bip01_L_Foot" 7
	$jointconstrain "ValveBiped.Bip01_L_Foot" x limit -10 22 0
	$jointconstrain "ValveBiped.Bip01_L_Foot" y limit -19 25 0
	$jointconstrain "ValveBiped.Bip01_L_Foot" z limit -17 24 0
	$jointcollide "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_L_Thigh"
	$jointcollide "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_Pelvis"
	$jointcollide "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_Spine1"
	$jointcollide "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_Spine2"
	$jointcollide "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_Head1"
	$jointcollide "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_R_Forearm"
	$jointcollide "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_R_Hand"
	$jointcollide "ValveBiped.Bip01_L_Hand" "ValveBiped.Bip01_L_Thigh"
	$jointcollide "ValveBiped.Bip01_L_Hand" "ValveBiped.Bip01_Pelvis"
	$jointcollide "ValveBiped.Bip01_L_Hand" "ValveBiped.Bip01_Spine1"
	$jointcollide "ValveBiped.Bip01_L_Hand" "ValveBiped.Bip01_Spine2"
	$jointcollide "ValveBiped.Bip01_L_Hand" "ValveBiped.Bip01_Head1"
	$jointcollide "ValveBiped.Bip01_L_Hand" "ValveBiped.Bip01_R_Hand"
	$jointcollide "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_R_Thigh"
	$jointcollide "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_Pelvis"
	$jointcollide "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_Spine1"
	$jointcollide "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_Spine2"
	$jointcollide "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_Head1"
	$jointcollide "ValveBiped.Bip01_R_Hand" "ValveBiped.Bip01_R_Thigh"
	$jointcollide "ValveBiped.Bip01_R_Hand" "ValveBiped.Bip01_Pelvis"
	$jointcollide "ValveBiped.Bip01_R_Hand" "ValveBiped.Bip01_Spine1"
	$jointcollide "ValveBiped.Bip01_R_Hand" "ValveBiped.Bip01_Spine2"
	$jointcollide "ValveBiped.Bip01_R_Hand" "ValveBiped.Bip01_Head1"
	$jointcollide "ValveBiped.Bip01_R_Thigh" "ValveBiped.Bip01_L_Thigh"
	$jointcollide "ValveBiped.Bip01_R_Thigh" "ValveBiped.Bip01_L_Calf"
	$jointcollide "ValveBiped.Bip01_R_Thigh" "ValveBiped.Bip01_L_Foot"
	$jointcollide "ValveBiped.Bip01_R_Calf" "ValveBiped.Bip01_L_Thigh"
	$jointcollide "ValveBiped.Bip01_R_Calf" "ValveBiped.Bip01_L_Calf"
	$jointcollide "ValveBiped.Bip01_R_Calf" "ValveBiped.Bip01_L_Foot"
	$jointcollide "ValveBiped.Bip01_R_Foot" "ValveBiped.Bip01_L_Thigh"
	$jointcollide "ValveBiped.Bip01_R_Foot" "ValveBiped.Bip01_L_Calf"
	$jointcollide "ValveBiped.Bip01_R_Foot" "ValveBiped.Bip01_L_Foot"
}
