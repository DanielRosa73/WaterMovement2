#include <graphics/shader.h>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = compileShader(vShaderCode, GL_VERTEX_SHADER);
    // fragment Shader
    fragment = compileShader(fShaderCode, GL_FRAGMENT_SHADER);

    // shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    // print linking errors if any
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        gShaderFile.open(geometryPath);
        std::stringstream vShaderStream, fShaderStream, gShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        gShaderStream << gShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        gShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        geometryCode = gShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment, geometry;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = compileShader(vShaderCode, GL_VERTEX_SHADER);
    // fragment Shader
    fragment = compileShader(fShaderCode, GL_FRAGMENT_SHADER);
    // geometry Shader
    geometry = compileShader(gShaderCode, GL_GEOMETRY_SHADER);

    // shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glAttachShader(id, geometry);
    glLinkProgram(id);
    // print linking errors if any
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glDeleteShader(geometry);
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* tesselationControlPath, const char* tesselationEvaluationPath) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string tesselationControlCode;
    std::string tesselationEvalutationCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream tcShaderFile;
    std::ifstream teShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    tcShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    teShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        tcShaderFile.open(tesselationControlPath);
        teShaderFile.open(tesselationEvaluationPath);

        std::stringstream vShaderStream, fShaderStream, tcShaderStream, teShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        tcShaderStream << tcShaderFile.rdbuf();
        teShaderStream << teShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        tcShaderFile.close();
        teShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        tesselationControlCode = tcShaderStream.str();
        tesselationEvalutationCode = teShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* tcShaderCode = tesselationControlCode.c_str();
    const char* teShaderCode = tesselationEvalutationCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment, tesselationControl, tesselationEvaluation;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = compileShader(vShaderCode, GL_VERTEX_SHADER);
    // fragment Shader
    fragment = compileShader(fShaderCode, GL_FRAGMENT_SHADER);
    // tesselation control Shader
    tesselationControl = compileShader(tcShaderCode, GL_TESS_CONTROL_SHADER);
    // tesselation evaluation Shader
    tesselationEvaluation = compileShader(teShaderCode, GL_TESS_EVALUATION_SHADER);

    // shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glAttachShader(id, tesselationControl);
    glAttachShader(id, tesselationEvaluation);
    glLinkProgram(id);
    // print linking errors if any
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glDeleteShader(tesselationControl);
    glDeleteShader(tesselationEvaluation);
}


// Constructeur pour le compute shader
Shader::Shader(const char* computePath) {
    std::string computeCode;
    std::ifstream cShaderFile;
    // ensure ifstream objects can throw exceptions:
    cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open file
        cShaderFile.open(computePath);
        std::stringstream cShaderStream;
        // read file's buffer contents into streams
        cShaderStream << cShaderFile.rdbuf();
        // close file handlers
        cShaderFile.close();
        // convert stream into string
        computeCode = cShaderStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* cShaderCode = computeCode.c_str();

    // compile shaders
    unsigned int compute;
    int success;
    char infoLog[512];

    // compute Shader
    compute = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(compute, 1, &cShaderCode, NULL);
    glCompileShader(compute);
    // print compile errors if any
    glGetShaderiv(compute, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(compute, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // shader Program
    id = glCreateProgram();
    glAttachShader(id, compute);
    glLinkProgram(id);
    // print linking errors if any
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // delete the shader as it's linked into our program now and no longer necessary
    glDeleteShader(compute);
}


unsigned int Shader::compileShader(const char* shaderCode, GLenum shaderType) {
    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::string shaderTypeStr;
        switch (shaderType) {
            case GL_VERTEX_SHADER: shaderTypeStr = "VERTEX"; break;
            case GL_FRAGMENT_SHADER: shaderTypeStr = "FRAGMENT"; break;
            case GL_GEOMETRY_SHADER: shaderTypeStr = "GEOMETRY"; break;
            case GL_TESS_CONTROL_SHADER: shaderTypeStr = "TESS CONTROL"; break;
            case GL_TESS_EVALUATION_SHADER: shaderTypeStr = "TESS EVALUATION"; break;
            case GL_COMPUTE_SHADER: shaderTypeStr = "COMPUTE"; break;
        }
        std::cout << "ERROR::SHADER::" << shaderTypeStr << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

void Shader::use() {
	glUseProgram(id);
}

void Shader::destroy() {
    glDeleteShader(id);
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, glm::vec2 value) const
{
    glUniform2f(glGetUniformLocation(id, name.c_str()), value.x, value.y);
}

void Shader::setVec3(const std::string& name, glm::vec3 value) const
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
}

void Shader::setVec4(const std::string& name, glm::vec4 value) const
{
    glUniform4f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::setMat2(const std::string& name, const glm::mat2& matrix) const {
    glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setMat3(const std::string& name, const glm::mat3& matrix) const {
    glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setMat4(const std::string& name, const glm::mat4& matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}