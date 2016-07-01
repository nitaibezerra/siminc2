<?

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

$db = new cls_banco();

function erro( $codigo, $mensagem, $arquivo, $linha ){
//	echo "$codigo, $mensagem, $arquivo, $linha";
	echo "Ocorreu um erro. Por favor tente mais tarde.";
	exit();
}

function excecao( Exception $excecao ){
//	echo $excecao->getMessage();
	echo "Ocorreu um erro. Por favor tente mais tarde.";
	exit();
}

set_error_handler( 'erro', E_USER_ERROR );
set_exception_handler( 'excecao' );

$strEvent = $_REQUEST[ 'rs' ];
$arrArgs =  $_REQUEST[ 'rsargs' ];

/*
$sql = "SELECT
			u.usucpf,
			u.usunome
		FROM
			seguranca.usuario AS u
		WHERE
			u.usunome LIKE '" . $_POST['searchterm'] . "%'
		LIMIT 1";
$dados = $db->carregar($sql);

for($i=0; count($dados); $i++) {
	echo "<li id=\"" . $dados[$i]['usucpf'] . "\">" . $dados[$i]['usunome'] . "</li>";
}*/

function PrepareSuggestList( $strWord , $intIdSuggest )
{
	global $db;
	$sql = "SELECT
				sidid
	 		FROM
	 			demandas.demanda
	 		WHERE 
	 			dmdid=".$_GET["dmdid"];
	
	$sidid = $db->pegaUm($sql);
	
	if ($sidid){
		$sql = "SELECT
					trim(u.usucpf) as usucpf,
					trim(u.usunome) as usunome
				FROM
					seguranca.usuario AS u
				INNER JOIN 
					demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf	
				INNER JOIN 
					seguranca.usuario_sistema us ON us.usucpf = u.usucpf and us.sisid=44 and us.suscod='A'			 
				WHERE 
					us.suscod='A' AND
					ur.rpustatus = 'A' AND 
					ur.celid in (select celid from demandas.sistemacelula where sidid = " . $sidid . ") AND
					TRANSLATE(u.usunome, 'áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ','aaaaeeiooouucAAAAEEIOOOUUC')  
					ILIKE TRANSLATE('" . $strWord . "%', 'áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ','aaaaeeiooouucAAAAEEIOOOUUC')
				ORDER BY u.usunome	
				";		
	} else {
		$sql = "SELECT DISTINCT
					trim(u.usucpf) as usucpf,
					trim(u.usunome) as usunome
				FROM
					seguranca.usuario  AS u
				INNER JOIN 
					demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf
				INNER JOIN 
					seguranca.usuario_sistema us ON us.usucpf = u.usucpf and us.sisid=44 and us.suscod='A'			 
				WHERE 
					us.suscod='A' AND 
					ur.rpustatus = 'A' AND 
					ur.sidid IS NULL AND
					TRANSLATE(u.usunome, 'áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ','aaaaeeiooouucAAAAEEIOOOUUC')  
					ILIKE TRANSLATE('" . $strWord . "%', 'áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ','aaaaeeiooouucAAAAEEIOOOUUC')
					AND 
					ur.ordid = (
							 	SELECT 
							 		tp.ordid
								FROM 
									demandas.demanda AS d
								LEFT JOIN 
									demandas.tiposervico AS tp ON tp.tipid = d.tipid
								WHERE 
									dmdid = ".$_GET["dmdid"].")
				ORDER BY 2
				";
	}
	$arrUsuarios = $db->carregar($sql);
	
	?>
		<div style="width:500px">
		  <? if($arrUsuarios) { ?>
			<? foreach ( $arrUsuarios as $intKey => $arrUsuario ): ?>
				<div id="<?=trim($arrUsuario['usucpf'])?>" style=""
				onmouseover="window.Suggest.arrInstances[<?= $intIdSuggest ?>].mouseOverSuggest(<?= $intKey ?>)"
				onclick="window.Suggest.arrInstances[<?= $intIdSuggest ?>].clickSuggest(<?= $intKey ?>)">
					<?= trim($arrUsuario['usunome']) ?>
				</div>
			<? endforeach ?>
		  <? } ?>
		</div>
	<?
}

try
{
	switch( $strEvent )
	{
		case 'SuggestsUsuario':
		{
			if( sizeof( $arrArgs ) < 0 )
			{
			//	throw new Exception( 'Parametros Invalidos' );
			}
			$strWord		= $arrArgs[ 0 ];
			$intIdSuggest	= $arrArgs[ 1 ]; 
			PrepareSuggestList( $strWord , $intIdSuggest);
			break; 
		}
		default:
		{
			throw new Exception( 'Parametros Invalidos' );
			break;		
		}
	}
}
catch( Exception $objError )
{
	excecao( $objError );
}

?>