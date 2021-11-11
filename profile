Flat profile:

Each sample counts as 0.01 seconds.
 no time accumulated

  %   cumulative   self              self     total           
 time   seconds   seconds    calls  Ts/call  Ts/call  name    
  0.00      0.00     0.00 44236800     0.00     0.00  float& cv::Mat::at<float>(int, int)
  0.00      0.00     0.00 44236800     0.00     0.00  std::atan2(float, float)
  0.00      0.00     0.00 14745600     0.00     0.00  std::cos(float)
  0.00      0.00     0.00  7372800     0.00     0.00  std::sqrt(float)
  0.00      0.00     0.00      153     0.00     0.00  cv::_InputArray::init(int, void const*)
  0.00      0.00     0.00      117     0.00     0.00  cv::Size_<int>::Size_()
  0.00      0.00     0.00       97     0.00     0.00  cv::_InputArray::~_InputArray()
  0.00      0.00     0.00       56     0.00     0.00  cv::_InputArray::_InputArray()
  0.00      0.00     0.00       56     0.00     0.00  cv::_OutputArray::~_OutputArray()
  0.00      0.00     0.00       52     0.00     0.00  cv::Range::Range(int, int)
  0.00      0.00     0.00       41     0.00     0.00  cv::_InputArray::_InputArray(cv::Mat const&)
  0.00      0.00     0.00       28     0.00     0.00  cv::_OutputArray::_OutputArray(cv::Mat const&)
  0.00      0.00     0.00       28     0.00     0.00  cv::_OutputArray::_OutputArray(cv::Mat&)
  0.00      0.00     0.00       28     0.00     0.00  cv::Rect_<int>::Rect_(int, int, int, int)
  0.00      0.00     0.00       28     0.00     0.00  cv::Mat::operator()(cv::Rect_<int> const&) const
  0.00      0.00     0.00       26     0.00     0.00  cv::Mat::operator()(cv::Range, cv::Range) const
  0.00      0.00     0.00        6     0.00     0.00  createCubeMapFace(cv::Mat const&, cv::Mat&, float*, int, int, int)
  0.00      0.00     0.00        6     0.00     0.00  cv::Vec<double, 4>::Vec()
  0.00      0.00     0.00        6     0.00     0.00  cv::Matx<double, 4, 1>::Matx()
  0.00      0.00     0.00        6     0.00     0.00  cv::Scalar_<double>::Scalar_(double, double, double, double)
  0.00      0.00     0.00        4     0.00     0.00  splitFace(cv::Mat const&, cv::Mat&, cv::Mat&, cv::Mat&, int, int)
  0.00      0.00     0.00        2     0.00     0.00  importImage(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >)
  0.00      0.00     0.00        2     0.00     0.00  cv::Mat::type() const
  0.00      0.00     0.00        1     0.00     0.00  _GLOBAL__sub_I_faceTransform
  0.00      0.00     0.00        1     0.00     0.00  assemblePacked(cv::Mat&, cv::Mat&, float)
  0.00      0.00     0.00        1     0.00     0.00  assembleCubemap(cv::Mat&, float, float*)
  0.00      0.00     0.00        1     0.00     0.00  __static_initialization_and_destruction_0(int, int)
  0.00      0.00     0.00        1     0.00     0.00  __gnu_cxx::new_allocator<int>::new_allocator()
  0.00      0.00     0.00        1     0.00     0.00  __gnu_cxx::new_allocator<int>::~new_allocator()
  0.00      0.00     0.00        1     0.00     0.00  std::allocator<int>::allocator()
  0.00      0.00     0.00        1     0.00     0.00  std::allocator<int>::~allocator()
  0.00      0.00     0.00        1     0.00     0.00  void std::_Destroy_aux<true>::__destroy<int*>(int*, int*)
  0.00      0.00     0.00        1     0.00     0.00  std::_Vector_base<int, std::allocator<int> >::_Vector_impl::_Vector_impl()
  0.00      0.00     0.00        1     0.00     0.00  std::_Vector_base<int, std::allocator<int> >::_Vector_impl::~_Vector_impl()
  0.00      0.00     0.00        1     0.00     0.00  std::_Vector_base<int, std::allocator<int> >::_M_deallocate(int*, unsigned long)
  0.00      0.00     0.00        1     0.00     0.00  std::_Vector_base<int, std::allocator<int> >::_Vector_impl_data::_Vector_impl_data()
  0.00      0.00     0.00        1     0.00     0.00  std::_Vector_base<int, std::allocator<int> >::_M_get_Tp_allocator()
  0.00      0.00     0.00        1     0.00     0.00  std::_Vector_base<int, std::allocator<int> >::_Vector_base()
  0.00      0.00     0.00        1     0.00     0.00  std::_Vector_base<int, std::allocator<int> >::~_Vector_base()
  0.00      0.00     0.00        1     0.00     0.00  std::vector<int, std::allocator<int> >::vector()
  0.00      0.00     0.00        1     0.00     0.00  std::vector<int, std::allocator<int> >::~vector()
  0.00      0.00     0.00        1     0.00     0.00  void std::_Destroy<int*>(int*, int*)
  0.00      0.00     0.00        1     0.00     0.00  void std::_Destroy<int*, int>(int*, int*, std::allocator<int>&)

 %         the percentage of the total running time of the
