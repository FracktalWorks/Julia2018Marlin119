#ifndef POWER_PANIC_UTIL_H
#define POWER_PANIC_UTIL_H

/*	
	char *ftostr31(const float &x);

	// Convert float to string with 123.4 format
	char *ftostr31ns(const float &x);


	// Convert float to string with 12345.678 format
	char *ftostr53(const float &x);
	
	// Convert float to string with 123.456 format
	char *ftostr33(const float &x);
*/	


	// Convert float to string with 123.4 format, signed
	void ftostr31s(float x, char c[8]);

	// Convert float to string with 123.4 format, unsigned
	void ftostr31u(float x, char c[8]);

	// Convert float to string with 123.456 format, signed
	void ftostr33s(float x, char c[8]);
	  
	// Convert float to string with 12345.678 format, signed
	void ftostr53s(float x, char c[8]);


	// fill command placeholder using float converter and value
	void fmtSaveLine(char *output, const char *placeholder, void (*converter)(float, char[10]), float value);
	
	
#endif