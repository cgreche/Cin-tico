
#ifndef __CINETICO_INPUT_H__
#define __CINETICO_INPUT_H__

namespace cinetico {

	class Input {

	public:
		class Keyboard {
			bool m_keys[256];
		public:
			Keyboard() {
				for (int i = 0; i < 256; ++i)
					m_keys[i] = false;
			}
			bool key(int key) const { return m_keys[key]; }
			friend class MainWindow;
		};

		class Mouse {
			int m_x;
			int m_y;
			int m_z;
			bool m_buttons[5];
		public:
			Mouse() {
				m_x = m_y = m_z = 0;
				for (int i = 0; i < 5; ++i)
					m_buttons[i] = false;
			}

			int x() const { return m_x; }
			int y() const { return m_y; }
			int z() const { return m_z; }
			bool button(int button) { return m_buttons[button]; }
			friend class MainWindow;
		};

		Keyboard keyboard;
		Mouse mouse;

		friend class MainWindow;
	};

}

#endif