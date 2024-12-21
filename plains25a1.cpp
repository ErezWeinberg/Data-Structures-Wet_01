#include "plains25a1.h"

// Constructor & Destructor

Plains::Plains() : herds(nullptr), horses(nullptr) {}

Plains::~Plains() {
    try {
        if (herds) {
            deleteHerdTree(herds);
            herds = nullptr;
        }
        if (horses) {
            deleteHorseTree(horses);
            horses = nullptr;
        }
    } catch (...) {
        // Handle cleanup errors
    }
}

void Plains::deleteHorseTree(HorseNode* root) {
    if (!root) return;
    deleteHorseTree(root->left);
    deleteHorseTree(root->right);
    // Make sure to delete all followers before deleting the node
    while (root->followers) {
        FollowerList* temp = root->followers;
        root->followers = root->followers->next;
        delete temp;
    }
    delete root;
}

void Plains::deleteHerdTree(HerdNode* root) {
    if (!root) return;
    deleteHerdTree(root->left);
    deleteHerdTree(root->right);
    if (root->horses) {
        deleteHorseTree(root->horses);
    }
    delete root;
}

Plains::HerdNode* Plains::removeHerd(HerdNode* root, int id) {
    if (!root) return nullptr;

    if (id < root->id)
        root->left = removeHerd(root->left, id);
    else if (id > root->id)
        root->right = removeHerd(root->right, id);
    else {
        if (!root->left || !root->right) {
            HerdNode* temp = root->left ? root->left : root->right;
            if (!temp) {
                temp = root;
                root = nullptr;
            } else {
                *root = *temp;
            }
            delete temp;
        } else {
            HerdNode* temp = findMin(root->right);
            root->id = temp->id;
            root->horses = temp->horses;
            temp->horses = nullptr;  // Prevent double deletion
            root->right = removeHerd(root->right, temp->id);
        }
    }

    if (!root) return nullptr;

    updateHeight(root);
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

Plains::HerdNode* Plains::findMin(HerdNode* node) {
    if (!node) return nullptr;
    while (node->left) {
        node = node->left;
    }
    return node;
}

// Helper functions for tree operations

int Plains::getHeight(HorseNode* node) {

    return node ? node->height : 0;

}

int Plains::getHeight(HerdNode* node) {

    return node ? node->height : 0;

}

void Plains::updateHeight(HorseNode* node) {

    if (node) {

        int leftHeight = getHeight(node->left);

        int rightHeight = getHeight(node->right);

        node->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);

    }

}

void Plains::updateHeight(HerdNode* node) {

    if (node) {

        int leftHeight = getHeight(node->left);

        int rightHeight = getHeight(node->right);

        node->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);

    }

}

int Plains::getBalance(HorseNode* node) {

    return node ? getHeight(node->left) - getHeight(node->right) : 0;

}

int Plains::getBalance(HerdNode* node) {

    return node ? getHeight(node->left) - getHeight(node->right) : 0;

}
// AVL Tree rotations

Plains::HorseNode* Plains::rightRotate(HorseNode* y) {

    if (!y || !y->left) return y;

    HorseNode* x = y->left;

    HorseNode* T2 = x->right;

    x->right = y;

    y->left = T2;

    updateHeight(y);

    updateHeight(x);

    return x;

}

Plains::HerdNode* Plains::rightRotate(HerdNode* y) {

    if (!y || !y->left) return y;

    HerdNode* x = y->left;

    HerdNode* T2 = x->right;

    x->right = y;

    y->left = T2;

    updateHeight(y);

    updateHeight(x);

    return x;

}

Plains::HorseNode* Plains::leftRotate(HorseNode* x) {

    if (!x || !x->right) return x;

    HorseNode* y = x->right;

    HorseNode* T2 = y->left;

    y->left = x;

    x->right = T2;

    updateHeight(x);

    updateHeight(y);

    return y;

}

Plains::HerdNode* Plains::leftRotate(HerdNode* x) {

    if (!x || !x->right) return x;

    HerdNode* y = x->right;

    HerdNode* T2 = y->left;

    y->left = x;

    x->right = T2;

    updateHeight(x);

    updateHeight(y);

    return x;

}
// Insert and find operations

Plains::HorseNode* Plains::insertHorse(HorseNode* node, int id, int speed) {

    if (!node) return new HorseNode(id, speed);



    if (id < node->id) {

        node->left = insertHorse(node->left, id, speed);

    } else if (id > node->id) {

        node->right = insertHorse(node->right, id, speed);

    } else {

        return node;

    }



    updateHeight(node);

    int balance = getBalance(node);



    if (balance > 1 && id < node->left->id)

        return rightRotate(node);



    if (balance < -1 && id > node->right->id)

        return leftRotate(node);



    if (balance > 1 && id > node->left->id) {

        node->left = leftRotate(node->left);

        return rightRotate(node);

    }



    if (balance < -1 && id < node->right->id) {

        node->right = rightRotate(node->right);

        return leftRotate(node);

    }



    return node;

}

