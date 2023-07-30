//////////////////////////////////////////////////////////////////////
// $Id: EventQueue.cpp 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A thread-safe event queue.
//   An event is defined by a string description, and a time stamp.
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2023: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
///////////////////////////////////////////////////////////////////////
#include "EventQueue.h"

#include "Exception.h"

bool EventQueue::Compare::operator()(const Element& a, const Element& b) const
{
    /*
    From https://en.cppreference.com/w/cpp/container/priority_queue:
    Note that the Compare parameter is defined such that it returns true if its 
    first argument comes before its second argument in a weak ordering.
    But because the priority queue outputs largest elements first, the elements 
    that "come before" are actually output last.That is, the front of the queue 
    contains the "last" element according to the weak ordering imposed by Compare.
    */
    return PrecisionTime::SignedDiff(a.second, b.second) > 0;
}

void EventQueue::PushBack(const std::string& inDescriptor, PrecisionTime inTimeStamp)
{
  if (!mEventsAllowed)
    throw std_runtime_error << "No events allowed when receiving \"" << inDescriptor
    << "\" event "
    "-- trying to record events outside the \"running\" state?";
  std::lock_guard<std::mutex> lock(mMutex);
  if (mQueue.size() == mMaxCount - 1)
    throw std_runtime_error << "Event queue length reached safety maximum of " << mMaxCount;
  if (mQueue.size() < mMaxCount - 1)
    mQueue.push(make_pair(inDescriptor, inTimeStamp));
}

void EventQueue::PopFront()
{
  std::lock_guard<std::mutex> lock(mMutex);
  mQueue.pop();
}

bool EventQueue::IsEmpty()
{
  std::lock_guard<std::mutex> lock(mMutex);
  return mQueue.empty();
}

const std::string& EventQueue::FrontDescriptor() const
{
  std::lock_guard<std::mutex> lock(mMutex);
  return mQueue.top().first;
}

PrecisionTime EventQueue::FrontTimeStamp() const
{
  std::lock_guard<std::mutex> lock(mMutex);
  return mQueue.top().second;
}

