#ifndef VAO_H_INCLUDED
#define VAO_H_INCLUDED


class VAO
{
    private:
        GLuint _Id;

    public:
        VAO()
        {
            glGenVertexArrays(1, &_Id);
            Bind();
        }

        ~VAO()
        {
            glDeleteVertexArrays(1, &_Id);
        }

        void Bind()
        {
            glBindVertexArray(_Id);
        }
};


#endif // VAO_H_INCLUDED
