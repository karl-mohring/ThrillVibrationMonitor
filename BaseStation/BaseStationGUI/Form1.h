#pragma once

namespace BaseStationGUI {

	using namespace System;
	using namespace System::IO;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>


	/**
	* Data command protocol for xbee communication.
	* Can have up to 32 commands
	*/
	enum COMMAND_IDS{
		CONNECT_REQUEST = 1,
		STATUS_REQUEST = 2,
		START_RECORDING = 3,
		STOP_RECORDING = 4,
		DATA_PACKET = 5,
		RESEND_PACKET = 6,
		RESET_UNIT = 7
	};

	/**
	* Xbee hardware addresses of patient modules.
	* Currently using placeholder values
	*/
	enum XBEE_ADDRESSES{
		UNIT1_ADDRESS = 1,
		UNIT2_ADDRESS = 2,
		UNIT3_ADDRESS = 3,
		UNIT4_ADDRESS = 4
	};

	enum XBEE_UNITS{
		UNIT1 = 0,
		UNIT2 = 1,
		UNIT3 = 2,
		UNIT4 = 3
	};

	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			initialiseBaseStation();


			//
			//TODO: Add the constructor code here
			//
		}

		ref struct RemoteUnit{
			bool isConnected;
			bool isReady;
			int address;
			bool hasData;
			StreamWriter^ fileWriter;
			String^ patientLName;
			String^ patientFName;
			String^ patientNumber;
		};

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components; 
			}
		}
	public: static const int NUM_PATIENT_UNITS = 4;

	private: cli::array<RemoteUnit^>^ patientUnits;
	private: cli::array < System::IO::FileStream ^> ^ OutputFileHandle;

			 // GUI Elements
	private: System::Windows::Forms::Panel^  panel1;
	protected: 



	private: System::Windows::Forms::Label^  label1;
	private: System::IO::Ports::SerialPort^  xbee;
	private: System::Windows::Forms::Timer^  statusUpdateTimer;

	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::Timer^  connectionCheckTimer;

	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  unit1FirstNameField;

	private: System::Windows::Forms::TextBox^  unit1PatientNumField;
	private: System::Windows::Forms::TextBox^  unit1LastNameField;


	private: System::Windows::Forms::Button^  unit1SaveButton;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::TextBox^  unit1ConnectStatusLabel;
	private: System::Windows::Forms::TextBox^  unit1StatusLabel;


	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->unit1StatusLabel = (gcnew System::Windows::Forms::TextBox());
			this->unit1ConnectStatusLabel = (gcnew System::Windows::Forms::TextBox());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->unit1SaveButton = (gcnew System::Windows::Forms::Button());
			this->unit1FirstNameField = (gcnew System::Windows::Forms::TextBox());
			this->unit1LastNameField = (gcnew System::Windows::Forms::TextBox());
			this->unit1PatientNumField = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->xbee = (gcnew System::IO::Ports::SerialPort(this->components));
			this->statusUpdateTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->connectionCheckTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->panel1->SuspendLayout();
			this->panel2->SuspendLayout();
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::SystemColors::ControlLight;
			this->panel1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->panel1->Controls->Add(this->unit1StatusLabel);
			this->panel1->Controls->Add(this->unit1ConnectStatusLabel);
			this->panel1->Controls->Add(this->panel2);
			this->panel1->Controls->Add(this->label1);
			this->panel1->Location = System::Drawing::Point(12, 12);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(600, 90);
			this->panel1->TabIndex = 0;
			// 
			// unit1StatusLabel
			// 
			this->unit1StatusLabel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->unit1StatusLabel->ForeColor = System::Drawing::Color::Maroon;
			this->unit1StatusLabel->Location = System::Drawing::Point(7, 64);
			this->unit1StatusLabel->Name = L"unit1StatusLabel";
			this->unit1StatusLabel->ReadOnly = true;
			this->unit1StatusLabel->Size = System::Drawing::Size(75, 20);
			this->unit1StatusLabel->TabIndex = 2;
			this->unit1StatusLabel->Text = L"Not Ready";
			this->unit1StatusLabel->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// unit1ConnectStatusLabel
			// 
			this->unit1ConnectStatusLabel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->unit1ConnectStatusLabel->ForeColor = System::Drawing::Color::Maroon;
			this->unit1ConnectStatusLabel->Location = System::Drawing::Point(7, 38);
			this->unit1ConnectStatusLabel->Name = L"unit1ConnectStatusLabel";
			this->unit1ConnectStatusLabel->ReadOnly = true;
			this->unit1ConnectStatusLabel->Size = System::Drawing::Size(75, 20);
			this->unit1ConnectStatusLabel->TabIndex = 2;
			this->unit1ConnectStatusLabel->Text = L"Disconnected";
			this->unit1ConnectStatusLabel->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// panel2
			// 
			this->panel2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel2->Controls->Add(this->unit1SaveButton);
			this->panel2->Controls->Add(this->unit1FirstNameField);
			this->panel2->Controls->Add(this->unit1LastNameField);
			this->panel2->Controls->Add(this->unit1PatientNumField);
			this->panel2->Controls->Add(this->label7);
			this->panel2->Controls->Add(this->label6);
			this->panel2->Controls->Add(this->label5);
			this->panel2->Location = System::Drawing::Point(278, 4);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(315, 79);
			this->panel2->TabIndex = 1;
			// 
			// unit1SaveButton
			// 
			this->unit1SaveButton->BackColor = System::Drawing::SystemColors::ControlDark;
			this->unit1SaveButton->Enabled = false;
			this->unit1SaveButton->Location = System::Drawing::Point(235, 33);
			this->unit1SaveButton->Name = L"unit1SaveButton";
			this->unit1SaveButton->Size = System::Drawing::Size(75, 39);
			this->unit1SaveButton->TabIndex = 3;
			this->unit1SaveButton->Text = L"Save";
			this->unit1SaveButton->UseVisualStyleBackColor = false;
			this->unit1SaveButton->Click += gcnew System::EventHandler(this, &Form1::unit1SaveButton_Click);
			// 
			// unit1FirstNameField
			// 
			this->unit1FirstNameField->BackColor = System::Drawing::SystemColors::ControlDark;
			this->unit1FirstNameField->Location = System::Drawing::Point(3, 49);
			this->unit1FirstNameField->Name = L"unit1FirstNameField";
			this->unit1FirstNameField->ReadOnly = true;
			this->unit1FirstNameField->Size = System::Drawing::Size(100, 20);
			this->unit1FirstNameField->TabIndex = 2;
			// 
			// unit1LastNameField
			// 
			this->unit1LastNameField->BackColor = System::Drawing::SystemColors::ControlDark;
			this->unit1LastNameField->Location = System::Drawing::Point(109, 49);
			this->unit1LastNameField->Name = L"unit1LastNameField";
			this->unit1LastNameField->ReadOnly = true;
			this->unit1LastNameField->Size = System::Drawing::Size(100, 20);
			this->unit1LastNameField->TabIndex = 2;
			this->unit1LastNameField->TextChanged += gcnew System::EventHandler(this, &Form1::textBox1_TextChanged);
			// 
			// unit1PatientNumField
			// 
			this->unit1PatientNumField->BackColor = System::Drawing::SystemColors::ControlDark;
			this->unit1PatientNumField->Location = System::Drawing::Point(90, 1);
			this->unit1PatientNumField->Name = L"unit1PatientNumField";
			this->unit1PatientNumField->ReadOnly = true;
			this->unit1PatientNumField->Size = System::Drawing::Size(119, 20);
			this->unit1PatientNumField->TabIndex = 2;
			this->unit1PatientNumField->TextChanged += gcnew System::EventHandler(this, &Form1::textBox2_TextChanged);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(4, 33);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(57, 13);
			this->label7->TabIndex = 1;
			this->label7->Text = L"First Name";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(106, 33);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(58, 13);
			this->label6->TabIndex = 1;
			this->label6->Text = L"Last Name";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(4, 4);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(80, 13);
			this->label5->TabIndex = 1;
			this->label5->Text = L"Patient Number";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(4, 4);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(80, 29);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Unit 1";
			// 
			// xbee
			// 
			this->xbee->DataReceived += gcnew System::IO::Ports::SerialDataReceivedEventHandler(this, &Form1::xbee_DataReceived);
			// 
			// statusUpdateTimer
			// 
			this->statusUpdateTimer->Interval = 1000;
			this->statusUpdateTimer->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// connectionCheckTimer
			// 
			this->connectionCheckTimer->Interval = 5000;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(459, 225);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(624, 441);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->panel1);
			this->Name = L"Form1";
			this->Text = L"Thrill Monitor Base Station";
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->panel2->ResumeLayout(false);
			this->panel2->PerformLayout();
			this->ResumeLayout(false);

		}

