getPrice = async (symbol) => {
  let response = await fetch ("/quote",
    {
      method: 'POST',
      body: new URLSearchParams ("symbol=" + symbol)
    }
  );
  let json = await response.json ();

  return (json["name"] == 404) ? "Invalid symbol" : "Current price of " + json["name"] + " is $" + json["price"].toFixed(2);
}


addCash = async (amt) => {
  let response = await fetch ("/addcash",
    {
      method: 'POST',
      body: new URLSearchParams ("amount=" + amt)
    }
  );
  let text = await response.text ();

  return text;
}


complexPassword = (str) => {
  return (
    str.match (/[a-z]/g) &&
    str.match (/[A-Z]/g) &&
    str.match (/[0-9]/g) &&
    str.match(/[^a-zA-Z\d]/g) &&
    str.length >= 8
  );
}