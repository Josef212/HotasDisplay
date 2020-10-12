const XMLHttpRequest = require("xmlhttprequest").XMLHttpRequest;
const fs = require("fs");


function receiveCommodity(responseText, commodityName, commodityId) {
	let buyers = JSON.parse(responseText);
	//console.log(responseText);
	buyers.sort((a, b) => a.price > b.price);

	for (const buyer of buyers) {
		if (buyer.pad == 'M')
			buyers.pop();

		let bestBuyerStr = 'Sys: ' + buyers[0].system + '\n';
		bestBuyerStr += 'St: ' + buyers[0].station + '\n';
		bestBuyerStr += 'Cr: ' + buyers[0].price + '\n';

		fs.writeFile("prices.txt", bestBuyerStr, err => {
			if (err) {
				console.log(err);
			}
		});
	}
}

function requestCommodity(commodityId, commodityName) {
	let url = 'https://edtools.cc/trd.php?f=json&cid=' + commodityId;
	let req = new XMLHttpRequest();

	req.onreadystatechange = function () {
		if (req.readyState == 4 && req.status == 200) {
			receiveCommodity(req.responseText, commodityName, commodityId);
		}
	};

	req.open("GET", url, true);
	req.send();
}

requestCommodity(83, "Painite")
