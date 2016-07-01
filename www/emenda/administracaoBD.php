<?php

set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as funções gerais
include_once "config.inc";
include_once "_funcoes.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

header('content-type: text/html; charset=ISO-8859-1');
if( $_SESSION['usucpforigem'] != '' && $_SESSION['usucpforigem'] != '' && $_SESSION['usucpforigem'] != '' ) {
	echo '<script>history.back();</script>';
}

if(!$_SESSION['usucpf'])
	$_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$_POST['script'] = str_replace( "\'", "'", $_POST['script'] );
if( $_POST['carregacolunas'] ){
	$sql = "SELECT DISTINCT
                    pg_class.relname AS tabela,
                    pg_attribute.attname AS coluna
                FROM 
                    pg_class
                JOIN 
                    pg_namespace ON pg_namespace.oid = pg_class.relnamespace AND pg_namespace.nspname NOT LIKE 'pg_%'
                JOIN 
                    pg_attribute ON pg_attribute.attrelid = pg_class.oid AND pg_attribute.attisdropped = 'f'
                JOIN
                    pg_type ON pg_type.oid = pg_attribute.atttypid
                JOIN 
                    pg_index ON pg_index.indrelid=pg_class.oid
                LEFT JOIN
                    pg_constraint ON (pg_attribute.attrelid = pg_constraint.conrelid AND pg_constraint.conkey[1] = pg_attribute.attnum AND pg_constraint.contype != 'u')
                WHERE 
                    pg_namespace.nspname = 'emenda'
                AND 
                    pg_attribute.attnum > 0
                AND 
                    pg_attribute.attrelid = pg_class.oid
                AND 
                    pg_attribute.atttypid = pg_type.oid
               	AND
                    pg_class.relname = '".$_POST['tabela']."'
                ORDER BY
                    pg_class.relname,
                    pg_attribute.attname";
	$arDados = $db->carregar( $sql );
	foreach ($arDados as $v) {
		echo '<input type="checkbox" name="colunas[]" id="colunas[]" value="'.$v['coluna'].'">'.$v['coluna'];
	}
	
	die;	
}

$script = $_POST['script'];

monta_titulo( 'Administração do Banco de Dados Emenda', '');
?>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<form id="formulario" method="post" action="">
<input type="hidden" name="action" id="action" value="" />

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<!-- <tr>
		<td class="SubTituloDireita" style="width: 25%;"><b>Esquema:</b></td>
        <td><?=campo_texto( 'tabela', 'S', 'S', 'Tabela', 50, 500, '', '','','','','id="tabela"','','','') ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width: 25%;"><b>Tabela:</b></td>
        <td><?=campo_texto('tabela', 'S', 'S', 'Tabela', 50, 500, '', '','','','','id="tabela"','','','carregaColunas(this.value)') ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width: 25%;"><b>Colunas:</b></td>
        <td><div id="coluna"></div></td>
	</tr>-->
	<tr>
		<td class="SubTituloDireita" style="width: 15%;"><b>Tipo de Execução:</b></td>
        <td>
        	<input type="radio" value="E" id="tipoexecucao" name="tipoexecucao" <? if($_REQUEST["tipoexecucao"] == "E") { echo "checked"; } ?> /> EXECUTAR
			<input type="radio" value="C" id="tipoexecucao" name="tipoexecucao" <? if($_REQUEST["tipoexecucao"] == "C" || empty($_REQUEST["tipoexecucao"]) ) { echo "checked"; } ?> /> CARREGAR
			<input type="radio" value="X" id="tipoexecucao" name="tipoexecucao" <? if($_REQUEST["tipoexecucao"] == "X") { echo "checked"; } ?> /> GERAR EXCEL
        </td>
	</tr>
	<tr>
		<td class="SubTituloDireita"><b>SQL:</b></td>
        <td><?=campo_textarea('script', 'S', 'S', 'SQL', 200, 25, 10000, '', '', '', '', 'SQL');?></td>
	</tr>
	<tr>
		<td colspan="2" style="text-align: center;"><input type="button" name="botao" value="Executar" onclick="submeterDados();"></td>
	</tr>
</table>
<script type="text/javascript">
	function submeterDados(){
		document.getElementById('action').value = 'executar';
		document.getElementById('formulario').submit();
	}
	function carregaColunas( tabela ){
		/*var myajax = new Ajax.Request('administracaoBD.php', {
				        method:     'post',
				        parameters: '&carregacolunas=true&tabela='+tabela,
				        asynchronous: false,
				        onComplete: function (res){
							$('coluna').innerHTML = res.responseText;
				        }
				  });*/
	}
</script>
</form>
<?
if( $_POST['action'] == 'executar' && ($_SESSION['usucpforigem'] != '' || $_SESSION['usucpforigem'] != '') ){
	$sql = $_POST['script'];
	
	if( $_POST['tipoexecucao'] == "E" ){
		$tipo = trim(substr( $sql, 0, 6 ));
		if( strtolower($tipo) == 'delete' || strtolower($tipo) == 'update' || strtolower($tipo) == 'insert' ){
			$db->executar( $sql );
			$db->commit();
			echo "<script>
					alert('Operação realizada com sucesso');
					window.location.href = window.location;
				</script>";
			die;
		}
	} else {
		$tipo = substr( $sql, 0, 6 );
		if( strtolower($tipo) == 'select' ){
			if( $_POST['tipoexecucao'] == "X" ){
				ob_clean();
				header('content-type: text/html; charset=ISO-8859-1');
				
				$sql = str_replace( '\"', '"', $sql );				
				$sql = str_replace( "\'", "'", $sql );				
				
				$db->sql_to_excel($sql, 'relEmendasPTA', $cabecalho, $formato);
				exit;
			} else {
				$cabecalho = substr( $sql, 6, (int)strpos( str_to_upper($sql), 'FROM' ) - 6 );
				$cab = $cabecalho; 
				$cabecalho = explode( ',', $cabecalho );
					
				$db->monta_lista($sql, $cabecalho, 20, 4, 'N','Center','','form');
			}
		}
	}		
}
?>