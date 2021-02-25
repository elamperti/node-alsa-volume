{
  "targets": [
    {
      "target_name": "NativeExtension",
      "sources": [
        "NativeExtension.cc",
        "functions.cc",
        "alsa-getvolume.c",
        "alsa-setvolume.c"
      ],
      "include_dirs" : [
        "<!(node -e \"require('nan')\")"
      ],
      "libraries": [
          "-lasound"
      ]
    }
  ],
}