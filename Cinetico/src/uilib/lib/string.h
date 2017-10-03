
//String
// File: string.h
// Last Edit: 13/12/2014 02:31 (UTC-3)
// Author: CGR

#ifndef __STRING_H__
#define __STRING_H__

#include "..\\types.h"

namespace uilib {

	enum {
		ENC_SYSTEM,
		ENC_UTF8,
		ENC_UTF16
	};

	class string {

		char *m_buf;
		uint m_bufferSize;
		uint m_len;

	public:
		static s32 convert(u8 destencoding, u8 *pDest, u16 size, u8 srcencoding, const u8 *pSrc)
		{
			extern s32 system_copyconvertstr(u8 destencoding, u8 *pDest, u16 size, u8 srcencoding, const u8 *pSrc);
			return system_copyconvertstr(destencoding, pDest, size, srcencoding, pSrc);
		}

		string(const char *source);
		string(const string &source);
		string();
		~string();

		string& insert(int at, const string &str);
		string& insert(int at, const char *str, int count);
		string& insert(int at, char ch);
		string& append(const string &str);
		string& append(const char *str, int count);
		string& append(char ch);

		bool reserve(uint size);
		void resize(int size);

		string& operator=(const string &source);
		string& operator=(const char *source);
		string& operator+=(const string &source);
		string& operator+=(const char *source);
		string& operator+=(char ch);
		bool operator==(const string& source) const;
		bool operator==(const char *source) const;
		bool operator!=(const string& source) const;
		bool operator!=(const char *source) const;

		inline uint length() const { return m_len; }
		inline uint size() const { return m_len + 1; }
		inline uint bufferSize() const { return m_bufferSize; }
		inline char *data() { return m_buf; }
		inline const char* data() const { return (const char*)m_buf; }
		inline const char* constData() const { return (const char*)m_buf; }

		int toInteger() const;
		float toFloat() const;

		static string fromInteger(int n, int base = 10);
		static string fromFloat(float f, int precision = 6);
	};




	class string_op
	{
	public:
		static int copy(char *dest, const char *src, int count = -1);
		static int compare(const char *string1, const char *string2, int count = 0xffffffff, bool cs = true);
		static bool match(const char *string1, const char *string2, bool cs = true);
		static bool contains(const char *string1, const char *string2, bool cs = true);
		static uint length(const char *string);
		static int integer(const char *string);
		static float decimal(const char *string);
	};

}

#endif
