#include <string>
#include <vector>

#include <kernel.h>
#include <scene.h>

int main(int argc, char **argv) {
	// TODO: Get psximager integrated.
	
	// TODO: verify disc images
	// "!imgs-unpacked\\NTSC\\DISC1\\MINT\\DISKINFO.CNF"
	// ...check first and third lines of each file.


	// initialize kernel
	Kernel kernel("!imgs-unpacked\\NTSC\\DISC1\\INIT\\KERNEL.BIN");
	
	// TODO: yamada class
	// Yamada yamada("!imgs-unpacked\\NTSC\\DISC1\\INIT\\YAMADA.BIN");

	// TODO: scene class
	Scene scene("!imgs-unpacked\\NTSC\\DISC1\\BATTLE\\SCENE.BIN");
	

// test of file splitter, needs a bit more work, i think
// fileSplitter(40, {20,20}, "kernel\\kernel_8_0.data");
	
	return 0;
}
