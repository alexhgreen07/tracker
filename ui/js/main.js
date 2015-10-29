/**
A module representing the main application objects.
@module main
@requires module:api
@requires module:calendar_form
@requires module:task_forms
@requires module:event_forms
@requires module:jquery
@requires module:jqueryui
*/
define( [ './api', 
          './calendar_form', 
          './task_forms',
          'jquery', 
          'jqueryui' ], 
          function(
		  libApi,
		  libCalendarForm,
		  libTaskForms,
		  $) {
	
	/**
	@class The main application object
	@constructor Application
	@alias module:main
	@param {Api} api - The JSON-RPC API object
	@param {CalendarForm} calendarForm - The calendar form
	@param {AddTaskForm} addTaskForm - The add task form
	*/
	function Application(api,calendarForm,addTaskForm)
	{
		this.api = api;
		this.calendarForm = calendarForm;
		this.addTaskForm = addTaskForm;
		
		this.tabsId = "tabs";
		this.tabs = [
             {
            	 title: "Calendar",
            	 tabDiv: null
             },
             {
            	 title: "Add",
            	 tabDiv: null
             },
             {
            	 title: "Settings",
            	 tabDiv: null
             }
		];
		
		this.tabsDiv = null;
	}
	
	/**
	Renders the main application.
	@method render
	@memberof module:main~Application
	@instance
	*/
	Application.prototype.render = function(parent)
	{
		var element = null;
		
		this.tabsDiv = parent.appendChild(document.createElement("div"));
		this.tabsDiv.id = this.tabsId;
		
		var listLinks = this.tabsDiv.appendChild(document.createElement("ul"));
		
		for(var i = 0; i < this.tabs.length; i++)
		{
			var li = listLinks.appendChild(document.createElement("li"));
			var a = li.appendChild(document.createElement("a"));
			a.innerHTML = this.tabs[i].title;
			a.href = "#" + this.tabsId + "-" + i;
		}
		for(var i = 0; i < this.tabs.length; i++)
		{
			this.tabs[i].tabDiv = this.tabsDiv.appendChild(document.createElement("div"));
			this.tabs[i].tabDiv.id = this.tabsId + "-" + i;
		}
		
		$(this.tabsDiv).tabs({
		    activate: (function(event, ui) {
		    	if(ui.newPanel[0] == this.tabs[0].tabDiv)
	    		{
		    		this.calendarForm.refresh(function(){});
	    		}
		    }).bind(this)
		});
		
		this.calendarForm.render(this.tabs[0].tabDiv);
		this.addTaskForm.render(this.tabs[1].tabDiv);

		this.tabs[2].tabDiv.innerHTML = "Under construction..."
		
	};
	
	/**
	Builds the main application object with all dependencies.
	@function buildApplication
	@return {module:main~Application}
	*/
	function buildApplication()
	{
		var api = new libApi.Api();
		var addTaskForm = new libTaskForms.AddTaskForm(api);
		var calendarForm = new libCalendarForm.buildCalendarForm(api);
		
		return new Application(api,calendarForm,addTaskForm);
	};
	
	/**
	Builds and runs the main application.
	@function main
	*/
	function main()
	{
		if(!isTest)
		{
			$(document).ready(function() {
				var application = buildApplication();
				application.api.getAppData(function(){
					application.render(document.body);
				});
			});
		}
	}
	
	main();

	return {
		Application: Application,
		buildApplication: buildApplication
	};
	
});
