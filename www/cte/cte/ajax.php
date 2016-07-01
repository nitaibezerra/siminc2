<?php

ob_start();

/**
 * Centraliza as requisiчѕes ajax do mѓdulo.  
 *
 * @author Renъ de Lima Barbosa <renebarbosa@mec.gov.br> 
 * @since 01/11/2007
 */

function erro( $codigo, $mensagem, $arquivo, $linha ){
	echo "Ocorreu um erro. Por favor tente mais tarde.";
	exit();
}

function excecao( Exception $excecao ){
	echo "Ocorreu um erro. Por favor tente mais tarde.";
	exit();
}

// captura controladamente eventuais erros
set_error_handler( 'erro', E_USER_ERROR );
set_exception_handler( 'excecao' );

// indica ao navegador o tipo de saэda
header( 'Content-type: text/plain' );
header( 'Cache-Control: no-store, no-cache' );

// carrega as funчѕes gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// atualiza aчуo do usuсrio no sistema
include APPRAIZ . "includes/registraracesso.php";

// carrega as funчѕes do mѓdulo
include '_constantes.php';
include '_funcoes.php';
include '_componentes.php';

// abre conexуo com o servidor de banco de dados
$db = new cls_banco();

function fechaDb()
{
    global $db;
    $db->close();
}

register_shutdown_function('fechaDb');

