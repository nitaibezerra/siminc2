<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

if( $_REQUEST['requisicao'] == 'enviar' ){
	include_once APPRAIZ.'emenda/classes/MontaObjetoImpositivoSIOP.class.inc';
	include_once APPRAIZ.'emenda/classes/WSEmendaImpositivoSIOP.class.inc';
	
	$arrParam = array('usuario' => $_POST['wsusuario'],
					  'senha' 	=> $_POST['wssenha'],
					  'post' 	=> $_POST
					);
	
	$obImpositivo = new MontaObjetoImpositivoSIOP( $arrParam );
	
	if( $_POST['wssolicitacao'] == 'obterAnalisesEmendas' ){
		$retorno = $obImpositivo->obterAnalisesEmendas();
	} else {
		$retorno = $obImpositivo->cadastrarAnalisesEmendas();
	}
}


?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.14.custom.min.js"></script>
<link rel="stylesheet" href="../includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" type="text/css" media="all" />

<script type="text/javascript" src="../estrutura/js/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<table class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" align="center" border="0">
	<tr bgcolor="#D0D0D0">
		<td colspan="15" align="center">
			<input type="button" value="Fechar" name="btnFechar" style="cursor: pointer;"/>
		</td>
	</tr>
</table>
<?

if( $_SESSION['exercicio'] > 2014 )	$filtroImp = ' and ei.edeid = vede.edeid';

$sql = "SELECT distinct
			vede.emecod,
			vfun.unicod||' - '||un.unidsc as unidade,
			case when vede.emerelator = 'S' then a.autcod||' - '||a.autnome||' - Relator Geral' else a.autcod||' - '||a.autnome end as autor,
			vfun.fupfuncionalprogramatica||' ' as funcional,
			vede.gndcod||' - '||gn.gnddsc as gndcod1, 
            vede.mapcod||' - '||map.mapdsc as modalidade,
            vede.foncod||' - '||fon.fondsc as fonte,
			ei.edivalor,
            array_to_string(array(select ei1.emiid from emenda.emendaimpositivo_tiposjust eit
											inner join emenda.emendaimpositivo ei1 on ei1.emiid = eit.emiid
										where
											eit.ediid = ei.ediid), ', ') as tiposimpedimento,
			lg.logretorno,
            lg.logtipo
		FROM
			emenda.v_emendadetalheentidade vede
			inner join emenda.ptemendadetalheentidade pte on pte.edeid = vede.edeid
			inner join emenda.emendadetalhe ed on ed.emdid = vede.emdid
			inner join emenda.autor a on a.autid = vede.autid
			left join emenda.v_funcionalprogramatica vfun on vfun.acaid = vede.acaid and vfun.prgano = '{$_SESSION['exercicio']}' and vfun.acastatus = 'A'
			inner join emenda.entidadebeneficiada enb on enb.enbid = vede.entid
			left join public.gnd gn on gn.gndcod = vede.gndcod and gn.gndstatus = 'A'
            left join public.modalidadeaplicacao map on map.mapcod = vede.mapcod
            left join public.fonterecurso fon on fon.foncod = vede.foncod and fon.fonstatus = 'A'
            inner join emenda.emendadetalheimpositivo ei on ei.emdid = vede.emdid $filtroImp
            inner join public.unidade un on un.unicod = vfun.unicod
            inner join (select l.emdid, l.emecod, l.logretorno, l.logtipo 
                    			from emenda.logerroswsdl l 
                                where l.logdatasolicitacao = (select max(logdatasolicitacao) 
                                								from emenda.logerroswsdl where emdid = l.emdid)) lg on lg.emdid = ed.emdid
		WHERE
			vede.edestatus = 'A'
			and vede.ededisponivelpta = 'S'
			and vede.emeano = '{$_SESSION['exercicio']}'
			and vede.emetipo = 'E'
			and vede.emdid in (".implode(',', $_POST['chk']).")
        order by vede.emecod, autor";
 
$cabecalho = array('Código', 'UO', 'Autor', 'Funcional Programática', 'GND', 'Mod', 'Fonte', 'Valor Impedimento', 'Tipos Impedimento', 'Log', 'Serviço');
$db->monta_lista_simples($sql, $cabecalho, 60000000, 1, '', '95%', '', '', '', '', true);

?>
<table class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" align="center" border="0">
	<tr bgcolor="#D0D0D0">
		<td colspan="15" align="center">
			<input type="button" value="Fechar" name="btnFechar" style="cursor: pointer;"/>
		</td>
	</tr>
</table>
<script>
jQuery('[name="btnFechar"]').click(function(){
	window.close();
});

jQuery(document).ready( function() {

	window.onbeforeunload = function( event ){
		// captura o botao clicado para fechar
		
		if( event.explicitOriginalTarget.URL != 'undefined' ){
			window.opener.location.href = window.opener.location;
		}
	}
});
</script>