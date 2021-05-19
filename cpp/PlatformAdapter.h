//
//  PlatformAdapter.h
//  my-turbo-utils
//
//  Created by Bartłomiej Klocek on 19/05/2021.
//

#ifndef PlatformAdapter_h
#define PlatformAdapter_h

#include <string>

namespace turboutils {


    class PlatformAdapter {
    public:
        virtual ~PlatformAdapter() = default;

        virtual std::string delegateGreeting(const std::string &name) = 0;
    };

}

#endif /* PlatformAdapter_h */
