#! /bin/bash
### BEGIN INIT INFO
# Provides: testdaemon
# Required-Start:
# Should-Start:
# Required-Stop:
# Should-Stop:
# Default-Start:  3 5
# Default-Stop:   0 1 2 6
# Short-Description: Test daemon process
# Description:    Runs up the test daemon process
### END INIT INFO

# Activate the python virtual environment
. $HOME/.virtualenvs/bespin/bin/activate

case "$1" in
  start)
    echo "Starting server"
    # Start the daemon
    python bespin_adapter.py start
    ;;
  stop)
    echo "Stopping server"
    # Stop the daemon
    python bespin_adapter.py stop
    ;;
  restart)
    echo "Restarting server"
    python bespin_adapter.py restart
    ;;
  *)
    # Refuse to do other stuff
    echo "Usage: /etc/init.d/testdaemon.sh {start|stop|restart}"
    exit 1
    ;;
esac

exit 0
