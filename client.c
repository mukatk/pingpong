#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]) {
	const char* server_name = "localhost";
	const int server_port = 52525;

	int tamanhoMensagem = 1024;

	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;

	inet_pton(AF_INET, server_name, &server_address.sin_addr);

	server_address.sin_port = htons(server_port);

	int idSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (idSocket < 0) {
		printf("Não foi possível criar o socket\n");
		return -1;
	}

	if (connect(idSocket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
		printf("Não foi possível conectar ao socket\n");
		return -1;
	}
	
	const char* mensagem;

	if (strcmp(argv[1], "Renan") == 0) {
		mensagem = "[Renan] Levei o Renanzinho pra ver a palestra do Tony Ramos. Ele ficou fascinado. Achou que era um ator de pelucia";
	} else if (strcmp(argv[1], "Julinho") == 0) {
		mensagem = "[Julinho] Eu nao vi esse filme mas eu sei que é bom.";
	} else if (strcmp(argv[1], "Maurilio") == 0) {
		mensagem = "[Maurilio] Foi golpe";
	} else {
		int lmsgSimone = 1000000;
		char msgSimone[lmsgSimone];
		int a = 0;
		while (a < lmsgSimone - 1) {
			msgSimone[a] = 'a';
			a++;
		}
		msgSimone[lmsgSimone - 1] = '\0';
		mensagem = msgSimone;
	}
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	send(idSocket, mensagem, strlen(mensagem), 0);

	int n = 0;
	int len = 0;
	char buffer[tamanhoMensagem];
	char* pbuffer = buffer;

	while ((n = recv(idSocket, pbuffer, tamanhoMensagem, 0)) > 0) {
		pbuffer += n;
		tamanhoMensagem -= n;
		len += n;

		buffer[len] = '\0';
		clock_gettime(CLOCK_MONOTONIC_RAW, &end);
		uint64_t delta_ms = ((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000) / 1000;
		printf("ack: (%ld ms) '%s'\n", delta_ms, buffer);
	}

	close(idSocket);
	return 0;
}
