#ifndef RSCRIPTFILE_H_INCLUDED
#define RSCRIPTFILE_H_INCLUDED


#include "../Utils/Resource.h"


class RScriptFile final : public Resource
{
	private:
		std::string _script;

	public:
		RScriptFile(const std::string& path, const std::string& script);

		inline void setScript(const std::string& newScript) { _script = newScript; }
		inline const std::string& getScript() { return _script; }

		static std::string loadScriptFromFile(const std::string& fileName);

};


#endif // RSCRIPTFILE_H_INCLUDED
