#include "ResourceManager.h"

#include "../Graphics/LoadTexture.h"
#include "../Graphics/LoadShader.h"
#include "../Graphics/Load3ds.h"
#include "../GUI/FontLoader.h"


static std::unique_ptr<ResourceManager> rsInstance;

ResourceManager::ResourceManager()
{
    std::cout << "ResourceManager: Konstruktor\n";
}


ResourceManager::~ResourceManager()
{
    std::cout << "ResourceManager: Destruktor\n";

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
            std::cout << "Resource istnieje. Zwracam istniejacy zasob: " << (*it)->getPath() << std::endl;
            std::unique_ptr<Resource>& res = *it;

            return res.get();
        }
    }

    return 0;
}


// Ładowanie tektur
RTexture2D* ResourceManager::loadTexture(std::string path)
{
    Resource* res = findResource(path);
    if (res != 0)
    {
        RTexture2D* tex = dynamic_cast<RTexture2D*>(res);
        return tex;
    }

    // Zasob nie istnieje
    int width, height;
    //GLuint tID = ::loadTexture(path.c_str(), &width, &height, true);
    RTexture2D* texture = ::loadTexture(path.c_str(), true);

    if ( texture )
    {
        //std::unique_ptr<RTexture> tex (new RTexture(path, tID, TT_2D, glm::uvec2(width, height)));
        std::unique_ptr<RTexture> tex (texture);
        std::cout << "Resource nie istnieje. Tworzenie nowego zasobu... "  << tex.get()->getPath() << std::endl;

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


void ResourceManager::reloadTexture(RTexture2D* texture)
{
    ::loadTexture(texture->getPath().c_str(), true, texture);
}


void ResourceManager::reloadTexture(std::string path)
{
    Resource* texture = findResource(path);
    if (texture != NULL)
    {
        reloadTexture(dynamic_cast<RTexture2D*>(texture));
    }
}


void ResourceManager::reloadAllTextures()
{
    std::list<std::unique_ptr<Resource>>::iterator it;
    for ( it = _resources.begin(); it != _resources.end(); ++it)
    {
        if ( (*it)->getType() == RT_TEXTURE )
        {
            std::unique_ptr<Resource>& res = *it;

            reloadTexture(dynamic_cast<RTexture2D*>(res.get()));
        }
    }
}


// Ładowanie shaderów
RShader* ResourceManager::loadShader(std::string vertexPath, std::string fragmPath, const std::vector<std::string>& defines)
{
    std::string path = vertexPath + ";" + fragmPath;
    for (int i = 0; i < defines.size(); ++i)
    {
        path += ";" + defines[i];
    }


    Resource* res = findResource(path);
    if (res != 0)
    {
        RShader* shdr = dynamic_cast<RShader*>(res);
        return shdr;
    }

    // std::unique_ptr<Shader> shdr1( new Shader(LoadShader("DirLight.vert", "DirLight.frag")) );

    std::unique_ptr<Resource> shader ( new RShader(path, ShaderLoader::loadShader(vertexPath.c_str(), fragmPath.c_str(), defines)) );

    std::cout << "Resource nie istnieje. Tworzenie nowego zasobu... "  << shader.get()->getPath() << std::endl;;

    RShader* s = dynamic_cast<RShader*>( shader.get() );

    if ( s )
    {
        _resources.push_back(std::move(shader));
        return s;
    }
    else
        return 0;
}


void ResourceManager::reloadShader(RShader* shader)
{
    std::istringstream stream(shader->getPath());

    std::string vertexShaderFilename;
    std::string fragmentShaderFilename;
    std::vector<std::string> defines;

    getline(stream, vertexShaderFilename, ';');
    getline(stream, fragmentShaderFilename, ';');

    string s;
    while (getline(stream, s, ';'))
    {
        defines.push_back(s);
    }

    shader->setNewShader(ShaderLoader::loadShader(vertexShaderFilename.c_str(), fragmentShaderFilename.c_str(), defines));
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


RModel* ResourceManager::loadModel(std::string path, std::string texturePath)
{
    Resource* res = findResource(path);
    if (res != 0)
    {
        RModel* model = dynamic_cast<RModel*>(res);
        return model;
    }


    Load3ds l3ds;

    Model* mTemp = l3ds.loadModel(path, texturePath);

    if (mTemp)
    {
        std::unique_ptr<RModel> model( new RModel(path, mTemp) );
        std::cout << "Resource nie istnieje. Tworzenie nowego zasobu... "  << model.get()->getPath() << std::endl;

        RModel* m = dynamic_cast<RModel*>( model.get() );

        if ( m )
        {
            _resources.push_back(std::move(model));
            return m;
        }
        else
            return 0;
    }

    return 0;
}


RFont* ResourceManager::loadFont(std::string path, int  pixelSize)
{
    Resource* res = findResource(path);
    if (res != 0)
    {
        RFont* font = dynamic_cast<RFont*>(res);
        return font;
    }

    FontLoader loader;

    RFont* mTemp = loader.loadFont(path.c_str(), pixelSize);

    if (mTemp)
    {
        std::unique_ptr<RFont> font(mTemp);
        std::cout << "Resource nie istnieje. Tworzenie nowego zasobu... "  << font.get()->getPath() << std::endl;

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
