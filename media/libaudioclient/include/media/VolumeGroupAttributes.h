/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#pragma once

#include <android/media/AudioAttributesEx.h>
#include <media/AudioCommonTypes.h>
#include <media/AidlConversionUtil.h>
#include <system/audio.h>
#include <system/audio_policy.h>
#include <binder/Parcelable.h>

namespace android {

class VolumeGroupAttributes : public Parcelable
{
public:
    VolumeGroupAttributes() = default;
    VolumeGroupAttributes(const audio_attributes_t &attributes)
        : mAttributes(attributes) {} // NOLINT
    VolumeGroupAttributes(volume_group_t groupId,
                    audio_stream_type_t stream,
                    const audio_attributes_t &attributes) :
         mAttributes(attributes), mStreamType(stream), mGroupId(groupId) {
        // TODO: align native & JAVA source initializer.
        // As far as this class concerns attributes for volume group, it applies only to playback.
        mAttributes.source = AUDIO_SOURCE_INVALID;
    }

    int matchesScore(const audio_attributes_t &attributes) const;

    audio_attributes_t getAttributes() const { return mAttributes; }

    status_t readFromParcel(const Parcel *parcel) override;
    status_t writeToParcel(Parcel *parcel) const override;

    audio_stream_type_t getStreamType() const { return mStreamType; }
    volume_group_t getGroupId() const { return mGroupId; }

private:
    audio_attributes_t mAttributes = AUDIO_ATTRIBUTES_INITIALIZER;
    /**
     * @brief mStreamType: for legacy volume management, we need to be able to convert an attribute
     * to a given stream type.
     */
    audio_stream_type_t mStreamType = AUDIO_STREAM_DEFAULT;

    /**
     * @brief mGroupId: for future volume management, define groups within a strategy that follows
     * the same curves of volume (extension of stream types to manage volume)
     */
    volume_group_t mGroupId = VOLUME_GROUP_NONE;
};

// AIDL conversion routines.
ConversionResult<media::AudioAttributesEx>
legacy2aidl_VolumeGroupAttributes_AudioAttributesEx(const VolumeGroupAttributes& legacy);
ConversionResult<VolumeGroupAttributes>
aidl2legacy_AudioAttributesEx_VolumeGroupAttributes(const media::AudioAttributesEx& aidl);

} // namespace android
