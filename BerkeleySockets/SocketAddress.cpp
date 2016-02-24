#include "SocketAddress.h"
#include <iostream>

SocketAddress::SocketAddress(const std::string & inString) {
	setAddress(inString);
}

SocketAddress::SocketAddress(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint16_t port) {
	m_address.sin_addr.S_un.S_un_b.s_b1 = byte1;
	m_address.sin_addr.S_un.S_un_b.s_b2 = byte2;
	m_address.sin_addr.S_un.S_un_b.s_b3 = byte3;
	m_address.sin_addr.S_un.S_un_b.s_b4 = byte4;
	m_address.sin_family = AF_INET;
	m_address.sin_port = port;
	for (int i = 0; i < 8; i++) m_address.sin_zero[i] = 0;
}

SocketAddress::SocketAddress(SocketAddress &a) {
	m_address = a.m_address;
}

int SocketAddress::setAddress(const std::string & inString) {
	//inString.find_last_of
	//Retorna la posici?n de la ?ltima aparici?n de este caracter.
	//Si no aparece, retorna string::npos.
	size_t pos = inString.find_last_of(':');
	std::string host, service;

	//npos es una constante que tiene el valor mayor posible que puede alcanzar una variable del tipo size_t.
	//size_t es un tipo num?rico tal que tiene el n?mero de bits sufientes para representar el n?mero m?s grande de tama?o de array que puede existir.
	//npos vale -1, porque como size_t es un tipo unsigned, -1 es el valor m?s grande que puede representarse (cuando da la vuelta en binario)
	if (pos != std::string::npos) {
		host = inString.substr(0, pos);
		service = inString.substr(pos + 1);
	} else {
		host = inString;
		//0 es el puerto por defecto
		service = "0";
	}

	addrinfo hint;
	//Fuerzo que toda la memoria que corresponde a hint valga 0. As? en getaddrinfo no habr? lugar a malentendidos
	//Recordamos que la funci?n de hint ser? la de filtrar los resultados que me retorne getaddrinfor.
	//En este caso, s?lo me retornar? aquellas direcciones que sean IPv4.
	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;

	//En result se almacenar? una lista de las direcciones IP:puerto que conincidan con mis
	//criterios de b?squeda
	addrinfo* result;
	int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);

	if (error != 0 && result != nullptr) {
		freeaddrinfo(result);
		return -1;
	}
	//Recorro las direcciones de result en busca de una v?lida
	while (!result->ai_addr && result->ai_next) {
		result = result->ai_next;
	}
	if (!result->ai_addr) {
		freeaddrinfo(result);
		return -1;
	}

	//memcpy copia byte a byte la direcci?n v?lida de result en la direcci?n atributo 
	//de la clase.
	memcpy(&m_address, result->ai_addr, sizeof(sockaddr_in));
	freeaddrinfo(result);

	return 0;
}

void SocketAddress::setAddress(sockaddr a) {
	memcpy(&m_address, &a, sizeof(sockaddr_in));
}

sockaddr SocketAddress::getAddress() {
	sockaddr ret;
	memcpy(&ret, &m_address, sizeof(sockaddr));
	return ret;
}

std::ostream& operator<<(std::ostream& os, const SocketAddress &sa) {
	return os << sa.m_address.sin_addr.S_un.S_un_b.s_b1 << "." << sa.m_address.sin_addr.S_un.S_un_b.s_b2 << "." << sa.m_address.sin_addr.S_un.S_un_b.s_b3 << "." << sa.m_address.sin_addr.S_un.S_un_b.s_b4 << ":" << sa.m_address.sin_port;
}

bool SocketAddress::operator==(SocketAddress & sa) {
	if (m_address.sin_port != sa.m_address.sin_port || m_address.sin_addr.S_un.S_addr != sa.m_address.sin_addr.S_un.S_addr) return false;
	return true;
}
