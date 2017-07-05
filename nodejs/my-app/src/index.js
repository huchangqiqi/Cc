import React from 'react';
import ReactDOM from 'react-dom';

class ProductCategoryRow extends React.Component {
	render() {
		return <tr > < th colSpan = "2" > {
			this.props.category
		} < /th></tr > ;
	}
}

class ProductRow extends React.Component {
	render() {
		var name = this.props.product.stocked ?
			this.props.product.name :
			<
			span style = {
				{
					color: 'red'
				}
			} > {
				this.props.product.name
			} <
			/span>;
		return ( <
						 tr >
						 <
						 td > {
							 name
						 } < /td> <
						 td > {
							 this.props.product.price
						 } < /td> <
						 /tr>
					 );
	}
}

class ProductTable extends React.Component {
	render() {
		var rows = [];
		var lastCategory = null;
		this.props.products.forEach(function(product) {
			if (product.category !== lastCategory) {
				rows.push( < ProductCategoryRow category = {
					product.category
				}
									 key = {
										 product.category
									 }
									 />);
			}
			rows.push( < ProductRow product = {
				product
			}
								 key = {
									 product.name
								 }
								 />);
			lastCategory = product.category;
		});
		return ( <
						 table >
						 <
						 thead >
						 <
						 tr >
						 <
						 th > Name < /th> <
						 th > Price < /th> <
						 /tr> <
						 /thead> <
						 tbody > {
							 rows
						 } < /tbody> <
						 /table>
					 );
	}
}

class SearchBar extends React.Component {
	render() {
		return ( <
						 form >
						 <
						 input type = "text"
						 placeholder = "Search..." / >
						 <
						 p >
						 <
						 input type = "checkbox" / > {
							 ' '
						 }
						 Only show products in stock <
						 /p> <
						 /form>
					 );
	}
}

class FilterableProductTable extends React.Component {
	constructor(props) {
		super(props);
		this.state = {
			filterText: '',
			inStockOnly: false
		};
	}
	render() {
		return ( <
						 div >
						 <
						 SearchBar filterText = {
							 this.state.filterText
						 }
						 inStockOnly = {
							 this.state.inStockOnly
						 }
						 /> <
						 ProductTable products = {
							 this.props.products
						 }
						 filterText = {
							 this.state.filterText
						 }
						 inStockOnly = {
							 this.state.inStockOnly
						 }
						 /> <
						 /div>
					 );
	}
}


var PRODUCTS = [{
	category: 'Sporting Goods',
	price: '$49.99',
	stocked: true,
	name: 'Football'
},
								{
									category: 'Sporting Goods',
									price: '$9.99',
									stocked: true,
									name: 'Baseball'
								},
								{
									category: 'Sporting Goods',
									price: '$29.99',
									stocked: false,
									name: 'Basketball'
								},
								{
									category: 'Electronics',
									price: '$99.99',
									stocked: true,
									name: 'iPod Touch'
								},
								{
									category: 'Electronics',
									price: '$399.99',
									stocked: false,
									name: 'iPhone 5'
								},
								{
									category: 'Electronics',
									price: '$199.99',
									stocked: true,
									name: 'Nexus 7'
								}
							 ];

ReactDOM.render( <
								 FilterableProductTable products = {
									 PRODUCTS
								 }
								 />,
								 document.getElementById('root')
							 );
