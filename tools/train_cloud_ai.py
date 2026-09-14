"""
=============================================================================
           AIoT_LIB - HƯỚNG DẪN DẠY (HUẤN LUYỆN / TÙY BIẾN) CLOUD AI
=============================================================================
Mục đích:
  - Cho phép người dùng hoặc kỹ sư công nghiệp "dạy" (huấn luyện tri thức) 
    cho Cloud AI (Google Gemini) theo ngữ cảnh dây chuyền, máy móc của riêng mình.
  - Cung cấp 3 phương pháp dạy Cloud AI hiện đại nhất:
    1. System Instruction & Persona Training (Định hình chuyên môn & phong cách).
    2. Few-Shot In-Context Learning (Dạy bằng các ví dụ mẫu tình huống thực tế).
    3. RAG / Domain Knowledge Injection (Nạp cẩm nang vận hành, mã lỗi, quy trình bảo trì).
    4. Hardware Tool / Action Calling (Dạy AI khi nào được phép ra lệnh cho ESP32).

Yêu cầu cài đặt thư viện:
  pip install google-generativeai
=============================================================================
"""

import os
import sys

# Thiết lập UTF-8 cho Windows console
if sys.stdout.encoding != 'utf-8':
    try:
        sys.stdout.reconfigure(encoding='utf-8')
    except Exception:
        pass

import google.generativeai as genai

# =============================================================================
# BƯỚC 1: CẤU HÌNH API KEY VÀ MODEL
# =============================================================================
GEMINI_API_KEY = os.getenv("GEMINI_API_KEY", "YOUR_GEMINI_API_KEY_HERE")
genai.configure(api_key=GEMINI_API_KEY)

# =============================================================================
# BƯỚC 2: DẠY BẰNG TRI THỨC CHUYÊN SÂU (DOMAIN KNOWLEDGE & SYSTEM PROMPT)
# =============================================================================
# Bạn có thể đọc file PDF/Text sổ tay kỹ thuật của nhà máy để nạp vào đây
PLANT_DOMAIN_KNOWLEDGE = """
[CẨM NANG VẬN HÀNH & MÃ LỖI ĐỘNG CƠ BƠM CÔNG NGHIỆP - DÂY CHUYỀN SỐ 1]
- Loại thiết bị: Động cơ dị bộ 3 pha 380V - 15kW.
- Tiêu chuẩn rung động ISO 10816-3:
  + RMS < 2.8 mm/s: Vùng A/B (Hoạt động tốt, an toàn tuyệt đối).
  + RMS 2.8 - 4.5 mm/s: Vùng C (Cảnh báo mòn bạc đạn, cho phép chạy có giám sát).
  + RMS > 7.1 mm/s: Vùng D (Nguy hiểm cấp bách - Bắt buộc dừng máy bảo dưỡng).
- Nhiệt độ thân máy cho phép: Max 75 độ C. Nếu > 80 độ C kèm rung động cao: Nguy cơ bó kẹt bạc đạn.
- Quy trình ứng cứu khẩn cấp:
  + Nếu RMS > 7.1 mm/s hoặc Nhiệt độ > 85 độ C: Ngay lập tức phát lệnh ngắt Relay 1 [CMD:RELAY1_OFF] 
    và kích hoạt đèn LED cảnh báo [CMD:LED_BLINK] để công nhân hiện trường nhận biết.
"""

