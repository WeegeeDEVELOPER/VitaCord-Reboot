#include "log.hpp"
#include <psp2/io/fcntl.h>

void logSD(std::string logData){

#ifdef DEFINE_DEBUG
#warning DEBUG BUILD!
	logData += "\n";
	const char * data = logData.c_str();
	int f = sceIoOpen("ux0:data/vitacord/vitacord-log.txt", SCE_O_WRONLY | SCE_O_APPEND | SCE_O_CREAT, 0777);
	sceIoWrite(f, data, strlen(data));
	sceIoClose(f);

#endif

}

void criticalLogSD(std::string logData){

	logData += "\n";
	const char * data = logData.c_str();
	int f = sceIoOpen("ux0:data/vitacord/vitacord-log.txt", SCE_O_WRONLY | SCE_O_APPEND | SCE_O_CREAT, 0777);
	sceIoWrite(f, data, strlen(data));
	sceIoClose(f);
	
}



