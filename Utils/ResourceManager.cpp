#include "ResourceManager.h"

#include "../Graphics/LoadTexture.h"


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
    /*
    if (!rsInstance)
        rsInstance = unique_ptr<ResourceManager>(new ResourceManager);

    return *rsInstance;
    */

    if( !rsInstance )
        rsInstance = std::unique_ptr<ResourceManager>(new ResourceManager);

    return* rsInstance;
}


void ResourceManager::getString()
{
    std::cout << "String z ResourceManagera\n";
}


RTexture* ResourceManager::loadTexture(std::string path)
{
    // Sprawdzamy czy zasob juz istnieje
    std::list<std::unique_ptr<Resource>>::iterator it;
    for ( it = _resources.begin(); it != _resources.end(); ++it)
    {
        if ( (*it)->getPath() == path )
        {
            std::cout << "Resource istnieje. Zwracam istniejacy wskaznik: " << (*it)->getPath() << std::endl;
            std::unique_ptr<Resource>& res = *it;

            return dynamic_cast<RTexture*>( res.get() );
        }
    }

    // Zasob nie istnieje
    GLuint tID = LoadTexture(path.c_str(), true);

    if ( tID )
    {
        std::unique_ptr<RTexture> tex (new RTexture(path, tID));
        std::cout << "Resource nie istnieje. Tworzenie nowego zasobu... "  << tex.get()->getPath() << std::endl;;

        _resources.push_back(std::move(tex));

        // Poniewaz std::move przenosi wartosc z pamieci obiektu 'tex' do pamiêci listy '_resources', nie mozna wiecej odwolac sie do obiektu 'tex'
        // Musialem odwolac sie do utworzonej tekstury poprzez iterator do ostatniego elementu na liscie (nowa tekstura jest zawsze wrzucana na koniec listy)
        std::list<std::unique_ptr<Resource>>::iterator it = _resources.end();
        std::unique_ptr<Resource>& res = *(--it);

        return dynamic_cast<RTexture*>( res.get() );
    }

    return 0;
}
