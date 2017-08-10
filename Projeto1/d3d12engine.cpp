
#include <vector>

#include <Windows.h>

#include <d3d12.h>
#include "d3dx12.h"
#include <DXGI.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

static const int g_frameCount = 2;

class D3D12
{
	HWND m_hwnd;

	IDXGIFactory1 *m_factory;
	ID3D12Device *m_device;
	
    D3D12_VIEWPORT m_viewport;
    D3D12_RECT m_scissorRect;
	ID3D12Resource *m_renderTargets[g_frameCount];
	ID3D12CommandAllocator *m_commandAllocator;
	ID3D12CommandQueue *m_commandQueue;
	ID3D12GraphicsCommandList *m_commandList;
	IDXGISwapChain *m_swapChain;
	ID3D12DescriptorHeap *m_rtvHeap;
	UINT m_rtvDescriptorSize;
	ID3D12PipelineState *m_pipelineState;

	ID3D12RootSignature *m_rootSignature;

	// App resources.
    ID3D12Resource *m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

	// Synchronization objects.
	UINT m_frameIndex;
	HANDLE m_fenceEvent;
	ID3D12Fence *m_fence;
	UINT64 m_fenceValue;


	void initCommandLists();
	void initSwapChain(int width, int height);

	void populateCommandList();
	void sync();
public:
	D3D12(HWND hwnd);

	void init();
	void present();
};

void D3D12::initCommandLists()
{
	HRESULT hr;
	// Describe and create the command queue.
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	hr = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
    hr = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));
	// Create the command list.
	hr = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator, NULL, IID_PPV_ARGS(&m_commandList));

	// Command lists are created in the recording state, but there is nothing
	// to record yet. The main loop expects it to be closed, so close it now.
	hr = m_commandList->Close();
}

void D3D12::initSwapChain(int width, int height)
{
	HRESULT hr;

	// Describe and create the swap chain.
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {0};
	swapChainDesc.BufferCount = g_frameCount;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.OutputWindow = m_hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = TRUE;

	// Swap chain needs the queue so that it can force a flush on it.
	hr = m_factory->CreateSwapChain(m_commandQueue,&swapChainDesc,&m_swapChain);

	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = (FLOAT)width;
	m_viewport.Height = (FLOAT)height;
	m_viewport.MinDepth = 0.f;
	m_viewport.MaxDepth = 1.f;
	m_scissorRect.left = m_scissorRect.top = 0;
	m_scissorRect.right = width;
	m_scissorRect.bottom = height;

	// Create descriptor heaps.
	// Describe and create a render target view (RTV) descriptor heap.
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = g_frameCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	hr = m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap));

	// Create frame resources.
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
	// Create a RTV for each frame.
	for (UINT n = 0; n < g_frameCount; n++)
	{
		hr = m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n]));
		m_device->CreateRenderTargetView(m_renderTargets[n], NULL, rtvHeapHandle);
		rtvHeapHandle.Offset(1, m_rtvDescriptorSize);
	}
}


D3D12::D3D12(HWND hwnd)
{
	m_hwnd = hwnd;
	m_device = NULL;
	m_frameIndex = 0;
}

void D3D12::init()
{
	HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&m_factory));

	UINT i = 0; 
	IDXGIAdapter1 * pAdapter; 
	std::vector <IDXGIAdapter1*> vAdapters; 
	while(m_factory->EnumAdapters1(i, &pAdapter) != DXGI_ERROR_NOT_FOUND) 
	{
		vAdapters.push_back(pAdapter); 
		++i; 
	}
	DXGI_ADAPTER_DESC1 desc;
	vAdapters[0]->GetDesc1(&desc);
	//factory->
//	factory->EnumAdapters1()
	hr = ::D3D12CreateDevice(vAdapters[0], D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));
	m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// Create synchronization objects and wait until assets have been uploaded to the GPU.
	hr = m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
	m_fenceValue = 1;

	/*
	// Create an event handle to use for frame synchronization.
	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (m_fenceEvent == nullptr)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
	}
	*/

	initCommandLists();
	initSwapChain(320,240);
	// This sample does not support fullscreen transitions.
	//hr = m_factory->MakeWindowAssociation(m_hwnd, DXGI_MWA_NO_ALT_ENTER);




	//
	//
	// Create an empty root signature.
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.NumParameters = 0;
	rootSignatureDesc.pParameters = NULL;
	rootSignatureDesc.NumStaticSamplers = 0;
	rootSignatureDesc.pStaticSamplers = NULL;
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	
	ID3DBlob *signature;
	ID3DBlob *error;
	hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	hr = m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));


    // Create the pipeline state, which includes compiling and loading shaders.
	ID3DBlob *vertexShader;
	ID3DBlob *pixelShader;

