/*
* Copyright (c) 2019 ChangSha JuSong Soft Inc. <service@pinusdb.cn>.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; version 3 of the License.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program; If not, see <http://www.gnu.org/licenses>
*/

#include "storage/data_part.h"

PdbErr_t DataPart::QueryAsc(const std::list<int64_t>& devIdList,
  const TableInfo* pTabInfo, IQuery* pQuery, uint64_t timeOut)
{
  PdbErr_t retVal = PdbE_OK;
  int64_t bgTs, edTs;
  pQuery->GetTstampRange(&bgTs, &edTs);

  if (bgTs >= edDayTs_ || edTs < bgDayTs_)
    return PdbE_OK;

  void* pQueryParam = InitQueryParam(pTabInfo, bgTs, edTs);
  if (pQueryParam == nullptr)
    return PdbE_NOMEM;

  for (auto devIt = devIdList.begin(); devIt != devIdList.end(); devIt++)
  {
    retVal = QueryDevAsc(*devIt, pQueryParam, pQuery, timeOut, false, nullptr);
    if (retVal != PdbE_OK)
      break;

    if (pQuery->GetIsFullFlag())
      break;
  }

  ClearQueryParam(pQueryParam);
  return retVal;
}

PdbErr_t DataPart::QueryDesc(const std::list<int64_t>& devIdList,
  const TableInfo* pTabInfo, IQuery* pQuery, uint64_t timeOut)
{
  PdbErr_t retVal = PdbE_OK;
  int64_t bgTs, edTs;
  pQuery->GetTstampRange(&bgTs, &edTs);

  if (bgTs >= edDayTs_ || edTs < bgDayTs_)
    return PdbE_OK;

  void* pQueryParam = InitQueryParam(pTabInfo, bgTs, edTs);
  if (pQueryParam == nullptr)
    return PdbE_NOMEM;

  for (auto devIt = devIdList.begin(); devIt != devIdList.end(); devIt++)
  {
    retVal = QueryDevDesc(*devIt, pQueryParam, pQuery, timeOut, false, nullptr);
    if (retVal != PdbE_OK)
      break;

    if (pQuery->GetIsFullFlag())
      break;
  }

  ClearQueryParam(pQueryParam);
  return retVal;
}

PdbErr_t DataPart::QueryFirst(std::list<int64_t>& devIdList,
  const TableInfo* pTabInfo, IQuery* pQuery, uint64_t timeOut)
{
  PdbErr_t retVal = PdbE_OK;
  bool isAdd = false;
  int64_t bgTs, edTs;
  pQuery->GetTstampRange(&bgTs, &edTs);

  if (bgTs >= edDayTs_ || edTs < bgDayTs_)
    return PdbE_OK;

  void* pQueryParam = InitQueryParam(pTabInfo, bgTs, edTs);
  if (pQueryParam == nullptr)
    return PdbE_NOMEM;

  for (auto devIt = devIdList.begin(); devIt != devIdList.end(); )
  {
    isAdd = false;
    retVal = QueryDevAsc(*devIt, pQueryParam, pQuery, timeOut, true, &isAdd);
    if (retVal != PdbE_OK)
      break;

    if (isAdd)
    {
      devIt = devIdList.erase(devIt);
    }
    else
    {
      devIt++;
    }
  }

  ClearQueryParam(pQueryParam);
  return retVal;
}

PdbErr_t DataPart::QueryLast(std::list<int64_t>& devIdList,
  const TableInfo* pTabInfo, IQuery* pQuery, uint64_t timeOut)
{
  PdbErr_t retVal = PdbE_OK;
  bool isAdd = false;
  int64_t bgTs, edTs;
  pQuery->GetTstampRange(&bgTs, &edTs);
  
  if (bgTs >= edDayTs_ || edTs < bgDayTs_)
    return PdbE_OK;

  void* pQueryParam = InitQueryParam(pTabInfo, bgTs, edTs);
  if (pQueryParam == nullptr)
    return PdbE_NOMEM;

  for (auto devIt = devIdList.begin(); devIt != devIdList.end();)
  {
    isAdd = false;
    retVal = QueryDevDesc(*devIt, pQueryParam, pQuery, timeOut, true, &isAdd);
    if (retVal != PdbE_OK)
      break;

    if (isAdd)
    {
      devIt = devIdList.erase(devIt);
    }
    else
    {
      devIt++;
    }
  }

  ClearQueryParam(pQueryParam);
  return retVal;
}

PdbErr_t DataPart::QuerySnapshot(std::list<int64_t>& devIdList,
  const TableInfo* pTabInfo, IQuery* pQuery, uint64_t timeOut)
{
  PdbErr_t retVal = PdbE_OK;
  bool isAdd = false;
  int64_t bgTs, edTs;
  pQuery->GetTstampRange(&bgTs, &edTs);

  void* pQueryParam = InitQueryParam(pTabInfo, MinMillis, MaxMillis);
  if (pQueryParam == nullptr)
    return PdbE_NOMEM;

  for (auto devIt = devIdList.begin(); devIt != devIdList.end();)
  {
    if (*devIt > pQuery->GetMaxDevId())
      break;

    isAdd = false;
    retVal = QueryDevSnapshot(*devIt, pQueryParam, pQuery, timeOut, &isAdd);
    if (retVal != PdbE_OK)
      break;

    if (isAdd)
    {
      devIt = devIdList.erase(devIt);
    }
    else
    {
      devIt++;
    }
  }

  ClearQueryParam(pQueryParam);
  return retVal;
}
