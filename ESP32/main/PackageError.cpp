#include "PackageError.h"

PackageError::PackageError(String namePackage) {
    packageName = namePackage;
}

void PackageError::setText(String text) {
    packageContent = text;
}

void PackageError::addContent(String label, String value) {
    packageContent += label +":" + value + ",";
}

String PackageError::getText() {
    return packageContent;
}
