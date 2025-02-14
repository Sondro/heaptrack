/*
 * Copyright 2016-2017 Milian Wolff <mail@milianw.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef SUMMARYDATA_H
#define SUMMARYDATA_H

#include "../allocationdata.h"
#include "../filterparameters.h"
#include "../suppressions.h"

#include <QMetaType>
#include <QString>
#include <QVector>

struct SummaryData
{
    SummaryData() = default;
    SummaryData(const QString& debuggee, const AllocationData& cost, int64_t totalTime,
                const FilterParameters& filterParameters, int64_t peakTime, int64_t peakRSS, int64_t totalSystemMemory,
                bool fromAttached, int64_t totalLeakedSuppressed, QVector<Suppression> suppressions)
        : debuggee(debuggee)
        , cost(cost)
        , totalLeakedSuppressed(totalLeakedSuppressed)
        , totalTime(totalTime)
        , filterParameters(filterParameters)
        , peakTime(peakTime)
        , peakRSS(peakRSS)
        , totalSystemMemory(totalSystemMemory)
        , fromAttached(fromAttached)
        , suppressions(std::move(suppressions))
    {
    }
    QString debuggee;
    AllocationData cost;
    int64_t totalLeakedSuppressed = 0;
    int64_t totalTime = 0;
    FilterParameters filterParameters;
    int64_t peakTime = 0;
    int64_t peakRSS = 0;
    int64_t totalSystemMemory = 0;
    bool fromAttached = false;
    QVector<Suppression> suppressions;
};
Q_DECLARE_METATYPE(SummaryData)

#endif // SUMMARYDATA_H
