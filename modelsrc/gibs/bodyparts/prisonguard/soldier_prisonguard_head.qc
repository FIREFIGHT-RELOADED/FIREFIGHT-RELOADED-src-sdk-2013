// Created by Crowbar 0.72
$modelname "gibs/soldier_prisonguard_head.mdl"
$body studio "soldier_prisonguard_head_soldier_head_reference.smd"

$surfaceprop "flesh"

$illumposition -0.651 -0.276 8.193

$cdmaterials "models\Combine_Soldier\"
$cdmaterials "models\zombie_classic\"

$sequence idle "soldier_prisonguard_head_reference.smd"

$collisionmodel "soldier_prisonguard_head_soldier_head_reference.smd"
{
	$mass 2
	$inertia 1
	$damping 1
	$rotdamping 0
	$rootbone " "
	$concave
	$maxconvexpieces 5
}

$keyvalues
{
	prop_data
	{
		"base" "Flesh.Small"
	}
}