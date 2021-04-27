part of rplayer;

class RPlayerView extends StatefulWidget {
  final RPlayer player;

  RPlayerView(this.player);

  @override
  _RPlayerViewState createState() => _RPlayerViewState();
}

class _RPlayerViewState extends State<RPlayerView> {
  int textureId = -1;

  void _playerListener() {
    if (widget.player.textureId < 0) {
      return;
    }

    setState(() {
      textureId = widget.player.textureId;
    });
  }

  @override
  void initState() {
    super.initState();
    widget.player.addListener(_playerListener);
  }

  @override
  Widget build(BuildContext context) {
    if (textureId < 0) {
      return Center(
        child: Text("loading..."),
      );
    }

    return AspectRatio(
      aspectRatio: widget.player.size.aspectRatio,
      child: Texture(textureId: textureId),
    );
  }

  @override
  void dispose() {
    widget.player.removeListener(_playerListener);
    widget.player.dispose();
    super.dispose();
  }
}