#pragma endregion

	private: void initialiseBaseStation(){
				 Console::WriteLine("Woo");
				 patientUnits = gcnew cli::array<RemoteUnit^>(NUM_PATIENT_UNITS);

				 initialisePatientUnits();

				 OutputFileHandle = gcnew cli::array < System::IO::FileStream ^ >(NUM_PATIENT_UNITS);


			 }

	private: void initialisePatientUnits(){
				 for (int i = 0; i < NUM_PATIENT_UNITS; i++){
					 patientUnits[i] = gcnew RemoteUnit;
				 }

				 patientUnits[UNIT1]->address = UNIT1_ADDRESS;
				 patientUnits[UNIT2]->address = UNIT2_ADDRESS;
				 patientUnits[UNIT3]->address = UNIT3_ADDRESS;
				 patientUnits[UNIT4]->address = UNIT4_ADDRESS;
			 }

	private: void unit1_onConnect(){
				 patientUnits[UNIT1]->isConnected = true;
				 patientUnits[UNIT1]->isReady = false;

				 unit1ConnectStatusLabel->Text = "Connected";
				 unit1ConnectStatusLabel->BackColor = System::Drawing::Color::LightGreen;
				 unit1ConnectStatusLabel->ForeColor = System::Drawing::Color::DarkGreen;

				 unit1UnlockFields();
			 }

	private: void unit1UnlockFields(){
				 unit1SaveButton->BackColor = System::Drawing::SystemColors::Control;
				 unit1SaveButton->Enabled = true;

				 unit1FirstNameField->BackColor = System::Drawing::SystemColors::Window;
				 unit1FirstNameField->ReadOnly = false;

				 unit1LastNameField->BackColor = System::Drawing::SystemColors::Window;
				 unit1LastNameField->ReadOnly = false;

				 unit1PatientNumField->BackColor = System::Drawing::SystemColors::Window;
				 unit1PatientNumField->ReadOnly = false;
			 }

	private: void unit1LockFields(){
				 unit1SaveButton->BackColor = System::Drawing::SystemColors::ControlDark;
				 unit1SaveButton->Enabled = false;

				 unit1FirstNameField->BackColor = System::Drawing::SystemColors::ControlDark;
				 unit1FirstNameField->ReadOnly = true;

				 unit1LastNameField->BackColor = System::Drawing::SystemColors::ControlDark;
				 unit1LastNameField->ReadOnly = true;

				 unit1PatientNumField->BackColor = System::Drawing::SystemColors::ControlDark;
				 unit1PatientNumField->ReadOnly = true;
			 }

	private: System::Void panel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
			 }

	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
				 checkPatientMonitors();
			 }

	private: System::Void xbee_DataReceived(System::Object^  sender, System::IO::Ports::SerialDataReceivedEventArgs^  e) {
				 Byte receivedByte;
				 Byte sourceID;
				 Byte commandID;

				 receivedByte = xbee->ReadByte();

				 // Extract source address 3-bit
				 sourceID = receivedByte >> 5;

				 // Extract command 5-bit
				 commandID = receivedByte | 0x1F;

				 switch (commandID)
				 {
					 // Patient module requesting connection
				 case CONNECT_REQUEST:
					 break;

					 // Reply from status request. Contains single status byte. (maybe 2 if battery included)
				 case STATUS_REQUEST:
					 break;

					 // Unit has started recording
				 case START_RECORDING:
					 break;

					 // Unit has stopped recording
				 case STOP_RECORDING:
					 break;

					 // Recorded data packet
				 case DATA_PACKET:
					 break;

					 // N/A: Command should be sent only
				 case RESEND_PACKET:
					 break;

					 // N/A: Command should be sent only
				 case RESET_UNIT:
					 break;

					 // Unknown command ID
				 default:
					 break;
				 }
			 }

	private: void checkPatientMonitors(){
				 // Cycle through each connected unit and request status
			 }

	private: void sendStatusRequest(int targetAddress){
				 //xbee_sendByte whatever
			 }

	private: System::Void label4_Click(System::Object^  sender, System::EventArgs^  e) {
			 }

	private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 }

	private: System::Void textBox2_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 }

	private: System::Void unit1SaveButton_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (unit1PatientNumField->Text != "" && unit1LastNameField->Text != ""){
					 patientUnits[UNIT1]->patientNumber = unit1PatientNumField->Text;
					 patientUnits[UNIT1]->patientLName = unit1LastNameField->Text;
					 patientUnits[UNIT1]->patientFName = unit1FirstNameField->Text;

					 patientUnits[UNIT1]->isReady = true;

					 unit1StatusLabel_setReady();
					 unit1LockFields();
				 }
			 }

	private: void unit1StatusLabel_setReady(){
				 unit1StatusLabel->Text = "Ready";
				 unit1StatusLabel->BackColor = System::Drawing::Color::Yellow;
				 unit1StatusLabel->ForeColor = System::Drawing::Color::DarkGray;
			 }

	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 unit1_onConnect();
			 }


	};
}

