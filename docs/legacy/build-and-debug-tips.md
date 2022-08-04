---
title: Build and Debug Tips
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Build_and_Debug_Tips
---
#### Contents
{:.no_toc}
1. contents
{:toc}

## MSVC

* Debugging msvc-built command-line ASL test apps from the command line:
  1. _To keep bjam from destroying test executables_ build with bjam --preserve-test-targets
  2. _For adam_smoke under cygwin:_ "C:/Program Files/Microsoft Visual Studio 8/Common7/IDE/devenv.exe" /DebugExe bin/adobe/test/adam_smoke/adam_smoke_test.test/msvc-8.0/debug/asl-dev/link-static/threading-multi/adam_smoke_test.exe ./adobe/test/adam_smoke/rtd
  3. F10 (to stop on first line of main), or F5 to let it rip

* Towards natural display of any_regular and poly<> in msvc and xcode debuggers.
  * C:\Program Files\Microsoft Visual Studio 8\Common7\Packages\Debugger\autoexp.dat can be extended, but not sure how useful it is. Perhaps <https://msdn.microsoft.com/en-us/library/8fwk67y3(vs.71).aspx> would be more useful for some common types.
  * Xcode has  Debug > Variables View > Edit Summary Format and ~/Library/Application Support/Apple/Developer Tools/CustomDataViews/CustomDataViews.plist , but I haven't explored it much either.

## Xcode

* Xcode uses summary expressions to display information about a variable during the debug process. These summary expressions can be entered in at the time you are debugging a variable. For example, to get the type of a value held by an adobe::any_regular_t, put this into the summary expression:<br/>
`type={$VAR.type_info().identity_m.name_m}:s`