time       program used by this function.

cumulative a running sum of the number of seconds accounted
 seconds   for by this function and those listed above it.

 self      the number of seconds accounted for by this
seconds    function alone.  This is the major sort for this
           listing.

calls      the number of times this function was invoked, if
           this function is profiled, else blank.

 self      the average number of milliseconds spent in this
ms/call    function per call, if this function is profiled,
	   else blank.

 total     the average number of milliseconds spent in this
ms/call    function and its descendents per call, if this
	   function is profiled, else blank.

name       the name of the function.  This is the minor sort
           for this listing. The index shows the location of
	   the function in the gprof listing. If the index is
	   in parenthesis it shows where it would appear in
	   the gprof listing if it were to be printed.

Copyright (C) 2012-2020 Free Software Foundation, Inc.

Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.

		     Call graph (explanation follows)


granularity: each sample hit covers 2 byte(s) no time propagated

index % time    self  children    called     name
                0.00    0.00 44236800/44236800     createCubeMapFace(cv::Mat const&, cv::Mat&, float*, int, int, int) [24]
[8]      0.0    0.00    0.00 44236800         float& cv::Mat::at<float>(int, int) [8]
-----------------------------------------------
                0.00    0.00 44236800/44236800     createCubeMapFace(cv::Mat const&, cv::Mat&, float*, int, int, int) [24]
[9]      0.0    0.00    0.00 44236800         std::atan2(float, float) [9]
-----------------------------------------------
                0.00    0.00 14745600/14745600     createCubeMapFace(cv::Mat const&, cv::Mat&, float*, int, int, int) [24]
[10]     0.0    0.00    0.00 14745600         std::cos(float) [10]
-----------------------------------------------
                0.00    0.00 7372800/7372800     createCubeMapFace(cv::Mat const&, cv::Mat&, float*, int, int, int) [24]
[11]     0.0    0.00    0.00 7372800         std::sqrt(float) [11]
-----------------------------------------------
                0.00    0.00      28/153         cv::_OutputArray::_OutputArray(cv::Mat&) [20]
                0.00    0.00      28/153         cv::_OutputArray::_OutputArray(cv::Mat const&) [19]
                0.00    0.00      41/153         cv::_InputArray::_InputArray(cv::Mat const&) [18]
                0.00    0.00      56/153         cv::_InputArray::_InputArray() [15]
