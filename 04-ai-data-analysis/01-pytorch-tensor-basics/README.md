# 01. 실습 환경 구축 및 PyTorch Tensor 기초

## 학습 목표

이 실습은 Python 3.11 기반 가상환경을 구성하고, PyTorch Tensor의 생성·속성·인덱싱·구조 변경·기본 연산을 직접 확인하는 내용입니다. 오류를 의도적으로 재현해 원인을 확인하고 올바른 사용법과 비교합니다.

## 파일 구성

```text
01-pytorch-tensor-basics/
├── README.md
├── requirements.txt
└── notebooks/
    └── pytorch-tensor-basics.ipynb
```

실행 코드와 출력이 보존된 노트북은 [`notebooks/pytorch-tensor-basics.ipynb`](notebooks/pytorch-tensor-basics.ipynb)입니다. 공개 사본에서는 개인 이름과 학번을 제거했습니다. 제출용 PDF와 로컬 화면 캡처는 개인 식별 정보가 포함되어 있어 이 공개 저장소에 복사하지 않았습니다.

## Part 1. 실습 환경 구축

### 1. Python 3.11 확인

Windows에서 Python 3.11 64-bit 설치 후 Python Launcher로 버전을 확인합니다.

```powershell
py -3.11 --version
py -3.11 -m pip --version
```

첫 명령은 설치된 인터프리터 버전, 두 번째 명령은 해당 Python에 연결된 pip 버전을 확인합니다. `py -3.11`을 사용하면 여러 Python 버전이 설치된 PC에서도 3.11을 명시적으로 호출할 수 있습니다.

### 2. uv 설치와 확인

```powershell
py -3.11 -m pip install uv
uv --version
```

`uv` 명령을 찾지 못하면 Python 사용자 Scripts 디렉터리가 PATH에 들어 있는지 확인하고 새 터미널을 열어 다시 확인합니다.

### 3. 가상환경 생성과 활성화

```powershell
uv venv .venv --python=3.11
.venv\Scripts\activate
python --version
# 실습이 끝난 뒤
deactivate
```

가상환경을 활성화하면 해당 프로젝트의 `python`과 패키지가 우선 사용됩니다. `deactivate`는 가상환경을 종료해 기존 셸의 Python 선택으로 되돌립니다.

### 4–5. 패키지 설치와 JupyterLab 실행

```powershell
uv pip install -r requirements.txt
uv run jupyter lab
```

`requirements.txt`는 LLMs-from-scratch의 기본 의존성 목록을 바탕으로 작성했습니다. GPU가 있는 Windows 환경에서는 과제 지침에 따라 호환 가능한 CUDA 빌드의 PyTorch를 별도로 설치합니다. 과제에서 안내한 CUDA 12.1 명령은 다음과 같습니다.

```powershell
uv pip install --index-url https://download.pytorch.org/whl/cu121 torch torchvision torchaudio
```

PyTorch CUDA 휠은 NVIDIA 드라이버·GPU 및 CUDA 휠 호환성에 영향을 받으므로 실제 설치 시 본인의 장치와 과제에서 지정한 조합을 확인해야 합니다. 본 실습 기록은 CUDA가 없는 CPU 환경입니다.

## Part 2. Tensor 생성과 속성

### 2.1 생성·차원·shape·dtype

노트북은 scalar(0차원), vector(1차원), matrix(2차원), 3차원 Tensor를 만들고 각 shape과 dtype을 출력합니다. 리스트 대괄호의 중첩 단계가 축의 수와 대응합니다. 예를 들어 `[ [ [값들] ] ]` 구조는 세 축을 갖습니다. 정수 리터럴로 만든 Tensor는 기본적으로 `int64`, 실수 리터럴은 기본 실수 dtype인 `float32`로 만들어집니다. 정수 dtype은 정확한 정수 표현을, 실수 dtype은 소수 계산을 지원합니다.

### 2.2 `ones`, `full`, `randn`

`torch.ones(shape)`는 지정한 크기를 1로 채우고, `torch.full(shape, value)`는 지정한 값으로 채웁니다. `torch.randn(shape)`는 표준 정규분포에서 난수를 뽑습니다. shape을 바꾸면 축별 원소 수가 달라지고 `full`의 값을 바꾸면 모든 원소가 바뀝니다. 난수 Tensor는 별도 seed를 고정하지 않는 한 실행마다 값이 달라질 수 있습니다.

### 2.3 장치와 CUDA 확인

`tensor.device`는 Tensor가 놓인 장치를 알려 줍니다. `torch.cuda.is_available()`이 True일 때 CUDA 장치 이름과 `torch.version.cuda`를 확인하고 `.to('cuda')`로 옮길 수 있습니다. 이 실습 환경에서는 CUDA를 사용할 수 없어 `.to('cuda')`를 실행하지 않고 CPU 경로를 사용했습니다.

