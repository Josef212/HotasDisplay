#include "stdafx.h"
#include "PainiteDataProvider.h"
#include "../HelperFuncs.h"

void PainiteDataProvider::Update()
{
	GetPainitePrice(m_data);
}

void PainiteDataProvider::OnScrollUpdated(int scroll)
{
}

std::string PainiteDataProvider::GetString(int page, int line)
{
	return (line >= 0 && line < m_data.size()) ? m_data[line] : std::string();
}
