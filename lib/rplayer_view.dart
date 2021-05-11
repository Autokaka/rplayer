part of rplayer;

class RPlayerView extends StatefulWidget {
  final RPlayer player;
  final double height;
  final double width;
  final Widget? placeholder;
  final Widget child;
  final Widget? error;

  RPlayerView({
    required this.player,
    this.height = 0,
    this.width = 0,
    this.placeholder,
    this.child = const SizedBox(),
    this.error,
  });

  @override
  _RPlayerViewState createState() => _RPlayerViewState();
}

class _RPlayerViewState extends State<RPlayerView> {
  bool showTextureWidget = false;

  void didChangePlayerState() {
    if (widget.player.textureId >= 0 && widget.player.size != Size.zero) {
      setState(() {
        showTextureWidget = true;
      });
    }
    if (widget.player.state == RPlayerState.ERROR) {
      setState(() {
        showTextureWidget = false;
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
    return Container(
      height: widget.height,
      width: widget.width,
      color: Colors.black,
      child: Stack(
        children: [
          Center(
            child: (showTextureWidget)
                ? AspectRatio(
                    aspectRatio: widget.player.size.aspectRatio,
                    child: Texture(textureId: widget.player.textureId),
                  )
                : (widget.player.state == RPlayerState.ERROR)
                ? widget.error ??
                    Center(
                      child: Text(
                        'Error: ${widget.player.message}',
                        style: TextStyle(color: Colors.white),
                      ),
                    )
                : widget.placeholder ??
                    SizedBox(
                      height: 40,
                      width: 40,
                      child: CircularProgressIndicator(
                        valueColor: AlwaysStoppedAnimation(Colors.white),
                      ),
                    ),
          ),
          widget.child,
        ],
      ),
    );
  }

  @override
  void dispose() {
    widget.player.removeListener(didChangePlayerState);
    super.dispose();
  }
}