Plains::HerdNode* Plains::insertHerd(HerdNode* node, int id) {

    if (!node) return new HerdNode(id);



    if (id < node->id) {

        node->left = insertHerd(node->left, id);

    } else if (id > node->id) {

        node->right = insertHerd(node->right, id);

    } else {

        return node;

    }



    updateHeight(node);

    int balance = getBalance(node);



    if (balance > 1 && id < node->left->id)

        return rightRotate(node);



    if (balance < -1 && id > node->right->id)

        return leftRotate(node);



    if (balance > 1 && id > node->left->id) {

        node->left = leftRotate(node->left);

        return rightRotate(node);

    }



    if (balance < -1 && id < node->right->id) {

        node->right = rightRotate(node->right);

        return leftRotate(node);

    }



    return node;

}

Plains::HorseNode* Plains::findMin(HorseNode* node) {
    if (!node) return nullptr;
    while (node->left) {
        node = node->left;
    }
    return node;
}

Plains::HorseNode* Plains::findHorse(HorseNode* root, int id) {

    if (!root || root->id == id) return root;

    if (id < root->id) return findHorse(root->left, id);

    return findHorse(root->right, id);

}

Plains::HerdNode* Plains::findHerd(HerdNode* root, int id) {

    if (!root || root->id == id) return root;

    if (id < root->id) return findHerd(root->left, id);

    return findHerd(root->right, id);

}

