
#include "d3d9engine.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

namespace render3d {

	struct D3D9ResData {
		LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
		LPDIRECT3DINDEXBUFFER9 indexBuffer;
		LPDIRECT3DVERTEXDECLARATION9 vertexDeclaration;
		unsigned int stride;
	};

	struct D3D9ResInstanceData {
		D3DXMATRIX transMatrix;
		D3DXMATRIX rotMatrix;
		D3DXMATRIX scaleMatrix;
		D3DMATRIX worldMatrix;
	};

	struct D3D9CameraData {
		D3DXMATRIX transMatrix;
		D3DXMATRIX rotMatrix;
		D3DXMATRIX scaleMatrix;
		D3DMATRIX viewMatrix;
	};

	D3D9Engine::D3D9Engine()
	{
		m_hwnd = NULL;
		m_d3d9 = NULL;
		m_device = NULL;
		m_currentFont = NULL;
	}

	void D3D9Engine::configure(void *options)
	{
		HWND hwnd = (HWND)options;
		m_hwnd = hwnd;
	}

	void D3D9Engine::init()
	{
		HRESULT hr;
		LPDIRECT3D9 d3d9 = ::Direct3DCreate9(D3D_SDK_VERSION);
		if (!d3d9) {
			int failed = 1;
		}

		RECT clientRect;
		::GetClientRect(m_hwnd, &clientRect);

		D3DPRESENT_PARAMETERS d3dpp = { 0 };

		d3dpp.BackBufferWidth = clientRect.right - clientRect.left;
		d3dpp.BackBufferHeight = clientRect.bottom - clientRect.top;
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferCount = 2;
		d3dpp.hDeviceWindow = m_hwnd;
		d3dpp.Windowed = true;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		LPDIRECT3DDEVICE9 device;
		hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &device);
		if (FAILED(hr)) {
			int a = 1;
		}

		m_d3d9 = d3d9;
		m_device = device;


		D3DXMATRIX projection_matrix;
		float aspect;

		aspect = ((float)d3dpp.BackBufferWidth / (float)d3dpp.BackBufferHeight);

		D3DXMatrixPerspectiveFovLH(&projection_matrix, //Result Matrix
			D3DX_PI / 4,          //Field of View, in radians.
			aspect,             //Aspect ratio
			1.0f,               //Near view plane
			1000.0f);           //Far view plane

