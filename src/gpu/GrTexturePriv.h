/*
 * Copyright 2014 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrTexturePriv_DEFINED
#define GrTexturePriv_DEFINED

#include "GrTexture.h"

/** Class that adds methods to GrTexture that are only intended for use internal to Skia.
    This class is purely a privileged window into GrTexture. It should never have additional data
    members or virtual methods.
    Non-static methods that are not trivial inlines should be spring-boarded (e.g. declared and
    implemented privately in GrTexture with a inline public method here). */
class GrTexturePriv {
public:
    void markMipMapsDirty() {
        fTexture->markMipMapsDirty();
    }

    void markMipMapsClean() {
        fTexture->markMipMapsClean();
    }

    bool mipMapsAreDirty() const {
        return GrMipMapsStatus::kValid != fTexture->fMipMapsStatus;
    }

    bool hasMipMaps() const {
        return GrMipMapsStatus::kNotAllocated != fTexture->fMipMapsStatus;
    }

    void setMaxMipMapLevel(int maxMipMapLevel) const {
        fTexture->fMaxMipMapLevel = maxMipMapLevel;
    }

    int maxMipMapLevel() const {
        return fTexture->fMaxMipMapLevel;
    }

    GrSLType imageStorageType() const {
        if (GrPixelConfigIsSint(fTexture->config())) {
            return kIImageStorage2D_GrSLType;
        } else {
            return kImageStorage2D_GrSLType;
        }
    }

    GrSLType samplerType() const { return fTexture->fSamplerType; }

    /** The filter used is clamped to this value in GrProcessor::TextureSampler. */
    GrSamplerState::Filter highestFilterMode() const { return fTexture->fHighestFilterMode; }

    void setMipColorMode(SkDestinationSurfaceColorMode colorMode) const {
        fTexture->fMipColorMode = colorMode;
    }
    SkDestinationSurfaceColorMode mipColorMode() const { return fTexture->fMipColorMode; }

    static void ComputeScratchKey(const GrSurfaceDesc&, GrScratchKey*);
    static void ComputeScratchKey(GrPixelConfig config, int width, int height,
                                  bool isRenderTarget, int sampleCnt,
                                  bool isMipMapped, GrScratchKey* key);


private:
    GrTexturePriv(GrTexture* texture) : fTexture(texture) { }
    GrTexturePriv(const GrTexturePriv& that) : fTexture(that.fTexture) { }
    GrTexturePriv& operator=(const GrTexturePriv&); // unimpl

    // No taking addresses of this type.
    const GrTexturePriv* operator&() const;
    GrTexturePriv* operator&();

    GrTexture* fTexture;

    friend class GrTexture; // to construct/copy this type.
};

inline GrTexturePriv GrTexture::texturePriv() { return GrTexturePriv(this); }

inline const GrTexturePriv GrTexture::texturePriv () const {
    return GrTexturePriv(const_cast<GrTexture*>(this));
}

#endif