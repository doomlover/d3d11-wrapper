# DirectX 11 Wrapper  
**Author: Sean Pesce**  


## Overview  
`d3d11.dll` wrapper for intercepting DirectX 11 function calls.  


## Built With  
 * [Microsoft Visual C++](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads)  
 * [Visual Studio 2017](https://www.visualstudio.com/)  
 * [inih](https://github.com/benhoyt/inih) - .INI file parsing library  


## License  
[GNU General Public License v3.0](LICENSE)  


---------------------------------------------

## Contact  
For inquiries and/or information about me, visit my **[personal website](https://SeanPesce.github.io)**.  

---------------------------------------------

# 使用方法
可以用于MuMu模拟器。将编译的d3d11.dll文件放入D:\Program Files\Netease\MuMuPlayer-12.0\shell下面，启动mumu就可以了，按F12截帧。

运行renderdoc，执行File->Attach to Running Instance，选MuMuVMMHeadLess就可以看到截帧了。

# 发布说明
## 2023.06.25
升级到vs2022，增加renderdoc注入
