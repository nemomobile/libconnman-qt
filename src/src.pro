TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = connman

!isEmpty(QT.quick.name) {
    src_imports.subdir = imports
    src_imports.depends = connman

    SUBDIRS += src_imports
}
