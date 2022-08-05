---
title: On-Disk File Monitor
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/On-Disk_File_Monitor
---
#### Contents
{:.no_toc}
1. contents
{:toc}

## Purpose

The purpose of the on-disk file monitor is to serve as a controller to the client application, notifying the client app when a file on-disk has had an operation applied to it. A single file monitor instance monitors a single file for on-disk changes. Current implementations exist for Carbon and Win32. <code>file_monitor_t</code> is available in ASL release 1.0.16 or later.

## Location

```cpp
#include <adobe/future/file_monitor.hpp>
```

## API

### Free Functions and Typedefs

```cpp
enum file_monitor_event_t
{
    file_monitor_contents_changed_k = 0,
    file_monitor_other_k // later expand this to deleted, moved, renamed, etc.
};
```

These are the types of changes that can happen to a file on-disk that the client application might be interested in. Currently the only supported operation is notifying when the file's contents have been modified on-disk. Other operations (file moved, deleted, renamed, etc.) should be a part of the notification suite at a later date.

```cpp
typedef boost::filesystem::path file_monitor_path_type;
```

<code>file_monitor_path_type</code> is a common associated type for the file monitor system, and is available to support the platform-specific implementation code.

```cpp
typedef boost::function<void (const file_monitor_path_type& path, file_monitor_event_t what)> file_monitor_callback_t;
```

<code>file_monitor_callback_t</code> is the signature for the callback routine the file monitor system will notify when a file at a specified path is modified on-disk. The <code>what</code> parameter enumerates what the on-disk change is, while the <code>path</code> is the path to the file as it currently stands on disk. In future revisions of the system, this will not necessarily be the same path as the one for which the monitor was constructed (e.g., if the file was renamed, moved, or deleted, the path could change.)

### <code>file_monitor_t</code> Class

```cpp
class file_monitor_t
{
public:
    file_monitor_t();
```

Default constructor. Wheeee.

```cpp
    file_monitor_t(const file_monitor_path_type& path, const file_monitor_callback_t& proc);
```

This constructor is used to specify a path and a callback for this file monitor.

```cpp
    void set_path(const file_monitor_path_type& path);
```

<code>set_path</code> is what the client calls if they would like to change the file to be monitored on-disk, but would like to keep the notificiation callback routine the same.

```cpp
    void monitor(const file_monitor_callback_t& proc);
```

<code>monitor</code> is what the client calls with a callback routine if they'd like to specify a new callback for this file monitor.

```cpp
private:
    file_monitor_platform_data_t plat_m;
};
```

<code>file_monitor_platform_data_t</code> is a platform-specific structure and/or support code that is the implementation of file monitor for this platform. Both the declaration and the definition are implementation-defined.
