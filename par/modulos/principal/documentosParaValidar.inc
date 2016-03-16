<?php 
function listaSubacoesDocumento(){
	
	global $db;
	
	$sql = "SELECT DISTINCT
				'<img src=../imagens/icone_lupa.png style=cursor:pointer; sbaid='||sbd.sbaid||' sbdano='|| sbd.sbdano ||' class=subacao tile=Subação />' as acao,
				'<input type=hidden '||lpad(replace(par.retornacodigosubacao(sbd.sbaid),'.',''),8,'0')||' />('||par.retornacodigosubacao(sbd.sbaid)||') '||sba.sbadsc as descricao,
				sbdano
			FROM 
				par.termocomposicao 	dpc
			INNER JOIN par.subacaodetalhe 	sbd ON sbd.sbdid = dpc.sbdid
			INNER JOIN par.subacao 			sba ON sba.sbaid = sbd.sbaid
			WHERE
				dopid = {$_REQUEST['dopid']}
			ORDER BY
				2";
	
	$cabecalho = array("&nbsp;","Subação", "Ano");
	$db->monta_lista_simples($sql,$cabecalho,100,5,'N','','N');
}

if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	die();
}

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" 			src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<script type="text/javascript">

$(document).ready(function(){

	$('.subacao').live('click',function(){
		var sbaid  = $(this).attr('sbaid');
		var sbdano = $(this).attr('sbdano');
		window.open('par.php?modulo=principal/subacaoVisualizar&acao=A&sbaid='+sbaid+'&sbdano='+sbdano,'subacao','scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no');
	});
	
	$('.subacoes').click(function(){
		var dopid = $(this).attr('dopid');
		$.ajax({
	   		type: 	"POST",
	   		url: 	window.location,
	   		data: 	'req=listaSubacoesDocumento&dopid='+dopid,
	   		async: 	false,
	   		success: function(msg){
				$( "#div_dialog" ).html(msg);		
				$( "#div_dialog" ).show();		
				$( "#div_dialog" ).dialog({
					resizable: true,
					width: 700,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					buttons: {
						"Fechar": function() {
							$( this ).dialog( "close" );
						},
					}
				});
	   		}
		});
	});
});

function abreReprogramacao(dopid){
	if( dopid != '' && dopid != undefined ){
		window.open("par.php?modulo=principal/popupReprogramacaoPAR&acao=A&dopid="+dopid+"&caminho=2","reprogramacaoPar","height=400,width=600,scrollbars=yes,top=50,left=200");
	}
}
</script>
<div id="div_dialog" style="display:none" ></div>
<?php

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

//include_once(APPRAIZ."par/modulos/principal/manutencao.inc");

//PAR
monta_titulo( $titulo_modulo, 'Documentos do PAR' );
//
$perfil = pegaArrayPerfil($_SESSION['usucpf']);
$inner 		= "";
$whereOR 	= Array();
$filtroModelo = "";
if( in_array(PAR_PERFIL_UNIVERSIDADE_ESTADUAL, $perfil) ) {
	$sql = "SELECT entid 
			FROM par.usuarioresponsabilidade 
			WHERE pflcod = ".PAR_PERFIL_UNIVERSIDADE_ESTADUAL." AND usucpf = '".$_SESSION['usucpf']."' AND rpustatus = 'A'";
	
	$entid = $db->pegaUm( $sql );

        if ($entid) {
	
            $sql = "SELECT inuid FROM par.instrumentounidadeentidade WHERE entid = ".$entid;

            $_SESSION['par']['inuid'] = $db->pegaUm( $sql );

            $sql = "SELECT estuf FROM par.instrumentounidade WHERE inuid = ".$_SESSION['par']['inuid'];

            $estuf = $db->pegaUm( $sql );
            $inner = "LEFT JOIN par.instrumentounidadeentidade iue ON iue.iuecnpj = emp.empcnpj AND iue.entid = $entid ";

            $whereOR[] 	= "iue.entid = $entid";
            $whereOR[] 	= "iu.estuf = '{$estuf}'";
            $filtroModelo = " AND mdoid in (34, 40, 44 ) "; 
        } else {
            echo "<script>
                alert('Seu perfil \"Universidade Estadual\" não possui associação com nenhuma entidade');
                window.location = '../logout.php';
                </script>";
            exit;
        }
} 
if( in_array(PAR_PERFIL_ENTIDADE_EXECUTORA, $perfil) ) {
	
	$sql = "SELECT urs.iueid FROM par.usuarioresponsabilidade urs WHERE urs.usucpf = '{$_SESSION['usucpf']}' 
				and urs.pflcod = ".PAR_PERFIL_ENTIDADE_EXECUTORA." and urs.rpustatus = 'A' and urs.iueid is not null";
	
	$arIueid = $db->carregarColuna($sql);
        if ($arIueid) {
	$arIueid = $arIueid ? $arIueid : array();
	
	$whereOR[] = "dp.iueid in (".implode(', ', $arIueid).")";
        } else {
            echo "<script>
                alert('Seu perfil \"Entidade Executora\" não possui associação com nenhuma entidade');
                window.location = '../logout.php';
                </script>";
            exit;
        }
}

