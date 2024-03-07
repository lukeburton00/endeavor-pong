#include <iostream>
#include "endeavor/endeavor.hpp"

struct Player
{
    Endeavor::Sprite sprite;
    glm::vec2 position;

    Player()
    {
        sprite.scale = glm::vec2(25.f, 150.f);
        position = glm::vec2(0.f, 0.f);
    }
};

struct Ball
{
    Endeavor::Sprite sprite;
    glm::vec2 position;
    glm::vec2 velocity;

    Ball()
    {
        sprite.scale = glm::vec2(10.f, 10.f);
        position = glm::vec2(0.f, 0.f);
        velocity = glm::vec2(0.f, 0.f);
    }
};

class TestGame : public Endeavor::Game
{
private:
    Endeavor::Camera m_Camera;
    Player m_Player_One;
    Player m_Player_Two;
    Ball m_Ball;

public:
    virtual void Init() override
    {
        m_Window.GetProperties().title = "Pong";

        m_Player_One.position = glm::vec2(-300.f, 0.f);
        m_Player_Two.position = glm::vec2(300.f, 0.f);

        if (Endeavor::Random::randomIntInRange(0, 1))
        {
            m_Ball.velocity.x = 2;
        }

        else
        {
            m_Ball.velocity.x = -2;
        }
    }

    virtual void Update() override
    {
        ProcessInput();
        m_Ball.position += m_Ball.velocity;
        CheckBounds();
    }

    void ProcessInput()
    {
        m_Camera.Update();

        // Player One
        if (Endeavor::Input::IsKeyPressed(Endeavor::KeyCode::W))
        {
            m_Player_One.position.y -= 3;
        }


        if (Endeavor::Input::IsKeyPressed(Endeavor::KeyCode::S))
        {
            m_Player_One.position.y += 3;
        }

        // Player Two
        if (Endeavor::Input::IsKeyPressed(Endeavor::KeyCode::Up))
        {
            m_Player_Two.position.y -= 3;
        }

        if (Endeavor::Input::IsKeyPressed(Endeavor::KeyCode::Down))
        {
            m_Player_Two.position.y += 3;
        }
    }

    void CheckBounds()
    {
        // Player One
        if (m_Player_One.position.y > 150)
        {
            m_Player_One.position.y = 150;
        }

        if (m_Player_One.position.y < -300)
        {
            m_Player_One.position.y = -300;
        }        
        
        // Player Two
        if (m_Player_Two.position.y > 150)
        {
            m_Player_Two.position.y = 150;
        }

        if (m_Player_Two.position.y < -300)
        {
            m_Player_Two.position.y = -300;
        }

        // Ball
        if (m_Ball.position.y > 290)
        {
            m_Ball.position.y = 290;
            m_Ball.velocity.y *= -1;
        }

        if (m_Ball.position.y < -300)
        {
            m_Ball.position.y = -300;
            m_Ball.velocity.y *= -1;
        }

        bool isCollidingWithPlayerOne = 
            m_Ball.position.x <= m_Player_One.position.x + m_Player_One.sprite.scale.x 
            && m_Ball.position.x > m_Player_One.position.x
            && m_Ball.position.y >= m_Player_One.position.y 
            && m_Ball.position.y <= m_Player_One.position.y + m_Player_One.sprite.scale.y;

        bool isCollidingWithPlayerTwo = 
            m_Ball.position.x + m_Ball.sprite.scale.x >= m_Player_Two.position.x 
            && m_Ball.position.x < m_Player_Two.position.x + m_Player_Two.sprite.scale.x
            && m_Ball.position.y >= m_Player_Two.position.y && m_Ball.position.y <= m_Player_Two.position.y + m_Player_Two.sprite.scale.y;

        if (isCollidingWithPlayerOne)
        {
            m_Ball.velocity.x -= 0.2;
            m_Ball.velocity.x *= -1;
            m_Ball.velocity.y = ((m_Ball.position.y + (m_Ball.sprite.scale.y / 2)) - (m_Player_One.position.y + (m_Player_One.sprite.scale.y / 2))) / 100;
        }

        if (isCollidingWithPlayerTwo)
        {
            m_Ball.velocity.x += 0.2;
            m_Ball.velocity.x *= -1;
            m_Ball.velocity.y = ((m_Ball.position.y + (m_Ball.sprite.scale.y / 2)) - (m_Player_Two.position.y + (m_Player_Two.sprite.scale.y / 2))) / 100;
        }
    }

    virtual void Render() override
    {
        auto renderer = Endeavor::Renderer::GetInstance();
        renderer->Begin(m_Camera);
        renderer->RenderSprite(m_Player_One.position, m_Player_One.sprite);
        renderer->RenderSprite(m_Player_Two.position, m_Player_Two.sprite);
        renderer->RenderSprite(m_Ball.position, m_Ball.sprite);
        renderer->Draw();
    }
};

Endeavor::Game* Endeavor::CreateGame()
{
    return new TestGame();
}