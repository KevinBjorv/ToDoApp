#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <limits>
#include <Windows.h>
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
			char buffer[MAX_PATH];
			GetModuleFileNameA(NULL, buffer, MAX_PATH);
			std::string fullPath(buffer);
			std::string::size_type pos = fullPath.find_last_of("\\/");

			// Remove the last directory (e.g, x64 or debug)
			fullPath = fullPath.substr(0, pos);
			pos = fullPath.find_last_of("\\/");

			// Remove the last directory (e.g, debug or PasswordManager)
			fullPath = fullPath.substr(0, pos);
			pos = fullPath.find_last_of("\\/");

			// Remove the last directory (e.g, PasswordManager)
			fullPath = fullPath.substr(0, pos);
			pos = fullPath.find_last_of("\\/");

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