<? if(!$versaoimpressao) : ?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type=text/javascript src=/includes/prototype.js></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>

<script>

function wf_exibirHistorico( docid )
{
	var url = 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/historico.php' +
		'?modulo=principal/tramitacao' +
		'&acao=C' +
		'&docid=' + docid;
	window.open(
		url,
		'alterarEstado',
		'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
	);
}

function detalhesPagamentos(sifid, fpbid) {

	ajaxatualizar('requisicao=exibirDetalhesPagamentos&sifid='+sifid+'&fpbid='+fpbid,'modalDv');
	
	jQuery("#modalDv").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 1024,
	                        height: 720,
	                        modal: true,
	                     	close: function(){} 
	                    });
	
}

function salvarRelatorioMensal() {

	if(jQuery('#remcargo').val()=='') {
		alert('Cargo em branco');
		return false;
	}

	if(jQuery('#remunidadelotacao').val()=='') {
		alert('Unidade de lotação em branco');
		return false;
	}

	if(jQuery('#remlinkcurriculolattes').val()=='') {
		alert('Link para Currículo Lattes em branco');
		return false;
	}

	if(jQuery('#remobssituacoescursos').val()=='') {
		alert('4.1 COMENTÁRIOS À SITUAÇÃO DOS CURSOS, DAS BOLSAS E/OU DOS CURSISTAS em branco');
		return false;
	}

	if(jQuery('#remobsexecucaofinanceira').val()=='') {
		alert('5.3 COMENTÁRIOS À EXECUÇÃO FINANCEIRA em branco');
		return false;
	}

	if(jQuery('#remsugestoes').val()=='') {
		alert('SUGESTÕES em branco');
		return false;
	}

	if(jQuery('#remoutroscomentario').val()=='') {
		alert('OUTROS COMENTÁRIOS em branco');
		return false;
	}

	if(jQuery('#termo').attr('checked')==false) {
		alert('Marque a declaração');
		return false;
	}
	
	jQuery('#relatoriomensal').submit();
}

function carregaUsuario_(esp) {
	var usucpf=document.getElementById('iuscpf'+esp).value;

	usucpf = usucpf.replace('-','');
	usucpf = usucpf.replace('.','');
	usucpf = usucpf.replace('.','');
				
	var comp = new dCPF();
	comp.buscarDados(usucpf);
	var arrDados = new Object();
	
	if(!comp.dados.no_pessoa_rf){
		alert('CPF Inválido');
		return false;
	}
	
	document.getElementById('iusnome'+esp).value=comp.dados.no_pessoa_rf;
	
	divCarregado();
}


</script>

<div id="modalDv" style="display:none;"></div>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td class="SubTituloCentro" colspan="2">Relatório Mensal</td>
    </tr>
    <tr>
        <td colspan="2" class="SubTituloCentro">
<?
$sql = "SELECT f.fpbid as codigo, 'Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia as descricao
			FROM sisfor.folhapagamento f
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
			WHERE to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd') AND fpbanoreferencia>='2015' ORDER BY (fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date";

$db->monta_combo('fpbid', $sql, 'S', 'Selecione', 'selecionarPeriodoReferencia', '', '', '', 'S', 'fpbid', '', $_REQUEST['fpbid']);

?>
        </td>
    </tr>
</table>

<? else : ?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<? endif; ?>

<? if($_REQUEST['fpbid']) : ?>

<?

if($_REQUEST['iusd']) {
	$iusd = $_REQUEST['iusd'];
	$iuscpf   = $db->pegaUm("SELECT iuscpf FROM sisfor.identificacaousuario WHERE iusd='".$_REQUEST['iusd']."'");
} else {
	$iusd   = $db->pegaUm("SELECT iusd FROM sisfor.identificacaousuario WHERE iuscpf='".$_SESSION['usucpf']."'");
	$iuscpf = $_SESSION['usucpf'];
}

