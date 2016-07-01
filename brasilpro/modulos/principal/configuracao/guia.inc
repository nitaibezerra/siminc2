<?php

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );

if ( $_REQUEST["operacao"] ) {
	try {
		
		switch ( $_REQUEST["operacao"] ) {
			
			case "incluirSubacao":
				$sql = sprintf( "select count(*) from cte.proposicaoacao where ppaid = %d", $_REQUEST["ppaid"] );
				if ( $db->pegaUm( $sql ) != 1 ) {
					throw new Exception( "A ação não existe." );
				}
				$sql = sprintf( "insert into cte.proposicaosubacao ( ppaid, ppsdsc, ppsordem ) values ( %d, '%s', ( select count(*) + 1 from cte.proposicaosubacao where ppaid = %d ) )", $_REQUEST["ppaid"], $_REQUEST["ppsdsc"], $_REQUEST["ppaid"] );
				if ( !$db->executar( $sql ) ) {
					throw new Exception( "Erro ao inserir subação." );
				}
				break;
			
			case "excluirSubacao":
				$sql = sprintf( "select count(*) from cte.subacaoindicador where ppsid = %d", $_REQUEST["ppsid"] );
				if ( $db->pegaUm( $sql ) != 0 ) {
					throw new Exception( "A subação está em uso." );
				}
				$sql = sprintf( "select ppaid from cte.proposicaosubacao where ppsid = %d", $_REQUEST["ppsid"] );
				$ppaid = $db->pegaUm( $sql );
				$sql = sprintf( "delete from cte.proposicaosubacao where ppsid = %d", $_REQUEST["ppsid"] );
				if ( !$db->executar( $sql ) ) {
					throw new Exception( "Erro ao excluir subação." );
				}
				$sql = sprintf(
					"update cte.proposicaosubacao s1 set ppsordem = (
						select count(*) + 1
						from cte.proposicaosubacao s2
						where s2.ppaid = s1.ppaid and s2.ppsordem < s1.ppsordem
					) where ppaid = %d;",
					$ppaid
				);
				if ( !$db->executar( $sql ) ) {
					throw new Exception( "Erro ao reordenar o guia." );
				}
				$sql = sprintf(
					"update cte.subacaoindicador set
					sbaordem = ( select ppsordem from cte.proposicaosubacao where ppsid = sbaid )
					where ppsid in ( select ppsid from cte.proposicaosubacao where ppaid = %d )",
					$ppaid
				);
				if ( !$db->executar( $sql ) ) {
					throw new Exception( "Erro ao reordenar as subações." );
				}
				break;
			
			default:
				break;
			
		}
		$db->commit();
		$_REQUEST["acao"] = "A";
		$db->sucesso( $_REQUEST["modulo"] );
		
	} catch ( Exception $erro ) {
		$db->rollback();
		?>
		<html>
			<head>
				<script type="text/javascript">
				alert( '<?= $erro->getMessage(); ?>' );
				location.href = "?modulo=<?= $_REQUEST["modulo"] ?>&acao=<?= $_REQUEST["acao"] ?>";
				</script>
			</head>
			<body/>
		</html>
		<?
	}
}

