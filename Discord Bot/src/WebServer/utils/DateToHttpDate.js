

function DateToHttpDate(date){
	var httpDate = "";
	switch(date.getUTCDay()){
		case 0:
			httpDate += "Mond";
			break;
		case 1:
			httpDate += "Tue";
			break;
		case 2:
			httpDate += "Wed";
			break;
		case 3:
			httpDate += "Thu";
			break;
		case 4:
			httpDate += "Fri";
			break;
		case 5:
			httpDate += "Sat";
			break;
		case 6:
			httpDate += "Sun";
			break;
	}
	httpDate += ", " + date.getUTCDate() + " ";
	switch(date.getUTCMonth()){
		case 0:
			httpDate += "Jan";
			break;
		case 1:
			httpDate += "Feb";
			break;
		case 2:
			httpDate += "Mar";
			break;
		case 3:
			httpDate += "Apr";
			break;
		case 4:
			httpDate += "May";
			break;
		case 5:
			httpDate += "Jun";
			break;
		case 6:
			httpDate += "Jul";
			break;
		case 7:
			httpDate += "Aug";
			break;
		case 8:
			httpDate += "Sep";
			break;
		case 9:
			httpDate += "Oct";
			break;
		case 10:
			httpDate += "Nov";
			break;
		case 11:
			httpDate += "Dev";
			break;
	}
	httpDate += " " + date.getUTCFullYear();
	httpDate += " " + date.getUTCHours() + ":" + date.getUTCMinutes() + ":" + date.getUTCSeconds() + " GMT";
	return httpDate;
}

module.exports = DateToHttpDate;