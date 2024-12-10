#include "common.h"

#define MAX_CLIENTS 10  // 최대 클라이언트 수 정의

// 형용사 리스트
const char* adjectives[] = {
    "추억의", "맛있는", "신비로운", "달콤한", "쫀득한", "바삭한", "화려한", "소중한", "희귀한", "상큼한"
};

// 명사 리스트
const char* nouns[] = {
    "호박", "사과", "쫀드기", "마카롱", "초콜릿", "김밥", "커피", "별사탕", "풍선껌", "붕어빵"
};

// 경매 물품 구조체
typedef struct {
    char item_name[50];
    int start_price;
    int bid_unit;
    int instant_price;
    int time_limit; // 초 단위 경매 시간
    int highest_bid;  // 최고 입찰가
    int highest_bidder;  // 최고 입찰자 인덱스
} AuctionItem;

// 사용자 구조체
typedef struct {
    char nickname[30];
    int budget;       // 초기 예산
    int current_bid;  // 현재 호가
    int is_active;    // 1: 경매 참여 중, 0: 경매 포기
} User;

// 물품 및 사용자 리스트 관리
AuctionItem items[5];
User users[MAX_CLIENTS];

// 공통 유틸리티 함수 구현
void err_quit(const char* msg) {
    MessageBoxA(NULL, msg, "Error", MB_OK | MB_ICONERROR);
    exit(1);
}

void err_display(const char* msg) {
    MessageBoxA(NULL, msg, "Error", MB_OK | MB_ICONERROR);
}

// 랜덤 물품 이름 생성 함수
void generateItemName(char* item_name) {
    // 랜덤으로 형용사와 명사 선택
    const char* adjective = adjectives[rand() % (sizeof(adjectives) / sizeof(adjectives[0]))];
    const char* noun = nouns[rand() % (sizeof(nouns) / sizeof(nouns[0]))];

    // 이름 조합
    sprintf(item_name, "%s %s", adjective, noun);
}

// 난수 생성 로직 
void generateAuctionItems(AuctionItem items[], int num_items) {
    int total_start_price = 0;
    srand((unsigned int)time(NULL)); // 난수 초기화

    for (int i = 0; i < num_items; i++) {
        // 물품 이름 생성 (한국어 형용사와 명사 조합)
        generateItemName(items[i].item_name);

        // 시작 가격 (10,000원 ~ 1,000,000원)
        items[i].start_price = (rand() % 991 + 10) * 1000;

        // 입찰 단위 (1,000원 ~ 10,000원)
        items[i].bid_unit = (rand() % 10 + 1) * 1000;

        // 즉시 낙찰가 (시작 가격의 1.5배 ~ 2배)
        items[i].instant_price = items[i].start_price + (rand() % (items[i].start_price / 2 + 1) + items[i].start_price / 2);

        // 경매 시간: 기본 4분 (240초)
        items[i].time_limit = 240;

        // 총 시작 가격 계산
        total_start_price += items[i].start_price;
        
        items[i].highest_bid = 0;
        items[i].highest_bidder = -1;
    }

    // 사용자 초기 예산 계산: 시작 가격 합산의 140%
    int initial_budget = (int)(total_start_price * 1.4);

    // 사용자들에게 초기 예산 배정 및 활성화 설정
    for (int i = 0; i < MAX_CLIENTS; i++) {
        users[i].budget = initial_budget;
        users[i].is_active = 1; // 경매 참여 상태
    }
}