if($iusd) {

	$relatoriomensal = $db->pegaLinha("SELECT * FROM sisfor.relatoriomensal WHERE fpbid='".$_REQUEST['fpbid']."' and iusd='{$iusd}'");
	
	$periodoReferencia = $db->pegaUm("SELECT m.mesdsc || ' / ' || fpbanoreferencia as descricao
										FROM sisfor.folhapagamento f
										INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
										WHERE f.fpbid='" . $_REQUEST['fpbid'] . "'");
	
	if($relatoriomensal) {
		$termo = true;
		extract($relatoriomensal);
		$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid='".$docid."'");
		if($esdid!=ESD_RELATORIOMENSAL_EMELABORACAO) $exibirhtml = true;
	}

	if(!$remid) {

		$docid = wf_cadastrarDocumento(WF_RELATORIO_MENSAL, "Relatório Mensal - fpb: {$_REQUEST['fpbid']} - ius: {$iusd}");
	
		$sql = "INSERT INTO sisfor.relatoriomensal(
		            fpbid, iusd, docid)
		    		VALUES ('".$_REQUEST['fpbid']."', '{$iusd}', '{$docid}') RETURNING remid;";
		
		$remid = $db->pegaUm($sql);
		$db->commit();
	
	}
}

$dadosgerais = $db->pegaLinha("SELECT u.unicod||' - '||u.unidsc as instituicao, i.iusnome, i.iusemailprincipal, (SELECT '('||itedddtel||') '||itenumtel FROM sisfor.identificacaotelefone WHERE iusd=i.iusd AND itetipo='T') as telefone FROM sisfor.sisfories s
								INNER JOIN public.unidade u ON u.unicod = s.unicod 
								INNER JOIN sisfor.identificacaousuario i ON i.iuscpf = s.usucpf
								WHERE s.usucpf='".$iuscpf."'");

if(!$dadosgerais) :
 echo '<p align=center style=font-size:large;>Usuário não possui perfil de Coordenador Institucional</p>';
else :

?>
<script>

<? if(!$exibirhtml) : ?>
jQuery(document).ready(function() {
	jQuery("[id^='participantes_']").each(function() {
		jQuery(this).click();
	});
});
<? endif; ?>


function inserirAtividade() {
	if(jQuery('#rmaid').val()=='') {
		alert('Atividade não selecionada');
		return false;
	}

	ajaxatualizar('requisicao=carregarRelatorioMensalAtividades&remid=<?=$remid ?>&rmaid='+jQuery('#rmaid').val(), 'dv_relatoriomensalatividade');

	jQuery("[id^='participantes_']").each(function() {
		jQuery(this).click();
	});
	
	
}

function adicionarParticipante(rmrid) {
	if(jQuery('#iuscpf__'+rmrid).val()=='') {
		alert('CPF em branco');
		return false;	
	}

	if(jQuery('#iusnome__'+rmrid).val()=='') {
		alert('Nome em branco');
		return false;	
	}

	if(jQuery('#iuscargo__'+rmrid).val()=='') {
		alert('Cargo em branco');
		return false;	
	}

	ajaxatualizar('requisicao=carregarRelatorioMensalParticipante&rmrid='+rmrid+'&repcpf='+jQuery('#iuscpf__'+rmrid).val()+'&repnome='+jQuery('#iusnome__'+rmrid).val()+'&repcargo='+jQuery('#iuscargo__'+rmrid).val(),'td_listaparticipantes_'+rmrid);
	
}

function excluirAtividades(rmrid) {
	var conf = confirm('Deseja realmente excluir esta atividade?');
	
	if(conf) {
		ajaxatualizar('requisicao=carregarRelatorioMensalParticipante&remid=<?=$remid ?>&repid='+repid,'td_listaparticipantes');
	}
}

function excluirParticipantes(repid) {
	var conf = confirm('Deseja realmente excluir este participante?');
	
	if(conf) {
		ajaxatualizar('requisicao=carregarRelatorioMensalParticipante&remid=<?=$remid ?>&repid='+repid,'td_listaparticipantes');
	}
}

function carregarParticipantes(obj, rmrid) {

	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;
	
	var nlinha   = tabela.insertRow(linha.rowIndex);
	var ncol     = nlinha.insertCell(0);
	ncol.colSpan = 4;
	ncol.id      = 'coluna_'+rmrid;

	ajaxatualizar('requisicao=carregarBlocoParticipantes<?=(($versaoimpressao)?"&consulta=1":"")?>&rmrid='+rmrid,'coluna_'+rmrid);

	
}

</script>

<form method=post name="relatoriomensal" id="relatoriomensal">
<input type="hidden" name="requisicao" value="salvarRelatorioMensal">
<input type="hidden" name="remid" value="<?=$remid ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td>

<?
if($exibirhtml && !$wf_func) :

	echo $remversaoanalise;

else :
?>
	

<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloEsquerda" colspan="2">1. DADOS GERAIS</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Instituição</td>
	<td><?=$dadosgerais['instituicao'] ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Coordenador COMFOR</td>
	<td><?=$dadosgerais['iusnome'] ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Período de Referência</td>
	<td><?=$periodoReferencia ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Cargo</td>
	<td>
	<? 
	if(!$versaoimpressao) {
		echo campo_texto('remcargo', 'N', 'S', 'Cargo', '25', '300', '', '', '', '', 0, 'id="remcargo"');
	} else {
		echo $remcargo;
	}
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Unidade de lotação</td>
	<td><?
	if(!$versaoimpressao) {
		echo campo_texto('remunidadelotacao', 'N', 'S', 'Unidade de lotação', '25', '300', '', '', '', '', 0, 'id="remunidadelotacao"');
	} else {
		echo $remunidadelotacao;
	}
	 ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Link para Currículo Lattes</td>
	<td><?
	if(!$versaoimpressao) {
		echo campo_texto('remlinkcurriculolattes', 'N', 'S', 'Link para Currículo Lattes', '25', '300', '', '', '', '', 0, 'id="remlinkcurriculolattes"');
	} else {
		echo $remlinkcurriculolattes;
	}
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">E-mail</td>
	<td><?=$dadosgerais['iusemailprincipal'] ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Telefone</td>
	<td><?=$dadosgerais['telefone'] ?></td>
</tr>
</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2">2. SITUAÇÃO DOS CURSOS</td>
</tr>
<tr>
	<td colspan="2">
	<? 
	
	$sql = "select 
			case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
				     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
				     when s.ocuid is not null then oc.ocunome 
				     when s.oatid is not null then oatnome end as curso,
			case when s.ieoid is not null then cor.coordsigla 
			     when s.cnvid is not null then cor2.coordsigla 
			     when s.ocuid is not null then cor3.coordsigla 
			     when s.oatid is not null then cor4.coordsigla end as secretaria,
			coalesce(usu.usunome,'Não cadastrado') as coordenador,
			s.sifprofmagisterio as vagas,
			(select sum(orcvlrunitario) as orcvlrunitario from sisfor.orcamento where sifid=s.sifid and orcstatus='A') as valortotal,
			to_char(htddata, 'dd/mm/YYYY HH24:MI') as htddata,
		
			(select to_char(min((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'01')::date),'mm/YYYY') ||' até '||to_char(max((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'01')::date),'mm/YYYY') from sisfor.folhapagamentoprojeto p 
			 inner join sisfor.folhapagamento f on f.fpbid = p.fpbid 
			 where sifid=s.sifid) as periodo 

		from sisfor.sisfor s
			inner join workflow.documento d on d.docid = s.docidprojeto
			left join workflow.historicodocumento h on h.hstid = d.hstid
			inner join workflow.estadodocumento e on e.esdid = d.esdid
			inner join public.unidade u on u.unicod = s.unicod
			left join workflow.documento dctur on dctur.docid = s.docidcomposicaoequipe
			left join workflow.estadodocumento ectur on ectur.esdid = dctur.esdid  
			left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
			left join catalogocurso2014.curso cur on cur.curid = ieo.curid
			left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
			left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
			left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid
			left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid
			left join seguranca.usuario usu on usu.usucpf = s.usucpf
			left join sisfor.outrocurso oc on oc.ocuid = s.ocuid
			left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid 
			left join sisfor.outraatividade oat on oat.oatid = s.oatid 
			left join catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid 
		where sifstatus='A' and s.unicod in(select unicod from sisfor.sisfories where usucpf='".$iuscpf."') ANd d.esdid='".ESD_PROJETO_VALIDADO."'  
		order by (select max((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'01')::date) from sisfor.folhapagamentoprojeto p 
			 inner join sisfor.folhapagamento f on f.fpbid = p.fpbid 
			 where sifid=s.sifid)
				";
	
	$cabecalho = array("Cursos validados em execução","Secretaria","Nome do coordenador","Vagas","Custeio","Data de tramitação","Período de Referência");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);
	
	?>
	</td>
