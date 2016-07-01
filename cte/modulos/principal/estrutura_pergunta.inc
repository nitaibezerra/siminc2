<?php

verifica_sessao();

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo_cte( $titulo_modulo );

$sql_dimensao = sprintf(
	"select d.dimid, d.dimcod, d.dimdsc
	from cte.dimensao d
	where d.dimstatus = 'A' and d.itrid = %d
	and exists ( select p.prgid from cte.pergunta p where p.dimid = d.dimid )
	order by d.dimcod",
	$_SESSION['itrid']
);
$dimensoes = $db->carregar( $sql_dimensao );
if ( !$dimensoes ) {
	$dimensoes = array();
}

$sql_pergunta = sprintf(
	"select p.prgid, p.prgdsc, p.prgcod, d.dimid, r.rspid
	from cte.pergunta p
	inner join cte.dimensao d on d.dimid = p.dimid
	left join cte.resposta r on r.prgid = p.prgid and r.inuid = %d
	where p.prgstatus = 'A' and d.dimstatus = 'A'
	order by p.prgcod",
	$_SESSION['inuid']
);
$perguntas = $db->carregar( $sql_pergunta );
if ( !$perguntas ) {
	$perguntas = array();
}

?>
<script language="javascript" type="text/javascript" src="../includes/dtree/dtree.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/dtree/dtree.css">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<colgroup>
		<col/>
	</colgroup>
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#fafafa; color:#404040;" colspan="4">
				<p>
					<a href="javascript: arvore.openAll();">Abrir Todos</a>
					&nbsp;|&nbsp;
					<a href="javascript: arvore.closeAll();">Fechar Todos</a>
				</p>
				<div id="estrutura"/>
			</td>
		</tr>
	</tbody>
</table>
<script type="text/javascript">
	
	function responder( dimensao, pergunta ){
		window.location = '?modulo=principal/questoes/questoespontuais&acao=C&dimid='+ dimensao +'&prgid='+ pergunta;
	}
	
	arvore = new dTree( 'arvore' );
	arvore.config.folderLinks = false;
	arvore.config.useIcons = true;
	arvore.config.useCookies = true;
	
	<?php
	$sql = sprintf( "select itrdsc from cte.instrumento where itrid = %d", $_SESSION['itrid'] );
	$instrumento = $db->pegaUm( $sql );
	?>
	arvore.add( 0, -1, '<b><?= $instrumento ?></b>' );
	
	<?php foreach( $dimensoes as $dimensao ): ?>
		<?php $texto = '<span style="color: #000000;"><b>'. $dimensao['dimcod'] .'. '. $dimensao['dimdsc'] .'</b></span>'; ?>
		arvore.add( 'd_<?= $dimensao['dimid'] ?>', 0, '<?= $texto ?>' );
	<?php endforeach; ?>
	
	<?php foreach( $perguntas as $pergunta ): ?>
		<?php
			$icone= $pergunta['rspid'] ? '../imagens/check_p.gif' : '';
			$cor = $icone ? '#959595' : '#133368';
			$texto = '<span style="color: '. $cor .';">'. $pergunta['prgcod'] .'. '. $pergunta['prgdsc'] .'</span>';
		?>
		arvore.add( 'i_<?= $pergunta['prgid'] ?>', 'd_<?= $pergunta['dimid'] ?>', '<?= $texto ?>', 'javascript: responder(<?= $pergunta['dimid'] ?>,<?= $pergunta['prgid'] ?>);', '', '', '<?= $icone ?>' );
	<?php endforeach; ?>
	
	elemento = document.getElementById( 'estrutura' );
	elemento.innerHTML = arvore;
</script>