$sql_instrumento = sprintf( "
	select it.itrid, it.itrdsc
	from cte.instrumento it
	where it.itrid in ( 3, 4 )
	order by it.itrdsc
" );
$instrumentos = $db->carregar( $sql_instrumento );
$instrumentos = $instrumentos ? $instrumentos : array();

$sql_dimensao = sprintf( "
	select d.dimid, d.dimcod, d.dimdsc, d.itrid
	from cte.dimensao d
	where d.dimstatus = 'A' and d.itrid in ( 3, 4 )
	order by d.dimcod
" );
$dimensoes = $db->carregar( $sql_dimensao );
$dimensoes = $dimensoes ? $dimensoes : array();

$sql_area = sprintf( "
	select ad.ardid, ad.ardcod, ad.arddsc, ad.dimid
	from cte.areadimensao ad
	inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A' and d.itrid in ( 3, 4 )
	where ad.ardstatus = 'A' 
	order by ad.ardcod
" );
$areas = $db->carregar( $sql_area );
$areas = $areas ? $areas : array();

$sql_indicador = sprintf( "
	select i.indid, i.indcod, i.inddsc, i.ardid
	from cte.indicador i
	inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
	inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A' and d.itrid in ( 3, 4 )
	where i.indstatus = 'A'
	order by i.indcod
" );
$indicadores = $db->carregar( $sql_indicador );
$indicadores = $indicadores ? $indicadores : array();

?>
<script language="javascript" type="text/javascript" src="../includes/dtree/dtree.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/dtree/dtree.css">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
<tr>
	<td><a href="javascript: windowOpen('/brasilpro/brasilpro.php?modulo=principal/configuracao/cadastroNumerosDeProcessos&acao=A','Cadastro de Números de Processos.','height=600,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes')" >Cadastra números de processo para geração do Parecer.</a></td>
	
</tr>
</table>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<colgroup>
		<col/>
	</colgroup>
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;" colspan="4">
				<div id="bloco" style="overflow: hidden;">
					<p>
						<a href="javascript: arvore.openAll();">Abrir Todos</a>
						&nbsp;|&nbsp;
						<a href="javascript: arvore.closeAll();">Fechar Todos</a>
					</p>
					<div id="_arvore"></div>
				</div>
			</td>
			<td style="background-color:#fafafa; color:#404040; width: 1px;" valign="top">
			</td>
		</tr>
	</tbody>
</table>

<script type="text/javascript">
	
	arvore = new dTree( 'arvore' );
	arvore.config.folderLinks = true;
	arvore.config.useIcons = true;
	arvore.config.useCookies = true; 
	
	arvore.add( 0, -1, 'Guia de Ações Padronizadas' );
	
	<?php foreach( $instrumentos as $instrumento ): ?>
		<?php
		$texto = "<b>{$instrumento["itrdsc"]}</b>";
		?>
		arvore.add( 'it_<?= $instrumento["itrid"] ?>', 0, "<?= $texto ?>", '', '', '', '../includes/dtree/img/question.gif', '../includes/dtree/img/question.gif' );
	<?php endforeach; ?>
	
	<?php foreach( $dimensoes as $dimensao ): ?>
		<?php
		$texto = "<b>{$dimensao["dimcod"]}</b> {$dimensao["dimdsc"]}";
		?>
		arvore.add( 'd_<?= $dimensao["dimid"] ?>', 'it_<?= $dimensao["itrid"] ?>', "<?= $texto ?>" );
	<?php endforeach; ?>
	
	<?php foreach( $areas as $area ): ?>
		<?php
		$texto = "<b>{$area["ardcod"]}</b> {$area["arddsc"]}";
		?>
		arvore.add( 'a_<?= $area["ardid"] ?>', 'd_<?= $area["dimid"] ?>', "<?= $texto ?>" );
	<?php endforeach; ?>
	
	<?php foreach( $indicadores as $indicador ): ?>
		<?php
		$texto = "<b>{$indicador["indcod"]}</b>&nbsp;{$indicador["inddsc"]}";
		?>
		arvore.add( 'i_<?= $indicador["indid"] ?>', 'a_<?= $indicador["ardid"] ?>', "<?= $texto ?>", 'javascript:editarIndicador(<?= $indicador["indid"] ?>);' );
	<?php endforeach; ?>
	
	elemento = document.getElementById( '_arvore' );
	elemento.innerHTML = arvore;
	
</script>

<script type="text/javascript">
	
	function editarIndicador( indid ){
		window.location = '?modulo=principal/configuracao/guia_indicador&acao=A&indid=' + indid;
	}
	
</script>