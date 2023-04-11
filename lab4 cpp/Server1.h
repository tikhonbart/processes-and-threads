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
	/* �������� ��������� � ������� � ��� ��� �� ������������� */
protected: HANDLE SendServerToClientLive = NULL;
		 /* ��������� � ��� ��� ���� ��������� �������������� ������ */
protected: HANDLE MailSlotAnswerFromClient = NULL;
		 /* �������� ��������� �� ������� */
protected: HANDLE AnswerPipeMatrix = NULL;
		 /* ���������� ����� � ������� */
protected: HANDLE SendClientFileMapping = NULL;
		 /* �������� ���������� �������������� ������*/
protected: bool CheckDopMission = false;
		 /* �����  ������� ������� �� ������� */
protected: vector<string>RequestMatrix;
		 /* ������� � ��� ������������� ������� �*/
protected: vector<vector<int>> MatrixA;
		 /* ������� � ��� ������������� ������� �*/
protected: vector<vector<int>> MatrixB;
		 /* �������������� ������ ������� ��������� �������������� ������ */
protected: vector<vector<int>> SortMassiv;
		 /*����� ������� �� ������ ������� � ���� ������*/
protected: string SendToClient = "";

public:

	/* ���������� ��������� � ������� � ��� ��� ������ ������������� */
	void CheckServerLiveSend()
	{
		/* ������ ������������ �����  */
		SendServerToClientLive = CreateNamedPipe(
			L"\\\\.\\pipe\\ServerCheck",
			PIPE_ACCESS_DUPLEX,//�����������������
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE//������ ������������ � ����� ��� ����� ��������� 
			| PIPE_WAIT,// ���������� ���������� �������� � �������
			PIPE_UNLIMITED_INSTANCES,//���������� ����������� ������, ���������� ������ ������������ ��������� ��������
			0,// ���������� ����, ������� ����� ��������������� ��� ��������� ������
			0, //���������� ����, ������� ����� ��������������� ��� ��������� ������
			NMPWAIT_USE_DEFAULT_WAIT,//�������� ������� �������� �� ���������
			NULL// ��� �������������� ��������� ������������
		);

		char buffer[MAX_PATH];
		DWORD dwwr;
		//���� �������� ����� � ������ ����������� �����,�� ������� ��������������� � ��� ���� � ������� �� ����� ������� ����������� 
		if (SendServerToClientLive != INVALID_HANDLE_VALUE)
		{
			while (true)
			{
				cout << endl;
				cout << "������ �����������" << endl;
				cout << endl;
				// ���� ���� �����������, �� �� ����� ������� ��� ���-�� ������������ 
				BOOL CheckConnection = ConnectNamedPipe(SendServerToClientLive, NULL);
				if (CheckConnection != FALSE)
				{
					cout << endl;
					cout << "����������� � �������� �������������, ��������� ����� � ���������������� ������� " << endl;
					cout << endl;

					strcpy_s(buffer, "server_live");
					WriteFile(
						SendServerToClientLive,
						&buffer,
						sizeof(buffer),
						&dwwr,// ���������� ���������� � ����� ����
						NULL);// ���������� ����/�����
				}
				DisconnectNamedPipe(SendServerToClientLive);
			}
		}
		else cout << "�������� ����������� � ������ ��������������� !" << endl;
	}

	void MultiplicationMatrix()
	{
		int StringsMatrixA = MatrixA.size();//������ 1 �������
		int ColumnsMatrixA = MatrixA[0].size();


		int StringsMatrixB = MatrixB.size();
		int ColumnsMatrixB = MatrixB[0].size();//������� 2 �������
		int** Massiv = new int* [StringsMatrixA]; //������
		if (ColumnsMatrixA == StringsMatrixB)
		{
			for (int i = 0; i < StringsMatrixA; i++)
			{
				Massiv[i] = new int[ColumnsMatrixB];
				//�������
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
			cout << "������������ ������ :";
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
		cout << "������� � �� ������� ";
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
		cout << "������� B �� ������� " << endl;
		for (int i = 0; i < MatrixB.size(); i++)
		{
			cout << endl;
			for (int j = 0; j < MatrixB[i].size(); j++)
			{
				cout << MatrixB[i][j] << " ";
			}

		}
		cout << endl;
		cout << "\n������������ �������" << endl;
		MultiplicationMatrix();
	}

	/* �������� ��������� �� ������� � ������� ������ */
	void RequestAnswerFromClientMatrix()
	{
		char buffer[1024];
		string text = "";
		DWORD dwwr;
		while (true)
		{/* �������� ������������� ������ */
			AnswerPipeMatrix = CreateNamedPipe(
				L"\\\\.\\pipe\\ServerSendMatrix",
				PIPE_ACCESS_DUPLEX,//�����������������
				PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE // ������ ������������ � ����� ��� ����� ���������
				| PIPE_WAIT,// ���������� ���������� �������� � �������
				PIPE_UNLIMITED_INSTANCES,//���������� ����������� ������, ���������� ������ ������������ ��������� ��������
				0, // ���������� ������, ������� ����� ��������������� ��� ��������� ������
				0, // ���������� ������, ������� ����� ��������������� ��� �������� ������
				NMPWAIT_USE_DEFAULT_WAIT,//�������� ������� �������� �� ���������
				NULL); //��� �������������� ��������� ������������

			if (AnswerPipeMatrix != INVALID_HANDLE_VALUE)
			{
				break;
			}
		}
		/* ������� ����������� �� ���� - ��*/
		if (ConnectNamedPipe(AnswerPipeMatrix, NULL) != FALSE)
		{

			ReadFile(
				AnswerPipeMatrix,
				&buffer,
				sizeof(buffer),
				&dwwr,  // ���������� ��������� �� ������ ����
				NULL);// ���������� ����/�����

			text = buffer;
			DisconnectNamedPipe(AnswerPipeMatrix);
			CreateMassiv(text);
		}
	}


	void SortMatrix2(vector<vector<int>> Matrix)
	{
		vector<int> massiv;
		int temp = 0;
		/* �������� ������� ������� ����� ������������� */
		for (int i = 0; i < Matrix.size(); i++)
		{
			for (int j = 0; j < Matrix[i].size(); j++)
			{
				massiv.push_back(Matrix[i][j]);
			}
		}
		/*------------------------------------------------*/
		/* ���������� ��������� */


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


		/* ���������� ������ */
		cout << "������������� ������:";
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

	/* ��������� ��������������� ������� �� ������� */
	void AnotherMessageFromClient()
	{
		string text;
		DWORD dwwr;
		char buffer[1024];

		MailSlotAnswerFromClient = CreateMailslot(
			L"\\\\.\\mailslot\\SendToServerMission",
			0, //������ ������ ��������� ����� ���� �����
			MAILSLOT_WAIT_FOREVER,
			NULL//���������� �� ����� ���� �����������
		);

		while (true)
		{
			if (MailSlotAnswerFromClient != INVALID_HANDLE_VALUE)
			{
				break;
			}
		}

		//������ ��� ���� � �������
		if (ReadFile(MailSlotAnswerFromClient, &buffer, sizeof(buffer),
			&dwwr, // ���������� ��������� �� ������ ����
			NULL// ���������� ����/�����
		) != 0)
		{
			text = buffer;
			cout << endl;
			/* ���� ��� ��� ������ �� ���������� ������� �*/
			if (text == "sort_matrix_a")
			{
				cout << "���� �������������� ������� " << " ��� ��������������� ������� " << text << endl;
				CloseHandle(MailSlotAnswerFromClient);
				SortMatrix2(MatrixA);
			}
			/* ���� ��� ��� ������ �� ���������� ������� �*/
			else if (text == "sort_matrix_b")
			{
				cout << "���� �������������� ������� " << " ��� ��������������� ������� " << text << endl;
				CloseHandle(MailSlotAnswerFromClient);
				SortMatrix2(MatrixB);
			}
			/* ���� ��� ��� ������� */
			else if (text == "empty_mission")
			{
				cout << "���� ��������������� ������� " << endl;
				CheckDopMission = true;
				CloseHandle(MailSlotAnswerFromClient);
				AnotherMessageFromClient();

			}
		}
	}

	/* ���������� ����� ������� */
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
			INVALID_HANDLE_VALUE,//�������������� ��������� ������ ��������
			NULL,// ���������� �� ����� ���� �����������
			PAGE_READWRITE,//��������� ���������� ������������� ��� ������, ����������� ��� ������ ��� ������ / ������.
			0,//DWORD �������� ������� ������������� ������� ������� ������������� ������.
			256,//���� ������� ������� ����������� �����
			L"Local\\AnswerServer"//��� ������� ������������� ������
		);
		char text[1024];
		if (SendClientFileMapping != FALSE)
		{
			//���������� ������ ��������� ����������� � �������� ������������ �������� ��������.
			PCHAR Buffer = (PCHAR)MapViewOfFile(
				SendClientFileMapping,
				FILE_MAP_ALL_ACCESS,//��� �������. ������������ ������������� ����� ��� ������ / ������.
				0,//DWORD �������� ������� �������� �����, � �������� ���������� ��������.
				0,//
				256//����� ������ ������������� �����, ������������� � �������������
			);

			if (Buffer != NULL)
			{
				strcpy_s(text, SendToClient.c_str());
				CopyMemory(Buffer, text, sizeof(text));//���������� � ���� ������ ������� ���� � ���������� text
				BOOL Result = UnmapViewOfFile(Buffer);//���������� ����������� ������� � �������� ������������ �������� ��������.
				cout << "� �������� ��� ��� �������" << endl;
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
	cout << "������ ����� ������, �������� ��������� !" << endl;
	cout << "------------------------------------------------------------------" << endl;
	Server1 ServerUser;
	ServerUser.BeginWorkServer();
}

