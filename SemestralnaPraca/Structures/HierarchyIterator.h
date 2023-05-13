#pragma once

#include <libds/amt/explicit_hierarchy.h>
#include "../Structures/TerritorialUnitStructure.h"

class HierarchyIterator {
public:
	HierarchyIterator(ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>* pHierarchy, ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* pPosition);
	ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* up();
	ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* down(size_t index);

	ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* getPosition();
	ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>::PreOrderHierarchyIterator begin();
private:
	ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* position;
	ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>* hierarchy;
};

HierarchyIterator::HierarchyIterator(ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>* pHierarchy, ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* pPosition) :
	hierarchy(pHierarchy),
	position(pPosition)
{ }

ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* HierarchyIterator::up() {
	return this->position = this->hierarchy->accessParent(*position);
}

ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* HierarchyIterator::down(size_t index) {
	return this->position = this->hierarchy->accessSon(*position, index - 1);
}

ds::amt::MultiWayExplicitHierarchyBlock<TerritorialUnit*>* HierarchyIterator::getPosition() {
	return this->position;
}

ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>::PreOrderHierarchyIterator HierarchyIterator::begin() {
	return ds::amt::MultiWayExplicitHierarchy<TerritorialUnit*>::PreOrderHierarchyIterator(this->hierarchy, this->position);
}
