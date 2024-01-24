#include "HsvKey.h"

namespace core {

HSVKey::Data::Data(): mEasing(), mHue(0), mSaturation(0), mValue(0), mAbsolute(0), mHSV{0, 0, 0, 0} {}

bool HSVKey::Data::isZero() const { return mHSV == QList<int>{0, 0, 0, 0}; }

void HSVKey::Data::clamp(const QString& type) {
    if (type == "hue" || type.isEmpty()) {
        mHue = util::MathUtil::getClamp(mHue, 0, 360);
    } else if (type == "sat" || type.isEmpty()) {
        mSaturation = util::MathUtil::getClamp(mSaturation, -100, 100);
    } else if (type == "val" || type.isEmpty()) {
        mValue = util::MathUtil::getClamp(mValue, -100, 100);
    }
}

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
