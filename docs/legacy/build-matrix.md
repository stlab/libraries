---
title: Build Matrix
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Build_Matrix
---
#### Contents
{:.no_toc}
1. contents
{:toc}

## Policies

* Anyone may edit the build matrix at any time
* Use the following format when filling in a square: {changelist} {initials} {details}
  * changelist is the changelist against which you tested this square
  * initials are your initials
  * details are any more details such as bug numbers, questions, etc. (if necessary)
* Color code the background of the matrix according to the status
  * Green (bgcolor='lightgrey') All is ok.
  * Yellow (bgcolor='yellow') Outstanding issue under investigation
    * Please mark the box with a note. If it is extended, use the <nowiki>[name](#subsection)</nowiki> notation. (Example: [1](#win32-bjam-issues))
  * Red (bgcolor='red') Failure
    * Please mark the box with a submitted [SourceForge.net](https://sourceforge.net/p/adobe-source/bugs/?page=1) bug number.

## QA Matrix for Macintosh

<table border="1" cellpadding="5" cellspacing="0">
    <tr>
        <td rowspan="4"></td>
        <th colspan="8">Mac</th>
    </tr>
    <tr>
        <th colspan="4">XCode 2.4 PPC</th>
        <th colspan="4">XCode 2.4 Intel</th>
    </tr>
    <tr>
        <td colspan="2" align="center">IDE</td>
        <td colspan="2" align="center">bjam</td>
        <td colspan="2" align="center">IDE</td>
        <td colspan="2" align="center">bjam</td>
    </tr>
    <tr>
        <td align="center">Debug</td>
        <td align="center">Release</td>
        <td align="center">Debug</td>
        <td align="center">Release</td>
        <td align="center">Debug</td>
        <td align="center">Release</td>
        <td align="center">Debug</td>
        <td align="center">Release</td>
    </tr>
    <tr>
        <th width='100'>Build (Adobe Begin and the bjam test suites)</th>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>681 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='green'>681 FB</td>
        <!-- XCode PPC bjam Debug --><td bgcolor='yellow' markdown="span">
            681 FB [2](#mac-os-bjam-issues)
        </td>
        <!-- XCode PPC bjam Release --><td bgcolor='yellow' markdown="span">
            681 FB [2](#mac-os-bjam-issues)
        </td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='green'>681 SP</td>
        <!-- XCode MacTel IDE Release --><td bgcolor='green'>681 SP</td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='yellow' markdown="span">
            681 SP [2](#mac-os-bjam-issues)
        </td>
        <!-- XCode MacTel bjam Release --><td bgcolor='yellow' markdown="span">
            681 SP [2](#mac-os-bjam-issues)
        </td>
    </tr>
    <tr>
        <td>alert</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>692 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>align_reverse</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>692 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>align_right_test</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>692 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>alignment_test</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>692 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>big_popup</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>692 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>checkbox_control</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>692 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>checkbox_radio_button_suite</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>692 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>cycle_test</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>692 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>e01_clipping_path</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>692 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>edit_number_suite</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>692 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>edit_text_suite</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>692 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>empty_containers</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>692 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>empty_dialog</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>692 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>find_and_replace (2 eve files)</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>692 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>grow</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>692 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>image_size (2 eve files)</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='yellow'>692 FB Bug #1533337</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>inequality</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>692 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>mini_image_size (3 eve files)</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>701 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>named_function_suite</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>692 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>nested_group</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>692 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>optional_panel</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>701 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>overconstrain</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green' markdown="span">
            692 FB [3](#stderr-issues)
        </td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>progress_bar_suite</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>692 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>slider_suite</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='yellow'>692 FB Bug# 1533468</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>sort_lines</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>692 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>tabs_and_panels</td>
        <!-- XCode PPC IDE Debug --><td bgcolor='green'>692 FB</td>
        <!-- XCode PPC IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode PPC bjam Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel IDE Release --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- XCode MacTel bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
</table>

## QA Matrix for Windows

<table border="1" cellpadding="5" cellspacing="0">
    <tr>
        <td rowspan = '4'></td>
        <th colspan = '6'>Win</th>
    </tr>
    <tr>
        <th colspan='4'>MSVC 8</th>
        <th colspan='2'>MSVC 7.1</th>
    </tr>
    <tr>
        <td colspan='2' align='center'>IDE</td>
        <td colspan='2' align='center'>bjam</td>
        <td colspan='2' align='center'>bjam</td>
    </tr>
    <tr>
        <td align='center'>Debug</td>
        <td align='center'>Release</td>
        <td align='center'>Debug</td>
        <td align='center'>Release</td>
        <td align='center'>Debug</td>
        <td align='center'>Release</td>
    </tr>
    <tr>
        <th width='100'>Build (Adobe Begin and the bjam test suites)</th>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>681 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='green'>689 FB</td>
        <!-- MSVC8 bjam Debug --><td bgcolor='green'>689 FB</td>
        <!-- MSVC8 bjam Release --><td bgcolor='green'>689 FB</td>
        <!-- MSVC71 bjam Debug --><td bgcolor='green'>689 FB</td>
        <!-- MSVC71 bjam Release --><td bgcolor='green'>689 FB</td>
    </tr>
    <tr>
        <td>alert</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>681 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>align_reverse</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>682 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>align_right_test</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>682 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>alignment_test</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>682 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>big_popup</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>682 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>checkbox_control</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>682 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>checkbox_radio_button_suite</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>682 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>cycle_test</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>682 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>e01_clipping_path</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>682 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>edit_number_suite</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>682 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>edit_text_suite</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>683 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>empty_containers</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>683 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>empty_dialog</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>683 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>find_and_replace (2 eve files)</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>683 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>grow</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>683 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>image_size (2 eve files)</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='yellow'>689 FB Bug# 1533337</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>inequality</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>689 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>mini_image_size (3 eve files)</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>701 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>named_function_suite</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>689 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>nested_group</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>689 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>optional_panel</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>701 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>overconstrain</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='yellow' markdown='span'>
            689 FB [3](#stderr-issues)</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>progress_bar_suite</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>689 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>slider_suite</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>689 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>sort_lines</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>689 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
    <tr>
        <td>tabs_and_panels</td>
        <!-- MSVC8 IDE Debug --><td bgcolor='green'>689 FB</td>
        <!-- MSVC8 IDE Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC8 bjam Release --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Debug --><td bgcolor='lightgrey'></td>
        <!-- MSVC71 bjam Release --><td bgcolor='lightgrey'></td>
    </tr>
</table>

## QA Matrix for Other Stuff

<table border="1" cellpadding="5" cellspacing="0">
    <tr>
        <td>Cygwin GCC 3.4.4 bjam Debug</td>
        <td bgcolor='green'>689 FB</td>
    </tr>
    <tr>
        <td>Cygwin GCC 3.4.4 bjam Release</td>
        <td bgcolor='green'>689 FB</td>
    </tr>
    <tr>
        <td>Doxygen documentation Build</td>
        <td bgcolor='lightgray'></td>
    </tr>
</table>

## Win32 bjam Issues {#win32-bjam-issues}

Due to the bjam hackery involved in getting the manifest files to copy to their destinations while building with bjam, the debug/release variants must each be built with separate commands. i.e., the command:
<pre>
bjam msvc-7.1 debug release
</pre>
...will fail for debug, while
<pre>
bjam msvc-7.1 debug
bjam msvc-7.1 release
</pre>
...will succeed.

## Mac OS bjam Issues {#mac-os-bjam-issues}
The 'install' stage of the bjam build process is hackneyed due to lack of an engineer fluent in bjamese. In some cases the copying of one or more file(s) will fail, in which case the destination file is then deleted successfully. A subsequent execution of bjam will then copy the file successfully to its final destination.

## stderr Issues {#stderr-issues}
Errors reported to stderr will go to the console on MacOS X; access to stderr is not easy on win32. An alternative should be explored.