### 2.4 장치 불일치 연산

CPU Tensor와 CUDA Tensor는 직접 더할 수 없습니다. 둘 다 CPU 또는 둘 다 CUDA에 두어야 합니다. CPU 전용 환경에서는 실제 CPU–CUDA 연산 오류를 발생시킬 CUDA 장치가 없으므로, 노트북은 조건부로 CUDA가 있을 때만 재현하고 장치가 없으면 제한 사항을 출력합니다.

## Part 3. 인덱싱과 슬라이싱

### 3.1 행 선택

2×3 Tensor에서 `t.shape`는 행·열 크기 `(2, 3)`을 보여 줍니다. `t[0]`과 `t[1]`은 각각 첫 번째와 두 번째 행을 선택하므로 shape은 `(3,)`입니다.

### 3.2 슬라이싱 및 오류

- `t[:, 1:]`: 모든 행의 두 번째 열부터 끝까지
- `t[1:, :2]`: 두 번째 행 이후에서 첫 두 열
- `t[::2]`, `t[::3]`: 행 축을 각각 2칸, 3칸 간격으로 선택
- `t[-1]`: 마지막 행
- `t[:, -2]`: 각 행의 끝에서 두 번째 열
- `t[-5]`: 행이 2개뿐이므로 유효 인덱스 범위를 벗어나 `IndexError`

음수 인덱스는 끝에서부터 세지만, 축의 길이를 넘어서는 음수 인덱스는 유효하지 않습니다.

### 3.3 Ellipsis

4차원 Tensor에서 `...`는 생략된 차원들을 모두 나타냅니다. `x[1, ..., 1]`은 첫 번째 축의 인덱스 1, 마지막 축의 인덱스 1을 고정하고 가운데 축들을 모두 남깁니다. `x[0, ..., 0]`도 같은 방식으로 첫 축과 마지막 축의 인덱스만 0으로 고정합니다.

### 3.4 불리언 마스크

`v > 3`, `v % 2 == 0`, `v % 2 == 1`은 각 원소에 대해 참/거짓을 계산하는 마스크입니다. `v[mask]`는 참인 위치의 값을 선택합니다. 잘못된 길이의 마스크는 대상 축 크기와 맞지 않아 `IndexError`를 일으킵니다. 노트북에는 잘못된 마스크의 오류와 올바른 행 마스크 예시가 포함됩니다.

## Part 4. Tensor 구조 변경

### 4.1 `reshape`, `view`, 평면화

12개 원소를 `reshape(3, 4)`와 `view(3, 4)`로 바꾸면 같은 값 배치를 얻습니다. `view`는 기존 메모리 배치와 stride로 표현할 수 있는 형태만 허용하고, `reshape`는 가능하면 view를 반환하되 필요하면 복사본을 만들 수 있습니다. `reshape(-1)`은 원소 수를 바탕으로 한 축 길이를 자동 계산해 1차원으로 펼칩니다. `reshape(-2)`는 `-1` 외 음수 크기를 자동 계산자로 허용하지 않으므로 오류가 납니다.

### 4.2 `unsqueeze`, `squeeze`

`unsqueeze(dim)`은 길이 1인 축을 추가해 원소 수는 유지합니다. 양수 및 음수 축 번호에 따라 추가 위치가 달라집니다. 1차원 Tensor에서 `unsqueeze(2)`는 허용 범위를 벗어나 `IndexError`가 납니다. 추가한 축에 `squeeze()`를 적용하면 길이가 1인 축이 제거됩니다. 모든 길이 1 축을 제거하는 `.squeeze()`보다 의도한 축만 제거하는 `.squeeze(dim)`가 더 명확할 수 있습니다.

### 4.3 전치와 `permute`

2차원 행렬 `m.T`는 행 축과 열 축을 맞바꿉니다. 3차원 Tensor에 `permute(0, 2, 1)`을 적용하면 결과 축 순서가 원본의 0번, 2번, 1번 축이 되어 `(3, 4, 5)`가 `(3, 5, 4)`로 바뀝니다. `permute` 인자는 결과의 각 축이 원본의 어느 축에서 오는지 순서대로 지정합니다.

### 4.4 `expand`와 `repeat`

두 연산은 반복된 값이 보이는 결과를 만들 수 있지만 메모리 동작이 다릅니다. `expand`는 길이 1인 축을 stride를 조정해 늘려 보이는 view를 만들며 보통 데이터를 복제하지 않습니다. `repeat`는 값을 실제로 복사해 새 저장공간을 만듭니다. 따라서 `expand`로 만든 겹치는 view의 원소를 제자리에서 수정하는 일은 주의해야 합니다.

