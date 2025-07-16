import streamlit as st
import time

# --- 1. 가짜 데이터베이스 (실제로는 크롤링으로 가져와야 할 데이터) ---
# 사용자가 검색할만한 책 몇 권을 미리 딕셔너리로 만들어 둡니다.
FAKE_DB = {
    "파친코": {
        "aladin": {"price": "12,500원", "stock": "재고 5개", "link": "https://www.aladin.co.kr/"},
        "yes24": {"price": "13,000원", "stock": "재고 3개", "link": "http://www.yes24.com/"}
    },
    "여행의 이유": {
        "aladin": {"price": "9,800원", "stock": "재고 10개 이상", "link": "https://www.aladin.co.kr/"},
        "yes24": {"price": "9,500원", "stock": "재고 8개", "link": "http://www.yes24.com/"}
    },
    "불편한 편의점": {
        "aladin": {"price": "11,200원", "stock": "재고 2개", "link": "https://www.aladin.co.kr/"},
        "yes24": {"price": "11,500원", "stock": "품절", "link": "http://www.yes24.com/"}
    },
    "나미아 잡화점의 기적": {
        "aladin": {"price": "10,050원", "stock": "재고 1개", "link": "https://www.aladin.co.kr/"},
        "yes24": {"price": "13,000원", "stock": "재고 5개", "link": "http://www.yes24.com/"}
    },
}

# --- 2. 웹페이지 UI 구성 ---
st.set_page_config(page_title="중고책 가격 비교", page_icon="📚")

# 제목
st.title("📚 중고책 최저가 비교")
st.write("알라딘, Yes24의 중고책 재고와 가격을 비교해 드립니다. (프로토타입)")

# 책 제목 입력창
book_title = st.text_input("책 제목을 입력하세요:", placeholder="예: 파친코")

# 검색 버튼
if st.button("최저가 검색하기"):
    if not book_title:
        st.warning("책 제목을 입력해주세요.")
    else:
        # 가짜 로딩 효과 (사용자 경험을 위해)
        with st.spinner('가격 정보를 비교하는 중...'):
            time.sleep(1) # 1초간 대기

        # --- 3. 데이터 처리 및 결과 표시 ---
        result = FAKE_DB.get(book_title)

        if result:
            st.success(f"'{book_title}' 검색 결과입니다.")
            
            # 결과를 2개의 컬럼으로 나누어 표시
            col1, col2 = st.columns(2)

            with col1:
                st.image("https://image.aladin.co.kr/img/header/2022/logo_aladin_wb.png")
                st.metric(label="가격", value=result["aladin"]["price"])
                st.info(f"재고: {result['aladin']['stock']}")
                st.link_button("바로가기", result["aladin"]["link"])

            with col2:
                st.image("https://www.yes24.com/Opener/Opener.asmx/GetYes24LogoImage?logoTypeCode=YES24_LOGO_11", width=150)
                st.metric(label="가격", value=result["yes24"]["price"])
                if result["yes24"]["stock"] == "품절":
                    st.error(f"재고: {result['yes24']['stock']}")
                else:
                    st.info(f"재고: {result['yes24']['stock']}")
                st.link_button("바로가기", result["yes24"]["link"])
        else:
            st.error("검색 결과가 없습니다. 다른 책을 검색해보세요. (예: 파친코, 여행의 이유, 불편한 편의점)")
