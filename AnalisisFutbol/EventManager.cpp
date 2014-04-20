#include "EventManager.h"
#include "GUI.h"

/* INICIA EL CONTROLADOR DE EVENTOS */
void EventManager::initMouseListener() {
	setMouseCallback(GUI_W, myMouseCallback);
}

/* TRATA LOS EVENTOS DEL RATÓN */
void EventManager::myMouseCallback(int ev, int x, int y, int flags, void* params) {
	switch(ev) {								// Analiza el evento recibido
		case CV_EVENT_MOUSEMOVE: {				// Si es un movimiento de ratón
			EventManager::onMouseMove(x,y);
			break;
		}
		case CV_EVENT_LBUTTONDOWN : {			// Si es un click
			EventManager::onClick(x,y);
			break;
		}
	}
}

/* TRATA LOS MOVIMIENTOS DEL RATÓN SOBRE LA INTERFAZ */
void EventManager::onMouseMove(int x, int y) {
	if(y >= BUTTON_TOP_MARGIN) {
		if(y <= BUTTON_TOP_MARGIN + BUTTON_HEIGHT) {
			if(x >= BUTTON_LEFT_MARGIN && x <= BUTTON_LEFT_MARGIN + BUTTON_WIDTH) {
				GUI::setHoverButton(0);		// Se trata del button 0
			} else if(x >= BUTTON_LEFT_MARGIN + BUTTON_SHIFT && x <= BUTTON_LEFT_MARGIN + BUTTON_SHIFT + BUTTON_WIDTH) {
				GUI::setHoverButton(1);		// Se trata del button 1
			} else if(x >= BUTTON_LEFT_MARGIN + BUTTON_SHIFT * 2 && x <= BUTTON_LEFT_MARGIN + BUTTON_SHIFT * 2 + BUTTON_WIDTH) {
				GUI::setHoverButton(2);		// Se trata del button 2
			} else {
				GUI::removeHover();
			}
		} else {
			GUI::removeHover();
		}
	} else {
		GUI::removeHover();
	}
}

/* TRATA LOS CLICKS DEL RATÓN */
void EventManager::onClick(int x, int y) {
	if(y >= BUTTON_TOP_MARGIN) {
		if(y <= BUTTON_TOP_MARGIN + BUTTON_HEIGHT) {
			if(x >= BUTTON_LEFT_MARGIN && x <= BUTTON_LEFT_MARGIN + BUTTON_WIDTH) {
				GUI::switchButton(0);	// Se trata del button 0
			} else if(x >= BUTTON_LEFT_MARGIN + BUTTON_SHIFT && x <= BUTTON_LEFT_MARGIN + BUTTON_SHIFT + BUTTON_WIDTH) {
				GUI::switchButton(1);	// Se trata del button 1
			} else if(x >= BUTTON_LEFT_MARGIN + BUTTON_SHIFT * 2 && x <= BUTTON_LEFT_MARGIN + BUTTON_SHIFT * 2 + BUTTON_WIDTH) {
				GUI::switchButton(2);	// Se trata del button 2
			}
		}
	} else if (y >= R_BUTTON_TOP_MARGIN - R_BUTTON_RADIUS &&
				y <= R_BUTTON_TOP_MARGIN + R_BUTTON_VERT_GROUP_SHIFT + R_BUTTON_VERT_SHIFT * 2 + R_BUTTON_RADIUS) {
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
				} else if(y >= R_BUTTON_TOP_MARGIN + R_BUTTON_VERT_GROUP_SHIFT - R_BUTTON_RADIUS) {
					if(y <= R_BUTTON_TOP_MARGIN + R_BUTTON_VERT_GROUP_SHIFT + R_BUTTON_RADIUS) {
						GUI::activateRadioButton(0,2);	// Radio button 0, grupo 2
					} else if(y >= R_BUTTON_TOP_MARGIN + R_BUTTON_VERT_GROUP_SHIFT +
								R_BUTTON_VERT_SHIFT * 2 - R_BUTTON_RADIUS) {
						GUI::activateRadioButton(2,2);	// Radio button 2, grupo 2
					} else if(y >= R_BUTTON_TOP_MARGIN + R_BUTTON_VERT_GROUP_SHIFT + R_BUTTON_VERT_SHIFT - R_BUTTON_RADIUS &&
								y <= R_BUTTON_TOP_MARGIN + R_BUTTON_VERT_GROUP_SHIFT + R_BUTTON_VERT_SHIFT + R_BUTTON_RADIUS) {
						GUI::activateRadioButton(1,2);	// Radio button 1, grupo 2
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
