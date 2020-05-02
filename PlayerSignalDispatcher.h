#ifndef __PLAYER_SIGNAL_DISPATCHER_H__
#define __PLAYER_SIGNAL_DISPATCHER_H__

#include <gst/gst.h>
#include "PlayerTypes.h"

G_BEGIN_DECLS

typedef struct _PlayerSignalDispatcher PlayerSignalDispatcher;
typedef struct _PlayerSignalDispatcherInterface PlayerSignalDispatcherInterface;

#define GST_TYPE_PLAYER_SIGNAL_DISPATCHER                (player_signal_dispatcher_get_type ())
#define GST_PLAYER_SIGNAL_DISPATCHER(obj)                (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_PLAYER_SIGNAL_DISPATCHER, PlayerSignalDispatcher))
#define GST_IS_PLAYER_SIGNAL_DISPATCHER(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_PLAYER_SIGNAL_DISPATCHER))
#define GST_PLAYER_SIGNAL_DISPATCHER_GET_INTERFACE(inst) (G_TYPE_INSTANCE_GET_INTERFACE ((inst), GST_TYPE_PLAYER_SIGNAL_DISPATCHER, PlayerSignalDispatcherInterface))

typedef void (*PlayerSignalDispatcherFunc)(gpointer data);

struct _PlayerSignalDispatcherInterface {
    GTypeInterface parent_interface;

    void (*dispatch)(PlayerSignalDispatcher *self,
                     Player *player,
                     PlayerSignalDispatcherFunc emitter,
                     gpointer data,
                     GDestroyNotify destroy);
};

GType player_signal_dispatcher_get_type(void);

G_END_DECLS

#endif /* __PLAYER_SIGNAL_DISPATCHER_H__ */
