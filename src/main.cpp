#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <bits/stdc++.h>

#define CAMERA_SPEED 0.1f
#define ZNEAR -1.0f
#define ZFAR 1.0f
#define SCREEN_HEIGHT 800.0f
#define SCREEN_WIDTH 800.0f

using namespace std;

int numOfSides;
int toggle = 0;

void processInput(GLFWwindow *window, GLfloat vertices[], GLuint VAO, GLuint VBO, GLuint EBO, GLuint shaderProgram, GLfloat *c, glm::vec3 &cPos, glm::vec3 &cLook, glm::vec3 &cUp, GLfloat *camx, GLfloat *camy, GLfloat *camz)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
        
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        *c += 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        *camx -= CAMERA_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        *camx += CAMERA_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        *camy -= CAMERA_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        *camy += CAMERA_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        *camz -= CAMERA_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        *camz += CAMERA_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        cPos -= glm::normalize(glm::cross(cLook, cUp)) * CAMERA_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        cPos += glm::normalize(glm::cross(cLook, cUp)) * CAMERA_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        cPos -= cUp * CAMERA_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        cPos += cUp * CAMERA_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        cPos += cLook * CAMERA_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        cPos -= cLook * CAMERA_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        if (toggle == 0)
        {
            for(int i = 3*(2 + numOfSides); i < 3*(2 + 2 * numOfSides); i+=3)
            {
                vertices[i] = vertices[3];
                vertices[i+1] = vertices[4];
                vertices[i+2] = vertices[5];
            }
            toggle = 1;
        }
        else
        {
            for(int i = 3*(2 + numOfSides); i < 3*(2 + 2 * numOfSides); i+=3)
            {
                vertices[i] = vertices[i - numOfSides*3];
                vertices[i+1] = vertices[i + 1 - numOfSides*3];
                vertices[i+2] = ZFAR;
            }
            toggle = 0;
        }
        usleep(100000);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * (2*numOfSides + 2), vertices, GL_STATIC_DRAW);
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        *c = 0.0, *camx = 0.0, *camy = 0.0, *camz = 2.0;
        cPos = glm::vec3(0.0f, 0.0f,  0.0f);
        cLook = glm::vec3(0.0f, 0.0f, -1.0f);
        cUp = glm::vec3(0.0f, 1.0f,  0.0f);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