switch ( $_REQUEST['evento'] )
{

	case "remover_subacao":
		
		// captura dados
		$sbaid = (integer) $_REQUEST['sbaid'];
		
		// monta requisiчуo
		$sql = "
			delete from cte.subacaoindicador
			where
				 sbaid = " . $sbaid . "
		";
		
		// realiza aчуo e retorna resultado da aчуo
		ob_end_clean();
		if ( $db->executar( $sql ) )
		{
			echo "sucesso";
			$db->commit();
		}
		else
		{
			echo"falha";
			$db->rollback();
		}
		exit();

	case "alterar_status_subacao":
		
		// captura dados
		$sbaid = (integer) $_REQUEST['sbaid'];
		$psuid = (integer) $_REQUEST['psuid'];
		$sbaparecer = trim( addslashes( urldecode ($_REQUEST['sbaparecer']) ));
				
		$ssuid = (integer) $_REQUEST['ssuid'];
		
		$psuid = $psuid ? $psuid : " null ";
		$sbaparecer = $sbaparecer ? "'" . $sbaparecer . "'" : " null ";
		$ssuid = $ssuid ? $ssuid : " null ";
		
		$dadosExtra = "";
		if ( isset( $_REQUEST['sbastgmpl'] ) )
		{
			$sbaunt = str_replace( ".", "", $_REQUEST['sbaunt'] );
			$sbaunt = (float) str_replace( ",", ".", $sbaunt );
			
			$undid = (integer) $_REQUEST['undid'];
			$undid = $undid ? " '" . $undid . "' " : " null ";
			
			$frmid = (integer) $_REQUEST['frmid'];
			$frmid = $frmid ? " '" . $frmid . "' " : " null ";
			
			$foaid = (integer) $_REQUEST['foaid'];
			$foaid = $foaid ? " '" . $foaid . "' " : " null ";

			$prgid = (integer) $_REQUEST['prgid'];
			$prgid = $prgid ? " '" . $prgid . "' " : " null ";
			/*
			$dadosExtra = "
				 sbastgmpl = '" . $_REQUEST['sbastgmpl'] . "',
				 sbaprm    = '" . $_REQUEST['sbaprm'] . "',
				 prgid     = " . $prgid  . ",
				 undid     = " . $undid . ",
				 frmid     = " . $frmid . ",
				 foaid     = " . $foaid . ",
				 sbapcr    = '" . $_REQUEST['sbapcr'] . "',
				 sba0ano   = '" . ( (integer) $_REQUEST['sba0ano'] ) . "',
				 sba1ano   = '" . ( (integer) $_REQUEST['sba1ano'] ) . "',
				 sba2ano   = '" . ( (integer) $_REQUEST['sba2ano'] ) . "',
				 sba3ano   = '" . ( (integer) $_REQUEST['sba3ano'] ) . "',
				 sba4ano   = '" . ( (integer) $_REQUEST['sba4ano'] ) . "',
				 sba0ini   = '" . $_REQUEST['sba0ini'] . "',
				 sba1ini   = '" . $_REQUEST['sba1ini'] . "',
				 sba2ini   = '" . $_REQUEST['sba2ini'] . "',
				 sba3ini   = '" . $_REQUEST['sba3ini'] . "',
				 sba4ini   = '" . $_REQUEST['sba4ini'] . "',
				 sba0fim   = '" . $_REQUEST['sba0fim'] . "',
				 sba1fim   = '" . $_REQUEST['sba1fim'] . "',
				 sba2fim   = '" . $_REQUEST['sba2fim'] . "',
				 sba3fim   = '" . $_REQUEST['sba3fim'] . "',
				 sba4fim   = '" . $_REQUEST['sba4fim'] . "',
				 sbaunt    = " . $sbaunt . ",
				 sbauntdsc = '" . $_REQUEST['sbauntdsc'] . "',
                 --prgid     = '" . $_REQUEST['prgid'] . "',
			";
			*/
			
			$dadosExtra = "
				 sbastgmpl = '" . urldecode( $_REQUEST['sbastgmpl'] ) . "',
				 sbaprm    = '" . urldecode( $_REQUEST['sbaprm'] ) . "',
				 prgid     = " . $prgid  . ",
				 undid     = " . $undid . ",
				 frmid     = " . $frmid . ",
				 foaid     = " . $foaid . ",
				 sbapcr    = '" .  urldecode( $_REQUEST['sbapcr'] )  . "',
				 sba0ano   = '" . ( (integer) $_REQUEST['sba0ano'] ) . "',
				 sba1ano   = '" . ( (integer) $_REQUEST['sba1ano'] ) . "',
				 sba2ano   = '" . ( (integer) $_REQUEST['sba2ano'] ) . "',
				 sba3ano   = '" . ( (integer) $_REQUEST['sba3ano'] ) . "',
				 sba4ano   = '" . ( (integer) $_REQUEST['sba4ano'] ) . "',
				 sba0ini   = '" . urldecode( $_REQUEST['sba0ini'] ) . "',
				 sba1ini   = '" . urldecode( $_REQUEST['sba1ini'] ) . "',
				 sba2ini   = '" . urldecode( $_REQUEST['sba2ini'] ) . "',
				 sba3ini   = '" . urldecode( $_REQUEST['sba3ini'] ) . "',
				 sba4ini   = '" . urldecode( $_REQUEST['sba4ini'] ) . "',
				 sba0fim   = '" . urldecode( $_REQUEST['sba0fim'] ) . "',
				 sba1fim   = '" . urldecode( $_REQUEST['sba1fim'] ) . "',
				 sba2fim   = '" . urldecode( $_REQUEST['sba2fim'] ) . "',
				 sba3fim   = '" . urldecode( $_REQUEST['sba3fim'] ) . "',
				 sba4fim   = '" . urldecode( $_REQUEST['sba4fim'] ) . "',
				 sbaunt    = " . $sbaunt . ",
				 sbauntdsc = '" .  urldecode( $_REQUEST['sbauntdsc'] )  . "',
                 --prgid     = '" . $_REQUEST['prgid'] . "',
			";
		}

		 $sql = "
			 update cte.subacaoindicador
			 set
				 " . $dadosExtra . "
				 psuid = " . $psuid . ",
				 sbaparecer = " . $sbaparecer . ",
				 ssuid = " . $ssuid . ",
				 sbadata = '" . date( "Y-m-d" ) . "'
			 where
				 sbaid = " . $sbaid . "
		";
		
		ob_end_clean();
		if ( $db->executar( $sql ) )
		{
			echo "sucesso" ;
			$db->commit();
		}
		else
		{
			echo "falha";
			$db->rollback();
		}
		exit();

	default:
		echo '';
		exit();

}

?>