#ifndef GAME_RENDER_OBJECTS_H
#define GAME_RENDER_OBJECTS_H

#include <map>
#include <memory>
#include <string>

#include <osg/ref_ptr>

namespace osg
{
    class Group;
}

namespace osgUtil
{
    class IncrementalCompileOperation;
}

namespace Resource
{
    class ResourceSystem;
}

namespace MWWorld
{
    class Ptr;
    class CellStore;
}

namespace MWRender{

class Animation;


class Objects{
    typedef std::map<MWWorld::Ptr,Animation*> PtrAnimationMap;

    typedef std::map<const MWWorld::CellStore*, osg::ref_ptr<osg::Group> > CellMap;
    CellMap mCellSceneNodes;
    PtrAnimationMap mObjects;

    osg::ref_ptr<osg::Group> mRootNode;

    void insertBegin(const MWWorld::Ptr& ptr);

    Resource::ResourceSystem* mResourceSystem;

    osg::ref_ptr<osgUtil::IncrementalCompileOperation> mIncrementalCompileOperation;

public:
    Objects(Resource::ResourceSystem* resourceSystem, osg::ref_ptr<osg::Group> rootNode);
    ~Objects();

    void setIncrementalCompileOperation(osgUtil::IncrementalCompileOperation* ico);

    /// @param animated Attempt to load separate keyframes from a .kf file matching the model file?
    /// @param allowLight If false, no lights will be created, and particles systems will be removed.
    void insertModel(const MWWorld::Ptr& ptr, const std::string &model, bool animated=false, bool allowLight=true);

    void insertNPC(const MWWorld::Ptr& ptr);
    void insertCreature (const MWWorld::Ptr& ptr, const std::string& model, bool weaponsShields);

    Animation* getAnimation(const MWWorld::Ptr &ptr);

    //Ogre::AxisAlignedBox getDimensions(MWWorld::CellStore*);
    ///< get a bounding box that encloses all objects in the specified cell

    bool deleteObject (const MWWorld::Ptr& ptr);
    ///< \return found?

    void removeCell(const MWWorld::CellStore* store);

    /// Updates containing cell for object rendering data
    void updatePtr(const MWWorld::Ptr &old, const MWWorld::Ptr &cur);

private:
    void operator = (const Objects&);
    Objects(const Objects&);
};
}
#endif
