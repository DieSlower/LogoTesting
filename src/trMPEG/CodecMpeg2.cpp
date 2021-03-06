/*
* True Reality Open Source Game and Simulation Engine
* Copyright � 2018 Acid Rain Studios LLC
*
* This library is free software; you can redistribute it and/or modify it under
* the terms of the GNU Lesser General Public License as published by the Free
* Software Foundation; either version 3.0 of the License, or (at your option)
* any later version.
*
* This library is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
* details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
* @author Maxim Serebrennik
*/

#include <trMPEG/CodecMpeg2.h>
#include <trUtil/RefStr.h>

namespace trMPEG
{
    const trUtil::RefStr CodecMpeg2::CLASS_TYPE = trUtil::RefStr("trMPEG::CodecMpeg2");

    //////////////////////////////////////////////////////////////////////////
    CodecMpeg2::CodecMpeg2(const std::string& name) : BaseClass(name)
    {        
        mExtension = trUtil::RefStr(".mpg");
        mMpegType = trUtil::RefStr("mpeg");
        mCodecID = AVCodecID::AV_CODEC_ID_MPEG2VIDEO;
    }

    //////////////////////////////////////////////////////////////////////////
    const std::string & CodecMpeg2::GetType() const
    {
        return CLASS_TYPE;
    }

    //////////////////////////////////////////////////////////////////////////
    CodecMpeg2::~CodecMpeg2()
    {
    }
}