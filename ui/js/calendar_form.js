define( [ 'moment', 'jquery', 'jqueryui', 'fullcalendar' ], function(moment,$) {
	
	function TaskActionForm(editTaskForm,addEventForm,editEventForm)
	{
		this.editTaskForm = editTaskForm;
		this.addEventForm = addEventForm;
		this.editEventForm = editEventForm;
		
		this.editTaskFormDiv = null;
		this.addEventFormDiv = null;
		this.editEventFormDiv = null;
		
		this.buttonsDiv = null;
		this.editTaskButton = null;
		this.addEventButton = null;
		this.editEventButton = null;
	}
	TaskActionForm.prototype.editTaskButtonClick = function()
	{
		$(this.buttonsDiv).hide();
		$(this.editTaskFormDiv).show();
	};
	TaskActionForm.prototype.addEventButtonClick = function()
	{
		$(this.buttonsDiv).hide();
		$(this.addEventFormDiv).show();
	};
	TaskActionForm.prototype.editEventButtonClick = function()
	{
		$(this.buttonsDiv).hide();
		$(this.editEventFormDiv).show();
	};
	TaskActionForm.prototype.showOnlyButtons = function()
	{
		$(this.editTaskFormDiv).hide();
		$(this.addEventFormDiv).hide();
		$(this.editEventFormDiv).hide();
		$(this.buttonsDiv).show();
	};
	TaskActionForm.prototype.render = function(parent)
	{
		var div = parent.appendChild(document.createElement("div"));
		
		this.editTaskFormDiv = div.appendChild(document.createElement("div"));
		this.editTaskForm.render(this.editTaskFormDiv);
		this.addEventFormDiv = div.appendChild(document.createElement("div"));
		this.addEventForm.render(this.addEventFormDiv);
		this.editEventFormDiv = div.appendChild(document.createElement("div"));
		this.editEventForm.render(this.editEventFormDiv);
		
		this.buttonsDiv = div.appendChild(document.createElement("div"));
		
		this.editTaskButton = this.buttonsDiv.appendChild(document.createElement("input"));
		this.editTaskButton.type = "submit";
		this.editTaskButton.value = "Edit Task";
		
		this.addEventButton = this.buttonsDiv.appendChild(document.createElement("input"));
		this.addEventButton.type = "submit";
		this.addEventButton.value = "Add Event";
		
		this.editEventButton = this.buttonsDiv.appendChild(document.createElement("input"));
		this.editEventButton.type = "submit";
		this.editEventButton.value = "Edit Event";
		
		this.showOnlyButtons();
		
		$(this.editTaskButton).button();
		$(this.editTaskButton).click(this.editTaskButtonClick.bind(this));
		
		$(this.addEventButton).button();
		$(this.addEventButton).click(this.addEventButtonClick.bind(this));
		
		$(this.editEventButton).button();
		$(this.editEventButton).click(this.editEventButtonClick.bind(this));
	};
	
	function CalendarForm(api,taskActionForm)
	{
		this.api = api;
		this.taskActionForm = taskActionForm;
		
		this.calendarDiv = null;
		this.calendar = null;
		this.taskActionFormDiv = null;
		
		this.backButton = null;
	}
	CalendarForm.prototype.backButtonClick = function()
	{
		this.taskActionForm.showOnlyButtons();
		
		$(this.taskActionFormDiv).hide();
		$(this.calendarDiv).show();
		
		this.refresh(function(){});
		
	};
	CalendarForm.prototype.eventClick = function(calEvent, jsEvent, view)
	{
		$(this.calendarDiv).hide();
		$(this.taskActionFormDiv).show();
		
		var parentTask = this.api.taskLookup[calEvent.serverEvent.taskId];
		
		this.taskActionForm.editTaskForm.setTaskData(
				parentTask.taskId,
				parentTask.name,
				parentTask.earliestStartTime,
				parentTask.latestEndTime,
				parentTask.duration,
				parentTask.recurringPeriod,
				parentTask.recurringLateOffset);
		
		this.taskActionForm.addEventForm.setEventData(
				parentTask.taskId,
				calEvent.serverEvent.recurringIndex);
		
		this.taskActionForm.editEventForm.setEventData(
				calEvent.serverEvent.eventId,
				calEvent.serverEvent.taskId,
				calEvent.serverEvent.startTime,
				calEvent.serverEvent.duration,
				calEvent.serverEvent.status,
				calEvent.serverEvent.recurringIndex);
	};
	CalendarForm.prototype.convertServerEventToCalendarEvent = function(serverEvent)
	{
		var parentTask = this.api.taskLookup[serverEvent.taskId];
		
		var eventColour = "blue";
		
		if(parentTask.status == "Complete")
		{
			eventColour = "green";
		}
		else if(parentTask.status == "Missed")
		{
			eventColour = "crimson";
		}
		else if(serverEvent.status == "Scheduled")
		{
			eventColour = "cornflowerblue";
		}
		else if(serverEvent.status == "Running")
		{
			eventColour = "indianred";
		}
		
		var calEvent = {
			serverEvent: serverEvent,
			title: parentTask.name,
			start: new Date(serverEvent.startTime * 1000),
			end: new Date((serverEvent.startTime + serverEvent.duration) * 1000),
			color: eventColour
		};
		
		return calEvent;
	};
	CalendarForm.prototype.refresh = function(success,error)
	{
		this.api.getEvents((function(result){
			
			var newEvents = [];
			
			for(var key in result)
			{
				var calendarEvent = this.convertServerEventToCalendarEvent(result[key]);
				newEvents.push(calendarEvent);
			}
			
			this.calendar.fullCalendar( 'removeEvents' );
			this.calendar.fullCalendar( 'addEventSource', newEvents);
			
			success();
			
		}).bind(this),error);
	};
	CalendarForm.prototype.render = function(parent)
	{
		this.calendarDiv = parent.appendChild(document.createElement("div"));
		
		this.calendar = $(this.calendarDiv).fullCalendar({
			header: {
				left: 'prev,next today',
				center: 'title',
				right: 'month,basicWeek,basicDay'
			},
			defaultView: 'basicDay',
			defaultDate: (new Date()),
			height: 850,
			editable: false,
			eventLimit: true,
			eventClick: this.eventClick.bind(this),
			events: []
		});
		
		this.taskActionFormDiv = parent.appendChild(document.createElement("div"));
		$(this.taskActionFormDiv).hide();
		
		this.taskActionForm.render(this.taskActionFormDiv);
		
		this.taskActionFormDiv.appendChild(document.createElement("br"));
		this.taskActionFormDiv.appendChild(document.createElement("br"));
		
		this.backButton = this.taskActionFormDiv.appendChild(document.createElement("input"));
		this.backButton.type = "submit";
		this.backButton.value = "Back";
		
		$(this.backButton).button();
		$(this.backButton).click(this.backButtonClick.bind(this));
		
		this.refresh(function(){});
	};
	
	return {
		TaskActionForm: TaskActionForm,
		CalendarForm: CalendarForm
	};
	
});