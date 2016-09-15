#ifndef CORE_LAYERNODE_H
#define CORE_LAYERNODE_H

#include <QVector>
#include "XC.h"
#include "gl/EasyShaderProgram.h"
#include "gl/Texture.h"
#include "img/Buffer.h"
#include "img/ResourceHandle.h"
#include "core/ObjectNode.h"
#include "core/Renderer.h"
#include "core/GridMesh.h"
#include "core/HeightMap.h"
#include "core/TimeLine.h"
#include "core/BoneInfluenceMap.h"
#include "core/MeshTransformer.h"
#include "core/ShaderHolder.h"

namespace core
{

class LayerNode
        : public ObjectNode
        , public Renderer
{
public:
    LayerNode(const QString& aName, ShaderHolder& aShaderHolder);
    ~LayerNode();

    util::LifeLink::Pointee<LayerNode> pointee() { return lifeLink().pointee<LayerNode>(this); }

    // image buffer
    void setImage(const img::ResourceHandle& aHandle);
    void setImage(const img::ResourceHandle& aHandle, img::BlendMode aBlendMode);
    int imageWidth() const { return mImageHandle.image().width(); }
    int imageHeight() const { return mImageHandle.image().height(); }
    QSize imageSize() const { return mImageHandle.image().pixelSize(); }
    //uint8* imageData() { return mImageHandle.image().data(); }
    const uint8* imageData() const { return mImageHandle.image().data(); }

    // from ObjectNode
    virtual ObjectType type() const { return ObjectType_Layer; }
    virtual void setName(const QString& aName) { mName = aName; }
    virtual const QString& name() const { return mName; }
    virtual void setDepth(float aDepth) { mDepth = aDepth; }
    virtual float depth() const { return mDepth; }
    virtual void setVisibility(bool aIsVisible) { mIsVisible = aIsVisible; }
    virtual bool isVisible() const { return mIsVisible; }
    virtual bool canHoldChild() const { return false; }
    virtual QRect initialRect() const { return mImageRect; }
    virtual void setInitialCenter(const QVector2D& aCenter) { mInitialCenter = aCenter; }
    virtual QVector2D initialCenter() const { return mInitialCenter; }
    virtual Renderer* renderer() { return this; }
    virtual const Renderer* renderer() const { return this; }
    virtual TimeLine* timeLine() { return &mTimeLine; }
    virtual const TimeLine* timeLine() const { return &mTimeLine; }
    virtual GridMesh* gridMesh() { return &mGridMesh; }
    virtual const GridMesh* gridMesh() const { return &mGridMesh; }

    virtual cmnd::Vector createResourceUpdater(const ResourceEvent& aEvent);

    virtual bool serialize(Serializer& aOut) const;
    virtual bool deserialize(Deserializer& aIn);

    // from Renderer
    virtual void prerender(const RenderInfo&, const TimeCacheAccessor&);
    virtual void render(const RenderInfo&, const TimeCacheAccessor&);
    virtual void renderClipper(
            const RenderInfo&, const TimeCacheAccessor&, uint8 aClipperId);
    virtual void setRenderDepth(float aDepth) { mRenderDepth = aDepth; }
    virtual float renderDepth() const { return mRenderDepth; }
    virtual void setClipped(bool aIsClipped);
    virtual bool isClipped() const { return mIsClipped; }
    virtual bool hasBlendMode() const { return true; }
    virtual img::BlendMode blendMode() const { return mBlendMode; }
    virtual void setBlendMode(img::BlendMode);

private:
    void clear();
    void readImageData(const img::Buffer& aBuffer, const QPoint& aPos);
    void transformShape(const RenderInfo& aInfo, const TimeCacheAccessor&);
    void renderShape(const RenderInfo& aInfo, const TimeCacheAccessor&);
    void renderClippees(const RenderInfo& aInfo, const TimeCacheAccessor&);
    bool isClipper() const;

    QString mName;
    float mDepth;
    bool mIsVisible;
    QRect mImageRect;
    QVector2D mInitialCenter;
    img::ResourceHandle mImageHandle;
    img::BlendMode mBlendMode;
    GridMesh mGridMesh;
    TimeLine mTimeLine;
    ShaderHolder& mShaderHolder;
    bool mIsClipped;

    gl::Texture mTexture;
    MeshTransformer mMeshTransformer;
    float mRenderDepth;
    LayerMesh* mCurrentMesh;
    std::vector<Renderer*> mClippees; // a cache for performance
};

} // namespace core

#endif // CORE_LAYERNODE_H