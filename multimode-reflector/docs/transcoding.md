# Transcoding Notes

Current transcoder is a stub.

Current proven path:

D-Star packet
→ MediaFrame(DSTAR)
→ RouteAction::TRANSCODE
→ Transcoder stub
→ MediaFrame(YSF)
→ ProtocolEncoder
→ YSFEncoder synthetic packet
→ UDP output

Future path:

source protocol frame
→ codec extraction
→ AMBE/codec decode
→ normalized audio/media
→ target codec encode
→ target protocol encoder
