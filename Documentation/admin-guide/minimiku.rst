================================
MiniMiku Kernel Boot Parameters
================================

MiniMiku supports additional kernel command-line parameters for
distribution-specific functionality.

minimiku.safemode
=================

This parameter enables MiniMiku safe mode.

Usage::

    minimiku.safemode

When supplied on the kernel command line, MiniMiku sets its
safe-mode flag during kernel initialization.

The parameter is intended to provide a foundation for reduced-
functionality booting and troubleshooting.

Note::

    Recognizing this parameter alone does not disable drivers,
    change kernel configuration, or guarantee a safer boot.
    Additional safe-mode behavior must be implemented separately.