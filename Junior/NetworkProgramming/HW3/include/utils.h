#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

/**
 * @brief Convert a int to an string
 * @param n The int to be converted
 * @return The string converted from the int
 **/
char *int_to_str(int n);

/**
 * @brief Convert a string with CRLF to LF
 * @param str The string to be converted
 **/
void crlf_to_lf(char *str);

/**
 * @brief Get ip:port string from ip and port
 * @param ip The ip string
 * @param port The port string
 * @return The ip:port string
 **/
char *get_ip_port(char *ip, char *port);
#endif // UTILS_H_INCLUDED
