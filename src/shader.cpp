#include "../headers/shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
   // 1. retrieve the vertex/fragment source code from filePath
   std::string vertexCode{};
   std::string fragmentCode{};
   std::ifstream vShaderFile{};
   std::ifstream fShaderFile{};

   // ensures ifstream objects can throw exceptions:
   vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
   fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
   try{
      // open files
      vShaderFile.open(vertexPath);
      fShaderFile.open(fragmentPath);
      std::stringstream vShaderStream{}, fShaderStream{};

      // read file's buffer contents into streams
      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();

      // Close file handlers
      vShaderFile.close();
      fShaderFile.close();

      // convert stream into string
      vertexCode   = vShaderStream.str();
      fragmentCode = fShaderStream.str();
      
   }catch(std::ifstream::failure &e){
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
   }

   const char* vShaderCode = vertexCode.c_str();
   const char* fShaderCode = fragmentCode.c_str();

   // 2. compile shaders
   unsigned int vertexShader{}, fragmentShader{};

   /**------ VERTEX SHADER -------**/
   vertexShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
   glCompileShader(vertexShader);

   /**------ PRINTS COMPILE ERRORS IF ANY (VERTEX)------**/
   m_checkCompilationErrors(vertexShader, "VERTEX");

   /**------ FRAGMENT SHADER ------**/
   fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
   glCompileShader(fragmentShader);

   /**------ PRINTS COMPILE ERRORS IF ANY (FRAGMENT)------**/
   m_checkCompilationErrors(fragmentShader, "FRAGMENT");

   /**------ LINK ID PROGRAM ------**/
   ID = glCreateProgram();
   glAttachShader(ID, vertexShader);
   glAttachShader(ID, fragmentShader);
   glLinkProgram(ID);

   /**------ PRINTS LINKING ERRORS IF ANY (ID PROGRAM)------**/
   m_checkCompilationErrors(ID, "PROGrAM");

   /**------ DELETE SHADERS AS THEY'RE LINKED TO PROGRAM NOW AND NO LONGER NECESSARY ------**/
   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);
   
}

void Shader::use()
{
   glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
   glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string &name, int value) const
{
   glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
   glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::m_checkCompilationErrors(unsigned int shader, std::string type)
{
   int success{};
   char infoLog[1024];
   std::transform(type.begin(), type.end(), type.begin(), [](char x){return std::toupper(x);});

   if(type != "PROGRAM")
   {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if(!success)
      {
         glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
         std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << '\n' << infoLog << std::endl;
      }
   }else{
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if(!success)
      {
         glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
         std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << '\n' << infoLog << std::endl;
      }
   }
   
   
}
