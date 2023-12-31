/*
	- 안티 앨리어싱(AA)

	1. 그래픽의 계단현상(앨리어싱)을 제거하기 위한 용도
	2. 3D 그래픽 뿐만이 아니라 2D 그래픽, 디지털 카메라 사진, 디지털 음원 등에 사용한다.
	3. 안티앨리어싱은 그래픽 카드를 통해 연산을 진행하므로 게임에 렉이 걸리고 그래픽 카드의 온도가 상승할 수 있다.
	4. SSAA, MSAA, AAA/TAA, TAA, FAA, MLAA 등 여러가지의 방식이 있다.

	- 종류
		1. SSAA(Super Sampling Anti-Aliasing, FSAA) : Randering되는 이미지를 크게 확대(가로 세로 각각 n배)하여 모니터 크기에 맞게 줄이는 방법
													  1920x1080 FHD 화면에 x4 SSAA를 하게 된다면, 3840x2160의 4K 해상도 수준으로 부하가 걸리게 된다.
		2. MSAA(Multi Sampling Anti-Aliasing)		: SSAA가 많은 성능과 메모리를 요구하기 때문에 만들어진 개량법, 도형의 외곽선만 AA 효과를 준다.
													  도형의 외곽선만 적용되기 때문에 텍스처 내부에는 적용이 되지 않는다는 단점이 있다.
		3. RSAA(Rotated Sampling Anti-Aliasing)		: MSAA의 기법과 비슷하나 격자형이 아닌 앨리어싱이 잘 나타내는 45도로 비틀고 색을 추출하고 보강하는 방식
*/

/*
	- BPC

	1. 색 요소를 가리길 때의 용어로 Bits Per Color의 약자 "색 당 비트"이다.
	2. 비트가 많아질수록 표현이 가능한 색이 많아지고 계단현상(컬러밴딩현상)이 감소하게 된다.
	3. 비트가 커지게 되면 처리해야 할 데이터가 많아져 더 높은 사양을 가진 장치들을 필요하게 된다.
*/