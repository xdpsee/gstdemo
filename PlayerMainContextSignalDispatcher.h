#ifndef __PLAYER_MAIN_CONTEXT_SIGNAL_DISPATCHER_H__
#define __PLAYER_MAIN_CONTEXT_SIGNAL_DISPATCHER_H__

#include "PlayerTypes.h"
#include "PlayerSignalDispatcher.h"

G_BEGIN_DECLS

typedef struct _PlayerMainContextSignalDispatcher PlayerMainContextSignalDispatcher;
typedef struct _PlayerMainContextSignalDispatcherClass PlayerMainContextSignalDispatcherClass;

#define GST_TYPE_PLAYER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER             (player_main_context_signal_dispatcher_get_type ())
#define GST_IS_PLAYER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_PLAYER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER))
#define GST_IS_PLAYER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_PLAYER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER))
#define GST_PLAYER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_PLAYER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER, PlayerMainContextSignalDispatcherClass))
#define GST_PLAYER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_PLAYER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER, PlayerMainContextSignalDispatcher))
#define GST_PLAYER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_PLAYER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER, PlayerMainContextSignalDispatcherClass))
#define GST_PLAYER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER_CAST(obj)        ((GstPlayerGMainContextSignalDispatcher*)(obj))

GType player_main_context_signal_dispatcher_get_type(void);

PlayerSignalDispatcher *player_main_context_signal_dispatcher_new(GMainContext *application_context);

G_END_DECLS

#endif /* __PLAYER_MAIN_CONTEXT_SIGNAL_DISPATCHER_H__ */
