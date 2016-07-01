<?php

switch( $_REQUEST['evento'] ){

	case 'cadastrar_acao':

		$dados     = explode("_", $_REQUEST['executora']);
		$unicod    = $dados[0];
		$unitpocod = $dados[1];
		
		if( $_REQUEST['alterar'] != 'S' )
		{			
			$sql = sprintf("INSERT INTO sisplan.acaolocalizadorunidade 
								(acacod, prsano, prgcod, loccod, unicod, unitpocod, acasnrap, alustatus) 
							VALUES 
								('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
							$_REQUEST['acacod'],
							$_REQUEST['exercicio'],
							$_REQUEST['prgcod'],
							$_REQUEST['loccod'],
							$unicod,
							$unitpocod,
							$_REQUEST['acasnrap'],
							$_REQUEST['alustatus']);
		}
		else 
		{
			$sql = sprintf("UPDATE sisplan.acaolocalizadorunidade SET
								acacod = '%s', 
								prsano = '%s', 
								prgcod = '%s', 
								loccod = '%s', 
								unicod = '%s', 
								unitpocod = '%s',
								acasnrap = '%s',
								alustatus = '%s'
							WHERE
								aluid = %d",
							$_REQUEST['acacod'],
							$_REQUEST['exercicio'],
							$_REQUEST['prgcod'],
							$_REQUEST['loccod'],
							$unicod,
							$unitpocod,
							$_REQUEST['acasnrap'],
							$_REQUEST['alustatus'],
							$_REQUEST['codigo']);
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
							sisplan.acaolocalizadorunidade 
						WHERE 
							aluid = %d", 
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
				al.aluid,
				al.acacod,
				al.prsano,
				al.prgcod,
				al.loccod,
				al.unicod||'_'||al.unitpocod as executora,
				al.acasnrap,
				al.alustatus
			FROM 
				sisplan.acaolocalizadorunidade al
			WHERE
				al.aluid = '{$_REQUEST['codigo']}'";
		
		$dados = $db->carregar($sql);

		$_REQUEST['aluid ']    = $dados[0]['aluid'];
		$_REQUEST['acacod']	   = $dados[0]['acacod'];
		$_REQUEST['prsano']    = $dados[0]['prsano'];
		$_REQUEST['prgcod']	   = $dados[0]['prgcod'];
		$_REQUEST['loccod']	   = $dados[0]['loccod'];
		$_REQUEST['executora'] = $dados[0]['executora'];
		$_REQUEST['acasnrap']  = $dados[0]['acasnrap'];
		$_REQUEST['alustatus'] = $dados[0]['alustatus'];
		$alterar   = 'S';
		break;
}

// CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Tabelas de Apoio - PPA', 'Ação Localizador Unidade' );
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
		
		
		if ( document.controle.prgcod.value == '' ) 
		{
			mensagem += '\nPrograma';
			validacao = false;
		}
		
		if ( document.controle.acacod.value == '' ) 
		{
			mensagem += '\nAção';
			validacao = false;
		}
		
		if ( document.controle.executora.value == '' ) 
		{
			mensagem += '\nUnidade Executora';
			validacao = false;
		}
		
		if ( document.controle.loccod.value == '' ) 
		{
			mensagem += '\nLocalizador';
			validacao = false;
		}
		
		if ( document.controle.acasnrap.value == '' ) 
		{
			mensagem += '\RAP';
			validacao = false;
		}

		if ( document.controle.alustatus.value == '' ) 
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
	
	
	function excluir_controle( aluid )
	{
		if ( confirm( 'Deseja excluir a Ação?' ) ) 
		{
			document.controle.codigo.value = aluid;
			document.controle.evento.value = 'excluir_acao';
			document.controle.submit();
		}
	}
	
	function editar_controle ( aluid )
	{
		document.controle.codigo.value = aluid;
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
					<input type="hidden" name="codigo" value="<?=$_REQUEST['aluid ']?>"/>
					<input type="hidden" name="alterar" value="<?= $alterar == "" ? "" : "S"; ?>"/>
					<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Ano:</td>
							<td><?= campo_texto( 'exercicio', 'S', 'N', '', 5, 4, '####', '' ); ?></td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Programa:</td>
							<td>
							<?
								$prgcod = $_REQUEST['prgcod'];
								
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
								
								$db->monta_combo( 'prgcod', $sql, 'S', '&nbsp;', 'document.controle.submit()', '' );
							?>
							<?=obrigatorio()?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Ação:</td>
							<td>
							<?
								$sql = "SELECT 
											acacod AS codigo,
											acacod ||' - '|| acanome as descricao 
										FROM 
											sisplan.acao 
										WHERE 
											prsano = '{$exercicio}' 
										AND acastatus = 'A' 
										AND prgcod = '{$prgcod}'
										ORDER BY 
											acanome";
								
								$dados = $db->carregar($sql);
								
								$hb = $prgcod == "" || $dados[0]['codigo'] == '' ? 'N' : 'S';
								
								$acacod = $_REQUEST['acacod'];
								
								$db->monta_combo( 'acacod', $sql, $hb, '&nbsp;', '', '','',200 );
							?>
							<?=obrigatorio()?>
							</td>
						</tr>

						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Unidade Executora:</td>
							<td>
							<?
							/*
								$sql = "SELECT 
											unicod||'_'||unitpocod AS codigo,
											unidsc AS descricao
										FROM 
											public.unidade
										WHERE 
											orgcod = '42000' 
										ORDER BY 
											unidsc";
								*/
								$sql = "SELECT * FROM planointerno.unidadeexecutora_uo";
								$executora = $_REQUEST['executora'];
								
								$db->monta_combo( 'executora', $sql, 'S', '&nbsp;', '', '' );
							?>
							</td>
						</tr>

						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Localizador:</td>
							<td>
							<?
								$sql = "SELECT 
											loccod AS codigo,
											loccod ||' - '|| locnome as descricao 
										FROM 
											sisplan.localizador 
										WHERE 
											locstatus = 'A' 
										ORDER BY 
											locnome";
								
								$loccod = $_REQUEST['loccod'];
								
								$db->monta_combo( 'loccod', $sql, 'S', '&nbsp;', '', '' );
							?>
							<?=obrigatorio()?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">RAP:</td>
							<td>
							<? $acasnrap = $_REQUEST['acasnrap'] == 'f' || $_REQUEST['acasnrap'] == '0' ? '0' : '1'; ?>
							<?= campo_radio_sim_nao('acasnrap', 'H'); ?>					
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Situação:</td>
							<td>
								<? $alustatus = $_REQUEST['alustatus'];//$alustatus == "" ? $_REQUEST['alustatus'] : $alustatus; ?>
								<input type="radio" id="A" name="alustatus" value="A" <?= $alustatus == 'A' || $alustatus == '' ? "checked='checked'" : ""; ?>  /> Ativo 
								<input type="radio" id="I" name="alustatus" value="I" <?= $alustatus == 'I' ? "checked='checked'" : ""; ?>/> Inativo
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

$cabecalho = array( 'Ações', 'Ano', 'Ação', 'Programa', 'Localizador', 'Unidade Executora', 'RAP', 'Situação' );

$img = "<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Ação\" onclick=\"editar_controle(' || '\'' || al.aluid || '\'' || ')\" >";
$img_1 = "<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir Ação\" onclick=\"excluir_controle(' || '\'' || al.aluid || '\'' || ')\" >";

if ( $_REQUEST['evento'] == 'pesquisar_acao' )
{
	$where = "";
	if ( !empty($_REQUEST['acacod']) )
		$where .= "AND al.acacod = '{$_REQUEST['acacod']}'";

	if ( !empty($_REQUEST['prgcod']) )
		$where .= "AND al.prgcod = '{$_REQUEST['prgcod']}'";
		
	if ( !empty($_REQUEST['loccod']) )
		$where .= "AND al.loccod = '{$_REQUEST['loccod']}'";
		
	if ( !empty($_REQUEST['executora']) )
	{
		$dados     = explode("_", $_REQUEST['executora']);
		$where .= "AND al.unicod = '{$dados[0]}'";
		$where .= "AND al.unitpocod = '{$dados[1]}'";
	}
		
	if ( !empty($_REQUEST['acasnrap']) )
		$where .= "AND al.acasnrap = '{$_REQUEST['acasnrap']}'";
		
	if ( !empty($_REQUEST['alustatus']) )
		$where .= "AND al.alustatus = '{$_REQUEST['alustatus']}'";
}

$sql = "
SELECT 
	'{$img}&nbsp;{$img_1}' AS acao,
	al.prsano,
	ac.acanome,
	pg.prgnome,
	lc.locnome,
	un.unidsc,
	CASE  
		WHEN al.acasnrap = 't' THEN 'Sim'
		ELSE 'Não'
	END AS acasnrap,
	CASE  
		WHEN al.alustatus = 'I' THEN 'Inativo'
		ELSE 'Ativo'
	END as acastatus
FROM 
	sisplan.acaolocalizadorunidade al
JOIN sisplan.acao ac
	ON al.acacod = ac.acacod
JOIN sisplan.programa pg
	ON al.prgcod = pg.prgcod AND pg.prsano = '{$exercicio}'
JOIN sisplan.localizador lc
	ON al.loccod = lc.loccod
JOIN public.unidade un
	ON al.unicod = un.unicod AND al.unitpocod = un.unitpocod
WHERE
	al.prsano = '{$exercicio}'
{$where}
";

// exibe o resultado da consulta
$db->monta_lista( $sql, $cabecalho, 100, 20, '', '', '' );
?>