### 4.5 `resize_` → `unsqueeze` → `expand`

노트북은 `(2,4) → (2,1,4) → (2,2,4)` 및 `(3,4) → (3,1,4) → (3,5,4)`의 shape 변화를 확인합니다. `resize_`는 in-place로 크기를 바꾸는 저수준 연산이므로 일반적인 코드에서는 의도를 명확히 하는 `reshape` 사용을 권장합니다. 이 실습 예시는 과제의 `resize_` 요구를 따라갑니다.

## Part 5. 기본 연산

### 5.1 내적

`[1,2,3]·[4,5,6] = 1×4 + 2×5 + 3×6 = 32`입니다. 노트북은 `zip` 반복문, 인덱스 반복문, NumPy의 `np.dot`·`@`, PyTorch의 `torch.dot`·`@` 결과를 서로 비교합니다.

### 5.2 벡터화와 브로드캐스팅

Python 반복문은 각 원소를 Python 수준에서 순회합니다. NumPy의 배열 덧셈·곱셈·거듭제곱·지수·로그 함수는 전체 배열을 벡터화해 처리합니다. `(3,1)`과 `(3,)`을 더하면 뒤쪽 축을 정렬해 각각 `(3,3)`으로 확장한 뒤 원소별 덧셈을 수행합니다. PyTorch도 같은 브로드캐스팅 규칙을 지원합니다.

### 5.3 백만 원소 성능 비교

`%timeit`으로 리스트 컴프리헨션과 `np.arange(1_000_000) ** 2`를 비교합니다. 정확한 시간은 CPU·Python·NumPy 버전 및 시스템 부하에 따라 달라집니다. 일반적으로 NumPy 방식은 연산 루프를 컴파일된 저수준 코드에서 처리해 Python 반복 비용을 줄이므로 더 빠릅니다.

### 5.4 PyTorch 연산과 장치 선택

노트북은 원소별 PyTorch 연산, broadcasting, `x.shape=(10000,128)`, `w.shape=(128,)`의 `x @ w`를 확인합니다. 장치는 `torch.device('cuda' if torch.cuda.is_available() else 'cpu')`로 선택합니다. 이 실행 환경에서는 CUDA가 False이므로 결과는 CPU에서 계산되며 결과 shape은 `(10000,)`입니다.

### 5.5 행렬 곱셈

`(100,128) @ (128,256)`은 안쪽 차원 128이 일치하므로 `(100,256)` 결과를 만듭니다. `(100,128) @ (256,128)`은 안쪽 차원이 맞지 않아 `RuntimeError`가 납니다. 오른쪽 행렬을 전치하면 `(100,128) @ (128,256)`이 되어 곱할 수 있습니다. `(50,100) @ (100,50)`의 결과는 `(50,50)`, 반대 순서 `(100,50) @ (50,100)`의 결과는 `(100,100)`이므로 행렬 곱셈은 일반적으로 교환법칙을 만족하지 않습니다.

### 5.6 Softmax와 수치 안정성

Softmax는 `exp(logit)`을 모두 더한 값으로 나누어 합이 1인 값을 만듭니다. `[2.3, 5.1, 0.7]`은 직접 계산할 수 있지만 `[1000, 2000, 3000]`처럼 큰 값은 `np.exp`가 overflow되어 `nan`이 생깁니다. 모든 logit에서 최댓값을 빼면 Softmax의 비율은 수학적으로 변하지 않으면서 지수 계산 범위를 줄여 안정적인 값을 구할 수 있습니다.

## 실행 환경 기록

| 항목 | 확인 결과 |
|---|---|
| OS | Windows (세부 버전은 캡처에서 확인되지 않음) |
| GPU | CUDA 사용 불가. 물리 GPU 모델명은 캡처에 표시되지 않아 미확인 |
| Python | 3.11.9, `.venv` 가상환경 |
| PyTorch | 2.15.0+cpu (실습 출력 기준) |
| CUDA | 사용 불가, `torch.version.cuda`는 `None` |
| 계산 장치 | CPU |

## 재현 방법

프로젝트 루트에서 Windows PowerShell을 열고 다음 명령을 실행합니다.

```powershell
py -3.11 -m pip install uv
uv venv .venv --python=3.11
.venv\Scripts\activate
uv pip install -r ai-data-analysis/01-pytorch-tensor-basics/requirements.txt
uv run jupyter lab
```

위 명령은 루트의 `.venv`를 활성화한 상태에서 실행합니다. 이 노트북은 과제에서 의도한 예외 셀을 포함하므로 셀 순서에 따라 오류 출력이 보이는 것이 정상입니다. 오류 셀은 원인 설명과 그 뒤의 정상 동작 예제로 이어집니다.
