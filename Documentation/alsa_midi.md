Connecting ALSA MIDI to Procdraw
================================

```
$ aconnect -l
client 0: 'System' [type=kernel]
    0 'Timer           '
    1 'Announce        '
client 14: 'Midi Through' [type=kernel]
    0 'Midi Through Port-0'
client 24: 'Alias_8' [type=kernel]
    0 'Alias_8 MIDI 1  '
    1 'Alias_8 MIDI 2  '
client 128: 'Procdraw' [type=user]
    0 'Procdraw Input  '

$ aconnect 24:0 128:0
```
