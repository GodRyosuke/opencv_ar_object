#pragma once

class Manager {
public:
    Manager();
    ~Manager();
    void Run();

private:
    bool Init();
    void Update();

    class Renderer* m_Renderer;
    bool m_IsRun;
};