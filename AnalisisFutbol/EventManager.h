#include "config_file.h"

/* MANEJO DE EVENTOS DEL USUARIO */
class EventManager {

private:

	/* TRATA LOS EVENTOS DEL RAT�N */
	static void myMouseCallbackSts(int ev, int x, int y, int flags, void* params);

	/* TRATA LOS CLICKS DEL RAT�N */
	static void onClickSts(int x, int y);

public:

	/* INICIA EL CONTROLADOR DE EVENTOS */
	static void initMouseListener();

};

