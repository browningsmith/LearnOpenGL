#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class FShader
{
public:
    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    FShader(const char* VertexFilePath, const char* FragmentFilePath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string VertexCode;
        std::string FragmentCode;
        std::ifstream VertexShaderFile;
        std::ifstream FragmentShaderFile;
        // ensure ifstream objects can throw exceptions:
        VertexShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        FragmentShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try 
        {
            // open files
            VertexShaderFile.open(VertexFilePath);
            FragmentShaderFile.open(FragmentFilePath);
            std::stringstream VertexShaderStream, FragmentShaderStream;
            // read file's buffer contents into streams
            VertexShaderStream << VertexShaderFile.rdbuf();
            FragmentShaderStream << FragmentShaderFile.rdbuf();
            // close file handlers
            VertexShaderFile.close();
            FragmentShaderFile.close();
            // convert stream into string
            VertexCode   = VertexShaderStream.str();
            FragmentCode = FragmentShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* VertexShaderCode = VertexCode.c_str();
        const char * FragmentShaderCode = FragmentCode.c_str();
        // 2. compile shaders
        unsigned int Vertex, Fragment;
        // vertex shader
        Vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(Vertex, 1, &VertexShaderCode, NULL);
        glCompileShader(Vertex);
        checkCompileErrors(Vertex, "VERTEX");
        // fragment Shader
        Fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(Fragment, 1, &FragmentShaderCode, NULL);
        glCompileShader(Fragment);
        checkCompileErrors(Fragment, "FRAGMENT");
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, Vertex);
        glAttachShader(ID, Fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(Vertex);
        glDeleteShader(Fragment);
    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use() 
    { 
        glUseProgram(ID); 
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
#endif