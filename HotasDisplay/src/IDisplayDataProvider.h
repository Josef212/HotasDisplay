#pragma once

#include <string>

__interface IDisplayDataProvider
{
public:
	virtual unsigned long DelayMs() const = 0;
	virtual bool CanResetScroll() const = 0;
	virtual void Update() = 0;
	virtual void OnScrollUpdated(int scroll) = 0;
	virtual std::string GetString(int page, int line) = 0;
};
