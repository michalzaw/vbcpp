#ifndef OPENDIALOGWINDOW_H_INCLUDED
#define OPENDIALOGWINDOW_H_INCLUDED


#include <vector>
#include <string>
#include <functional>

#include "../../ImGuiInterface/ImGuiWindow.h"

#include "../../Utils/ResourceDescription.h"


class SceneManager;


class OpenDialogWindow final : public ImGuiWindow
{
	private:
		struct File
		{
			std::string name;
			std::string path;
			std::vector<File> children;
			bool isSelectable;
			bool isChildrenInitialized;

			ResourceDescription description;
			bool isDescriptionInitialized;

			File(const std::string& name = "", const std::string& path = "")
				: name(name), path(path), isSelectable(true), isChildrenInitialized(false), isDescriptionInitialized(false)
			{}
		};

		std::string _title;
		std::string _buttonOkTitle;
		std::vector<std::string> _rootPaths;

		std::function<void(const std::string&)> _onOkClickCallback;
		std::function<bool(const std::string&)> _directoryFilter;
		std::function<void(const std::string&, ResourceDescription&)> _descriptionLoader;


		int _currentItem;

		std::vector<File> _rootFiles;
		int _currentRootFileIndex;

		File* _selectedFile;


		void showBasePathCombo();
		void inspectPath(File& file);

		virtual void drawWindow() override;

	public:
		OpenDialogWindow(const std::string& title, const std::string& buttonOkTitle, const std::vector<std::string>& rootPaths, SceneManager* sceneManager, bool isOpen = false);

		void setOnOkClickCallback(const std::function<void(const std::string&)>& onOkClickCallback);
		void setDirectoryFilter(const std::function<bool(const std::string&)>& directoryFilter);
		void setDescriptionLoader(const std::function<void(const std::string&, ResourceDescription&)>& descriptionLoader);

		void setDefaultDirectoryFilter(const std::string& resourceFileName);
		void setDefaultDescriptionLoader(const std::string& resourceFileName, const std::string& rootElementName);

};

// deprecated
bool openDialogWindow(const std::string& title, const std::string& buttonOkTitle, const std::vector<std::string>& items, const std::function<void(int)> onOkClickCallback);


#endif // OPENDIALOGWINDOW_H_INCLUDED
