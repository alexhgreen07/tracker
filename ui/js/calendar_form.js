define( [ 'moment', 'jquery', 'jqueryui', 'fullcalendar' ], function(moment,$) {
	
	function CalendarForm(api)
	{
		this.api = api;
		this.calendarDiv = null;
		this.calendar = null;
	}
	CalendarForm.prototype.convertServerEventToCalendarEvent = function(serverEvent)
	{
		return {
			title: 'Event',
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
			events: []
		});
		
		this.refresh(function(){});
	};
	
	return {
		CalendarForm: CalendarForm
	};
	
});