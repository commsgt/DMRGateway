/*
*   Copyright (C) 2016,2017 by Jonathan Naylor G4KLX
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "Reflectors.h"
#include "Log.h"

#include <algorithm>
#include <functional>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <cctype>

CReflectors::CReflectors(const std::string& hostsFile) :
m_hostsFile(hostsFile),
m_reflectors()
{
}

CReflectors::~CReflectors()
{
	for (std::vector<CReflector*>::iterator it = m_reflectors.begin(); it != m_reflectors.end(); ++it)
		delete *it;

	m_reflectors.clear();
}

bool CReflectors::load()
{
	for (std::vector<CReflector*>::iterator it = m_reflectors.begin(); it != m_reflectors.end(); ++it)
		delete *it;

	m_reflectors.clear();

	FILE* fp = ::fopen(m_hostsFile.c_str(), "rt");
	if (fp != NULL) {
		char buffer[100U];
		while (::fgets(buffer, 100U, fp) != NULL) {
			if (buffer[0U] == '#')
				continue;

			char* p1 = ::strtok(buffer, ";\r\n");
			char* p2 = ::strtok(NULL, ";\r\n");
			char* p3 = ::strtok(NULL, ";\r\n");
			char* p4 = ::strtok(NULL, ";\r\n");
			char* p5 = ::strtok(NULL, "\r\n");

			if (p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL && p5 != NULL) {
				CReflector* refl = new CReflector;
				refl->m_id       = (unsigned int)::atoi(p1);
				refl->m_address  = std::string(p2);
				refl->m_port     = (unsigned int)::atoi(p3);
				refl->m_startup  = (unsigned int)::atoi(p4);
				refl->m_password = std::string(p5);
				m_reflectors.push_back(refl);
			}
		}

		::fclose(fp);
	}

	size_t size = m_reflectors.size();
	LogInfo("Loaded %u XLX reflectors", size);

	size = m_reflectors.size();
	if (size == 0U)
		return false;

	return true;
}

CReflector* CReflectors::find(unsigned int id)
{
	for (std::vector<CReflector*>::iterator it = m_reflectors.begin(); it != m_reflectors.end(); ++it) {
		if (id == (*it)->m_id)
			return *it;
	}

	LogMessage("Trying to find non existent XLX reflector with an id of %u", id);

	return NULL;
}