if( $whereOR[0] != '' ){
	$inner .= " WHERE (".implode(" OR ", $whereOR).")";
}

if( in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array( PAR_PERFIL_ADMINISTRADOR, $perfil )){
	$filtro = '';
	$inner 	= '';
} 


$cabecalho = array("&nbsp;","Nº do Documento", "Tipo de documento");
$acoes = "'<img src=../imagens/icone_lupa.png style=cursor:pointer; onclick=\"window.open(\'par.php?modulo=principal/teladeassinatura&acao=A&validacao=S&dopid='||id||'\',\'assinatura\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\">&nbsp;
			<img src=../imagens/ico_config_i.gif title=Subações class=subacoes dopid='|| id ||' style=cursor:pointer; >'";

if( in_array(PAR_PERFIL_UNIVERSIDADE_ESTADUAL, $perfil) || in_array(PAR_PERFIL_ENTIDADE_EXECUTORA, $perfil) ) {
	$cabecalho = array("&nbsp;","Nº do Documento", "Tipo de documento","Situação");
	$botaoreprogramacao = ",CASE WHEN dopdatafimvigencia IS NOT NULL
			THEN 
				CASE WHEN mdoid <> 3 THEN 
					CASE WHEN id IN ( SELECT dopid FROM par.documentoparreprogramacao WHERE dprstatus = 'P' ) THEN 
						'<center><b>Aguardando Validação de Reprogramação</b></center>' 
					ELSE 
						CASE WHEN id IS NOT NULL THEN '<center><input type=\"button\" value=\"Reprogramar\" onclick=\"abreReprogramacao(' || id || ')\"></center>' ELSE '' END 
					END
				ELSE
					'<center><input type=\"button\" value=\"Reprogramar\" disabled \"></center>'
				END
			END as r";
}

$sqlfiltro =	"case when contagem = mdoqtdvalidacao then '<center><img src=\"../imagens/obras/check.png\" title=\"Documento Validado\" width=\"18\">&nbsp;'||$acoes||'</center>' else '<center>'||$acoes||'</center>' end as acoes,";

$sql = "SELECT DISTINCT
			$sqlfiltro dopnumerodocumento, tipodocumento
			{$botaoreprogramacao}
		FROM (
			SELECT 	
				dp.dopid as id, dp.dopnumerodocumento, dp.mdonome as tipodocumento, iu.inuid, iu.estuf, iu.muncod, dpv.dpvcpf, dp.mdoid, d.mdoqtdvalidacao, --dp.iueid,
				(SELECT count(dopid) FROM par.documentoparvalidacao WHERE dopid = dp.dopid AND dpvstatus = 'A' ) as contagem, dp.dopdatafimvigencia
			FROM par.vm_documentopar_ativos   dp
			INNER JOIN par.modelosdocumentos   		d   ON d.mdoid = dp.mdoid
			INNER JOIN par.processopar 				pp  ON pp.prpid = dp.prpid
			INNER JOIN par.empenho 					emp ON emp.empnumeroprocesso = pp.prpnumeroprocesso AND emp.empsituacao = '2 - EFETIVADO' and empstatus = 'A' 
			INNER JOIN par.instrumentounidade 		iu  ON iu.inuid = pp.inuid 
			LEFT  JOIN par.documentoparvalidacao 	dpv ON dpv.dopid = dp.dopid
			$inner
			UNION
			SELECT 
				dp.dopid as id, dp.dopnumerodocumento, dp.mdonome as tipodocumento, iu.inuid, iu.estuf, iu.muncod, dpv.dpvcpf, dp.mdoid, d.mdoqtdvalidacao, --dp.iueid,
				(SELECT count(dopid) FROM par.documentoparvalidacao WHERE dopid = dp.dopid AND dpvstatus = 'A' ) as contagem, dp.dopdatafimvigencia
			FROM par.vm_documentopar_ativos   dp
			INNER JOIN par.modelosdocumentos   		d   ON d.mdoid = dp.mdoid
			INNER JOIN par.processoobraspar 		pp  ON pp.proid = dp.proid and pp.prostatus = 'A'
			INNER JOIN par.empenho 					emp ON emp.empnumeroprocesso = pp.pronumeroprocesso AND emp.empsituacao = '2 - EFETIVADO' and empstatus = 'A' 
			INNER JOIN par.instrumentounidade 		iu  ON iu.inuid = pp.inuid 
			LEFT  JOIN par.documentoparvalidacao 	dpv ON dpv.dopid = dp.dopid
			$inner
			) as foo
		WHERE 1=1 $filtroModelo "; 
//ver( simec_htmlentities( $sql ));
$db->monta_lista_simples($sql,$cabecalho,100,5,'N','','N');
?>