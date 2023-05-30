const lowercaseChars = "abcdefghijklmnopqrstuvwxyz";
const uppercaseChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const numberChars = "0123456789";
const symbolChars = "!@#$%^&*()_+-=[]{};':\"\\|,.<>/?";

function generatePassword(length, chars) {
	let password = "";
	for (let i = 0; i < length; i++) {
		password += chars.charAt(Math.floor(Math.random() * chars.length));
	}
	return password;
}

function generatePasswords() {
	const includeLowercase = document.getElementById("lowercase").checked;
	const includeUppercase = document.getElementById("uppercase").checked;
	const includeNumbers = document.getElementById("numbers").checked;
	const includeSymbols = document.getElementById("symbols").checked;
	const excludeChars = document.getElementById("exclude").checked ? document.getElementById("exclude-chars").value :
		"";
	const length = parseInt(document.getElementById("code-length").value);
	const number = parseInt(document.getElementById("code-number").value);

	let usedChars = "";
	if (includeLowercase) usedChars += lowercaseChars;
	if (includeUppercase) usedChars += uppercaseChars;
	if (includeNumbers) usedChars += numberChars;
	if (includeSymbols) usedChars += document.getElementById("symbols-text").value;
	usedChars = usedChars.replace(new RegExp(`[${excludeChars}]`, "g"), "");

	let passwords = "";
	for (let i = 0; i < number; i++) {
		passwords += generatePassword(length, usedChars) + "\n";
	}
	document.getElementById("result").value = passwords;
}

document.getElementById("generate-passwords-btn").addEventListener("click", generatePasswords);