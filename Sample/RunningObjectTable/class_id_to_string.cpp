/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2003, Samuel R. Blackburn
**
** "You can get credit for something or get it done, but not both."
** Dr. Richard Garwin
**
** BSD License follows.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** Redistributions of source code must retain the above copyright notice,
** this list of conditions and the following disclaimer. Redistributions
** in binary form must reproduce the above copyright notice, this list
** of conditions and the following disclaimer in the documentation and/or
** other materials provided with the distribution. Neither the name of
** the WFC nor the names of its contributors may be used to endorse or
** promote products derived from this software without specific prior
** written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "RunningObjectTable.h"
#pragma hdrstop

void class_id_to_string( const CLSID& class_id, CString& class_name )
{
   WFCTRACEINIT( TEXT( "class_id_to_string()" ) );

   class_name.Format( TEXT( "{%08lX-%04lX-%04lX-%02X%02X-%02X%02X%02X%02X%02X%02X}" ),
                 (unsigned long) class_id.Data1,
                 (unsigned int)  class_id.Data2,
                 (unsigned int)  class_id.Data3,
                 (unsigned int)  class_id.Data4[ 0 ],
                 (unsigned int)  class_id.Data4[ 1 ],
                 (unsigned int)  class_id.Data4[ 2 ],
                 (unsigned int)  class_id.Data4[ 3 ],
                 (unsigned int)  class_id.Data4[ 4 ],
                 (unsigned int)  class_id.Data4[ 5 ],
                 (unsigned int)  class_id.Data4[ 6 ],
                 (unsigned int)  class_id.Data4[ 7 ] );
}