#if defined(_DEBUG)
    // Enable better shader debugging with the graphics debugging tools.
    //UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    UINT compileFlags = 0;
#endif
	/*
	hr = D3DCompileFromFile(GetAssetFullPath(L"shaders.hlsl").c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr);
	hr = D3DCompileFromFile(GetAssetFullPath(L"shaders.hlsl").c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr);
	*/

    struct Vertex
    {
        float position[3];
        float color[4];
    };

	// Define the vertex input layout.
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

	// Describe and create the graphics pipeline state object (PSO).
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {0};
//	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = m_rootSignature;
//	psoDesc.VS = { reinterpret_cast<UINT8*>(vertexShader->GetBufferPointer()), vertexShader->GetBufferSize() };
//	psoDesc.PS = { reinterpret_cast<UINT8*>(pixelShader->GetBufferPointer()), pixelShader->GetBufferSize() };
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
//	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
//	psoDesc.DepthStencilState.DepthEnable = FALSE;
//	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;
//	hr = m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));

	// Create the vertex buffer.
	// Define the geometry for a triangle.
	Vertex triangleVertices[] =
	{
		{ { 1.5f, 0.55f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ { 0.55f, -0.55f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { -0.55f, -0.55f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
	};

	const UINT vertexBufferSize = sizeof(triangleVertices);

	hr = m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		NULL,
		IID_PPV_ARGS(&m_vertexBuffer));

	// Copy the triangle data to the vertex buffer.
	UINT8* pVertexDataBegin;
	D3D12_RANGE readRange = { 0, 0 }; // We do not intend to read from this resource on the CPU.
	hr = m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
	int test = sizeof(triangleVertices);
	memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
	m_vertexBuffer->Unmap(0, NULL);

	// Initialize the vertex buffer view.
	m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView.StrideInBytes = sizeof(Vertex);
	m_vertexBufferView.SizeInBytes = vertexBufferSize;
	sync();
	return;
}

void D3D12::sync()
{
	UINT64 mCurrentFence = 0;

	// Advance the fence value to mark commands up to this fence point.
	m_fenceValue++;

	// Add an instruction to the command queue to set a new fence point.
	// Because we are on the GPU timeline, the new fence point won’t be
	// set until the GPU finishes processing all the commands prior to
	// this Signal().
	m_commandQueue->Signal(m_fence, m_fenceValue);

	// Wait until the GPU has completed commands up to this fence point.
	if(m_fence->GetCompletedValue() < mCurrentFence) {
		HANDLE eventHandle = CreateEventEx(NULL, false, false, EVENT_ALL_ACCESS);
		// Fire event when GPU hits current fence.
		m_fence->SetEventOnCompletion(mCurrentFence, eventHandle);
		// Wait until the GPU hits current fence event is
	}
}

void D3D12::present()
{
	HRESULT hr;
	hr = m_commandAllocator->Reset();
	// A command list can be reset after it has been added to the
	// command queue via ExecuteCommandList. Reusing the command list reuses memory.
	hr = m_commandList->Reset(m_commandAllocator, NULL);
	// Indicate a state transition on the resource usage.
	m_commandList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex],D3D12_RESOURCE_STATE_PRESENT,D3D12_RESOURCE_STATE_RENDER_TARGET)
	);
	// Set the viewport and scissor rect. This needs to be reset
	// whenever the command list is reset.
	m_commandList->RSSetViewports(1, &m_viewport);
	m_commandList->RSSetScissorRects(1, &m_scissorRect);
	
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
	
	// Clear the back buffer and depth buffer.
	const float clearColor[] = { 1.f, 1.f, 1.f, 0.5f };
	m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, NULL);

	// Specify the buffers we are going to render to.
	m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, NULL);
	
	m_commandList->SetGraphicsRootSignature(m_rootSignature);
	m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	m_commandList->DrawInstanced(3, 1, 0, 0);

	// Indicate a state transition on the resource usage.
	m_commandList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex],D3D12_RESOURCE_STATE_RENDER_TARGET,D3D12_RESOURCE_STATE_PRESENT)
	);
	// Done recording commands.
	hr = m_commandList->Close();
	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] = { m_commandList };
	m_commandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	
	// swap the back and front buffers
	m_swapChain->Present(1, 0);
	m_frameIndex = (m_frameIndex + 1) % 2;

	sync();
}
