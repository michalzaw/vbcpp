#include "ResourceManager.h"

#include "FilesHelper.h"
#include "../Graphics/LoadTexture.h"
#include "../Graphics/LoadShader.h"
#include "../Graphics/StaticModelLoader.h"
#include "../Graphics/AnimatedModelLoader.h"
#include "../Graphics/AnimationLoader.h"
#include "../GUI/FontLoader.h"
#include "../Game/Directories.h"
#include "../Game/GameConfig.h"


static std::unique_ptr<ResourceManager> rsInstance;

ResourceManager::ResourceManager()
{
    LOG_INFO("ResourceManager: Konstruktor");

	_alternativeResourcePath = "";
}


ResourceManager::~ResourceManager()
{
    LOG_INFO("ResourceManager: Destruktor");

}


ResourceManager& ResourceManager::getInstance()
{
    if( !rsInstance )
        rsInstance = std::unique_ptr<ResourceManager>(new ResourceManager);

    return* rsInstance;
}


Resource* ResourceManager::findResource(std::string path)
{
// Sprawdzamy czy zasob juz istnieje
    std::list<std::unique_ptr<Resource>>::iterator it;
    for ( it = _resources.begin(); it != _resources.end(); ++it)
    {
        if ( (*it)->getPath() == path )
        {
            LOG_INFO("Resource istnieje. Zwracam istniejacy zasob: " + (*it)->getPath());
            std::unique_ptr<Resource>& res = *it;

            return res.get();
        }
    }

#ifdef DEVELOPMENT_RESOURCES
	path = _alternativeResourcePath + path;

	for (it = _resources.begin(); it != _resources.end(); ++it)
	{
		if ((*it)->getPath() == path)
		{
            LOG_INFO("Resource istnieje. Zwracam istniejacy zasob: " + (*it)->getPath());
			std::unique_ptr<Resource>& res = *it;

			return res.get();
		}
	}
#endif // DEVELOPMENT_RESOURCES

    return 0;
}


// Ładowanie tektur
RTexture2D* ResourceManager::loadTexture(std::string path, bool useCompression, bool mipmapping, bool useAnisotropicFiltering)
{
    Resource* res = findResource(path);
    if (res != 0)
    {
        RTexture2D* tex = dynamic_cast<RTexture2D*>(res);
        return tex;
    }

    // Zasob nie istnieje
#ifdef DEVELOPMENT_RESOURCES
	if (!FilesHelper::isFileExists(path))
		path = _alternativeResourcePath + path;
#endif // DEVELOPMENT_RESOURCES

    int width, height;
    //GLuint tID = ::loadTexture(path.c_str(), &width, &height, true);
	bool textureCompression = useCompression && GameConfig::getInstance().textureCompression;
    RTexture2D* texture = ::loadTexture(path.c_str(), textureCompression, mipmapping);

    if ( texture )
    {
		bool anisotropic = useAnisotropicFiltering && GameConfig::getInstance().anisotropicFiltering;
		float anisotropySamples = GameConfig::getInstance().anisotropySamples;
		if (anisotropic)
			texture->setAnisotropyFiltering(anisotropic, anisotropySamples);

        //std::unique_ptr<RTexture> tex (new RTexture(path, tID, TT_2D, glm::uvec2(width, height)));
        std::unique_ptr<RTexture> tex (texture);
        LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + tex.get()->getPath());

        // Poniewaz std::move przenosi wartosc z pamieci obiektu 'tex' do pamiêci listy '_resources', nie mozna wiecej odwolac sie do obiektu 'tex'
        // Dlatego kopiuje sobie ID textury przez przesunieciem wskaznika do listy
        //GLuint texID = tex->getID();
        _resources.push_back(std::move(tex));


        // Poniewaz std::move przenosi wartosc z pamieci obiektu 'tex' do pamiêci listy '_resources', nie mozna wiecej odwolac sie do obiektu 'tex'
        // Musialem odwolac sie do utworzonej tekstury poprzez iterator do ostatniego elementu na liscie (nowa tekstura jest zawsze wrzucana na koniec listy)
        std::list<std::unique_ptr<Resource>>::iterator it = _resources.end();
        std::unique_ptr<Resource>& res = *(--it);

        RTexture2D* t = dynamic_cast<RTexture2D*>(res.get());


        //std::cout << "Texture ID: " << tID << std::endl;

        return t;
    }

    return 0;
}


