#include <iostream>
#include "MenuManager.h"

int main() {
	std::unique_ptr<Dashboard> dashboard = std::make_unique<Dashboard>();
	dashboard->start();

	return 0;
}
