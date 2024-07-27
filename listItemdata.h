#ifndef LISTITEMDATA_H
#define LISTITEMDATA_H

#include <QMetaType>

typedef struct {
    QString iconPath;
    QString singer;
    QString songsNb;
} MuItemData;

Q_DECLARE_METATYPE(MuItemData)

#endif // LISTITEMDATA_H
