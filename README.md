# DirectX 11
DirectX와 C++로 만든 2차원 슈팅 게임
<hr>

## 목차
<ol>
    <b><li><a href="#Play">게임 시연 영상</a></li></b>
    <b><li><a href="#Outline">게임 개요</a></li></b>
    <b><li>주요 기능</li></b>
    <b><li>도전 과제</li></b>
    <b><li>결과 및 성과</li></b>
</ol>

<hr>
<h2 id="Play">게임 시연 영상</h2>
<a href="https://www.youtube.com/watch?v=qiwBTG6fsT4">
    <img src="https://img.youtube.com/vi/qiwBTG6fsT4/0.jpg" width="100%" height="100%"/>
</a>
<p>이미지를 클릭하면 재생됩니다.</p>

<hr>
<h2 id="Outline">게임 개요</h2>
<ul>
    <li>
        <h3>Game Genre</h3>
        슈팅 게임
    </li> 
    <li>
        <h3>Development Stack</h3>
        <p>개발 도구 : <img src="https://img.shields.io/badge/VisualStudio-5C2D91?style=flat&logo=VisualStudio&logoColor=white"/></p>
        <p>개발 언어 : <img src="https://img.shields.io/badge/C-A8B9CC?style=flat&logo=C&logoColor=white"/> <img src="https://img.shields.io/badge/C++-00599C?style=flat&logo=cplusplus&logoColor=white"/> <img src="https://img.shields.io/badge/HLSL-f26b00?style=for-the-badge&logo=data:image/svg+xml;base64,PHN2ZyBmaWxsPSIjMDAwMDAwIiB3aWR0aD0iODAwcHgiIGhlaWdodD0iODAwcHgiIHZpZXdCb3g9IjAgMCAzMiAzMiIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj48dGl0bGU+ZmlsZV90eXBlX2hsc2w8L3RpdGxlPjxwb2x5Z29uIHBvaW50cz0iOS41MjUgMjEuOTQ3IDcuNTYgMjEuOTQ3IDcuNTYgMTcuMSAzLjk1OCAxNy4xIDMuOTU4IDIxLjk0NyAyIDIxLjk0NyAyIDEwLjA0NCAzLjk1OCAxMC4wNDQgMy45NTggMTQuNzkyIDcuNTYgMTQuNzkyIDcuNTYgMTAuMDQ0IDkuNTI1IDEwLjA0NCA5LjUyNSAyMS45NDciIHN0eWxlPSJmaWxsOmJsdWUiLz48cG9seWdvbiBwb2ludHM9IjE2LjY5MiAyMS45NDcgMTEuNTEzIDIxLjk0NyAxMS41MTMgMTAuMDQ0IDEzLjQ3MiAxMC4wNDQgMTMuNDcyIDE5Ljc3MyAxNi42OTIgMTkuNzczIDE2LjY5MiAyMS45NDciIHN0eWxlPSJmaWxsOmJsdWUiLz48cGF0aCBkPSJNMTcuNDYyLDIxLjQ5MVYxOC44MzRhMy42NzYsMy42NzYsMCwwLDAsMS4xNDYuOTA5LDIuODA5LDIuODA5LDAsMCwwLDEuMjQ5LjMsMi4wNSwyLjA1LDAsMCwwLC42NDYtLjA5MiwxLjMwOSwxLjMwOSwwLDAsMCwuNDYxLS4yNTIsMS4wMjksMS4wMjksMCwwLDAsLjI3Ni0uMzgyLDEuMjMxLDEuMjMxLDAsMCwwLC4wOTEtLjQ3OCwxLjMsMS4zLDAsMCwwLS4xNDUtLjYyMywxLjkwNywxLjkwNywwLDAsMC0uNC0uNTA2LDQuMDc3LDQuMDc3LDAsMCwwLS42LS40NDhxLS4zNDYtLjIxNS0uNzQ2LS40MzlhNC4wNDUsNC4wNDUsMCwwLDEtMS41MTktMS40MTksMy44ODQsMy44ODQsMCwwLDEtLjUtMi4wMjYsNC4yMzIsNC4yMzIsMCwwLDEsLjI3My0xLjYsMy4wMiwzLjAyLDAsMCwxLC43NDQtMS4xLDIuOTM4LDIuOTM4LDAsMCwxLDEuMDg4LS42MzQsNC4xNjcsNC4xNjcsMCwwLDEsMS4zMS0uMiw1LjgxNiw1LjgxNiwwLDAsMSwxLjIuMTExLDMuOCwzLjgsMCwwLDEsLjk2Ny4zNDV2Mi40ODJhMi44NTksMi44NTksMCwwLDAtLjQ3Ni0uMzY1LDMuMjEyLDMuMjEyLDAsMCwwLS41My0uMjYyQTMuMSwzLjEsMCwwLDAsMjEuNDYsMTJhMi43OTQsMi43OTQsMCwwLDAtLjUxMi0uMDUsMS45OTQsMS45OTQsMCwwLDAtLjYwNi4wODcsMS4zNDcsMS4zNDcsMCwwLDAtLjQ2MS4yNDUsMS4wOTIsMS4wOTIsMCwwLDAtLjI5MS4zNzcsMS4xNDQsMS4xNDQsMCwwLDAtLjEuNDk0LDEuMiwxLjIsMCwwLDAsLjExNS41MzYsMS42NTcsMS42NTcsMCwwLDAsLjMyOC40NDgsMy40NjEsMy40NjEsMCwwLDAsLjUxNS40MTVjLjIuMTM2LjQzLjI3NS42ODYuNDE5YTcuMDk0LDcuMDk0LDAsMCwxLC45MzYuNjM1LDMuNzEyLDMuNzEyLDAsMCwxLC43MTMuNzU5LDMuMjY5LDMuMjY5LDAsMCwxLC40NTUuOTY3LDQuNTczLDQuNTczLDAsMCwxLC4xNTgsMS4yNjYsNC40MzksNC40MzksMCwwLDEtLjI3NywxLjY3MiwyLjg4OCwyLjg4OCwwLDAsMS0uNzQ5LDEuMSwyLjgyMywyLjgyMywwLDAsMS0xLjEuNiw0Ljc0Miw0Ljc0MiwwLDAsMS0xLjMyNS4xODIsNS40NzYsNS40NzYsMCwwLDEtMS4zNjEtLjE2NkEzLjQxLDMuNDEsMCwwLDEsMTcuNDYyLDIxLjQ5MVoiIHN0eWxlPSJmaWxsOmJsdWUiLz48cG9seWdvbiBwb2ludHM9IjMwIDIxLjk0NyAyNC44MjIgMjEuOTQ3IDI0LjgyMiAxMC4wNDQgMjYuNzggMTAuMDQ0IDI2Ljc4IDE5Ljc3MyAzMCAxOS43NzMgMzAgMjEuOTQ3IiBzdHlsZT0iZmlsbDpibHVlIi8+PC9zdmc+"/></p>
    </li>
    <li>
        <h3>Development Period</h3>
        <p>시작 일자 : 2023.02.08</p>
        <p>총 개발 기간 : 6주</p>
    </li>
    <li>
        <h3>Development Member</h3>
        서정현
    </li>
</ul>
