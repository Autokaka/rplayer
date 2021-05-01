part of rplayer;

class RPlayerView extends StatefulWidget {
  final RPlayer player;
  final double height;
  final double width;
  final Widget placeholder;
  final Widget child;

  RPlayerView({
    required this.player,
    this.height = 0,
    this.width = 0,
    this.placeholder = const SizedBox(),
    this.child = const SizedBox(),
  });

  @override
  _RPlayerViewState createState() => _RPlayerViewState();
}

class _RPlayerViewState extends State<RPlayerView> {
  int textureId = -1;

  void didChangePlayerState() {
    if (widget.player.textureId != textureId) {
      setState(() {
        textureId = widget.player.textureId;
      });
    }
  }

  @override
  void initState() {
    super.initState();
    widget.player.addListener(didChangePlayerState);
  }

  @override
  Widget build(BuildContext context) {
    return Hero(
      tag: "Texture_$textureId",
      child: Container(
        height: widget.height,
        width: widget.width,
        color: Colors.black,
        child: Stack(
          children: [
            Center(
              child: (widget.player.textureId < 0)
                  ? widget.placeholder
                  : AspectRatio(
                      aspectRatio: widget.player.size.aspectRatio,
                      child: Texture(textureId: textureId),
                    ),
            ),
            widget.child,
          ],
        ),
      ),
    );
  }

  @override
  void dispose() {
    widget.player.removeListener(didChangePlayerState);
    widget.player.dispose();
    super.dispose();
  }
}
