<?php
if($_SESSION['sisid'] == 23){
	$sql = " select 
			 	count(ed.emdid) 
			 from 
			 	emenda.ptemendadetalheentidade pt
			    inner join emenda.emendadetalheentidade ede on ede.edeid = pt.edeid
			    inner join emenda.emendadetalhe ed on ed.emdid = ede.emdid
			    inner join emenda.emenda e on e.emeid = ed.emeid
			 where
			 	pt.ptrid = $ptrid
			 	and e.etoid = 4
			    and ede.edestatus = 'A'";
	$mod = $db->pegaUm($sql);
	
	if( $mod > 0 && $_REQUEST['ano'] >= '2013' ){
		$federal = true;
	} else {
		$federal = false;
	}
} else {
	$federal = verificaEmendaFederal($ptrid);
}

if( !$federal ){
	$filtro = ' and vede.edeid in ( select edeid from emenda.emendapariniciativa ) ';
}

if( $_SESSION['exercicio'] > 2014 ) $filtroImped = " and ei.edeid = vede.edeid ";

$sql = "SELECT
				'<center><img border=\"0\" title=\"Impositivos\" src=\"../imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"abreEmpedimento('||vede.emdid||', \''||sum(vede.edevalor) - coalesce(ei.edivalor, 0)||'\', '||vede.edeid||');\" alt=\"Ir\"/></center>' as acoes,
				vede.emecod,
				(CASE WHEN vede.emetipo = 'E' THEN 'Emenda' ELSE 'Complemento' END) as tipoemenda,
				case when vede.emerelator = 'S' then a.autnome||' - Relator Geral' else a.autnome end as autnome,
				vfun.fupfuncionalprogramatica||'&nbsp;' as funcional,
				vfun.fupdsc,
				vede.gndcod||' - '||gn.gnddsc as gndcod1, 
	            vede.mapcod||' - '||map.mapdsc as modalidade,
	            vede.foncod||' - '||fon.fondsc as fonte,
				sum(vede.edevalor) - coalesce(ei.edivalor, 0) as valor
				--(select ptrid from emenda.ptemendadetalheentidade where edeid = vede.edeid limit 1) as pta
			FROM
				emenda.v_emendadetalheentidade vede
				inner join emenda.ptemendadetalheentidade pte on pte.edeid = vede.edeid
				inner join emenda.autor a on a.autid = vede.autid
				left join emenda.v_funcionalprogramatica vfun on vfun.acaid = vede.acaid and vfun.prgano = '{$_SESSION['exercicio']}' and vfun.acastatus = 'A'
				inner join emenda.entidadebeneficiada enb on enb.enbid = vede.entid
				left join public.gnd gn on gn.gndcod = vede.gndcod and gn.gndstatus = 'A'
	            left join public.modalidadeaplicacao map on map.mapcod = vede.mapcod
	            left join public.fonterecurso fon on fon.foncod = vede.foncod and fon.fonstatus = 'A'
	            left join emenda.emendadetalheimpositivo ei on ei.emdid = vede.emdid $filtroImped
			WHERE
				vede.edestatus = 'A'
				and vede.ededisponivelpta = 'S'
				and vede.emeano = '{$ano}'
				and pte.ptrid = {$ptrid}
				and vede.emetipo = 'E'
				$filtro
			GROUP BY
	          vede.emecod, a.autnome, vede.emerelator, vfun.fupfuncionalprogramatica, vfun.fupdsc,
	          vede.emedescentraliza, vede.emdid, vede.edevalor, vede.foncod, vede.gndcod,
	          vede.mapcod, gn.gnddsc, map.mapdsc, fon.fondsc,vede.emetipo, ei.edivalor, vede.edeid";
?>
<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
	<tr>
		<td>
		<table cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" align="center" class="tabela" style="border-top: none; border-bottom: none; width: 100%">
		<tbody>
			<tr>
				<td width="100%" align="center"><label style="color:#000000;" class="TituloTela"></label></td>
			</tr>
			<tr>
				<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')">Dados Orçamentários</td>
			</tr>
		</tbody>
		</table>
		<?
		$cabecalho = array('Ação', 'Código', 'Tipo', 'Autor', 'Funcional Programática', 'SubTítulo', 'GND', 'Mod', 'Fonte', 'Valor');
		$db->monta_lista_simples($sql, $cabecalho, 6000, 1, '', '100%', '', '');
		?></td>
	</tr>
	<tr bgcolor="#dedfde">
		<td align="center"><input type="button" name="btnFechar" id="btnFechar" value="Fechar" onclick="javascript:window.close();"></td>
	</tr>
</table>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
//jQuery.noConflict();

$(document).ready(function(){
	$.each([ <?=$fluxo ?> ], function( index, value ) {
		$('#td_acao_'+value).css('display', 'none');
	});
});

function abreEmpedimento( emdid, valor, edeid ){
		
	var largura = 900;
	var altura = 700;
	//pega a resolução do visitante
	w = screen.width;
	h = screen.height;
	
	//divide a resolução por 2, obtendo o centro do monitor
	meio_w = w/2;
	meio_h = h/2;
	
	//diminui o valor da metade da resolução pelo tamanho da janela, fazendo com q ela fique centralizada
	altura2 = altura/2;
	largura2 = largura/2;
	meio1 = meio_h-altura2;
	meio2 = meio_w-largura2;
		
	window.open('<?=$_SERVER['SCRIPT_NAME']?>?modulo=principal/emendaImpositivo&acao=A&emdid='+emdid+'&valor='+valor+'&edeid='+edeid,'Emenda Empendimento','height=' + altura + ', width=' + largura + ', top='+meio1+', left='+meio2+',scrollbars=yes,location=no,toolbar=no,menubar=no');
}
</script>