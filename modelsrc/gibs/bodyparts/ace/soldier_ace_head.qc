// Created by Crowbar 0.72
$modelname "gibs/soldier_ace_head.mdl"
$body studio "soldier_ace_head_soldier_elite_head_reference.smd"

$surfaceprop "flesh"

$illumposition -0.012 -0.23 9.278

$cdmaterials "models\Combine_Soldier\"
$cdmaterials "models\zombie_classic\"

$sequence idle "soldier_ace_head_soldier_elite_head_reference.smd"

$collisionmodel "soldier_ace_head_soldier_elite_head_reference.smd"
{
	$mass 2
	$inertia 1
	$damping 1
	$rotdamping 0
	$rootbone " "
	$concave
	$maxconvexpieces 3
}

$keyvalues
{
	prop_data
	{
		"base" "Flesh.Small"
	}
}