</tr>
</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2">3. SITUAÇÃO DAS BOLSAS DE ESTUDO E PESQUISA</td>
</tr>
<tr>
	<td colspan="2">
	<? 
	
$sql = "select 
            case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
			     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
			     when s.ocuid is not null then oc.ocunome 
			     when s.oatid is not null then oatnome end as curdesc, 
               	to_char((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date,'mm/YYYY') as mesanoini,
			  	esd.esddsc,
				(select count(*) as total from sisfor.mensario m 
				 inner join sisfor.tipoperfil t on t.tpeid = m.tpeid 
				 inner join sisfor.mensarioavaliacoes ma on ma.menid = m.menid 
				 where t.sifid = s.sifid and m.fpbid = f.fpbid and mavatividadesrealizadas='A') as qtdbolsasaptas 
        from sisfor.sisfor s
                inner join public.unidade u on u.unicod = s.unicod and u.unitpocod = s.unitpocod 
                left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid 
                left join catalogocurso2014.curso cur on cur.curid = ieo.curid AND cur.curstatus = 'A'
                left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
                left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid 
                left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid AND cur2.curstatus = 'A' 
                left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid 
                left join seguranca.usuario usu on usu.usucpf = s.usucpf 
                left join sisfor.outrocurso oc on oc.ocuid = s.ocuid 
                left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid 
				left join sisfor.outraatividade oat on oat.oatid = s.oatid 
				left join catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid 
                inner join sisfor.folhapagamentoprojeto fpp on s.sifid = fpp.sifid
                INNER JOIN sisfor.folhapagamento f ON f.fpbid = fpp.fpbid
                inner join workflow.documento doc on doc.docid = fpp.docid 
				inner join workflow.estadodocumento esd on esd.esdid = doc.esdid 
				left join workflow.historicodocumento hst on hst.hstid = doc.hstid 
				left join seguranca.usuario uu on uu.usucpf = hst.usucpf 
                INNER JOIN sisfor.identificacaousuario i ON i.iuscpf=s.usucpf
            where doc.esdid in( ".ESD_ANALISE_MEC.", ".ESD_ENVIADO_PAGAMENTO.",".ESD_ANALISE_COORDENADORINSTITUCIONAL.",".ESD_AVALIACAO.") and u.unicod in (select si.unicod from sisfor.sisfories si 
						inner join sisfor.tipoperfil tp on tp.tpeid = si.tpeid 
						inner join sisfor.identificacaousuario i on i.iusd = tp.iusd 
						where i.iuscpf='".$iuscpf."') and f.fpbid='".$_REQUEST['fpbid']."'
			order by curdesc, mesanoini, hst.htddata";
	
	
	$cabecalho = array("Curso","Período de Referência","Situação","Bolsas solicitadas");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);
	
	echo "<br>";
	echo "<p><b>Síntese - SITUAÇÃO DAS BOLSAS DE ESTUDO E PESQUISA</b></p>";
	
	$sql = "select 
				 esd.esddsc, count(*) as qtd
        from sisfor.sisfor s
                inner join public.unidade u on u.unicod = s.unicod and u.unitpocod = s.unitpocod 
                inner join sisfor.folhapagamentoprojeto fpp on s.sifid = fpp.sifid
                INNER JOIN sisfor.folhapagamento f ON f.fpbid = fpp.fpbid
                inner join workflow.documento doc on doc.docid = fpp.docid 
				inner join workflow.estadodocumento esd on esd.esdid = doc.esdid 
				left join workflow.historicodocumento hst on hst.hstid = doc.hstid 
            where doc.esdid in( ".ESD_ANALISE_MEC.", ".ESD_ENVIADO_PAGAMENTO.",".ESD_ANALISE_COORDENADORINSTITUCIONAL.",".ESD_AVALIACAO.") and u.unicod in (select si.unicod from sisfor.sisfories si 
						inner join sisfor.tipoperfil tp on tp.tpeid = si.tpeid 
						inner join sisfor.identificacaousuario i on i.iusd = tp.iusd 
						where i.iuscpf='".$iuscpf."') and f.fpbid='".$_REQUEST['fpbid']."' 
			group by esd.esddsc";
	
	$cabecalho = array("Situação","Quantidade");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','50%','',false, false, false, false);
	?>
	
	
	</td>
