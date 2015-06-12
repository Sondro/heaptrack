/*
 * Copyright 2015 Milian Wolff <mail@milianw.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef MODEL_H
#define MODEL_H

#include <QAbstractItemModel>
#include <QVector>

struct LocationData
{
    QString function;
    QString file;
    QString module;
    bool operator==(const LocationData& rhs) const
    {
        return function == rhs.function
            && file == rhs.file
            && module == rhs.module;
    }
    bool operator<(const LocationData& rhs) const
    {
        int i = function.compare(rhs.function);
        if (!i) {
            i = file.compare(rhs.file);
        }
        if (!i) {
            i = module.compare(rhs.module);
        }
        return i < 0;
    }
};

struct RowData
{
    quint64 allocations;
    quint64 peak;
    quint64 leaked;
    quint64 allocated;
    LocationData location;
    const RowData* parent;
    QVector<RowData> children;
    bool operator<(const LocationData& rhs) const
    {
        return location < rhs;
    }
};

Q_DECLARE_TYPEINFO(RowData, Q_MOVABLE_TYPE);

Q_DECLARE_METATYPE(QVector<RowData>)

class Model : public QAbstractItemModel
{
    Q_OBJECT
public:
    Model(QObject* parent);
    virtual ~Model();

    enum Columns {
        AllocationsColumn,
        PeakColumn,
        LeakedColumn,
        AllocatedColumn,
        FunctionColumn,
        FileColumn,
        ModuleColumn,
        NUM_COLUMNS
    };

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    void loadFile(const QString& path);

signals:
    void dataReady(const QString& summary);

    /// emitted from the background for message passing into the foreground
    void dataReadyBackground(const QVector<RowData>& data, const QString& summary);

private:
    /// called in the main thread to actually reset the data of this model and notify views
    void dataReadyForeground(const QVector<RowData>& data, const QString& summary);

    /// @return the row resembled by @p index
    const RowData* toRow(const QModelIndex& index) const;
    /// @return the parent row containing @p index
    const RowData* toParentRow(const QModelIndex& index) const;
    /// @return the row number of @p row in its parent
    int rowOf(const RowData* row) const;

    QVector<RowData> m_data;
};

#endif // MODEL_H

