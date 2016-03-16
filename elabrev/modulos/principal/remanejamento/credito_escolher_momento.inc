<?php

$mcrid = (integer) $_REQUEST['mcrid'];

//Forca resultado para perfil Consulta UO
//$_REQUEST['escolher'] = 1;
//$mcrid = 35;

if ( $mcrid )
{
	unset( $_SESSION['creditos_adicionais_tcrid'] );
	unset( $_SESSION['creditos_adicionais_unicod'] );
	$_SESSION['credito_mcrid'] = $mcrid;
	header( 'Location: ?modulo=principal/remanejamento/credito&acao=A' );
	exit();
}

$ano = $_SESSION['exercicio'];

// captura somente os momentos atuais
/*** Verifica se perfil do usuário é de Super usuário, CGO / Coordenador de Orçamento ou CGO / Equipe Orçamentária ***/
$pflcods = array(SUPER_USUARIO,CGO_COORDENADOR_ORCAMENTO,CGO_EQUIPE_ORCAMENTARIA);
/*** Se for exibe todos os momentocredito ***/ 
if( possuiPerfil( $pflcods ) ){
	$sql_momento = "select 
			m.mcrid as codigo,
			m.mcrdsc as descricao
		from elabrev.momentocredito m
		where
			(
				( m.mcrdatainiciocgo < now() and m.mcrdatafimcgo > now() )
				or
				( m.mcrdatainiciouo < now() and m.mcrdatafimuo > now() )
			)
			and
			m.mcrstatus = 'A' and m.mcrano = '$ano'
		order by
			m.mcrdatainiciocgo desc";
	
	// captura a quantidade somente os momentos atuais
	$sql_qtd_momento = "select
			count(*)
		from elabrev.momentocredito
		where
			(
				( mcrdatainiciocgo < now() and mcrdatafimcgo > now() )
				or
				( mcrdatainiciouo < now() and mcrdatafimuo > now() )
			)
			and
			mcrstatus = 'A' and mcrano = '$ano'";
	$qtd_momento = $db->pegaUm( $sql_qtd_momento );
	
	
}
else{
	$sql_momento = "select 
			m.mcrid as codigo,
			m.mcrdsc as descricao
		from elabrev.momentocredito m
		inner join elabrev.momentocreditounid mu ON mu.mcrid = m.mcrid
		inner join elabrev.usuarioresponsabilidade ur ON ur.unicod = mu.unicod and ur.rpustatus = 'A' and ur.usucpf = '".$_SESSION['usucpf']."' and ur.prsano = '".$_SESSION['exercicio']."'
		where
			(
				( m.mcrdatainiciocgo < now() and m.mcrdatafimcgo >= now() )
				or
				( m.mcrdatainiciouo < now() and m.mcrdatafimuo >= now() )
			)
			and
			m.mcrstatus = 'A' and m.mcrano = '$ano'
		group by 
			m.mcrid, m.mcrdsc, m.mcrdatainiciocgo
		order by
			m.mcrdatainiciocgo desc";
	//dbg($sql_momento,1);
	
	// captura a quantidade somente os momentos atuais
	$sql_qtd_momento = "select
			count(m.mcrid)
		from elabrev.momentocredito m
		inner join elabrev.momentocreditounid mu ON mu.mcrid = m.mcrid
		inner join elabrev.usuarioresponsabilidade ur ON ur.unicod = mu.unicod and ur.rpustatus = 'A' and ur.usucpf = '".$_SESSION['usucpf']."' and ur.prsano = '".$_SESSION['exercicio']."'
		where
			(
				( m.mcrdatainiciocgo < now() and m.mcrdatafimcgo > now() )
				or
				( m.mcrdatainiciouo < now() and m.mcrdatafimuo > now() )
			)
			and
			m.mcrstatus = 'A' and m.mcrano = '$ano'";
	$qtd_momento = $db->pegaUm( $sql_qtd_momento );
	
	
}



include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( $titulo_modulo, 'Suplementação e Cancelamento de Créditos Adicionais' );

?>
<form action="" method="post" name="escolher_momento">
	<input type="hidden" name="escolher" value="1"/>
	<table width="90%" class="tabela" cellspacing="1"  cellpadding="3" bgcolor="#f5f5f5" align="center">
		<?php if ( $qtd_momento ) : ?>
			<tr>
				<td width="20%" align="right" class="SubTituloDireita">
					Momento
				</td>
				<td width="80%">
					<?= $db->monta_combo( 'mcrid', $sql_momento, 'S', '', '', '', '', '', '', '' ) ?>
				</td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita">
					&nbsp;
				</td>
				<td>
					<input type="submit" name="enviar" value="Avançar"/>
				</td>
			</tr>
		<?php else : ?>
			<tr>
				<td colspan="2" align="center" style="color: #bb3030;">
					Nenhum momento está disponível para a data atual
				</td>
			</tr>
		<?php endif; ?>
	</table>
</form>
<?php if ( $qtd_momento == 1 ) : ?>
	<script type="text/javascript">
		document.escolher_momento.submit();
	</script>
<?php endif; ?>
