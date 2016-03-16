<script>
function relatorioFinalInstitucional(iusd,fpbid) {
		window.open('sisfor.php?modulo=principal/mec/mec&acao=A&requisicao=exibirRelatorioFinalInstitucional&iusd='+iusd+'&fpbid='+fpbid,'imagem','scrollbars=yes,width=1280,height=1024,resizable=yes'); 
}
</script>

<form method="post" name="formulario" id="formulario">

	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="subtituloDireita">Universidade:</td>
			<td>
			<?php
			
			$sql = "SELECT 	DISTINCT ent.entunicod as codigo, COALESCE(ent.entsig||' - ','')||ent.entnome as descricao
					FROM 		entidade.entidade ent
					INNER JOIN 	entidade.funcaoentidade fun ON fun.entid = ent.entid AND funid IN (12,11) 
					INNER JOIN sisfor.sisfories si ON si.unicod = ent.entunicod 
					WHERE ent.entstatus='A' AND ent.entunicod IS NOT NULL 
					ORDER BY 2";
			
			$db->monta_combo('unicod', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'unicod', '', $_REQUEST['unicod']); 
			?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Período de Referência:</td>
			<td>
			<?
			$sql = "SELECT f.fpbid as codigo, 'Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia as descricao
					FROM sisfor.folhapagamento f
					INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
					WHERE to_char(NOW(),'YYYY-mm-dd')::date>=(fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date AND fpbanoreferencia>='2014' ORDER BY (fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date";
			
			$db->monta_combo('fpbid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'fpbid', '', $_REQUEST['fpbid']);
				
			?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Situação:</td>
			<td>
			<?
			$sql = "SELECT esdid as codigo, esddsc as descricao
					FROM workflow.estadodocumento
					WHERE tpdid='".WF_RELATORIO_MENSAL."' ORDER BY esdordem";
			
			$db->monta_combo('esdid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'esdid', '', $_REQUEST['esdid']);
				
			?>
			</td>
		</tr>
		<tr>
			<td align="center" bgcolor="#DCDCDC" colspan="2">
				<input type="submit" value="Pesquisar" id="btnPesquisar"/>
				<input type="button" value="Ver todos" id="btnTodos" onclick="window.location='sisfor.php?modulo=principal/mec/mec&acao=A&aba=listarelatoriomensal';"/>
			</td>
		</tr>
	</table>

</form>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td colspan="2">
	<? 
	if($_REQUEST['esdid']) {
		$wh[] = "e.esdid='".$_REQUEST['esdid']."'";
	}
	
	
	if($_REQUEST['unicod']) {
		$wh[] = "ss.unicod='".$_REQUEST['unicod']."'";
	}
	
	if($_REQUEST['fpbid']) {
		$wh[] = "f.fpbid='".$_REQUEST['fpbid']."'";
	}
	
	$sql = "select '<img src=\"../imagens/consultar.gif\" border=\"0\" style=\"cursor:pointer;\" onclick=\"relatorioFinalInstitucional('||r.iusd||','||r.fpbid||');\">' as acao, i.iusnome, 'Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia as periodo, e.esddsc, uu.uniabrev||' - '||uu.unidsc as ies from sisfor.relatoriomensal r 
			inner join sisfor.identificacaousuario i on i.iusd = r.iusd 
			inner join sisfor.folhapagamento f on f.fpbid = r.fpbid 
			inner join public.meses m ON m.mescod::integer = f.fpbmesreferencia
			left join sisfor.sisfories ss on ss.usucpf = i.iuscpf 
			left join public.unidade uu on uu.unicod = ss.unicod 
			inner join workflow.documento d on d.docid = r.docid 
			inner join workflow.estadodocumento e on e.esdid = d.esdid 
			where fpbanoreferencia>='2015'".(($wh)?" and ".implode(" and ",$wh):"")." 
			order by i.iusnome, f.fpbid";
	
	$cabecalho = array("&nbsp;","Nome","Período de referência","Situação","IES");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);
	
	?>
	</td>
</tr>
</table>