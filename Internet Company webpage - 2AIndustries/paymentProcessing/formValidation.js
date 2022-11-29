/*
        Anushka Verma, Aby Shulgan, Vansh Rai, Addison Alvey-Blanco
        CSCI 467: Group 2A Final Project
        API call to credit card processing system
        12/3/2021
*/
$(document).ready(function()
{

	//run api call to credit card processing system
	$('#ccProcessing').submit(function()
	{

		//data for the credit card system
		var t = Date.now();
		var trans = '420-' + t + '-420';
		var data = {'cc': $('#cardNumber').val(), 'name': $('#name').val(), 'exp': $('#expiration').val(), 'amount': $('#total').val(), 'trans': trans, 'vendor': '2A Industries'};

		var response;
		$.ajax({
			type: 'POST',
			url: 'http://blitz.cs.niu.edu/CreditCard/',
			dataType: 'json',
			data: data,
			async: false
		}).done(function(data) {
			response = data;
		});

		if (response.errors)
		{
			alert(response.errors.join('\n'));
			return false;
		}
		else
		{
			console.log('response is: ', response);
			$('<input>').attr('type', 'hidden').attr('name', 'authorization').attr('value', response.authorization).appendTo('#ccProcessing');
			$('<input>').attr('type', 'hidden').attr('name', 'trans').attr('value', trans).appendTo('#ccProcessing');
			return true;
		}
	});
});
