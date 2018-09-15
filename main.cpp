// std libs
// N/A

//
// psx-imager for working with psx disc isos directly
// #include <psximager/psxbuild.h>
// #include <psximager/psxinject.h>
// #include <psximager/psxrip.h>

//
// custom libs
#include <kernel.h>
#include <scene.h>
#include <window.h>
#include <field.h>
#include <yamada.h>

int main(int argc, char **argv) {
	// TODO: Get psximager integrated.

  //
  // TODO: connect & create seed concurrency with GUI
  unsigned long seed = 4294967295;
  
	// TODO: verify disc images
	// "!imgs-unpacked\\NTSC\\DISC1\\MINT\\DISKINFO.CNF"
	// ...check first and third lines of each file.

	// init kernel: handles items; and character bases and growth rates
	Kernel kernel("!imgs-unpacked\\NTSC\\DISC1\\INIT\\KERNEL.BIN", seed);

  // init Window: handles font sets and menu layouts
	// Window window("!imgs-unpacked\\NTSC\\DISC1\\INIT\\WINDOW.BIN");

  // init Menu: handles menu-specific data such as shop item IDs, dialogues, and character portraits
  // Menu menu("!imgs-unpacked\\NTSC\\DISC1\\MENU");

  // init Scene: handles battle data modidifications
	Scene scene("!imgs-unpacked\\NTSC\\DISC1\\BATTLE\\SCENE.BIN");

  // init Field: handles overworld data
  // Field field("!imgs-unpacked\\NTSC\\DISC1\\FIELD");  

	// init Yamada: handles critical file checksums
  Yamada yamada("!imgs-unpacked\\NTSC\\DISC1\\INIT\\YAMADA.BIN");

	
  // NOTE: Placeholder/Test for Al-Bhed cypher swapping. Swaps characters based on hex encoded binary value.
	std::cout << "Al Bhed Swapped \"d\": " + fftext::albhedEncode("45") << std::endl;
	
	return 0;
}