RTextureCubeMap* ResourceManager::loadTextureCubeMap(std::string* fileNames)
{
    std::string path = "";
    for (int i = 0; i < 6; ++i)
    {
        path += fileNames[i] + ";";
    }

    Resource* res = findResource(path);
    if (res != 0)
    {
        RTextureCubeMap* tex = dynamic_cast<RTextureCubeMap*>(res);
        return tex;
    }

    // Zasob nie istnieje
#ifdef DEVELOPMENT_RESOURCES
	for (int i = 0; i < 6; ++i)
	{
		if (!FilesHelper::isFileExists(fileNames[i]))
			fileNames[i] = _alternativeResourcePath + fileNames[i];
	}
#endif // DEVELOPMENT_RESOURCES
    RTextureCubeMap* texture = ::loadTextureCubeMap(fileNames, path.c_str(), true);

    if ( texture )
    {
        //std::unique_ptr<RTexture> tex (new RTexture(path, tID, TT_2D, glm::uvec2(width, height)));
        std::unique_ptr<RTexture> tex (texture);
        LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + tex.get()->getPath());

        // Poniewaz std::move przenosi wartosc z pamieci obiektu 'tex' do pamiêci listy '_resources', nie mozna wiecej odwolac sie do obiektu 'tex'
        // Dlatego kopiuje sobie ID textury przez przesunieciem wskaznika do listy
        //GLuint texID = tex->getID();
        _resources.push_back(std::move(tex));


        // Poniewaz std::move przenosi wartosc z pamieci obiektu 'tex' do pamiêci listy '_resources', nie mozna wiecej odwolac sie do obiektu 'tex'
        // Musialem odwolac sie do utworzonej tekstury poprzez iterator do ostatniego elementu na liscie (nowa tekstura jest zawsze wrzucana na koniec listy)
        std::list<std::unique_ptr<Resource>>::iterator it = _resources.end();
        std::unique_ptr<Resource>& res = *(--it);

        RTextureCubeMap* t = dynamic_cast<RTextureCubeMap*>(res.get());


        //std::cout << "Texture ID: " << tID << std::endl;

        return t;
    }

    return 0;
}


void ResourceManager::reloadTexture(RTexture2D* texture)
{
	::loadTexture(texture->getPath().c_str(), texture->isCompressed(), true, texture);
}


void ResourceManager::reloadTexture(std::string path)
{
    Resource* texture = findResource(path);
    if (texture != NULL)
    {
        reloadTexture(dynamic_cast<RTexture2D*>(texture));
    }
}


void ResourceManager::reloadTexture(RTextureCubeMap* texture)
{
    /*std::string fileNames[6];
    istringstream stream(texture->getPath());
    std::string s;
    int index = 0;
    while (getline(stream, s, ';')) {
        fileNames[index++] = s;
    }

    ::loadTextureCubeMap(fileNames, texture->getPath().c_str(), true, texture);*/
}


void ResourceManager::reloadAllTextures()
{
    std::list<std::unique_ptr<Resource>>::iterator it;
    for ( it = _resources.begin(); it != _resources.end(); ++it)
    {
        if ( (*it)->getType() == RT_TEXTURE )
        {
            std::unique_ptr<Resource>& res = *it;

            RTexture* texture = dynamic_cast<RTexture*>(res.get());
            if (texture->getTextureType() == TT_2D)
                reloadTexture(dynamic_cast<RTexture2D*>(texture));
            else if (texture->getTextureType() == TT_CUBE)
                reloadTexture(dynamic_cast<RTextureCubeMap*>(texture));
        }
    }
}


