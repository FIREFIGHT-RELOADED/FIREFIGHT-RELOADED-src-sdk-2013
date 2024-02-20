// Created by Crowbar 0.57

$modelname "blueheart.mdl"

$bodygroup "axebane"
{
	studio "heart.smd"
}

$surfaceprop "plastic"

$contents "solid"

$mostlyopaque

$cdmaterials "models\blueheart\"

$sequence "idle" {
	"heart.smd"
	activity "ACT_IDLE" 1
	fadein 0.2
	fadeout 0.2
	fps 30
}

$collisionmodel "heart.smd"
{
	$mass 150
	$inertia 1
	$damping 1
	$rotdamping 0
	$rootbone "root"
}
