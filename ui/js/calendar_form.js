define( [ 'moment', 'jquery', 'jqueryui', 'fullcalendar' ], function(moment,$) {
	
	function TaskActionForm(editTaskForm,addEventForm,editEventForm)
	{
		
		
		this.editTaskButton = null;
		this.addEventButton = null;
		this.editEventButton = null;
	}
	
	function CalendarForm(api,editTaskForm)
	{
		this.api = api;
		this.editTaskForm = editTaskForm;
		
		this.calendarDiv = null;
		this.calendar = null;
		this.editTaskFormDiv = null;
		
		this.backButton = null;
	}
	CalendarForm.prototype.backButtonClick = function()
	{
		$(this.editTaskFormDiv).hide();
		$(this.calendarDiv).show();
		
		this.refresh(function(){});
		
	};
	CalendarForm.prototype.eventClick = function(calEvent, jsEvent, view)
	{
		$(this.calendarDiv).hide();
		$(this.editTaskFormDiv).show();
		
		var parentTask = this.api.taskLookup[calEvent.serverEvent.taskId];
		
		this.editTaskForm.setTaskData(
				parentTask.taskId,
				parentTask.name,
				parentTask.earliestStartTime,
				parentTask.latestEndTime,
				parentTask.duration,
				parentTask.recurringPeriod,
				parentTask.recurringLateOffset);
	};
	CalendarForm.prototype.convertServerEventToCalendarEvent = function(serverEvent)
	{
		return {
			serverEvent: serverEvent,
			title: serverEvent.name,
			start: new Date(serverEvent.startTime * 1000),
			end: new Date((serverEvent.startTime + serverEvent.duration) * 1000)
		};
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
		
		this.editTaskFormDiv = parent.appendChild(document.createElement("div"));
		$(this.editTaskFormDiv).hide();
		
		this.editTaskForm.render(this.editTaskFormDiv);
		
		this.editTaskFormDiv.appendChild(document.createElement("br"));
		this.editTaskFormDiv.appendChild(document.createElement("br"));
		
		this.backButton = this.editTaskFormDiv.appendChild(document.createElement("input"));
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