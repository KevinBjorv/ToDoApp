#include <iostream>
#include "MenuManager.h"

int main() {
    auto dashboard = std::make_shared<Dashboard>();
    auto actionMenu = std::make_shared<ActionMenu>(dashboard);
    dashboard->setActionMenu(actionMenu);
    dashboard->start();
	return 0;
}