		//Our Projection matrix won't change either, so we set it now and never touch
		//it again.
		m_device->SetTransform(D3DTS_PROJECTION, &projection_matrix);
	}

	void D3D9Engine::destroy()
	{
		if (m_device)
			m_device->Release();
		if (m_d3d9)
			m_d3d9->Release();
	}

	void D3D9Engine::beginScene()
	{
		m_device->BeginScene();
		if (m_currentCamera && m_currentCamera->dirtyFlags() != 0) {
			updateInternalCamera(m_currentCamera);
			setViewFromCamera(m_currentCamera);
		}
	}

	void D3D9Engine::endScene()
	{
		HRESULT hr;

		hr = m_device->EndScene();
		hr = m_device->Present(0, 0, 0, 0);
	}

	void D3D9Engine::clear(const Color &clearColor) {
		m_device->Clear(0,
			NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_XRGB(clearColor.r(), clearColor.g(), clearColor.b()),
			1.0f, 0);
	}

	void D3D9Engine::updateInternalResourceData(ResourceData *resData) {
		HRESULT hr;
		LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
		LPDIRECT3DINDEXBUFFER9 indexBuffer;

		int dirtyFlags = resData->dirtyFlags();

		if (dirtyFlags == 0)
			return;
			
		int vertexCount = resData->vertexCount();
		Vector3 *vertices = resData->vertices();
		Vector3 *normals = resData->normals();
		Color *colors = resData->colors();
		int indexCount = resData->indexCount();
		D3D9ResData *internalData = (D3D9ResData*)resData->internalData();

		BYTE *pData;
		if (dirtyFlags & (ResourceData::VERTICES_DIRTY | ResourceData::NORMALS_DIRTY | ResourceData::COLORS_DIRTY)) {	
			internalData->vertexBuffer->Release();
			hr = m_device->CreateVertexBuffer(vertexCount * internalData->stride, 0, 0, D3DPOOL_MANAGED, &vertexBuffer, NULL);

			vertexBuffer->Lock(0, 0, (void**)&pData, 0);
			for (int i = 0; i < vertexCount; ++i) {
				memcpy(pData, &vertices[i], sizeof(Vector3));
				pData += sizeof(Vector3);

				//*(D3DVECTOR*)pData = D3DVECTOR({ 0.f, -1.f, 0.f }); //todo: use this method to set normal data
				memcpy(pData, &normals[i], sizeof(Vector3));
				pData += sizeof(Vector3);

				*(D3DCOLOR*)pData = D3DCOLOR_ARGB(colors[i].a(), colors[i].r(), colors[i].g(), colors[i].b());
				pData += sizeof(D3DCOLOR);
			}
			vertexBuffer->Unlock();
			internalData->vertexBuffer = vertexBuffer;
		}
			
		if (dirtyFlags & ResourceData::INDICES_DIRTY) {
			internalData->indexBuffer->Release();
			hr = m_device->CreateIndexBuffer(indexCount * 4, 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &indexBuffer, NULL);

			if (indexCount > 0) {
				indexBuffer->Lock(0, 0, (void**)&pData, 0);
				memcpy(pData, resData->indices(), indexCount * sizeof(int));
				indexBuffer->Unlock();
			}
			internalData->indexBuffer = indexBuffer;
		}
		resData->setDirtyFlags(0);
	}

	void D3D9Engine::updateInternalResourceInstanceData(ResourceInstance *resInstance) {
		unsigned long dirty = resInstance->dirtyFlags();
		D3D9ResInstanceData *data = (D3D9ResInstanceData *)resInstance->internalData();

		if (dirty & ResourceInstance::POS_DIRTY | ResourceInstance::ROT_DIRTY | ResourceInstance::ROT_DIRTY) {
			if (dirty & ResourceInstance::POS_DIRTY) {
				Vector3 pos = resInstance->pos();
				D3DXMatrixTranslation(&data->transMatrix, pos.x(), pos.y(), pos.z());
			}

			if (dirty & ResourceInstance::ROT_DIRTY) {
				cinetico_core::Quaternion rot = resInstance->rot();
				cinetico_core::Matrix4x4 rotMatrix = rot.toRotationMatrix().transposed();
				data->rotMatrix._11 = rotMatrix.m[0][0];
				data->rotMatrix._12 = rotMatrix.m[0][1];
				data->rotMatrix._13 = rotMatrix.m[0][2];
				data->rotMatrix._14 = rotMatrix.m[0][3];
				data->rotMatrix._21 = rotMatrix.m[1][0];
				data->rotMatrix._22 = rotMatrix.m[1][1];
				data->rotMatrix._23 = rotMatrix.m[1][2];
				data->rotMatrix._24 = rotMatrix.m[1][3];
				data->rotMatrix._31 = rotMatrix.m[2][0];
				data->rotMatrix._32 = rotMatrix.m[2][1];
				data->rotMatrix._33 = rotMatrix.m[2][2];
				data->rotMatrix._34 = rotMatrix.m[2][3];
				data->rotMatrix._41 = rotMatrix.m[3][0];
				data->rotMatrix._42 = rotMatrix.m[3][1];
				data->rotMatrix._43 = rotMatrix.m[3][2];
				data->rotMatrix._44 = rotMatrix.m[3][3];
				//D3DXMatrixRotationYawPitchRoll(&data->rotMatrix, rot.y(), rot.x(), rot.z());
			}

			if (dirty & ResourceInstance::SCALE_DIRTY) {
				Vector3 scale = resInstance->scale();
				D3DXMatrixScaling(&data->scaleMatrix, scale.x(), scale.y(), scale.z());
			}

			D3DXMATRIX matRes;
			D3DXMatrixMultiply(&matRes, &data->rotMatrix, &data->transMatrix);
			D3DXMatrixMultiply(&matRes, &data->scaleMatrix, &matRes);
			data->worldMatrix = matRes;
		}

		resInstance->setDirtyFlags(0);
	}

	const float PI = 3.14159f;
	void D3D9Engine::updateInternalCamera(Camera *camera) {
		D3DXMATRIX temp;
		if (camera && camera->dirtyFlags() != 0) {
			unsigned long flags = camera->dirtyFlags();
			D3D9CameraData *data = (D3D9CameraData*)camera->internalData();
			Vector3 pos = camera->pos();
			Vector3 rot = camera->rot();
			if (flags & Camera::DIRTY_POS)
				D3DXMatrixTranslation(&data->transMatrix, -pos.x(), -pos.y(), -pos.z());
			if (flags & Camera::DIRTY_ROT) {
				D3DXMATRIX rotZ, rotX, rotY;
				D3DXMATRIX revRot;
				D3DXMatrixRotationZ(&rotZ, rot.z());
				D3DXMatrixRotationX(&rotX, rot.x());
				D3DXMatrixRotationY(&rotY, rot.y());
				revRot = rotZ;
				D3DXMatrixMultiply(&revRot, &rotX, &revRot);
				D3DXMatrixMultiply(&revRot, &rotY, &revRot);
				data->rotMatrix = revRot;
			}
			if (flags & Camera::DIRTY_ZOOM)
				D3DXMatrixScaling(&data->scaleMatrix, 1.f, 1.f, camera->zoom());

			D3DXMatrixIdentity(&temp);
			D3DXMatrixMultiply(&temp, &data->rotMatrix, &data->scaleMatrix);
			D3DXMatrixMultiply(&temp, &data->transMatrix, &temp);

			data->viewMatrix = temp;
			camera->setDirtyFlags(0);
		}
	}

	void D3D9Engine::updateInternalViewport(Viewport *viewport) {
		D3DVIEWPORT9 *data = (D3DVIEWPORT9*)viewport->internalData();
		data->X = viewport->x();
		data->Y = viewport->y();
		data->Width = viewport->width();
		data->Height = viewport->height();
	}

	void D3D9Engine::setViewFromCamera(Camera *camera) {
		HRESULT hr;
		D3D9CameraData *data = (D3D9CameraData*)camera->internalData();
		hr = m_device->SetTransform(D3DTS_VIEW, &data->viewMatrix);
	}

	void *D3D9Engine::newInternalResource(ResourceData *resData) {
		HRESULT hr;
		LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
		LPDIRECT3DINDEXBUFFER9 indexBuffer;
		int vertexCount = resData->vertexCount();
		Vector3 *vertices = resData->vertices();
		int indexCount = resData->indexCount();
		Vector3 *normals = resData->normals();
		Color *colors = resData->colors();
		unsigned int stride = sizeof(Vector3) + sizeof(Vector3) + sizeof(D3DCOLOR);
		hr = m_device->CreateVertexBuffer(vertexCount * stride, 0, 0, D3DPOOL_MANAGED, &vertexBuffer, NULL);
		hr = m_device->CreateIndexBuffer(indexCount * 4, 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &indexBuffer, NULL);
		BYTE *pData;
		vertexBuffer->Lock(0, 0, (void**)&pData, 0);/*
		for (int i = 0; i < vertexCount; ++i) {
			memcpy(pData, &vertices[i], sizeof(Vector3));
			pData += sizeof(Vector3);

			*(D3DVECTOR*)pData = D3DVECTOR({ 0.f, 1.f, 0.f });
			//memcpy(pData, &normals[i], sizeof(Vector3));
			pData += sizeof(Vector3);

			*(D3DCOLOR*)pData = D3DCOLOR_ARGB(colors[i].a(), colors[i].r(), colors[i].g(), colors[i].b());
			pData += sizeof(D3DCOLOR);
		}
		*/
		vertexBuffer->Unlock();

		if (indexCount > 0) {
			indexBuffer->Lock(0, 0, (void**)&pData, 0);
			memcpy(pData, resData->indices(), indexCount * sizeof(int));
			indexBuffer->Unlock();
		}

		D3DVERTEXELEMENT9 custom_vertex[] =
		{
		  { 0,  0, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		  { 0, 12, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
		  { 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0 },
		  D3DDECL_END()
		};
		static LPDIRECT3DVERTEXDECLARATION9 vertexDeclaration = NULL;
		hr = m_device->CreateVertexDeclaration(custom_vertex, &vertexDeclaration);
		D3D9ResData *internalData = new D3D9ResData;
		internalData->vertexBuffer = vertexBuffer;
		internalData->indexBuffer = indexBuffer;
		internalData->vertexDeclaration = vertexDeclaration;
		internalData->stride = stride;
		return internalData;
	}

	void *D3D9Engine::newInternalCamera(Camera *camera) {
		D3DXMATRIX res;
		D3D9CameraData *data = new D3D9CameraData;
		Vector3 pos = camera->pos();
		Vector3 rot = camera->rot();
		D3DXMatrixTranslation(&data->transMatrix, -pos.x(), -pos.y(), -pos.z());
		D3DXMatrixRotationYawPitchRoll(&data->rotMatrix, rot.y(), rot.x(), rot.z());
		D3DXMatrixScaling(&data->scaleMatrix, 1.f, 1.f, camera->zoom());
		D3DXMatrixMultiply(&res, &data->rotMatrix, &data->scaleMatrix);
		D3DXMatrixMultiply(&res, &data->transMatrix, &res);
		data->viewMatrix = res;
		return data;
	}

	void *D3D9Engine::newInternalViewport(Viewport *viewport) {
		D3DVIEWPORT9 *data = new D3DVIEWPORT9;
		data->X = viewport->x();
		data->Y = viewport->y();
		data->Width = viewport->width();
		data->Height = viewport->height();
		data->MinZ = 0.f;
		data->MaxZ = 1.f;
		return data;
	}

	void *D3D9Engine::newInternalResourceInstance(ResourceInstance *instance) {
		D3D9ResInstanceData *internalData = new D3D9ResInstanceData;
		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&internalData->transMatrix);
		D3DXMatrixIdentity(&internalData->rotMatrix);
		D3DXMatrixIdentity(&internalData->scaleMatrix);
		D3DXMatrixIdentity(&matWorld);
		internalData->worldMatrix = matWorld;
		return internalData;
	}

	void *D3D9Engine::newInternalFontResource(FontResource *font) {
		ID3DXFont *d3dxFont;
		unsigned long flags = font->flags();
		UINT weight = flags & FontResource::BOLD ? FW_BOLD : FW_NORMAL;
		BOOL italic = (flags & FontResource::ITALIC) != 0;
		HRESULT hr = ::D3DXCreateFont(m_device, font->height(), font->width(), weight,
			1, italic,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLEARTYPE_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			font->face().c_str(), &d3dxFont);
		if (hr == S_OK) {
			return d3dxFont;
		}
		return NULL;
	}

	void *D3D9Engine::newInternalTextResource(TextResource *text) {
		return NULL;
	}

	void D3D9Engine::setCurrentInternalCamera(Camera *camera)
	{
		if (camera) {
			if (camera->dirtyFlags() != 0) {
				updateInternalCamera(camera);
			}
			setViewFromCamera(camera);
			/*
			D3DXMATRIX view_matrix;
			D3DXVECTOR3 eye_vector;
			D3DXVECTOR3 lookat_vector;
			D3DXVECTOR3 up_vector;

			Vector3 pos = camera->pos();
			Vector3 right = camera->right();
			Vector3 up = camera->up();
			eye_vector = D3DXVECTOR3(pos.x(), pos.y(), pos.z());

			//We are looking towards the origin
			lookat_vector = D3DXVECTOR3(right.x(),right.y(),right.z());

			//The "up" direction is the positive direction on the y-axis
			up_vector = D3DXVECTOR3(up.x(),up.y(),up.z());
			D3DXMatrixLookAtLH(&view_matrix, &eye_vector, &lookat_vector, &up_vector);
			*/
		}
	}

	void D3D9Engine::setCurrentInternalViewport(Viewport *viewport)
	{
		HRESULT hr;
		if (viewport) {
			if (viewport->dirtyFlags() != 0) {
				updateInternalViewport(viewport);
			}

			hr = m_device->SetViewport((D3DVIEWPORT9*)viewport->internalData());
		}
	}

	void D3D9Engine::setCurrentInternalFont(FontResource *font)
	{
		if (font) {
			m_currentFont = (LPD3DXFONT)font->internalData();
		}
	}

	D3DMATRIX translate(float dx, float dy, float dz) {
		D3DMATRIX matT = { 0 };
		matT._11 = matT._22 = matT._33 = matT._44 = 1.;
		matT._41 = dx;
		matT._42 = dy;
		matT._43 = dz;
		return matT;
	}

	D3DMATRIX scale(float sx, float sy, float sz) {
		D3DMATRIX matS = { 0 };
		matS._11 = sx;
		matS._22 = sy;
		matS._33 = sz;
		matS._44 = 1.;
		return matS;
	}

	D3DMATRIX scale(float s) {
		D3DMATRIX matS = { 0 };
		matS._11 = matS._22 = matS._33 = s;
		matS._44 = 1.;
		return matS;
	}

	D3DMATRIX rotateX(float angle) {
		D3DMATRIX matRX = { 0 };
		float sin, cos;
		sin = sinf(angle);
		cos = cosf(angle);
		matRX._11 = matRX._44 = 1.0f;
		matRX._22 = cos;
		matRX._23 = sin;
		matRX._32 = -sin;
		matRX._33 = cos;
		return matRX;
	}

	D3DMATRIX rotateY(float angle) {
		D3DMATRIX matRY = { 0 };
		float sin, cos;
		sin = sinf(angle);
		cos = cosf(angle);
		matRY._22 = matRY._44 = 1.0f;
		matRY._11 = cos;
		matRY._13 = -sin;
		matRY._31 = sin;
		matRY._33 = cos;
		return matRY;
	}

	D3DMATRIX rotateZ(float angle) {
		D3DMATRIX matRZ = { 0 };
		float sin, cos;
		sin = sinf(angle);
		cos = cosf(angle);
		matRZ._33 = matRZ._44 = 1.0f;
		matRZ._11 = cos;
		matRZ._12 = sin;
		matRZ._21 = -sin;
		matRZ._22 = cos;
		return matRZ;
	}


	void D3D9Engine::drawInternalResource(ResourceInstance *instance) {
		HRESULT hr;
		static bool first = false;
		if (!first) {
			D3DLIGHT9 light;
			D3DMATERIAL9 material;

			ZeroMemory(&light, sizeof(D3DMATERIAL9));
			ZeroMemory(&material, sizeof(D3DMATERIAL9));

			//light.Type = D3DLIGHT_POINT;
			//light.Position = D3DVECTOR({ 0.f, 5.f, 0.f });
			//light.Direction = D3DVECTOR({ 0.f,-1.f,0.f });
			light.Type = D3DLIGHT_DIRECTIONAL;
			light.Direction = D3DXVECTOR3(0, -1, 0);
			light.Range = 100.0f;
			light.Diffuse.r = 1.f;
			light.Diffuse.g = 1.f;
			light.Diffuse.b = 1.f;

			light.Ambient.r = 0.2f;
			light.Ambient.g = 0.2f;
			light.Ambient.b = 0.2f;

			material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			//material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			m_device->SetMaterial(&material);

			m_device->SetLight(0, &light);
			
			first = true;
		}
		m_device->LightEnable(0, TRUE);
		ResourceData *resData = this->resourceData(instance->resDataId());
		if (resData->dirtyFlags() != 0) {
			updateInternalResourceData(resData);
		}

		if (instance->dirtyFlags() != 0) {
			if (resData->dirtyFlags())
				int a = 1; //do something
			updateInternalResourceInstanceData(instance);
		}

		D3D9ResInstanceData *d3d9InstanceData = (D3D9ResInstanceData*)instance->internalData();
		D3D9ResData *d3d9ResData = (D3D9ResData*)resData->internalData();

		m_device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		m_device->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

		if (instance->drawFlags() & ResourceInstance::BACKFACE_CULL)
			m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		else
			m_device->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);

		if (instance->drawFlags() & ResourceInstance::WIREFRAME)
			m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		else
			m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		m_device->SetTransform(D3DTS_WORLD, &d3d9InstanceData->worldMatrix);
		hr = m_device->SetVertexDeclaration(d3d9ResData->vertexDeclaration);
		hr = m_device->SetStreamSource(0, d3d9ResData->vertexBuffer, 0, d3d9ResData->stride);
		hr = m_device->SetIndices(d3d9ResData->indexBuffer);
		hr = m_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
			0, 0,
			resData->vertexCount(),
			0,
			resData->indexCount() / 3
		);
	}

	void D3D9Engine::drawText(const char *text, int x, int y, const Color &color) {
		ID3DXFont *font = m_currentFont;
		if (font) {
			RECT rect = { x,y,0,0 };
			font->DrawText(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_RGBA(color.r(), color.g(), color.b(), color.a()));
		}
	}

	void D3D9Engine::drawResourceDirect(render3d::Vector3 vertices[], int vertexCount, render3d::Color colorList[]) {
		struct CustomVertex {
			float x, y, z;
			D3DCOLOR color;
		};
		CustomVertex *data = new CustomVertex[vertexCount];
		for (int i = 0; i < vertexCount; ++i) {
			data[i].x = vertices[i].x();
			data[i].y = vertices[i].y();
			data[i].z = vertices[i].z();
			data[i].color = D3DCOLOR_ARGB(255, colorList[i].r(), colorList[i].g(), colorList[i].b());
		}
		D3DXMATRIX mat;
		D3DXMatrixIdentity(&mat);
		m_device->SetTransform(D3DTS_WORLD, &mat);
		m_device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
		m_device->LightEnable(0, FALSE);
		m_device->DrawPrimitiveUP(D3DPT_LINELIST,vertexCount/2, data, sizeof(CustomVertex));
		delete[] data;
	}

}