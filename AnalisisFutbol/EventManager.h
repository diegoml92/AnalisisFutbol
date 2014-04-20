#include "config_file.h"

/* MANEJO DE EVENTOS DEL USUARIO */
class EventManager {

private:

	/* TRATA LOS EVENTOS DEL RAT�N */
	static void myMouseCallback(int ev, int x, int y, int flags, void* params);

	/* TRATA LOS MOVIMIENTOS DEL RAT�N SOBRE LA INTERFAZ */
	static void onMouseMove(int x, int y);

	/* TRATA LOS CLICKS DEL RAT�N */
	static void onClick(int x, int y);

public:

	/* INICIA EL CONTROLADOR DE EVENTOS */
	static void initMouseListener();

};

