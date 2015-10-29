/**
Contains the calendar form and associated helper forms
@module calendar_form
@requires module:moment
@requires module:jquery
@requires module:jqueryui
@requires module:fullcalendar
*/
define( [ './task_forms', 
          './event_forms',
          'moment', 
          'jquery', 
          'jqueryui', 
          'fullcalendar' ], 
          function(
		  libTaskForms,
		  libEventForms,
		  moment,
		  $) {
	
	/**
	@class The form containing buttons for actions to add/edit tasks/events
	@constructor TaskActionForm
	@alias module:calendar_form
	@param {module:api~Api} api
	@param editTaskForm
	@param addEventForm
	@param editEventForm
	*/
	function TaskActionForm(api,editTaskForm,addEventForm,editEventForm)
	{
		this.api = api;
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
		
		this.startStopButton = null;
		this.completeButton = null;
		this.missedButton = null;
		
		this.currentEvent = null;
		this.currentTask = null;
	}
	
	/**
	@method setSelectEventAndTask
	@memberof module:calendar_form~TaskActionForm
	@instance
	@param currentEvent
	@param currentTask
	*/
	TaskActionForm.prototype.setSelectEventAndTask = function(currentEvent, currentTask)
	{
		this.currentEvent = currentEvent;
		this.currentTask = currentTask;
	};
	
	/**
	@method editTaskButtonClick
	@memberof module:calendar_form~TaskActionForm
	@instance
	*/
	TaskActionForm.prototype.editTaskButtonClick = function()
	{
		$(this.buttonsDiv).hide();
		$(this.editTaskFormDiv).show();
	};
	
	/**
	@method addEventButtonClick
	@memberof module:calendar_form~TaskActionForm
	@instance
	*/
	TaskActionForm.prototype.addEventButtonClick = function()
	{
		$(this.buttonsDiv).hide();
		$(this.addEventFormDiv).show();
	};
	
	/**
	@method editEventButtonClick
	@memberof module:calendar_form~TaskActionForm
	@instance
	*/
	TaskActionForm.prototype.editEventButtonClick = function()
	{
		$(this.buttonsDiv).hide();
		$(this.editEventFormDiv).show();
	};
	
	/**
	@method startStopButtonClick
	@memberof module:calendar_form~TaskActionForm
	@instance
	*/
	TaskActionForm.prototype.startStopButtonClick = function()
	{
		//TODO: find a more elegant way to do this
		if(this.startStopButton.value == "Start")
		{
			this.addEventForm.statusInput.setValue("Running");
			this.addEventForm.submitButtonClick();
		}
		else
		{
			this.editEventForm.statusInput.setValue("Logged");
			this.editEventForm.submitButtonClick();
		}
	};
	
	TaskActionForm.prototype.setTaskStatus = function(status)
	{
		if(this.currentTask.recurringCount == 0)
		{
			this.editTaskForm.statusInput.setValue(status);
			this.editTaskForm.submitClickEvent();
		}
		else
		{
			this.api.updateRecurringTaskStatus(
					this.currentTask.taskId,
					this.currentEvent.recurringIndex,
					status,
					function(){});
		}
	};
	
	/**
	@method completeButtonClick
	@memberof module:calendar_form~TaskActionForm
	@instance
	*/
	TaskActionForm.prototype.completeButtonClick = function()
	{
		var desiredValue = "Incomplete";
		
		if(this.completeButton.value == "Complete")
		{
			desiredValue = "Complete";
		}
		
		this.setTaskStatus(desiredValue);
	};
	
	/**
	@method missedButtonClick
	@memberof module:calendar_form~TaskActionForm
	@instance
	*/
	TaskActionForm.prototype.missedButtonClick = function()
	{
		this.setTaskStatus("Missed");
	};
	
	/**
	@method showOnlyButtons
	@memberof module:calendar_form~TaskActionForm
	@instance
	*/
	TaskActionForm.prototype.showOnlyButtons = function()
	{
		$(this.editTaskFormDiv).hide();
		$(this.addEventFormDiv).hide();
		$(this.editEventFormDiv).hide();
		$(this.buttonsDiv).show();
	};
	
	/**
	@method render
	@memberof module:calendar_form~TaskActionForm
	@instance
	@param {HTMLElement} parent
	*/
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
		
		this.buttonsDiv.appendChild(document.createElement("br"));
		this.buttonsDiv.appendChild(document.createElement("br"));
		
		this.startStopButton = this.buttonsDiv.appendChild(document.createElement("input"));
		this.startStopButton.type = "submit";
		this.startStopButton.value = "Start";
		
		this.completeButton = this.buttonsDiv.appendChild(document.createElement("input"));
		this.completeButton.type = "submit";
		this.completeButton.value = "Complete";
		
		this.missedButton = this.buttonsDiv.appendChild(document.createElement("input"));
		this.missedButton.type = "submit";
		this.missedButton.value = "Missed";
		
		this.showOnlyButtons();
		
		$(this.editTaskButton).button();
		$(this.editTaskButton).click(this.editTaskButtonClick.bind(this));
		
		$(this.addEventButton).button();
		$(this.addEventButton).click(this.addEventButtonClick.bind(this));
		
		$(this.editEventButton).button();
		$(this.editEventButton).click(this.editEventButtonClick.bind(this));
		
		$(this.startStopButton).button();
		$(this.startStopButton).click(this.startStopButtonClick.bind(this));
		
		$(this.completeButton).button();
		$(this.completeButton).click(this.completeButtonClick.bind(this));
		
		$(this.missedButton).button();
		$(this.missedButton).click(this.missedButtonClick.bind(this));
	};
	
	/**
	@class The form containing the calendar of events
	@constructor CalendarForm
	@alias module:calendar_form
	@param {module:api~Api} api
	@param {module:calendar_form~TaskActionForm} taskActionForm
	*/
	function CalendarForm(api,taskActionForm)
	{
		this.api = api;
		this.taskActionForm = taskActionForm;
		
		this.refreshInterval = 60000;
		this.timedRefreshIntervalId = -1;
		
		this.calendarDiv = null;
		this.calendar = null;
		this.taskActionFormDiv = null;
		
		this.backButton = null;
	}
	
	/**
	@method backButtonClick
	@memberof module:calendar_form~CalendarForm
	@instance
	*/
	CalendarForm.prototype.backButtonClick = function()
	{
		this.taskActionForm.showOnlyButtons();
		
		$(this.taskActionFormDiv).hide();
		$(this.calendarDiv).show();
		
		this.refresh(function(){});
		
	};
	
	/**
	@method eventClick
	@memberof module:calendar_form~CalendarForm
	@instance
	@param calEvent
	@param jsEvent
	@param view
	*/
	CalendarForm.prototype.eventClick = function(calEvent, jsEvent, view)
	{
		//TODO: find a more elegant way to do this
		if(calEvent.serverEvent.status == "Running")
		{
			this.taskActionForm.startStopButton.value = "Stop";
		}
		else
		{
			this.taskActionForm.startStopButton.value = "Start";
		}
		
		if(calEvent.serverEvent.taskStatus == "Incomplete")
		{
			this.taskActionForm.completeButton.value = "Complete";
		}
		else
		{
			this.taskActionForm.completeButton.value = "Incomplete";
		}
		
		$(this.calendarDiv).hide();
		$(this.taskActionFormDiv).show();
		
		var parentTask = this.api.taskLookup[calEvent.serverEvent.taskId];
		
		this.taskActionForm.setSelectEventAndTask(calEvent.serverEvent,parentTask);
		
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
	
	/**
	@method convertServerEventToCalendarEvent
	@memberof module:calendar_form~CalendarForm
	@instance
	@param serverEvent
	*/
	CalendarForm.prototype.convertServerEventToCalendarEvent = function(serverEvent)
	{
		var parentTask = this.api.taskLookup[serverEvent.taskId];
		
		var eventColour = "blue";
		
		if(serverEvent.taskStatus == "Complete")
		{
			eventColour = "green";
		}
		else if(serverEvent.taskStatus == "Missed")
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
	
	/**
	@method refresh
	@memberof module:calendar_form~CalendarForm
	@instance
	@param success
	@param error
	*/
	CalendarForm.prototype.refresh = function(success,error)
	{
		this.api.getAppData((function(result){
			
			var newEvents = [];
			
			for(var key in result.events)
			{
				var calendarEvent = this.convertServerEventToCalendarEvent(result.events[key]);
				newEvents.push(calendarEvent);
			}
			
			this.calendar.fullCalendar( 'removeEvents' );
			this.calendar.fullCalendar( 'addEventSource', newEvents);
			
			success();
			
		}).bind(this),error);
	};
	
	/**
	@method timedRefresh
	@memberof module:calendar_form~CalendarForm
	@instance
	*/
	CalendarForm.prototype.timedRefresh = function()
	{
		//TODO: add proper error callback
		this.refresh(function(){},function(){});
	};
	
	/**
	@method stopTimedRefresh
	@memberof module:calendar_form~CalendarForm
	@instance
	*/
	CalendarForm.prototype.stopTimedRefresh = function()
	{
		clearInterval(this.timedRefreshIntervalId);
		this.timedRefreshIntervalId = -1;
	};
	
	/**
	@method render
	@memberof module:calendar_form~CalendarForm
	@instance
	@param {HTMLElement} parent
	*/
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
		
		this.timedRefreshIntervalId = setInterval(this.timedRefresh.bind(this), this.refreshInterval);
	};
	
	/**
	Builds the Calendar Form object with its associated dependencies
	@function buildCalendarForm
	@param {module:api~Api} api
	@return {module:calendar_form~CalendarForm}
	*/
	function buildCalendarForm(api)
	{
		var editTaskForm = new libTaskForms.UpdateTaskForm(api);
		var addEventForm = new libEventForms.AddEventForm(api);
		var editEventForm = new libEventForms.EditEventForm(api);
		var taskActionsForm = new TaskActionForm(api,editTaskForm,addEventForm,editEventForm);
		var calendarForm = new CalendarForm(api,taskActionsForm);
		
		return calendarForm;
	}
	
	return {
		TaskActionForm: TaskActionForm,
		CalendarForm: CalendarForm,
		buildCalendarForm: buildCalendarForm
	};
	
});