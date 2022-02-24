#ifndef OPENDIALOGWINDOW_H_INCLUDED
#define OPENDIALOGWINDOW_H_INCLUDED

#include <vector>
#include <string>
#include <functional>

bool openDialogWindow(const std::string& title, const std::string& buttonOkTitle, const std::vector<std::string>& items, const std::function<void(int)> onOkClickCallback);


#endif // OPENDIALOGWINDOW_H_INCLUDED
