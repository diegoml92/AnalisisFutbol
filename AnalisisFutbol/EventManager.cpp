#include "EventManager.h"
#include "GUI.h"

/* INICIA EL CONTROLADOR DE EVENTOS */
void EventManager::initMouseListener() {
	setMouseCallback(GUI_W, myMouseCallback);
	setMouseCallback(SGUI_W, myMouseCallbackSts);
}

/* TRATA LOS EVENTOS DEL RATÓN */
void EventManager::myMouseCallback(int ev, int x, int y, int flags, void* params) {
	switch(ev) {								// Analiza el evento recibido
		case CV_EVENT_LBUTTONDOWN : {			// Si es un click
			EventManager::onClick(x,y);
			break;
		}
	}
}

/* TRATA LOS EVENTOS DEL RATÓN */
void EventManager::myMouseCallbackSts(int ev, int x, int y, int flags, void* params) {
	if(ev == CV_EVENT_LBUTTONDOWN) {
		EventManager::onClickSts(x,y);
	}
}

/* TRATA LOS CLICKS DEL RATÓN */
void EventManager::onClick(int x, int y) {
	if (y >= R_BUTTON_TOP_MARGIN - R_BUTTON_RADIUS &&
				y <= R_BUTTON_TOP_MARGIN + R_BUTTON_VERT_GROUP_SHIFT + R_BUTTON_RADIUS) {
		if(x >= R_BUTTON_LEFT_MARGIN - R_BUTTON_RADIUS) {
			if(x <= R_BUTTON_LEFT_MARGIN + R_BUTTON_RADIUS) {
				if(y <= R_BUTTON_TOP_MARGIN  + R_BUTTON_VERT_SHIFT * 2 + R_BUTTON_RADIUS) {
					if(y <= R_BUTTON_TOP_MARGIN + R_BUTTON_RADIUS) {
						GUI::activateRadioButton(0,0);	// Radio button 0, grupo 0
					} else if(y >= R_BUTTON_TOP_MARGIN + R_BUTTON_VERT_SHIFT * 2 - R_BUTTON_RADIUS) {
						GUI::activateRadioButton(2,0);	// Radio button 2, grupo 0
					} else if(y >= R_BUTTON_TOP_MARGIN + R_BUTTON_VERT_SHIFT - R_BUTTON_RADIUS &&
								y <= R_BUTTON_TOP_MARGIN + R_BUTTON_VERT_SHIFT + R_BUTTON_RADIUS) {
						GUI::activateRadioButton(1,0);	// Radio button 1, grupo 0
					}
				}
			} else if(x <= R_BUTTON_LEFT_MARGIN + R_BUTTON_GROUP_SHIFT + R_BUTTON_RADIUS &&
						x >= R_BUTTON_LEFT_MARGIN + R_BUTTON_GROUP_SHIFT - R_BUTTON_RADIUS) {
				if(y <= R_BUTTON_TOP_MARGIN  + R_BUTTON_VERT_SHIFT * 2 + R_BUTTON_RADIUS) {
					if(y <= R_BUTTON_TOP_MARGIN + R_BUTTON_RADIUS) {
						GUI::activateRadioButton(0,1);	// Radio button 0, grupo 1
					} else if(y >= R_BUTTON_TOP_MARGIN + R_BUTTON_VERT_SHIFT * 2 - R_BUTTON_RADIUS) {
						GUI::activateRadioButton(2,1);	// Radio button 2, grupo 1
					} else if(y >= R_BUTTON_TOP_MARGIN + R_BUTTON_VERT_SHIFT - R_BUTTON_RADIUS &&
								y <= R_BUTTON_TOP_MARGIN + R_BUTTON_VERT_SHIFT + R_BUTTON_RADIUS) {
						GUI::activateRadioButton(1,1);	// Radio button 1, grupo 1
					}
				}
			}
		}
	}
}

/* TRATA LOS CLICKS DEL RATÓN */
void EventManager::onClickSts(int x, int y) {
	if(y>=375) {
		if(x<=15) { // Comprobamos si es el primer checkBox
			if(x>=5 && y<=375+10) {
				GUI::switchCheckBox(0);
			}
		} else if(x<=5+150+10 && x>=5+150 && y<=375+10) { //Comprobamos si es el segundo checkbox
			GUI::switchCheckBox(1);
		}
	} else if(y<=25+5) {
		// RADIO BUTTONS 0
		if(x<=300/2+10+5 && y>=25-5) {
			if(x>=300/2+10-5) {
				GUI::activateRadioButtonSts(1,0);
			} else if(x>=10-5 && x<=10+5) {
				GUI::activateRadioButtonSts(0,0);
			}
		}
	} else if(y>=75-5 && y<=325+5) {
		if(x<=300/2+10+5) {
			if(x>=300/2+10-5) {
				// RADIO BUTTONS 2
				int mod = (y-(75-5)) % 25;
				if(mod<=10) {
					GUI::activateRadioButtonSts((y-75+5)/25,2);
				}
			} else if(x<=10+5 && x>=10-5) {
				// RADIO BUTTONS 1
				int mod = (y-(75-5)) % 25;
				if(mod<=10) {
					GUI::activateRadioButtonSts((y-75+5)/25,1);
				}
			}
		}
	}
}
