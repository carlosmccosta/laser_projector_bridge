#include <jmlaser.h>

int main(int argc, char** argv) {
	int devices = jmLaserEnumerateDevices();
	return 0;
}