<?php

if(!$boPopup) require_once APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo(MODULO_NAME, '' );
montaCabecalhoUG(Array('termo'=>true));
echo "<br>";
montaAbaTermoCooperacao();

verificaCodigoTermoSessao();

if( $_SESSION['exercicio'] > 2014 ) $filtroImped = " and ei.edeid = ede.edeid ";

$sql = "SELECT
				'<center><img border=\"0\" title=\"Impositivos\" src=\"../imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"abreEmpedimento('||ed.emdid||', \''||sum(ede.edevalor) - coalesce(ei.edivalor, 0)||'\', '||ede.edeid||');\" alt=\"Ir\"/></center>' as acoes,
				ee.emecod,
				a.autnome,
				vfun.fupfuncionalprogramatica||'&nbsp;' as funcional,
				vfun.fupdsc,
				ed.gndcod||' - '||gn.gnddsc as gndcod1, 
	            ed.mapcod||' - '||map.mapdsc as modalidade,
	            ed.foncod||' - '||fon.fondsc as fonte,
				sum(ede.edevalor) - coalesce(ei.edivalor, 0) as valor
			FROM
				emenda.emenda ee
                inner join emenda.emendadetalhe ed on ed.emeid = ee.emeid
                inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid and ede.edestatus = 'A'
				inner join emenda.autor a on a.autid = ee.autid
				left join emenda.v_funcionalprogramatica vfun on vfun.acaid = ee.acaid and vfun.prgano = '{$_SESSION['exercicio']}' and vfun.acastatus = 'A'
                inner join emenda.emendatermocooperacao etc on etc.emeid = ee.emeid
				left join public.gnd gn on gn.gndcod = ed.gndcod and gn.gndstatus = 'A'
	            left join public.modalidadeaplicacao map on map.mapcod = ed.mapcod
	            left join public.fonterecurso fon on fon.foncod = ed.foncod and fon.fonstatus = 'A'
	            left join emenda.emendadetalheimpositivo ei on ei.emdid = ed.emdid $filtroImped
			WHERE
				ee.emeano = '{$_SESSION['exercicio']}'
				and ee.emetipo = 'E'
				and etc.tcpid = {$_SESSION['elabrev']['tcpid']}
			GROUP BY
	          	ee.emecod, a.autnome, vfun.fupfuncionalprogramatica, vfun.fupdsc,
	          	ed.emdid, ed.foncod, ed.gndcod,
	          	ed.mapcod, gn.gnddsc, map.mapdsc, fon.fondsc, ee.emetipo, ei.edivalor, ede.edeid";

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
		$cabecalho = array('Ação', 'Código', 'Autor', 'Funcional Programática', 'SubTítulo', 'GND', 'Mod', 'Fonte', 'Valor');
		$db->monta_lista_simples($sql, $cabecalho, 6000, 1, '', '100%', '', '');
		?></td>
	</tr>
</table>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

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
		
	window.open('elabrev.php?modulo=principal/termoCooperacao/emendatermoimpositivo&acao=A&emdid='+emdid+'&valor='+valor+'&edeid='+edeid,'Emenda Empendimento','height=' + altura + ', width=' + largura + ', top='+meio1+', left='+meio2+',scrollbars=yes,location=no,toolbar=no,menubar=no');
}
</script>