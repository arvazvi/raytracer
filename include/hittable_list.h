#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <memory>
#include <vector>

#include "hittable.h"

using std::make_shared;
using std::shared_ptr;

class HittableList : public Hittable {
   public:
    std::vector<shared_ptr<Hittable>> objects;

    HittableList() {};
    ~HittableList() {};
    HittableList(shared_ptr<Hittable> object) { add(object); };
    void clear() { objects.clear(); };
    void add(shared_ptr<Hittable> object) { objects.push_back(object); };

    bool hit(const Ray& r, double ray_tmin, double ray_tmax,
             Record& rec) const override {
        Record tmp;
        bool any = false;
        double closest = ray_tmax;

        for (const auto& object : objects) {
            if (object->hit(r, ray_tmin, closest, tmp)) {
                any = true;
                closest = tmp.t;
                rec = tmp;
            }
        }

        return any;
    }
};

#endif