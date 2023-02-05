#ifndef HSVKEY_H
#define HSVKEY_H

#include "util/Easing.h"
#include "core/TimeKey.h"
#include "util/MathUtil.h"

namespace core
{

class HSVKey : public TimeKey
{
public:
    class Data
    {
        util::Easing::Param mEasing;
        int mHue;
        int mSaturation;
        int mValue;
        QList<int> mHSV;
        void clamp(QString type);
    public:
        Data();

        util::Easing::Param& easing() { return mEasing; }
        const util::Easing::Param& easing() const { return mEasing; }

        void setHue(int aHue) { mHue = aHue; clamp("hue"); }
        void setSaturation(int aSaturation) {mSaturation = aSaturation; clamp("sat");}
        void setValue(int aValue) {mValue = aValue; clamp("val");}
        void setHSV(QList<int> aHSV) {mHue = aHSV[0]; mSaturation = aHSV[1]; mValue = aHSV[2]; clamp("hsv");}
        void updateHSV() {mHSV.clear(); mHSV = {mHue, mSaturation, mValue};}

        const QList<int>& hsv() const { return mHSV; }

        bool isZero() const;
    };

    HSVKey();

    Data& data() { return mData; }
    const Data& data() const { return mData; }

    void setHue(int aHue) { mData.setHue(aHue);}
    void setSaturation(int aSaturation) {mData.setSaturation(aSaturation);}
    void setValue(int aValue) {mData.setValue(aValue);}
    void setHSV (QList<int> aHSV) {mData.setHue(aHSV[0]); mData.setSaturation(aHSV[1]), mData.setValue(aHSV[2]);}
    void updateHSV() {mData.updateHSV();}

    const QList<int>& hsv() const { return mData.hsv(); }

    virtual TimeKeyType type() const { return TimeKeyType_HSV; }
    virtual TimeKey* createClone();
    virtual bool serialize(Serializer& aOut) const;
    virtual bool deserialize(Deserializer& aIn);

private:
    Data mData;
};

} // namespace core
#endif // HSVKEY_H