RTexture2D* ResourceManager::loadDefaultWhiteTexture()
{
    Resource* res = findResource(DEFAULT_WHITE_TEXTURE_NAME);
    if (res != 0)
    {
        RTexture2D* tex = dynamic_cast<RTexture2D*>(res);
        return tex;
    }

    // Zasob nie istnieje
    glm::uvec2 size(2, 2);
	bool textureCompression = GameConfig::getInstance().textureCompression;
    RTexture2D* texture = RTexture2D::createWhiteTexture(DEFAULT_WHITE_TEXTURE_NAME, size, textureCompression);

    if ( texture )
    {
        //std::unique_ptr<RTexture> tex (new RTexture(path, tID, TT_2D, glm::uvec2(width, height)));
        std::unique_ptr<RTexture> tex (texture);
        LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + tex.get()->getPath());

        // Poniewaz std::move przenosi wartosc z pamieci obiektu 'tex' do pamiêci listy '_resources', nie mozna wiecej odwolac sie do obiektu 'tex'
        // Dlatego kopiuje sobie ID textury przez przesunieciem wskaznika do listy
        //GLuint texID = tex->getID();
        _resources.push_back(std::move(tex));


        // Poniewaz std::move przenosi wartosc z pamieci obiektu 'tex' do pamiêci listy '_resources', nie mozna wiecej odwolac sie do obiektu 'tex'
        // Musialem odwolac sie do utworzonej tekstury poprzez iterator do ostatniego elementu na liscie (nowa tekstura jest zawsze wrzucana na koniec listy)
        std::list<std::unique_ptr<Resource>>::iterator it = _resources.end();
        std::unique_ptr<Resource>& res = *(--it);

        RTexture2D* t = dynamic_cast<RTexture2D*>(res.get());


        //std::cout << "Texture ID: " << tID << std::endl;

        return t;
    }

    return 0;
}


RTexture2D* ResourceManager::loadOneColorTexture(glm::vec4 color)
{
	std::string name = "texture(" + toString(color.r) + "," + toString(color.g) + "," + toString(color.b) + "," + toString(color.a) + ")";

	Resource* res = findResource(name);
	if (res != 0)
	{
		RTexture2D* tex = dynamic_cast<RTexture2D*>(res);
		return tex;
	}

	// Zasob nie istnieje
	bool textureCompression = GameConfig::getInstance().textureCompression;
	RTexture2D* texture = RTexture2D::createOneColorTexture(name, color, textureCompression);

	if (texture)
	{
		//std::unique_ptr<RTexture> tex (new RTexture(path, tID, TT_2D, glm::uvec2(width, height)));
		std::unique_ptr<RTexture> tex(texture);
        LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + tex.get()->getPath());

		// Poniewaz std::move przenosi wartosc z pamieci obiektu 'tex' do pamiêci listy '_resources', nie mozna wiecej odwolac sie do obiektu 'tex'
		// Dlatego kopiuje sobie ID textury przez przesunieciem wskaznika do listy
		//GLuint texID = tex->getID();
		_resources.push_back(std::move(tex));


		// Poniewaz std::move przenosi wartosc z pamieci obiektu 'tex' do pamiêci listy '_resources', nie mozna wiecej odwolac sie do obiektu 'tex'
		// Musialem odwolac sie do utworzonej tekstury poprzez iterator do ostatniego elementu na liscie (nowa tekstura jest zawsze wrzucana na koniec listy)
		std::list<std::unique_ptr<Resource>>::iterator it = _resources.end();
		std::unique_ptr<Resource>& res = *(--it);

		RTexture2D* t = dynamic_cast<RTexture2D*>(res.get());


		//std::cout << "Texture ID: " << tID << std::endl;

		return t;
	}

	return 0;
}


