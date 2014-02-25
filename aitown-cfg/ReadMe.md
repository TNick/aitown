aitown-cfg                         {#aitown_cfg}
==========

For a high level overview of the library and design guide-lines read the
[design page](http://tnick.github.io/aitown/reference/aitown-cfg.html).

The main structure for the module is aitown_cfg_t. It must be prepared prior
to use and terminated when no longer needed:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
aitown_cfg_t cfg;
aitown_cfg_init (&cfg);
// ...
aitown_cfg_end (&cfg);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




