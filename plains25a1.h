#ifndef PLAINS25A1_H

#define PLAINS25A1_H



typedef enum {

    SUCCESS = 0,

    ALLOCATION_ERROR = 1,

    INVALID_INPUT = 2,

    FAILURE = 3

} StatusType;



template<class T>

class output_t {

private:

    StatusType _status;

    T _ans;

public:

    output_t() : _status(SUCCESS), _ans(T()) {}

    output_t(StatusType status) : _status(status), _ans(T()) {}

    output_t(const T& ans) : _status(SUCCESS), _ans(ans) {}

    StatusType status() const { return _status; }

    T ans() const { return _ans; }

};


class Plains {
public:
    // Move these struct declarations to public section
    struct HorseNode;
    struct HerdNode;
    struct FollowerList;

    struct FollowerList {
        HorseNode* horse;
        FollowerList* next;
        explicit FollowerList(HorseNode* h) : horse(h), next(nullptr) {}
    };

    struct HorseNode {
        int id;
        int speed;
        HorseNode* left;
        HorseNode* right;
        int height;
        HerdNode* herd;
        HorseNode* following;
        FollowerList* followers;

        HorseNode(int horseId, int horseSpeed) :
            id(horseId), speed(horseSpeed), left(nullptr), right(nullptr),
            height(1), herd(nullptr), following(nullptr), followers(nullptr) {}
    };

    struct HerdNode {
        int id;
        HorseNode* horses;
        HerdNode* left;
        HerdNode* right;
        int height;

        explicit HerdNode(int herdId) :
            id(herdId), horses(nullptr), left(nullptr), right(nullptr), height(1) {}
    };

    Plains();
    ~Plains();
    StatusType add_herd(int herdId);
    StatusType remove_herd(int herdId);
    StatusType add_horse(int horseId, int speed);
    StatusType join_herd(int horseId, int herdId);
    StatusType follow(int horseId, int horseToFollowId);
    StatusType leave_herd(int horseId);
    output_t<int> get_speed(int horseId);
    output_t<bool> leads(int horseId, int otherHorseId);
    output_t<bool> can_run_together(int herdId);

private:
    HerdNode* herds;
    HorseNode* horses;

    int getHeight(HorseNode* node);
    int getHeight(HerdNode* node);
    void updateHeight(HorseNode* node);
    void updateHeight(HerdNode* node);
    int getBalance(HorseNode* node);
    int getBalance(HerdNode* node);
    HorseNode* rightRotate(HorseNode* y);
    HerdNode* rightRotate(HerdNode* y);
    HorseNode* leftRotate(HorseNode* x);
    HerdNode* leftRotate(HerdNode* x);

    // Tree operations
    HorseNode* insertHorse(HorseNode* root, int id, int speed);
    HerdNode* insertHerd(HerdNode* root, int id);
    HorseNode* removeHorse(HorseNode* root, int id);
    HerdNode* removeHerd(HerdNode* root, int id);  // Add this declaration
    HorseNode* findMin(HorseNode* node);
    HerdNode* findMin(HerdNode* node);

    HorseNode* findHorse(HorseNode* root, int id);
    HerdNode* findHerd(HerdNode* root, int id);

    void removeHorseFromHerd(HorseNode* horse);
    bool doesLead(HorseNode* leader, HorseNode* follower);

    void deleteHorseTree(HorseNode* root);
    void deleteHerdTree(HerdNode* root);
};



#endif