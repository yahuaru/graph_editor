#ifndef GRAPHITEM_H
#define GRAPHITEM_H
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QString>
#include <QPainter>
#include <QGraphicsScene>
#include <QFile>
#include <QTreeView>
#include <QStandardItemModel>
#include <QFileSystemModel>
#include <QMessageBox>
#include <QDebug>
#include <QList>
#include <QVariant>

class GraphEdge;

class GraphItem: public QGraphicsRectItem
{
public:
    GraphItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem* parent = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setFile(QModelIndex index, QFileSystemModel *model);
    QString fileName();

    void addOutEdge(GraphEdge* edge);
    void addInEdge(GraphEdge* edge);

    void eraseEdges();

    QStandardItemModel* model();

    //Selection events
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QRectF boundingRect() const;


    friend QDebug operator<< (QDebug d, GraphItem &item);
protected:

private:
    QStandardItemModel *propModel;
    QGraphicsTextItem *nameText;

    QModelIndex fileIndex;
    QFileSystemModel *fileModel;
    
    QList<GraphEdge *> outEdges;
    QList<GraphEdge *> inEdges;
};

#endif // GRAPHITEM_H
