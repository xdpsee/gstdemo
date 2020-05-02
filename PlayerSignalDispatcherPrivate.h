#ifndef __PLAYER_SIGNAL_DISPATCHER_PRIVATE_H__
#define __PLAYER_SIGNAL_DISPATCHER_PRIVATE_H__

#include "PlayerSignalDispatcher.h"

G_BEGIN_DECLS

G_GNUC_INTERNAL void player_signal_dispatcher_dispatch (PlayerSignalDispatcher * self,
                                                        Player * player,
                                                        PlayerSignalDispatcherFunc emitter,
                                                        gpointer data,
                                                        GDestroyNotify destroy);

G_END_DECLS

#endif /* __PLAYER_SIGNAL_DISPATCHER_PRIVATE_H__ */
