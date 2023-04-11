#pragma once
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <thread>
#include <future>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>

using namespace std;

class Server1
{
	/* Отправка сообщение к клиенту о том что он функционирует */
protected: HANDLE SendServerToClientLive = NULL;
		 /* Сообщение о том что надо выполнить дополнительную задачу */
protected: HANDLE MailSlotAnswerFromClient = NULL;
		 /* Получает сообщение от клиента */
protected: HANDLE AnswerPipeMatrix = NULL;
		 /* Отправляет ответ к клиенту */
protected: HANDLE SendClientFileMapping = NULL;
		 /* Проверка выполнения дополнительной миссий*/
protected: bool CheckDopMission = false;
		 /* Текст  который получил от клиента */
protected: vector<string>RequestMatrix;
		 /* Матрица А для переумножения массива В*/
protected: vector<vector<int>> MatrixA;
		 /* Матрица В для переумножения массива А*/
protected: vector<vector<int>> MatrixB;
		 /* Дополнительный массив который выполняет дополнительную задачу */
protected: vector<vector<int>> SortMassiv;
		 /*Текст который мы скинем клиенту в виде ответа*/
protected: string SendToClient = "";

public:

	/* Отправляет сообщение к клиенту о том что сервер функционирует */
	void CheckServerLiveSend()
	{
		/* Создаём именнованный канал  */
		SendServerToClientLive = CreateNamedPipe(
			L"\\\\.\\pipe\\ServerCheck",
			PIPE_ACCESS_DUPLEX,//дввнаправленность
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE//данные записываются в канал как поток сообщений 
			| PIPE_WAIT,// Синхронное выполнение операций с каналом
			PIPE_UNLIMITED_INSTANCES,//количество экземпляров канала, ограничено только доступностью системных ресурсов
			0,// Количество байт, которые нужно зарезервировать для выходного буфера
			0, //Количество байт, которые нужно зарезервировать для выходного буфера
			NMPWAIT_USE_DEFAULT_WAIT,//Значение времени ожидания по умолчанию
			NULL// Без дополнительных атрибутов безопасности
		);

		char buffer[MAX_PATH];
		DWORD dwwr;
		//Если возможно зайти в данный именованный канал,то условие удовлетворяется и идёт цикл в котором он будет ожидать подключение 
		if (SendServerToClientLive != INVALID_HANDLE_VALUE)
		{
			while (true)
			{
				cout << endl;
				cout << "Ожидаю подключения" << endl;
				cout << endl;
				// Если нету подключение, то он будет ожидать что кто-то подключиться 
				BOOL CheckConnection = ConnectNamedPipe(SendServerToClientLive, NULL);
				if (CheckConnection != FALSE)
				{
					cout << endl;
					cout << "Подключение с клиентом положительное, отправляю ответ о функционирований сервера " << endl;
					cout << endl;

					strcpy_s(buffer, "server_live");
					WriteFile(
						SendServerToClientLive,
						&buffer,
						sizeof(buffer),
						&dwwr,// количество записанных в канал байт
						NULL);// синхронный ввод/вывод
				}
				DisconnectNamedPipe(SendServerToClientLive);
			}
		}
		else cout << "Внимание подключение к северу приостановленно !" << endl;
	}

	void MultiplicationMatrix()
	{
		int StringsMatrixA = MatrixA.size();//строка 1 матрицы
		int ColumnsMatrixA = MatrixA[0].size();


		int StringsMatrixB = MatrixB.size();
		int ColumnsMatrixB = MatrixB[0].size();//столбцы 2 матрицы
		int** Massiv = new int* [StringsMatrixA]; //строки
		if (ColumnsMatrixA == StringsMatrixB)
		{
			for (int i = 0; i < StringsMatrixA; i++)
			{
				Massiv[i] = new int[ColumnsMatrixB];
				//Столбцы
				for (int j = 0; j < ColumnsMatrixB; j++)
				{
					Massiv[i][j] = 0;
					for (int k = 0; k < ColumnsMatrixA; k++)
					{
						Massiv[i][j] += MatrixA[i][k] * MatrixB[k][j];
					}
				}
			}
			cout << endl;
			cout << "Переделанный массив :";
			cout << endl;
			for (int i = 0; i < StringsMatrixA; i++)
			{
				if (i != 0)
				{
					SendToClient = SendToClient + "\n";
				}
				cout << endl;
				for (int j = 0; j < ColumnsMatrixB; j++)
				{
					cout << Massiv[i][j] << " ";
					SendToClient = SendToClient +
						to_string(Massiv[i][j]) + " ";
				}

			}
			SendToClient = SendToClient + "*";

		}
		else
		{
			SendToClient = SendToClient + "Error *";
		}
		RequestAnswerFromClientMatrix();
	}