// Ładowanie shaderów
RShader* ResourceManager::loadShader(std::string vertexPath, std::string fragmPath, const std::vector<std::string>& defines,
                                     const std::unordered_map<std::string, std::string>& constants)
{
    std::string path = vertexPath + ";" + fragmPath;
    for (int i = 0; i < defines.size(); ++i)
    {
        path += ";" + defines[i];
    }
    if (constants.size() > 0)
        path += ";c:";

    for (std::pair<std::string, std::string> element : constants)
    {
        path += ";" + element.first + ":" + element.second;
    }

    Resource* res = findResource(path);
    if (res != 0)
    {
        RShader* shdr = dynamic_cast<RShader*>(res);
        return shdr;
    }

    // std::unique_ptr<Shader> shdr1( new Shader(LoadShader("DirLight.vert", "DirLight.frag")) );

#ifdef DEVELOPMENT_RESOURCES
	if (!FilesHelper::isFileExists(vertexPath))
		vertexPath = _alternativeResourcePath + vertexPath;
	if (!FilesHelper::isFileExists(fragmPath))
		fragmPath = _alternativeResourcePath + fragmPath;
#endif // DEVELOPMENT_RESOURCES

    std::unique_ptr<Resource> shader ( new RShader(path, ShaderLoader::loadShader(vertexPath.c_str(), fragmPath.c_str(), defines, constants), ST_NORMAL) );

    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + shader.get()->getPath());

    RShader* s = dynamic_cast<RShader*>( shader.get() );

    if ( s )
    {
        _resources.push_back(std::move(shader));
        return s;
    }
    else
        return 0;
}


RShader* ResourceManager::loadComputeShader(std::string shaderPath)
{
    Resource* res = findResource(shaderPath);
    if (res != 0)
    {
        RShader* shdr = dynamic_cast<RShader*>(res);
        return shdr;
    }

    // std::unique_ptr<Shader> shdr1( new Shader(LoadShader("DirLight.vert", "DirLight.frag")) );

#ifdef DEVELOPMENT_RESOURCES
    if (!FilesHelper::isFileExists(shaderPath))
        shaderPath = _alternativeResourcePath + shaderPath;
#endif // DEVELOPMENT_RESOURCES

    std::unique_ptr<Resource> shader(new RShader(shaderPath, ShaderLoader::loadComputeShader(shaderPath.c_str()), ST_COMPUTE));

    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + shader.get()->getPath());

    RShader* s = dynamic_cast<RShader*>(shader.get());

    if (s)
    {
        _resources.push_back(std::move(shader));
        return s;
    }
    else
        return 0;
}


void ResourceManager::reloadShader(RShader* shader)
{
    if (shader->getShaderType() == ST_NORMAL)
    {
        std::istringstream stream(shader->getPath());

        std::string vertexShaderFilename;
        std::string fragmentShaderFilename;
        std::vector<std::string> defines;
        std::unordered_map<std::string, std::string> constants;

        getline(stream, vertexShaderFilename, ';');
        getline(stream, fragmentShaderFilename, ';');

        string s;
        while (getline(stream, s, ';'))
        {
            if (s == "c:")
                break;

            defines.push_back(s);
        }

        while (getline(stream, s, ';'))
        {
            unsigned int pos = s.find(":");
            std::string name = s.substr(0, pos);
            std::string value = s.substr(pos + 1, s.size() - pos);

            constants[name] = value;
        }


        shader->setNewShader(ShaderLoader::loadShader(vertexShaderFilename.c_str(), fragmentShaderFilename.c_str(), defines, constants));
    }
    else
    {
        shader->setNewShader(ShaderLoader::loadComputeShader(shader->getPath().c_str()));
    }
}


void ResourceManager::reloadAllShaders()
{
    std::list<std::unique_ptr<Resource>>::iterator it;
    for ( it = _resources.begin(); it != _resources.end(); ++it)
    {
        if ( (*it)->getType() == RT_SHADER )
        {
            std::unique_ptr<Resource>& res = *it;

            reloadShader(dynamic_cast<RShader*>(res.get()));
        }
    }
}


RStaticModel* ResourceManager::loadModelWithHierarchy(std::string path, std::string texturePath, bool normalsSmoothing)
{
    Resource* res = findResource(path);
    if (res != 0)
    {
        RStaticModel* model = dynamic_cast<RStaticModel*>(res);
        return model;
    }

#ifdef DEVELOPMENT_RESOURCES
	if (!FilesHelper::isFileExists(path))
		path = _alternativeResourcePath + path;
#endif // DEVELOPMENT_RESOURCES

    StaticModelLoader loader(normalsSmoothing);
    std::unique_ptr<RStaticModel> model( loader.loadModelWithHierarchy(path, texturePath) );
    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + model.get()->getPath());

    RStaticModel* m = dynamic_cast<RStaticModel*>( model.get() );

    if ( m )
    {
        _resources.push_back(std::move(model));
        return m;
    }
    else
        return 0;
}


