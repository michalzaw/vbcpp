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


// Ładowanie tektur
RTexture2D* ResourceManager::loadTexture(std::string path)
{
    // Sprawdzamy czy zasob juz istnieje
    std::list<std::unique_ptr<Resource>>::iterator it;
    for ( it = _resources.begin(); it != _resources.end(); ++it)
    {
        if ( (*it)->getPath() == path )
        {
            std::cout << "Resource istnieje. Zwracam istniejacy zasob: " << (*it)->getPath() << std::endl;
            std::unique_ptr<Resource>& res = *it;

            RTexture2D* tex =  dynamic_cast<RTexture2D*>( res.get());

//            std::cout << "Texture ID: " << tex->getID() << std::endl;

            return tex;//tex->getID();
        }
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


// Ładowanie shaderów
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

    std::unique_ptr<Resource> shader ( new RShader(path, ::loadShader(vertexPath.c_str(), fragmPath.c_str())) );

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


RModel* ResourceManager::loadModel(std::string path, std::string texturePath)
{
    //std::string path = vertexPath + ";" + fragmPath;

    // Sprawdzamy czy zasob juz istnieje
    std::list<std::unique_ptr<Resource>>::iterator it;
    for ( it = _resources.begin(); it != _resources.end(); ++it)
    {
        if ( (*it)->getPath() == path )
        {
            std::cout << "Resource istnieje. Zwracam istniejacy zasob: " << (*it)->getPath() << std::endl;
            std::unique_ptr<Resource>& res = *it;

            return dynamic_cast<RModel*>( res.get() );
        }
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
    // Sprawdzamy czy zasob juz istnieje
    std::list<std::unique_ptr<Resource>>::iterator it;
    for ( it = _resources.begin(); it != _resources.end(); ++it)
    {
        if ( (*it)->getPath() == path )
        {
            std::cout << "Resource istnieje. Zwracam istniejacy zasob: " << (*it)->getPath() << std::endl;
            std::unique_ptr<Resource>& res = *it;

            return dynamic_cast<RFont*>( res.get() );
        }
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