</tr>
</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2">4. SITUAÇÃO DOS CURSISTAS</td>
</tr>
<tr>
	<td colspan="2">
	<? 
	
	$opsituacao = opcoesComboSituacoes();
	unset($cabecalho);
	if($opsituacao) {
		$cabecalho[] = "Curso";
		foreach($opsituacao as $op) {
			$cabecalho[] = $op['descricao'];
			$cl1[] = "CASE WHEN cavsituacao='".$op['codigo']."' THEN 1 ELSE 0 END as ".$op['descricao'];
			$cl2[] = " SUM(foo.".$op['descricao'].") as s".$op['descricao'];
		}
	}
	
$sql = "select foo.curdesc,
			   ".(($cl2)?implode(",",$cl2):"")."
			
		from (
			   
		select 
            case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
			     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
			     when s.ocuid is not null then oc.ocunome 
			     when s.oatid is not null then oatnome end as curdesc, 
			".(($cl1)?implode(",",$cl1):"")."	  
        from sisfor.sisfor s
                inner join public.unidade u on u.unicod = s.unicod and u.unitpocod = s.unitpocod 
                left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid 
                left join catalogocurso2014.curso cur on cur.curid = ieo.curid AND cur.curstatus = 'A'
                left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
                left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid 
                left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid AND cur2.curstatus = 'A' 
                left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid 
                left join seguranca.usuario usu on usu.usucpf = s.usucpf 
                left join sisfor.outrocurso oc on oc.ocuid = s.ocuid 
                left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid 
				left join sisfor.outraatividade oat on oat.oatid = s.oatid 
				left join catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid 
                inner join sisfor.folhapagamentocursista fpc on s.sifid = fpc.sifid
                INNER JOIN sisfor.folhapagamento f ON f.fpbid = fpc.fpbid
                inner join workflow.documento doc on doc.docid = fpc.docid
				inner join sisfor.cursistacurso cc on cc.sifid = s.sifid and cc.fpbid = f.fpbid 
				inner join sisfor.cursistaavaliacoes ca on ca.cucid = cc.cucid   
				inner join workflow.estadodocumento esd on esd.esdid = doc.esdid 
				left join workflow.historicodocumento hst on hst.hstid = doc.hstid 
				left join seguranca.usuario uu on uu.usucpf = hst.usucpf 
                INNER JOIN sisfor.identificacaousuario i ON i.iuscpf=s.usucpf
            where u.unicod in (select si.unicod from sisfor.sisfories si 
						inner join sisfor.tipoperfil tp on tp.tpeid = si.tpeid 
						inner join sisfor.identificacaousuario i on i.iusd = tp.iusd 
						where i.iuscpf='".$iuscpf."') and f.fpbid='".$_REQUEST['fpbid']."'
			) foo 
			group by foo.curdesc 
			order by foo.curdesc";
	
	
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%','',true, false, false, true);

	?>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">4.1. COMENTÁRIOS À SITUAÇÃO DOS CURSOS, DAS BOLSAS E/OU DOS CURSISTAS</td>