[12]     0.0    0.00    0.00     153         cv::_InputArray::init(int, void const*) [12]
-----------------------------------------------
                0.00    0.00       2/117         assembleCubemap(cv::Mat&, float, float*) [33]
                0.00    0.00       8/117         splitFace(cv::Mat const&, cv::Mat&, cv::Mat&, cv::Mat&, int, int) [28]
                0.00    0.00      10/117         assemblePacked(cv::Mat&, cv::Mat&, float) [32]
                0.00    0.00      41/117         cv::_InputArray::_InputArray(cv::Mat const&) [18]
                0.00    0.00      56/117         cv::_InputArray::_InputArray() [15]
[13]     0.0    0.00    0.00     117         cv::Size_<int>::Size_() [13]
-----------------------------------------------
                0.00    0.00       1/97          main [6]
                0.00    0.00       4/97          assembleCubemap(cv::Mat&, float, float*) [33]
                0.00    0.00       8/97          splitFace(cv::Mat const&, cv::Mat&, cv::Mat&, cv::Mat&, int, int) [28]
                0.00    0.00      10/97          assemblePacked(cv::Mat&, cv::Mat&, float) [32]
                0.00    0.00      18/97          createCubeMapFace(cv::Mat const&, cv::Mat&, float*, int, int, int) [24]
                0.00    0.00      56/97          cv::_OutputArray::~_OutputArray() [16]
[14]     0.0    0.00    0.00      97         cv::_InputArray::~_InputArray() [14]
-----------------------------------------------
                0.00    0.00      28/56          cv::_OutputArray::_OutputArray(cv::Mat&) [20]
                0.00    0.00      28/56          cv::_OutputArray::_OutputArray(cv::Mat const&) [19]
[15]     0.0    0.00    0.00      56         cv::_InputArray::_InputArray() [15]
                0.00    0.00      56/117         cv::Size_<int>::Size_() [13]
                0.00    0.00      56/153         cv::_InputArray::init(int, void const*) [12]
-----------------------------------------------
                0.00    0.00       6/56          createCubeMapFace(cv::Mat const&, cv::Mat&, float*, int, int, int) [24]
                0.00    0.00       8/56          splitFace(cv::Mat const&, cv::Mat&, cv::Mat&, cv::Mat&, int, int) [28]
                0.00    0.00      18/56          assembleCubemap(cv::Mat&, float, float*) [33]
                0.00    0.00      24/56          assemblePacked(cv::Mat&, cv::Mat&, float) [32]
[16]     0.0    0.00    0.00      56         cv::_OutputArray::~_OutputArray() [16]
                0.00    0.00      56/97          cv::_InputArray::~_InputArray() [14]
-----------------------------------------------
                0.00    0.00      24/52          splitFace(cv::Mat const&, cv::Mat&, cv::Mat&, cv::Mat&, int, int) [28]
                0.00    0.00      28/52          assemblePacked(cv::Mat&, cv::Mat&, float) [32]
[17]     0.0    0.00    0.00      52         cv::Range::Range(int, int) [17]
-----------------------------------------------
                0.00    0.00       1/41          main [6]
                0.00    0.00       4/41          assembleCubemap(cv::Mat&, float, float*) [33]
                0.00    0.00       8/41          splitFace(cv::Mat const&, cv::Mat&, cv::Mat&, cv::Mat&, int, int) [28]
                0.00    0.00      10/41          assemblePacked(cv::Mat&, cv::Mat&, float) [32]
                0.00    0.00      18/41          createCubeMapFace(cv::Mat const&, cv::Mat&, float*, int, int, int) [24]
[18]     0.0    0.00    0.00      41         cv::_InputArray::_InputArray(cv::Mat const&) [18]
                0.00    0.00      41/117         cv::Size_<int>::Size_() [13]
                0.00    0.00      41/153         cv::_InputArray::init(int, void const*) [12]
-----------------------------------------------
                0.00    0.00      14/28          assembleCubemap(cv::Mat&, float, float*) [33]
                0.00    0.00      14/28          assemblePacked(cv::Mat&, cv::Mat&, float) [32]
