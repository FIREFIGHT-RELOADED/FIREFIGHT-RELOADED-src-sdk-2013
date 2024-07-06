// Created by Crowbar 0.72

$modelname "Police_animations_noconflicts.mdl"

$surfaceprop "default"

$contents "solid"

$illumposition 0 -0.752 33.456

$cdmaterials "models\police_xsi\"

$attachment "forward" "ValveBiped.forward" 0 0 0 rotate 0 0 0
$attachment "anim_attachment_RH" "ValveBiped.Anim_Attachment_RH" 0 0 0 rotate -90 -90 0
$attachment "anim_attachment_LH" "ValveBiped.Anim_Attachment_LH" 0 0 0 rotate -90 -90 0
$attachment "anim_attachment_head" "ValveBiped.Bip01_Head1" 0 0 0 rotate -90 -90 0
$attachment "eyes" "ValveBiped.Bip01_Head1" 2.7 -7.43 0 rotate 0 -80.1 -90
$attachment "baton" "ValveBiped.baton_parent" 0 0 0 rotate 0 0 0

$cbox 0 0 0 0 0 0

$bbox -13.702 -22.723 0.328 15.207 22.723 66.584

$definebone "ValveBiped.Bip01_Pelvis" "" -0.000005 -0.533615 38.566917 0 0 89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine" "ValveBiped.Bip01_Pelvis" 0.000005 3.345128 -2.981901 4.944224 89.999982 89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine1" "ValveBiped.Bip01_Spine" 4.018331 0 0 0 -1.675443 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine2" "ValveBiped.Bip01_Spine1" 3.518565 0 0 0 5.74883 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Spine4" "ValveBiped.Bip01_Spine2" 8.942642 0 0 0 11.120881 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Neck1" "ValveBiped.Bip01_Spine4" 3.307271 0 0 0 22.945694 179.999855 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_Head1" "ValveBiped.Bip01_Neck1" 3.593712 -0.000001 0 -0.000057 23.295723 0 0 0 0 0 0 0
$definebone "ValveBiped.forward" "ValveBiped.Bip01_Head1" 1.999997 -3.000001 0 0 -76 -90.000003 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Clavicle" "ValveBiped.Bip01_Spine4" 2.03335 1.000771 -1.93761 73.71203 168.618779 -95.596496 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_UpperArm" "ValveBiped.Bip01_R_Clavicle" 6.028143 0.000002 0 -0.46209 -33.623802 93.935236 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Forearm" "ValveBiped.Bip01_R_UpperArm" 11.692549 0 0.000002 -0.000007 -3.459061 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Hand" "ValveBiped.Bip01_R_Forearm" 11.481698 0 0 6.108069 2.532072 -89.664285 0 0 0 0 0 0
$definebone "ValveBiped.Anim_Attachment_RH" "ValveBiped.Bip01_R_Hand" 2.676087 -1.71244 0 0 -89.999975 -89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Clavicle" "ValveBiped.Bip01_Spine4" 2.033342 1.000767 1.93766 -73.711859 168.619271 91.480542 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_UpperArm" "ValveBiped.Bip01_L_Clavicle" 6.028142 0.000002 0 2.738795 -33.527944 -90.505156 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Forearm" "ValveBiped.Bip01_L_UpperArm" 11.692555 0 0.000005 -0.000007 -3.459061 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Hand" "ValveBiped.Bip01_L_Forearm" 11.481671 0 -0.00003 -6.108482 2.532072 90.136743 0 0 0 0 0 0
$definebone "ValveBiped.Anim_Attachment_LH" "ValveBiped.Bip01_L_Hand" 2.676088 -1.712441 0 -0.000019 90.000023 90.000023 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Thigh" "ValveBiped.Bip01_Pelvis" -3.890452 0.000004 0.000007 2.9705 -90.299349 -89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Calf" "ValveBiped.Bip01_R_Thigh" 17.848171 0 0 0 1.958771 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Foot" "ValveBiped.Bip01_R_Calf" 16.525253 0 0 2.336451 -61.399999 2.212226 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Toe0" "ValveBiped.Bip01_R_Foot" 6.879454 0 0 -0.16906 -33.48213 -4.660049 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Thigh" "ValveBiped.Bip01_Pelvis" 3.890452 -0.000004 -0.000003 2.970156 -89.700792 -89.999982 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Calf" "ValveBiped.Bip01_L_Thigh" 17.848175 0 0 0 1.959516 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Foot" "ValveBiped.Bip01_L_Calf" 16.525249 0 0 -2.336618 -61.400341 -1.599413 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Toe0" "ValveBiped.Bip01_L_Foot" 6.879452 0 0 -0.169039 -33.482137 -0.418217 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger4" "ValveBiped.Bip01_L_Hand" 3.859704 -0.142404 -1.191975 8.719849 -50.413059 -27.452933 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger41" "ValveBiped.Bip01_L_Finger4" 1.312563 0.000001 0.000001 0.553764 -25.194097 -0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger42" "ValveBiped.Bip01_L_Finger41" 0.729362 0.000002 0 0.284816 -13.997302 0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger3" "ValveBiped.Bip01_L_Hand" 3.942327 0.046774 -0.431443 4.201386 -46.446697 -8.881647 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger31" "ValveBiped.Bip01_L_Finger3" 1.539092 0.000002 0 0.255539 -14.197714 -0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger32" "ValveBiped.Bip01_L_Finger31" 1.196319 -0.000001 0 0.421753 -24.683137 0 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger2" "ValveBiped.Bip01_L_Hand" 3.883689 -0.046767 0.431443 1.250767 -33.682282 4.921822 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger21" "ValveBiped.Bip01_L_Finger2" 1.719575 0.000002 0 0.316046 -20.997625 0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger22" "ValveBiped.Bip01_L_Finger21" 1.209182 -0.000001 0 0.176703 -12.398794 0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger1" "ValveBiped.Bip01_L_Hand" 3.859859 -0.13795 1.332466 -2.449395 -30.465185 20.308434 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger11" "ValveBiped.Bip01_L_Finger1" 1.719429 0.000003 0 0.233431 -20.79887 0.000006 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger12" "ValveBiped.Bip01_L_Finger11" 1.099668 0.000001 0 0.148748 -13.999125 0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger0" "ValveBiped.Bip01_L_Hand" 0.806048 -0.348923 1.321258 -38.931808 -45.224011 -69.147647 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger01" "ValveBiped.Bip01_L_Finger0" 1.789785 0 0.000002 0.423234 13.093399 0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_L_Finger02" "ValveBiped.Bip01_L_Finger01" 1.206998 0 0 0.645542 20.790345 -0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger4" "ValveBiped.Bip01_R_Hand" 3.859672 -0.132566 1.193108 -9.083957 -50.36534 27.452701 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger41" "ValveBiped.Bip01_R_Finger4" 1.31256 0 -0.000001 -0.683651 -25.191353 -0.000005 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger42" "ValveBiped.Bip01_R_Finger41" 0.729361 0.000002 0 -0.351451 -13.995692 -0.000005 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger3" "ValveBiped.Bip01_R_Hand" 3.942293 0.05033 0.431043 -4.543841 -46.421333 8.880789 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger31" "ValveBiped.Bip01_R_Finger3" 1.539095 0.000003 0 -0.335409 -14.196231 -0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger32" "ValveBiped.Bip01_R_Finger31" 1.196323 0 0 -0.553649 -24.680776 -0.000003 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger2" "ValveBiped.Bip01_R_Hand" 3.883655 -0.050326 -0.431043 -1.512837 -33.672447 -4.921995 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger21" "ValveBiped.Bip01_R_Finger2" 1.719575 -0.000005 0 -0.456876 -20.99524 -0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger22" "ValveBiped.Bip01_R_Finger21" 1.209182 -0.000001 0 -0.255596 -12.397432 -0.000002 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger1" "ValveBiped.Bip01_R_Hand" 3.859828 -0.148935 -1.331282 2.209727 -30.48142 -20.306719 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger11" "ValveBiped.Bip01_R_Finger1" 1.719425 0 0 -0.378896 -20.796657 0.000003 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger12" "ValveBiped.Bip01_R_Finger11" 1.099665 0 0 -0.241272 -13.998055 0.000003 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger0" "ValveBiped.Bip01_R_Hand" 0.806014 -0.359835 -1.318334 38.595463 -45.49033 68.995573 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger01" "ValveBiped.Bip01_R_Finger0" 1.789783 0.000001 0 -0.361024 13.095241 -0.000001 0 0 0 0 0 0
$definebone "ValveBiped.Bip01_R_Finger02" "ValveBiped.Bip01_R_Finger01" 1.207003 0 0.000004 -0.551073 20.792923 -0.000002 0 0 0 0 0 0
$definebone "ValveBiped.baton_parent" "ValveBiped.Bip01_R_Hand" 2.338454 -1.535743 -6.391511 -5.407568 -114.703342 87.103627 0 0 0 0 0 0

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


// The 32 below is a guess until further is known about the format.
$animblocksize 32

$bonesaveframe "ValveBiped.Bip01_Pelvis" position rotation
$bonesaveframe "ValveBiped.Bip01_Spine" rotation
$bonesaveframe "ValveBiped.Bip01_Spine1" rotation
$bonesaveframe "ValveBiped.Bip01_Spine2" rotation
$bonesaveframe "ValveBiped.Bip01_Spine4" rotation
$bonesaveframe "ValveBiped.Bip01_Neck1" rotation
$bonesaveframe "ValveBiped.Bip01_Head1" rotation
$bonesaveframe "ValveBiped.Bip01_R_Clavicle" rotation
$bonesaveframe "ValveBiped.Bip01_R_UpperArm" rotation
$bonesaveframe "ValveBiped.Bip01_R_Forearm" rotation
$bonesaveframe "ValveBiped.Bip01_R_Hand" rotation
$bonesaveframe "ValveBiped.Bip01_L_Clavicle" rotation
$bonesaveframe "ValveBiped.Bip01_L_UpperArm" rotation
$bonesaveframe "ValveBiped.Bip01_L_Forearm" rotation
$bonesaveframe "ValveBiped.Bip01_L_Hand" rotation
$bonesaveframe "ValveBiped.Bip01_R_Thigh" rotation
$bonesaveframe "ValveBiped.Bip01_R_Calf" rotation
$bonesaveframe "ValveBiped.Bip01_R_Foot" rotation
$bonesaveframe "ValveBiped.Bip01_L_Thigh" rotation
$bonesaveframe "ValveBiped.Bip01_L_Calf" rotation
$bonesaveframe "ValveBiped.Bip01_L_Foot" rotation

$sectionframes 30 120

$poseparameter "body_yaw" -29.73426 29.73423 loop 0
$poseparameter "spine_yaw" -30.7085 30.70848 loop 0
$poseparameter "neck_trans" -0.207508 0.2594 loop 0
$poseparameter "head_yaw" -66.70654 66.70631 loop 0
$poseparameter "head_pitch" -34.86658 25.37666 loop 0
$poseparameter "head_roll" -10.93143 10.85243 loop 0
$poseparameter "move_yaw" -180 180 loop 360
$poseparameter "aim_pitch" -86.32401 56.20353 loop 0
$poseparameter "aim_yaw" -60 60 loop 0

$ikchain "rhand" "ValveBiped.Bip01_R_Hand" knee 0.707 0.707 0
$ikchain "lhand" "ValveBiped.Bip01_L_Hand" knee 0.707 0.707 0
$ikchain "rfoot" "ValveBiped.Bip01_R_Foot" knee 0.707 -0.707 0
$ikchain "lfoot" "ValveBiped.Bip01_L_Foot" knee 0.707 -0.707 0

$ikautoplaylock "rfoot" 1 0.1
$ikautoplaylock "lfoot" 1 0.1
$ikautoplaylock "rfoot" 1 0.1
$ikautoplaylock "lfoot" 1 0.1

$weightlist "weights_body_rot_z" {
	 "ValveBiped.Bip01_Pelvis" 1
	 "ValveBiped.Bip01_Spine" 1
	 "ValveBiped.Bip01_Spine1" 1
	 "ValveBiped.Bip01_Spine2" 1
	 "ValveBiped.Bip01_Spine4" 1
	 "ValveBiped.Bip01_Neck1" 1
	 "ValveBiped.Bip01_Head1" 0
	 "ValveBiped.forward" 0
	 "ValveBiped.Bip01_R_Clavicle" 0
	 "ValveBiped.Bip01_R_UpperArm" 0
	 "ValveBiped.Bip01_R_Forearm" 0
	 "ValveBiped.Bip01_R_Hand" 0
	 "ValveBiped.Anim_Attachment_RH" 0
	 "ValveBiped.Bip01_L_Clavicle" 0
	 "ValveBiped.Bip01_L_UpperArm" 0
	 "ValveBiped.Bip01_L_Forearm" 0
	 "ValveBiped.Bip01_L_Hand" 0
	 "ValveBiped.Anim_Attachment_LH" 0
	 "ValveBiped.Bip01_R_Thigh" 0
	 "ValveBiped.Bip01_R_Calf" 0
	 "ValveBiped.Bip01_R_Foot" 0
	 "ValveBiped.Bip01_R_Toe0" 0
	 "ValveBiped.Bip01_L_Thigh" 0
	 "ValveBiped.Bip01_L_Calf" 0
	 "ValveBiped.Bip01_L_Foot" 0
	 "ValveBiped.Bip01_L_Toe0" 0
	 "ValveBiped.Bip01_L_Finger4" 0
	 "ValveBiped.Bip01_L_Finger41" 0
	 "ValveBiped.Bip01_L_Finger42" 0
	 "ValveBiped.Bip01_L_Finger3" 0
	 "ValveBiped.Bip01_L_Finger31" 0
	 "ValveBiped.Bip01_L_Finger32" 0
	 "ValveBiped.Bip01_L_Finger2" 0
	 "ValveBiped.Bip01_L_Finger21" 0
	 "ValveBiped.Bip01_L_Finger22" 0
	 "ValveBiped.Bip01_L_Finger1" 0
	 "ValveBiped.Bip01_L_Finger11" 0
	 "ValveBiped.Bip01_L_Finger12" 0
	 "ValveBiped.Bip01_L_Finger0" 0
	 "ValveBiped.Bip01_L_Finger01" 0
	 "ValveBiped.Bip01_L_Finger02" 0
	 "ValveBiped.Bip01_R_Finger4" 0
	 "ValveBiped.Bip01_R_Finger41" 0
	 "ValveBiped.Bip01_R_Finger42" 0
	 "ValveBiped.Bip01_R_Finger3" 0
	 "ValveBiped.Bip01_R_Finger31" 0
	 "ValveBiped.Bip01_R_Finger32" 0
	 "ValveBiped.Bip01_R_Finger2" 0
	 "ValveBiped.Bip01_R_Finger21" 0
	 "ValveBiped.Bip01_R_Finger22" 0
	 "ValveBiped.Bip01_R_Finger1" 0
	 "ValveBiped.Bip01_R_Finger11" 0
	 "ValveBiped.Bip01_R_Finger12" 0
	 "ValveBiped.Bip01_R_Finger0" 0
	 "ValveBiped.Bip01_R_Finger01" 0
	 "ValveBiped.Bip01_R_Finger02" 0
	 "ValveBiped.baton_parent" 0
}