</tr>
<tr>
	<td colspan="2">
	<? 
	if(!$versaoimpressao) {
		echo campo_textarea( 'remobssituacoescursos', 'S', 'S', '', '150', '15', '2000');
	} else {
		echo nl2br($remobssituacoescursos);
	} 
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">5. EXECUÇÃO FINANCEIRA</td>
</tr>
<tr>
	<td colspan="2">
	<p align="center"><b>AÇÃO : 20RJ - Apoio à Capacitação e Formação Inicial e Continuada de Professores, Profissionais, Funcionários e Gestores para a Educação Básica</b></p>
	<? 
	
	$sql = "SELECT g.gdeid, UPPER(g.gdedesc) as gdedesc, sum(o.orcvlrunitario) as totalprevisto FROM sisfor.orcamento o
			INNER JOIN sisfor.grupodespesa g ON g.gdeid = o.gdeid 
			INNER JOIN sisfor.sisfor s ON s.sifid = o.sifid 
			WHERE s.unicod in(select si.unicod from sisfor.sisfories si 
							inner join sisfor.tipoperfil tp on tp.tpeid = si.tpeid 
							inner join sisfor.identificacaousuario i on i.iusd = tp.iusd 
							where i.iuscpf='".$iuscpf."') and o.orcstatus='A' 
			GROUP BY g.gdedesc, g.gdeid";
	
	$listaorcamento = $db->carregar($sql);

	$unicod = $db->pegaUm("select si.unicod from sisfor.sisfories si 
							inner join sisfor.tipoperfil tp on tp.tpeid = si.tpeid 
							inner join sisfor.identificacaousuario i on i.iusd = tp.iusd 
							where i.iuscpf='".$iuscpf."'");

	$acao   = "20RJ";
	
	// Parâmetros para a nova conexão com o banco do SIAFI
	$servidor_bd = $servidor_bd_siafi;
	$porta_bd    = $porta_bd_siafi;
	$nome_bd     = $nome_bd_siafi;
	$usuario_db  = $usuario_db_siafi;
	$senha_bd    = $senha_bd_siafi;
	
	// Cria o novo objeto de conexão
	$db2 = new cls_banco();
	
	$sql = "SELECT
					natureza AS cod_agrupador3,
					natureza_desc AS dsc_agrupador3,
					sum(valor1) AS coluna1,sum(valor2) AS coluna2,sum(valor3) AS coluna3,sum(valor4) AS coluna4,sum(valor5) AS coluna5
					FROM
					(SELECT 
					   sld.unicod,sld.acacod,sld.ctecod || '.' || sld.gndcod || '.' || sld.mapcod || '.' || sld.edpcod AS natureza,
					   uni.unidsc,aca.acadsc,ndp.ndpdsc AS natureza_desc,
					   CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192190101','192190109') THEN (sld.sldvalor) ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302') THEN (sld.sldvalor) ELSE 0 END AS valor2,CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN (sld.sldvalor) ELSE 0 END AS valor3,CASE WHEN sld.sldcontacontabil in ('292130201','292130202','292130301') THEN (sld.sldvalor) ELSE 0 END AS valor4,CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN (sld.sldvalor) ELSE 0 END AS valor5
				       FROM
					   dw.saldo2014 sld
					   LEFT JOIN dw.uo uni ON uni.unicod = sld.unicod LEFT JOIN dw.acao aca ON aca.acacod = sld.acacod LEFT JOIN ( select distinct ctecod, gndcod, mapcod, edpcod, ndpdsc from dw.naturezadespesa where sbecod = '00' ) as ndp ON cast(ndp.ctecod AS text) = sld.ctecod AND cast(ndp.gndcod AS text) = sld.gndcod AND cast(ndp.mapcod AS text) = sld.mapcod AND cast(ndp.edpcod AS text) = sld.edpcod
					   WHERE sld.unicod in ('{$unicod}') AND sld.acacod in ('{$acao}') AND sld.sldcontacontabil in ('192110101','192110201','192110209','192190101','192190109','192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302','292130100','292130201','292130202','292130203','292130301','292130201','292130202','292130301','292130301','292410403')) as foo
					WHERE
					valor1 <> 0 OR valor2 <> 0 OR valor3 <> 0 OR valor4 <> 0 OR valor5 <> 0
					GROUP BY
					unicod,acacod,natureza,
					unidsc,acadsc,natureza_desc
					ORDER BY
					unicod,acacod,natureza,
					unidsc,acadsc,natureza_desc";
	
	$listasiafi = $db2->carregar($sql);
	
	if($listasiafi[0]) {
		foreach($listasiafi as $siafi) {
			$arrSIAFI[$siafi['cod_agrupador3']] = $siafi;
		}
	}
	
	$_DEPARA = array('1'=>'3.3.90.33','339147'=>'3.3.90.30','339039'=>'3.3.90.14');
	
	if($listaorcamento) {
		foreach($listaorcamento as $orc) {
			$arrORC[] = array('codsiafi'=>$arrSIAFI[$_DEPARA[$orc['gdeid']]]['cod_agrupador3'].'&nbsp;','gdedesc'=>$orc['gdedesc'],'totalprevisto'=>$orc['totalprevisto'],(($arrSIAFI[$_DEPARA[$orc['gdeid']]]['coluna1'])?$arrSIAFI[$_DEPARA[$orc['gdeid']]]['coluna1']:"0,00"),(($arrSIAFI[$_DEPARA[$orc['gdeid']]]['coluna2'])?$arrSIAFI[$_DEPARA[$orc['gdeid']]]['coluna2']:"0,00"),(($arrSIAFI[$_DEPARA[$orc['gdeid']]]['coluna3'])?$arrSIAFI[$_DEPARA[$orc['gdeid']]]['coluna3']:"0,00"),(($arrSIAFI[$_DEPARA[$orc['gdeid']]]['coluna4'])?$arrSIAFI[$_DEPARA[$orc['gdeid']]]['coluna4']:"0,00"),(($arrSIAFI[$_DEPARA[$orc['gdeid']]]['coluna5'])?$arrSIAFI[$_DEPARA[$orc['gdeid']]]['coluna5']:"0,00"));
			unset($arrSIAFI[$_DEPARA[$orc['gdeid']]]);
		}
	}
	
	if($arrSIAFI) {
		foreach($arrSIAFI as $siaf) {
			$arrORC[] = array('codsiafi'=>$siaf['cod_agrupador3'].'&nbsp;','gdedesc'=>$siaf['dsc_agrupador3'],'totalprevisto'=>"0,00",(($siaf['coluna1'])?$siaf['coluna1']:"0,00"),(($siaf['coluna2'])?$siaf['coluna2']:"0,00"),(($siaf['coluna3'])?$siaf['coluna3']:"0,00"),(($siaf['coluna4'])?$siaf['coluna4']:"0,00"),(($siaf['coluna5'])?$siaf['coluna5']:"0,00"));
		}
	}
	
	if(!$arrORC) $arrORC = array();
	
	$cabecalho = array("Código","Natureza de Despesa","Orçamento Previsto","Dotação inicial","Dotação atualizada","Despesas empenhadas","Despesas liquidadas","Valores pagos");
	$db->monta_lista_simples($arrORC,$cabecalho,1000,5,'S','100%','S',false, false, false, false);

	?>
	</td>
