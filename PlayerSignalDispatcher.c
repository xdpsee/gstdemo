#include "PlayerSignalDispatcher.h"
#include "PlayerSignalDispatcherPrivate.h"

G_DEFINE_INTERFACE (PlayerSignalDispatcher, player_signal_dispatcher, G_TYPE_OBJECT);

static void player_signal_dispatcher_default_init(PlayerSignalDispatcherInterface *interface) {

}

void player_signal_dispatcher_dispatch(PlayerSignalDispatcher *self,
                                       Player *player,
                                       PlayerSignalDispatcherFunc emitter,
                                       gpointer data,
                                       GDestroyNotify destroy) {
    PlayerSignalDispatcherInterface *interface;

    if (!self) {
        emitter(data);
        if (destroy) {
            destroy(data);
        }
        return;
    }

    g_return_if_fail (GST_IS_PLAYER_SIGNAL_DISPATCHER(self));
    interface = GST_PLAYER_SIGNAL_DISPATCHER_GET_INTERFACE (self);
    g_return_if_fail (interface->dispatch != NULL);

    interface->dispatch(self, player, emitter, data, destroy);
}