$weightlist "weights_neck_trans_x" {
	 "ValveBiped.Bip01_Pelvis" 1
	 "ValveBiped.Bip01_Spine" 1
	 "ValveBiped.Bip01_Spine1" 1
	 "ValveBiped.Bip01_Spine2" 1
	 "ValveBiped.Bip01_Spine4" 1
	 "ValveBiped.Bip01_Neck1" 1
	 "ValveBiped.Bip01_Head1" 1
	 "ValveBiped.forward" 1
	 "ValveBiped.Bip01_R_Clavicle" 0
	 "ValveBiped.Bip01_R_UpperArm" 0
	 "ValveBiped.Bip01_R_Forearm" 0
	 "ValveBiped.Bip01_R_Hand" 0
	 "ValveBiped.Anim_Attachment_RH" 0
	 "ValveBiped.Bip01_L_Clavicle" 0
	 "ValveBiped.Bip01_L_UpperArm" 0
	 "ValveBiped.Bip01_L_Forearm" 0
	 "ValveBiped.Bip01_L_Hand" 0
	 "ValveBiped.Anim_Attachment_LH" 0
	 "ValveBiped.Bip01_R_Thigh" 0
	 "ValveBiped.Bip01_R_Calf" 0
	 "ValveBiped.Bip01_R_Foot" 0
	 "ValveBiped.Bip01_R_Toe0" 0
	 "ValveBiped.Bip01_L_Thigh" 0
	 "ValveBiped.Bip01_L_Calf" 0
	 "ValveBiped.Bip01_L_Foot" 0
	 "ValveBiped.Bip01_L_Toe0" 0
	 "ValveBiped.Bip01_L_Finger4" 0
	 "ValveBiped.Bip01_L_Finger41" 0
	 "ValveBiped.Bip01_L_Finger42" 0
	 "ValveBiped.Bip01_L_Finger3" 0
	 "ValveBiped.Bip01_L_Finger31" 0
	 "ValveBiped.Bip01_L_Finger32" 0
	 "ValveBiped.Bip01_L_Finger2" 0
	 "ValveBiped.Bip01_L_Finger21" 0
	 "ValveBiped.Bip01_L_Finger22" 0
	 "ValveBiped.Bip01_L_Finger1" 0
	 "ValveBiped.Bip01_L_Finger11" 0
	 "ValveBiped.Bip01_L_Finger12" 0
	 "ValveBiped.Bip01_L_Finger0" 0
	 "ValveBiped.Bip01_L_Finger01" 0
	 "ValveBiped.Bip01_L_Finger02" 0
	 "ValveBiped.Bip01_R_Finger4" 0
	 "ValveBiped.Bip01_R_Finger41" 0
	 "ValveBiped.Bip01_R_Finger42" 0
	 "ValveBiped.Bip01_R_Finger3" 0
	 "ValveBiped.Bip01_R_Finger31" 0
	 "ValveBiped.Bip01_R_Finger32" 0
	 "ValveBiped.Bip01_R_Finger2" 0
	 "ValveBiped.Bip01_R_Finger21" 0
	 "ValveBiped.Bip01_R_Finger22" 0
	 "ValveBiped.Bip01_R_Finger1" 0
	 "ValveBiped.Bip01_R_Finger11" 0
	 "ValveBiped.Bip01_R_Finger12" 0
	 "ValveBiped.Bip01_R_Finger0" 0
	 "ValveBiped.Bip01_R_Finger01" 0
	 "ValveBiped.Bip01_R_Finger02" 0
	 "ValveBiped.baton_parent" 0
}

$weightlist "weights_gesture_reload_pistolarms" {
	 "ValveBiped.Bip01_Pelvis" 0
	 "ValveBiped.Bip01_Spine" 0
	 "ValveBiped.Bip01_Spine1" 0
	 "ValveBiped.Bip01_Spine2" 0
	 "ValveBiped.Bip01_Spine4" 0
	 "ValveBiped.Bip01_Neck1" 0
	 "ValveBiped.Bip01_Head1" 0
	 "ValveBiped.forward" 0
	 "ValveBiped.Bip01_R_Clavicle" 1
	 "ValveBiped.Bip01_R_UpperArm" 1
	 "ValveBiped.Bip01_R_Forearm" 1
	 "ValveBiped.Bip01_R_Hand" 1
	 "ValveBiped.Anim_Attachment_RH" 1
	 "ValveBiped.Bip01_L_Clavicle" 1
	 "ValveBiped.Bip01_L_UpperArm" 1
	 "ValveBiped.Bip01_L_Forearm" 1
	 "ValveBiped.Bip01_L_Hand" 1
	 "ValveBiped.Anim_Attachment_LH" 1
	 "ValveBiped.Bip01_R_Thigh" 0
	 "ValveBiped.Bip01_R_Calf" 0
	 "ValveBiped.Bip01_R_Foot" 0
	 "ValveBiped.Bip01_R_Toe0" 0
	 "ValveBiped.Bip01_L_Thigh" 0
	 "ValveBiped.Bip01_L_Calf" 0
	 "ValveBiped.Bip01_L_Foot" 0
	 "ValveBiped.Bip01_L_Toe0" 0
	 "ValveBiped.Bip01_L_Finger4" 1
	 "ValveBiped.Bip01_L_Finger41" 1
	 "ValveBiped.Bip01_L_Finger42" 1
	 "ValveBiped.Bip01_L_Finger3" 1
	 "ValveBiped.Bip01_L_Finger31" 1
	 "ValveBiped.Bip01_L_Finger32" 1
	 "ValveBiped.Bip01_L_Finger2" 1
	 "ValveBiped.Bip01_L_Finger21" 1
	 "ValveBiped.Bip01_L_Finger22" 1
	 "ValveBiped.Bip01_L_Finger1" 1
	 "ValveBiped.Bip01_L_Finger11" 1
	 "ValveBiped.Bip01_L_Finger12" 1
	 "ValveBiped.Bip01_L_Finger0" 1
	 "ValveBiped.Bip01_L_Finger01" 1
	 "ValveBiped.Bip01_L_Finger02" 1
	 "ValveBiped.Bip01_R_Finger4" 1
	 "ValveBiped.Bip01_R_Finger41" 1
	 "ValveBiped.Bip01_R_Finger42" 1
	 "ValveBiped.Bip01_R_Finger3" 1
	 "ValveBiped.Bip01_R_Finger31" 1
	 "ValveBiped.Bip01_R_Finger32" 1
	 "ValveBiped.Bip01_R_Finger2" 1
	 "ValveBiped.Bip01_R_Finger21" 1
	 "ValveBiped.Bip01_R_Finger22" 1
	 "ValveBiped.Bip01_R_Finger1" 1
	 "ValveBiped.Bip01_R_Finger11" 1
	 "ValveBiped.Bip01_R_Finger12" 1
	 "ValveBiped.Bip01_R_Finger0" 1
	 "ValveBiped.Bip01_R_Finger01" 1
	 "ValveBiped.Bip01_R_Finger02" 1
	 "ValveBiped.baton_parent" 1
}

$weightlist "weights_gesture_reload_pistol" {
	 "ValveBiped.Bip01_Pelvis" 0
	 "ValveBiped.Bip01_Spine" 0
	 "ValveBiped.Bip01_Spine1" 0
	 "ValveBiped.Bip01_Spine2" 0
	 "ValveBiped.Bip01_Spine4" 0
	 "ValveBiped.Bip01_Neck1" 0
	 "ValveBiped.Bip01_Head1" 0
	 "ValveBiped.forward" 0
	 "ValveBiped.Bip01_R_Clavicle" 0
	 "ValveBiped.Bip01_R_UpperArm" 0
	 "ValveBiped.Bip01_R_Forearm" 0
	 "ValveBiped.Bip01_R_Hand" 0
	 "ValveBiped.Anim_Attachment_RH" 0
	 "ValveBiped.Bip01_L_Clavicle" 0
	 "ValveBiped.Bip01_L_UpperArm" 0
	 "ValveBiped.Bip01_L_Forearm" 0
	 "ValveBiped.Bip01_L_Hand" 0
	 "ValveBiped.Anim_Attachment_LH" 0
	 "ValveBiped.Bip01_R_Thigh" 0
	 "ValveBiped.Bip01_R_Calf" 0
	 "ValveBiped.Bip01_R_Foot" 0
	 "ValveBiped.Bip01_R_Toe0" 0
	 "ValveBiped.Bip01_L_Thigh" 0
	 "ValveBiped.Bip01_L_Calf" 0
	 "ValveBiped.Bip01_L_Foot" 0
	 "ValveBiped.Bip01_L_Toe0" 0
	 "ValveBiped.Bip01_L_Finger4" 0
	 "ValveBiped.Bip01_L_Finger41" 0
	 "ValveBiped.Bip01_L_Finger42" 0
	 "ValveBiped.Bip01_L_Finger3" 0
	 "ValveBiped.Bip01_L_Finger31" 0
	 "ValveBiped.Bip01_L_Finger32" 0
	 "ValveBiped.Bip01_L_Finger2" 0
	 "ValveBiped.Bip01_L_Finger21" 0
	 "ValveBiped.Bip01_L_Finger22" 0
	 "ValveBiped.Bip01_L_Finger1" 0
	 "ValveBiped.Bip01_L_Finger11" 0
	 "ValveBiped.Bip01_L_Finger12" 0
	 "ValveBiped.Bip01_L_Finger0" 0
	 "ValveBiped.Bip01_L_Finger01" 0
	 "ValveBiped.Bip01_L_Finger02" 0
	 "ValveBiped.Bip01_R_Finger4" 0
	 "ValveBiped.Bip01_R_Finger41" 0
	 "ValveBiped.Bip01_R_Finger42" 0
	 "ValveBiped.Bip01_R_Finger3" 0
	 "ValveBiped.Bip01_R_Finger31" 0
	 "ValveBiped.Bip01_R_Finger32" 0
	 "ValveBiped.Bip01_R_Finger2" 0
	 "ValveBiped.Bip01_R_Finger21" 0
	 "ValveBiped.Bip01_R_Finger22" 0
	 "ValveBiped.Bip01_R_Finger1" 0
	 "ValveBiped.Bip01_R_Finger11" 0
	 "ValveBiped.Bip01_R_Finger12" 0
	 "ValveBiped.Bip01_R_Finger0" 0
	 "ValveBiped.Bip01_R_Finger01" 0
	 "ValveBiped.Bip01_R_Finger02" 0
	 "ValveBiped.baton_parent" 0
}

$weightlist "weights_pistol_Aim_all" {
	 "ValveBiped.Bip01_Pelvis" 1
	 "ValveBiped.Bip01_Spine" 1
	 "ValveBiped.Bip01_Spine1" 1
	 "ValveBiped.Bip01_Spine2" 1
	 "ValveBiped.Bip01_Spine4" 1
	 "ValveBiped.Bip01_Neck1" 1
	 "ValveBiped.Bip01_Head1" 1
	 "ValveBiped.forward" 1
	 "ValveBiped.Bip01_R_Clavicle" 1
	 "ValveBiped.Bip01_R_UpperArm" 1
	 "ValveBiped.Bip01_R_Forearm" 1
	 "ValveBiped.Bip01_R_Hand" 1
	 "ValveBiped.Anim_Attachment_RH" 1
	 "ValveBiped.Bip01_L_Clavicle" 1
	 "ValveBiped.Bip01_L_UpperArm" 1
	 "ValveBiped.Bip01_L_Forearm" 1
	 "ValveBiped.Bip01_L_Hand" 1
	 "ValveBiped.Anim_Attachment_LH" 1
	 "ValveBiped.Bip01_R_Thigh" 0
	 "ValveBiped.Bip01_R_Calf" 0
	 "ValveBiped.Bip01_R_Foot" 0
	 "ValveBiped.Bip01_R_Toe0" 0
	 "ValveBiped.Bip01_L_Thigh" 0
	 "ValveBiped.Bip01_L_Calf" 0
	 "ValveBiped.Bip01_L_Foot" 0
	 "ValveBiped.Bip01_L_Toe0" 0
	 "ValveBiped.Bip01_L_Finger4" 1
	 "ValveBiped.Bip01_L_Finger41" 1
	 "ValveBiped.Bip01_L_Finger42" 1
	 "ValveBiped.Bip01_L_Finger3" 1
	 "ValveBiped.Bip01_L_Finger31" 1
	 "ValveBiped.Bip01_L_Finger32" 1
	 "ValveBiped.Bip01_L_Finger2" 1
	 "ValveBiped.Bip01_L_Finger21" 1
	 "ValveBiped.Bip01_L_Finger22" 1
	 "ValveBiped.Bip01_L_Finger1" 1
	 "ValveBiped.Bip01_L_Finger11" 1
	 "ValveBiped.Bip01_L_Finger12" 1
	 "ValveBiped.Bip01_L_Finger0" 1
	 "ValveBiped.Bip01_L_Finger01" 1
	 "ValveBiped.Bip01_L_Finger02" 1
	 "ValveBiped.Bip01_R_Finger4" 1
	 "ValveBiped.Bip01_R_Finger41" 1
	 "ValveBiped.Bip01_R_Finger42" 1
	 "ValveBiped.Bip01_R_Finger3" 1
	 "ValveBiped.Bip01_R_Finger31" 1
	 "ValveBiped.Bip01_R_Finger32" 1
	 "ValveBiped.Bip01_R_Finger2" 1
	 "ValveBiped.Bip01_R_Finger21" 1
	 "ValveBiped.Bip01_R_Finger22" 1
	 "ValveBiped.Bip01_R_Finger1" 1
	 "ValveBiped.Bip01_R_Finger11" 1
	 "ValveBiped.Bip01_R_Finger12" 1
	 "ValveBiped.Bip01_R_Finger0" 1
	 "ValveBiped.Bip01_R_Finger01" 1
	 "ValveBiped.Bip01_R_Finger02" 1
	 "ValveBiped.baton_parent" 1
}

