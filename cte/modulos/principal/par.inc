<?php

verifica_sessao();

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo_cte( $titulo_modulo );

include_once APPRAIZ . 'includes/workflow.php';

switch ( $_REQUEST['evento'] ) {
	case 'excluir':
		$sql = sprintf(
			"delete from cte.acaoindicador where ptoid = %d and acilocalizador = '%s'",
			$_REQUEST['ptoid'],
			$_REQUEST['acilocalizador']
		);
		$db->executar( $sql );
		$db->commit();
		break;
}

$sql_dimensao = sprintf(
	"select d.dimid, d.dimcod, d.dimdsc
	from cte.dimensao d
	where d.dimstatus = 'A' and d.itrid = %d
	order by d.dimcod",
	$_SESSION['itrid']
);
$dimensoes = $db->carregar( $sql_dimensao );
if ( !$dimensoes ) {
	$dimensoes = array();
}

$sql_area = sprintf(
	"select a.ardid, a.ardcod, a.arddsc, a.dimid
	from cte.areadimensao a
	inner join cte.dimensao d on d.dimid = a.dimid
	where a.ardstatus = 'A' and d.dimstatus = 'A' and d.itrid = %d
	order by a.ardcod",
	$_SESSION['itrid']
);
$areas = $db->carregar( $sql_area );
if ( !$areas ) {
	$areas = array();
}

$sql_indicador = sprintf(
	"select i.indid, i.inddsc, i.ardid, i.indcod, p.ptoid, c.ctrpontuacao, aie.aciid as acaoestadual, aim.aciid as acaomunicipal, length(ptodemandaestadual) as demandaestadual, length(ptodemandamunicipal) as demandamunicipal
	from cte.indicador i
	inner join cte.areadimensao a on a.ardid = i.ardid
	inner join cte.dimensao d on d.dimid = a.dimid
	inner join cte.pontuacao p on p.indid = i.indid and p.inuid = %d
	left join cte.acaoindicador aie on aie.ptoid = p.ptoid and aie.acilocalizador = 'E'
	left join cte.acaoindicador aim on aim.ptoid = p.ptoid and aim.acilocalizador = 'M'
	inner join cte.criterio c on c.crtid = p.crtid
	where i.indstatus = 'A' and a.ardstatus = 'A' and d.dimstatus = 'A' and d.itrid = %d
	order by i.indcod",
	$_SESSION['inuid'],
	$_SESSION['itrid']
);
$indicadores = $db->carregar( $sql_indicador );
if ( !$indicadores ) {
	$indicadores = array();
}

$sql_acao_estadual = sprintf(
	"select ai.aciid, ai.ptoid, ai.acidsc, ai.acilocalizador, i.indid, count( sai.sbaid ) as subacoes
	from cte.indicador i
	inner join cte.areadimensao a on a.ardid = i.ardid
	inner join cte.dimensao d on d.dimid = a.dimid
	inner join cte.pontuacao p on p.indid = i.indid and p.inuid = %d
	inner join cte.acaoindicador ai on ai.ptoid = p.ptoid and ai.acilocalizador = 'E'
	left join cte.subacaoindicador sai on sai.aciid = ai.aciid
	where i.indstatus = 'A' and a.ardstatus = 'A' and d.dimstatus = 'A' and d.itrid = %d
	group by  ai.aciid, ai.ptoid, ai.acidsc, ai.acilocalizador, i.indid",
	$_SESSION['inuid'],
	$_SESSION['itrid']
);
$acoes_estaduais = $db->carregar( $sql_acao_estadual );
if ( !$acoes_estaduais ) {
	$acoes_estaduais = array();
}

$sql_acao_municipal = sprintf(
	"select ai.aciid, ai.ptoid, ai.acidsc, ai.acilocalizador, i.indid, count( sai.sbaid ) as subacoes
	from cte.indicador i
	inner join cte.areadimensao a on a.ardid = i.ardid
	inner join cte.dimensao d on d.dimid = a.dimid
	inner join cte.pontuacao p on p.indid = i.indid and p.inuid = %d
	inner join cte.acaoindicador ai on ai.ptoid = p.ptoid and ai.acilocalizador = 'M'
	left join cte.subacaoindicador sai on sai.aciid = ai.aciid
	where i.indstatus = 'A' and a.ardstatus = 'A' and d.dimstatus = 'A' and d.itrid = %d
	group by  ai.aciid, ai.ptoid, ai.acidsc, ai.acilocalizador, i.indid",
	$_SESSION['inuid'],
	$_SESSION['itrid']
);
$acoes_municipais = $db->carregar( $sql_acao_municipal );
if ( !$acoes_municipais ) {
	$acoes_municipais = array();
}

