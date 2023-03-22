#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settins");
		if (ImGui::Button("Render"))
		{
			Render();
		}
		ImGui::End();
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoScrollbar);
		m_ViewportW = ImGui::GetContentRegionAvail().x;
		m_ViewportH = ImGui::GetContentRegionAvail().y;

		if (m_Image)
			ImGui::Image(m_Image->GetDescriptorSet(), { (float)m_Image->GetWidth(), (float)m_Image->GetHeight() });
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void Render()
	{
		if (!m_Image || m_ViewportW != m_Image->GetWidth() || m_ViewportH != m_Image->GetHeight())
		{
			m_Image = std::make_shared<Image>(m_ViewportW, m_ViewportH, ImageFormat::RGBA);
			delete[] m_ImageData;
			m_ImageData = new uint32_t[m_ViewportW * m_ViewportH];
		}
		for (uint32_t i = 0; i < m_ViewportW * m_ViewportH; i++)
		{
			m_ImageData[i] = Random::UInt();
			m_ImageData[i] |= 0xff000000;
		}

		m_Image->SetData(m_ImageData);

	}
private:
	std::shared_ptr<Image> m_Image;
	uint32_t m_ViewportW = 0;
	uint32_t m_ViewportH = 0;
	uint32_t* m_ImageData = nullptr;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracer";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}