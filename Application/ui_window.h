#pragma once

#include <memory>
#include <string>
#include <deque>

#include "main_view.h"

class UIWindow
{
public:
    UIWindow() {}
    virtual ~UIWindow(){}

    bool Shown = false;
    std::string Name;

    void Show(MainView* view = nullptr);

    virtual void Update();
    virtual void Resize();

    virtual void Shutdown() {}

protected:
    virtual void OnShow(MainView* view) {}
};

constexpr char LogWindowName[] = "Log###RaylibLogWindow";

class LogWindow : public UIWindow
{
public:
    LogWindow();
    void OnShow(MainView* view) override;

private:
    std::deque<LogSink::LogItem> LogLines;
};