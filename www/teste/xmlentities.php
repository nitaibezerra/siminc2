<?php

/**
 * Altera os caracteres especiais para o formato aceito em
 * html, xhtml, xml, xslt, etc.
 *
 * @param string $strText
 * @param string $strQuoteStype
 * @return string
 */
function xmlentities( $strText, $strQuoteStyle = ENT_QUOTES, $strCharSet = 'ISO' )
{

	if ( $strCharSet == 'UTF8' )
	{
		$strText = utf8_decode( $strText );
	}

	static $trans;
	if ( !isset( $trans ) )
	{
		$trans = get_html_translation_table( HTML_ENTITIES, $strQuoteStyle );
		foreach ( $trans as $key => $value )
		{
			$trans[ $key ] = '&#' . ord( $key ) . ';';
		}
		$trans[ chr(38) ] = '&';
	}
	return preg_replace( "/&(?![A-Za-z]{0,4}\w{2,3};|#[0-9]{2,5};)/" , "&#38;" , strtr( $strText, $trans ) );
}

?>
