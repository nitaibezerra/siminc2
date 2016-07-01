<?php

cte_verificaSessao();

foreach ( $_REQUEST as $chave => $valor ) {
	if ( is_string( $valor ) ) {
		$_REQUEST[$chave] = stripslashes( str_replace("'","''", trim( stripslashes( $valor ) ) ) );
	}
}

include APPRAIZ . 'includes/cabecalho.inc';

/* configurações */
ini_set("memory_limit", "2048M");
/* FIM configurações */


print '<br/>';
$db->cria_aba( $abacod_tela, $url, '&indid='. $_REQUEST['indid'] );
cte_montaTitulo( $titulo_modulo, '&nbsp;' );

$habil = 'S';

if ( !$_SESSION['inuid'] )
{
	?>
  	<script type="text/javascript">
  		window.location.href = '/brasilpro/brasilpro.php?modulo=principal/estrutura_avaliacao&acao=A';
  	</script>
   	<?php
	exit();
}

// ----- CARREGA DADOS DO BANCO ------------------------------------------------
	$sql = "select * from cte.instrumentounidade where inuid = " . $_SESSION["inuid"];
	$dados = $db->recuperar( $sql );
	if ( is_array( $dados ) )
	{
		$inuid = $dados['inuid'];
		$itrid = $dados['itrid'];
		$estuf = $dados['estuf'];
		$muncod = $dados['muncod'];
		$mun_estuf = $dados['mun_estuf'];
		$inueqploc = $dados['inueqploc'];
		$inuddsdmgr = $dados['inuddsdmgr'];
		$inucadsec = $dados['inucadsec'];
		$inucadcmt = $dados['inucadcmt'];
		$usucpf = $dados['usucpf'];
		$inudata = $dados['inudata'];
	}
	
	$inuid = $inuid == "null" ? "" : $inuid;
	$itrid = $itrid == "null" ? "" : $itrid;
	$estuf = $estuf == "null" ? "" : $estuf;
	$muncod = $muncod == "null" ? "" : $muncod;
	$mun_estuf = $mun_estuf == "null" ? "" : $mun_estuf;
	$inueqploc = $inueqploc == "null" ? "" : $inueqploc;
	$inuddsdmgr = $inuddsdmgr == "null" ? "" : $inuddsdmgr;
	$inucadsec = $inucadsec == "null" ? "" : $inucadsec;
	$inucadcmt = $inucadcmt == "null" ? "" : $inucadcmt;
	$usucpf  = $usucpf == "null" or trim($usucpf) == "" ? "" : $usucpf;
	$inudata  = $inudata == "null" ? "" : $inudata;
// ----- FIM CARREGA DADOS DO BANCO --------------------------------------------

// ----- TRATAMENTO SUBMISSAO --------------------------------------------------
if ( isset( $_REQUEST['formulario'] ) && $habil == 'S' )
{
	// trata entradas
	$inueqploc  = trim( str_replace( "'", "\\'", $_REQUEST['inueqploc'] ) );
	$inuddsdmgr  = trim( str_replace( "'", "\\'", $_REQUEST['inuddsdmgr'] ) );
	$inucadsec  = trim( str_replace( "'", "\\'", $_REQUEST['inucadsec'] ) );
	$inucadcmt  = trim( str_replace( "'", "\\'", $_REQUEST['inucadcmt'] ) );
	
	
	// ----- UPDATE ------------------------------------------------------------
	$sql = "
		update cte.instrumentounidade set
			inueqploc = '%s',		inuddsdmgr = '%s',
			inucadsec = '%s',       inucadcmt = '%s',
			inudata = '%s',	    	usucpf = '%s'
		where
			inuid = %d
	";
	$sql = sprintf(
		$sql,
			$inueqploc,				$inuddsdmgr,
			$inucadsec,				$inucadcmt,
			'now()',				$_SESSION['usucpf'],
			$_SESSION['inuid']
	);
	
	$sucesso = (boolean) $db->executar( $sql, false );
	
	// executa ação
	if ( $sucesso )
	{
		$mensagem = "Dados gravados com sucesso!";
		$db->commit();
		$_REQUEST["acao"] = "A";
		$db->sucesso( "principal/dados_unidade" );
	}
	else
	{
		$mensagem = "Não foi possível gravar os dados. Tente novamente mais tarde.";
		$db->rollback();
	}
	
	$itrid = $itrid == "null" ? "" : $itrid;
	$estuf = $estuf == "null" ? "" : $estuf;
	$muncod = $muncod == "null" ? "" : $muncod;
	$mun_estuf = $mun_estuf == "null" ? "" : $mun_estuf;
	$inueqploc = $inueqploc == "null" ? "" : $inueqploc;
	$inuddsdmgr = $inuddsdmgr == "null" ? "" : $inuddsdmgr;
	$inucadsec = $inucadsec == "null" ? "" : $inucadsec;
	$inucadcmt = $inucadcmt == "null" ? "" : $inucadcmt;
	$usucpf  = $usucpf == "null" or trim($usucpf) == "" ? "" : $usucpf;
	$inudata  = $inudata == "null" ? "" : $inudata;
	
	
}

