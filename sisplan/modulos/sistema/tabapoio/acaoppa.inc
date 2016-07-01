<?php

switch( $_REQUEST['evento'] ){

	case 'cadastrar_acao':
 
		if( $_REQUEST['alterar'] != 'S' )
		{
			$sql = sprintf("INSERT INTO sisplan.acao 
								(acacod, prsano, prgcod, proid, unmid, acanome, acastatus) 
							VALUES 
								('%s', '%s', '%s', %d, %d, '%s', '%s')",
							$_REQUEST['acacod'],
							$_REQUEST['exercicio'],
							$_REQUEST['prgcod'],
							$_REQUEST['proid'],
							$_REQUEST['unmid'],
							$_REQUEST['acanome'],
							$_REQUEST['acastatus']);
		}
		else 
		{
			$sql = sprintf("UPDATE sisplan.acao SET
								prgcod = '%s', 
								proid = %d, 
								unmid = %d, 
								acanome = '%s', 
								acastatus = '%s'
							WHERE
								acacod = '%s'",
							$_REQUEST['prgcod'],
							$_REQUEST['proid'],
							$_REQUEST['unmid'],
							$_REQUEST['acanome'],
							$_REQUEST['acastatus'],
							$_REQUEST['acacod']);
		}
		
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
		} else {
			$db->commit();
		}
		$db->sucesso( $_REQUEST['modulo'], $parametros );

		break;
		
	case 'excluir_acao':
		
		$sql = sprintf("DELETE FROM 
							sisplan.acao 
						WHERE 
							acacod = '%s'", 
						$_REQUEST['codigo']);
		
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
		} else {
			$db->commit();
		}

		$db->sucesso( $_REQUEST['modulo'], $parametros );
		
		break;
		
	case 'editar_acao':
		$sql = "
			SELECT 
				ac.prsano,
				ac.acacod,
				ac.acanome,
				pg.prgcod,
				pd.proid,
				um.unmid,
				ac.acastatus
			FROM 
				sisplan.acao ac
			JOIN sisplan.programa pg
				ON ac.prgcod = pg.prgcod
			JOIN sisplan.produto pd
				ON ac.proid = pd.proid
			JOIN sisplan.unidademedida um
				ON ac.unmid = um.unmid
			WHERE
				ac.acacod = '{$_REQUEST['codigo']}'";
		
		$dados = $db->carregar($sql);

		$exercicio = $dados[0]['prsano'];
		$acacod	   = $dados[0]['acacod'];
		$acanome   = $dados[0]['acanome'];
		$prgcod	   = $dados[0]['prgcod'];
		$proid	   = $dados[0]['proid'];
		$unmid     = $dados[0]['unmid'];
		$acastatus = $dados[0]['acastatus'];
		$alterar   = 'S';
		break;
}

// CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Tabelas de Apoio - PPA', 'Ação' );
?>


<script language="javascript" type="text/javascript">
	
	function cadastrar_controle()
	{
		if ( validar_formulario_controle() ) 
		{
			document.controle.evento.value = 'cadastrar_acao';
			document.controle.submit();
		}
	}
	
	function validar_formulario_controle()
	{
		var validacao = true;
		var mensagem = 'Os seguintes campos não foram preenchidos:';
		
		if ( document.controle.acacod.value == '' ) 
		{
			mensagem += '\nCódigo';
			validacao = false;
		}
		
		if ( document.controle.prgcod.value == '' ) 
		{
			mensagem += '\nPrograma';
			validacao = false;
		}
		
		if ( document.controle.proid.value == '' ) 
		{
			mensagem += '\nProduto';
			validacao = false;
		}
		
		if ( document.controle.unmid.value == '' ) 
		{
			mensagem += '\nUnidade de Medida';
			validacao = false;
		}
		
		if ( document.controle.acanome.value == '' ) 
		{
			mensagem += '\nNome';
			validacao = false;
		}

		if ( document.controle.acastatus.value == '' ) 
		{
			mensagem += '\nSituação';
			validacao = false;
		}
  
		if ( !validacao ) 
			alert( mensagem );

		return validacao;
	}
	
	function pesquisar_controle () 
	{
		document.controle.evento.value = 'pesquisar_acao';
		document.controle.submit();
	}
	
	
	function limpar_controle () 
	{
		location.href = '<?=$_SESSION['sisarquivo'] . ".php?modulo=".$_REQUEST['modulo']."&acao=A"?>';
	}
	
	
	function excluir_controle( acacod )
	{
		if ( confirm( 'Deseja excluir a Ação?' ) ) 
		{
			document.controle.codigo.value = acacod;
			document.controle.evento.value = 'excluir_acao';
			document.controle.submit();
		}
	}
	
	function editar_controle ( acacod )
	{
		document.controle.codigo.value = acacod;
		document.controle.evento.value = 'editar_acao';
		document.controle.submit();
	}

	
	function ltrim( value ){
		var re = /\s*((\S+\s*)*)/;
		return value.replace(re, "$1");
	}
	
	function rtrim( value ){
		var re = /((\s*\S+)*)\s*/;
		return value.replace(re, "$1");
	}
	
	function trim( value ){
		return ltrim(rtrim(value));
	}