[19]     0.0    0.00    0.00      28         cv::_OutputArray::_OutputArray(cv::Mat const&) [19]
                0.00    0.00      28/56          cv::_InputArray::_InputArray() [15]
                0.00    0.00      28/153         cv::_InputArray::init(int, void const*) [12]
-----------------------------------------------
                0.00    0.00       4/28          assembleCubemap(cv::Mat&, float, float*) [33]
                0.00    0.00       6/28          createCubeMapFace(cv::Mat const&, cv::Mat&, float*, int, int, int) [24]
                0.00    0.00       8/28          splitFace(cv::Mat const&, cv::Mat&, cv::Mat&, cv::Mat&, int, int) [28]
                0.00    0.00      10/28          assemblePacked(cv::Mat&, cv::Mat&, float) [32]
[20]     0.0    0.00    0.00      28         cv::_OutputArray::_OutputArray(cv::Mat&) [20]
                0.00    0.00      28/56          cv::_InputArray::_InputArray() [15]
                0.00    0.00      28/153         cv::_InputArray::init(int, void const*) [12]
-----------------------------------------------
                0.00    0.00      14/28          assembleCubemap(cv::Mat&, float, float*) [33]
                0.00    0.00      14/28          assemblePacked(cv::Mat&, cv::Mat&, float) [32]
[21]     0.0    0.00    0.00      28         cv::Rect_<int>::Rect_(int, int, int, int) [21]
-----------------------------------------------
                0.00    0.00      14/28          assembleCubemap(cv::Mat&, float, float*) [33]
                0.00    0.00      14/28          assemblePacked(cv::Mat&, cv::Mat&, float) [32]
[22]     0.0    0.00    0.00      28         cv::Mat::operator()(cv::Rect_<int> const&) const [22]
-----------------------------------------------
                0.00    0.00      12/26          splitFace(cv::Mat const&, cv::Mat&, cv::Mat&, cv::Mat&, int, int) [28]
                0.00    0.00      14/26          assemblePacked(cv::Mat&, cv::Mat&, float) [32]
[23]     0.0    0.00    0.00      26         cv::Mat::operator()(cv::Range, cv::Range) const [23]
-----------------------------------------------
                0.00    0.00       6/6           assembleCubemap(cv::Mat&, float, float*) [33]
[24]     0.0    0.00    0.00       6         createCubeMapFace(cv::Mat const&, cv::Mat&, float*, int, int, int) [24]
                0.00    0.00 44236800/44236800     std::atan2(float, float) [9]
                0.00    0.00 44236800/44236800     float& cv::Mat::at<float>(int, int) [8]
                0.00    0.00 14745600/14745600     std::cos(float) [10]
                0.00    0.00 7372800/7372800     std::sqrt(float) [11]
                0.00    0.00      18/41          cv::_InputArray::_InputArray(cv::Mat const&) [18]
                0.00    0.00      18/97          cv::_InputArray::~_InputArray() [14]
                0.00    0.00       6/6           cv::Scalar_<double>::Scalar_(double, double, double, double) [27]
                0.00    0.00       6/28          cv::_OutputArray::_OutputArray(cv::Mat&) [20]
                0.00    0.00       6/56          cv::_OutputArray::~_OutputArray() [16]
-----------------------------------------------
                0.00    0.00       6/6           cv::Scalar_<double>::Scalar_(double, double, double, double) [27]
[25]     0.0    0.00    0.00       6         cv::Vec<double, 4>::Vec() [25]
                0.00    0.00       6/6           cv::Matx<double, 4, 1>::Matx() [26]
-----------------------------------------------
                0.00    0.00       6/6           cv::Vec<double, 4>::Vec() [25]
[26]     0.0    0.00    0.00       6         cv::Matx<double, 4, 1>::Matx() [26]
-----------------------------------------------
                0.00    0.00       6/6           createCubeMapFace(cv::Mat const&, cv::Mat&, float*, int, int, int) [24]
