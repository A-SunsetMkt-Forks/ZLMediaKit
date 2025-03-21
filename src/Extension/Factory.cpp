﻿/*
 * Copyright (c) 2016-present The ZLMediaKit project authors. All Rights Reserved.
 *
 * This file is part of ZLMediaKit(https://github.com/ZLMediaKit/ZLMediaKit).
 *
 * Use of this source code is governed by MIT-like license that can be found in the
 * LICENSE file in the root of the source tree. All contributing project authors
 * may be found in the AUTHORS file in the root of the source tree.
 */

#include "Factory.h"
#include "Rtmp/Rtmp.h"
#include "CommonRtmp.h"
#include "CommonRtp.h"
#include "Common/config.h"

using namespace std;
using namespace toolkit;

namespace mediakit {

static std::unordered_map<int, const CodecPlugin *> s_plugins;

REGISTER_CODEC(h264_plugin);
REGISTER_CODEC(h265_plugin);
REGISTER_CODEC(jpeg_plugin);
REGISTER_CODEC(aac_plugin);
REGISTER_CODEC(opus_plugin);
REGISTER_CODEC(g711a_plugin)
REGISTER_CODEC(g711u_plugin);
REGISTER_CODEC(l16_plugin);
REGISTER_CODEC(mp3_plugin);

void Factory::registerPlugin(const CodecPlugin &plugin) {
    InfoL << "Load codec: " << getCodecName(plugin.getCodec());
    s_plugins[(int)(plugin.getCodec())] = &plugin;
}

Track::Ptr Factory::getTrackBySdp(const SdpTrack::Ptr &track) {
    auto codec = getCodecId(track->_codec);
    if (codec == CodecInvalid) {
        // 根据传统的payload type 获取编码类型以及采样率等信息  [AUTO-TRANSLATED:d01ca068]
        // Get the encoding type, sampling rate, and other information based on the traditional payload type
        codec = RtpPayload::getCodecId(track->_pt);
    }
    auto it = s_plugins.find(codec);
    if (it == s_plugins.end()) {
        return getTrackByCodecId(codec, track->_samplerate, track->_channel);
    }
    return it->second->getTrackBySdp(track);
}

Track::Ptr Factory::getTrackByAbstractTrack(const Track::Ptr &track) {
    auto codec = track->getCodecId();
    if (track->getTrackType() == TrackVideo) {
        return getTrackByCodecId(codec);
    }
    auto audio_track = dynamic_pointer_cast<AudioTrack>(track);
    return getTrackByCodecId(codec, audio_track->getAudioSampleRate(), audio_track->getAudioChannel(), audio_track->getAudioSampleBit());
}

RtpCodec::Ptr Factory::getRtpEncoderByCodecId(CodecId codec, uint8_t pt) {
    auto it = s_plugins.find(codec);
    if (it == s_plugins.end()) {
        WarnL << "Unsupported codec: " << getCodecName(codec) << ", use CommonRtpEncoder";
        return std::make_shared<CommonRtpEncoder>();
    }
    return it->second->getRtpEncoderByCodecId(pt);
}

RtpCodec::Ptr Factory::getRtpDecoderByCodecId(CodecId codec) {
    auto it = s_plugins.find(codec);
    if (it == s_plugins.end()) {
        WarnL << "Unsupported codec: " << getCodecName(codec) << ", use CommonRtpDecoder";
        return std::make_shared<CommonRtpDecoder>(codec, 10 * 1024);
    }
    return it->second->getRtpDecoderByCodecId();
}

// ///////////////////////////rtmp相关///////////////////////////////////////////  [AUTO-TRANSLATED:da9645df]
// ///////////////////////////rtmp related///////////////////////////////////////////

static CodecId getVideoCodecIdByAmf(const AMFValue &val) {
    if (val.type() == AMF_STRING) {
        auto str = val.as_string();
        if (str == "avc1") {
            return CodecH264;
        }
        if (str == "hev1" || str == "hvc1") {
            return CodecH265;
        }
        WarnL << "Unsupported codec: " << str;
        return CodecInvalid;
    }

    if (val.type() != AMF_NULL) {
        auto type_id = (RtmpVideoCodec)val.as_integer();
        switch (type_id) {
            case RtmpVideoCodec::h264: return CodecH264;
            case RtmpVideoCodec::fourcc_hevc:
            case RtmpVideoCodec::h265: return CodecH265;
            case RtmpVideoCodec::fourcc_av1: return CodecAV1;
            case RtmpVideoCodec::fourcc_vp9: return CodecVP9;
            default: WarnL << "Unsupported codec: " << (int)type_id; return CodecInvalid;
        }
    }
    return CodecInvalid;
}

Track::Ptr Factory::getTrackByCodecId(CodecId codec, int sample_rate, int channels, int sample_bit) {
    auto it = s_plugins.find(codec);
    if (it == s_plugins.end()) {
        auto type = mediakit::getTrackType(codec);
        switch (type) {
            case TrackAudio: {
                WarnL << "Unsupported codec: " << getCodecName(codec) << ", use default audio track";
                return std::make_shared<AudioTrackImp>(codec, sample_rate, channels, sample_bit);
            }
            case TrackVideo: {
                WarnL << "Unsupported codec: " << getCodecName(codec) << ", use default video track";
                return std::make_shared<VideoTrackImp>(codec, 0, 0, 0);
            }
            default: WarnL << "Unsupported codec: " << getCodecName(codec); return nullptr;
        }
    }
    return it->second->getTrackByCodecId(sample_rate, channels, sample_bit);
}

Track::Ptr Factory::getVideoTrackByAmf(const AMFValue &amf) {
    CodecId codecId = getVideoCodecIdByAmf(amf);
    if (codecId == CodecInvalid) {
        return nullptr;
    }
    return getTrackByCodecId(codecId);
}

static CodecId getAudioCodecIdByAmf(const AMFValue &val) {
    if (val.type() == AMF_STRING) {
        auto str = val.as_string();
        if (str == "mp4a") {
            return CodecAAC;
        }
        WarnL << "Unsupported codec: " << str;
        return CodecInvalid;
    }

    if (val.type() != AMF_NULL) {
        auto type_id = (RtmpAudioCodec)val.as_integer();
        switch (type_id) {
            case RtmpAudioCodec::aac: return CodecAAC;
            case RtmpAudioCodec::mp3: return CodecMP3;
            case RtmpAudioCodec::adpcm: return CodecADPCM;
            case RtmpAudioCodec::g711a: return CodecG711A;
            case RtmpAudioCodec::g711u: return CodecG711U;
            case RtmpAudioCodec::opus: return CodecOpus;
            default: WarnL << "Unsupported codec: " << (int)type_id; return CodecInvalid;
        }
    }
    return CodecInvalid;
}

Track::Ptr Factory::getAudioTrackByAmf(const AMFValue &amf, int sample_rate, int channels, int sample_bit) {
    CodecId codecId = getAudioCodecIdByAmf(amf);
    if (codecId == CodecInvalid) {
        return nullptr;
    }
    return getTrackByCodecId(codecId, sample_rate, channels, sample_bit);
}

RtmpCodec::Ptr Factory::getRtmpDecoderByTrack(const Track::Ptr &track) {
    auto it = s_plugins.find(track->getCodecId());
    if (it == s_plugins.end()) {
        WarnL << "Unsupported codec: " << track->getCodecName() << ", use CommonRtmpDecoder";
        return std::make_shared<CommonRtmpDecoder>(track);
    }
    return it->second->getRtmpDecoderByTrack(track);
}

RtmpCodec::Ptr Factory::getRtmpEncoderByTrack(const Track::Ptr &track) {
    auto it = s_plugins.find(track->getCodecId());
    if (it == s_plugins.end()) {
        auto amf = Factory::getAmfByCodecId(track->getCodecId());
        WarnL << "Unsupported codec: " << track->getCodecName() << (amf ? ", use CommonRtmpEncoder" : "");
        return amf ? std::make_shared<CommonRtmpEncoder>(track) : nullptr;
    }
    return it->second->getRtmpEncoderByTrack(track);
}

AMFValue Factory::getAmfByCodecId(CodecId codecId) {
    GET_CONFIG(bool, enhanced, Rtmp::kEnhanced);
    switch (codecId) {
        case CodecAAC: return AMFValue((int)RtmpAudioCodec::aac);
        case CodecH264: return AMFValue((int)RtmpVideoCodec::h264);
        case CodecH265: return enhanced ? AMFValue((int)RtmpVideoCodec::fourcc_hevc) : AMFValue((int)RtmpVideoCodec::h265);
        case CodecG711A: return AMFValue((int)RtmpAudioCodec::g711a);
        case CodecG711U: return AMFValue((int)RtmpAudioCodec::g711u);
        case CodecOpus: return AMFValue((int)RtmpAudioCodec::opus);
        case CodecADPCM: return AMFValue((int)RtmpAudioCodec::adpcm);
        case CodecMP3: return AMFValue((int)RtmpAudioCodec::mp3);
        case CodecAV1: return AMFValue((int)RtmpVideoCodec::fourcc_av1);
        case CodecVP9: return AMFValue((int)RtmpVideoCodec::fourcc_vp9);
        default: return AMFValue(AMF_NULL);
    }
}

Frame::Ptr Factory::getFrameFromPtr(CodecId codec, const char *data, size_t bytes, uint64_t dts, uint64_t pts) {
    auto it = s_plugins.find(codec);
    if (it == s_plugins.end()) {
        // 创建不支持codec的frame  [AUTO-TRANSLATED:00936c6c]
        // Create a frame that does not support the codec
        return std::make_shared<FrameFromPtr>(codec, (char *)data, bytes, dts, pts);
    }
    return it->second->getFrameFromPtr(data, bytes, dts, pts);
}

Frame::Ptr Factory::getFrameFromBuffer(CodecId codec, Buffer::Ptr data, uint64_t dts, uint64_t pts) {
    auto frame = Factory::getFrameFromPtr(codec, data->data(), data->size(), dts, pts);
    if (!frame) {
        return nullptr;
    }
    return std::make_shared<FrameCacheAble>(frame, false, std::move(data));
}

} // namespace mediakit
