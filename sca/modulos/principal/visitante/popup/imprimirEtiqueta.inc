<?php
$file 		= $_REQUEST['file'];
$urlFile	= 'http://'.$_SERVER['HTTP_HOST'].'/sca/geraPDF.php?file='. $file;
?>
<html>
<head>

<script language='javascript' type='text/javascript' src='<?php echo 'http://'.$_SERVER['HTTP_HOST']; ?>/includes/JQuery/jquery-1.4.2.min.js'></script>
<script type="text/javascript">
/*
 * Função que valida a impressão pela Applet.
 * caso haja falha na applet, é  chamada para
 * fazer a validação. Caso true fecha a janela,
 * caso false , exibe um confirm, e ao clicar em OK 
 * é dado um reload na página para tentar imprimir 
 * novamente.  
 *
 * @author Uilson Rosa <uilson.lopes@squadra.com.br>
 * @param <bool> isPrinting
 * @since 2012-02-17
 * @return void
  */

   
 function validarImpressao( isPrinting, msg ){

	
	switch ( isPrinting ) {
		case false:
				alert( msg );
			if( confirm( "Deseja Imprimir Etiqueta novamente?" ) ){
				window.location.reload();
			}else{
				//alert( msg );
					closeWindow( false );
			}
		break;

		default:
				alert( msg );
				  	closeWindow( true );
		break;

		}
	}

	function closeWindow( tipo ){

		switch ( tipo ) {
		case false:
			return window.close()
		break;

		default:
			return setTimeout( 
					 function(){  
									window.close() 
								}, 2000 );
			
			break;
		}
	}
 
</script>
</head>
<body>
        <applet name="ImagePrint" code="br.com.squadra.util.ImagePrint.class" archive="componentes/ImpressaoEtiquetaSCA-1.1.jar, componentes/commons-logging-1.1.1.jar, componentes/fontbox-1.7.1.jar, componentes/pdfbox-1.7.1.jar">
	<param name="caminhoImagem" value="<?php echo $urlFile;?>" />
	<param name="larguraEtiqueta" value="200">
	<param name="alturaEtiqueta" value="140">
	<param name="removerArquivo" value="false">
        </applet>
</body>
</html>