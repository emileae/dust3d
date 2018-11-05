#ifndef DUST3D_POSE_DOCUMENT_H
#define DUST3D_POSE_DOCUMENT_H
#include <map>
#include <QString>
#include <deque>
#include "skeletondocument.h"
#include "rigger.h"

struct PoseHistoryItem
{
    std::map<QString, std::map<QString, QString>> parameters;
};

class PoseDocument : public SkeletonDocument
{
    Q_OBJECT
signals:
    void turnaroundChanged();
    void cleanup();
    void nodeAdded(QUuid nodeId);
    void edgeAdded(QUuid edgeId);
    void nodeOriginChanged(QUuid nodeId);
    void parametersChanged();
    
public:
    bool undoable() const override;
    bool redoable() const override;
    bool hasPastableNodesInClipboard() const override;
    bool originSettled() const override;
    bool isNodeEditable(QUuid nodeId) const override;
    bool isEdgeEditable(QUuid edgeId) const override;
    void copyNodes(std::set<QUuid> nodeIdSet) const override;
    
    void updateTurnaround(const QImage &image);
    void updateRigBones(const std::vector<RiggerBone> *rigBones, const QVector3D &rootTranslation=QVector3D(0, 0, 0));
    void reset();
    
    void toParameters(std::map<QString, std::map<QString, QString>> &parameters) const;
    void fromParameters(const std::vector<RiggerBone> *rigBones,
        const std::map<QString, std::map<QString, QString>> &parameters);
    
public slots:
    void saveHistoryItem();
    void undo() override;
    void redo() override;
    void paste() override;
    
    void moveNodeBy(QUuid nodeId, float x, float y, float z);
    void setNodeOrigin(QUuid nodeId, float x, float y, float z);
    float findGroundY() const;
    
public:
    static const float m_nodeRadius;
    static const float m_groundPlaneHalfThickness;
    
private:
    std::map<QString, std::pair<QUuid, QUuid>> m_boneNameToIdsMap;
    QUuid m_groundPartId;
    QUuid m_bonesPartId;
    QUuid m_groundEdgeId;
    std::deque<PoseHistoryItem> m_undoItems;
    std::deque<PoseHistoryItem> m_redoItems;
    std::vector<RiggerBone> m_riggerBones;
};

#endif