	void CreateMassiv(string text)
	{
		bool check = false;
		int count = 0;
		string TextSendToMatrix;
		RequestMatrix.push_back(text);
		MatrixA.push_back(vector<int>());
		MatrixB.push_back(vector<int>());
		for (int i = 0; i < RequestMatrix.size(); i++)
		{
			for (char element : RequestMatrix[i])
			{
				switch (element)
				{
				case ' ':
					if (check == false && TextSendToMatrix != "")
					{
						MatrixA[count].push_back(
							atoi(TextSendToMatrix.c_str()));
						TextSendToMatrix = "";
					}
					else if (check == true && TextSendToMatrix != "")
					{

						MatrixB[count].push_back(
							atoi(TextSendToMatrix.c_str()));
						TextSendToMatrix = "";
					}
					break;

				case '\n':
					if (check == false)
					{
						MatrixA.push_back(vector<int>());
						count++;
					}
					else
					{
						MatrixB.push_back(vector<int>());
						count++;
					}
					break;

				case '/':
					check = true;
					count = 0;
					break;


				default:
					TextSendToMatrix = TextSendToMatrix + element;
					break;
				}
			}
		}
		cout << endl;
		cout << "Матрица А от клиента ";
		cout << endl;
		for (int i = 0; i < MatrixA.size(); i++)
		{
			cout << endl;
			for (int j = 0; j < MatrixA[i].size(); j++)
			{
				cout << MatrixA[i][j] << " ";
			}

		}
		cout << "\n" << endl;
		cout << "Матрица B от клиента " << endl;
		for (int i = 0; i < MatrixB.size(); i++)
		{
			cout << endl;
			for (int j = 0; j < MatrixB[i].size(); j++)
			{
				cout << MatrixB[i][j] << " ";
			}

		}
		cout << endl;
		cout << "\nПереумножаем матрицы" << endl;
		MultiplicationMatrix();
	}

	/* Получает сообщения от клиента с помощью канала */
	void RequestAnswerFromClientMatrix()
	{
		char buffer[1024];
		string text = "";
		DWORD dwwr;
		while (true)
		{/* Создание именнованного канала */
			AnswerPipeMatrix = CreateNamedPipe(
				L"\\\\.\\pipe\\ServerSendMatrix",
				PIPE_ACCESS_DUPLEX,//двунаправленность
				PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE // данные записываются в канал как поток сообщений
				| PIPE_WAIT,// Синхронное выполнение операций с каналом
				PIPE_UNLIMITED_INSTANCES,//количество экземпляров канала, ограничено только доступностью системных ресурсов
				0, // Количество байтов, которые нужно зарезервировать для выходного буфера
				0, // Количество байтов, которые нужно зарезервировать для входного буфера
				NMPWAIT_USE_DEFAULT_WAIT,//Значение времени ожидания по умолчанию
				NULL); //Без дополнительных атрибутов безопасности

			if (AnswerPipeMatrix != INVALID_HANDLE_VALUE)
			{
				break;
			}
		}
		/* Ожидает подключения от кого - то*/
		if (ConnectNamedPipe(AnswerPipeMatrix, NULL) != FALSE)
		{

			ReadFile(
				AnswerPipeMatrix,
				&buffer,
				sizeof(buffer),
				&dwwr,  // количество считанных из канала байт
				NULL);// синхронный ввод/вывод

			text = buffer;
			DisconnectNamedPipe(AnswerPipeMatrix);
			CreateMassiv(text);
		}
	}


	void SortMatrix2(vector<vector<int>> Matrix)
	{
		vector<int> massiv;
		int temp = 0;
		/* Создание массива который будет сортироваться */
		for (int i = 0; i < Matrix.size(); i++)
		{
			for (int j = 0; j < Matrix[i].size(); j++)
			{
				massiv.push_back(Matrix[i][j]);
			}
		}
		/*------------------------------------------------*/
		/* Сортировка пузырьком */


		int tmp = 0;
		for (int i = 0; i < massiv.size(); i++) {
			for (int j = (massiv.size() - 1); j >= (i + 1); j--) {
				if (massiv[j] < massiv[j - 1]) {
					tmp = massiv[j];
					massiv[j] = massiv[j - 1];
					massiv[j - 1] = tmp;
				}
			}
		}

		int n = 0;
		for (int i = 0; i < Matrix.size(); i++)
		{
			SortMassiv.push_back(vector<int>());
			for (int j = 0; j < Matrix[i].size(); j++)
			{
				SortMassiv[i].push_back(massiv[n]);
				n++;
			}
		}


		/* Показывает массив */
		cout << "Сортированный массив:";
		for (int i = 0; i < SortMassiv.size(); i++)
		{
			cout << endl;
			SendToClient = SendToClient + "\n";
			for (int j = 0; j < SortMassiv[i].size(); j++)
			{
				cout << SortMassiv[i][j] << " ";
				SendToClient = SendToClient +
					to_string(SortMassiv[i][j]) + " ";
			}
		}
		CheckDopMission = true;
		AnotherMessageFromClient();
	}

