#pragma once

#include "IDisplayDataProvider.h"

#include <vector>
#include <string>

class PainiteDataProvider : public IDisplayDataProvider
{
public:
	PainiteDataProvider() = default;
	virtual ~PainiteDataProvider() = default;

	virtual bool CanResetScroll() const override { return true; }
	virtual void Update();
	virtual void OnScrollUpdated(int scroll) override;
	virtual std::string GetString(int page, int line) override;

private:
	std::vector<std::string> m_data;
};
