<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="19008000">
	<Item Name="My Computer" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="HMI" Type="Folder" URL="../HMI">
			<Property Name="NI.DISK" Type="Bool">true</Property>
		</Item>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="8.6CompatibleGlobalVar.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/8.6CompatibleGlobalVar.vi"/>
				<Item Name="API Main.lvlib" Type="Library" URL="/&lt;vilib&gt;/NI/Modbus Library/API/Wrapper/API Main.lvlib"/>
				<Item Name="Application Directory.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Application Directory.vi"/>
				<Item Name="ASCII Data Unit.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/NI/Modbus Library/Transmission Data Unit/ASCII/ASCII Data Unit.lvclass"/>
				<Item Name="Bits to Bytes.vi" Type="VI" URL="/&lt;vilib&gt;/NI/Modbus Library/Utility/Bits to Bytes.vi"/>
				<Item Name="BuildHelpPath.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/BuildHelpPath.vi"/>
				<Item Name="Bytes to Bits.vi" Type="VI" URL="/&lt;vilib&gt;/NI/Modbus Library/Utility/Bytes to Bits.vi"/>
				<Item Name="Bytes to U16s.vi" Type="VI" URL="/&lt;vilib&gt;/NI/Modbus Library/Utility/Bytes to U16s.vi"/>
				<Item Name="Check if File or Folder Exists.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/libraryn.llb/Check if File or Folder Exists.vi"/>
				<Item Name="Check Special Tags.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Check Special Tags.vi"/>
				<Item Name="Clear Errors.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Clear Errors.vi"/>
				<Item Name="Close File+.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Close File+.vi"/>
				<Item Name="compatReadText.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/compatReadText.vi"/>
				<Item Name="Convert property node font to graphics font.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Convert property node font to graphics font.vi"/>
				<Item Name="Delimited String to 1D String Array.vi" Type="VI" URL="/&lt;vilib&gt;/AdvancedString/Delimited String to 1D String Array.vi"/>
				<Item Name="Details Display Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Details Display Dialog.vi"/>
				<Item Name="Device Data Model.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/NI/Modbus Library/Data Model/Device Data Model.lvclass"/>
				<Item Name="DialogType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogType.ctl"/>
				<Item Name="DialogTypeEnum.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogTypeEnum.ctl"/>
				<Item Name="Equal Comparable.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/Comparison/Equal/Equal Comparable/Equal Comparable.lvclass"/>
				<Item Name="Equal Functor.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/Comparison/Equal/Equal Functor/Equal Functor.lvclass"/>
				<Item Name="Equals.vim" Type="VI" URL="/&lt;vilib&gt;/Comparison/Equals.vim"/>
				<Item Name="Error Cluster From Error Code.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Cluster From Error Code.vi"/>
				<Item Name="Error Code Database.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Code Database.vi"/>
				<Item Name="ErrWarn.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/ErrWarn.ctl"/>
				<Item Name="eventvkey.ctl" Type="VI" URL="/&lt;vilib&gt;/event_ctls.llb/eventvkey.ctl"/>
				<Item Name="Find First Error.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Find First Error.vi"/>
				<Item Name="Find Tag.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Find Tag.vi"/>
				<Item Name="Format Message String.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Format Message String.vi"/>
				<Item Name="General Error Handler Core CORE.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler Core CORE.vi"/>
				<Item Name="General Error Handler.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler.vi"/>
				<Item Name="Generate UUID.vi" Type="VI" URL="/&lt;vilib&gt;/NI/Modbus Library/Utility/Generate UUID.vi"/>
				<Item Name="Get String Text Bounds.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Get String Text Bounds.vi"/>
				<Item Name="Get Text Rect.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Get Text Rect.vi"/>
				<Item Name="GetHelpDir.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/GetHelpDir.vi"/>
				<Item Name="GetRTHostConnectedProp.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/GetRTHostConnectedProp.vi"/>
				<Item Name="IP Data Unit.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/NI/Modbus Library/Transmission Data Unit/IP/IP Data Unit.lvclass"/>
				<Item Name="Longest Line Length in Pixels.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Longest Line Length in Pixels.vi"/>
				<Item Name="LVBoundsTypeDef.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/miscctls.llb/LVBoundsTypeDef.ctl"/>
				<Item Name="LVRectTypeDef.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/miscctls.llb/LVRectTypeDef.ctl"/>
				<Item Name="Master Function Definition.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/NI/Modbus Library/Master Function Definition/Master Function Definition.lvclass"/>
				<Item Name="Modbus API.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/NI/Modbus Library/API/Modbus API.lvclass"/>
				<Item Name="Modbus Data Unit.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/NI/Modbus Library/Modbus Data Unit/Modbus Data Unit.lvclass"/>
				<Item Name="Modbus Master.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/NI/Modbus Library/API/Master/Modbus Master.lvclass"/>
				<Item Name="Modbus Slave.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/NI/Modbus Library/API/Slave/Modbus Slave.lvclass"/>
				<Item Name="Network Master.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/NI/Modbus Library/Network Protocol/Network Master/Network Master.lvclass"/>
				<Item Name="Network Protocol.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/NI/Modbus Library/Network Protocol/Network Protocol.lvclass"/>
				<Item Name="Network Slave.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/NI/Modbus Library/Network Protocol/Network Slave/Network Slave.lvclass"/>
				<Item Name="NI_FileType.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/lvfile.llb/NI_FileType.lvlib"/>
				<Item Name="NI_LVConfig.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/config.llb/NI_LVConfig.lvlib"/>
				<Item Name="NI_PackedLibraryUtility.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/LVLibp/NI_PackedLibraryUtility.lvlib"/>
				<Item Name="Not Found Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Not Found Dialog.vi"/>
				<Item Name="Open File+.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Open File+.vi"/>
				<Item Name="Read Delimited Spreadsheet (DBL).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Read Delimited Spreadsheet (DBL).vi"/>
				<Item Name="Read Delimited Spreadsheet (I64).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Read Delimited Spreadsheet (I64).vi"/>
				<Item Name="Read Delimited Spreadsheet (string).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Read Delimited Spreadsheet (string).vi"/>
				<Item Name="Read Delimited Spreadsheet.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Read Delimited Spreadsheet.vi"/>
				<Item Name="Read File+ (string).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Read File+ (string).vi"/>
				<Item Name="Read Lines From File (with error IO).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Read Lines From File (with error IO).vi"/>
				<Item Name="RTU Data Unit.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/NI/Modbus Library/Transmission Data Unit/RTU/RTU Data Unit.lvclass"/>
				<Item Name="Search and Replace Pattern.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Search and Replace Pattern.vi"/>
				<Item Name="Search Unsorted 1D Array Core.vim" Type="VI" URL="/&lt;vilib&gt;/Array/Helpers/Search Unsorted 1D Array Core.vim"/>
				<Item Name="Search Unsorted 1D Array.vim" Type="VI" URL="/&lt;vilib&gt;/Array/Search Unsorted 1D Array.vim"/>
				<Item Name="Serial Data Unit.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/NI/Modbus Library/Transmission Data Unit/Serial Interface/Serial Data Unit.lvclass"/>
				<Item Name="Serial Master.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/NI/Modbus Library/Network Protocol/Network Master/Serial/Serial Master.lvclass"/>
				<Item Name="Serial Shared Components.lvlib" Type="Library" URL="/&lt;vilib&gt;/NI/Modbus Library/Network Protocol/Serial Shared Components/Serial Shared Components.lvlib"/>
				<Item Name="Serial Slave.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/NI/Modbus Library/Network Protocol/Network Slave/Serial/Serial Slave.lvclass"/>
				<Item Name="Set Bold Text.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set Bold Text.vi"/>
				<Item Name="Set String Value.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set String Value.vi"/>
				<Item Name="Simple Error Handler.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Simple Error Handler.vi"/>
				<Item Name="Space Constant.vi" Type="VI" URL="/&lt;vilib&gt;/dlg_ctls.llb/Space Constant.vi"/>
				<Item Name="Standard Data Model.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/NI/Modbus Library/Data Model/Standard Data Model/Standard Data Model.lvclass"/>
				<Item Name="TagReturnType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/TagReturnType.ctl"/>
				<Item Name="TCP Master.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/NI/Modbus Library/Network Protocol/Network Master/TCP/TCP Master.lvclass"/>
				<Item Name="TCP Shared Components.lvlib" Type="Library" URL="/&lt;vilib&gt;/NI/Modbus Library/Network Protocol/TCP Shared Components/TCP Shared Components.lvlib"/>
				<Item Name="TCP Slave.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/NI/Modbus Library/Network Protocol/Network Slave/TCP/TCP Slave.lvclass"/>
				<Item Name="Three Button Dialog CORE.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Three Button Dialog CORE.vi"/>
				<Item Name="Three Button Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Three Button Dialog.vi"/>
				<Item Name="Transmission Data Unit.lvclass" Type="LVClass" URL="/&lt;vilib&gt;/NI/Modbus Library/Transmission Data Unit/Transmission Data Unit.lvclass"/>
				<Item Name="Trim Whitespace.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Trim Whitespace.vi"/>
				<Item Name="U16s to Bytes.vi" Type="VI" URL="/&lt;vilib&gt;/NI/Modbus Library/Utility/U16s to Bytes.vi"/>
				<Item Name="usereventprio.ctl" Type="VI" URL="/&lt;vilib&gt;/event_ctls.llb/usereventprio.ctl"/>
				<Item Name="VISA Flush IO Buffer Mask.ctl" Type="VI" URL="/&lt;vilib&gt;/Instr/_visa.llb/VISA Flush IO Buffer Mask.ctl"/>
				<Item Name="whitespace.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/whitespace.ctl"/>
				<Item Name="Write Delimited Spreadsheet (DBL).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Write Delimited Spreadsheet (DBL).vi"/>
				<Item Name="Write Delimited Spreadsheet (I64).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Write Delimited Spreadsheet (I64).vi"/>
				<Item Name="Write Delimited Spreadsheet (string).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Write Delimited Spreadsheet (string).vi"/>
				<Item Name="Write Delimited Spreadsheet.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Write Delimited Spreadsheet.vi"/>
				<Item Name="Write Spreadsheet String.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Write Spreadsheet String.vi"/>
			</Item>
			<Item Name="Build Error Cluster__ogtk.vi" Type="VI" URL="../../../../../../Program Files (x86)/National Instruments/LabVIEW 2020/user.lib/_OpenG.lib/error/error.llb/Build Error Cluster__ogtk.vi"/>
			<Item Name="Get Data Name from TD__ogtk.vi" Type="VI" URL="../../../../../../Program Files (x86)/National Instruments/LabVIEW 2020/user.lib/_OpenG.lib/lvdata/lvdata.llb/Get Data Name from TD__ogtk.vi"/>
			<Item Name="Get Data Name__ogtk.vi" Type="VI" URL="../../../../../../Program Files (x86)/National Instruments/LabVIEW 2020/user.lib/_OpenG.lib/lvdata/lvdata.llb/Get Data Name__ogtk.vi"/>
			<Item Name="Get Header from TD__ogtk.vi" Type="VI" URL="../../../../../../Program Files (x86)/National Instruments/LabVIEW 2020/user.lib/_OpenG.lib/lvdata/lvdata.llb/Get Header from TD__ogtk.vi"/>
			<Item Name="Get Last PString__ogtk.vi" Type="VI" URL="../../../../../../Program Files (x86)/National Instruments/LabVIEW 2020/user.lib/_OpenG.lib/lvdata/lvdata.llb/Get Last PString__ogtk.vi"/>
			<Item Name="Get PString__ogtk.vi" Type="VI" URL="../../../../../../Program Files (x86)/National Instruments/LabVIEW 2020/user.lib/_OpenG.lib/lvdata/lvdata.llb/Get PString__ogtk.vi"/>
			<Item Name="Get Strings from Enum TD__ogtk.vi" Type="VI" URL="../../../../../../Program Files (x86)/National Instruments/LabVIEW 2020/user.lib/_OpenG.lib/lvdata/lvdata.llb/Get Strings from Enum TD__ogtk.vi"/>
			<Item Name="Get Strings from Enum__ogtk.vi" Type="VI" URL="../../../../../../Program Files (x86)/National Instruments/LabVIEW 2020/user.lib/_OpenG.lib/lvdata/lvdata.llb/Get Strings from Enum__ogtk.vi"/>
			<Item Name="Set Control Value {Variant}__ogtk.vi" Type="VI" URL="../../../../../../Program Files (x86)/National Instruments/LabVIEW 2020/user.lib/_OpenG.lib/appcontrol/appcontrol.llb/Set Control Value {Variant}__ogtk.vi"/>
			<Item Name="sql_close.vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_LV_plus.llb/sql_close.vi"/>
			<Item Name="sql_cmd (db cluster).vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_instances.llb/sql_cmd (db cluster).vi"/>
			<Item Name="sql_cmd (db ref).vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_instances.llb/sql_cmd (db ref).vi"/>
			<Item Name="sql_cmd.vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_LV_plus.llb/sql_cmd.vi"/>
			<Item Name="sql_cmd_resp (db ref).vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_instances.llb/sql_cmd_resp (db ref).vi"/>
			<Item Name="sql_columns (db cluster).vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_instances.llb/sql_columns (db cluster).vi"/>
			<Item Name="sql_columns (db ref).vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_instances.llb/sql_columns (db ref).vi"/>
			<Item Name="sql_columns.vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_LV_plus.llb/sql_columns.vi"/>
			<Item Name="sql_delete (db cluster).vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_instances.llb/sql_delete (db cluster).vi"/>
			<Item Name="sql_delete (db ref).vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_instances.llb/sql_delete (db ref).vi"/>
			<Item Name="sql_delete.vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_LV_plus.llb/sql_delete.vi"/>
			<Item Name="sql_error.vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_LV_plus.llb/sql_error.vi"/>
			<Item Name="sql_info.vi" Type="VI" URL="../../../../../../../&lt;userlib&gt;/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_LV_plus.llb/sql_info.vi"/>
			<Item Name="sql_insert.vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_LV_plus.llb/sql_insert.vi"/>
			<Item Name="sql_insert_clust (db cluster).vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_instances.llb/sql_insert_clust (db cluster).vi"/>
			<Item Name="sql_insert_clust (db ref).vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_instances.llb/sql_insert_clust (db ref).vi"/>
			<Item Name="sql_open.vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_LV_plus.llb/sql_open.vi"/>
			<Item Name="sql_select.vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_LV_plus.llb/sql_select.vi"/>
			<Item Name="sql_select_clust (db ref).vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_instances.llb/sql_select_clust (db ref).vi"/>
			<Item Name="sql_select_str (db cluster).vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_instances.llb/sql_select_str (db cluster).vi"/>
			<Item Name="sql_table_XML.vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_instances.llb/sql_table_XML.vi"/>
			<Item Name="sql_table_XML_sub.vi" Type="VI" URL="../../../../../../../&lt;userlib&gt;/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_instances.llb/sql_table_XML_sub.vi"/>
			<Item Name="sql_tables (db cluster).vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_instances.llb/sql_tables (db cluster).vi"/>
			<Item Name="sql_tables (db_ref).vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_instances.llb/sql_tables (db_ref).vi"/>
			<Item Name="sql_tables.vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_LV_plus.llb/sql_tables.vi"/>
			<Item Name="sql_update (db cluster).vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_instances.llb/sql_update (db cluster).vi"/>
			<Item Name="sql_update (db ref).vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_instances.llb/sql_update (db ref).vi"/>
			<Item Name="sql_update.vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_LV_plus.llb/sql_update.vi"/>
			<Item Name="sql_update_clust (db cluster).vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_instances.llb/sql_update_clust (db cluster).vi"/>
			<Item Name="sql_update_clust (db ref).vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_instances.llb/sql_update_clust (db ref).vi"/>
			<Item Name="sql_XML.vi" Type="VI" URL="../HMI/SubVI/Database/sql_LV-1.12-LabView14 -ORA-MySQL/sql_LV-1.12/sql_LV_plus.llb/sql_XML.vi"/>
			<Item Name="Type Descriptor Enumeration__ogtk.ctl" Type="VI" URL="../../../../../../Program Files (x86)/National Instruments/LabVIEW 2020/user.lib/_OpenG.lib/lvdata/lvdata.llb/Type Descriptor Enumeration__ogtk.ctl"/>
			<Item Name="Type Descriptor Header__ogtk.ctl" Type="VI" URL="../../../../../../Program Files (x86)/National Instruments/LabVIEW 2020/user.lib/_OpenG.lib/lvdata/lvdata.llb/Type Descriptor Header__ogtk.ctl"/>
			<Item Name="Type Descriptor__ogtk.ctl" Type="VI" URL="../../../../../../Program Files (x86)/National Instruments/LabVIEW 2020/user.lib/_OpenG.lib/lvdata/lvdata.llb/Type Descriptor__ogtk.ctl"/>
			<Item Name="Variant to Header Info__ogtk.vi" Type="VI" URL="../../../../../../Program Files (x86)/National Instruments/LabVIEW 2020/user.lib/_OpenG.lib/lvdata/lvdata.llb/Variant to Header Info__ogtk.vi"/>
		</Item>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="SM Installer" Type="Installer">
				<Property Name="Destination[0].name" Type="Str">Solar Monitor</Property>
				<Property Name="Destination[0].parent" Type="Str">{3912416A-D2E5-411B-AFEE-B63654D690C0}</Property>
				<Property Name="Destination[0].tag" Type="Str">{75667EF3-469D-4770-B47D-A32341E7A38C}</Property>
				<Property Name="Destination[0].type" Type="Str">userFolder</Property>
				<Property Name="DestinationCount" Type="Int">1</Property>
				<Property Name="INST_author" Type="Str">VISI Sdn Bhd</Property>
				<Property Name="INST_autoIncrement" Type="Bool">true</Property>
				<Property Name="INST_buildLocation" Type="Path">../builds/Solar Monitor/SM Installer</Property>
				<Property Name="INST_buildLocation.type" Type="Str">relativeToCommon</Property>
				<Property Name="INST_buildSpecName" Type="Str">SM Installer</Property>
				<Property Name="INST_defaultDir" Type="Str">{75667EF3-469D-4770-B47D-A32341E7A38C}</Property>
				<Property Name="INST_includeError" Type="Bool">false</Property>
				<Property Name="INST_installerName" Type="Str">install.exe</Property>
				<Property Name="INST_productName" Type="Str">Solar Monitor</Property>
				<Property Name="INST_productVersion" Type="Str">1.0.6</Property>
				<Property Name="InstSpecBitness" Type="Str">32-bit</Property>
				<Property Name="InstSpecVersion" Type="Str">19018009</Property>
				<Property Name="MSI_arpCompany" Type="Str">VISI Sdn Bhd</Property>
				<Property Name="MSI_distID" Type="Str">{7E76B2FE-1253-4336-8700-C1E66E5594F2}</Property>
				<Property Name="MSI_hideNonRuntimes" Type="Bool">true</Property>
				<Property Name="MSI_osCheck" Type="Int">0</Property>
				<Property Name="MSI_upgradeCode" Type="Str">{72811D70-0CBC-4864-AD79-965C6309E611}</Property>
				<Property Name="RegDest[0].dirName" Type="Str">Software</Property>
				<Property Name="RegDest[0].dirTag" Type="Str">{DDFAFC8B-E728-4AC8-96DE-B920EBB97A86}</Property>
				<Property Name="RegDest[0].parentTag" Type="Str">2</Property>
				<Property Name="RegDestCount" Type="Int">1</Property>
				<Property Name="Source[0].dest" Type="Str">{75667EF3-469D-4770-B47D-A32341E7A38C}</Property>
				<Property Name="Source[0].File[0].dest" Type="Str">{75667EF3-469D-4770-B47D-A32341E7A38C}</Property>
				<Property Name="Source[0].File[0].name" Type="Str">Solar Monitor.exe</Property>
				<Property Name="Source[0].File[0].Shortcut[0].destIndex" Type="Int">0</Property>
				<Property Name="Source[0].File[0].Shortcut[0].name" Type="Str">Solar Monitor</Property>
				<Property Name="Source[0].File[0].Shortcut[0].subDir" Type="Str">Solar Monitor</Property>
				<Property Name="Source[0].File[0].ShortcutCount" Type="Int">1</Property>
				<Property Name="Source[0].File[0].tag" Type="Str">{2C8F34AF-BDB1-4CCE-A889-4CCC82E4144B}</Property>
				<Property Name="Source[0].FileCount" Type="Int">1</Property>
				<Property Name="Source[0].name" Type="Str">Solar Monitor</Property>
				<Property Name="Source[0].tag" Type="Ref">/My Computer/Build Specifications/Solar Monitor</Property>
				<Property Name="Source[0].type" Type="Str">EXE</Property>
				<Property Name="SourceCount" Type="Int">1</Property>
			</Item>
			<Item Name="Solar Monitor" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{B65FE1D5-E9DF-4623-914D-559DB3D6AE06}</Property>
				<Property Name="App_INI_GUID" Type="Str">{A5B2293E-1C16-4D91-88DB-8DF9C2101963}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{483A70E3-D7AD-460D-95E4-8BABB71CA89A}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">Solar Monitor</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../builds/NI_AB_PROJECTNAME/Solar Monitor</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{431A364A-4248-4AB7-8698-6990926E4F70}</Property>
				<Property Name="Bld_version.build" Type="Int">55</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">Solar Monitor.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../builds/NI_AB_PROJECTNAME/Solar Monitor/Solar Monitor.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../builds/NI_AB_PROJECTNAME/Solar Monitor/Config</Property>
				<Property Name="Destination[2].destName" Type="Str">Module</Property>
				<Property Name="Destination[2].path" Type="Path">../builds/NI_AB_PROJECTNAME/Solar Monitor/Module</Property>
				<Property Name="DestinationCount" Type="Int">3</Property>
				<Property Name="Exe_iconItemID" Type="Ref">/My Computer/HMI/Resource/icons8_Summer.ico</Property>
				<Property Name="Source[0].itemID" Type="Str">{AD8D6EB7-5D1A-4132-9DFD-D50B2CDD9DF7}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/HMI/Main HMI.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="Source[2].Container.applyDestination" Type="Bool">true</Property>
				<Property Name="Source[2].Container.applyInclusion" Type="Bool">true</Property>
				<Property Name="Source[2].Container.depDestIndex" Type="Int">0</Property>
				<Property Name="Source[2].destinationIndex" Type="Int">2</Property>
				<Property Name="Source[2].itemID" Type="Ref">/My Computer/HMI/Module</Property>
				<Property Name="Source[2].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[2].type" Type="Str">Container</Property>
				<Property Name="Source[3].Container.applyInclusion" Type="Bool">true</Property>
				<Property Name="Source[3].Container.depDestIndex" Type="Int">0</Property>
				<Property Name="Source[3].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[3].itemID" Type="Ref">/My Computer/HMI/Config</Property>
				<Property Name="Source[3].sourceInclusion" Type="Str">Include</Property>
				<Property Name="Source[3].type" Type="Str">Container</Property>
				<Property Name="SourceCount" Type="Int">4</Property>
				<Property Name="TgtF_companyName" Type="Str">VISI Sdn Bhd</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Solar Monitor</Property>
				<Property Name="TgtF_internalName" Type="Str">Solar Monitor</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2022 VISI Sdn Bhd</Property>
				<Property Name="TgtF_productName" Type="Str">Solar Monitor</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{2C8F34AF-BDB1-4CCE-A889-4CCC82E4144B}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">Solar Monitor.exe</Property>
				<Property Name="TgtF_versionIndependent" Type="Bool">true</Property>
			</Item>
		</Item>
	</Item>
</Project>
