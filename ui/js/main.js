define( [ './api', './calendar_form', 'jquery', 'jqueryui' ], function(libApi,libCalendarForm,$) {
	
	function Application(api,calendarForm)
	{
		this.api = api;
		this.calendarForm = calendarForm;
		
		this.tabsId = "tabs";
		this.tabs = [
             {
            	 title: "Calendar",
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
		
		this.tabs[1].tabDiv.innerHTML = "Under construction..."
		
		
		
		this.calendarForm.render(this.tabs[0].tabDiv);
	};
	
	function buildApplication()
	{
		var api = new libApi.Api();
		var calendarForm = new libCalendarForm.CalendarForm();
		
		return new Application(api,calendarForm);
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
