#pragma once

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

/**
 * Basic automated test to verify ALSXT character can move forward.
 * Found in: Tools > Test Automation > Project.ALSXT.Movement.MoveForward
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAlsxtTest_BasicMovement, "Project.ALSXT.Movement.MoveForward", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)