</script>
<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="0" align="center">
	<tr>
		<td>
			<!-- NOVO CONTROLE -->
				<form method="post" name="controle">
					<input type="hidden" name="evento" value=""/>
					<input type="hidden" name="codigo" value=""/>
					<input type="hidden" name="alterar" value="<?= $alterar == "" ? "" : "S"; ?>"/>
					<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Ano:</td>
							<td><?= campo_texto( 'exercicio', 'S', 'N', '', 5, 4, '####', '' ); ?></td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Código:</td>
							<td>
								<?= campo_texto( 'acacod', 'S', $acacod == '' ? '' : 'N', '', 8, 4, '', '', '','',0, '', 'this.value=this.value.toUpperCase()' ); ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Programa:</td>
							<td>
							<?
								$sql = "SELECT 
											prgcod AS codigo,
											prgcod ||' - '|| prgnome as descricao 
										FROM 
											sisplan.programa 
										WHERE 
											prsano = '{$exercicio}' 
										AND prgstatus = 'A' 
										ORDER BY 
											prgnome";
								
								$db->monta_combo( 'prgcod', $sql, 'S', '&nbsp;', '', '' );
							?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Produto:</td>
							<td>
							<?
								$sql = "SELECT 
											proid AS codigo,
											procod ||' - '|| pronome as descricao 
										FROM 
											sisplan.produto 
										WHERE 
											prostatus = 'A' 
										ORDER BY 
											pronome";
								
								$db->monta_combo( 'proid', $sql, 'S', '&nbsp;', '', '' );
							?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Unidade de Medida:</td>
							<td>
							<?
								$sql = "SELECT 
											unmid AS codigo,
											unmcod ||' - '|| unmnome as descricao 
										FROM 
											sisplan.unidademedida 
										WHERE 
											unmstatus = 'A' 
										ORDER BY 
											unmnome";
								
								$db->monta_combo( 'unmid', $sql, 'S', '&nbsp;', '', '' );
							?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Nome:</td>
							<td>
								<?= campo_texto( 'acanome', 'S', 'S', '', 40, 400, '', '' ); ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Situação:</td>
							<td>
								<input type="radio" id="A" name="acastatus" value="A" <?= $acastatus == 'A' || $acastatus == '' ? "checked='checked'" : ""; ?>  /> Ativo 
								<input type="radio" id="I" name="acastatus" value="I" <?= $acastatus == 'I' ? "checked='checked'" : ""; ?>/> Inativo
													
							</td>
						</tr>
						<tr style="background-color: #cccccc">
							<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
							<td>
							<input type="button" name="botao" value="Pesquisar" onclick="pesquisar_controle();"/>
							<input type="button" name="botao" value="Limpar" onclick="limpar_controle();"/>
							<input type="button" name="botao" value="Gravar" onclick="cadastrar_controle();"/>
							</td>
						</tr>
					</table>
				</form>
		</td>
	</tr>
</table>
<!-- LISTA -->
<?php

unset( $sql );

$cabecalho = array( 'Ações', 'Ano', 'Código', 'Nome', 'Programa', 'Produto', 'Unidade de Medida', 'Situação' );

$img = "<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Ação\" onclick=\"editar_controle(' || '\'' || acacod || '\'' || ')\" >";
$img_1 = "<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir Ação\" onclick=\"excluir_controle(' || '\'' || acacod || '\'' || ')\" >";

if ( $_REQUEST['evento'] == 'pesquisar_acao' )
{
	$where = "";
	if ( !empty($_REQUEST['acacod']) )
		$where .= "AND ac.acacod = '{$_REQUEST['acacod']}'";

	if ( !empty($_REQUEST['prgcod']) )
		$where .= "AND ac.prgcod = '{$_REQUEST['prgcod']}'";
		
	if ( !empty($_REQUEST['proid']) )
		$where .= "AND ac.proid = {$_REQUEST['proid']}";
		
	if ( !empty($_REQUEST['unmid']) )
		$where .= "AND ac.unmid = {$_REQUEST['unmid']}";
		
	if ( !empty($_REQUEST['acanome']) )
		$where .= "AND ac.acanome ILIKE '%{$_REQUEST['acanome']}%'";
		
	if ( !empty($_REQUEST['acastatus']) )
		$where .= "AND ac.acastatus = '{$_REQUEST['acastatus']}'";
}

$sql = "
SELECT 
	'{$img}&nbsp;{$img_1}' AS acao,
	ac.prsano,
	ac.acacod,
	ac.acanome,
	pg.prgnome,
	pd.pronome,
	um.unmnome,
	CASE  
		WHEN ac.acastatus = 'I' THEN 'Inativo'
		ELSE 'Ativo'
	END as acastatus
FROM 
	sisplan.acao ac
JOIN sisplan.programa pg
	ON ac.prgcod = pg.prgcod
JOIN sisplan.produto pd
	ON ac.proid = pd.proid
JOIN sisplan.unidademedida um
	ON ac.unmid = um.unmid
WHERE
	ac.prsano = '{$exercicio}'
AND pg.prsano = '{$exercicio}'
{$where}
";

// exibe o resultado da consulta
$db->monta_lista( $sql, $cabecalho, 100, 20, '', '', '' );
?>