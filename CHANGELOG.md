# Changelog

All notable changes to the software are documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/).

## [1.0] - 2018-06-30 - Open Source Release

### Added

- Common
    - Added integrated help manual
- All 3D views (Inspection, Fragment, ReAssembly)
    - Added lighting options
- Inspection View and Fragment View
    - Added parallel visualization of:
      - Geometric properties
      - Facets
      - Annotations
      - Minimal Bounding Box
    - Added Distance Measurement tool
    - Added Value Picker tool (to measure the value of a property in a specific point)
- Fragment View
    - Full integration of the feature recognition functionality
- ReAssembly View
    - GUI for visualizing the ReAssembly results
- History View
    - Added space for text notes
- Groups View
    - Added UI for writing text notes

### Changed

- Improved UI layout: added support for High DPI screens
- Improved wireframe visualization mode
- Removed Python dependency: now the parsing of RDF triples is done in pure C++
thanks to the [Serd][serd-home] and [Sord][sord-home] libraries.
- Updated the partonomy with an improved version

[serd-home]: https://drobilla.net/software/serd
[sord-home]: https://drobilla.net/software/sord

## Release 3 (MS8) - 2017-11-30

### Added

- Facets visualization
- Shape Diameter Function property visualization
- Minimal Bounding Box visualization
- Manual Selection and Part Annotation functionalities (annotation is
saved on local disk as a temporary solution)
- Groups view (create/edit/delete groups)
- Artefact metadata organized in a tree structure

### Changed

- Clipboard two-way synchronization
- Clipboard panel has been improved to match the Web UI functionalities
- Lightness property is coloured using a grey scale palette

### Security

- Added support to secure HTTP protocol

## Release 2 (MS7) - 2017-04-30

### Added

- Shape Index property visualization
- Lightness property visualization (L channel of the CIELab space)
- Load and visualize the Cultural Heritage Artefact Partonomy, expressed
as a SKOS vocabulary
- Clipboard: one-way synchronization (from web to desktop)
- ReAssembly view skeleton

### Changed

- Models can be visualized in four different resolutions: Full, 1 Million vertices,
100K vertices, 50K vertices
- Improved the list of metadata displayed in the Inspection and Fragment view

## Release 1 (MS6) - 2016-12-12

### Added

- Initial implementation of the Inspection and Fragment view
- Loading and visualization of one or more artefacts (3D model + metadata)
- Graphical interaction functionalities (moving/zooming 3D models)
- Visualization of scalar properties computed on the 3D model,
using a color transfer function
- Mean curvature property visualization
- Hard-coded clipboard (no synchronization with the back-end)
