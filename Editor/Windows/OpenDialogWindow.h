#ifndef OPENDIALOGWINDOW_H_INCLUDED
#define OPENDIALOGWINDOW_H_INCLUDED

#include <vector>
#include <string>
#include <functional>


struct ResourceDescription;


bool openDialogWindow2(const std::string& title, const std::string& buttonOkTitle, const std::string& rootPath, const std::function<void(const std::string&)>& onOkClickCallback,
					   const std::function<bool(const std::string&)>& directoryFilter = nullptr,
					   const std::function<void(const std::string&, ResourceDescription&)>& descriptionLoader = nullptr);
bool openDialogWindow(const std::string& title, const std::string& buttonOkTitle, const std::vector<std::string>& items, const std::function<void(int)> onOkClickCallback);


#endif // OPENDIALOGWINDOW_H_INCLUDED