$sql_subacao = sprintf(
	"select sa.sbaid, sa.sbadsc, sa.aciid
	from cte.subacaoindicador sa
	inner join cte.acaoindicador ai on ai.aciid = sa.aciid
	inner join cte.pontuacao p on p.ptoid = ai.ptoid and p.inuid = %d",
	$_SESSION['inuid'],
	$_SESSION['itrid']
);
$subacoes = $db->carregar( $sql_subacao );
if ( !$subacoes ) {
	$subacoes = array();
}

?>

<?php if( !cte_podeElaborarPlanoDeAcoes( $_SESSION['docid'] ) ): ?>
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<colgroup>
			<col/>
		</colgroup>
		<tbody>
			<tr>
				<td style="text-align:center; padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;" colspan="4">
					<img align="absmiddle" src="/imagens/atencao.png"/>
					<p>Para elaborar o Plano de Ações Articuladas é necessário finalizar o <a href="?modulo=principal/estrutura_avaliacao&acao=A" title="Exibir diagnóstico">diagnóstico</a>.</p>
				</td>
			</tr>
		</tbody>
	</table>
	<?php return; ?>
<?php endif; ?>

<script language="javascript" type="text/javascript" src="../includes/dtree/dtree.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/dtree/dtree.css">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<colgroup>
		<col/>
	</colgroup>
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;" colspan="4">
				<p>
					<a href="javascript: estrutura.openAll();">Abrir Todos</a>
					&nbsp;|&nbsp;
					<a href="javascript: estrutura.closeAll();">Fechar Todos</a>
				</p>
				<!-- <p><a href="cte.php?modulo=principal/par_acao&acao=A&ptoid=1">CADASTRAR AÇÃO</a></p> -->
				<!-- <p><a href="cte.php?modulo=principal/par_subacao&acao=A&aciid=1">CADASTRAR SUBAÇÃO</a></p> -->
				<div id="_estrutura"/>
			</td>
		</tr>
	</tbody>