	/* Проверяет дополнительного задания от клиента */
	void AnotherMessageFromClient()
	{
		string text;
		DWORD dwwr;
		char buffer[1024];

		MailSlotAnswerFromClient = CreateMailslot(
			L"\\\\.\\mailslot\\SendToServerMission",
			0, //размер одного сообщения может быть любым
			MAILSLOT_WAIT_FOREVER,
			NULL//дескриптор не может быть унаследован
		);

		while (true)
		{
			if (MailSlotAnswerFromClient != INVALID_HANDLE_VALUE)
			{
				break;
			}
		}

		//Читает что есть в файлике
		if (ReadFile(MailSlotAnswerFromClient, &buffer, sizeof(buffer),
			&dwwr, // количество считанных из канала байт
			NULL// синхронный ввод/вывод
		) != 0)
		{
			text = buffer;
			cout << endl;
			/* Если идёт доп миссия на сортировку матрицы А*/
			if (text == "sort_matrix_a")
			{
				cout << "Есть дополнительное задание " << " код дополнительного задания " << text << endl;
				CloseHandle(MailSlotAnswerFromClient);
				SortMatrix2(MatrixA);
			}
			/* Если идёт доп миссия на сортировку матрицы Б*/
			else if (text == "sort_matrix_b")
			{
				cout << "Есть дополнительное задание " << " код дополнительного задания " << text << endl;
				CloseHandle(MailSlotAnswerFromClient);
				SortMatrix2(MatrixB);
			}
			/* Если нет доп задания */
			else if (text == "empty_mission")
			{
				cout << "Нету дополнительного задания " << endl;
				CheckDopMission = true;
				CloseHandle(MailSlotAnswerFromClient);
				AnotherMessageFromClient();

			}
		}
	}

	/* Отправляет ответ клиенту */
	void SendAnswerToClient()
	{
		string TextSend = "";
		while (true)
		{
			if (CheckDopMission == true)
			{
				break;
			}

		}

		SendClientFileMapping = CreateFileMapping(
			INVALID_HANDLE_VALUE,//поддерживается системным файлом подкачки
			NULL,// дескриптор не может быть унаследован
			PAGE_READWRITE,//Позволяет отображать представления для чтения, копирования при записи или чтения / записи.
			0,//DWORD высокого порядка максимального размера объекта сопоставления файлов.
			256,//макс размера объекта отображения файла
			L"Local\\AnswerServer"//Имя объекта сопоставления файлов
		);
		char text[1024];
		if (SendClientFileMapping != FALSE)
		{
			//отображает объект файлового отображения в адресное пространство текущего процесса.
			PCHAR Buffer = (PCHAR)MapViewOfFile(
				SendClientFileMapping,
				FILE_MAP_ALL_ACCESS,//тип доступа. Отображается представление файла для чтения / записи.
				0,//DWORD высокого порядка смещения файла, с которого начинается просмотр.
				0,//
				256//Число байтов сопоставления файла, отображаемого в представление
			);

			if (Buffer != NULL)
			{
				strcpy_s(text, SendToClient.c_str());
				CopyMemory(Buffer, text, sizeof(text));//Записывает в файл данные которое есть в переменной text
				BOOL Result = UnmapViewOfFile(Buffer);//прекращает отображение объекта в адресное пространство текущего процесса.
				cout << "Я отправил все это клиенту" << endl;
				CloseHandle(SendClientFileMapping);
				Server1::CheckDopMission = false;
				MatrixA.clear();
				MatrixB.clear();
				SendToClient = "";
				RequestMatrix.clear();
				SortMassiv.clear();
				this_thread::sleep_for(chrono::milliseconds(5000));
				SendAnswerToClient();

			}
		}
	}

	void BeginWorkServer()
	{
		thread BeginServer(&Server1::CheckServerLiveSend, this);
		thread CheckOperationFromClientMatrix(&Server1::RequestAnswerFromClientMatrix, this);
		thread CheckDopMission(&Server1::AnotherMessageFromClient, this);
		thread SendToClient(&Server1::SendAnswerToClient, this);

		BeginServer.join();
		CheckOperationFromClientMatrix.join();
		CheckDopMission.join();
		SendToClient.join();

	}
};
int main()
{
	setlocale(0, "Rus");
	cout << "Сервер начал работу, ожидание сообщений !" << endl;
	cout << "------------------------------------------------------------------" << endl;
	Server1 ServerUser;
	ServerUser.BeginWorkServer();
}