[27]     0.0    0.00    0.00       6         cv::Scalar_<double>::Scalar_(double, double, double, double) [27]
                0.00    0.00       6/6           cv::Vec<double, 4>::Vec() [25]
-----------------------------------------------
                0.00    0.00       4/4           assembleCubemap(cv::Mat&, float, float*) [33]
[28]     0.0    0.00    0.00       4         splitFace(cv::Mat const&, cv::Mat&, cv::Mat&, cv::Mat&, int, int) [28]
                0.00    0.00      24/52          cv::Range::Range(int, int) [17]
                0.00    0.00      12/26          cv::Mat::operator()(cv::Range, cv::Range) const [23]
                0.00    0.00       8/117         cv::Size_<int>::Size_() [13]
                0.00    0.00       8/28          cv::_OutputArray::_OutputArray(cv::Mat&) [20]
                0.00    0.00       8/41          cv::_InputArray::_InputArray(cv::Mat const&) [18]
                0.00    0.00       8/97          cv::_InputArray::~_InputArray() [14]
                0.00    0.00       8/56          cv::_OutputArray::~_OutputArray() [16]
-----------------------------------------------
                0.00    0.00       2/2           main [6]
[29]     0.0    0.00    0.00       2         importImage(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) [29]
-----------------------------------------------
                0.00    0.00       1/2           assembleCubemap(cv::Mat&, float, float*) [33]
                0.00    0.00       1/2           main [6]
[30]     0.0    0.00    0.00       2         cv::Mat::type() const [30]
-----------------------------------------------
                0.00    0.00       1/1           __libc_csu_init [56]
[31]     0.0    0.00    0.00       1         _GLOBAL__sub_I_faceTransform [31]
                0.00    0.00       1/1           __static_initialization_and_destruction_0(int, int) [34]
-----------------------------------------------
                0.00    0.00       1/1           main [6]
[32]     0.0    0.00    0.00       1         assemblePacked(cv::Mat&, cv::Mat&, float) [32]
                0.00    0.00      28/52          cv::Range::Range(int, int) [17]
                0.00    0.00      24/56          cv::_OutputArray::~_OutputArray() [16]
                0.00    0.00      14/26          cv::Mat::operator()(cv::Range, cv::Range) const [23]
                0.00    0.00      14/28          cv::Rect_<int>::Rect_(int, int, int, int) [21]
                0.00    0.00      14/28          cv::Mat::operator()(cv::Rect_<int> const&) const [22]
                0.00    0.00      14/28          cv::_OutputArray::_OutputArray(cv::Mat const&) [19]
                0.00    0.00      10/117         cv::Size_<int>::Size_() [13]
                0.00    0.00      10/28          cv::_OutputArray::_OutputArray(cv::Mat&) [20]
                0.00    0.00      10/41          cv::_InputArray::_InputArray(cv::Mat const&) [18]
                0.00    0.00      10/97          cv::_InputArray::~_InputArray() [14]
-----------------------------------------------
                0.00    0.00       1/1           main [6]
[33]     0.0    0.00    0.00       1         assembleCubemap(cv::Mat&, float, float*) [33]
                0.00    0.00      18/56          cv::_OutputArray::~_OutputArray() [16]
                0.00    0.00      14/28          cv::Rect_<int>::Rect_(int, int, int, int) [21]
                0.00    0.00      14/28          cv::Mat::operator()(cv::Rect_<int> const&) const [22]
                0.00    0.00      14/28          cv::_OutputArray::_OutputArray(cv::Mat const&) [19]
                0.00    0.00       6/6           createCubeMapFace(cv::Mat const&, cv::Mat&, float*, int, int, int) [24]
                0.00    0.00       4/4           splitFace(cv::Mat const&, cv::Mat&, cv::Mat&, cv::Mat&, int, int) [28]
                0.00    0.00       4/28          cv::_OutputArray::_OutputArray(cv::Mat&) [20]
                0.00    0.00       4/41          cv::_InputArray::_InputArray(cv::Mat const&) [18]
                0.00    0.00       4/97          cv::_InputArray::~_InputArray() [14]
                0.00    0.00       2/117         cv::Size_<int>::Size_() [13]
                0.00    0.00       1/2           cv::Mat::type() const [30]
