function varargout = guimechpit(varargin)
% GUIMECHPIT MATLAB code for guimechpit.fig
%      GUIMECHPIT, by itself, creates a new GUIMECHPIT or raises the existing
%      singleton*.
%
%      H = GUIMECHPIT returns the handle to a new GUIMECHPIT or the handle to
%      the existing singleton*.
%
%      GUIMECHPIT('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in GUIMECHPIT.M with the given input arguments.
%
%      GUIMECHPIT('Property','Value',...) creates a new GUIMECHPIT or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before guimechpit_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to guimechpit_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help guimechpit

% Last Modified by GUIDE v2.5 24-Oct-2013 11:44:21

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @guimechpit_OpeningFcn, ...
                   'gui_OutputFcn',  @guimechpit_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before guimechpit is made visible.
function guimechpit_OpeningFcn(hObject, eventdata, handles, varargin)
%Load_system('segway_complimentarry_filter.m');
segway_complimentarry_filter;
axes(handles.axes3);
imshow('segw1.jpg');
axes(handles.axes1);
legend('PWM','DriveSum')

 set(handles.checkbox1,'value',1);
  
 for i=1:5000
     arr(i,1)=0;
    
 end
 for i=1:5000
     arr(i,1)=0;
    
 end
 assignin('base','ausgabe',arr);
 assignin('base','ausgabePwm',arr);
 %Hier werden die Variablen gesetz für das auswählen welche kurve geplottet
 %werden soll
 assignin('base','anzeigePwm',1);
 assignin('base','anzeigeDriveSum',1);
 %   ausgabe
 
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to guimechpit (see VARARGIN)

% Choose default command line output for guimechpit
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes guimechpit wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = guimechpit_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;



% --- Executes on slider movement.
function slider1_Callback(hObject, eventdata, handles)
set(handles.text3, 'String', num2str(get(hObject, 'Value')));
if get(handles.slider1,'Value') == 0
    set(handles.checkbox1,'value',1);
else
    set(handles.checkbox1,'Value',0);
end
ang =  get(handles.slider1,'value');
ang = -ang*15;
axes(handles.axes3);
image = imread('segw1.jpg');
imgrot= imrotate(image,ang,'crop');
imshow(imgrot);
axes(handles.axes1);
if get(handles.checkbox1, 'Value') == 0 
    assignin('base','stand',0);
    val = get(handles.slider1,'Value');
     assignin ('base','eingang',val);
   set_param('segway_complimentarry_filter','SimulationCommand','update');
else
    assignin ('base','stand',1);
     assignin ('base','eingang',0);
    set_param('segway_complimentarry_filter','SimulationCommand','update');
   
end


%axes(handles.axes3);
%imrotate
%imshow('segw1.jpg');

% hObject    handle to slider1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider


% --- Executes during object creation, after setting all properties.
function slider1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to slider1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end




% --- Executes on button press in checkbox1.
function checkbox1_Callback(hObject, eventdata, handles)
set(handles.slider1, 'value', 0);
set(handles.text3, 'String',0);
axes(handles.axes3);
image = imread('segw1.jpg');
imgrot= imrotate(image,0);
imshow(imgrot);
axes(handles.axes1);
    assignin ('base','stand',1);
     assignin ('base','eingang',0);
    set_param('segway_complimentarry_filter','SimulationCommand','update');

% hObject    handle to checkbox1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkbox1


% --- Executes when figure1 is resized.
function figure1_ResizeFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in checkbox2.
function checkbox2_Callback(hObject, eventdata, handles)
% hObject    handle to checkbox2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkbox2


% --- Executes on button press in pushbutton3.
function pushbutton3_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
stand = 1;
set_param('segway_complimentarry_filter','SimulationCommand','start');


% --- Executes during object creation, after setting all properties.
function pushbutton3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to pushbutton3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

evalin('base','stand =1;');
evalin('base','eingang  =0;');
%set_param('segway_complimentarry_filter','SimulationCommand','update');


% --- Executes on button press in pushbutton4.
function pushbutton4_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set_param('segway_complimentarry_filter','SimulationCommand','stop');


% --- Executes during object creation, after setting all properties.
function axes3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
% Hint: place code in OpeningFcn to populate axes3



function curState_Callback(hObject, eventdata, handles)
% hObject    handle to curState (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of curState as text
%        str2double(get(hObject,'String')) returns contents of curState as a double


% --- Executes during object creation, after setting all properties.
function curState_CreateFcn(hObject, eventdata, handles)
% hObject    handle to curState (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in checkbox3.
function checkbox3_Callback(hObject, eventdata, handles)
if get(handles.checkbox3,'Value') == 1
assignin('base','anzeigePwm',1);
else
assignin('base','anzeigePwm',0); 
end


% hObject    handle to checkbox3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkbox3


% --- Executes on button press in checkbox4.
function checkbox4_Callback(hObject, eventdata, handles)
if get(handles.checkbox4,'Value') == 1
assignin('base','anzeigeDriveSum',1);
else
assignin('base','anzeigeDriveSum',0); 
end
% hObject    handle to checkbox4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkbox4



function currPWM_Callback(hObject, eventdata, handles)
% hObject    handle to currPWM (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of currPWM as text
%        str2double(get(hObject,'String')) returns contents of currPWM as a double


% --- Executes during object creation, after setting all properties.
function currPWM_CreateFcn(hObject, eventdata, handles)
% hObject    handle to currPWM (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes during object creation, after setting all properties.
function checkbox3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to checkbox3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called


% --------------------------------------------------------------------
function Untitled_1_Callback(hObject, eventdata, handles)
% hObject    handle to Untitled_1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes when user attempts to close figure1.
function figure1_CloseRequestFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: delete(hObject) closes the figure
set_param('segway_complimentarry_filter','SimulationCommand','stop');
delete(hObject);
