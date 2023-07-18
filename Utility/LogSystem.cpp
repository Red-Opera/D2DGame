// LogSystem.cpp

#include "stdafx.h"
#include "LogSystem.h"

using namespace std;

// private ����
namespace LogSystem
{
	TARGET target = TARGET::ALL;

	std::filesystem::path log_path = "./MyLog.txt";

	bool is_first = true;

	void Print(LPCSTR log);
}

// �Լ� ���Ǻ�
namespace LogSystem
{
	void SetTarget(const TARGET target) { LogSystem::target = target; }

	void SetLog(const std::string type, const std::string function, const std::string file, const std::string line, const std::string format, ...)
	{
		string log = "";
		va_list list = nullptr;

		va_start(list, format);
		{
			// ȭ�鿡 ����� ũ�⸦ ����
			size_t size = _vscprintf(format.c_str(), list) + sizeof('\0');
			
			char* message = new char[size];

			vsprintf_s(message, size, format.c_str(), list);

			// log�� ������ �Է�
			log += "CALLER\t:";
			log += function + " (";
			log += file + ", ";
			log += line + ")\n";
			log += type + "\t: ";

			log += string(message) + "\n\n";

			delete[] message;

		}
		va_end(list, format);

		Print(log.c_str());
	}

	void Print(LPCSTR log)
	{
		switch (target)
		{
		case LogSystem::TARGET::ALL:

		case LogSystem::TARGET::CONSOLE:
			printf("%s", log);

			if(target != TARGET::ALL) break;

		case LogSystem::TARGET::FILE:
		{
			ofstream file;

			if (is_first)
			{
				file.open(log_path, ios::out);
				is_first = false;
			}

			else file.open(log_path, ios::app);

			file << log;
			file.close();

			break;
		}

		default:
			break;
		}
	}
}