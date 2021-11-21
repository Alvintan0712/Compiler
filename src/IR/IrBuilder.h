//
// Created by tjan2 on 17/11/2021.
//

#ifndef SRC_IRBUILDER_H
#define SRC_IRBUILDER_H

#include <string>

class IrBuilder {
public:
    IrBuilder();
    int genGlobal();
    int genLabel();
private:
    int var_id, global_id, label_id;
};


#endif //SRC_IRBUILDER_H
