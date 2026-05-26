# Routing Notes

MediaRouter currently returns:

- RouteAction
- reason
- destinations
- transcodedFrames

Current destination model supports:

- ALL
- PEER
- PROTOCOL
- MODULE
- BRIDGE

Current default destination is ALL.

Future goal:

Replace broadcast forwarding with router-selected destinations.
