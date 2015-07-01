define( [ './api', './calendar_form', './task_forms', 'jquery', 'jqueryui' ], function(libApi,libCalendarForm,libTaskForms,$) {
	
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
		
		$(this.tabsDiv).tabs();
		
		this.calendarForm.render(this.tabs[0].tabDiv);
		this.addTaskForm.render(this.tabs[1].tabDiv);

		this.tabs[2].tabDiv.innerHTML = "Under construction..."
		
	};
	
	function buildApplication()
	{
		var api = new libApi.Api();
		var calendarForm = new libCalendarForm.CalendarForm(api);
		var addTaskForm = new libTaskForms.AddTaskForm(api);
		
		return new Application(api,calendarForm,addTaskForm);
	};

	function main()
	{
		if(!isTest)
		{
			$(document).ready(function() {
				var application = buildApplication();
				application.render(document.body);
			});
		}
	}
	
	main();
	
	return {
		Application: Application,
		buildApplication: buildApplication
	};
	
});
