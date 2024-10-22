// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "MonoLibrary.h"

class CCompiledMonoLibrary final  : public CMonoLibrary
{
public:
	CCompiledMonoLibrary(const char* szDirectory, CMonoDomain* pDomain);

private:
	// CMonoLibrary
	virtual bool Load() override;
	virtual bool WasCompiledAtRuntime() override { return true; }
	// ~CMonoLibrary

	void FindSourceFilesInDirectoryRecursive(const char* szDirectory, std::vector<string>& sourceFiles);

	string m_directory;
};