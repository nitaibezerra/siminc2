<?php
$_SESSION["emenda"]["ptridAnalise"] = !empty( $_REQUEST["ptridAnalise"] ) ? $_REQUEST["ptridAnalise"] : $_SESSION["emenda"]["ptridAnalise"];

$ptrid = $_SESSION["emenda"]["ptridAnalise"];

include_once APPRAIZ . 'includes/workflow.php';
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$boReformulacao = pegaPaiPTA( $_SESSION["emenda"]["ptridAnalise"] );
$boReformulacao = ( !empty($boReformulacao) ? true : false );

$estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptridAnalise"] );

montaAbasAnalise( $abacod_tela, $url, '' , $estadoAtual, $boReformulacao);

$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );


monta_titulo( 'Orçamento Impositivo', '' );

echo cabecalhoPlanoTrabalho($ptrid, true);
$docid = criarDocumento( $ptrid );

if( $_SESSION['exercicio'] > 2014 ) $filtroImped = " and ei.edeid = vede.edeid ";
//Espaço em branco
$sql = "SELECT 
			'<center><img border=\"0\" title=\"Impositivos\" src=\"../imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"abreEmpedimento('||vede.emdid||', \''||vede.edevalor||'\', '||vede.edeid||');\" alt=\"Ir\"/></center>' as acoes,
		    vede.emecod,
            COALESCE(fup.fupfuncionalprogramatica, 'Dados Incompletos') as fupfuncionalprogramatica,
		    (CASE WHEN vede.emetipo = 'E' THEN 'Emenda' ELSE 'Complemento' END) as tipoemenda, 
		    case when vede.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autnome,
		    vede.gndcod||' - '||gn.gnddsc as gndcod,
		    vede.mapcod||' - '||map.mapdsc as mapcod,
		    vede.foncod||' - '||fon.fondsc as foncod, 
		    tpe.tpedsc,
		    (sum(per.pervalor) - coalesce(ei.edivalor, 0)) as pervalor,
		    case when vede.emdliberado = 'S' then 'Sim' else 'Não' end as liberado
		FROM emenda.ptiniciativa pti
			inner join emenda.ptiniciativaespecificacao pte on pte.ptiid = pti.ptiid and pte.ptestatus = 'A'
		 	inner join emenda.ptemendadetalheentidade ped on ped.ptrid = pti.ptrid
		 	inner join emenda.ptiniesprecurso per on per.pteid = pte.pteid and per.pedid = ped.pedid
		   	inner join emenda.tipoensino tpe on pti.tpeid = tpe.tpeid
		   	inner join emenda.v_emendadetalheentidade vede on vede.edeid = ped.edeid
		   	inner join emenda.autor aut on aut.autid = vede.autid
		   	left join emenda.v_funcionalprogramatica fup on fup.acaid = vede.acaid
		  	left join public.gnd gn on gn.gndcod = vede.gndcod and gn.gndstatus = 'A'
		  	left join public.modalidadeaplicacao map on map.mapcod = vede.mapcod
		  	left join public.fonterecurso fon on fon.foncod = vede.foncod and fon.fonstatus = 'A'
		  	left join emenda.emendadetalheimpositivo ei on ei.emdid = vede.emdid and ei.edistatus = 'A' $filtroImped
		WHERE 
		    pti.ptrid = $ptrid
		    and per.pervalor <> 0
		GROUP BY 
		    tpe.tpedsc, vede.emecod, vede.edeid,
		    aut.autnome, vede.emerelator, vede.gndcod, vede.mapcod, 
		    vede.foncod, fup.fupfuncionalprogramatica,
		    vede.emetipo, gn.gnddsc, map.mapdsc, fon.fondsc,
		    vede.emdliberado, vede.emdid, vede.edevalor, ei.edivalor
		ORDER BY
			vede.gndcod, fup.fupfuncionalprogramatica,
			tpe.tpedsc, vede.emecod,
		    vede.mapcod, vede.foncod";

?> 
<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
	<tr>
		<td><?
		monta_titulo( '', 'Dados Orçamentários' );
		$cabecalho = array('Ação', 'Código', 'Funcional', 'Tipo', 'Autor', 'GND', 'Mod', 'Fonte', 'Nível de Ensino', 'Valor', 'Limite Autorizado');
		$db->monta_lista_simples($sql, $cabecalho, 6000, 1, '', '95%', '', '');
		?></td>
		<td valign="top" align="center">
			<?php
				wf_desenhaBarraNavegacao( $docid , array( 'url' => $_SESSION['favurl'], 'ptrid' => $ptrid, 'resid' => '' , 'tipo' => "" ) );
				
				$arFluxo = verificaFluxo( $ptrid );
				
				if( $arFluxo ){
					$fluxo = implode(', ', $arFluxo);
				}
			?>
		</td>
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
		
	var largura = 810;
	var altura = 600;
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
		
	window.open('emenda.php?modulo=principal/emendaImpositivo&acao=A&emdid='+emdid+'&valor='+valor+'&edeid='+edeid,'Emenda Empendimento','height=' + altura + ', width=' + largura + ', top='+meio1+', left='+meio2+'');
}
</script>