$weightlist "weights_layer_walk_hold_baton_angry" {
	 "ValveBiped.Bip01_Pelvis" 0
	 "ValveBiped.Bip01_Spine" 1
	 "ValveBiped.Bip01_Spine1" 1
	 "ValveBiped.Bip01_Spine2" 1
	 "ValveBiped.Bip01_Spine4" 1
	 "ValveBiped.Bip01_Neck1" 1
	 "ValveBiped.Bip01_Head1" 1
	 "ValveBiped.forward" 1
	 "ValveBiped.Bip01_R_Clavicle" 1
	 "ValveBiped.Bip01_R_UpperArm" 1
	 "ValveBiped.Bip01_R_Forearm" 1
	 "ValveBiped.Bip01_R_Hand" 1
	 "ValveBiped.Anim_Attachment_RH" 1
	 "ValveBiped.Bip01_L_Clavicle" 1
	 "ValveBiped.Bip01_L_UpperArm" 1
	 "ValveBiped.Bip01_L_Forearm" 1
	 "ValveBiped.Bip01_L_Hand" 1
	 "ValveBiped.Anim_Attachment_LH" 1
	 "ValveBiped.Bip01_R_Thigh" 0
	 "ValveBiped.Bip01_R_Calf" 0
	 "ValveBiped.Bip01_R_Foot" 0
	 "ValveBiped.Bip01_R_Toe0" 0
	 "ValveBiped.Bip01_L_Thigh" 0
	 "ValveBiped.Bip01_L_Calf" 0
	 "ValveBiped.Bip01_L_Foot" 0
	 "ValveBiped.Bip01_L_Toe0" 0
	 "ValveBiped.Bip01_L_Finger4" 1
	 "ValveBiped.Bip01_L_Finger41" 1
	 "ValveBiped.Bip01_L_Finger42" 1
	 "ValveBiped.Bip01_L_Finger3" 1
	 "ValveBiped.Bip01_L_Finger31" 1
	 "ValveBiped.Bip01_L_Finger32" 1
	 "ValveBiped.Bip01_L_Finger2" 1
	 "ValveBiped.Bip01_L_Finger21" 1
	 "ValveBiped.Bip01_L_Finger22" 1
	 "ValveBiped.Bip01_L_Finger1" 1
	 "ValveBiped.Bip01_L_Finger11" 1
	 "ValveBiped.Bip01_L_Finger12" 1
	 "ValveBiped.Bip01_L_Finger0" 1
	 "ValveBiped.Bip01_L_Finger01" 1
	 "ValveBiped.Bip01_L_Finger02" 1
	 "ValveBiped.Bip01_R_Finger4" 1
	 "ValveBiped.Bip01_R_Finger41" 1
	 "ValveBiped.Bip01_R_Finger42" 1
	 "ValveBiped.Bip01_R_Finger3" 1
	 "ValveBiped.Bip01_R_Finger31" 1
	 "ValveBiped.Bip01_R_Finger32" 1
	 "ValveBiped.Bip01_R_Finger2" 1
	 "ValveBiped.Bip01_R_Finger21" 1
	 "ValveBiped.Bip01_R_Finger22" 1
	 "ValveBiped.Bip01_R_Finger1" 1
	 "ValveBiped.Bip01_R_Finger11" 1
	 "ValveBiped.Bip01_R_Finger12" 1
	 "ValveBiped.Bip01_R_Finger0" 1
	 "ValveBiped.Bip01_R_Finger01" 1
	 "ValveBiped.Bip01_R_Finger02" 1
	 "ValveBiped.baton_parent" 1
}

$weightlist "weights_Pistol_aim_walk_all_delta" {
	 "ValveBiped.Bip01_Pelvis" 1
	 "ValveBiped.Bip01_Spine" 0
	 "ValveBiped.Bip01_Spine1" 0
	 "ValveBiped.Bip01_Spine2" 0
	 "ValveBiped.Bip01_Spine4" 0
	 "ValveBiped.Bip01_Neck1" 0
	 "ValveBiped.Bip01_Head1" 0
	 "ValveBiped.forward" 0
	 "ValveBiped.Bip01_R_Clavicle" 0
	 "ValveBiped.Bip01_R_UpperArm" 0
	 "ValveBiped.Bip01_R_Forearm" 0
	 "ValveBiped.Bip01_R_Hand" 0
	 "ValveBiped.Anim_Attachment_RH" 0
	 "ValveBiped.Bip01_L_Clavicle" 0
	 "ValveBiped.Bip01_L_UpperArm" 0
	 "ValveBiped.Bip01_L_Forearm" 0
	 "ValveBiped.Bip01_L_Hand" 0
	 "ValveBiped.Anim_Attachment_LH" 0
	 "ValveBiped.Bip01_R_Thigh" 0
	 "ValveBiped.Bip01_R_Calf" 0
	 "ValveBiped.Bip01_R_Foot" 0
	 "ValveBiped.Bip01_R_Toe0" 0
	 "ValveBiped.Bip01_L_Thigh" 0
	 "ValveBiped.Bip01_L_Calf" 0
	 "ValveBiped.Bip01_L_Foot" 0
	 "ValveBiped.Bip01_L_Toe0" 0
	 "ValveBiped.Bip01_L_Finger4" 0
	 "ValveBiped.Bip01_L_Finger41" 0
	 "ValveBiped.Bip01_L_Finger42" 0
	 "ValveBiped.Bip01_L_Finger3" 0
	 "ValveBiped.Bip01_L_Finger31" 0
	 "ValveBiped.Bip01_L_Finger32" 0
	 "ValveBiped.Bip01_L_Finger2" 0
	 "ValveBiped.Bip01_L_Finger21" 0
	 "ValveBiped.Bip01_L_Finger22" 0
	 "ValveBiped.Bip01_L_Finger1" 0
	 "ValveBiped.Bip01_L_Finger11" 0
	 "ValveBiped.Bip01_L_Finger12" 0
	 "ValveBiped.Bip01_L_Finger0" 0
	 "ValveBiped.Bip01_L_Finger01" 0
	 "ValveBiped.Bip01_L_Finger02" 0
	 "ValveBiped.Bip01_R_Finger4" 0
	 "ValveBiped.Bip01_R_Finger41" 0
	 "ValveBiped.Bip01_R_Finger42" 0
	 "ValveBiped.Bip01_R_Finger3" 0
	 "ValveBiped.Bip01_R_Finger31" 0
	 "ValveBiped.Bip01_R_Finger32" 0
	 "ValveBiped.Bip01_R_Finger2" 0
	 "ValveBiped.Bip01_R_Finger21" 0
	 "ValveBiped.Bip01_R_Finger22" 0
	 "ValveBiped.Bip01_R_Finger1" 0
	 "ValveBiped.Bip01_R_Finger11" 0
	 "ValveBiped.Bip01_R_Finger12" 0
	 "ValveBiped.Bip01_R_Finger0" 0
	 "ValveBiped.Bip01_R_Finger01" 0
	 "ValveBiped.Bip01_R_Finger02" 0
	 "ValveBiped.baton_parent" 0
}

$animation "a_body_rot_z_lt_corrective_animation" "police_animations_anims\a_body_rot_z_lt_corrective_animation.smd" {
}

$animation "a_body_rot_corrective_animation" "police_animations_anims\a_body_rot_corrective_animation.smd" {
}

$animation "a_body_rot_z_rt_corrective_animation" "police_animations_anims\a_body_rot_z_rt_corrective_animation.smd" {
}

$animation "a_spine_rot_z_lt_corrective_animation" "police_animations_anims\a_spine_rot_z_lt_corrective_animation.smd" {
}

$animation "a_spine_rot_corrective_animation" "police_animations_anims\a_spine_rot_corrective_animation.smd" {
}

$animation "a_spine_rot_z_rt_corrective_animation" "police_animations_anims\a_spine_rot_z_rt_corrective_animation.smd" {
}

$animation "a_neck_trans_back_corrective_animation" "police_animations_anims\a_neck_trans_back_corrective_animation.smd" {
}

$animation "a_neck_trans_corrective_animation" "police_animations_anims\a_neck_trans_corrective_animation.smd" {
}

$animation "a_neck_trans_fwd_corrective_animation" "police_animations_anims\a_neck_trans_fwd_corrective_animation.smd" {
}

$animation "a_head_rot_z_lt_corrective_animation" "police_animations_anims\a_head_rot_z_lt_corrective_animation.smd" {
}

$animation "a_head_rot_corrective_animation" "police_animations_anims\a_head_rot_corrective_animation.smd" {
}

$animation "a_head_rot_z_rt_corrective_animation" "police_animations_anims\a_head_rot_z_rt_corrective_animation.smd" {
}

$animation "a_head_rot_dwn_corrective_animation" "police_animations_anims\a_head_rot_dwn_corrective_animation.smd" {
}

$animation "a_head_rot_up_corrective_animation" "police_animations_anims\a_head_rot_up_corrective_animation.smd" {
}

$animation "a_head_rot_lt_corrective_animation" "police_animations_anims\a_head_rot_lt_corrective_animation.smd" {
}

$animation "a_head_rot_rt_corrective_animation" "police_animations_anims\a_head_rot_rt_corrective_animation.smd" {
}

$animation "a_flinch_gesture_corrective_animation" "police_animations_anims\a_flinch_gesture_corrective_animation.smd" {
}

$animation "a_flinch_head_gesture1_corrective_animation" "police_animations_anims\a_flinch_head_gesture1_corrective_animation.smd" {
}

$animation "a_flinch_head_gesture2_corrective_animation" "police_animations_anims\a_flinch_head_gesture2_corrective_animation.smd" {
}

$animation "a_flinch_gut_gesture1_corrective_animation" "police_animations_anims\a_flinch_gut_gesture1_corrective_animation.smd" {
}

$animation "a_flinch_gut_gesture2_corrective_animation" "police_animations_anims\a_flinch_gut_gesture2_corrective_animation.smd" {
}

$animation "a_flinch_larm_gesture_corrective_animation" "police_animations_anims\a_flinch_larm_gesture_corrective_animation.smd" {
}

$animation "a_flinch_rarm_gesture_corrective_animation" "police_animations_anims\a_flinch_rarm_gesture_corrective_animation.smd" {
}

$animation "@gesture_shoot_pistol_corrective_animation" "police_animations_anims\@gesture_shoot_pistol_corrective_animation.smd" {
}

$animation "@gesture_reload_pistolspine_corrective_animation" "police_animations_anims\@gesture_reload_pistolspine_corrective_animation.smd" {
}

$animation "@swinggesturespine_corrective_animation" "police_animations_anims\@swinggesturespine_corrective_animation.smd" {
}

$animation "a_pistol_aim_straight_up_corrective_animation" "police_animations_anims\a_pistol_aim_straight_up_corrective_animation.smd" {
}

$animation "a_pistol_aim_up_right_corrective_animation" "police_animations_anims\a_pistol_aim_up_right_corrective_animation.smd" {
}

$animation "a_pistol_aim_up_center_corrective_animation" "police_animations_anims\a_pistol_aim_up_center_corrective_animation.smd" {
}

$animation "a_pistol_aim_up_left_corrective_animation" "police_animations_anims\a_pistol_aim_up_left_corrective_animation.smd" {
}

$animation "a_pistol_aim_mid_right_corrective_animation" "police_animations_anims\a_pistol_aim_mid_right_corrective_animation.smd" {
}

$animation "a_pistol_aim_mid_center_corrective_animation" "police_animations_anims\a_pistol_aim_mid_center_corrective_animation.smd" {
}

$animation "a_pistol_aim_mid_left_corrective_animation" "police_animations_anims\a_pistol_aim_mid_left_corrective_animation.smd" {
}

$animation "a_pistol_aim_down_right_corrective_animation" "police_animations_anims\a_pistol_aim_down_right_corrective_animation.smd" {
}

$animation "a_pistol_aim_down_center_corrective_animation" "police_animations_anims\a_pistol_aim_down_center_corrective_animation.smd" {
}

$animation "a_pistol_aim_down_left_corrective_animation" "police_animations_anims\a_pistol_aim_down_left_corrective_animation.smd" {
}

$animation "a_WalkN_pistol_aim_straight_up_corrective_animation" "police_animations_anims\a_WalkN_pistol_aim_straight_up_corrective_animation.smd" {
}

$animation "a_WalkN_pistol_aim_up_right_corrective_animation" "police_animations_anims\a_WalkN_pistol_aim_up_right_corrective_animation.smd" {
}

$animation "a_WalkN_pistol_aim_up_center_corrective_animation" "police_animations_anims\a_WalkN_pistol_aim_up_center_corrective_animation.smd" {
}

$animation "a_WalkN_pistol_aim_up_left_corrective_animation" "police_animations_anims\a_WalkN_pistol_aim_up_left_corrective_animation.smd" {
}

$animation "a_WalkN_pistol_aim_mid_right_corrective_animation" "police_animations_anims\a_WalkN_pistol_aim_mid_right_corrective_animation.smd" {
}

$animation "a_WalkN_pistol_aim_mid_center_corrective_animation" "police_animations_anims\a_WalkN_pistol_aim_mid_center_corrective_animation.smd" {
}

$animation "a_WalkN_pistol_aim_mid_left_corrective_animation" "police_animations_anims\a_WalkN_pistol_aim_mid_left_corrective_animation.smd" {
}

$animation "a_WalkN_pistol_aim_down_right_corrective_animation" "police_animations_anims\a_WalkN_pistol_aim_down_right_corrective_animation.smd" {
}

$animation "a_WalkN_pistol_aim_down_center_corrective_animation" "police_animations_anims\a_WalkN_pistol_aim_down_center_corrective_animation.smd" {
}

$animation "a_WalkN_pistol_aim_down_left_corrective_animation" "police_animations_anims\a_WalkN_pistol_aim_down_left_corrective_animation.smd" {
}

$animation "a_walkS_pistol_aim_delta_corrective_animation" "police_animations_anims\a_walkS_pistol_aim_delta_corrective_animation.smd" {
}

$animation "a_walkSE_pistol_aim_delta_corrective_animation" "police_animations_anims\a_walkSE_pistol_aim_delta_corrective_animation.smd" {
}

$animation "a_walkE_pistol_aim_delta_corrective_animation" "police_animations_anims\a_walkE_pistol_aim_delta_corrective_animation.smd" {
}

$animation "a_walkNE_pistol_aim_delta_corrective_animation" "police_animations_anims\a_walkNE_pistol_aim_delta_corrective_animation.smd" {
}

$animation "a_walkN_pistol_aim_delta_corrective_animation" "police_animations_anims\a_walkN_pistol_aim_delta_corrective_animation.smd" {
}

$animation "a_walkNW_pistol_aim_delta_corrective_animation" "police_animations_anims\a_walkNW_pistol_aim_delta_corrective_animation.smd" {
}

$animation "a_walkW_pistol_aim_delta_corrective_animation" "police_animations_anims\a_walkW_pistol_aim_delta_corrective_animation.smd" {
}

$animation "a_walkSW_pistol_aim_delta_corrective_animation" "police_animations_anims\a_walkSW_pistol_aim_delta_corrective_animation.smd" {
}

$animation "a_runS_pistol_aim_delta_corrective_animation" "police_animations_anims\a_runS_pistol_aim_delta_corrective_animation.smd" {
}

