function varargout = updategui(varargin)

%create a run time object that can return the value of the gain block's
%output and then put the value in a string.
rto = get_param('segway_complimentarry_filter/Out1','RuntimeObject');
ausgangInt = rto.InputPort(1).Data;
str = num2str(ausgangInt);
testArr = evalin('base','ausgabe');
testArr(1)=[];
testArr(5000)=[ausgangInt];
assignin('base','ausgabe',testArr);
testArr2 = evalin('base','ausgabePwm');
testArr2(1)=[];

if ausgangInt >204
testArr2(5000)=[204];
assignin('base','ausgabePwm',testArr2);
elseif ausgangInt <-204
testArr2(5000)=[-204];
assignin('base','ausgabePwm',testArr2);
else
testArr2(5000)=[ausgangInt];
assignin('base','ausgabePwm',testArr2);    
end

%get a handle to the GUI's 'current state' window
t = [0:1:4999];
%axes(handles.axes1);
%plot(t,testArr);
statestxt = findobj('Tag','curState');
set(statestxt,'string',str);

statestxt = findobj('Tag','currPWM');
if ausgangInt >204
set(statestxt,'string','204');
elseif ausgangInt <-204
set(statestxt,'string',-204);
else
set(statestxt,'string',str);   
end

%axisHandles=findobj('Tag','axes1');
%axes(axisHandles);
if evalin('base','anzeigePwm') == 1 
    if evalin('base','anzeigeDriveSum') == 1
    plot(t,testArr2,'g',t,testArr,'r');
    elseif evalin('base','anzeigeDriveSum') == 0
    plot(t,testArr2,'g');
    end
end
if evalin('base','anzeigePwm') == 0 
    if evalin('base','anzeigeDriveSum') == 1
    plot(t,testArr,'r');
    elseif evalin('base','anzeigeDriveSum') == 0  
    end
end







%get a handle to the GUI's 'current state' window
%set(handles.curState,'String',str);


%update the gui


