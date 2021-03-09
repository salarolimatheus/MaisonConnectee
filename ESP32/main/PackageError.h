#ifndef PackageError_h
#define PackageError_h

#include "Arduino.h"

class PackageError {
	public:
        PackageError(String namePackage);
        void setText(String text);
        void addContent(String label, String value);
		String getText();
	private:
        String packageName = "";
        String packageContent = "";
};

#endif
