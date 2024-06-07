#include "HsvKey.h"

namespace core {

HSVKey::Data::Data(): mEasing(), mHue(0), mSaturation(100), mValue(100), mAbsolute(0), mHSV{0, 100, 100, 0} {}

bool HSVKey::Data::isZero() const { return mHSV == QList<int>{0, 0, 0, 0}; }

HSVKey::HSVKey(): mData() {}

TimeKey* HSVKey::createClone() {
    auto newKey = new HSVKey();
    newKey->mData = this->mData;
    return newKey;
}

bool HSVKey::serialize(Serializer& aOut) const {
    aOut.write(mData.easing());
    aOut.write(mData.hsv());
    return aOut.checkStream();
}

bool HSVKey::deserialize(Deserializer& aIn) {
    aIn.pushLogScope("HSVKey");

    if (!aIn.read(mData.easing())) {
        return aIn.errored("invalid easing param");
    }

    QList<int> hsv = data().hsv();
    aIn.read(hsv);
    mData.setHSV(hsv);

    aIn.popLogScope();
    return aIn.checkStream();
}

} // namespace core
