#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include <iostream>
using namespace std;

#include <memory>

class Mesh
{
    public:
        Mesh() : _firstVertex(0), _vertexCount(0), _material(nullptr)
        {
            std::cout << "Mesh: Konstruktor\n";

            _material = make_shared<Material>();
        }

        virtual ~Mesh()
        {
            std::cout << "Mesh: Destruktor\n";
        }

        shared_ptr<Material>& getMaterial()
        {
            return _material;
        }

         //Material _material;
        unsigned int _firstVertex;
        unsigned int _vertexCount;


        private:
            shared_ptr<Material> _material;


};



#endif // MESH_H_INCLUDED
