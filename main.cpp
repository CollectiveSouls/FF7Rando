// std libs
#include <string>
#include <vector>
// custom libs
//#include <field.h>
#include <kernel.h>
#include <scene.h>
#include <window.h>
#include <yamada.h>
#include <utility.h>

int main(int argc, char **argv) {
	// TODO: Get psximager integrated.
	
	// TODO: verify disc images
	// "!imgs-unpacked\\NTSC\\DISC1\\MINT\\DISKINFO.CNF"
	// ...check first and third lines of each file.

	// initialize kernel
	Kernel kernel("!imgs-unpacked\\NTSC\\DISC1\\INIT\\KERNEL.BIN");
	Window window("!imgs-unpacked\\NTSC\\DISC1\\INIT\\WINDOW.BIN");
	Yamada yamada("!imgs-unpacked\\NTSC\\DISC1\\INIT\\YAMADA.BIN");
	Scene scene("!imgs-unpacked\\NTSC\\DISC1\\BATTLE\\SCENE.BIN");
	
	std::cout << "Al Bhed Swapped \"d\": " + fftext::albhedEncode("45") << std::endl;
	
	return 0;
}
