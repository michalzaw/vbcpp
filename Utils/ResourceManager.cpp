#include "ResourceManager.h"

#include "../Graphics/LoadTexture.h"
#include "../Graphics/LoadShader.h"
#include "../Graphics/Load3ds.h"


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


GLuint ResourceManager::loadTexture(std::string path)
{
    // Sprawdzamy czy zasob juz istnieje
    std::list<std::unique_ptr<Resource>>::iterator it;
    for ( it = _resources.begin(); it != _resources.end(); ++it)
    {
        if ( (*it)->getPath() == path )
        {
            std::cout << "Resource istnieje. Zwracam istniejacy zasob: " << (*it)->getPath() << std::endl;
            std::unique_ptr<Resource>& res = *it;

            RTexture* tex =  dynamic_cast<RTexture*>( res.get());

            std::cout << "Texture ID: " << tex->getID() << std::endl;

            return tex->getID();
        }
    }

    // Zasob nie istnieje
    GLuint tID = LoadTexture(path.c_str(), true);

    if ( tID )
    {
        std::unique_ptr<RTexture> tex (new RTexture(path, tID));
        std::cout << "Resource nie istnieje. Tworzenie nowego zasobu... "  << tex.get()->getPath() << std::endl;;

        // Poniewaz std::move przenosi wartosc z pamieci obiektu 'tex' do pamiêci listy '_resources', nie mozna wiecej odwolac sie do obiektu 'tex'
        // Dlatego kopiuje sobie ID textury przez przesunieciem wskaznika do listy
        GLuint texID = tex->getID();
        _resources.push_back(std::move(tex));

        /*
        // Poniewaz std::move przenosi wartosc z pamieci obiektu 'tex' do pamiêci listy '_resources', nie mozna wiecej odwolac sie do obiektu 'tex'
        // Musialem odwolac sie do utworzonej tekstury poprzez iterator do ostatniego elementu na liscie (nowa tekstura jest zawsze wrzucana na koniec listy)
        std::list<std::unique_ptr<Resource>>::iterator it = _resources.end();
        std::unique_ptr<Resource>& res = *(--it);

        RTexture* tex = dynamic_cast<RTexture*>(res.get());
        */

        std::cout << "Texture ID: " << texID << std::endl;

        return texID;
    }

    return 0;
}


RShader* ResourceManager::loadShader(std::string vertexPath, std::string fragmPath)
{
    std::string path = vertexPath + ";" + fragmPath;

    // Sprawdzamy czy zasob juz istnieje
    std::list<std::unique_ptr<Resource>>::iterator it;
    for ( it = _resources.begin(); it != _resources.end(); ++it)
    {
        if ( (*it)->getPath() == path )
        {
            std::cout << "Resource istnieje. Zwracam istniejacy zasob: " << (*it)->getPath() << std::endl;
            std::unique_ptr<Resource>& res = *it;

            return dynamic_cast<RShader*>( res.get() );
        }
    }

    // std::unique_ptr<Shader> shdr1( new Shader(LoadShader("DirLight.vert", "DirLight.frag")) );

    std::unique_ptr<Resource> shader ( new RShader(path, LoadShader(vertexPath.c_str(), fragmPath.c_str())) );

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


Model* ResourceManager::loadModel(std::string path, std::string texturePath, OGLDriver* driver)
{

    return 0;
}