RStaticModel* ResourceManager::loadModelWithHierarchy(std::string path, std::string texturePath, std::vector<std::string> nodesToSkipNames, bool normalsSmoothing)
{
    std::string pathForResourceManager = path + "-";
    for (int i = 0; i < nodesToSkipNames.size(); ++i)
    {
        pathForResourceManager += ";" + nodesToSkipNames[i];
    }

    Resource* res = findResource(pathForResourceManager);
    if (res != 0)
    {
        RStaticModel* model = dynamic_cast<RStaticModel*>(res);
        return model;
    }

#ifdef DEVELOPMENT_RESOURCES
	if (!FilesHelper::isFileExists(path))
		path = _alternativeResourcePath + path;
#endif // DEVELOPMENT_RESOURCES

    StaticModelLoader loader(normalsSmoothing);
    std::unique_ptr<RStaticModel> model( loader.loadModelWithHierarchy(path, texturePath, nodesToSkipNames) );
    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + model.get()->getPath());

    RStaticModel* m = dynamic_cast<RStaticModel*>( model.get() );

    if ( m )
    {
        _resources.push_back(std::move(model));
        return m;
    }
    else
        return 0;
}


RStaticModel* ResourceManager::loadModelWithHierarchyOnlyNode(std::string path, std::string texturePath, std::string nodeToLoadName, Transform& loadedNodeTransformInModel, bool normalsSmoothing)
{
    std::string pathForResourceManager = path + "+" + nodeToLoadName;

    Resource* res = findResource(pathForResourceManager);
    if (res != 0)
    {
        RStaticModel* model = dynamic_cast<RStaticModel*>(res);
        return model;
    }

#ifdef DEVELOPMENT_RESOURCES
	if (!FilesHelper::isFileExists(path))
		path = _alternativeResourcePath + path;
#endif // DEVELOPMENT_RESOURCES

    StaticModelLoader loader(normalsSmoothing);
    std::unique_ptr<RStaticModel> model( loader.loadModelWithHierarchyOnlyNode(path, texturePath, nodeToLoadName, loadedNodeTransformInModel) );
    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + model.get()->getPath());

    RStaticModel* m = dynamic_cast<RStaticModel*>( model.get() );

    if ( m )
    {
        _resources.push_back(std::move(model));
        return m;
    }
    else
        return 0;
}


void ResourceManager::loadModelWithHierarchyOnlyNodes(std::string path, std::string texturePath, std::vector<std::string> nodesToLoadNames,
                                                      std::vector<Transform>& loadedNodesTransformsInModel, std::vector<RStaticModel*>& loadedNodes,
													  bool normalsSmoothing)
{
    loadedNodesTransformsInModel.resize(nodesToLoadNames.size());
    loadedNodes.resize(nodesToLoadNames.size());

    StaticModelLoader loader(normalsSmoothing);

#ifdef DEVELOPMENT_RESOURCES
	if (!FilesHelper::isFileExists(path))
		path = _alternativeResourcePath + path;
#endif // DEVELOPMENT_RESOURCES

    for (int i = 0; i < nodesToLoadNames.size(); ++i)
    {
        RStaticModel* model = loader.loadModelWithHierarchyOnlyNode(path, texturePath, nodesToLoadNames[i], loadedNodesTransformsInModel[i]);
        LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + path);

        loadedNodes[i] = model;
    }
}