//void CMyDialog::DoDataExchange(CDataExchange* pDX) {
//    CDialogEx::DoDataExchange(pDX);
//    DDX_Control(pDX, IDC_LIST_MSG, m_msgList); // 메시지 리스트 박스
//    DDX_Control(pDX, IDC_EDIT_MSG, m_msgEdit); // 메시지 입력 창
//    DDX_Control(pDX, IDC_BUTTON_CONNECT, m_connectBtn); // 연결 버튼
//    DDX_Control(pDX, IDC_BUTTON_SEND, m_sendBtn); // 전송 버튼
//}
//
//// 메시지 추가 함수 (UI 업데이트)
//void CMyDialog::InsertMsg(CString msg) {
//    m_msgList.AddString(msg); // 메시지 리스트에 메시지 추가
//    m_msgList.SetCurSel(m_msgList.GetCount() - 1); // 마지막 항목 선택
//}
//
//// 연결 버튼 클릭 이벤트 핸들러
//void CMyDialog::OnConnectClicked() {
//    // 서버 소켓 생성 및 바인딩
//    m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
//    if (m_serverSocket == INVALID_SOCKET) {
//        InsertMsg(_T("소켓 생성 오류"));
//        return;
//    }
//
//    // 서버 주소 설정
//    m_serverAddr.sin_family = AF_INET;
//    m_serverAddr.sin_port = htons(12345); // 포트 12345
//    m_serverAddr.sin_addr.s_addr = INADDR_ANY; // 모든 네트워크 인터페이스에서 연결 가능
//
//    // 서버 소켓 바인딩
//    if (bind(m_serverSocket, (SOCKADDR*)&m_serverAddr, sizeof(m_serverAddr)) == SOCKET_ERROR) {
//        InsertMsg(_T("서버 소켓 바인딩 오류"));
//        return;
//    }
//
//    // 클라이언트 연결 대기
//    if (listen(m_serverSocket, 5) == SOCKET_ERROR) {
//        InsertMsg(_T("서버 소켓 리슨 오류"));
//        return;
//    }
//
//    InsertMsg(_T("서버 대기 중..."));
//
//    // 클라이언트 연결 수락 및 스레드 시작
//    while (true) {
//        m_clientSocket = accept(m_serverSocket, NULL, NULL);
//        if (m_clientSocket == INVALID_SOCKET) {
//            InsertMsg(_T("클라이언트 연결 수락 오류"));
//            continue;
//        }
//
//        InsertMsg(_T("클라이언트 연결됨"));
//
//        // 클라이언트 통신을 위한 스레드 시작
//        AfxBeginThread(ClientThread, (LPVOID)this);
//    }
//}
//
//// 클라이언트 메시지 처리 스레드
//UINT CMyDialog::ClientThread(LPVOID pParam) {
//    CMyDialog* pDlg = (CMyDialog*)pParam;
//    pDlg->ProcessClientMessages(pDlg->m_clientSocket);
//    return 0;
//}
//
//// 클라이언트와의 메시지 송수신 처리
//void CMyDialog::ProcessClientMessages(SOCKET clientSocket) {
//    char recvBuffer[1024];
//    int bytesReceived;
//
//    // 클라이언트 메시지 수신
//    while (true) {
//        bytesReceived = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
//        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
//            InsertMsg(_T("클라이언트 연결 종료"));
//            closesocket(clientSocket);
//            break;
//        }
//
//        recvBuffer[bytesReceived] = '\0';
//        CString msg = CString(recvBuffer);
//        InsertMsg(msg); // 수신한 메시지 리스트에 추가
//    }
//}
//
//// 전송 버튼 클릭 이벤트 핸들러
//void CMyDialog::OnSendClicked() {
//    CString msg;
//    m_msgEdit.GetWindowText(msg); // 입력된 메시지 가져오기
//
//    // 메시지가 비어있지 않으면 전송
//    if (!msg.IsEmpty()) {
//        int sendResult = send(m_clientSocket, msg.GetBuffer(), msg.GetLength(), 0);
//        if (sendResult == SOCKET_ERROR) {
//            InsertMsg(_T("메시지 전송 오류"));
//        }
//    }
//
//    m_msgEdit.SetWindowText(_T("")); // 메시지 입력 창 초기화
//}
//
//// 대화 상자 닫기 전 처리
//void CMyDialog::OnClose() {
//    // 소켓 닫기
//    if (m_clientSocket != INVALID_SOCKET) {
//        closesocket(m_clientSocket);
//    }
//    if (m_serverSocket != INVALID_SOCKET) {
//        closesocket(m_serverSocket);
//    }
//
//    CDialogEx::OnClose();
//}
//
//// 메시지 맵
//BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
//    ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CMyDialog::OnConnectClicked)
//    ON_BN_CLICKED(IDC_BUTTON_SEND, &CMyDialog::OnSendClicked)
//    ON_WM_CLOSE()
//END_MESSAGE_MAP()
//
//// 경매 서버 소켓 및 클라이언트 관리
//SOCKET server_socket;
//SOCKET client_sockets[MAX_CLIENTS];
//HANDLE hThreads[MAX_CLIENTS];
//
//// 클라이언트와의 통신을 처리하는 스레드
//UINT ClientCommThread(LPVOID pParam) {
//    SOCKET client_socket = (SOCKET)pParam;
//    char buffer[1024];
//    int bytes_received;
//    int user_id = -1;
//
//    // 클라이언트로부터 닉네임 받기
//    send(client_socket, "닉네임을 입력해주세요: ", 25, 0);
//    bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
//    buffer[bytes_received] = '\0';
//
//    // 유효한 닉네임 처리
//    for (int i = 0; i < MAX_CLIENTS; i++) {
//        if (users[i].is_active == 0) {
//            users[i].is_active = 1;
//            user_id = i;
//            strcpy_s(users[i].nickname, sizeof(users[i].nickname), buffer);
//            break;
//        }
//    }
//
//    // 경매 진행 로직
//    while (true) {
//        // 경매 아이템 보여주기
//        for (int i = 0; i < 5; i++) {
//            // 입찰 시도하는 사용자에게 메시지 전송
//        }
//    }
//
//    return 0;
//}
