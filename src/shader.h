//
// Created by james on 5/20/2023.
//

#pragma once

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdint>
#include <glm/glm.hpp>

class Shader {
public:
    unsigned int id;

    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char *vertex_path, const char *fragment_path, const char* geometry_path = nullptr, const char* tess_control_path = nullptr, const char* tess_eval_path = nullptr) {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertex_code;
        std::ifstream v_shader_file;

        std::string fragment_code;
        std::ifstream f_shader_file;

        std::string geometry_code;
        std::ifstream g_shader_file;

        std::string tess_control_code;
        std::ifstream tc_shader_file;

        std::string tess_eval_code;
        std::ifstream te_shader_file;



        // ensure ifstream objects can throw exceptions:
        v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        g_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        tc_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        te_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            // open files
            v_shader_file.open(vertex_path);
            std::stringstream v_shader_stream;
            // read file's buffer contents into streams
            v_shader_stream << v_shader_file.rdbuf();
            // close file handlers
            v_shader_file.close();
            // convert stream into string
            vertex_code = v_shader_stream.str();
        }
        catch (std::ifstream::failure &e) {
            std::cout << "ERROR::VERTEX_SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }

        try {
            // open files
            f_shader_file.open(fragment_path);
            std::stringstream f_shader_stream;
            // read file's buffer contents into streams
            f_shader_stream << f_shader_file.rdbuf();
            // close file handlers
            f_shader_file.close();
            // convert stream into string
            fragment_code = f_shader_stream.str();
        }
        catch (std::ifstream::failure &e) {
            std::cout << "ERROR::FRAGMENT_SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }

        if (geometry_path != nullptr)
        {
            try {
                g_shader_file.open(geometry_path);
                std::stringstream g_shader_stream;
                g_shader_stream << g_shader_file.rdbuf();
                g_shader_file.close();
                geometry_code = g_shader_stream.str();
            }
            catch (std::ifstream::failure &e) {
                std::cout << "ERROR::GEOMETRY_SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
            }
        }

        if (tess_control_path != nullptr) {
            try {
                tc_shader_file.open(tess_control_path);
                std::stringstream tc_shader_stream;
                tc_shader_stream << tc_shader_file.rdbuf();
                tc_shader_file.close();
                tess_control_code = tc_shader_stream.str();
            }
            catch (std::ifstream::failure &e) {
                std::cout << "ERROR::TESS_CONTROL_SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
            }
        }

        if (tess_eval_path != nullptr) {
            try {
                te_shader_file.open(tess_eval_path);
                std::stringstream te_shader_stream;
                te_shader_stream << te_shader_file.rdbuf();
                te_shader_file.close();
                tess_eval_code = te_shader_stream.str();
            }
            catch (std::ifstream::failure &e) {
                std::cout << "ERROR::TESS_EVAL_SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
            }
        }



        const char *v_shader_code = vertex_code.c_str();
        const char *f_shader_code = fragment_code.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &v_shader_code, NULL);
        glCompileShader(vertex);
        check_compile_errors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &f_shader_code, NULL);
        glCompileShader(fragment);
        check_compile_errors(fragment, "FRAGMENT");

        unsigned int geometry;
        if(geometry_path != nullptr)
        {
            const char * g_shader_code = geometry_code.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &g_shader_code, NULL);
            glCompileShader(geometry);
            check_compile_errors(geometry, "GEOMETRY");
        }
        // if tessellation shader is given, compile tessellation shader
        unsigned int tess_control;
        if(tess_control_path != nullptr)
        {
            const char * tc_shader_code = tess_control_code.c_str();
            tess_control = glCreateShader(GL_TESS_CONTROL_SHADER);
            glShaderSource(tess_control, 1, &tc_shader_code, NULL);
            glCompileShader(tess_control);
            check_compile_errors(tess_control, "TESS_CONTROL");
        }
        unsigned int tess_eval;
        if(tess_eval_path != nullptr)
        {
            const char * teShaderCode = tess_eval_code.c_str();
            tess_eval = glCreateShader(GL_TESS_EVALUATION_SHADER);
            glShaderSource(tess_eval, 1, &teShaderCode, NULL);
            glCompileShader(tess_eval);
            check_compile_errors(tess_eval, "TESS_EVALUATION");
        }

        // shader Program
        id = glCreateProgram();
        glAttachShader(id, vertex);
        glAttachShader(id, fragment);
        glLinkProgram(id);
        check_compile_errors(id, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (geometry_path) glDeleteShader(geometry);
        if (tess_control_path) glDeleteShader(tess_control);
        if (tess_eval_path)  glDeleteShader(tess_eval);
    }

    // activate the shader
    // ------------------------------------------------------------------------
    void use() {
        glUseProgram(id);
    }

    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const
    {
        glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:

    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void check_compile_errors(unsigned int shader, std::string type) {
        int success;
        char info_log[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, info_log);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << info_log
                          << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, info_log);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << info_log
                          << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};