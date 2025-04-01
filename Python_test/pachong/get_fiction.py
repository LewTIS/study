import requests
from bs4 import BeautifulSoup


def get_chapter_content(url):
    headers = {
        'User - Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/116.0.0.0 Safari/537.36'
    }
    all_content = ""
    page_num = 1
    while True:
        if page_num == 1:
            current_url = url
        else:
            # 处理 URL 生成，确保正确生成下一页 URL
            if f"_{page_num - 1}" in url:
                current_url = url.replace(f"_{page_num - 1}", f"_{page_num}")
            else:
                base, ext = url.rsplit('.', 1)
                current_url = f"{base}_{page_num}.{ext}"

        try:
            response = requests.get(current_url, headers=headers)
            response.raise_for_status()
            soup = BeautifulSoup(response.text, 'html.parser')
            # 查找包含小说内容的元素，修改为查找 id 为 chaptercontent 的标签
            content_element = soup.find(id='chaptercontent')
            if content_element:
                # 去除提示信息
                for p in content_element.find_all('p'):
                    if '看后求收藏' in p.text:
                        p.decompose()
                content = content_element.get_text()
                all_content += content
            else:
                print(f"未找到内容元素，URL: {current_url}")
                break

            # 检查是否有下一页
            next_page = soup.find('a', text='下一页')
            if not next_page:
                break
            page_num += 1
        except requests.RequestException as e:
            print(f"请求出错: {e}")
            break
    return all_content


start_chapter = 829
end_chapter = 848
base_url = "https://www.biqugie.com/7/7563/{}.html"

# 打开文件以写入模式
with open('novel_content.txt', 'w', encoding='utf-8') as file:
    for chapter_num in range(start_chapter, end_chapter + 1):
        url = base_url.format(3394046 + chapter_num - 829)
        chapter_content = get_chapter_content(url)
        if chapter_content:
            file.write(f"第{chapter_num}章内容:\n{chapter_content}\n\n")
            print(f"第{chapter_num}章内容已保存到文件中。")
    