?>
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<script language="javascript" type="text/javascript" src="../includes/TextEditor.js"></script>
<script language="javascript" type="text/javascript">
       TextEditor.init();
</script>

<style>

table.cronograma { margin: 10px; }
table.cronograma thead {}
table.cronograma thead tr {}
table.cronograma thead tr th { text-align: center; font-weight: bold; }
table.cronograma tbody {}
table.cronograma tbody tr {}
table.cronograma tbody tr td { text-align: left; }
table.cronograma tbody tr td.nome { text-align: right; }
table.cronograma tbody tr td div.data_inicio { float: left; }
table.cronograma tbody tr td div.data_fim { float: right; }


</style>

<form action="" method="post" name="formulario">
<input type="hidden" name="formulario" value="1"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita">Equipe Local:</td>
			<td>
				<?php if( $habil == 'S' ):?>
					<textarea name="inueqploc" class="cte" rows="20"><?=$inueqploc?></textarea>
				<?php else: ?>
					<?=$inueqploc?>
				<?php endif; ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Dados demográficos e educacionais:</td>
			<td>
				<?php if( $habil == 'S' ):?>
					<textarea name="inuddsdmgr" class="cte" rows="20"><?=$inuddsdmgr?></textarea>
				<?php else: ?>
					<?=$inuddsdmgr?>
				<?php endif; ?>
			</td>
		</tr>	
		<tr>
			<td class="SubTituloDireita">Cadastro Secretário(a) de Estado de Educação:</td>
			<td>
				<?php if( $habil == 'S' ):?>
					<textarea name="inucadsec" class="cte" rows="20"><?=$inucadsec?></textarea>
				<?php else: ?>
					<?=$inucadsec?>
				<?php endif; ?>
			</td>
		</tr>	
		<tr>
			<td class="SubTituloDireita">Cadastro Comitê Local:</td>
			<td>
				<?php if( $habil == 'S' ):?>
					<textarea name="inucadcmt" class="cte" rows="20"><?=$inucadcmt?></textarea>
				<?php else: ?>
					<?=$inucadcmt?>
				<?php endif; ?>
			</td>
		</tr>
		<!-- Fim Dados da Unidade -->
		<tr bgcolor="#C0C0C0">
			<td ></td>
			<td>
				<div style="float: left;">
					<?php if ($habil=='S'): ?>
						<input type='button' class="botao" name='cadastra' value='Salvar' onclick="salvar();"/>
					<?php endif; ?>
						<input type='button' class="botao" name='fecha' value='Fechar' onclick="fechar();"/>
				</div>
			</td>
		</tr>
	</table>
</form>

<br>

<script language="javascript" type="text/javascript">
	
	var editou = false;
	
	function validacaoFormulario( id )
	{
		var valida = true;
//		if( trim( tinyMCE.getContent('inueqploc') ) == "" ) valida = false;
//		if( trim( tinyMCE.getContent('inuddsdmgr') ) == "" ) valida = false;
//		if( trim( tinyMCE.getContent('inucadsec') ) == "" ) valida = false;
//		if( trim( tinyMCE.getContent('inucadcmt') ) == "" ) valida = false;
		return valida;
	}
	
	function salvar()
	{
		if( validacaoFormulario( 1 ) ) 
		{
			document.formulario.action = '';
			document.formulario.submit();
		}
		else
		{
			alert(msg);
			return false;
		}
	}
	
	function marcarAlteracao(){
		editou = true;
	}
	
	function fechar()
	{
		if(editou) 
		{	
			if( confirm( 'Atenção! As alterações realizadas não serão gravadas. Deseja fechar o formulário?' ) )
				window.location.href = '/brasilpro/brasilpro.php?modulo=principal/estrutura_avaliacao&acao=A';
		}
		else
		{
			window.location.href = '/brasilpro/brasilpro.php?modulo=principal/estrutura_avaliacao&acao=A';
		}
	}
	
</script>