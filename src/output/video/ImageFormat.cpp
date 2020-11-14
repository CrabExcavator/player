//
// Created by CrabExcavator on 2020/11/12.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "ImageFormat.h"

namespace video {

std::map<ImageFormat, ImageFormatAttribute *> ImageFormatAttributeMap = {
    {ImageFormat::yuv420p, new ImageFormatAttribute{
        .image_format = ImageFormat::yuv420p
    }}
};

}