# =============================================================================
# BƯỚC 3: DẠY BẰNG CÁC VÍ DỤ TÌNH HUỐNG (FEW-SHOT LEARNING)
# =============================================================================
# Đưa cho AI các ví dụ mẫu (Prompt -> Trả lời mong muốn) để AI học phong cách ra quyết định
FEW_SHOT_EXAMPLES = [
    {
        "role": "user",
        "parts": ["Tình trạng máy: Rung RMS = 1.8 mm/s, Nhiệt độ = 42 C. Máy đang chạy thế nào?"]
    },
    {
        "role": "model",
        "parts": ["Động cơ đang hoạt động rất tốt trong Vùng A (tiêu chuẩn ISO 10816-3). Mọi thông số rung động và nhiệt độ đều ổn định hoàn hảo."]
    },
    {
        "role": "user",
        "parts": ["Tôi nghe thấy tiếng rè rè, cảm biến báo rung RMS = 5.2 mm/s, Nhiệt độ = 68 C!"]
    },
    {
        "role": "model",
        "parts": ["Cảnh báo: Rung động 5.2 mm/s đã vượt ngưỡng an toàn và rơi vào Vùng C. Có dấu hiệu thoái hóa vòng bi hoặc mất cân bằng cơ khí. Tôi đã bật đèn LED cảnh báo để kỹ sư kiểm tra sớm: [CMD:LED_ON]"]
    },
    {
        "role": "user",
        "parts": ["Cấp cứu! Cảm biến đo được rung RMS = 9.8 mm/s, nhiệt độ 88 C, có khói bốc ra!"]
    },
    {
        "role": "model",
        "parts": ["NGUY CẤP! Rung động 9.8 mm/s thuộc Vùng D cực kỳ nguy hiểm. Tôi ĐÃ TỰ ĐỘNG CẮT NGUỒN ĐỘNG CƠ và kích hoạt đèn chớp khẩn cấp ngay lập tức: [CMD:RELAY1_OFF] [CMD:LED_BLINK]"]
    }
]

# =============================================================================
# BƯỚC 4: KHỞI TẠO MÔ HÌNH VỚI TOÀN BỘ TRI THỨC ĐÃ DẠY
# =============================================================================
def build_custom_cloud_ai():
    system_instruction = f"""
Bạn là chuyên gia chẩn đoán sự cố công nghiệp cấp cao tích hợp trong hệ thống AIoT_LIB.
Bạn giám sát bo mạch vi điều khiển ESP32-S3 kết nối các cảm biến và cơ cấu chấp hành.

Tri thức chuyên sâu của nhà máy:
{PLANT_DOMAIN_KNOWLEDGE}

Nguyên tắc điều khiển phần cứng qua thẻ lệnh (Action Calling):
- Bật đèn LED onboard: đính kèm [CMD:LED_ON]
- Tắt đèn LED onboard: đính kèm [CMD:LED_OFF]
- Chớp nháy đèn LED khẩn cấp: đính kèm [CMD:LED_BLINK]
- Đóng Relay 1 (Cấp điện động cơ): đính kèm [CMD:RELAY1_ON]
- Ngắt Relay 1 (Cắt nguồn bảo vệ): đính kèm [CMD:RELAY1_OFF]

Hãy suy luận sắc bén theo ISO 10816-3, đưa ra hướng khắc phục cụ thể và luôn thực hiện lệnh phần cứng kịp thời.
"""

    model = genai.GenerativeModel(
        model_name="gemini-3.6-flash",
        system_instruction=system_instruction
    )
    return model

# =============================================================================
# BƯỚC 5: KIỂM THỬ TƯ DUY VÀ SUY LUẬN CỦA CLOUD AI
# =============================================================================
def test_cloud_ai():
    print("=================================================================")
    print("      🧠 KHỞI TẠO VÀ KIỂM THỬ TRÍ TUỆ NHÂN TẠO CLOUD AI          ")
    print("=================================================================")
    
    model = build_custom_cloud_ai()
    chat = model.start_chat(history=FEW_SHOT_EXAMPLES)
    
    test_queries = [
        "Chào bạn, động cơ đang có độ rung RMS = 1.5 mm/s, nhiệt độ 40 C. Bạn kiểm tra giúp tôi.",
        "Này AI, bật đèn led trên mạch ESP32 lên để tôi soi kiểm tra buồng máy.",
        "Cảm biến rung đo được RMS đột biến lên 8.2 mm/s, nhiệt độ tăng vọt lên 86 C, phải làm sao?!"
    ]
    
    for q in test_queries:
        print(f"\n👤 [NGƯỜI DÙNG HỎI]: {q}")
        response = chat.send_message(q)
        print(f"🤖 [CLOUD AI TRẢ LỜI]:\n{response.text.strip()}\n")
        print("-" * 65)

if __name__ == "__main__":
    test_cloud_ai()
