#include "plains25a1.h"
#include <algorithm>

// AVL Tree Helper Methods
int Plains::getHeight(AVLNode<Horse*>* node) {
    if (!node) return 0;
    return node->height;
}

int Plains::getBalanceFactor(AVLNode<Horse*>* node) {
    if (!node) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

AVLNode<Horse*>* Plains::rightRotate(AVLNode<Horse*>* y) {
    AVLNode<Horse*>* x = y->left;
    AVLNode<Horse*>* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

AVLNode<Horse*>* Plains::leftRotate(AVLNode<Horse*>* x) {
    AVLNode<Horse*>* y = x->right;
    AVLNode<Horse*>* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

// Tree Operations
AVLNode<Horse*>* Plains::insertHorse(AVLNode<Horse*>* node, Horse* horse) {
    if (!node) return new AVLNode<Horse*>(horse);

    if (horse->id < node->data->id)
        node->left = insertHorse(node->left, horse);
    else if (horse->id > node->data->id)
        node->right = insertHorse(node->right, horse);
    else
        return node;

    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

    int balance = getBalanceFactor(node);

    // Left Left
    if (balance > 1 && horse->id < node->left->data->id)
        return rightRotate(node);

    // Right Right
    if (balance < -1 && horse->id > node->right->data->id)
        return leftRotate(node);

    // Left Right
    if (balance > 1 && horse->id > node->left->data->id) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left
    if (balance < -1 && horse->id < node->right->data->id) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

Horse* Plains::findHorse(AVLNode<Horse*>* node, int horseId) {
    if (!node) return nullptr;
    
    if (horseId < node->data->id)
        return findHorse(node->left, horseId);
    else if (horseId > node->data->id)
        return findHorse(node->right, horseId);
    else
        return node->data;
}

Herd* Plains::findHerd(AVLNode<Herd*>* node, int herdId) {
    if (!node) return nullptr;
    
    if (herdId < node->data->id)
        return findHerd(node->left, herdId);
    else if (herdId > node->data->id)
        return findHerd(node->right, herdId);
    else
        return node->data;
}

bool Plains::isInCycle(Horse* horse, Horse* target) {
    if (!horse || !horse->following) return false;
    if (horse->following == target) return true;
    return isInCycle(horse->following, target);
}

void Plains::deleteTree(AVLNode<Horse*>* node) {
    if (!node) return;
    
    deleteTree(node->left);
    deleteTree(node->right);
    delete node->data;
    delete node;
}

void Plains::deleteHerdTree(AVLNode<Herd*>* node) {
    if (!node) return;
    
    deleteHerdTree(node->left);
    deleteHerdTree(node->right);
    delete node->data;
    delete node;
}

// Constructor & Destructor
Plains::Plains() : horses(nullptr), herds(nullptr) {}

Plains::~Plains() {
    deleteTree(horses);
    deleteHerdTree(herds);
}

// Public Interface Methods
StatusType Plains::add_herd(int herdId) {
    if (herdId <= 0) return StatusType::INVALID_INPUT;
    
    if (findHerd(herds, herdId)) return StatusType::FAILURE;
    
    try {
        Herd* newHerd = new Herd(herdId);
        herds = insertHerd(herds, newHerd);
        return StatusType::SUCCESS;
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::add_horse(int horseId, int speed) {
    if (horseId <= 0 || speed <= 0) return StatusType::INVALID_INPUT;
    
    if (findHorse(horses, horseId)) return StatusType::FAILURE;
    
    try {
        Horse* newHorse = new Horse(horseId, speed);
        horses = insertHorse(horses, newHorse);
        return StatusType::SUCCESS;
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::join_herd(int horseId, int herdId) {
    if (horseId <= 0 || herdId <= 0) return StatusType::INVALID_INPUT;
    
    Horse* horse = findHorse(horses, horseId);
    Herd* herd = findHerd(herds, herdId);
    
    if (!horse || !herd) return StatusType::FAILURE;
    if (horse->herdId != -1) return StatusType::FAILURE;
    
    try {
        horse->herdId = herdId;
        herd->members = insertHorse(herd->members, horse);
        return StatusType::SUCCESS;
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::follow(int horseId, int horseToFollowId) {
    if (horseId <= 0 || horseToFollowId <= 0) return StatusType::INVALID_INPUT;
    if (horseId == horseToFollowId) return StatusType::INVALID_INPUT;
    
    Horse* horse = findHorse(horses, horseId);
    Horse* horseToFollow = findHorse(horses, horseToFollowId);
    
    if (!horse || !horseToFollow) return StatusType::FAILURE;
    if (isInCycle(horseToFollow, horse)) return StatusType::FAILURE;
    
    horse->following = horseToFollow;
    return StatusType::SUCCESS;
}

output_t<int> Plains::get_speed(int horseId) {
    if (horseId <= 0) return output_t<int>(StatusType::INVALID_INPUT);
    
    Horse* horse = findHorse(horses, horseId);
    if (!horse) return output_t<int>(StatusType::FAILURE);
    
    return output_t<int>(horse->speed);
}

output_t<bool> Plains::leads(int horseId, int otherHorseId) {
    if (horseId <= 0 || otherHorseId <= 0) return output_t<bool>(StatusType::INVALID_INPUT);
    
    Horse* horse = findHorse(horses, horseId);
    Horse* otherHorse = findHorse(horses, otherHorseId);
    
    if (!horse || !otherHorse) return output_t<bool>(StatusType::FAILURE);
    
    Horse* current = otherHorse;
    while (current && current->following) {
        if (current->following == horse) return output_t<bool>(true);
        current = current->following;
    }
    
    return output_t<bool>(false);
}

bool Plains::canAllRunTogether(AVLNode<Horse*>* members) {
    if (!members) return true;
    
    // Recursively check all members
    if (!canAllRunTogether(members->left)) return false;
    if (!canAllRunTogether(members->right)) return false;
    
    Horse* horse = members->data;
    Horse* current = horse;
    
    // Check if this horse forms any cycles with other members
    while (current && current->following) {
        current = current->following;
        if (current == horse) return false;
    }
    
    return true;
}

output_t<bool> Plains::can_run_together(int herdId) {
    if (herdId <= 0) return output_t<bool>(StatusType::INVALID_INPUT);
    
    Herd* herd = findHerd(herds, herdId);
    if (!herd) return output_t<bool>(StatusType::FAILURE);
    
    return output_t<bool>(canAllRunTogether(herd->members));
}

StatusType Plains::remove_herd(int herdId) {
    if (herdId <= 0) return StatusType::INVALID_INPUT;
    
    Herd* herd = findHerd(herds, herdId);
    if (!herd) return StatusType::FAILURE;
    
    // Remove all horses from the herd
    AVLNode<Horse*>* current = herd->members;
    while (current) {
        current->data->herdId = -1;
        current = current->right;
    }
    
    // Remove the herd from the herds tree
    herds = removeHerd(herds, herdId);
    return StatusType::SUCCESS;
}

StatusType Plains::leave_herd(int horseId) {
    if (horseId <= 0) return StatusType::INVALID_INPUT;
    
    Horse* horse = findHorse(horses, horseId);
    if (!horse) return StatusType::FAILURE;
    
    if (horse->herdId == -1) return StatusType::FAILURE;
    
    Herd* herd = findHerd(herds, horse->herdId);
    if (!herd) return StatusType::FAILURE;
    
    herd->members = removeHorse(herd->members, horseId);
    horse->herdId = -1;
    
    return StatusType::SUCCESS;
}