define( [ './api', 'jquery', 'jqueryui' ], function(libapi,$) {
	
	function Application(api)
	{
		this.api = api;
		
		this.tabsDiv = null;
		this.testForm = null;
		this.nameInput = null;
		this.resultsDiv = null;
	}
	Application.prototype.render = function(parent)
	{
		var element = null;
		
		this.tabsDiv = parent.appendChild(document.createElement("div"));
		this.tabsDiv.id = "tabs";
		
		var listLinks = this.tabsDiv.appendChild(document.createElement("ul"));
		listLinks.innerHTML = '<li><a href="#tabs-1">Summary</a></li>';
		listLinks.innerHTML += '<li><a href="#tabs-2">Settings</a></li>';
		
		var tabDiv = this.tabsDiv.appendChild(document.createElement("div"));
		tabDiv.id = "tabs-1";
		this.testForm = tabDiv.appendChild(document.createElement("form"));
		this.testForm.id = "testform";
		var label = this.testForm.appendChild(document.createElement("label"));
		label.innerHTML = "Enter your name:";
		this.nameInput = this.testForm.appendChild(document.createElement("input"));
		this.nameInput.type = "text";
		this.nameInput.id = "thename";
		
		this.resultsDiv = tabDiv.appendChild(document.createElement("div"));
		this.resultsDiv.innerHTML = "Result will be displayed here...";
		
		tabDiv = this.tabsDiv.appendChild(document.createElement("div"));
		tabDiv.id = "tabs-2";
		tabDiv.innerHTML = "Under construction..."
		
		$(this.tabsDiv).tabs();
		$(this.testForm).submit(this.formSubmitEvent.bind(this));
	};
	Application.prototype.formSubmitEvent = function(event)
	{
		event.preventDefault();
		
		this.api.sayHello($(this.nameInput).val(),function(result){
			$(this.resultsDiv).html(result);
		}.bind(this));
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
