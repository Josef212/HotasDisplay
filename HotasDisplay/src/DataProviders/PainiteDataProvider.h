#pragma once

#include "IDisplayDataProvider.h"

#include <vector>
#include <string>

class PainiteDataProvider : public IDisplayDataProvider
{
public:
	PainiteDataProvider() = default;
	virtual ~PainiteDataProvider() = default;

	virtual unsigned long DelayMs() const override { return 10000; }
	virtual bool CanResetScroll() const override { return true; }
	virtual void Update() override;
	virtual void OnScrollUpdated(int scroll) override;
	virtual std::string GetString(int page, int line) override;

private:
	std::vector<std::string> m_data;
};
