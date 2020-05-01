#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "player-signal-dispatcher.h"
#include "player-signal-dispatcher-private.h"

G_DEFINE_INTERFACE (PlayerSignalDispatcher, player_signal_dispatcher,
    G_TYPE_OBJECT);

static void
player_signal_dispatcher_default_init (G_GNUC_UNUSED
    PlayerSignalDispatcherInterface * iface)
{

}

void
player_signal_dispatcher_dispatch (PlayerSignalDispatcher * self,
                                   Player * player, PlayerSignalDispatcherFunc emitter, gpointer data,
                                   GDestroyNotify destroy)
{
  PlayerSignalDispatcherInterface *iface;

  if (!self) {
    emitter (data);
    if (destroy)
      destroy (data);
    return;
  }

  g_return_if_fail (GST_IS_PLAYER_SIGNAL_DISPATCHER (self));
  iface = GST_PLAYER_SIGNAL_DISPATCHER_GET_INTERFACE (self);
  g_return_if_fail (iface->dispatch != NULL);

  iface->dispatch (self, player, emitter, data, destroy);
}