-----------------------------------------------
                0.00    0.00       1/1           _GLOBAL__sub_I_faceTransform [31]
[34]     0.0    0.00    0.00       1         __static_initialization_and_destruction_0(int, int) [34]
-----------------------------------------------
                0.00    0.00       1/1           std::allocator<int>::allocator() [37]
[35]     0.0    0.00    0.00       1         __gnu_cxx::new_allocator<int>::new_allocator() [35]
-----------------------------------------------
                0.00    0.00       1/1           std::allocator<int>::~allocator() [38]
[36]     0.0    0.00    0.00       1         __gnu_cxx::new_allocator<int>::~new_allocator() [36]
-----------------------------------------------
                0.00    0.00       1/1           std::_Vector_base<int, std::allocator<int> >::_Vector_impl::_Vector_impl() [40]
[37]     0.0    0.00    0.00       1         std::allocator<int>::allocator() [37]
                0.00    0.00       1/1           __gnu_cxx::new_allocator<int>::new_allocator() [35]
-----------------------------------------------
                0.00    0.00       1/1           std::_Vector_base<int, std::allocator<int> >::_Vector_impl::~_Vector_impl() [41]
[38]     0.0    0.00    0.00       1         std::allocator<int>::~allocator() [38]
                0.00    0.00       1/1           __gnu_cxx::new_allocator<int>::~new_allocator() [36]
-----------------------------------------------
                0.00    0.00       1/1           void std::_Destroy<int*>(int*, int*) [49]
[39]     0.0    0.00    0.00       1         void std::_Destroy_aux<true>::__destroy<int*>(int*, int*) [39]
-----------------------------------------------
                0.00    0.00       1/1           std::_Vector_base<int, std::allocator<int> >::_Vector_base() [45]
[40]     0.0    0.00    0.00       1         std::_Vector_base<int, std::allocator<int> >::_Vector_impl::_Vector_impl() [40]
                0.00    0.00       1/1           std::allocator<int>::allocator() [37]
                0.00    0.00       1/1           std::_Vector_base<int, std::allocator<int> >::_Vector_impl_data::_Vector_impl_data() [43]
-----------------------------------------------
                0.00    0.00       1/1           std::_Vector_base<int, std::allocator<int> >::~_Vector_base() [46]
[41]     0.0    0.00    0.00       1         std::_Vector_base<int, std::allocator<int> >::_Vector_impl::~_Vector_impl() [41]
                0.00    0.00       1/1           std::allocator<int>::~allocator() [38]
-----------------------------------------------
                0.00    0.00       1/1           std::_Vector_base<int, std::allocator<int> >::~_Vector_base() [46]
[42]     0.0    0.00    0.00       1         std::_Vector_base<int, std::allocator<int> >::_M_deallocate(int*, unsigned long) [42]
-----------------------------------------------
                0.00    0.00       1/1           std::_Vector_base<int, std::allocator<int> >::_Vector_impl::_Vector_impl() [40]
[43]     0.0    0.00    0.00       1         std::_Vector_base<int, std::allocator<int> >::_Vector_impl_data::_Vector_impl_data() [43]
-----------------------------------------------
                0.00    0.00       1/1           std::vector<int, std::allocator<int> >::~vector() [48]
[44]     0.0    0.00    0.00       1         std::_Vector_base<int, std::allocator<int> >::_M_get_Tp_allocator() [44]
-----------------------------------------------
                0.00    0.00       1/1           std::vector<int, std::allocator<int> >::vector() [47]
