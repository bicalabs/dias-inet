/* ALIGN.H   switchs the copiler into a specified structur alignment mode
             the mode is given from a predefined  STRUCT_ALIGNMENT konstant

             set STRUCT_ALIGNMENT to 0 to setup default compiler settings

  Tested with:  MSC 5.01
                BC  4.5  (win16)
*/


#ifndef STRUCT_ALIGNMENT
#error "please define STRUCT_ALIGNMENT before including align.h"
#else

/* ------------------------------------------------- MICROSOFT */
#if defined(_MSC_VER)

#if   STRUCT_ALIGNMENT == 0
#pragma pack()
#elif STRUCT_ALIGNMENT == 1
#pragma pack(1)
#elif STRUCT_ALIGNMENT == 2
#pragma pack(2)
#elif STRUCT_ALIGNMENT == 4
#pragma pack(4)
#elif STRUCT_ALIGNMENT == 8
#pragma pack(8)
#else
#error "illegal value STRUCT_ALIGNMENT"
#endif

#pragma warning( disable : 4103 )

#endif  // #if defined(_MSC_VER)

/* ------------------------------------------------- BORLAND */
#if defined(__BORLANDC__)
#if __BORLANDC__ > 0x410

#if   STRUCT_ALIGNMENT == 0
  #pragma option -a.
#elif STRUCT_ALIGNMENT == 1
  #pragma option -a1
#elif STRUCT_ALIGNMENT == 2
  #pragma option -a2
#elif defined(__DPMI32__) || defined(_WIN32)

  #if STRUCT_ALIGNMENT == 4
    #pragma option -a4
  #elif STRUCT_ALIGNMENT == 8
    #pragma option -a8
  #endif

#else
#error "illegal value STRUCT_ALIGNMENT"
#endif

#pragma warn -pck

#endif   // #if __BORLANDC__ > 0x410
#endif   // #if defined(__BORLANDC__)

/* ------------------------------------------------- HIGHC */
#if defined(__HIGHC__)

#if   STRUCT_ALIGNMENT == 0
#pragma pop_align_members();
#elif STRUCT_ALIGNMENT == 1
#pragma push_align_members(1);
#elif STRUCT_ALIGNMENT == 2
#pragma push_align_members(2);
#elif STRUCT_ALIGNMENT == 4
#pragma push_align_members(4);
#elif STRUCT_ALIGNMENT == 8
#pragma push_align_members(8);
#else
#error "illegal value STRUCT_ALIGNMENT"
#endif

#endif   // #if defined(__HIGHC__)

/* ------------------------------------------------- WATCOM */
#if defined(__WATCOMC__)

#if   STRUCT_ALIGNMENT == 0
#pragma pack ( pop );
#elif STRUCT_ALIGNMENT == 1
#pragma pack ( push, 1 );
#elif STRUCT_ALIGNMENT == 2
#pragma pack ( push, 2 );
#elif STRUCT_ALIGNMENT == 4
#pragma pack ( push, 4 );
#elif STRUCT_ALIGNMENT == 8
#pragma pack ( push, 8 );
#else
#error "illegal value STRUCT_ALIGNMENT"
#endif

#endif  // #if defined(__WATCOMC__)


#undef STRUCT_ALIGNMENT



#endif  // #ifndef STRUCT_ALIGNMENT


