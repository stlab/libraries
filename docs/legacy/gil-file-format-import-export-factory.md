---
title: GIL File Format Import/Export Factory
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/GIL_File_Format_Import/Export_Factory
---
#### Contents
{:.no_toc}
1. contents
{:toc}

## Basic Description

The GIL IO Factory is intended to be a generic way of:
* Detecting the image file format of a given file on disk (detection)
* Loading the image data from an on-disk file into an <code>adobe::gil</code> image or <code>adobe::gil</code> variant (import)
* Saving an <code>adobe::gil</code> image or <code>adobe::gil</code> variant to a file (of specific format) on-disk (export)

## API

### Location

<pre>
<adobe/gil/extension/io/io_factory.hpp>
</pre>

### ASL Public Interface

```cpp
template <typename ViewType>
class image_factory_t
{
    typedef ViewType                  view_type;
    typedef GIL::image<view_type>     image_type;
    typedef image_format_t<view_type> image_format_type;

    void register_format(const image_format_type& format)

    bool is_registered(adobe::name_t format_tag);

    void unregister_format(adobe::name_t format_tag)

    template <typename O>
    O detect_all_for(std::streambuf& stream_buffer,
                     O               output) const;

    adobe::name_t read(image_type&          image,
                       std::streambuf&      stream_buffer,
                       adobe::dictionary_t& parameters,
                       adobe::name_t        format_tag = adobe::name_t())

    void write(const view_type&     image_view,
               std::streambuf&      stream_buffer,
               adobe::name_t        format_tag,
               adobe::dictionary_t  parameters = adobe::dictionary_t())
};
```

### Notes

<code>image_format_t<view_type></code> is a runtime-polymorphic API that leverages <code>adobe::regular_object</code> under the hood to maintain a list of possible io format factories. This gives each factory the ability to be implemented separately from the IO factory as a whole, who then can be registered with the factory with no intrusiveness into the module's implementation.

<code>register_format</code> takes an <code>adobe::regular_object</code>-wrapped IO factory module and stores it as a candidate for future file detection, reading and writing. Each <code>image_format_t</code> registered with the IO factory needs to have a unique tag (stored as an <code>adobe::name_t</code>) for identification purposes.

<code>is_registered</code> will return whether or not an IO factory module has already been registered under a specified tag name.

<code>unregister_format</code> will disconnect the IO factory module registered under a specified tag name from the IO factory.

<code>detect_all_for</code> is a means by which one can obtain all the possible registered IO module tags for which the <code>detect</code> call over a specified stream returns <code>true</code>. These tags can be used later to perform <code>read</code> and <code>write</code> operations using a specific registered IO module.

<code>read</code> will take in image data from a std::streambuf and inject it into an <code>adobe::gil</code> image as specified by the template type of the IO factory. The <code>format_tag</code> parameter can be used to explicitly identify the UI module the client would like to use in order to perform the read operation. In the case when this is not supplied, the IO factory will first traverse the list of IO modules for the first candidate for whom their <code>detect</code> API returns <code>true</code>, and use that module for the read. The <code>parameters</code> parameter is an <code>adobe::dictionary_t</code> that, upon return, will contain metadata about the file that was read. The contents of this dictionary are implementation-defined according to the IO module specification. The return value of the <code>read</code> API will be the tag of the IO module used to perform the read.

<code>write</code> will take in image data from an <code>adobe::gil</code> image and inject it into a <code>std::streambuf</code>. The <code>format_tag</code> parameter can be used to explicitly identify the UI module the client would like to use in order to perform the write operation, and is required. The <code>parameters</code> parameter is an <code>adobe::dictionary_t</code> that can be used to supply metadata to the IO module to assist in saving the file format.

When the <code>image_factory_t::read</code> API is invoked, the factory will iterate through the registered file format IO modules to find the first one whose <code>detect</code> function returns <code>true</code>. Once this candidate is found, it is given the path to the file on which the import needs to be done, and the IO module returns the <code>adobe::gil</code> image as a result. This result can then be fed into the <code>adobe::image_t</code> widget.

## Issues and Future Considerations

* Can the IO Factory leverage GIL more throughly?
* What about file metadata?
  * We might be able to leverage XMP for that
  * Can we preserve what's in the file so that round-trips through the IO Factory don't strip the metadata out?
* What about parameter blocks for scripting import/export?
  * What about the case when more information is needed than what the param block provides?
  * We might be able to leverage the ASL modal dialog interface for that
* The detect call should return an adobe::dictionary_t filled with metadata discovered about the image during the time of detection, if applicable. The contents of the adobe::dictionary_t are implementation-defined based on the io module that handled the detection. A common set of keys should be defined to help the client of the IO factory discern what metadata values are present. Keys might be some of:
  * height
  * width
  * bits_per_channel
  * bits_per_pixel
  * xmp (and/or other metadata format names (exif, etc.))
  * channel_count
  * colorspace (with specific valid enumerations as possible values)
* The IO Factory needs to be templated based on the ImageType, not the ViewType, of the image format with which we are dealing. This is because the ViewType can be derived from the ImageType, but not the other way around.
* We should move away from <code>std::streambuf</code> in the API and to <code>boost::filesystem::path</code>, as some IO modules will have internal libraries that want file names to open, not buffers from which to read. By <code>using boost::filesystem::path</code> we concede to a specification that is a superset over std::streambuf.
* The file's extension should also be passed through the API (NOTE: this will be fixed by the fix to migrate from std::streambuf to boost::filesystem::path.)