$animation "a_runSE_pistol_aim_delta_corrective_animation" "police_animations_anims\a_runSE_pistol_aim_delta_corrective_animation.smd" {
}

$animation "a_runE_pistol_aim_delta_corrective_animation" "police_animations_anims\a_runE_pistol_aim_delta_corrective_animation.smd" {
}

$animation "a_runNE_pistol_aim_delta_corrective_animation" "police_animations_anims\a_runNE_pistol_aim_delta_corrective_animation.smd" {
}

$animation "a_runN_pistol_aim_delta_corrective_animation" "police_animations_anims\a_runN_pistol_aim_delta_corrective_animation.smd" {
}

$animation "a_runNW_pistol_aim_delta_corrective_animation" "police_animations_anims\a_runNW_pistol_aim_delta_corrective_animation.smd" {
}

$animation "a_runW_pistol_aim_delta_corrective_animation" "police_animations_anims\a_runW_pistol_aim_delta_corrective_animation.smd" {
}

$animation "a_runSW_pistol_aim_delta_corrective_animation" "police_animations_anims\a_runSW_pistol_aim_delta_corrective_animation.smd" {
}

$animation "@gesture_turn_left_45default_corrective_animation" "police_animations_anims\@gesture_turn_left_45default_corrective_animation.smd" {
}

$animation "@gesture_turn_left_45inDelta_corrective_animation" "police_animations_anims\@gesture_turn_left_45inDelta_corrective_animation.smd" {
}

$animation "@gesture_turn_left_45outDelta_corrective_animation" "police_animations_anims\@gesture_turn_left_45outDelta_corrective_animation.smd" {
}

$animation "@gesture_turn_left_90default_corrective_animation" "police_animations_anims\@gesture_turn_left_90default_corrective_animation.smd" {
}

$animation "@gesture_turn_left_90inDelta_corrective_animation" "police_animations_anims\@gesture_turn_left_90inDelta_corrective_animation.smd" {
}

$animation "@gesture_turn_left_90outDelta_corrective_animation" "police_animations_anims\@gesture_turn_left_90outDelta_corrective_animation.smd" {
}

$animation "@gesture_turn_right_45default_corrective_animation" "police_animations_anims\@gesture_turn_right_45default_corrective_animation.smd" {
}

$animation "@gesture_turn_right_45inDelta_corrective_animation" "police_animations_anims\@gesture_turn_right_45inDelta_corrective_animation.smd" {
}

$animation "@gesture_turn_right_45outDelta_corrective_animation" "police_animations_anims\@gesture_turn_right_45outDelta_corrective_animation.smd" {
}

$animation "@gesture_turn_right_90default_corrective_animation" "police_animations_anims\@gesture_turn_right_90default_corrective_animation.smd" {
}

$animation "@gesture_turn_right_90inDelta_corrective_animation" "police_animations_anims\@gesture_turn_right_90inDelta_corrective_animation.smd" {
}

$animation "@gesture_turn_right_90outDelta_corrective_animation" "police_animations_anims\@gesture_turn_right_90outDelta_corrective_animation.smd" {
}

$animation "Man_Gun_up_right_corrective_animation" "police_animations_anims\Man_Gun_up_right_corrective_animation.smd" {
}

$animation "Man_Gun_up_center_corrective_animation" "police_animations_anims\Man_Gun_up_center_corrective_animation.smd" {
}

$animation "Man_Gun_up_left_corrective_animation" "police_animations_anims\Man_Gun_up_left_corrective_animation.smd" {
}

$animation "Man_Gun_mid_right_corrective_animation" "police_animations_anims\Man_Gun_mid_right_corrective_animation.smd" {
}

$animation "Man_Gun_mid_center_corrective_animation" "police_animations_anims\Man_Gun_mid_center_corrective_animation.smd" {
}

$animation "Man_Gun_mid_left_corrective_animation" "police_animations_anims\Man_Gun_mid_left_corrective_animation.smd" {
}

$animation "Man_Gun_down_right_corrective_animation" "police_animations_anims\Man_Gun_down_right_corrective_animation.smd" {
}

$animation "Man_Gun_down_center_corrective_animation" "police_animations_anims\Man_Gun_down_center_corrective_animation.smd" {
}

$animation "Man_Gun_down_left_corrective_animation" "police_animations_anims\Man_Gun_down_left_corrective_animation.smd" {
}

$animation "a_pose_neutral" "police_animations_anims\a_pose_neutral.smd" {
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 3
}

$animation "a_body_rot" "police_animations_anims\a_body_rot.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_body_rot_corrective_animation" 0
	weightlist "weights_body_rot_z"
}

$animation "a_body_rot_z_rt" "police_animations_anims\a_body_rot_z_rt.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_body_rot_z_rt_corrective_animation" 0
	weightlist "weights_body_rot_z"
}

$animation "a_body_rot_z_lt" "police_animations_anims\a_body_rot_z_lt.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_body_rot_z_lt_corrective_animation" 0
	weightlist "weights_body_rot_z"
}

$animation "a_spine_rot" "police_animations_anims\a_spine_rot.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_spine_rot_corrective_animation" 0
	weightlist "weights_body_rot_z"
}

$animation "a_spine_rot_z_rt" "police_animations_anims\a_spine_rot_z_rt.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_spine_rot_z_rt_corrective_animation" 0
	weightlist "weights_body_rot_z"
}

$animation "a_spine_rot_z_lt" "police_animations_anims\a_spine_rot_z_lt.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_spine_rot_z_lt_corrective_animation" 0
	weightlist "weights_body_rot_z"
}

$animation "a_head_rot" "police_animations_anims\a_head_rot.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_head_rot_corrective_animation" 0
	weightlist "weights_neck_trans_x"
}

$animation "a_head_rot_lt" "police_animations_anims\a_head_rot_lt.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_head_rot_lt_corrective_animation" 0
	weightlist "weights_neck_trans_x"
}

$animation "a_head_rot_rt" "police_animations_anims\a_head_rot_rt.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_head_rot_rt_corrective_animation" 0
	weightlist "weights_neck_trans_x"
}

$animation "a_neck_trans" "police_animations_anims\a_neck_trans.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_neck_trans_corrective_animation" 0
	weightlist "weights_neck_trans_x"
}

$animation "a_neck_trans_fwd" "police_animations_anims\a_neck_trans_fwd.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_neck_trans_fwd_corrective_animation" 0
	weightlist "weights_neck_trans_x"
}

$animation "a_neck_trans_back" "police_animations_anims\a_neck_trans_back.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_neck_trans_back_corrective_animation" 0
	weightlist "weights_neck_trans_x"
}

$animation "a_head_rot_dwn" "police_animations_anims\a_head_rot_dwn.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_head_rot_dwn_corrective_animation" 0
	weightlist "weights_neck_trans_x"
}

$animation "a_head_rot_up" "police_animations_anims\a_head_rot_up.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_head_rot_up_corrective_animation" 0
	weightlist "weights_neck_trans_x"
}

$animation "a_head_rot_z_rt" "police_animations_anims\a_head_rot_z_rt.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_head_rot_z_rt_corrective_animation" 0
	weightlist "weights_neck_trans_x"
}

$animation "a_head_rot_z_lt" "police_animations_anims\a_head_rot_z_lt.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_head_rot_z_lt_corrective_animation" 0
	weightlist "weights_neck_trans_x"
}

$animation "a_flinch_gesture" "police_animations_anims\a_flinch_gesture.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_flinch_gesture_corrective_animation" 0
}

$animation "a_flinch_head_gesture1" "police_animations_anims\a_flinch_head_gesture1.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_flinch_head_gesture1_corrective_animation" 0
}

$animation "a_flinch_head_gesture2" "police_animations_anims\a_flinch_head_gesture2.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_flinch_head_gesture2_corrective_animation" 0
}

$animation "a_flinch_gut_gesture1" "police_animations_anims\a_flinch_gut_gesture1.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_flinch_gut_gesture1_corrective_animation" 0
}

$animation "a_flinch_gut_gesture2" "police_animations_anims\a_flinch_gut_gesture2.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_flinch_gut_gesture2_corrective_animation" 0
}

$animation "a_flinch_larm_gesture" "police_animations_anims\a_flinch_larm_gesture.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_flinch_larm_gesture_corrective_animation" 0
}

$animation "a_flinch_rarm_gesture" "police_animations_anims\a_flinch_rarm_gesture.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_flinch_rarm_gesture_corrective_animation" 0
}

$animation "a_aim_pistol_neutral" "police_animations_anims\a_aim_pistol_neutral.smd" {
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
}

$animation "a_pistol_aim_down_left" "police_animations_anims\a_pistol_aim_down_left.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_pistol_aim_down_left_corrective_animation" 0
	ikrule "lhand" touch "ValveBiped.Bip01_R_Hand" contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	weightlist "weights_pistol_Aim_all"
}

$animation "a_pistol_aim_down_center" "police_animations_anims\a_pistol_aim_down_center.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_pistol_aim_down_center_corrective_animation" 0
	ikrule "lhand" touch "ValveBiped.Bip01_R_Hand" contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	weightlist "weights_pistol_Aim_all"
}

$animation "a_pistol_aim_down_right" "police_animations_anims\a_pistol_aim_down_right.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_pistol_aim_down_right_corrective_animation" 0
	ikrule "lhand" touch "ValveBiped.Bip01_R_Hand" contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	weightlist "weights_pistol_Aim_all"
}

$animation "a_pistol_aim_mid_left" "police_animations_anims\a_pistol_aim_mid_left.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_pistol_aim_mid_left_corrective_animation" 0
	ikrule "lhand" touch "ValveBiped.Bip01_R_Hand" contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	weightlist "weights_pistol_Aim_all"
}

$animation "a_pistol_aim_mid_center" "police_animations_anims\a_pistol_aim_mid_center.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_pistol_aim_mid_center_corrective_animation" 0
	ikrule "lhand" touch "ValveBiped.Bip01_R_Hand" contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	weightlist "weights_pistol_Aim_all"
}

$animation "a_pistol_aim_mid_right" "police_animations_anims\a_pistol_aim_mid_right.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_pistol_aim_mid_right_corrective_animation" 0
	ikrule "lhand" touch "ValveBiped.Bip01_R_Hand" contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	weightlist "weights_pistol_Aim_all"
}

$animation "a_pistol_aim_up_left" "police_animations_anims\a_pistol_aim_up_left.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_pistol_aim_up_left_corrective_animation" 0
	ikrule "lhand" touch "ValveBiped.Bip01_R_Hand" contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	weightlist "weights_pistol_Aim_all"
}

$animation "a_pistol_aim_up_center" "police_animations_anims\a_pistol_aim_up_center.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_pistol_aim_up_center_corrective_animation" 0
	ikrule "lhand" touch "ValveBiped.Bip01_R_Hand" contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	weightlist "weights_pistol_Aim_all"
}

$animation "a_pistol_aim_up_right" "police_animations_anims\a_pistol_aim_up_right.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_pistol_aim_up_right_corrective_animation" 0
	ikrule "lhand" touch "ValveBiped.Bip01_R_Hand" contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	weightlist "weights_pistol_Aim_all"
}

$animation "a_pistol_aim_straight_up" "police_animations_anims\a_pistol_aim_straight_up.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_pistol_aim_straight_up_corrective_animation" 0
	ikrule "lhand" touch "ValveBiped.Bip01_R_Hand" contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	weightlist "weights_pistol_Aim_all"
}

$animation "a_walkn_aim_pistol_neutral" "police_animations_anims\a_walkn_aim_pistol_neutral.smd" {
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
}

$animation "a_WalkN_pistol_aim_down_left" "police_animations_anims\a_WalkN_pistol_aim_down_left.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_WalkN_pistol_aim_down_left_corrective_animation" 0
	ikrule "lhand" touch "ValveBiped.Bip01_R_Hand" contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	weightlist "weights_pistol_Aim_all"
}

$animation "a_WalkN_pistol_aim_down_center" "police_animations_anims\a_WalkN_pistol_aim_down_center.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_WalkN_pistol_aim_down_center_corrective_animation" 0
	ikrule "lhand" touch "ValveBiped.Bip01_R_Hand" contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	weightlist "weights_pistol_Aim_all"
}

$animation "a_WalkN_pistol_aim_down_right" "police_animations_anims\a_WalkN_pistol_aim_down_right.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_WalkN_pistol_aim_down_right_corrective_animation" 0
	ikrule "lhand" touch "ValveBiped.Bip01_R_Hand" contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	weightlist "weights_pistol_Aim_all"
}

$animation "a_WalkN_pistol_aim_mid_left" "police_animations_anims\a_WalkN_pistol_aim_mid_left.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_WalkN_pistol_aim_mid_left_corrective_animation" 0
	ikrule "lhand" touch "ValveBiped.Bip01_R_Hand" contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	weightlist "weights_pistol_Aim_all"
}

$animation "a_WalkN_pistol_aim_mid_center" "police_animations_anims\a_WalkN_pistol_aim_mid_center.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_WalkN_pistol_aim_mid_center_corrective_animation" 0
	ikrule "lhand" touch "ValveBiped.Bip01_R_Hand" contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	weightlist "weights_pistol_Aim_all"
}

$animation "a_WalkN_pistol_aim_mid_right" "police_animations_anims\a_WalkN_pistol_aim_mid_right.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_WalkN_pistol_aim_mid_right_corrective_animation" 0
	ikrule "lhand" touch "ValveBiped.Bip01_R_Hand" contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	weightlist "weights_pistol_Aim_all"
}

$animation "a_WalkN_pistol_aim_up_left" "police_animations_anims\a_WalkN_pistol_aim_up_left.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_WalkN_pistol_aim_up_left_corrective_animation" 0
	ikrule "lhand" touch "ValveBiped.Bip01_R_Hand" contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	weightlist "weights_pistol_Aim_all"
}

$animation "a_WalkN_pistol_aim_up_center" "police_animations_anims\a_WalkN_pistol_aim_up_center.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_WalkN_pistol_aim_up_center_corrective_animation" 0
	ikrule "lhand" touch "ValveBiped.Bip01_R_Hand" contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	weightlist "weights_pistol_Aim_all"
}

$animation "a_WalkN_pistol_aim_up_right" "police_animations_anims\a_WalkN_pistol_aim_up_right.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_WalkN_pistol_aim_up_right_corrective_animation" 0
	ikrule "lhand" touch "ValveBiped.Bip01_R_Hand" contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	weightlist "weights_pistol_Aim_all"
}

$animation "a_WalkN_pistol_aim_straight_up" "police_animations_anims\a_WalkN_pistol_aim_straight_up.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_WalkN_pistol_aim_straight_up_corrective_animation" 0
	ikrule "lhand" touch "ValveBiped.Bip01_R_Hand" contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	weightlist "weights_pistol_Aim_all"
}

$animation "a_Crouch_walkS" "police_animations_anims\a_Crouch_walkS.smd" {
	fps 30
	loop
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 3
	walkframe 20 LX LY
}

