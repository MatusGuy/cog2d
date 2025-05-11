const path = require('path');

module.exports = {
	entry: './src/index.ts', // Your entry point
	output: {
		filename: 'index.js', // Output file name
		path: path.resolve(__dirname, 'build'), // Output directory
		clean: true, // Clean the output directory before emit
	},
	resolve: {
		extensions: ['.ts', '.js'], // Resolve these extensions
	},
	module: {
		rules: [
			{
				test: /\.ts$/, // Apply this rule to .ts files
				use: 'ts-loader', // Use ts-loader to transpile TypeScript
				exclude: /node_modules/, // Exclude node_modules
			},
		],
	},
	optimization: {
		minimize: false, // Disable minimization
	},
	devtool: false,
	mode: 'development', // Set to 'development' for development mode
};
