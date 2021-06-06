#pragma once

#include <memory>
#include <string>
#include <deque>

class UIWindow
{
public:
    UIWindow() {}
    virtual ~UIWindow(){}

    bool Shown = false;
    std::string Name;

    void Show();

    virtual void Update();
    virtual void Resize();

protected:
    virtual void OnShow() {}
};

constexpr char LogWindowName[] = "Log###RaylibLogWindow";

class LogWindow : public UIWindow
{
public:
    LogWindow();
    void OnShow() override;

private:
    std::deque<LogSink::LogItem> LogLines;
};