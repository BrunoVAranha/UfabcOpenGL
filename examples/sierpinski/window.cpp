#include "window.hpp"

#include <chrono>
#include <thread>

auto num_points{0};
bool circuloDesenhado = false;
bool reta1Desenhada = false;
bool reta2Desenhada = false;
auto reta_offset{0.0f};

void Window::onCreate() {
  auto const *vertexShader{R"gl(#version 300 es
    layout(location = 0) in vec2 inPosition;

    void main() { 
      gl_PointSize = 2.0;
      gl_Position = vec4(inPosition, 0, 1); 
    }
  )gl"};

  auto const *fragmentShader{R"gl(#version 300 es
    precision mediump float;

    out vec4 outColor;

    void main() {
      // VERDE //
      // outColor = vec4(0.0, 1.0, 0.0, 1.0);
      // BRANCO //
      outColor = vec4(1); 
    }
  )gl"};

  // Create shader program
  m_program = abcg::createOpenGLProgram(
      {{.source = vertexShader, .stage = abcg::ShaderStage::Vertex},
       {.source = fragmentShader, .stage = abcg::ShaderStage::Fragment}});


  // Set the clear color
  abcg::glClearColor(0, 0.5f, 0, 1);


  std::array<GLfloat, 2> sizes{};
#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
  abcg::glGetFloatv(GL_POINT_SIZE_RANGE, sizes.data());
#else
  abcg::glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, sizes.data());
#endif
  fmt::print("Point size: {:.2f} (min), {:.2f} (max)\n", sizes.at(0),
             sizes.at(1));

  // Start pseudorandom number generator
  auto const seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);

  // Randomly pick a pair of coordinates in the range [-1; 1)
  //std::uniform_real_distribution<float> realDistribution(-1.0f, 1.0f);
  m_P.x = 0;
  m_P.y = 0;
}

void Window::onPaint() {

  // Set the uniform variable outColor in your shader
GLint outColorLocation = glGetUniformLocation(m_program, "outColor");
glUseProgram(m_program);
glUniform4f(outColorLocation, 0.0f, 0.0f, 1.0f, 1.0f); // Set outColor to blue

  
  // Create OpenGL buffers for drawing the point at m_P
  setupModel();

  // Set the viewport
  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  // Start using the shader program
  abcg::glUseProgram(m_program);
  // Start using VAO
  abcg::glBindVertexArray(m_VAO);

  // Draw a single point
  abcg::glDrawArrays(GL_POINTS, 0, 1);

  // End using VAO
  abcg::glBindVertexArray(0);
  // End using the shader program
  abcg::glUseProgram(0);

  // CIRCULO//////////
  if(!circuloDesenhado){
    float raio = 0.9f;

    // Randomly pick the index of a triangle vertex
    std::uniform_int_distribution<int> intDistribution(0, m_points.size() - 1);
    auto const index{intDistribution(m_randomEngine)};

    // Gerar angulo aleatório
    float random_angle = 2.0 * 3.141592653589793 * (rand() % 1000) / 1000.0;
    m_P.x = raio * cos(random_angle);
    m_P.y = raio * sin(random_angle);

    m_P = (m_P + m_points.at(index)) / 4.0f;

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    num_points++;
    if(num_points>= 1500){
       num_points = 0;
       circuloDesenhado = true;
    }
  }
  ///////////////////////////////

  //////////// RETA 1 ////////////
  else if(!reta1Desenhada){
    //// Line properties
    float x1 = 0.2f; // x-coordinate of the first point
    float y1 = 0.0f; // y-coordinate of the first point
    float x2 = - 0.4f; // x-coordinate of the second point
    float y2 = 0.3f;
    auto num_points_reta{120};
    std::uniform_int_distribution<int> pointDistribution(0, num_points_reta);
    auto const point{pointDistribution(m_randomEngine)};

    float t = static_cast<float>(point) / num_points_reta;

    m_P.x = (x1 + t * (x2 - x1)) - reta_offset;
    m_P.y = (y1 + t * (y2 - y1));
    num_points++;
    if(num_points >= 500){
      num_points = 0;
      reta_offset = reta_offset + 0.01f;
    }
    if(reta_offset >= 0.1f){
      reta_offset = 0;
      reta1Desenhada = true;
    }
  }
    ////////////////////////

    //////////// RETA 2 ////////////
  else if(!reta2Desenhada){
    //// Line properties
    float x1 = 0.0f; // x-coordinate of the first point
    float y1 = 0.0f; // y-coordinate of the first point
    float x2 = -0.6f; // x-coordinate of the second point
    float y2 = 0.3f;
    auto num_points_reta{120};
    std::uniform_int_distribution<int> pointDistribution(0, num_points_reta);
    auto const point{pointDistribution(m_randomEngine)};

    float t = static_cast<float>(point) / num_points_reta;

    m_P.x = (x1 + t * (x2 - x1)) - reta_offset;
    m_P.y = (y1 + t * (y2 - y1));
    num_points++;
    if(num_points >= 500){
      num_points = 0;
      reta_offset = reta_offset + 0.01f;
    }
    if(reta_offset >= 0.1f){
      reta_offset = 0;
      reta2Desenhada = true;
    }
  }
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  {
    ImGui::SetNextWindowPos(ImVec2(5, 75));
    ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoDecoration);

    if (ImGui::Button("Clear window", ImVec2(150, 30))) {
      abcg::glClear(GL_COLOR_BUFFER_BIT);
      circuloDesenhado = false;
      reta1Desenhada = false;
      reta2Desenhada = false;
      num_points = 0;
    }

    ImGui::End();
  }
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}


void Window::onDestroy() {
  // Release shader program, VBO and VAO
  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_VBOVertices);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Window::setupModel() {
  // Release previous VBO and VAO
  abcg::glDeleteBuffers(1, &m_VBOVertices);
  abcg::glDeleteVertexArrays(1, &m_VAO);

  // Generate a new VBO and get the associated ID
  abcg::glGenBuffers(1, &m_VBOVertices);
  // Bind VBO in order to use it
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertices);
  // Upload data to VBO
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_P), &m_P, GL_STATIC_DRAW);
  // Unbinding the VBO is allowed (data can be released now)
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertices);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}