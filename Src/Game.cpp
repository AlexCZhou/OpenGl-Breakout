#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"


// Game-related State data
SpriteRenderer  *Renderer;
GameObject      *Player;


Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{ 

}

Game::~Game()
{
    delete Renderer;
    delete Player;
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("../Resource/Shaders/sprite.vertex", "../Resource/Shaders/sprite.fragment", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), 
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Shader shader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(shader);
    // load textures
    ResourceManager::LoadTexture("../Resource/Textures/background.jpg", false, "background");
    ResourceManager::LoadTexture("../Resource/Textures/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("../Resource/Textures/block.png", false, "block");
    ResourceManager::LoadTexture("../Resource/Textures/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("../Resource/Textures/paddle.png", true, "paddle");
    // load levels
    GameLevel one; one.Load("../Resource/Levels/one.lvl", this->Width, this->Height / 2);
    GameLevel two; two.Load("../Resource/Levels/two.lvl", this->Width, this->Height / 2);
    // GameLevel three; three.Load("../Levels/three.lvl", this->Width, this->Height / 2);
    // GameLevel four; four.Load("../Levels/four.lvl", this->Width, this->Height / 2);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    // this->Levels.push_back(three);
    // this->Levels.push_back(four);
    this->Level = 0;
    // configure game objects
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
}

void Game::Update(float dt)
{
    ImguiUpdate();

}

void Game::ProcessInput(float dt)
{
   if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0.0f)
                Player->Position.x -= velocity;
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
                Player->Position.x += velocity;
        }
    }
}

void Game::Render()
{
    if(this->State == GAME_ACTIVE)
    {
        // draw background
        Texture2D texture = ResourceManager::GetTexture("background");
        Renderer->DrawSprite(texture, glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        // draw level
        this->Levels[this->Level].Draw(*Renderer);
        // draw player
        Player->Draw(*Renderer);
    }

    ImguiRander();
}

void Game::ImguiInit(GLFWwindow* window){
    //创建IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //ImFont* font = io.Fonts->AddFontFromFileTTF("Resource/font/SourceHanSerifCN-Medium-6.otf", 20.0f,NULL,io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void Game::ImguiUpdate(){
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // ImGui::ShowDemoWindow();
}

void Game::ImguiRander(){
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}