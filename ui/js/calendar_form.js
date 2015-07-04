define( [ 'moment', 'jquery', 'jqueryui', 'fullcalendar' ], function(moment,$) {
	
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
	};
	CalendarForm.prototype.eventClick = function()
	{
		$(this.calendarDiv).hide();
		$(this.editTaskFormDiv).show();
	};
	CalendarForm.prototype.convertServerEventToCalendarEvent = function(serverEvent)
	{
		return {
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
				right: 'month,agendaWeek,agendaDay'
			},
			defaultView: 'agendaDay',
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
		CalendarForm: CalendarForm
	};
	
});