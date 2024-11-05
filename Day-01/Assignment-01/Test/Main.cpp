#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

void processInput(GLFWwindow* window, glm::vec3& translation1, glm::vec3& scale1, float& angle1,
    glm::vec3& translation2, glm::vec3& scale2, float& angle2,
    glm::vec3& combinedTranslation, glm::vec3& combinedScale, float& combinedAngle,
    int& currentMode);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    layout(location = 1) in vec3 aColor;
    out vec3 ourColor;
    uniform mat4 transform;
    void main() {
        gl_Position = transform * vec4(aPos, 1.0);
        ourColor = aColor;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    in vec3 ourColor;
    out vec4 FragColor;
    void main() {
        FragColor = vec4(ourColor, 1.0);
    }
)";

int main() {
    // initailizing GLFW
    if (!glfwInit()) {
        cerr << "Failed to initialize GLFW" <<endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Transformation Example", NULL, NULL);
    if (!window) {
        cerr << "Failed to create GLFW window" <<endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cerr << "Failed to initialize GLAD" <<endl;
        return -1;
    }

    
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Define both objects around the origin
    GLfloat vertices1[] =
    {
            -0.58f, -0.28f, 0.0f,	1.0f, 0.85f, 0.57f, //plate left top
           
            //- 0.05f,  -0.77f, 0.0f,	1.0f, 0.85f, 0.57f, // Plate Middle

             -0.44f, -0.39f, 0.0f,	1.0f, 0.85f, 0.57f, //plate left bottom
            0.52f,	-0.28f,	0.0f,	1.0f, 0.85f, 0.57f, // Plate Right top
           
            0.36f,	-.39f,	0.0f,	1.0f, 0.85f, 0.57f, // Plate right bottom


    };

    GLfloat vertices2[] =
    {
            -0.37f, -0.03f,   0.0f,	0.93f, 0.45f, 0.13f, //bottom cake top left
            -0.37f, -0.28f,   0.0f,	0.93f, 0.45f, 0.13f,//bottom cake bottom left
            0.33f, -0.03f,  0.0f,	0.93f, 0.45f, 0.13f,//bottom cake Top Right
            0.33f, -0.28f,  0.0f,	0.93f, 0.45f, 0.13f,//bottom cake Bottom Right

            -0.29f, 0.21f,   0.0f,	0.67f, 0.79f, 0.61f, //Top cake top left
            -0.29f, -0.03f,  0.0f,	0.67f, 0.79f, 0.61f,//Top cake bottom left
            0.25f, 0.21f,  0.0f,	0.67f, 0.79f, 0.61f,//Top cake Top Right
            0.25f, -0.03f,  0.0f,	0.67f, 0.79f, 0.61f,//Top cake Bottom Right

            -0.07f, 0.21f, 0.0f,    0.32f, 0.59f, 0.84f,//Candle Bottom Left
             0.03f, 0.21f, 0.0f,    0.32f, 0.59f, 0.84f,//Candle Bottom Right
            -0.07f, 0.42f, 0.0f,    0.32f, 0.59f, 0.84f,//Candle Top Left
             0.03f, 0.42f, 0.0f,    0.32f, 0.59f, 0.84f,//Candle Top Right


             -0.02f, 0.42f, 0.0f,   0.0f,0.0f,0.0f, //Thread
             -0.02f, 0.47f, 0.0f,   0.0f,0.0f,0.0f, //Thread end
    };

    GLuint VAO1, VBO1, VAO2, VBO2;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // transformation variables
    glm::vec3 translation1(0.0f), scale1(1.0f), translation2(0.0f), scale2(1.0f);
    glm::vec3 combinedTranslation(0.0f), combinedScale(1.0f);
    float angle1 = 0.0f, angle2 = 0.0f, combinedAngle = 0.0f;
    int currentMode = 0; // 0 = combined, 1 = individual for object1, 2 = individual for object2

    while (!glfwWindowShouldClose(window)) {
        processInput(window, translation1, scale1, angle1, translation2, scale2, angle2,
            combinedTranslation, combinedScale, combinedAngle, currentMode);

       // glClearColor(0.67f, .67f, 0.67f, 0.2f);
        glClearColor(.8f, 0.8f, 0.82f, .5f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");

        // combined transformation
        glm::mat4 combinedTransform = glm::translate(glm::mat4(1.0f), combinedTranslation);
        combinedTransform = glm::rotate(combinedTransform, combinedAngle, glm::vec3(0.0f, 0.0f, 1.0f));
        combinedTransform = glm::scale(combinedTransform, combinedScale);

        // apply transformations
        glm::mat4 transform1 = combinedTransform;
        transform1 = glm::translate(transform1, translation1);
        transform1 = glm::rotate(transform1, angle1, glm::vec3(0.0f, 0.0f, 1.0f));
        transform1 = glm::scale(transform1, scale1);

        glm::mat4 transform2 = combinedTransform;
        transform2 = glm::translate(transform2, translation2);
        transform2 = glm::rotate(transform2, angle2, glm::vec3(0.0f, 0.0f, 1.0f));
        transform2 = glm::scale(transform2, scale2);

        // draw object 1
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform1));
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        //glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

        // draw object 2
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform2));
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
        glLineWidth(5.0f);
        glDrawArrays(GL_LINES, 12, 2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window, glm::vec3& translation1, glm::vec3& scale1, float& angle1,
    glm::vec3& translation2, glm::vec3& scale2, float& angle2,
    glm::vec3& combinedTranslation, glm::vec3& combinedScale, float& combinedAngle,
    int& currentMode) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        currentMode = 1; // Individual transformations for object1
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        currentMode = 2; // Individual transformations for object2
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
        currentMode = 0; // Combined transformation
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        currentMode = 3; // Combined transformation

    // Adjust transformations based on the selected mode
    if (currentMode == 1) { // Object 1 transformations
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            translation1.y += 0.002f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            translation1.y -= 0.002f;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            translation1.x -= 0.002f;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            translation1.x += 0.002f;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            angle1 += 0.002f;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            angle1 -= 0.002f;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            scale1.y += 0.002f;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            scale1.y -= 0.002f;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            scale1.x -= 0.002f;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            scale1.x += 0.002f;
    }
    else if (currentMode == 2) { // Object 2 transformations
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            translation2.y += 0.002f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            translation2.y -= 0.002f;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            translation2.x -= 0.002f;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            translation2.x += 0.002f;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            angle2 += 0.002f;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            angle2 -= 0.002f;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            scale2.y += 0.002f;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            scale2.y -= 0.002f;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            scale2.x -= 0.002f;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            scale2.x += 0.002f;
    }
    else if(currentMode==0){ // Combined transformations
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            combinedTranslation.y += 0.002f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            combinedTranslation.y -= 0.002f;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            combinedTranslation.x -= 0.002f;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            combinedTranslation.x += 0.002f;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            combinedAngle += 0.002f;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            combinedAngle -= 0.002f;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            combinedScale.y += 0.002f;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            combinedScale.y -= 0.002f;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            combinedScale.x -= 0.002f;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            combinedScale.x += 0.002f;
    }
    else
    {
        combinedTranslation.y =0.0f;
        combinedTranslation.x =0.0f;
        combinedAngle = 0.0f;
        combinedScale.x= 1.0f;
        combinedScale.y= 1.0f;
        translation1.y = 0.00f;
        translation1.x = 0.00f;
        translation2.x = 0.00f;
        translation2.y = 0.00f;
        angle1 = 0.00f;
        angle2 = 0.00;
        scale1.x = 1.00;
        scale1.y = 1.00;
        scale2.x = 1.00;
        scale2.y = 1.00;




    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