Plains::HorseNode* Plains::removeHorse(HorseNode* root, int id) {
    if (!root) return nullptr;

    if (id < root->id)
        root->left = removeHorse(root->left, id);
    else if (id > root->id)
        root->right = removeHorse(root->right, id);
    else {
        // Node to delete found
        if (!root->left || !root->right) {
            HorseNode* temp = root->left ? root->left : root->right;
            if (!temp) {  // No child case
                temp = root;
                root = nullptr;
            } else {  // One child case
                *root = *temp;  // Copy the contents
            }
            temp->followers = nullptr;  // Prevent double deletion of followers
            delete temp;
        } else {
            // Node with two children
            HorseNode* temp = findMin(root->right);
            root->id = temp->id;
            root->speed = temp->speed;
            // Keep the relationships
            root->herd = temp->herd;
            root->following = temp->following;
            root->followers = temp->followers;
            temp->followers = nullptr;  // Prevent double deletion
            root->right = removeHorse(root->right, temp->id);
        }
    }

    if (!root) return nullptr;

    // Update height and rebalance
    updateHeight(root);
    int balance = getBalance(root);

    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void Plains::removeHorseFromHerd(HorseNode* horse) {
    if (!horse) return;

    // First, make all followers stop following this horse
    while (horse->followers) {
        HorseNode* follower = horse->followers->horse;
        follower->following = nullptr;  // Clear the following pointer
        FollowerList* next = horse->followers->next;
        delete horse->followers;
        horse->followers = next;
    }

    // Then remove this horse from whoever it's following
    if (horse->following) {
        FollowerList* curr = horse->following->followers;
        FollowerList* prev = nullptr;

        while (curr) {
            if (curr->horse == horse) {
                if (prev)
                    prev->next = curr->next;
                else
                    horse->following->followers = curr->next;
                delete curr;
                break;
            }
            prev = curr;
            curr = curr->next;
        }
        horse->following = nullptr;
    }

    horse->herd = nullptr;
}

// Public interface implementations

StatusType Plains::add_herd(int herdId) {

    if (herdId <= 0) return StatusType::INVALID_INPUT;



    if (findHerd(herds, herdId)) return StatusType::FAILURE;



    try {

        herds = insertHerd(herds, herdId);

        return StatusType::SUCCESS;

    } catch (...) {

        return StatusType::ALLOCATION_ERROR;

    }

}

StatusType Plains::remove_herd(int herdId) {
    if (herdId <= 0) return StatusType::INVALID_INPUT;

    HerdNode* herd = findHerd(herds, herdId);
    if (!herd || herd->horses) return StatusType::FAILURE;

    try {
        herds = removeHerd(herds, herdId);
        return StatusType::SUCCESS;
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::add_horse(int horseId, int speed) {

    if (horseId <= 0 || speed <= 0) return StatusType::INVALID_INPUT;



    if (findHorse(horses, horseId)) return StatusType::FAILURE;



    try {

        horses = insertHorse(horses, horseId, speed);

        return StatusType::SUCCESS;

    } catch (...) {

        return StatusType::ALLOCATION_ERROR;

    }

}

StatusType Plains::join_herd(int horseId, int herdId) {

    if (horseId <= 0 || herdId <= 0) return StatusType::INVALID_INPUT;



    HorseNode* horse = findHorse(horses, horseId);

    HerdNode* herd = findHerd(herds, herdId);



    if (!horse || !herd || horse->herd) return StatusType::FAILURE;



    try {

        horse->herd = herd;

        if (!herd->horses) {

            herd->horses = new HorseNode(*horse);

            herd->horses->left = nullptr;

            herd->horses->right = nullptr;

        } else {

            herd->horses = insertHorse(herd->horses, horseId, horse->speed);

        }

        return StatusType::SUCCESS;

    } catch (...) {

        return StatusType::ALLOCATION_ERROR;

    }

}

StatusType Plains::follow(int horseId, int horseToFollowId) {
    if (horseId <= 0 || horseToFollowId <= 0 || horseId == horseToFollowId)
        return StatusType::INVALID_INPUT;

    HorseNode* horse = findHorse(horses, horseId);
    HorseNode* horseToFollow = findHorse(horses, horseToFollowId);

    if (!horse || !horseToFollow || !horse->herd || !horseToFollow->herd ||
        horse->herd != horseToFollow->herd)
        return StatusType::FAILURE;

    try {
        // Remove from current following relationship if exists
        if (horse->following) {
            FollowerList* curr = horse->following->followers;
            FollowerList* prev = nullptr;

            while (curr) {
                if (curr->horse == horse) {
                    if (prev)
                        prev->next = curr->next;
                    else
                        horse->following->followers = curr->next;
                    delete curr;
                    break;
                }
                prev = curr;
                curr = curr->next;
            }
        }

        // Add new following relationship
        horse->following = horseToFollow;
        FollowerList* newFollower = new FollowerList(horse);
        newFollower->next = horseToFollow->followers;
        horseToFollow->followers = newFollower;

        return StatusType::SUCCESS;
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::leave_herd(int horseId) {
    if (horseId <= 0) return StatusType::INVALID_INPUT;

    HorseNode* horse = findHorse(horses, horseId);
    if (!horse || !horse->herd) return StatusType::FAILURE;

    try {
        HerdNode* herd = horse->herd;
        // First remove the horse from herd's tree
        if (herd->horses) {
            herd->horses = removeHorse(herd->horses, horseId);
        }
        // Then clean up relationships
        removeHorseFromHerd(horse);
        return StatusType::SUCCESS;
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> Plains::get_speed(int horseId) {

    if (horseId <= 0) return output_t<int>(StatusType::INVALID_INPUT);



    HorseNode* horse = findHorse(horses, horseId);

    if (!horse) return output_t<int>(StatusType::FAILURE);



    return output_t<int>(horse->speed);

}

output_t<bool> Plains::leads(int horseId, int otherHorseId) {

    if (horseId <= 0 || otherHorseId <= 0 || horseId == otherHorseId)

        return output_t<bool>(StatusType::INVALID_INPUT);



    HorseNode* horse = findHorse(horses, horseId);

    HorseNode* otherHorse = findHorse(horses, otherHorseId);



    if (!horse || !otherHorse)

        return output_t<bool>(StatusType::FAILURE);



    return output_t<bool>(doesLead(otherHorse, horse));

}

output_t<bool> Plains::can_run_together(int herdId) {
    if (herdId <= 0) return output_t<bool>(StatusType::INVALID_INPUT);

    HerdNode* herd = findHerd(herds, herdId);
    if (!herd || !herd->horses) return output_t<bool>(StatusType::FAILURE);

    // Special case: single horse can always run together
    if (!herd->horses->left && !herd->horses->right)
        return output_t<bool>(true);

    HorseNode* current = herd->horses;
    while (current) {
        bool leadsAll = true;
        HorseNode* other = herd->horses;

        while (other && leadsAll) {
            if (other != current) {
                // Check if current leads other and other doesn't lead current
                if (!doesLead(current, other) || doesLead(other, current)) {
                    leadsAll = false;
                    break;
                }
            }
            // Move to next horse in-order
            if (other->right) {
                other = other->right;
                while (other->left) other = other->left;
            } else {
                while (other->right && other == other->right) other = other->right;
                other = other->right;
            }
        }

        if (leadsAll) return output_t<bool>(true);

        // Move to next horse in-order
        if (current->right) {
            current = current->right;
            while (current->left) current = current->left;
        } else {
            while (current->right && current == current->right) current = current->right;
            current = current->right;
        }
    }

    return output_t<bool>(false);
}

bool Plains::doesLead(HorseNode* leader, HorseNode* follower) {
    if (!leader || !follower || leader->herd != follower->herd) {
        return false;
    }

    // Use an array for visited nodes - assuming max horse ID is 100000
    int visited[100001] = {0};  // Initialize all to 0
    HorseNode* current = follower;

    while (current && !visited[current->id]) {
        if (current == leader) return true;
        visited[current->id] = 1;
        current = current->following;
    }

    return false;
}