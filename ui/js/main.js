define( [ 'jquery', 'jqueryjsonrpc' ], function($) {

	function doAjaxRequest(m, parameters, target) {

		$.jsonRPC.request(m, {
			params : parameters,
			success : function(data) {
				$(target).html(data.result);
			},
			error : function(result) {
				alert("Ajax failed to fetch data");
			}
		});
	}

	$(document).ready(function() {

		$.jsonRPC.setup( {
			endPoint : '/',
			namespace : ''
		});

		$("#testform").submit(function() {
			doAjaxRequest('sayHello', {
				name : $("#thename").val()
			}, "#result");
			return false;
		});
	});

});