$animation "a_Crouch_walkSE" "police_animations_anims\a_Crouch_walkSE.smd" {
	fps 30
	loop
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 24 24 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 24 24 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 24 24 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 24 24 target 3
	walkframe 24 LX LY
}

$animation "a_Crouch_walkE" "police_animations_anims\a_Crouch_walkE.smd" {
	fps 30
	loop
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 21 21 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 21 21 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 21 21 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 21 21 target 3
	walkframe 21 LX LY
}

$animation "a_Crouch_walkNE" "police_animations_anims\a_Crouch_walkNE.smd" {
	fps 30
	loop
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 21 21 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 21 21 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 21 21 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 21 21 target 3
	walkframe 21 LX LY
}

$animation "a_Crouch_walkN" "police_animations_anims\a_Crouch_walkN.smd" {
	fps 30
	loop
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 24 24 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 24 24 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 24 24 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 24 24 target 3
	walkframe 24 LX LY
}

$animation "a_Crouch_walkNW" "police_animations_anims\a_Crouch_walkNW.smd" {
	fps 30
	loop
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 21 21 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 21 21 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 21 21 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 21 21 target 3
	walkframe 21 LX LY
}

$animation "a_Crouch_walkW" "police_animations_anims\a_Crouch_walkW.smd" {
	fps 30
	loop
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 3
	walkframe 20 LX LY
}

$animation "a_Crouch_walkSW" "police_animations_anims\a_Crouch_walkSW.smd" {
	fps 30
	loop
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 21 21 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 21 21 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 21 21 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 21 21 target 3
	walkframe 21 LX LY
}

$animation "a_WalkS" "police_animations_anims\a_WalkS.smd" {
	fps 31
	loop
	ikrule "lfoot" footstep contact 7 fakeorigin -8.31 8.22 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 29 7 21 29 target 3
	ikrule "rfoot" footstep contact 22 fakeorigin -11.48 -3.43 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 14 22 6 14 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 1
	walkframe 30 LX LY
}

$animation "a_WalkSE" "police_animations_anims\a_WalkSE.smd" {
	fps 29
	loop
	ikrule "lfoot" footstep contact 7 fakeorigin -4.37 8.31 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 29 7 21 29 target 3
	ikrule "rfoot" footstep contact 22 fakeorigin -6.05 -7.44 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 14 22 6 14 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 1
	walkframe 30 LX LY
}

$animation "a_WalkE" "police_animations_anims\a_WalkE.smd" {
	fps 33
	loop
	ikrule "lfoot" footstep contact 7 fakeorigin 8.4 -0.33 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 29 7 21 29 target 3
	ikrule "rfoot" footstep contact 22 fakeorigin -3.88 -10.25 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 14 22 6 14 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 1
	walkframe 30 LX LY
}

$animation "a_WalkNE" "police_animations_anims\a_WalkNE.smd" {
	fps 32
	loop
	ikrule "lfoot" footstep contact 7 fakeorigin 8.82 3.92 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 29 7 21 29 target 3
	ikrule "rfoot" footstep contact 22 fakeorigin 3.71 -11.55 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 14 22 6 14 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 1
	walkframe 30 LX LY
}

$animation "a_WalkN" "police_animations_anims\a_WalkN.smd" {
	fps 30
	loop
	ikrule "lfoot" footstep contact 7 fakeorigin 13.65 6.55 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 29 7 21 29 target 3
	ikrule "rfoot" footstep contact 22 fakeorigin 13.7 -1.18 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 14 22 6 14 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 1
	walkframe 30 LX LY
}

$animation "a_WalkNW" "police_animations_anims\a_WalkNW.smd" {
	fps 32
	loop
	ikrule "lfoot" footstep contact 7 fakeorigin 8.2 9.8 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 29 7 21 29 target 3
	ikrule "rfoot" footstep contact 22 fakeorigin 6.42 8.17 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 14 22 6 14 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 1
	walkframe 30 LX LY
}

$animation "a_WalkW" "police_animations_anims\a_WalkW.smd" {
	fps 33
	loop
	ikrule "lfoot" footstep contact 7 fakeorigin 5.76 10.9 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 29 7 21 29 target 3
	ikrule "rfoot" footstep contact 22 fakeorigin -1.32 8.4 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 14 22 6 14 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 1
	walkframe 30 LX LY
}

$animation "a_WalkSW" "police_animations_anims\a_WalkSW.smd" {
	fps 29
	loop
	ikrule "lfoot" footstep contact 7 fakeorigin 0.4 8.73 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 29 7 21 29 target 3
	ikrule "rfoot" footstep contact 22 fakeorigin -9.1 5.01 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 14 22 6 14 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 1
	walkframe 30 LX LY
}

$animation "a_walkN_hold_baton_angry" "police_animations_anims\a_walkN_hold_baton_angry.smd" {
	fps 30
	loop
	ikrule "lfoot" footstep contact 7 fakeorigin 13.65 6.55 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 29 7 21 29 target 3
	ikrule "rfoot" footstep contact 22 fakeorigin 13.7 -1.18 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 14 22 6 14 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 1
	walkframe 30 LX LY
}

$animation "a_walkS_pistol_aim_delta" "police_animations_anims\a_walkS_pistol_aim_delta.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_walkS_pistol_aim_delta_corrective_animation" 0
	weightlist "weights_Pistol_aim_walk_all_delta"
}

$animation "a_walkSE_pistol_aim_delta" "police_animations_anims\a_walkSE_pistol_aim_delta.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_walkSE_pistol_aim_delta_corrective_animation" 0
	weightlist "weights_Pistol_aim_walk_all_delta"
}

$animation "a_walkE_pistol_aim_delta" "police_animations_anims\a_walkE_pistol_aim_delta.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_walkE_pistol_aim_delta_corrective_animation" 0
	weightlist "weights_Pistol_aim_walk_all_delta"
}

$animation "a_walkNE_pistol_aim_delta" "police_animations_anims\a_walkNE_pistol_aim_delta.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_walkNE_pistol_aim_delta_corrective_animation" 0
	weightlist "weights_Pistol_aim_walk_all_delta"
}

$animation "a_walkN_pistol_aim_delta" "police_animations_anims\a_walkN_pistol_aim_delta.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_walkN_pistol_aim_delta_corrective_animation" 0
	weightlist "weights_Pistol_aim_walk_all_delta"
}

$animation "a_walkNW_pistol_aim_delta" "police_animations_anims\a_walkNW_pistol_aim_delta.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_walkNW_pistol_aim_delta_corrective_animation" 0
	weightlist "weights_Pistol_aim_walk_all_delta"
}

$animation "a_walkW_pistol_aim_delta" "police_animations_anims\a_walkW_pistol_aim_delta.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_walkW_pistol_aim_delta_corrective_animation" 0
	weightlist "weights_Pistol_aim_walk_all_delta"
}

$animation "a_walkSW_pistol_aim_delta" "police_animations_anims\a_walkSW_pistol_aim_delta.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_walkSW_pistol_aim_delta_corrective_animation" 0
	weightlist "weights_Pistol_aim_walk_all_delta"
}

$animation "a_WalkN_aim_pistol" "police_animations_anims\a_WalkN_aim_pistol.smd" {
	fps 30
	loop
	ikrule "lfoot" footstep contact 7 fakeorigin 13.65 6.55 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 29 7 21 29 target 3
	ikrule "rfoot" footstep contact 22 fakeorigin 13.7 -1.18 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 14 22 6 14 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 1
	walkframe 30 LX LY
}

$animation "a_walkN_hold_pistol" "police_animations_anims\a_walkN_hold_pistol.smd" {
	fps 30
	loop
	ikrule "lfoot" footstep contact 7 fakeorigin 13.65 6.55 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 29 7 21 29 target 3
	ikrule "rfoot" footstep contact 22 fakeorigin 13.7 -1.18 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 14 22 6 14 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 1
	walkframe 30 LX LY
}

$animation "a_RunS" "police_animations_anims\a_RunS.smd" {
	fps 28
	loop
	ikrule "lfoot" footstep contact 9 fakeorigin -1.03 6.18 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 18 7 10 18 target 3
	ikrule "rfoot" footstep contact 19 fakeorigin -3 -1.16 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 8 16 1 8 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 1
	walkframe 20 LX LY
}

$animation "a_RunSE" "police_animations_anims\a_RunSE.smd" {
	fps 33
	loop
	ikrule "lfoot" footstep contact 9 fakeorigin 2.17 8.46 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 18 7 10 18 target 3
	ikrule "rfoot" footstep contact 19 fakeorigin 2.16 1.19 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 8 16 1 8 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 1
	walkframe 20 LX LY
}

$animation "a_RunE" "police_animations_anims\a_RunE.smd" {
	fps 29
	loop
	ikrule "lfoot" footstep contact 10 fakeorigin 5.83 6.44 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 18 7 10 18 target 3
	ikrule "rfoot" footstep contact 19 fakeorigin -3.99 -3.14 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 8 16 1 8 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 1
	walkframe 20 LX LY
}

$animation "a_RunNE" "police_animations_anims\a_RunNE.smd" {
	fps 35
	loop
	ikrule "lfoot" footstep contact 9 fakeorigin 5.3 2.41 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 18 7 10 18 target 3
	ikrule "rfoot" footstep contact 19 fakeorigin 4.2 -7.29 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 8 16 1 8 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 1
	walkframe 20 LX LY
}

$animation "a_RunN" "police_animations_anims\a_RunN.smd" {
	fps 30
	loop
	ikrule "lfoot" footstep contact 10 fakeorigin -1.82 6.13 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 18 7 10 18 target 3
	ikrule "rfoot" footstep contact 19 fakeorigin 5.1 -1.25 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 8 16 1 8 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 1
	walkframe 20 LX LY
}

$animation "a_RunNW" "police_animations_anims\a_RunNW.smd" {
	fps 35
	loop
	ikrule "lfoot" footstep contact 8 fakeorigin 2.32 13.27 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 18 7 10 18 target 3
	ikrule "rfoot" footstep contact 20 fakeorigin -0.93 2.33 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 8 16 1 8 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 1
	walkframe 20 LX LY
}

$animation "a_RunW" "police_animations_anims\a_RunW.smd" {
	fps 29
	loop
	ikrule "lfoot" footstep contact 10 fakeorigin 5.89 4.81 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 18 7 10 18 target 3
	ikrule "rfoot" footstep contact 1 fakeorigin -3.72 -4.06 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 10 16 4 10 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 1
	walkframe 20 LX LY
}

$animation "a_RunSW" "police_animations_anims\a_RunSW.smd" {
	fps 33
	loop
	ikrule "lfoot" footstep contact 9 fakeorigin 5.37 6.98 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 18 7 10 18 target 3
	ikrule "rfoot" footstep contact 20 fakeorigin -1.64 -2.46 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 8 16 1 8 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 1
	walkframe 20 LX LY
}

$animation "a_runS_pistol_aim_delta" "police_animations_anims\a_runS_pistol_aim_delta.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_runS_pistol_aim_delta_corrective_animation" 0
	weightlist "weights_Pistol_aim_walk_all_delta"
}

$animation "a_runSE_pistol_aim_delta" "police_animations_anims\a_runSE_pistol_aim_delta.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_runSE_pistol_aim_delta_corrective_animation" 0
	weightlist "weights_Pistol_aim_walk_all_delta"
}

$animation "a_runE_pistol_aim_delta" "police_animations_anims\a_runE_pistol_aim_delta.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_runE_pistol_aim_delta_corrective_animation" 0
	weightlist "weights_Pistol_aim_walk_all_delta"
}

$animation "a_runNE_pistol_aim_delta" "police_animations_anims\a_runNE_pistol_aim_delta.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_runNE_pistol_aim_delta_corrective_animation" 0
	weightlist "weights_Pistol_aim_walk_all_delta"
}

$animation "a_runN_pistol_aim_delta" "police_animations_anims\a_runN_pistol_aim_delta.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_runN_pistol_aim_delta_corrective_animation" 0
	weightlist "weights_Pistol_aim_walk_all_delta"
}

$animation "a_runNW_pistol_aim_delta" "police_animations_anims\a_runNW_pistol_aim_delta.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_runNW_pistol_aim_delta_corrective_animation" 0
	weightlist "weights_Pistol_aim_walk_all_delta"
}

$animation "a_runW_pistol_aim_delta" "police_animations_anims\a_runW_pistol_aim_delta.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_runW_pistol_aim_delta_corrective_animation" 0
	weightlist "weights_Pistol_aim_walk_all_delta"
}

$animation "a_runSW_pistol_aim_delta" "police_animations_anims\a_runSW_pistol_aim_delta.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "a_runSW_pistol_aim_delta_corrective_animation" 0
	weightlist "weights_Pistol_aim_walk_all_delta"
}

$animation "a_runN_hold_pistol" "police_animations_anims\a_runN_hold_pistol.smd" {
	fps 30
	loop
	ikrule "lfoot" footstep contact 8 fakeorigin 10.25 6.08 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 18 7 10 18 target 3
	ikrule "rfoot" footstep contact 18 fakeorigin 11.04 -1.4 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 8 16 1 8 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 1
	walkframe 20 LX LY
}

$animation "a_runN_aim_pistol" "police_animations_anims\a_runN_aim_pistol.smd" {
	fps 30
	loop
	ikrule "lfoot" footstep contact 8 fakeorigin 10.25 6.08 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 18 7 10 18 target 3
	ikrule "rfoot" footstep contact 18 fakeorigin 11.04 -1.4 0 fakerotate 0 0 0 floor 0 height 18 radius 2.5 range 8 16 1 8 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 1
	walkframe 20 LX LY
}

$animation "a_jump_holding_hit" "police_animations_anims\a_jump_holding_hit.smd" {
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 3
}

$animation "Man_Gun_neutral" "police_animations_anims\Man_Gun_neutral.smd" {
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 0 0 target 3
}

$animation "Man_Gun_down_left" "police_animations_anims\Man_Gun_down_left.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "Man_Gun_down_left_corrective_animation" 0
}

$animation "Man_Gun_down_center" "police_animations_anims\Man_Gun_down_center.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "Man_Gun_down_center_corrective_animation" 0
}

$animation "Man_Gun_down_right" "police_animations_anims\Man_Gun_down_right.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "Man_Gun_down_right_corrective_animation" 0
}

$animation "Man_Gun_mid_left" "police_animations_anims\Man_Gun_mid_left.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "Man_Gun_mid_left_corrective_animation" 0
}

$animation "Man_Gun_mid_center" "police_animations_anims\Man_Gun_mid_center.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "Man_Gun_mid_center_corrective_animation" 0
}

$animation "Man_Gun_mid_right" "police_animations_anims\Man_Gun_mid_right.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "Man_Gun_mid_right_corrective_animation" 0
}