</tr>

<tr>
	<td class="SubTituloEsquerda" colspan="2">5.3 COMENTÁRIOS À EXECUÇÃO FINANCEIRA</td>
</tr>
<tr>
	<td colspan="2">
	<?
	if(!$versaoimpressao) {
		echo campo_textarea( 'remobsexecucaofinanceira', 'S', 'S', '', '150', '15', '2000');
	} else {
		echo nl2br($remobsexecucaofinanceira);
	} 
	?>
	</td>
</tr>

<tr>
	<td class="SubTituloEsquerda" colspan="2">6. COMITÊ GESTOR INSTITUCIONAL</td>
</tr>
<tr>
	<td colspan="2">
	<? if(!$versaoimpressao) : ?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita">Atividade:</td>
		<td>
		<?
		$sql = "SELECT rmaid as codigo, rmadsc as descricao FROM sisfor.relatoriomensalatividades ORDER BY rmaid";
		$db->monta_combo('rmaid', $sql, 'S', 'Selecione', '', '', '', '400', 'S', 'rmaid', '');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">&nbsp;</td>
		<td><input type="button" name="inserir" value="Inserir atividade" onclick="inserirAtividade();"></td>
	</tr>
	</table>
	<? endif; ?>
	
	<div id="dv_relatoriomensalatividade">
	<? carregarRelatorioMensalAtividades(array('remid'=>$remid,'consulta'=>(($versaoimpressao)?true:false))); ?>
	</div>
	
	</td>
