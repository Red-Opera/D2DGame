# DirectX 11
DirectX와 C++로 만든 2차원 슈팅 게임
<hr>

## 목차
<ol>
    <b><li><a href="#Play">게임 포트폴리오 영상</a></li></b>
    <b><li><a href="#Outline">게임 개요</a></li></b>
    <b><li><a href="#Func">주요 기능</a></li></b>
    <b><li>도전 과제</li></b>
    <b><li>결과 및 성과</li></b>
</ol>

<hr>
<h2 id="Play">게임 포트폴리오 영상</h2>
<a href="https://www.youtube.com/watch?v=qiwBTG6fsT4">
    <img src="https://img.youtube.com/vi/qiwBTG6fsT4/0.jpg" width="100%" height="100%"/>
</a>
<p>이미지를 클릭하면 재생됩니다.</p>

<hr>
<h2 id="Outline">게임 개요</h2>
<ul>
    <li>
        <h3>Game Genre</h3>
        2D 슈팅 게임
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
    <li>
        <h3>Target Platform</h3>
        <img src="https://img.shields.io/badge/Windows-0078D4?style=flat&logo=windows&logoColor=white"/></p>
    </li>
</ul>

<hr>
<h2 id="Func">주요 기능</h2>
<h4><mark><em>위 내용은 게임 포트폴리오 영상과 내용이 일치합니다</em></mark></h4>

<h3>플레이어 UI</h3>

<img src="https://github.com/Red-Opera/D2DGame/assets/121187608/4b73204d-5cc5-4f27-9e1f-edb5aa821f52" width="100%" height="100%"/>
먼저 왼쪽 위에 보면 Kills라고 써있는 Score UI가 표시됩니다. 점수는 몬스터를 처지하면 오르도록 제작했습니다.<br><br><br><br>

<img src="https://github.com/Red-Opera/D2DGame/assets/121187608/8f07fde8-26b8-4d51-9223-bc1957afff27" width="100%" height="100%"/>
Kill 점수는 총 10자리까지 표시할 수 있어 최대 100억 킬 전까지 표시할 수 있습니다.<br><br><br><br>

<img src="https://github.com/Red-Opera/D2DGame/assets/121187608/f2b35a17-5b12-4dd6-b1c0-797c99fbdd0f" width="100%" height="100%"/>
왼쪽 아래 화면을 보면 키보드와 마우스 UI가 있습니다.
이 UI는 1, 2, W, A, S, D, C와 마우스로 구성되며 해당 키를 누르게 될 경우 눌린 UI가 표시되도록 제작했습니다.<br><br><br><br>

<h3>플레이어의 컨트롤</h3>

<img src="https://github.com/Red-Opera/D2DGame/assets/121187608/96af0a4e-bf6f-44b3-b818-e4ba0f8c9040" width="100%" height="100%"/>
플레이어는 위 테이블에 적혀 있는 키보드나 마우스를 누르게 될 경우 키에 대응하는 기능을 수행하도록 제작했습니다.

<h3>플레이어</h3>

<img src="https://github.com/Red-Opera/D2DGame/assets/121187608/6719ca93-0903-4256-a351-b12eceffc2e1" width="100%" height="100%"/>
플레이어는 체력 바와 마나바가 존재하여 플레이어의 상태를 실시간으로 확인할 수 있습니다.


<h4>플레이어의 상태</h4>
플레이어는 Idle, Move, Attack로 3가지의 상태를 가집니다. 상태마다 다른 스프라이트를 직접 제작하여 생동감 있는 애니메이션을 구현했습니다.<br><br>

<img src="https://github.com/Red-Opera/D2DGame/assets/121187608/b24c57b6-8ab9-4633-a0f2-dd21782be459" width="100%" height="100%"/>

<br>
Idle 상태는 아무것도 하지 않을 때 발생하며 캐릭터가 숨 쉬는 것처럼 애니메이션을 제작했습니다.<br><br>

<img src="https://github.com/Red-Opera/D2DGame/assets/121187608/4d70f403-2629-434a-8ab1-3eb30fc7373c" width="100%" height="100%"/>

<br>
Move 상태는 W, A, S, D 키를 눌렸을 때 캐릭터가 해당 방향으로 움직이며 해당 방향 맞는 애니메이션이 실행됩니다.
플레이어를 따라다니는 카메라를 자연스럽게 구현하여 플레이어의 움직임을 잘 따라가도록 했습니다.<br><br><br><br>



Attack 상태는 왼쪽 마우스 클릭했을 때 전환됩니다. 플레이어의 무기에는 장거리와 단거리 두 가지가 존재합니다.<br><br>

<img src="https://github.com/Red-Opera/D2DGame/assets/121187608/0226227c-e43d-45b7-a0cd-e0eb051f508e" width="100%" height="100%"/>


