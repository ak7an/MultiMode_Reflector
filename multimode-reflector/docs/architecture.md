# Architecture Notes

Current pipeline:

UDP packet
→ Protocol detector
→ Protocol handler
→ MediaFrame
→ MediaRouter
→ RouteAction
→ JitterBuffer
→ ProtocolEncoder
→ outbound packet

Design goal:

Keep protocol-specific logic at the edges.
Use MediaFrame as the internal common media representation.
Keep transport layer as dumb as possible.
Centralize routing and transcoding decisions in MediaRouter.