[45]     0.0    0.00    0.00       1         std::_Vector_base<int, std::allocator<int> >::_Vector_base() [45]
                0.00    0.00       1/1           std::_Vector_base<int, std::allocator<int> >::_Vector_impl::_Vector_impl() [40]
-----------------------------------------------
                0.00    0.00       1/1           std::vector<int, std::allocator<int> >::~vector() [48]
[46]     0.0    0.00    0.00       1         std::_Vector_base<int, std::allocator<int> >::~_Vector_base() [46]
                0.00    0.00       1/1           std::_Vector_base<int, std::allocator<int> >::_M_deallocate(int*, unsigned long) [42]
                0.00    0.00       1/1           std::_Vector_base<int, std::allocator<int> >::_Vector_impl::~_Vector_impl() [41]
-----------------------------------------------
                0.00    0.00       1/1           main [6]
[47]     0.0    0.00    0.00       1         std::vector<int, std::allocator<int> >::vector() [47]
                0.00    0.00       1/1           std::_Vector_base<int, std::allocator<int> >::_Vector_base() [45]
-----------------------------------------------
                0.00    0.00       1/1           main [6]
[48]     0.0    0.00    0.00       1         std::vector<int, std::allocator<int> >::~vector() [48]
                0.00    0.00       1/1           std::_Vector_base<int, std::allocator<int> >::_M_get_Tp_allocator() [44]
                0.00    0.00       1/1           std::_Vector_base<int, std::allocator<int> >::~_Vector_base() [46]
                0.00    0.00       1/1           void std::_Destroy<int*, int>(int*, int*, std::allocator<int>&) [50]
-----------------------------------------------
                0.00    0.00       1/1           void std::_Destroy<int*, int>(int*, int*, std::allocator<int>&) [50]
[49]     0.0    0.00    0.00       1         void std::_Destroy<int*>(int*, int*) [49]
                0.00    0.00       1/1           void std::_Destroy_aux<true>::__destroy<int*>(int*, int*) [39]
-----------------------------------------------
                0.00    0.00       1/1           std::vector<int, std::allocator<int> >::~vector() [48]
[50]     0.0    0.00    0.00       1         void std::_Destroy<int*, int>(int*, int*, std::allocator<int>&) [50]
                0.00    0.00       1/1           void std::_Destroy<int*>(int*, int*) [49]