단거리 공격은 플레이어의 정면에서 칼 스프라이트와 피격 스프라이트가 재생됩니다. 피격 스프라이트가 재생되는 범위 안에 있는 몬스터들이 플레이어 공격력만큼 데미지를 받도록 설정했습니다. <br><br>


<img src="https://github.com/Red-Opera/D2DGame/assets/121187608/9d981ce6-d912-482f-9eb7-02f96ebb0ece" width="100%" height="100%"/>


장거리 공격은 플레이어가 화면 기준으로 마우스 방향에 따라 총이 조준되도록 구현했습니다. 마우스 왼쪽을 클릭할 경우 총알이 생성되어 앞으로 나아갑니다. 총알이 몬스터에 맞았을 경우 해당 몬스터의 체력이 감소하고 총알이 사라지도록 제작했습니다. 또한, 장거리 공격은 단거리 공격과 달리 스킬이 존재하고 C 버튼을 눌렸을 경우 총알이 여러 방향으로 나가는 스킬을 구현했습니다. 

<table border="2">
    <td>
        <img src="https://github.com/Red-Opera/D2DGame/assets/121187608/36c85e30-d0d1-4814-8221-64bf0810517e"/>
    </td>
    <td>
        <img src="https://github.com/Red-Opera/D2DGame/assets/121187608/e49add28-cd92-463a-bc5e-1a4cc63ed5a2"/>
    </td>
</table>
왼쪽 : 장거리 무기를 소지 중인 경우, 오른쪽 : 단거리 무기를 소지 중인 경우<br><br>

마나를 소비하고 나서 시간이 지나면 마나가 천천히 회복합니다. 단, 단거리 무기로 바꿨을 경우 마나를 회복하지 않도록 했습니다. Attack상태는 Move 상태일 때도 실행할 수 있습니다.<br><br>

<h3>몬스터</h3>

다음은 몬스터에 대해 설명하겠습니다. 

![image](https://github.com/Red-Opera/D2DGame/assets/121187608/68553177-79b4-402c-94bf-3914092e7bce)

몬스터는 단거리와 장거리 공격 유닛을 제작했습니다. 몬스터는 플레이어와 마찬가지로 체력바와 마나 바가 있습니다.<br><br>

<img src="https://github.com/Red-Opera/D2DGame/assets/121187608/158c9a1e-2bba-43d3-b437-53a48415c58b" width="100%" height="100%"/>

장거리 몬스터는 플레이어뿐만이 아니라 지형도 충돌하도록 구현했습니다. 단거리 몬스터는 길막방지를 위해 오브젝트까리 충돌하지 않도록 설정했습니다.

<h2>몬스터 상태</h2>
몬스터 상태는 Move, Attack, Death 상태가 존재합니다. 
<br>
<img src="https://github.com/Red-Opera/D2DGame/assets/121187608/1615179f-e408-49d7-9ea8-cc73d44ceb8a" width="100%" height="100%"/>
몬스터는 생성하자마자 Move 상태가 되어 플레이어를 쫒아옵니다. 
<br>
<br>

<table border="2">
    <td>
        <img src="https://github.com/Red-Opera/D2DGame/assets/121187608/437cbe86-5e15-4c4b-8da0-b7a68fbe5578" width="450px" height="100%"/>
    </td>
    <td>
        <img src="https://github.com/Red-Opera/D2DGame/assets/121187608/adbc2c5c-a195-475f-bb94-1d84b89ee6d0" width="450px" height="100%"/>
    </td>
</table>

장거리 몬스터는 플레이어가 일정한 거리 이내로 들어올 경우 Attack 상태가 되어 플레이어 방향을 탄알을 발사하도록 설정했습니다. 
단거리 몬스터는 플레이어와 닿을 경우 공격 애니메이션을 실행하면서 인스턴스 없는 공격하도록 설정했습니다. 

<img src="https://github.com/Red-Opera/D2DGame/assets/121187608/11cc2fba-2ef2-480d-b440-750d40417293" width="100%" height="100%"/>

몬스터들은 체력이 0 이하가 되면 죽는 애니메이션을 실행하며 사라집니다.

<img src="https://github.com/Red-Opera/D2DGame/assets/121187608/ab9e4c07-f27f-4d86-91d3-9f99091fe569" width="100%" height="100%"/>


몬스터를 처치했을 때 왼쪽 위의 UI의 점수가 1점 상승하는 것을 확인할 수 있습니다.

<img src="https://github.com/Red-Opera/D2DGame/assets/121187608/e5f3ae3d-1f22-4794-a008-686bb11df69f" width="100%" height="100%"/>

지형 오른쪽 아래에 내려가면 다른 맵으로 이동할 수 있습니다.
