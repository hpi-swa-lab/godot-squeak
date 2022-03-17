Metacello new
  baseline: 'Godot';
  repository: 'github://hpi-swa-lab/godot-squeak:main/squeak';
  load.

GDMessageListener restart.

Smalltalk snapshot: true andQuit: true