-----------------------------------------------

 This table describes the call tree of the program, and was sorted by
 the total amount of time spent in each function and its children.

 Each entry in this table consists of several lines.  The line with the
 index number at the left hand margin lists the current function.
 The lines above it list the functions that called this function,
 and the lines below it list the functions this one called.
 This line lists:
     index	A unique number given to each element of the table.
		Index numbers are sorted numerically.
		The index number is printed next to every function name so
		it is easier to look up where the function is in the table.

     % time	This is the percentage of the `total' time that was spent
		in this function and its children.  Note that due to
		different viewpoints, functions excluded by options, etc,
		these numbers will NOT add up to 100%.

     self	This is the total amount of time spent in this function.

     children	This is the total amount of time propagated into this
		function by its children.

     called	This is the number of times the function was called.
		If the function called itself recursively, the number
		only includes non-recursive calls, and is followed by
		a `+' and the number of recursive calls.

     name	The name of the current function.  The index number is
		printed after it.  If the function is a member of a
		cycle, the cycle number is printed between the
		function's name and the index number.


 For the function's parents, the fields have the following meanings:

     self	This is the amount of time that was propagated directly
		from the function into this parent.

     children	This is the amount of time that was propagated from
		the function's children into this parent.

     called	This is the number of times this parent called the
		function `/' the total number of times the function
		was called.  Recursive calls to the function are not
		included in the number after the `/'.

     name	This is the name of the parent.  The parent's index
		number is printed after it.  If the parent is a
		member of a cycle, the cycle number is printed between
		the name and the index number.

 If the parents of the function cannot be determined, the word
 `<spontaneous>' is printed in the `name' field, and all the other
 fields are blank.

 For the function's children, the fields have the following meanings:

     self	This is the amount of time that was propagated directly
		from the child into the function.

     children	This is the amount of time that was propagated from the
		child's children to the function.

     called	This is the number of times the function called
		this child `/' the total number of times the child
		was called.  Recursive calls by the child are not
		listed in the number after the `/'.

     name	This is the name of the child.  The child's index
		number is printed after it.  If the child is a
		member of a cycle, the cycle number is printed
		between the name and the index number.

 If there are any cycles (circles) in the call graph, there is an
 entry for the cycle-as-a-whole.  This entry shows who called the
 cycle (as parents) and the members of the cycle (as children.)
 The `+' recursive calls entry shows the number of function calls that
 were internal to the cycle, and the calls entry for each member shows,
 for that member, how many times it was called from other members of
 the cycle.

Copyright (C) 2012-2020 Free Software Foundation, Inc.

Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.

Index by function name

  [31] _GLOBAL__sub_I_faceTransform (project.cpp) [25] cv::Vec<double, 4>::Vec() [41] std::_Vector_base<int, std::allocator<int> >::_Vector_impl::~_Vector_impl()
  [29] importImage(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) [26] cv::Matx<double, 4, 1>::Matx() [42] std::_Vector_base<int, std::allocator<int> >::_M_deallocate(int*, unsigned long)
  [32] assemblePacked(cv::Mat&, cv::Mat&, float) [17] cv::Range::Range(int, int) [43] std::_Vector_base<int, std::allocator<int> >::_Vector_impl_data::_Vector_impl_data()
  [33] assembleCubemap(cv::Mat&, float, float*) [21] cv::Rect_<int>::Rect_(int, int, int, int) [44] std::_Vector_base<int, std::allocator<int> >::_M_get_Tp_allocator()
  [24] createCubeMapFace(cv::Mat const&, cv::Mat&, float*, int, int, int) [13] cv::Size_<int>::Size_() [45] std::_Vector_base<int, std::allocator<int> >::_Vector_base()
  [34] __static_initialization_and_destruction_0(int, int) (project.cpp) [27] cv::Scalar_<double>::Scalar_(double, double, double, double) [46] std::_Vector_base<int, std::allocator<int> >::~_Vector_base()
  [28] splitFace(cv::Mat const&, cv::Mat&, cv::Mat&, cv::Mat&, int, int) [35] __gnu_cxx::new_allocator<int>::new_allocator() [47] std::vector<int, std::allocator<int> >::vector()
  [12] cv::_InputArray::init(int, void const*) [36] __gnu_cxx::new_allocator<int>::~new_allocator() [48] std::vector<int, std::allocator<int> >::~vector()
  [15] cv::_InputArray::_InputArray() [30] cv::Mat::type() const [10] std::cos(float)
  [18] cv::_InputArray::_InputArray(cv::Mat const&) [23] cv::Mat::operator()(cv::Range, cv::Range) const [11] std::sqrt(float)
  [14] cv::_InputArray::~_InputArray() [22] cv::Mat::operator()(cv::Rect_<int> const&) const [9] std::atan2(float, float)
  [19] cv::_OutputArray::_OutputArray(cv::Mat const&) [37] std::allocator<int>::allocator() [49] void std::_Destroy<int*>(int*, int*)
  [20] cv::_OutputArray::_OutputArray(cv::Mat&) [38] std::allocator<int>::~allocator() [50] void std::_Destroy<int*, int>(int*, int*, std::allocator<int>&)
  [16] cv::_OutputArray::~_OutputArray() [39] void std::_Destroy_aux<true>::__destroy<int*>(int*, int*)
   [8] float& cv::Mat::at<float>(int, int) [40] std::_Vector_base<int, std::allocator<int> >::_Vector_impl::_Vector_impl()