int main()
{
    const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform float yrot;"
    "uniform float x;"
    "uniform float y;"
    "uniform float z;"
    "uniform mat4 view;"
    "uniform mat4 transview;"
    "uniform mat4 projection;"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * transview * view * vec4(aPos.x * cos(yrot) - aPos.z * sin(yrot), aPos.y, aPos.z * cos(yrot) + aPos.x * sin(yrot), 2.0)/2.0;\n"
    "}\0";
    
    const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0f);\n"
    "}\n\0";

    int flag = 1;
    cout << "Please enter the number of sides of the polygon: ";
    
    while(flag)
    {
        cin >> numOfSides;
        if (numOfSides < 3)
        {
            flag = 1;
            cout << "Error, please enter an integer greater than 2: ";
        }
        else flag = 0;
    }
    float angle = 2/(float)numOfSides * M_PI;
    float x = 0;
    float y = 1;

    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLfloat vertices[(numOfSides * 2 + 2) * 3]={0};
    vertices[0] = 0.0f;
    vertices[1] = 0.0f;
    vertices[2] = ZNEAR;
    vertices[3] = 0.0f;
    vertices[4] = 0.0f;
    vertices[5] = ZFAR;

    float r, g, b;
    srand(time(0));

    float avgr1 = 0, avgr2 = 0, avgg1 = 0, avgg2 = 0, avgb1 = 0, avgb2 = 0;

    for(int i = 6; i < 6 + numOfSides * 3; i+=3)
    {
        vertices[i] = x;
        vertices[i+1] = y;
        vertices[i+2] = ZNEAR;

        vertices[i + numOfSides * 3] = x;
        vertices[i+1+ numOfSides * 3] = y;
        vertices[i+2+ numOfSides * 3] = ZFAR;

        float xnew = x * cos(angle) - y * sin(angle);
        float ynew = x * sin(angle) + y * cos(angle);
        x = xnew;
        y = ynew;
    }

    GLuint indices[4 * numOfSides * 3] = {0};

    int ind = 0;
    for(int i=2; i<= numOfSides + 1; i++)
    {
        indices[ind] = 0;
        indices[ind+1] = i;
        indices[ind+2] = ((i-numOfSides+2) > 2)?((i) % numOfSides + 1):(i+1);    // check
        ind += 3;
    }
    for(int i=2 + numOfSides; i<= 2*numOfSides + 1; i++)
    {
        indices[ind] = 1;
        indices[ind+1] = i;
        indices[ind+2] = ((i-numOfSides*2+2) > 2)?((i) % (numOfSides*2)+numOfSides+1):(i+1);    // check
        ind += 3;
    }
    for(int i=2; i<= numOfSides + 1; i++)
    {
        indices[ind] = i + numOfSides;
        indices[ind+1] = i;
        indices[ind+2] = ((i-numOfSides+2) > 2)?((i) % numOfSides + 1):(i+1);    // check
        ind += 3;
    }
    for(int i=2 + numOfSides; i<= 2*numOfSides + 1; i++)
    {
        indices[ind] = ((i-numOfSides*2+2) > 2)?((i) % (numOfSides*2)+numOfSides+1):(i+1);
        indices[ind] -= numOfSides;
        indices[ind+1] = i;
        indices[ind+2] = ((i-numOfSides*2+2) > 2)?((i) % (numOfSides*2)+numOfSides+1):(i+1);    // check
        ind += 3;
    }
    
    GLFWwindow *window = glfwCreateWindow(800, 800, "Assignment-0", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    gladLoadGL();

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader); // to make GPU understand code

    // Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Shader program to wrap both shaders to output
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram); // Link shader program

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // VAO & VBO -- order matters
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    GLfloat yrot = 0.0, camx = 0.0, camy = 0.0, camz = 2.0;
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glm::vec3 cPos = glm::vec3(0.0f, 0.0f,  0.0f);
    glm::vec3 cLook = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cUp = glm::vec3(0.0f, 1.0f,  0.0f);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);

    srand(time(0));
    GLfloat colours[numOfSides][3];
    for(int i=0; i<numOfSides; i++)
    {
        colours[i][0] = rand()/(float)RAND_MAX;
        colours[i][1] = rand()/(float)RAND_MAX;
        colours[i][2] = rand()/(float)RAND_MAX;
    }

    while(!glfwWindowShouldClose(window))
    {
        processInput(window, vertices, VAO, VBO, EBO, shaderProgram, &yrot, cPos, cLook, cUp, &camx, &camy, &camz);

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        int thetaLocation = glGetUniformLocation(shaderProgram, "yrot");
        glUniform1f(thetaLocation, yrot);
       
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(glm::vec3(0.0f - camx, 0.0f - camy, 0.03f - camz), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 viewtrans = glm::mat4(1.0f);
        viewtrans = glm::lookAt(cPos, cPos + cLook, cUp);
        
        int viewLocation = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        int transLocation = glGetUniformLocation(shaderProgram, "transview");
        glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(viewtrans));
        int projLocation = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(VAO);

        glm::vec3 colour = glm::vec3(1.0f, 0.5f, 0.0f);
        int colorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform3fv(colorLocation, 1, glm::value_ptr(colour));
        
        for(int i=0; i<numOfSides; i++)
        {
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)(i * 3 * sizeof(GLuint)));
        }
        colour = glm::vec3(0.0f, 0.5f, 0.0f);
        glUniform3fv(colorLocation, 1, glm::value_ptr(colour));
        for(int i=numOfSides; i<numOfSides*2; i++)
        {
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)(i * 3 * sizeof(GLuint)));
        }
        glUniform3fv(colorLocation, 1, glm::value_ptr(colour));
        for(int i=numOfSides*2; i<numOfSides*4; i++)
        {
            colour = glm::make_vec3(colours[(i - numOfSides * 2) % numOfSides]);
            glUniform3fv(colorLocation, 1, glm::value_ptr(colour));

            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)(i * 3 * sizeof(GLuint)));
        }

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);    
    glfwTerminate();
}