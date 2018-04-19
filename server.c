#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	int portaServidor = 52525;

	struct sockaddr_in configServidor;
	memset(&configServidor, 0, sizeof(configServidor));
	configServidor.sin_family = AF_INET;

	configServidor.sin_port = htons(portaServidor);

	configServidor.sin_addr.s_addr = htonl(INADDR_ANY);

	int idSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (idSocket < 0) {
		printf("Erro ao criar socket\n");
		return -1;
	}

	int retornoBind = (bind(idSocket, (struct sockaddr *)&configServidor, sizeof(configServidor)));
	if (retornoBind < 0) {
		printf("Erro ao dar bind no socket\n");
		return -1;
	}

	int numClientes = 3;
	if (listen(idSocket, numClientes) < 0) {
		printf("Erro ao abrir conexão\n");
		return -1;
	}

	struct sockaddr_in client_address;
	int client_address_len = 0;

	while (true) {
		int sock;
		if ((sock = accept(idSocket, (struct sockaddr *)&client_address, &client_address_len)) < 0) {
			printf("Não pode abrir socket\n");
			return 1;
		}

		int n = 0;
		int len = 0, tamanhoMensagem = 1024000;
		char buffer[tamanhoMensagem];
		char *pbuffer = buffer;

		printf("Piloto conectado\n");
		memset(buffer, 0, tamanhoMensagem);
		while ((n = recv(sock, pbuffer, tamanhoMensagem, 0)) > 0) {
			pbuffer += n;
			tamanhoMensagem -= n;
			len += n;
			
			printf("Aqui tem informação: (%d bytes) '%s'\n", len, buffer);
			char* msgRetorno = "[Rogerinho] Boa informação!";
			send(sock, msgRetorno, strlen(msgRetorno), 0);
		}
		close(sock);
	}

	close(idSocket);
	return 0;
}