</table>
<script type="text/javascript">
	
	function inserirAcaoEstadual( ptoid ){
		window.location = "?modulo=principal/par_acao&acao=A&acilocalizador=E&ptoid=" + ptoid;
	}
	
	function excluirAcaoEstadual( ptoid ){
		if ( !confirm( 'Deseja excluir a ação?' ) ) {
			return;
		}
		window.location = "?modulo=principal/par&acao=A&evento=excluir&acilocalizador=E&ptoid=" + ptoid;
	}
	
	function inserirAcaoMunicipal( ptoid ){
		window.location = "?modulo=principal/par_acao&acao=A&acilocalizador=M&ptoid=" + ptoid;
	}
	
	function excluirAcaoMunicipal( ptoid ){
		if ( !confirm( 'Deseja excluir a ação?' ) ) {
			return;
		}
		window.location = "?modulo=principal/par&acao=A&evento=excluir&acilocalizador=M&ptoid=" + ptoid;
	}
	
	function inserirSubacao( aciid ){
		window.location = "?modulo=principal/par_subacao&acao=A&aciid=" + aciid;
	}
	
	function alterarSubacao( sbaid ){
		window.location = "?modulo=principal/par_subacao&acao=A&sbaid=" + sbaid;
	}
	
	function excluirSubacao( sbaid ){
		if ( !confirm( 'Deseja excluir a subação?' ) ) {
			return;
		}
	}
	
	estrutura = new dTree( 'estrutura' );
	//estrutura.config.folderLinks = true;
	//estrutura.config.useSelection = false;
	estrutura.config.useIcons = true;
	estrutura.config.useCookies = true;
	
	estrutura.add( 0, -1, 'Plano de Ações Articuladas' );
	
	<?php foreach( $dimensoes as $dimensao ): ?>
		<?php $texto = $dimensao['dimcod'] .'. '. $dimensao['dimdsc'] ?>
		estrutura.add( 'd_<?= $dimensao['dimid'] ?>', 0, '<?= $texto ?>', 'javascript:void(0);' );
	<?php endforeach; ?>
	
	<?php foreach( $areas as $area ): ?>
		<?php $texto = $area['ardcod'] .'. '. $area['arddsc'] ?>
		estrutura.add( 'a_<?= $area['ardid'] ?>', 'd_<?= $area['dimid'] ?>', '<?= $texto ?>', 'javascript:void(0);' );
	<?php endforeach; ?>
	
	<?php foreach( $indicadores as $indicador ): ?>
		<?php
		$plano_estadual = $indicador['demandaestadual'] > 0 || in_array( $indicador['ctrpontuacao'], array(1,2) );
		$plano_municipal = $indicador['demandamunicipal'] > 0;
		if ( !$plano_estadual && !$plano_municipal ) {
			continue;
		}
		?>
		<?php $texto = $indicador['indcod'] .'. '. $indicador['inddsc'] ?>
		estrutura.add( 'i_<?= $indicador['indid'] ?>', 'a_<?= $indicador['ardid'] ?>', '<?= $texto ?>', 'javascript:void(0);' );
		
		<?php if( $plano_estadual ): ?>
			<?php $prefixo = $indicador['acaoestadual'] ? '' : '<img align="absmiddle" src="/imagens/gif_inclui.gif" style="cursor: pointer;" onclick="inserirAcaoEstadual('.$indicador['ptoid'].');" title="Inserir ação para rede estadual"/>&nbsp;'; ?>
			estrutura.add( 'ie_<?= $indicador['indid'] ?>', 'i_<?= $indicador['indid'] ?>', '<?= $prefixo ?>Ação para Rede Estadual', 'javascript:void(0);', '', '', '#', '#' );
		<?php endif; ?>
		
		<?php if( $plano_municipal ): ?>
			<?php $prefixo = $indicador['acaomunicipal'] ? '' : '<img align="absmiddle" src="/imagens/gif_inclui.gif" style="cursor: pointer;" onclick="inserirAcaoMunicipal('.$indicador['ptoid'].');" title="Inserir ação para rede municipal"/>&nbsp;'; ?>
			estrutura.add( 'im_<?= $indicador['indid'] ?>', 'i_<?= $indicador['indid'] ?>', '<?= $prefixo ?>Ação para Redes Municipais', 'javascript:void(0);', '', '', '#', '#' ); 
		<?php endif; ?>
	<?php endforeach; ?>
	
	<?php foreach( $acoes_estaduais as $acao ): ?>
		<?php
		$prefixo = '<img align="absmiddle" src="/imagens/alterar.gif" style="cursor: pointer;" onclick="inserirAcaoEstadual('.$acao['ptoid'].');" title="Alterar ação para rede estadual"/>&nbsp;';
		if ( $acao['subacoes'] == 0 ) {
			$prefixo .= '<img align="absmiddle" src="/imagens/excluir.gif" style="cursor: pointer;" onclick="excluirAcaoEstadual('.$acao['ptoid'].');" title="Excluir ação para rede estadual"/>&nbsp;';
		}
		?>
		estrutura.add( 'ai_<?= $acao['aciid'] ?>', 'ie_<?= $acao['indid'] ?>', '<?= $prefixo ?><?= $acao['acidsc'] ?>', 'javascript:void(0);', '', '', '#', '#' );
		<?php $prefixo = '<img align="absmiddle" src="/imagens/gif_inclui.gif" style="cursor: pointer;" onclick="" title="Inserir subação"/>'; ?>
		estrutura.add( '<?= microtime() ?>', 'ai_<?= $acao['aciid'] ?>', '<?= $prefixo ?> Subações (<?=  $acao['subacoes'] ?>)', 'javascript:inserirSubacao(<?= $acao['aciid'] ?>);', '', '', '#', '#' );
	<?php endforeach; ?>
	
	<?php foreach( $acoes_municipais as $acao ): ?>
		<?php
		$prefixo = '<img align="absmiddle" src="/imagens/alterar.gif" style="cursor: pointer;" onclick="inserirAcaoMunicipal('.$acao['ptoid'].');" title="Alterar ação para rede municipal"/>&nbsp;';
		if ( $acao['subacoes'] == 0 ) {
			$prefixo .= '<img align="absmiddle" src="/imagens/excluir.gif" style="cursor: pointer;" onclick="excluirAcaoMunicipal('.$acao['ptoid'].');" title="Excluir ação para rede municipal"/>&nbsp;';
		}
		?>
		estrutura.add( 'ai_<?= $acao['aciid'] ?>', 'im_<?= $acao['indid'] ?>', '<?= $prefixo ?><?= $acao['acidsc'] ?>', 'javascript:void(0);', '', '', '#', '#' );
		<?php $prefixo = '<img align="absmiddle" src="/imagens/gif_inclui.gif" style="cursor: pointer;" onclick="" title="Inserir subação"/>'; ?>
		estrutura.add( 'sb_ai_<?= $acao['aciid'] ?>', 'ai_<?= $acao['aciid'] ?>', '<?= $prefixo ?> Subações (<?=  $acao['subacoes'] ?>)', 'javascript:inserirSubacao(<?= $acao['aciid'] ?>);', '', '', '#', '#' );
	<?php endforeach; ?>
	
	<?php foreach( $subacoes as $subacao ): ?>
		<?php
		$prefixo = ''
			. '<img align="absmiddle" src="/imagens/alterar.gif" style="cursor: pointer;" onclick="alterarSubacao('.$subacao['sbaid'].');" title="Alterar subação"/>&nbsp;'
			. '<img align="absmiddle" src="/imagens/excluir.gif" style="cursor: pointer;" onclick="excluirSubacao('.$subacao['sbaid'].');" title="Excluir subação"/>&nbsp;';
		?>
		estrutura.add( 'sa_<?= $subacao['sbaid'] ?>', 'sb_ai_<?= $subacao['aciid'] ?>', '<?= $prefixo ?><?= $subacao['sbadsc'] ?>', 'javascript:void(0);', '', '', '#', '#' );
	<?php endforeach; ?>
	
	document.getElementById( '_estrutura' ).innerHTML = estrutura;
	
</script>