RStaticModel* ResourceManager::loadModel(std::string path, std::string texturePath, bool normalsSmoothing)
{
    Resource* res = findResource(path);
    if (res != 0)
    {
        RStaticModel* model = dynamic_cast<RStaticModel*>(res);
        return model;
    }

#ifdef DEVELOPMENT_RESOURCES
	if (!FilesHelper::isFileExists(path))
		path = _alternativeResourcePath + path;
#endif // DEVELOPMENT_RESOURCES

    StaticModelLoader loader(normalsSmoothing);
    std::unique_ptr<RStaticModel> model( loader.loadModel(path, texturePath) );
    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + model.get()->getPath());

    RStaticModel* m = dynamic_cast<RStaticModel*>( model.get() );

    if ( m )
    {
        _resources.push_back(std::move(model));
        return m;
    }
    else
        return 0;
}


RAnimatedModel* ResourceManager::loadAnimatedModel(const std::string& path, const std::string& texturePath, const std::unordered_map<std::string, BoneInfo*>& boneInfosFromExistingModel/* = {}*/)
{
    Resource* res = findResource(path);
    if (res != 0)
    {
        RAnimatedModel* model = dynamic_cast<RAnimatedModel*>(res);
        return model;
    }

    std::string finalPath = path;

#ifdef DEVELOPMENT_RESOURCES
    if (!FilesHelper::isFileExists(path))
        finalPath = _alternativeResourcePath + path;
#endif // DEVELOPMENT_RESOURCES

    AnimatedModelLoader loader;
    std::unique_ptr<RAnimatedModel> model(loader.loadAnimatedModelWithHierarchy(finalPath, texturePath, boneInfosFromExistingModel));
    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + model.get()->getPath());

    RAnimatedModel* m = dynamic_cast<RAnimatedModel*>(model.get());

    if (m)
    {
        _resources.push_back(std::move(model));
        return m;
    }
    else
        return 0;
}


RAnimation* ResourceManager::loadAnimation(const std::string& path)
{
    Resource* res = findResource(path);
    if (res != 0)
    {
        RAnimation* animation = dynamic_cast<RAnimation*>(res);
        return animation;
    }

    std::string finalPath = path;

#ifdef DEVELOPMENT_RESOURCES
    if (!FilesHelper::isFileExists(path))
        finalPath = _alternativeResourcePath + path;
#endif // DEVELOPMENT_RESOURCES

    AnimationLoader loader;
    std::unique_ptr<RAnimation> animation(loader.loadAnimation(finalPath));
    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + animation.get()->getPath());

    RAnimation* a = dynamic_cast<RAnimation*>(animation.get());

    if (a)
    {
        _resources.push_back(std::move(animation));
        return a;
    }
    else
        return 0;
}


RFont* ResourceManager::loadFont(std::string path, int  pixelSize)
{
    Resource* res = findResource(FontLoader::createFontResourceName(path.c_str(), pixelSize));
    if (res != 0)
    {
        RFont* font = dynamic_cast<RFont*>(res);
        return font;
    }

#ifdef DEVELOPMENT_RESOURCES
	if (!FilesHelper::isFileExists(path))
		path = _alternativeResourcePath + path;
#endif // DEVELOPMENT_RESOURCES

    FontLoader loader;

    RFont* mTemp = loader.loadFont(path, pixelSize);

    if (mTemp)
    {
        std::unique_ptr<RFont> font(mTemp);
        LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + font.get()->getPath());

        RFont* f = dynamic_cast<RFont*>( font.get() );

        if ( f )
        {
            _resources.push_back(std::move(font));
            return f;
        }
        else
            return 0;
    }

    return 0;
}


RSound* ResourceManager::loadSound(std::string path)
{
    Resource* res = findResource(path);
    if (res != 0)
    {
        RSound* sound = dynamic_cast<RSound*>(res);
        return sound;
    }

#ifdef DEVELOPMENT_RESOURCES
	if (!FilesHelper::isFileExists(path))
		path = _alternativeResourcePath + path;
#endif // DEVELOPMENT_RESOURCES

    std::unique_ptr<RSound> sound( ::loadSound(path.c_str()) );
    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + sound.get()->getPath());

    RSound* s = dynamic_cast<RSound*>( sound.get() );

    if ( s )
    {
        _resources.push_back(std::move(sound));
        return s;
    }
    else
        return 0;
}


