#ifndef PackageError_h
#define PackageError_h

#include "Arduino.h"

class PackageError {
	public:
        PackageError(String topic);
        void setText(String text);
        void addContent(String label, String value);
		String getText(void);
	private:
        String packageTopic = "";
        String packageContent = "";
};

class PackageDataBsec {
	public:
        PackageDataBsec(String topic);
        void getData(float staticIaq, uint8_t iaqAccuracy, float temperature, float humidity);
		String toString(void);
	private:
        String packageTopic = "";
		float _staticIaq, _temperature, _humidity;
		uint8_t _iaqAccuracy;
};

#endif
