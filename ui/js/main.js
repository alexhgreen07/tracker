define( [ './api', 'moment', 'jquery', 'jqueryui', 'fullcalendar' ], function(libapi,moment,$) {
	
	function Application(api)
	{
		this.api = api;
		
		this.tabsDiv = null;
		this.calendarDiv = null;
		
		this.dummyEvents = [ {
			title : 'All Day Event',
			start : '2015-02-01'
		}, {
			title : 'Long Event',
			start : '2015-02-07',
			end : '2015-02-10'
		}, {
			id : 999,
			title : 'Repeating Event',
			start : '2015-02-09T16:00:00'
		}, {
			id : 999,
			title : 'Repeating Event',
			start : '2015-02-16T16:00:00'
		}, {
			title : 'Conference',
			start : '2015-02-11',
			end : '2015-02-13'
		}, {
			title : 'Meeting',
			start : '2015-02-12T10:30:00',
			end : '2015-02-12T12:30:00'
		}, {
			title : 'Lunch',
			start : '2015-02-12T12:00:00'
		}, {
			title : 'Meeting',
			start : '2015-02-12T14:30:00'
		}, {
			title : 'Happy Hour',
			start : '2015-02-12T17:30:00'
		}, {
			title : 'Dinner',
			start : '2015-02-12T20:00:00'
		}, {
			title : 'Birthday Party',
			start : '2015-02-13T07:00:00'
		}, {
			title : 'Click for Google',
			url : 'http://google.com/',
			start : '2015-02-28'
		} ];
	}
	Application.prototype.render = function(parent)
	{
		var element = null;
		
		this.tabsDiv = parent.appendChild(document.createElement("div"));
		this.tabsDiv.id = "tabs";
		
		var listLinks = this.tabsDiv.appendChild(document.createElement("ul"));
		listLinks.innerHTML = '<li><a href="#tabs-1">Calendar</a></li>';
		listLinks.innerHTML += '<li><a href="#tabs-2">Settings</a></li>';
		
		var tabDiv = this.tabsDiv.appendChild(document.createElement("div"));
		tabDiv.id = "tabs-1";
		this.calendarDiv = tabDiv.appendChild(document.createElement("div"));
		
		tabDiv = this.tabsDiv.appendChild(document.createElement("div"));
		tabDiv.id = "tabs-2";
		tabDiv.innerHTML = "Under construction..."
		
		$(this.tabsDiv).tabs();

		$(this.calendarDiv).fullCalendar({
			header: {
				left: 'prev,next today',
				center: 'title',
				right: 'month,agendaWeek,agendaDay'
			},
			defaultDate: '2015-02-12',
			editable: true,
			eventLimit: true,
			events: this.dummyEvents
		});
	};

	function main()
	{
		if(!isTest)
		{
			$(document).ready(function() {
				
				var api = new libapi.Api();
				var application = new Application(api);
				
				application.render(document.body);
			});
		}
	}
	
	main();
	
	return {
		Application: Application
	};
	
});