RObject* ResourceManager::loadRObject(std::string name)
{
	std::string dirPath = GameDirectories::OBJECTS + name + "/";

	Resource* res = findResource(dirPath);
	if (res != 0)
	{
		RObject* object = dynamic_cast<RObject*>(res);
		return object;
	}

#ifdef DEVELOPMENT_RESOURCES
	if (!FilesHelper::isDirectoryExists(dirPath))
		dirPath = _alternativeResourcePath + dirPath;
#endif // DEVELOPMENT_RESOURCES

	std::unique_ptr<RObject> object(RObjectLoader::loadObject(dirPath, name));
    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + object.get()->getPath());

	RObject* o = dynamic_cast<RObject*>(object.get());

	if (o)
	{
		_resources.push_back(std::move(object));
		return o;
	}
	else
		return 0;
}


RRoadProfile* ResourceManager::loadRoadProfile(std::string name)
{
	std::string dirPath = GameDirectories::ROAD_PROFILES + name + "/";

	Resource* res = findResource(dirPath);
	if (res != 0)
	{
		RRoadProfile* object = dynamic_cast<RRoadProfile*>(res);
		return object;
	}

#ifdef DEVELOPMENT_RESOURCES
	if (!FilesHelper::isDirectoryExists(dirPath))
		dirPath = _alternativeResourcePath + dirPath;
#endif // DEVELOPMENT_RESOURCES

	std::unique_ptr<RRoadProfile> object(RoadProfileLoader::loadRoadProfile(dirPath));
    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + object.get()->getPath());

	RRoadProfile* o = dynamic_cast<RRoadProfile*>(object.get());

	if (o)
	{
		_resources.push_back(std::move(object));
		return o;
	}
	else
		return 0;
}


RDisplayFont* ResourceManager::loadDisplayFont(std::string name)
{
	std::string dirPath = GameDirectories::DISPLAYS + name + "/";

	Resource* res = findResource(dirPath);
	if (res != 0)
	{
		RDisplayFont* object = dynamic_cast<RDisplayFont*>(res);
		return object;
	}

#ifdef DEVELOPMENT_RESOURCES
	if (!FilesHelper::isDirectoryExists(dirPath))
		dirPath = _alternativeResourcePath + dirPath;
#endif // DEVELOPMENT_RESOURCES

	std::unique_ptr<RDisplayFont> object(new RDisplayFont(dirPath));
    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + object.get()->getPath());

	RDisplayFont* o = dynamic_cast<RDisplayFont*>(object.get());

	if (o)
	{
		_resources.push_back(std::move(object));
		return o;
	}
	else
		return 0;
}


RMaterialsCollection* ResourceManager::loadMaterialsCollection(std::string path)
{
    Resource* res = findResource(path);
    if (res != 0)
    {
        RMaterialsCollection* materialsCollection = dynamic_cast<RMaterialsCollection*>(res);
        return materialsCollection;
    }

#ifdef DEVELOPMENT_RESOURCES
    if (!FilesHelper::isFileExists(path))
        path = _alternativeResourcePath + path;
#endif // DEVELOPMENT_RESOURCES

    RMaterialsCollection* temp = new RMaterialsCollection(path);

    MaterialLoader materialLoader;
    materialLoader.openFile(path.c_str());

    materialLoader.loadAllMaterials(temp->getMaterials(), FilesHelper::getPathToDirectoryFromFileName(path));

    materialLoader.closeFile();

    std::unique_ptr<RMaterialsCollection> materialsCollection(temp);
    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + materialsCollection.get()->getPath());

    RMaterialsCollection* mc = dynamic_cast<RMaterialsCollection*>(materialsCollection.get());

    if (mc)
    {
        _resources.push_back(std::move(materialsCollection));
        return mc;
    }
    else
        return 0;
}


void ResourceManager::setAlternativeResourcePath(std::string path)
{
	_alternativeResourcePath = path;
}


std::string ResourceManager::getAlternativeResourcePath()
{
	return _alternativeResourcePath;
}
