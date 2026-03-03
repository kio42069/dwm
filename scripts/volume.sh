VOLUME=$(pamixer --get-volume)
STATUS=$(pamixer --get-mute)

if [ "$STATUS" = "false" ]; then
  ICON="🔈"
else
  ICON="🔇"
  VOLUME="Muted"
fi
echo "^c#eb45f7^$ICON $VOLUME ^d^"