$animation "Man_Gun_up_left" "police_animations_anims\Man_Gun_up_left.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "Man_Gun_up_left_corrective_animation" 0
}

$animation "Man_Gun_up_center" "police_animations_anims\Man_Gun_up_center.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "Man_Gun_up_center_corrective_animation" 0
}

$animation "Man_Gun_up_right" "police_animations_anims\Man_Gun_up_right.smd" {
	fps 30
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "Man_Gun_up_right_corrective_animation" 0
}

$sequence "body_rot_z" {
	"a_body_rot_z_lt"
	"a_body_rot"
	"a_body_rot_z_rt"
	autoplay
	blend "body_yaw" 29.73423 -29.73426
	blendwidth 3
	delta
	fadein 0.2
	fadeout 0.2
	hidden
}

$sequence "spine_rot_z" {
	"a_spine_rot_z_lt"
	"a_spine_rot"
	"a_spine_rot_z_rt"
	autoplay
	blend "spine_yaw" 30.70848 -30.7085
	blendwidth 3
	delta
	fadein 0.2
	fadeout 0.2
	hidden
}

$sequence "neck_trans_x" {
	"a_neck_trans_back"
	"a_neck_trans"
	"a_neck_trans_fwd"
	autoplay
	blend "neck_trans" -0.207508 0.2594
	blendwidth 3
	delta
	fadein 0.2
	fadeout 0.2
	hidden
}

$sequence "head_rot_z" {
	"a_head_rot_z_lt"
	"a_head_rot"
	"a_head_rot_z_rt"
	autoplay
	blend "head_yaw" 66.70631 -66.70654
	blendwidth 3
	delta
	fadein 0.2
	fadeout 0.2
	hidden
}

$sequence "head_rot_y" {
	"a_head_rot_dwn"
	"a_head_rot"
	"a_head_rot_up"
	autoplay
	blend "head_pitch" 25.37666 -34.86658
	blendwidth 3
	delta
	fadein 0.2
	fadeout 0.2
	hidden
}

$sequence "head_rot_x" {
	"a_head_rot_lt"
	"a_head_rot"
	"a_head_rot_rt"
	autoplay
	blend "head_roll" -10.93143 10.85243
	blendwidth 3
	delta
	fadein 0.2
	fadeout 0.2
	hidden
}

$sequence "batonidle1" {
	"police_animations_anims\batonidle1.smd"
	activity "ACT_IDLE" 2
	fadein 0.2
	fadeout 0.2
	node "standing_baton"
	fps 30
	loop
	ikrule "lfoot" footstep contact 0 fakeorigin -1.69 7.7 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 300 300 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -1.92 -6.52 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 300 300 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 300 300 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 300 300 target 1
}

$sequence "batonidle2" {
	"police_animations_anims\batonidle2.smd"
	activity "ACT_IDLE" 1
	fadein 0.2
	fadeout 0.2
	node "standing_baton"
	fps 30
	loop
	ikrule "lfoot" footstep contact 0 fakeorigin -1.69 7.71 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 200 200 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -1.92 -6.53 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 200 200 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 200 200 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 200 200 target 1
}

$sequence "pistolidle1" {
	"police_animations_anims\pistolidle1.smd"
	activity "ACT_IDLE_PISTOL" 2
	fadein 0.2
	fadeout 0.2
	node "standing_pistol"
	fps 30
	loop
	ikrule "lfoot" footstep contact 0 fakeorigin -1.69 7.7 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 300 300 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -1.92 -6.52 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 300 300 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 300 300 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 300 300 target 1
}

$sequence "pistolidle2" {
	"police_animations_anims\pistolidle2.smd"
	activity "ACT_IDLE_PISTOL" 1
	fadein 0.2
	fadeout 0.2
	node "standing_pistol"
	fps 30
	loop
	ikrule "lfoot" footstep contact 0 fakeorigin -1.69 7.71 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 200 200 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -1.92 -6.53 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 200 200 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 200 200 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 200 200 target 1
}

$sequence "Idle_Baton" {
	"police_animations_anims\Idle_Baton.smd"
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
	ikrule "lfoot" footstep contact 0 fakeorigin -7.21 5.8 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 100 100 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -1.26 -3.48 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 100 100 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 100 100 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 100 100 target 1
}

$sequence "BlockEntry" {
	"police_animations_anims\BlockEntry.smd"
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin -7.21 5.8 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 120 120 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -1.26 -3.48 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 120 120 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 120 120 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 120 120 target 1
}

$sequence "buttonright" {
	"police_animations_anims\buttonright.smd"
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin -1.69 7.71 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 65 65 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin 2.29 -3.96 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 65 65 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 65 65 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 65 65 target 1
}

$sequence "buttonfront" {
	"police_animations_anims\buttonfront.smd"
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin -1.69 7.71 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 65 65 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin 2.29 -3.96 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 65 65 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 65 65 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 65 65 target 1
}

$sequence "buttonleft" {
	"police_animations_anims\buttonleft.smd"
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin -1.69 7.71 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 65 65 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin 2.29 -3.96 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 65 65 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 65 65 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 65 65 target 1
}

$sequence "busyidle1" {
	"police_animations_anims\busyidle1.smd"
	activity "ACT_BUSY_LEAN_BACK" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
	ikrule "lfoot" footstep contact 0 fakeorigin -7.21 5.8 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 100 100 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -1.26 -3.48 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 100 100 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 100 100 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 100 100 target 1
}

$sequence "busyidle2" {
	"police_animations_anims\busyidle2.smd"
	activity "ACT_BUSY_STAND" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
	ikrule "lfoot" footstep contact 0 fakeorigin -1.69 7.71 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 160 160 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -1.92 -6.53 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 160 160 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 160 160 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 160 160 target 1
}

$sequence "harassfront1" {
	"police_animations_anims\harassfront1.smd"
	activity "ACT_POLICE_HARASS1" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin -1.69 7.71 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 60 60 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -1.92 -6.53 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 60 60 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 60 60 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 60 60 target 1
}

$sequence "harassfront2" {
	"police_animations_anims\harassfront2.smd"
	activity "ACT_POLICE_HARASS2" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin -1.69 7.71 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 49 49 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -1.92 -6.53 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 49 49 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 49 49 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 49 49 target 1
}

