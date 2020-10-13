#pragma once

#include "IDisplayDataProvider.h"

class EmptyDataProvider final : public IDisplayDataProvider
{
public:
	EmptyDataProvider() = default;
	~EmptyDataProvider() = default;

	virtual bool CanResetScroll() const override { return true; }
	virtual void Update() override { }
	virtual void OnScrollUpdated(int scroll) override { }
	virtual std::string GetString(int page, int line) override { return std::string(); }
};

extern EmptyDataProvider DefaultDataProvider;