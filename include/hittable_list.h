#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>

#include "hittable.h"
#include "utils.h"

class HittableList : public Hittable {
   public:
    std::vector<shared_ptr<Hittable>> objects;

    HittableList() {};
    ~HittableList() {};
    HittableList(shared_ptr<Hittable> object) { add(object); };
    void clear() { objects.clear(); };
    void add(shared_ptr<Hittable> object) { objects.push_back(object); };

    bool hit(const Ray& r, Interval ray_t, Record& rec) const override {
        Record tmp;
        bool any = false;
        double closest = ray_t.max;

        for (const auto& object : objects) {
            if (object->hit(r, Interval(ray_t.min, closest), tmp)) {
                any = true;
                closest = tmp.t;
                rec = tmp;
            }
        }

        return any;
    }
};

#endif