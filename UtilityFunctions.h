#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#elif __linux__ || __APPLE__
#include <unistd.h>
#include <limits.h>
#endif

#undef max

namespace utility {
	namespace console {
		inline void setTitle(const std::string& title) {
	#if defined(_WIN32) || defined(_WIN64)
				// Determine the size of the wide string
				int wideSize = MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, nullptr, 0);
				std::wstring wideTitle(wideSize, 0);
				// Convert the UTF-8 string to a wide string
				MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, &wideTitle[0], wideSize);
				std::wstring bankSystemText = L"To-Do Application - ";
				std::wstring fullTitle = bankSystemText + wideTitle;

				// Use the wide string with the Windows API
				SetConsoleTitle(fullTitle.c_str());
	#endif
		}

		inline void clearInputBuffer() {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		}

		inline std::string getBaseDirectory() {
			std::string fullPath;

#ifdef _WIN32
			char buffer[MAX_PATH];
			GetModuleFileNameA(NULL, buffer, MAX_PATH);
			fullPath = std::string(buffer);
#elif __linux__ || __APPLE__
			char buffer[PATH_MAX];
			ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
			if (len != -1) {
				buffer[len] = '\0';
				fullPath = std::string(buffer);
			}
#endif

			// Remove the last three directories (specific to your structure)
			for (int i = 0; i < 3; ++i) {
				size_t pos = fullPath.find_last_of("/\\");
				if (pos != std::string::npos) {
					fullPath = fullPath.substr(0, pos);
				}
			}

			return fullPath; // Return the base directory
		}

		inline std::string getEntryFilePath() {
			return utility::console::getBaseDirectory() + "/Resources/Entries.json";
		}

		inline void clearScreen() {
#if defined(_WIN32) || defined(_WIN64)
			system("cls");
#elif defined(__linux__) || defined(__APPLE__)
			system("clear");
#else 
			std::cerr << "Unsupported Console/OS" << endl;
#endif
		}

	}

	namespace string {
		inline void convertToLowercase(std::string& str) {
			std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {return std::tolower(c); });
		}
		inline std::string toLowerCase(const std::string str) {
			std::string lowerStr = str;
			std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
			return lowerStr;
		}
		inline bool isNumber(const std::string& str) {
			return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
		}

		inline bool containsNumber(const std::string& str) {
			for (char c : str) {
				if (std::isdigit(c)) {
					return true;
				}
			}
			return false;
		}
		inline int convertStringToNum(const std::string& str) {
			try {
				int i = std::stoi(str);
				return i;
			}
			catch (const std::exception&) {
				std::cerr << "An exception occured when trying to convert string to integer\n";
			} catch (...) {
				std::cerr << "An unknown exception occured when trying to convert string to integer\n";
			}
			return NULL; // Failure
		}
	}
}