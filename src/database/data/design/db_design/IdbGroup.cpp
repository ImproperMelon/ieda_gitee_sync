/**
 * @project		iDB
 * @file		IdbGroup.h
 * @date		25/05/2021
 * @version		0.1
* @description


    Defines groups in a design.
 *
 */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "IdbGroup.h"

#include <algorithm>

using namespace std;
namespace idb {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IdbGroup::IdbGroup(string group_name)
{
  _group_name = group_name;
  _region = nullptr;
  _instance_list = new IdbInstanceList();
}

IdbGroup::~IdbGroup()
{
  if (_instance_list) {
    delete _instance_list;
    _instance_list = nullptr;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IdbGroupList::IdbGroupList()
{
  _num = 0;
}

IdbGroupList::~IdbGroupList()
{
  reset();
}

IdbGroup* IdbGroupList::add_group(string name)
{
  IdbGroup* group = new IdbGroup(name);

  _group_list.emplace_back(group);
  _num++;

  return group;
}

void IdbGroupList::reset()
{
  for (auto& group : _group_list) {
    if (group != nullptr) {
      delete group;
      group = nullptr;
    }
  }

  _group_list.clear();
}

}  // namespace idb