</tr>

<tr>
	<td class="SubTituloEsquerda" colspan="2">OUTROS COMENTÁRIOS</td>
</tr>
<tr>
	<td colspan="2">
	<? 
	if(!$versaoimpressao) {
		echo campo_textarea( 'remoutroscomentario', 'S', 'S', '', '150', '15', '2000'); 
	} else {
		echo nl2br($remoutroscomentario);
	}
	?>
	</td>
</tr>
<tr>
	<td colspan="2">
	<p><input type="checkbox" id="termo" name="termo" value="1" <?=(($termo)?"checked":"")?>> Declaro que as informações constantes deste Relatório descrevem as atividades por mim realizadas neste período, em atendimento ao disposto no art. 6º, inciso II, alínea “e” da Resolução CD/FNDE nº 23, de 24/10/2014.</p>
	</td>
</tr>
<? if(!$versaoimpressao) : ?>
<tr>
	<td colspan="2" class="SubTituloCentro">
	<input type="button" name="salvar" value="Salvar" onclick="salvarRelatorioMensal();">
	</td>
</tr>
<? endif; ?>

</table>

<? endif; ?>
	

	</td>
	<? if(!$versaoimpressao) : ?>
	<td valign="top"><? wf_desenhaBarraNavegacao( $docid, array('remid' => $remid) ); ?></td>
	<? else :?>
	<td>&nbsp;</td>
	<? endif; ?>
</tr>
</table>

</form>

<? endif; ?>

<? endif; ?>