
import tvutils as c_tvutils
import sys

c_tvutils.tvu_init(sys.argv[0])

time = c_tvutils.tvu_time
service_socket = c_tvutils.tvu_service_socket
service_client_bind = c_tvutils.tvu_service_client_bind
service_bind = c_tvutils.tvu_service_bind
service_unbind = c_tvutils.tvu_service_unbind
service_close = c_tvutils.tvu_service_close
service_sendto = c_tvutils.tvu_service_sendto
service_recvfrom = c_tvutils.tvu_service_recvfrom