$sequence "pickup" {
	"police_animations_anims\pickup.smd"
	activity "ACT_PICKUP_GROUND" 1
	{ event 2040 13 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin -1.69 7.7 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 40 40 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -1.92 -6.52 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 40 40 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 40 40 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 40 40 target 1
}

$sequence "point" {
	"police_animations_anims\point.smd"
	activity "ACT_METROPOLICE_POINT" 1
	fadein 0.2
	fadeout 0.2
	node "standing_pistol"
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin 12.34 5.65 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 28 28 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -8.81 -4.8 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 28 28 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 28 28 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 28 28 target 1
}

$sequence "idleonfire" {
	"police_animations_anims\idleonfire.smd"
	activity "ACT_IDLE_ON_FIRE" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 122 122 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 122 122 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 122 122 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 122 122 target 3
}

$sequence "moveonfire" {
	"police_animations_anims\moveonfire.smd"
	activity "ACT_RUN_ON_FIRE" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 66 66 target 0 usesequence 
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 66 66 target 1 usesequence 
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 66 66 target 2 usesequence 
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 66 66 target 3 usesequence 
	walkframe 66 LX LY
}

$sequence "rappelloop" {
	"police_animations_anims\rappelloop.smd"
	activity "ACT_RAPPEL_LOOP" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 50 50 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 50 50 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 50 50 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 50 50 target 3
}

$sequence "rappelland" {
	"police_animations_anims\rappelland.smd"
	activity "ACT_RAPPEL_LAND" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 50 50 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 50 50 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 50 50 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 50 50 target 3
}

$sequence "dummy1" {
	"police_animations_anims\batonidle1.smd"
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
	loop
	ikrule "lfoot" footstep contact 0 fakeorigin -1.69 7.7 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 300 300 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -1.92 -6.52 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 300 300 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 300 300 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 300 300 target 1
}

$sequence "dummy2" {
	"police_animations_anims\batonidle1.smd"
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
	loop
	ikrule "lfoot" footstep contact 0 fakeorigin -1.69 7.7 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 300 300 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -1.92 -6.52 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 300 300 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 300 300 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 300 300 target 1
}

$sequence "pushplayer" {
	"police_animations_anims\pushplayer.smd"
	activity "ACT_PUSH_PLAYER" 1
	{ event AE_METROPOLICE_SHOVE 11 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin 3.96 10.2 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 30 30 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -4.64 -6.44 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 30 30 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 1
}

$sequence "activatebaton" {
	"police_animations_anims\activatebaton.smd"
	activity "ACT_ACTIVATE_BATON" 1
	{ event AE_METROPOLICE_BATON_ON 7 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin 3.84 10.83 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 16 16 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -4.76 -5.81 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 16 16 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 16 16 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 16 16 target 1
}

$sequence "deactivatebaton" {
	"police_animations_anims\deactivatebaton.smd"
	activity "ACT_DEACTIVATE_BATON" 1
	{ event AE_METROPOLICE_BATON_OFF 7 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin 3.84 10.83 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 16 16 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -4.76 -5.81 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 16 16 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 16 16 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 16 16 target 1
}

$sequence "cower" {
	"police_animations_anims\cower.smd"
	activity "ACT_COWER" 1
	fadein 0.2
	fadeout 0.2
	fps 35
	ikrule "lfoot" footstep contact 0 fakeorigin 11.96 7.22 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 40 40 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -9.2 -3.23 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 40 40 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 40 40 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 40 40 target 1
}

$sequence "flinch_head1" {
	"police_animations_anims\flinch_head1.smd"
	activity "ACT_FLINCH_HEAD" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 15 15 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 15 15 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 15 15 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 15 15 target 3
}

$sequence "flinch_head2" {
	"police_animations_anims\flinch_head2.smd"
	activity "ACT_FLINCH_HEAD" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 15 15 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 15 15 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 15 15 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 15 15 target 3
}

$sequence "flinch_stomach1" {
	"police_animations_anims\flinch_stomach1.smd"
	activity "ACT_FLINCH_STOMACH" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 18 18 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 18 18 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 18 18 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 18 18 target 3
}

$sequence "flinch_stomach2" {
	"police_animations_anims\flinch_stomach2.smd"
	activity "ACT_FLINCH_STOMACH" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 18 18 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 18 18 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 18 18 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 18 18 target 3
}

$sequence "flinch_leftarm1" {
	"police_animations_anims\flinch_leftarm1.smd"
	activity "ACT_FLINCH_LEFTARM" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 18 18 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 18 18 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 18 18 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 18 18 target 3
}

$sequence "flinch_rightarm1" {
	"police_animations_anims\flinch_rightarm1.smd"
	activity "ACT_FLINCH_RIGHTARM" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 18 18 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 18 18 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 18 18 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 18 18 target 3
}

$sequence "flinch_back1" {
	"police_animations_anims\flinch_back1.smd"
	activity "ACT_METROPOLICE_FLINCH_BEHIND" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 15 15 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 15 15 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 15 15 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 15 15 target 3
}

$sequence "flinch1" {
	"police_animations_anims\flinch1.smd"
	activity "ACT_SMALL_FLINCH" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 8 8 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 8 8 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 8 8 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 8 8 target 3
}

$sequence "flinch2" {
	"police_animations_anims\flinch2.smd"
	activity "ACT_BIG_FLINCH" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 10 10 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 10 10 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 10 10 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 10 10 target 3
}

$sequence "flinch_gesture" {
	"a_flinch_gesture"
	activity "ACT_GESTURE_SMALL_FLINCH" 1
	delta
	fadein 0.2
	fadeout 0.2
}

$sequence "flinchheadgest1" {
	"a_flinch_head_gesture1"
	activity "ACT_GESTURE_FLINCH_HEAD" 1
	delta
	fadein 0.2
	fadeout 0.2
}

$sequence "flinchheadgest2" {
	"a_flinch_head_gesture2"
	activity "ACT_GESTURE_FLINCH_HEAD" 1
	delta
	fadein 0.2
	fadeout 0.2
}

$sequence "flinchgutgest1" {
	"a_flinch_gut_gesture1"
	activity "ACT_GESTURE_FLINCH_STOMACH" 1
	delta
	fadein 0.2
	fadeout 0.2
}

$sequence "flinchgutgest2" {
	"a_flinch_gut_gesture2"
	activity "ACT_GESTURE_FLINCH_STOMACH" 1
	delta
	fadein 0.2
	fadeout 0.2
}

$sequence "flinchlarmgest" {
	"a_flinch_larm_gesture"
	activity "ACT_GESTURE_FLINCH_LEFTARM" 1
	delta
	fadein 0.2
	fadeout 0.2
}

$sequence "flinchrarmgest" {
	"a_flinch_rarm_gesture"
	activity "ACT_GESTURE_FLINCH_RIGHTARM" 1
	delta
	fadein 0.2
	fadeout 0.2
}

$sequence "physflinch1" {
	"police_animations_anims\physflinch1.smd"
	activity "ACT_FLINCH_PHYSICS" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 28 28 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 28 28 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 28 28 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 28 28 target 3
}

$sequence "physflinch2" {
	"police_animations_anims\physflinch2.smd"
	activity "ACT_FLINCH_PHYSICS" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 28 28 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 28 28 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 28 28 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 28 28 target 3
}

$sequence "deathpose_front" {
	"police_animations_anims\deathpose_front.smd"
	activity "ACT_DIE_FRONTSIDE" 1
	fadein 0.2
	fadeout 0.2
	fps 1
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 6 6 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 6 6 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 6 6 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 6 6 target 3
}

$sequence "deathpose_back" {
	"police_animations_anims\deathpose_back.smd"
	activity "ACT_DIE_BACKSIDE" 1
	fadein 0.2
	fadeout 0.2
	fps 1
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 6 6 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 6 6 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 6 6 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 6 6 target 3
}

$sequence "deathpose_right" {
	"police_animations_anims\deathpose_right.smd"
	activity "ACT_DIE_RIGHTSIDE" 1
	fadein 0.2
	fadeout 0.2
	fps 1
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 6 6 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 6 6 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 6 6 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 6 6 target 3
}

$sequence "deathpose_left" {
	"police_animations_anims\deathpose_left.smd"
	activity "ACT_DIE_LEFTSIDE" 1
	fadein 0.2
	fadeout 0.2
	fps 1
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 6 6 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 6 6 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 6 6 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 6 6 target 3
}

$sequence "deploy" {
	"police_animations_anims\deploy.smd"
	activity "ACT_METROPOLICE_DEPLOY_MANHACK" 1
	{ event AE_METROPOLICE_START_DEPLOY 10 "" }
	{ event AE_METROPOLICE_DEPLOY_MANHACK 33 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin 12.34 5.65 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 50 50 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -8.81 -4.8 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 50 50 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 50 50 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 50 50 target 1
}

$sequence "grenadethrow" {
	"police_animations_anims\grenadethrow.smd"
	activity "ACT_COMBINE_THROW_GRENADE" 1
	{ event 7 23 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin 12.34 5.65 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 39 39 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -8.81 -4.8 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 39 39 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 39 39 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 39 39 target 1
}

$sequence "pistolangryidle2" {
	"police_animations_anims\pistolangryidle2.smd"
	activity "ACT_IDLE_ANGRY_PISTOL" 1
	fadein 0.2
	fadeout 0.2
	addlayer "pistol_Aim_all"
	node "shooting_pistol"
	fps 4
	ikrule "lfoot" footstep contact 0 fakeorigin 11.96 7.22 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 3 3 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -9.2 -3.23 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 3 3 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 3 3 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 3 3 target 1
}

$sequence "Crouch_idle_pistol" {
	"police_animations_anims\Crouch_idle_pistol.smd"
	activity "ACT_COVER_PISTOL_LOW" 1
	fadein 0.2
	fadeout 0.2
	node "crouching_pistol"
	fps 30
	loop
	ikrule "lfoot" footstep contact 0 fakeorigin 5.77 8.81 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 80 80 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -11.36 9.68 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 80 80 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 80 80 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 80 80 target 1
}

$sequence "drawpistol" {
	"police_animations_anims\drawpistol.smd"
	activity "ACT_METROPOLICE_DRAW_PISTOL" 1
	{ event AE_METROPOLICE_DRAW_PISTOL 11 "" }
	fadein 0.2
	fadeout 0.2
	blendlayer "pistol_Aim_all" 24 36 36 36 spline
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin 12.34 5.65 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 35 35 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -8.81 -4.8 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 35 35 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 35 35 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 35 35 target 1
}

$sequence "shootp1" {
	"police_animations_anims\shootp1.smd"
	activity "ACT_RANGE_ATTACK_PISTOL" 1
	fadein 0.2
	fadeout 0.2
	addlayer "pistol_Aim_all"
	node "shooting_pistol"
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin 11.95 7.22 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 4 4 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -9.2 -3.23 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 4 4 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 4 4 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 4 4 target 1
}

$sequence "gesture_shoot_pistol" {
	"police_animations_anims\gesture_shoot_pistol.smd"
	activity "ACT_GESTURE_RANGE_ATTACK_PISTOL" 1
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@gesture_shoot_pistol_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	fps 30
}

$sequence "reload_pistol" {
	"police_animations_anims\reload_pistol.smd"
	activity "ACT_RELOAD_PISTOL" 1
	{ event 3017 0 "" }
	{ event 3018 34 "" }
	fadein 0.2
	fadeout 0.2
	blendlayer "pistol_Aim_all" 0 0 0 15 spline
	blendlayer "pistol_Aim_all" 30 41 41 41 spline
	node "shooting_pistol"
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin 11.95 7.22 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 40 40 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -9.2 -3.23 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 40 40 target 2
	ikrule "lhand" touch "ValveBiped.Bip01_R_Hand" contact 29 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 26 29 30 31 target 1
	ikrule "lhand" touch "ValveBiped.Bip01_Spine" contact 13 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 10 13 16 18 target 1
	ikrule "lhand" touch "ValveBiped.Bip01_R_Hand" contact 40 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 38 40 10 10 target 1 usesequence 
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 40 40 target 0
}

$sequence "crouch_reload_pistol" {
	"police_animations_anims\crouch_reload_pistol.smd"
	activity "ACT_RELOAD_PISTOL_LOW" 1
	{ event 3017 0 "" }
	{ event 3018 56 "" }
	fadein 0.2
	fadeout 0.2
	node "crouching_pistol"
	fps 30
	ikrule "lfoot" footstep contact 0 fakeorigin 5.77 8.81 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 60 60 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -11.36 9.68 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 60 60 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 60 60 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 60 60 target 1
}

$sequence "gesture_reload_pistolspine" {
	"police_animations_anims\gesture_reload_pistolspine.smd"
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@gesture_reload_pistolspine_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
	weightlist "weights_neck_trans_x"
}

$sequence "gesture_reload_pistolarms" {
	"police_animations_anims\gesture_reload_pistolarms.smd"
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 40 40 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 40 40 target 1
	weightlist "weights_gesture_reload_pistolarms"
}

$sequence "gesture_reload_pistol" {
	"police_animations_anims\gesture_reload_pistol.smd"
	activity "ACT_GESTURE_RELOAD_PISTOL" 1
	{ event 3017 0 "" }
	{ event 3018 37 "" }
	fadein 0.2
	fadeout 0.2
	iklock "rfoot" 1 0
	iklock "lfoot" 1 0
	blendlayer "gesture_reload_pistolarms" 0 5 32 40 spline
	blendlayer "gesture_reload_pistolspine" 0 5 32 40 spline
	node "standing_pistol"
	fps 30
	ikrule "lhand" touch "ValveBiped.Bip01_Pelvis" contact 8 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 1 8 16 20 target 1 usesequence 
	ikrule "lhand" touch "ValveBiped.Bip01_R_Hand" contact 22 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 18 22 38 39 target 1 usesequence 
	weightlist "weights_gesture_reload_pistol"
}

$sequence "lowcover_shoot_pistol" {
	"police_animations_anims\lowcover_shoot_pistol.smd"
	activity "ACT_RANGE_ATTACK_PISTOL_LOW" 1
	fadein 0.2
	fadeout 0.2
	addlayer "pistol_Aim_all"
	node "lowcover_pistol"
	fps 10
	loop
	ikrule "lfoot" footstep contact 0 fakeorigin 12.5 5.65 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 3 3 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -8.65 -4.8 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 3 3 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 3 3 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 3 3 target 1
}

$sequence "lowcover_aim_pistol" {
	"police_animations_anims\lowcover_aim_pistol.smd"
	activity "ACT_RANGE_AIM_PISTOL_LOW" 1
	fadein 0.2
	fadeout 0.2
	addlayer "pistol_Aim_all"
	node "lowcover_pistol"
	fps 30
	loop
	ikrule "lfoot" footstep contact 0 fakeorigin 12.34 5.65 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 50 50 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -8.81 -4.8 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 50 50 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 50 50 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 50 50 target 1
}

$sequence "batonangryidle1" {
	"police_animations_anims\batonangryidle1.smd"
	activity "ACT_IDLE_ANGRY_MELEE" 1
	fadein 0.2
	fadeout 0.2
	node "standing_baton"
	fps 30
	loop
	ikrule "lfoot" footstep contact 0 fakeorigin 3.84 10.83 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 90 90 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -4.76 -5.81 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 90 90 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 90 90 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 90 90 target 1
}

$sequence "swing" {
	"police_animations_anims\swing.smd"
	activity "ACT_MELEE_ATTACK_SWING" 1
	{ event 3001 13 "" }
	fadein 0.2
	fadeout 0.2
	fps 35
	ikrule "lfoot" footstep contact 0 fakeorigin 3.96 10.2 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 28 28 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -4.64 -6.44 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 28 28 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 28 28 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 28 28 target 1
}

$sequence "thrust" {
	"police_animations_anims\thrust.smd"
	activity "ACT_MELEE_ATTACK_THRUST" 1
	{ event 3001 13 "" }
	fadein 0.2
	fadeout 0.2
	fps 35
	ikrule "lfoot" footstep contact 0 fakeorigin 3.96 10.2 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 35 35 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -4.64 -6.44 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 35 35 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 35 35 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 35 35 target 1
}

$sequence "swinggesturespine" {
	"police_animations_anims\swinggesturespine.smd"
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@swinggesturespine_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
	weightlist "weights_neck_trans_x"
}

$sequence "swinggesturearms" {
	"police_animations_anims\swinggesturearms.smd"
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 1
	weightlist "weights_gesture_reload_pistolarms"
}

$sequence "swinggesture" {
	"police_animations_anims\swinggesture.smd"
	activity "ACT_MELEE_ATTACK_SWING_GESTURE" 1
	{ event 3001 13 "" }
	fadein 0.2
	fadeout 0.2
	iklock "rfoot" 1 0
	iklock "lfoot" 1 0
	blendlayer "swinggesturearms" 0 7 19 30 spline
	blendlayer "swinggesturespine" 0 7 19 30 spline
	fps 35
	weightlist "weights_gesture_reload_pistol"
}

$sequence "pistol_Aim_all" {
	"a_pistol_aim_straight_up"
	"a_pistol_aim_straight_up"
	"a_pistol_aim_straight_up"
	"a_pistol_aim_up_right"
	"a_pistol_aim_up_center"
	"a_pistol_aim_up_left"
	"a_pistol_aim_mid_right"
	"a_pistol_aim_mid_center"
	"a_pistol_aim_mid_left"
	"a_pistol_aim_down_right"
	"a_pistol_aim_down_center"
	"a_pistol_aim_down_left"
	blend "aim_yaw" -43.47391 45.51665
	blend "aim_pitch" -82.85746 45.59517
	blendwidth 3
	delta
	fadein 0.2
	fadeout 0.2
	hidden
	iklock "rfoot" 1 0
	iklock "lfoot" 1 0
}

$sequence "WalkN_pistol_Aim_all" {
	"a_WalkN_pistol_aim_straight_up"
	"a_WalkN_pistol_aim_straight_up"
	"a_WalkN_pistol_aim_straight_up"
	"a_WalkN_pistol_aim_up_right"
	"a_WalkN_pistol_aim_up_center"
	"a_WalkN_pistol_aim_up_left"
	"a_WalkN_pistol_aim_mid_right"
	"a_WalkN_pistol_aim_mid_center"
	"a_WalkN_pistol_aim_mid_left"
	"a_WalkN_pistol_aim_down_right"
	"a_WalkN_pistol_aim_down_center"
	"a_WalkN_pistol_aim_down_left"
	blend "aim_yaw" -45.59397 44.46127
	blend "aim_pitch" -78.71045 43.57745
	blendwidth 3
	delta
	fadein 0.2
	fadeout 0.2
	hidden
	iklock "rfoot" 1 0
	iklock "lfoot" 1 0
}

$sequence "layer_walk_hold_baton_angry" {
	"police_animations_anims\layer_walk_hold_baton_angry.smd"
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
	loop
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 1
	weightlist "weights_layer_walk_hold_baton_angry"
}

$sequence "walk_hold_baton_angry" {
	"a_WalkS"
	"a_WalkSE"
	"a_WalkE"
	"a_WalkNE"
	"a_walkN_hold_baton_angry"
	"a_WalkNW"
	"a_WalkW"
	"a_WalkSW"
	"a_WalkS"
	activity "ACT_WALK_ANGRY" 1
	blend "move_yaw" -180 180
	blendwidth 9
	{ event 6004 9 "NPC_MetroPolice" }
	{ event 6005 25 "NPC_MetroPolice" }
	fadein 0.2
	fadeout 0.2
	addlayer "layer_walk_hold_baton_angry"
}

$sequence "Pistol_aim_walk_all_delta" {
	"a_walkS_pistol_aim_delta"
	"a_walkSE_pistol_aim_delta"
	"a_walkE_pistol_aim_delta"
	"a_walkNE_pistol_aim_delta"
	"a_walkN_pistol_aim_delta"
	"a_walkNW_pistol_aim_delta"
	"a_walkW_pistol_aim_delta"
	"a_walkSW_pistol_aim_delta"
	"a_walkS_pistol_aim_delta"
	blend "move_yaw" -180 180
	blendwidth 9
	delta
	fadein 0.2
	fadeout 0.2
	hidden
	iklock "lfoot" 1 1
	iklock "rfoot" 1 1
}

$sequence "layer_walk_aiming_pistol" {
	"police_animations_anims\layer_walk_aiming_pistol.smd"
	fadein 0.2
	fadeout 0.2
	hidden
	addlayer "WalkN_pistol_Aim_all"
	fps 30
	loop
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 1
	weightlist "weights_layer_walk_hold_baton_angry"
}

$sequence "walk_aiming_pistol_all" {
	"a_WalkS"
	"a_WalkSE"
	"a_WalkE"
	"a_WalkNE"
	"a_WalkN_aim_pistol"
	"a_WalkNW"
	"a_WalkW"
	"a_WalkSW"
	"a_WalkS"
	activity "ACT_WALK_AIM_PISTOL" 1
	blend "move_yaw" -180 180
	blendwidth 9
	{ event 6004 9 "NPC_MetroPolice" }
	{ event 6005 25 "NPC_MetroPolice" }
	fadein 0.2
	fadeout 0.2
	addlayer "layer_walk_aiming_pistol"
}

$sequence "layer_walk_hold_pistol" {
	"police_animations_anims\layer_walk_hold_pistol.smd"
	fadein 0.2
	fadeout 0.2
	hidden
	addlayer "WalkN_pistol_Aim_all"
	fps 30
	loop
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 30 30 target 1
	weightlist "weights_layer_walk_hold_baton_angry"
}

$sequence "walk_hold_pistol" {
	"a_WalkS"
	"a_WalkSE"
	"a_WalkE"
	"a_WalkNE"
	"a_walkN_hold_pistol"
	"a_WalkNW"
	"a_WalkW"
	"a_WalkSW"
	"a_WalkS"
	activity "ACT_WALK_PISTOL" 1
	blend "move_yaw" -180 180
	blendwidth 9
	{ event 6004 9 "NPC_MetroPolice" }
	{ event 6005 25 "NPC_MetroPolice" }
	fadein 0.2
	fadeout 0.2
	addlayer "layer_walk_hold_pistol"
	node "standing_pistol"
}

$sequence "Pistol_aim_run_all_delta" {
	"a_runS_pistol_aim_delta"
	"a_runSE_pistol_aim_delta"
	"a_runE_pistol_aim_delta"
	"a_runNE_pistol_aim_delta"
	"a_runN_pistol_aim_delta"
	"a_runNW_pistol_aim_delta"
	"a_runW_pistol_aim_delta"
	"a_runSW_pistol_aim_delta"
	"a_runS_pistol_aim_delta"
	blend "move_yaw" -180 180
	blendwidth 9
	delta
	fadein 0.2
	fadeout 0.2
	hidden
	iklock "lfoot" 1 1
	iklock "rfoot" 1 1
}

$sequence "layer_run_hold_pistol" {
	"police_animations_anims\layer_run_hold_pistol.smd"
	fadein 0.2
	fadeout 0.2
	hidden
	addlayer "WalkN_pistol_Aim_all"
	fps 30
	loop
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 1
	weightlist "weights_layer_walk_hold_baton_angry"
}

$sequence "run_hold_pistol" {
	"a_RunS"
	"a_RunSE"
	"a_RunE"
	"a_RunNE"
	"a_runN_hold_pistol"
	"a_RunNW"
	"a_RunW"
	"a_RunSW"
	"a_RunS"
	activity "ACT_RUN_PISTOL" 1
	blend "move_yaw" -180 180
	blendwidth 9
	{ event 6004 7 "NPC_MetroPolice" }
	{ event 6005 16 "NPC_MetroPolice" }
	fadein 0.2
	fadeout 0.2
	addlayer "layer_run_hold_pistol"
	node "standing_pistol"
}

$sequence "layer_run_aiming_pistol" {
	"police_animations_anims\layer_run_aiming_pistol.smd"
	fadein 0.2
	fadeout 0.2
	hidden
	addlayer "WalkN_pistol_Aim_all"
	fps 30
	loop
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 1
	weightlist "weights_layer_walk_hold_baton_angry"
}

$sequence "run_aiming_pistol_all" {
	"a_RunS"
	"a_RunSE"
	"a_RunE"
	"a_RunNE"
	"a_runN_aim_pistol"
	"a_RunNW"
	"a_RunW"
	"a_RunSW"
	"a_RunS"
	activity "ACT_RUN_AIM_PISTOL" 1
	blend "move_yaw" -180 180
	blendwidth 9
	{ event 6004 7 "NPC_MetroPolice" }
	{ event 6005 16 "NPC_MetroPolice" }
	fadein 0.2
	fadeout 0.2
	addlayer "layer_run_aiming_pistol"
}

$sequence "Stand_to_crouchpistol" {
	"police_animations_anims\Stand_to_crouchpistol.smd"
	activity "ACT_TRANSITION" 1
	fadein 0.2
	fadeout 0.2
	transition "standing_pistol" "crouching_pistol"
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 17 17 target 0 usesequence 
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 17 17 target 1 usesequence 
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 17 17 target 2 usesequence 
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 17 17 target 3 usesequence 
	walkframe 17 LX LY
}

$sequence "Crouch_to_standpistol" {
	"police_animations_anims\Crouch_to_standpistol.smd"
	activity "ACT_TRANSITION" 1
	fadein 0.2
	fadeout 0.2
	transition "crouching_pistol" "standing_pistol"
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 0 usesequence 
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 1 usesequence 
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 2 usesequence 
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 20 20 target 3 usesequence 
	walkframe 20 LX LY
}

$sequence "Shoot_to_crouchpistol" {
	"police_animations_anims\Shoot_to_crouchpistol.smd"
	activity "ACT_TRANSITION" 1
	fadein 0.2
	fadeout 0.2
	transition "shooting_pistol" "crouching_pistol"
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 17 17 target 0 usesequence 
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 17 17 target 1 usesequence 
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 17 17 target 2 usesequence 
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 17 17 target 3 usesequence 
	walkframe 17 LX LY
}

$sequence "Crouch_to_shootpistol" {
	"police_animations_anims\Crouch_to_shootpistol.smd"
	activity "ACT_TRANSITION" 1
	fadein 0.2
	fadeout 0.2
	blendlayer "pistolangryidle2" 14 20 26 26
	transition "crouching_pistol" "shooting_pistol"
	fps 40
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 25 25 target 0 usesequence 
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 25 25 target 1 usesequence 
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 25 25 target 2 usesequence 
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 25 25 target 3 usesequence 
	walkframe 20 LX LY
}

$sequence "shoottostandpistol" {
	"police_animations_anims\shoottostandpistol.smd"
	activity "ACT_TRANSITION" 1
	fadein 0.2
	fadeout 0.2
	blendlayer "pistol_Aim_all" 0 0 0 10 spline
	transition "shooting_pistol" "standing_pistol"
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 16 16 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 16 16 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 16 16 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 16 16 target 3
}

$sequence "standtoshootpistol" {
	"police_animations_anims\standtoshootpistol.smd"
	activity "ACT_TRANSITION" 1
	fadein 0.2
	fadeout 0.2
	blendlayer "pistol_Aim_all" 0 5 18 18 spline
	transition "standing_pistol" "shooting_pistol"
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 17 17 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 17 17 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 17 17 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 17 17 target 3
}

$sequence "crouch_to_lowcoverpistol" {
	"police_animations_anims\crouch_to_lowcoverpistol.smd"
	activity "ACT_TRANSITION" 1
	fadein 0.2
	fadeout 0.2
	blendlayer "pistol_Aim_all" 4 14 23 23 spline
	transition "crouching_pistol" "lowcover_pistol"
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 22 22 target 0 usesequence 
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 22 22 target 1 usesequence 
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 22 22 target 2 usesequence 
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 22 22 target 3 usesequence 
	walkframe 20 LX LY
}

$sequence "lowcover_to_crouchpistol" {
	"police_animations_anims\lowcover_to_crouchpistol.smd"
	activity "ACT_TRANSITION" 1
	fadein 0.2
	fadeout 0.2
	blendlayer "pistol_Aim_all" 0 0 0 10 spline
	transition "lowcover_pistol" "crouching_pistol"
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 16 16 target 0 usesequence 
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 16 16 target 1 usesequence 
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 16 16 target 2 usesequence 
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 16 16 target 3 usesequence 
	walkframe 16 LX LY
}

$sequence "turnleft" {
	"police_animations_anims\turnleft.smd"
	activity "ACT_GESTURE_TURN_LEFT" 1
	fadein 0.2
	fadeout 0.2
	fps 20
	ikrule "lfoot" unlatch contact 4 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 4 4 8 target 3
	ikrule "rfoot" unlatch contact 7 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 3 7 7 10 target 2
	weightlist "weights_gesture_reload_pistol"
}

$sequence "turnright" {
	"police_animations_anims\turnright.smd"
	activity "ACT_GESTURE_TURN_RIGHT" 1
	fadein 0.2
	fadeout 0.2
	fps 20
	ikrule "lfoot" unlatch contact 7 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 3 7 7 10 target 3
	ikrule "rfoot" unlatch contact 4 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 4 4 8 target 2
	weightlist "weights_gesture_reload_pistol"
}

$sequence "gesture_turn_left_45default" {
	"police_animations_anims\gesture_turn_left_45default.smd"
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@gesture_turn_left_45default_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
}

$sequence "gesture_turn_left_45inDelta" {
	"police_animations_anims\gesture_turn_left_45inDelta.smd"
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@gesture_turn_left_45inDelta_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
}

$sequence "gesture_turn_left_45outDelta" {
	"police_animations_anims\gesture_turn_left_45outDelta.smd"
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@gesture_turn_left_45outDelta_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
}

$sequence "gesture_turn_left_45" {
	"police_animations_anims\gesture_turn_left_45.smd"
	activity "ACT_GESTURE_TURN_LEFT45" 1
	fadein 0.2
	fadeout 0.2
	blendlayer "gesture_turn_left_45default" 0 20 20 20
	blendlayer "gesture_turn_left_45default" 20 20 20 50 spline
	blendlayer "gesture_turn_left_45inDelta" 0 0 20 20
	blendlayer "gesture_turn_left_45outDelta" 20 20 20 50 spline
	fps 60
	ikrule "lfoot" release contact 10 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 10 10 20 target 3
	ikrule "rfoot" release contact 24 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 20 24 28 28 target 2
	weightlist "weights_gesture_reload_pistol"
}

$sequence "gesture_turn_left_90default" {
	"police_animations_anims\gesture_turn_left_90default.smd"
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@gesture_turn_left_90default_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
}

$sequence "gesture_turn_left_90inDelta" {
	"police_animations_anims\gesture_turn_left_90inDelta.smd"
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@gesture_turn_left_90inDelta_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
}

$sequence "gesture_turn_left_90outDelta" {
	"police_animations_anims\gesture_turn_left_90outDelta.smd"
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@gesture_turn_left_90outDelta_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
}

$sequence "gesture_turn_left_90" {
	"police_animations_anims\gesture_turn_left_90.smd"
	activity "ACT_GESTURE_TURN_LEFT90" 1
	fadein 0.2
	fadeout 0.2
	blendlayer "gesture_turn_left_90default" 0 20 20 20
	blendlayer "gesture_turn_left_90default" 20 20 20 57 spline
	blendlayer "gesture_turn_left_90inDelta" 0 0 20 20
	blendlayer "gesture_turn_left_90outDelta" 20 20 20 57 spline
	fps 60
	ikrule "lfoot" release contact 10 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 10 10 20 target 3
	ikrule "rfoot" release contact 27 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 20 27 34 34 target 2
	weightlist "weights_gesture_reload_pistol"
}

$sequence "gesture_turn_right_45default" {
	"police_animations_anims\gesture_turn_right_45default.smd"
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@gesture_turn_right_45default_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
}

$sequence "gesture_turn_right_45inDelta" {
	"police_animations_anims\gesture_turn_right_45inDelta.smd"
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@gesture_turn_right_45inDelta_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
}

$sequence "gesture_turn_right_45outDelta" {
	"police_animations_anims\gesture_turn_right_45outDelta.smd"
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@gesture_turn_right_45outDelta_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
}

$sequence "gesture_turn_right_45" {
	"police_animations_anims\gesture_turn_right_45.smd"
	activity "ACT_GESTURE_TURN_RIGHT45" 1
	fadein 0.2
	fadeout 0.2
	blendlayer "gesture_turn_right_45default" 0 20 20 20
	blendlayer "gesture_turn_right_45default" 20 20 20 50 spline
	blendlayer "gesture_turn_right_45inDelta" 0 0 20 20
	blendlayer "gesture_turn_right_45outDelta" 20 20 20 50 spline
	fps 60
	ikrule "rfoot" release contact 10 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 10 10 20 target 2
	ikrule "lfoot" release contact 24 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 20 24 28 28 target 3
	weightlist "weights_gesture_reload_pistol"
}

$sequence "gesture_turn_right_90default" {
	"police_animations_anims\gesture_turn_right_90default.smd"
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@gesture_turn_right_90default_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
}

$sequence "gesture_turn_right_90inDelta" {
	"police_animations_anims\gesture_turn_right_90inDelta.smd"
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@gesture_turn_right_90inDelta_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
}

$sequence "gesture_turn_right_90outDelta" {
	"police_animations_anims\gesture_turn_right_90outDelta.smd"
	delta
	// Crowbar writes this subtract option to prevent jigglebone and poseparameter problems when delta sequences are recompiled.
	subtract "@gesture_turn_right_90outDelta_corrective_animation" 0
	fadein 0.2
	fadeout 0.2
	hidden
	fps 30
}

$sequence "gesture_turn_right_90" {
	"police_animations_anims\gesture_turn_right_90.smd"
	activity "ACT_GESTURE_TURN_RIGHT90" 1
	fadein 0.2
	fadeout 0.2
	blendlayer "gesture_turn_right_90default" 0 20 20 20
	blendlayer "gesture_turn_right_90default" 20 20 20 50 spline
	blendlayer "gesture_turn_right_90inDelta" 0 0 20 20
	blendlayer "gesture_turn_right_90outDelta" 20 20 20 50 spline
	fps 60
	ikrule "rfoot" release contact 10 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 10 10 20 target 2
	ikrule "lfoot" release contact 27 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 20 27 34 34 target 3
	weightlist "weights_gesture_reload_pistol"
}

$sequence "jump_holding_jump" {
	"police_animations_anims\jump_holding_jump.smd"
	activity "ACT_JUMP" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 13 13 target 0 usesequence 
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 13 13 target 1 usesequence 
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 13 13 target 2 usesequence 
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 13 13 target 3 usesequence 
	walkframe 13 LX LY LZ
}

$sequence "jump_holding_glide" {
	"police_animations_anims\jump_holding_glide.smd"
	activity "ACT_GLIDE" 1
	fadein 0.2
	fadeout 0.2
	fps 5
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 5 5 target 0 usesequence 
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 5 5 target 1 usesequence 
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 5 5 target 2 usesequence 
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 5 5 target 3 usesequence 
	walkframe 5 LX LY LZ
}

$sequence "jump_holding_land" {
	"police_animations_anims\jump_holding_land.smd"
	activity "ACT_LAND" 1
	fadein 0.15
	fadeout 0.2
	fps 45
	ikrule "lfoot" footstep contact 0 fakeorigin 7.22 6.41 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 28 28 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -4.03 -7.18 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 28 28 target 2
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 28 28 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 28 28 target 1
}

$sequence "Neutral_to_Choked_Barnacle" {
	"police_animations_anims\Neutral_to_Choked_Barnacle.smd"
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 10 10 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 10 10 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 10 10 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 10 10 target 3
}

$sequence "Choked_Barnacle" {
	"police_animations_anims\Choked_Barnacle.smd"
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 35 35 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 35 35 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 35 35 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 35 35 target 3
}

$sequence "Crushed_Barnacle" {
	"police_animations_anims\Crushed_Barnacle.smd"
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 144 144 target 0
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 144 144 target 1
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 144 144 target 2
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 144 144 target 3
}

$sequence "Dropship_Deploy" {
	"police_animations_anims\Dropship_Deploy.smd"
	fadein 0.2
	fadeout 0.2
	fps 30
	ikrule "rhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 80 80 target 0 usesequence 
	ikrule "lhand" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 80 80 target 1 usesequence 
	ikrule "rfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 80 80 target 2 usesequence 
	ikrule "lfoot" release contact 0 fakeorigin 0 0 0 fakerotate 0 0 0 floor 0 height 0 radius 0 range 0 0 80 80 target 3 usesequence 
	walkframe 12 LX LY LZ
	walkframe 34 LX LY LZ
	walkframe 60 LX LY LZ
	walkframe 72 LX LY LZ
}

$sequence "Man_Gun_Aim_all" {
	"Man_Gun_up_right"
	"Man_Gun_up_center"
	"Man_Gun_up_left"
	"Man_Gun_mid_right"
	"Man_Gun_mid_center"
	"Man_Gun_mid_left"
	"Man_Gun_down_right"
	"Man_Gun_down_center"
	"Man_Gun_down_left"
	blend "aim_yaw" -60 60
	blend "aim_pitch" -35 50
	delta
	fadein 0.2
	fadeout 0.2
	hidden
	iklock "lfoot" 1 0
}

$sequence "Man_Gun" {
	"police_animations_anims\Man_Gun.smd"
	activity "ACT_IDLE_MANNEDGUN" 1
	fadein 0.2
	fadeout 0.2
	addlayer "Man_Gun_Aim_all"
	fps 30
	loop
	ikrule "lhand" attachment "L_hand" contact 0 fakeorigin 13.44 4.2 43.62 fakerotate 0.23 -0.37 -0.54 floor 0 height 0 radius 10 range 0 0 0 0 target 1
	ikrule "rhand" attachment "R_hand" contact 0 fakeorigin 15.76 -7.07 45.24 fakerotate -3.09 0 0.44 floor 0 height 0 radius 10 range 0 0 0 0 target 0
	ikrule "lfoot" footstep contact 0 fakeorigin 8.05 5.2 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 0 0 target 3
	ikrule "rfoot" footstep contact 0 fakeorigin -8.27 -8.34 0 fakerotate 0 0 0 floor 0 height 20 radius 2.5 range 0 0 0 0 target 2
}
