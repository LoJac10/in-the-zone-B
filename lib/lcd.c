/**
 * lcd.c - LCD Selection Library
 **/


void lcdClear() {
    clearLCDLine(0);
    clearLCDLine(1);
}

/**
 * UI Component: Pick between two options, using left and right buttons to choose, and center to confirm
 * @param char * leftOption The option to display on the left
 * @param char * rightOption The option to display on the right
 * @return int The index of the choosen option, 0 for the left option, 1 for the right
 **/
int lcdPick(int line, char * leftOption, char * rightOption) {
    int choice = 0;

    string indicator = "*";
    string spacer = "  ";

    clearLCDLine(line);

    while(nLCDButtons != 2) {
        string buffer = "";
        if (nLCDButtons == 1) choice = 0;
        if (nLCDButtons == 4) choice = 1;

        if(!choice) strncat(buffer, indicator, 1);
        strncat(buffer, leftOption, sizeof(leftOption));
        strncat(buffer, spacer, 2);
        if(choice) strncat(buffer, indicator, 1);
        strncat(buffer, rightOption, sizeof(rightOption));

        displayLCDCenteredString(line, buffer); // All UI components, by default, display on line 1
    }
    lcdClear();
    return choice;

}


/**
 * UI Component: Pick between multiple options
 * @param int line The line to display on
 * @param string options The options to choose from, split by commas
 * @return int The index of the choosen option
 **/
int lcdMenu(int line, string * options, int size) {
	int choice = 0;
	int prev_choice = -1;

    clearLCDLine(line);

	while(true) {
		if (nLCDButtons == kButtonLeft && choice > 0) {
			choice--;
		}
		if (nLCDButtons == kButtonRight && choice < size - 1) {
			choice++;
		}
		if (nLCDButtons == kButtonCenter) {
			break;
		}
		string buffer = (string) options[choice];

		if(prev_choice != choice) {
			displayLCDCenteredString(line, buffer);
            displayLCDChar(line, 1, '<');
            displayLCDChar(line, 14, '>');
		}

		prev_choice = choice;
        wait1Msec(140);
	}

    return choice;
}


void lcdStartup() {
	  bLCDBacklight = true;
    lcdClear();

    displayLCDCenteredString(0, "Fast  Boye");
    displayLCDCenteredString(1, "3796B");
    wait1Msec(1000);

    lcdClear();
}

/**
 * Holds the code for autonomous selection, etc.
 * Run in pre_auton();
 **/
void robotConfigure() {

    displayLCDCenteredString(0, "Match Type");
    string matchTypes[] = { "Standard", "Driver", "Prog", "Rerun" };
    match.type = lcdMenu(1, matchTypes, 4);

    wait1Msec(500);

    if (match.type == 0) {
        displayLCDCenteredString(0, "Alliance");
        match.alliance = lcdPick(1, "Red", "Blue");
        wait1Msec(500);
    }

    displayLCDCenteredString(0, "Routine");
    // Menu Tree based on Match Type
    switch(match.type) {
        case 0:
            match.routine = lcdMenu(1, autonRoutines, arraySize(autonRoutines));
            break;
        case 2:
            match.routine = lcdMenu(1, skillsRoutines, arraySize(skillsRoutines));
            break;
        default: break;
    }


}
