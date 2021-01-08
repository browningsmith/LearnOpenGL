#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class FShaderProgram
{
public:
    unsigned int ProgramID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    FShaderProgram(const char* VertexFilePath, const char* FragmentFilePath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string VertexShaderCode;
        std::string FragmentShaderCode;
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
            VertexShaderCode   = VertexShaderStream.str();
            FragmentShaderCode = FragmentShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* VertexShaderCodeNullTerminated = VertexShaderCode.c_str();
        const char * FragmentShaderCodeNullTerminated = FragmentShaderCode.c_str();
        // 2. compile shaders
        unsigned int VertexShaderID, FragmentShaderID;
        // vertex shader
        VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(VertexShaderID, 1, &VertexShaderCodeNullTerminated, NULL);
        glCompileShader(VertexShaderID);
        checkCompileErrors(VertexShaderID, "VERTEX");
        // fragment Shader
        FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(FragmentShaderID, 1, &FragmentShaderCodeNullTerminated, NULL);
        glCompileShader(FragmentShaderID);
        checkCompileErrors(FragmentShaderID, "FRAGMENT");
        // shader Program
        ProgramID = glCreateProgram();
        glAttachShader(ProgramID, VertexShaderID);
        glAttachShader(ProgramID, FragmentShaderID);
        glLinkProgram(ProgramID);
        checkCompileErrors(ProgramID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);
    }
    // activate the shader
    // ------------------------------------------------------------------------
    void Use() 
    { 
        glUseProgram(ProgramID);
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ProgramID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ProgramID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ProgramID, name.c_str()), value); 
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