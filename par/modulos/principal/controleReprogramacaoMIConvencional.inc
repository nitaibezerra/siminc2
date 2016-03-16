<?php

if( $_REQUEST['requisicao'] == 'mostraDadosObras' ){
	
	if( $_REQUEST['tipoobra'] ){
		$filtro = " and pre.ptoid in ( ".$_REQUEST['tipoobra']." )";
	}
	
	$acao = "'<center><img src=\"../imagens/alterar.gif\" onclick=\"abreObraControle( '||pre.preid||', '||pre.preano||', \''||case when pc.preid is not null then 'pac' else 'obra' end||'\' );\" style=\"cursor:pointer\"/></center>'";
	
	$sql = "SELECT distinct
				$acao,
			    pre.preid,    
			    pre.predescricao,
			    pre.preano,
			    cast(pre.prevalorobra as numeric(20,2)) as prevalorobra
			FROM 
			    obras.preobra pre
			    inner join workflow.documento doc ON pre.docid = doc.docid
				left join par.processoobraspaccomposicao pc on pc.preid = pre.preid and pc.pocstatus = 'A'
			WHERE 
			    doc.esdid in (".$_REQUEST['esdid'].") 
			    $filtro
			    AND pre.prestatus = 'A'
			ORDER BY pre.predescricao";
	
	$cabecalho 	= array("Ação", "preid", "Descrição", "Ano", "Valor");
	$db->monta_lista_simples( $sql, $cabecalho,1,5,'N','100%','N', true, false, false, true);
	
	exit();
}

if( $_REQUEST['requisicao'] == 'mostraDadosMunicipio' ){
	
	if( $_REQUEST['tipoobra'] ){
		$filtro = " and pre.ptoid in ( ".$_REQUEST['tipoobra']." )";
	}
	
	$sql = "SELECT distinct
				mun.muncod,
			    mun.estuf,
			    mun.mundescricao
			FROM 
			    obras.preobra pre
			    inner join workflow.documento doc ON pre.docid = doc.docid
			    inner join territorios.municipio mun on (mun.muncod = pre.muncod or mun.muncod = pre.muncodpar)
			WHERE 
			    doc.esdid in (".$_REQUEST['esdid'].") 
			    $filtro
			    AND pre.prestatus = 'A'
			ORDER BY mun.estuf, mun.mundescricao";
	
	$cabecalho 	= array("IBGE", "UF", "Municipio");
	$db->monta_lista_simples( $sql, $cabecalho,1,5,'N','100%','N', true, false, false, true);
	
	exit();
}


//$arrDadosQuadroResumo = pegaInformacoesQuadroResumo();

include APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );

$arrAbas = array(
		0 => array("descricao" => "Validação de MI para convencional", "link" 	=> "par.php?modulo=principal/validacaoReprogramacaoMIConvencional&acao=A"),
		1 => array("descricao" => "Controle da Reformulação MI para convencional", "link" 	=> "par.php?modulo=principal/controleReprogramacaoMIConvencional&acao=A")
);


echo montarAbasArray($arrAbas, "par.php?modulo=principal/controleReprogramacaoMIConvencional&acao=A");

monta_titulo( "Controle Gerencial Reformulações MI para Convencional", '' );
?>
<!-- Div de aguardando -->
<center>
	<div id="aguardando" style="display:none; position: absolute; background-color: white; height:300%; width:100%; opacity:0.4; filter:alpha(opacity=40); " >
		<div style="margin-top:250px; align:center;">
			<img border="0" title="Aguardando" src="../imagens/carregando.gif">
			Carregando...
		</div>
	</div>
</center>
<!-- Div de aguardando - FIM -->
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<!-- Bibliotecas para funcionar a Modal do Jquery -->
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<!-- Bibliotecas para funcionar a Modal do Jquery - FIM -->
<script type="text/javascript">

$(document).ready(function(){


	$('#aguardando').show();







	
	$('#aguardando').hide();
	

});

function abreObraControle( preid, ano, tipo ){
	if( tipo == 'obra' ){
		return window.open('par.php?modulo=principal/subacaoObras&acao=A&preid='+preid+'&ano='+ano, 'subacaoObras', "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();
	} else {
		return window.open('par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&preid='+preid+'&ano='+ano, 'ProInfancia', "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();
	}
}


</script>

<?php 
function getValoresControle()
{

	global $db;

	$sql = "SELECT 
			CASE 
				WHEN doc.ESDID IN (1486, 1488) THEN 'reform_MI_convenc'
				WHEN doc.ESDID IN 	(1561) THEN 'analise_solicita_reformul_MI_convenc'
				WHEN doc.ESDID IN 	(1578) THEN 'dilig_solicita_reform_MI_convenc'
				WHEN doc.ESDID IN 	(1579) THEN 'analise_solicita_reformu_MI_convenc_retorno_dilig'
				WHEN doc.ESDID IN (1487, 1489) THEN 'analise_reformul_MI_convenc'
				WHEN doc.ESDID IN (1548, 1549) THEN 'dilig_reformu_MI_convenc'
				WHEN doc.ESDID IN (1564, 1568) THEN 'analise_reform_MI_para_convenc_retorn_dilig'
				WHEN doc.ESDID IN (1553, 1566) THEN 'valida_deferimen_Reform_MI_convenc'
				WHEN doc.ESDID IN (1551, 1550) THEN 'valida_gestor_reformu_MI_convenc'
				WHEN doc.ESDID IN (1552, 1567) THEN 'valida_indefer_reformu_MI_convenc'
			END AS esd,
            CASE 
				WHEN doc.ESDID IN (1486, 1488) THEN '1486, 1488'
				WHEN doc.ESDID IN 	(1561) THEN '1561'
				WHEN doc.ESDID IN 	(1578) THEN '1578'
				WHEN doc.ESDID IN 	(1579) THEN '1579'
				WHEN doc.ESDID IN (1487, 1489) THEN '1487, 1489'
				WHEN doc.ESDID IN (1548, 1549) THEN '1548, 1549'
				WHEN doc.ESDID IN (1564, 1568) THEN '1564, 1568'
				WHEN doc.ESDID IN (1553, 1566) THEN '1553, 1566'
				WHEN doc.ESDID IN (1551, 1550) THEN '1551, 1550'
				WHEN doc.ESDID IN (1552, 1567) THEN '1552, 1567'
			END AS estado,	
			'' as tipoobra,
			COUNT(pre.preid) as quantidade,
			COUNT(DISTINCT muncod) as municipio
		FROM  obras.preobra pre
			INNER JOIN workflow.documento doc ON pre.docid = doc.docid
		WHERE doc.esdid in ( 1486, 1488,1561 ,1578,1579,1487,1489,1548,1549,1564, 1568,1553,1566,1551,1550,1552,1567 )
		AND pre.prestatus = 'A'
		group by esd, estado
		
		UNION ALL 
			SELECT 
				'aguard_aprova_de_prorrog' as esd,
                '1260, 755' as estado,
				'43, 42, 44, 45, 75, 76, 73, 74' as tipoobra,
				count(distinct pre.preid) as quantidade,
				count(distinct muncod) as municipio		
			FROM 
				obras.preobra pre
				INNER JOIN workflow.documento doc ON pre.docid = doc.docid			
			WHERE 
				pre.ptoid in (43, 42, 44, 45, 75, 76, 73, 74) -- MI  -- emendas também?
				AND doc.esdid in (1260, 755) 
            	AND pre.prestatus = 'A'
		UNION ALL 		
				SELECT 
					'nao_solicitada'as esd,
                    '360, 624, 337, 228' as estado,
					'43, 42, 44, 45' as tipoobra,
					count(distinct pre.preid) as quantidade,
					count(distinct muncod) as municipio		
				FROM 
					obras.preobra pre
					INNER JOIN workflow.documento doc ON pre.docid = doc.docid
				WHERE 
					pre.ptoid in (43, 42, 44, 45) -- MI
					AND doc.esdid in ( 360, 624, 337, 228) 
                    AND pre.prestatus = 'A'
		UNION ALL
				SELECT 
					'obra_aprovada'as esd,
                    '228, 337' as estado,
					'75, 76, 73, 74' as tipoobra,
					count(distinct pre.preid) as quantidade,
					count(distinct muncod) as municipios
				FROM 
					obras.preobra pre
					INNER JOIN workflow.documento doc ON pre.docid = doc.docid
				WHERE 
					pre.ptoid in (75, 76, 73, 74) -- MI
                    AND doc.esdid in (228, 337) 
                    AND pre.prestatus = 'A'
		UNION ALL
			SELECT 
			'total_mun_1' as esd,
            '' as estado,
			'' as tipoobra,
			0 as quantidade ,
            COUNT( distinct muncod) as municipio
			FROM(
				SELECT 
					distinct muncod
				FROM 
					obras.preobra pre
					INNER JOIN workflow.documento doc ON pre.docid = doc.docid
				WHERE 
					pre.ptoid in (43, 42, 44, 45) -- MI
                    AND doc.esdid in ( 360, 624, 337, 228) 
                    AND pre.prestatus = 'A'		
				UNION ALL 		
				SELECT
					DISTINCT muncod
				FROM  obras.preobra pre
					INNER JOIN workflow.documento doc ON pre.docid = doc.docid
				WHERE doc.esdid in ( 1486, 1488 )
				AND pre.prestatus  = 'A'
			) AS foo
		UNION ALL 
		SELECT 
			'totais_2' AS esd,
            '' as estado,
			'' as tipoobra,
			COUNT(pre.preid) as quantidade,
			COUNT(DISTINCT muncod) as municipio	
		FROM  obras.preobra pre
			INNER JOIN workflow.documento doc ON pre.docid = doc.docid
		WHERE doc.esdid in (1561, 1578, 1579)
		AND pre.prestatus = 'A'		
UNION ALL		
		SELECT 
			'total_mun_3' as esd,
            '' as estado,
			'' as tipoobra,
			0 as quantidade ,
            COUNT( distinct muncod) as municipio
		FROM
		(
			SELECT 
				distinct muncod as muncod
			FROM 
				obras.preobra pre
				INNER JOIN workflow.documento doc ON pre.docid = doc.docid
			WHERE 
				pre.ptoid in (75, 76, 73, 74) -- MI
                AND doc.esdid in (228, 337  ) 
                AND pre.prestatus = 'A'		
			UNION ALL 		
			SELECT
				DISTINCT muncod
			FROM  obras.preobra pre
				INNER JOIN workflow.documento doc ON pre.docid = doc.docid
			WHERE doc.esdid in ( 1487, 1489, 1548, 1549, 1564, 1568, 1553 , 1566 , 1551 , 1550 )
			AND pre.prestatus  = 'A'
		) AS foo		
		UNION ALL		
		--Aguardando Aprovação de Prorroga mais 		
		SELECT 
			'total_mun_4' as esd,
            '' as estado,
			'' as tipoobra,
			0 as quantidade ,
				COUNT( distinct muncod) as municipio
		FROM
		(
			SELECT 		
				distinct muncod
			FROM 
				obras.preobra pre
				INNER JOIN workflow.documento doc ON pre.docid = doc.docid			
			WHERE 
				pre.ptoid in (43, 42, 44, 45, 75, 76, 73, 74) -- MI  -- emendas também?
                AND doc.esdid in (1260, 755)
                AND pre.prestatus = 'A'		
			UNION ALL 		
			SELECT
				DISTINCT muncod
			FROM  obras.preobra pre
				INNER JOIN workflow.documento doc ON pre.docid = doc.docid
			WHERE doc.esdid in (1552, 1567)
			AND pre.prestatus  = 'A'
		) AS foo";

	$resultado = $db->carregar( $sql);

	$resultado = ($resultado ) ? $resultado  : Array();
	
	foreach($resultado as $k => $v )
	{
		$arrRetorno[$v['esd']] = $v;
	}
	
	return $arrRetorno;

}

$dadosControle = getValoresControle();

function montaArrayTabela( $dadosControle ){
	
return	array(
		array(
			'titulo' 	=> 'Não solicitado Reformulação MI para convencional',
			'qtd_obras'	=> $dadosControle['nao_solicitada']['quantidade'],
			'qtd_mun'	=> $dadosControle['nao_solicitada']['municipio'],
			'estado'	=> $dadosControle['nao_solicitada']['estado'],
			'tipoobra'	=> $dadosControle['nao_solicitada']['tipoobra']
		),
		array(
			'titulo' 	=> 'RMC - Em Reformulação MI para Convencional',
			'qtd_obras'	=> $dadosControle['reform_MI_convenc']['quantidade'],
			'qtd_mun'	=> $dadosControle['reform_MI_convenc']['municipio'],
			'estado'	=> $dadosControle['reform_MI_convenc']['estado'],
			'tipoobra'	=> $dadosControle['reform_MI_convenc']['tipoobra'],
		),
		array(
			'titulo' 	=> 'Total',
			'qtd_obras'	=> ($dadosControle['nao_solicitada']['quantidade'] + $dadosControle['reform_MI_convenc']['quantidade']),
			'qtd_mun'	=> $dadosControle['total_mun_1']['municipio'],
			'estado'	=> $dadosControle['total_mun_1']['estado'],
			'tipoobra'	=> $dadosControle['total_mun_1']['tipoobra'],
		),
		array(
			'titulo' 	=> 'null',
			'qtd_obras'	=> '',
			'qtd_mun'	=> '',
			'estado'	=> '',
			'tipoobra'	=> '',
		),
		array(
			'titulo' 	=> 'Subtotal da fase',
			'qtd_obras'	=> '',
			'qtd_mun'	=> '',
			'estado'	=> '',
			'tipoobra'	=> '',
		),
		array(
			'titulo' 	=> 'RMC - Em Análise da Solicitação de Reformulação MI para convencional',
			'qtd_obras'	=> $dadosControle['analise_solicita_reformul_MI_convenc']['quantidade'],
			'qtd_mun'	=> $dadosControle['analise_solicita_reformul_MI_convenc']['municipio'],
			'estado'	=> $dadosControle['analise_solicita_reformul_MI_convenc']['estado'],
			'tipoobra'	=> $dadosControle['analise_solicita_reformul_MI_convenc']['tipoobra']
		),
		array(
			'titulo' 	=> 'RMC - Em Diligência da Solicitação de Reformulação MI para convencional',
			'qtd_obras'	=> $dadosControle['dilig_solicita_reform_MI_convenc']['quantidade'],
			'qtd_mun'	=> $dadosControle['dilig_solicita_reform_MI_convenc']['municipio'],
			'estado'	=> $dadosControle['dilig_solicita_reform_MI_convenc']['estado'],
			'tipoobra'	=> $dadosControle['dilig_solicita_reform_MI_convenc']['tipoobra'],
		),
		array(
			'titulo' 	=> 'RMC - Em Análise da Solicitação de Reformulação MI para convencional / Retorno de Diligência',
			'qtd_obras'	=> $dadosControle['analise_solicita_reformu_MI_convenc_retorno_dilig']['quantidade'],
			'qtd_mun'	=> $dadosControle['analise_solicita_reformu_MI_convenc_retorno_dilig']['municipio'],
			'estado'	=> $dadosControle['analise_solicita_reformu_MI_convenc_retorno_dilig']['estado'],
			'tipoobra'	=> $dadosControle['analise_solicita_reformu_MI_convenc_retorno_dilig']['tipoobra'],
		),
		array(
			'titulo' 	=> 'Total',
			'qtd_obras'	=> ($dadosControle['totais_2']['quantidade']),
			'qtd_mun'	=> $dadosControle['totais_2']['municipio'],
			'estado'	=> $dadosControle['totais_2']['estado'],
			'tipoobra'	=> $dadosControle['totais_2']['tipoobra'],
		),
		array(
		'titulo' 	=> 'null',
		'qtd_obras'	=> '',
		'qtd_mun'	=> '',
		'estado'	=> '',
		'tipoobra'	=> '',
		),
		array(
			'titulo' 	=> 'Subtotal da fase',
			'qtd_obras'	=> '',
			'qtd_mun'	=> '',
			'estado'	=> '',
			'tipoobra'	=> '',
		),
		array(
			'titulo' 	=> 'RMC - Em Análise da Reformulação MI para Convencional',
			'qtd_obras'	=> $dadosControle['analise_reformul_MI_convenc']['quantidade'],
			'qtd_mun'	=> $dadosControle['analise_reformul_MI_convenc']['municipio'],
			'estado'	=> $dadosControle['analise_reformul_MI_convenc']['estado'],
			'tipoobra'	=> $dadosControle['analise_reformul_MI_convenc']['tipoobra'],
		),
		array(
			'titulo' 	=> 'RMC - Em Diligência da Reformulação MI para Convencional',
			'qtd_obras'	=> $dadosControle['dilig_reformu_MI_convenc']['quantidade'] ,
			'qtd_mun'	=> $dadosControle['dilig_reformu_MI_convenc']['municipio'],
			'estado'	=> $dadosControle['dilig_reformu_MI_convenc']['estado'],
			'tipoobra'	=> $dadosControle['dilig_reformu_MI_convenc']['tipoobra'],
		),
		array(
			'titulo' 	=> 'RMC - Em Análise da Reformulação MI para Convencional / Retorno de Diligência',
			'qtd_obras'	=> $dadosControle['analise_reform_MI_para_convenc_retorn_dilig']['quantidade'],
			'qtd_mun'	=> $dadosControle['analise_reform_MI_para_convenc_retorn_dilig']['municipio'],
			'estado'	=> $dadosControle['analise_reform_MI_para_convenc_retorn_dilig']['estado'],
			'tipoobra'	=> $dadosControle['analise_reform_MI_para_convenc_retorn_dilig']['tipoobra'],
		),
		array(
			'titulo' 	=> 'RMC - Validação de Deferimento de Reformulação MI para Convencional',
			'qtd_obras'	=> $dadosControle['valida_deferimen_Reform_MI_convenc']['quantidade'],
			'qtd_mun'	=> $dadosControle['valida_deferimen_Reform_MI_convenc']['municipio'],
			'estado'	=> $dadosControle['valida_deferimen_Reform_MI_convenc']['estado'],
			'tipoobra'	=> $dadosControle['valida_deferimen_Reform_MI_convenc']['tipoobra'],
		),
		array(
			'titulo' 	=> 'RMC - Validação do Gestor da Reformulação MI para Convencional',
			'qtd_obras'	=> $dadosControle['valida_gestor_reformu_MI_convenc']['quantidade'],
			'qtd_mun'	=> $dadosControle['valida_gestor_reformu_MI_convenc']['municipio'],
			'estado'	=> $dadosControle['valida_gestor_reformu_MI_convenc']['estado'],
			'tipoobra'	=> $dadosControle['valida_gestor_reformu_MI_convenc']['tipoobra'],
		),
		array(
			'titulo' 	=> 'Obra Aprovada',
			'qtd_obras'	=> $dadosControle['obra_aprovada']['quantidade'],
			'qtd_mun'	=> $dadosControle['obra_aprovada']['municipio'],
			'estado'	=> $dadosControle['obra_aprovada']['estado'],
			'tipoobra'	=> $dadosControle['obra_aprovada']['tipoobra'],
		),
		array(
				'titulo' 	=> 'Total',
				'qtd_obras'	=> ( 	$dadosControle['analise_reformul_MI_convenc']['quantidade'] + $dadosControle['dilig_reformu_MI_convenc']['quantidade'] + 
									$dadosControle['analise_reform_MI_para_convenc_retorn_dilig']['quantidade'] + $dadosControle['valida_deferimen_Reform_MI_convenc']['quantidade'] + 
									$dadosControle['valida_gestor_reformu_MI_convenc']['quantidade'] + $dadosControle['obra_aprovada']['quantidade']
								),
				'qtd_mun'	=> $dadosControle['total_mun_3']['municipio'],
				'estado'	=> $dadosControle['total_mun_3']['estado'],
				'tipoobra'	=> $dadosControle['total_mun_3']['tipoobra'],
		),
		array(
			'titulo' 	=> 'null',
			'qtd_obras'	=> '',
			'qtd_mun'	=> '',
			'estado'	=> '',
			'tipoobra'	=> '',
		),
		array(
			'titulo' 	=> 'Subtotal da fase',
			'qtd_obras'	=> '',
			'qtd_mun'	=> '',
			'estado'	=> '',
			'tipoobra'	=> '',
		),
		array(
			'titulo' 	=> 'Aguardando Aprovação de Prorrogação',
			'qtd_obras'	=> $dadosControle['aguard_aprova_de_prorrog']['quantidade'],
			'qtd_mun'	=>$dadosControle['aguard_aprova_de_prorrog']['municipio'],
			'estado'	=>$dadosControle['aguard_aprova_de_prorrog']['estado'],
			'tipoobra'	=>$dadosControle['aguard_aprova_de_prorrog']['tipoobra'],
		),
		array(
			'titulo' 	=> 'RMC - Em Validação de Indeferimento de Reformulação MI para Convencional',
			'qtd_obras'	=> $dadosControle['valida_indefer_reformu_MI_convenc']['quantidade'],
			'qtd_mun'	=> $dadosControle['valida_indefer_reformu_MI_convenc']['municipio'],
			'estado'	=> $dadosControle['valida_indefer_reformu_MI_convenc']['estado'],
			'tipoobra'	=> $dadosControle['valida_indefer_reformu_MI_convenc']['tipoobra'],
		),
		array(
				'titulo' 	=> 'Total',
				'qtd_obras'	=> ( 	$dadosControle['valida_indefer_reformu_MI_convenc']['quantidade'] +$dadosControle['aguard_aprova_de_prorrog']['quantidade']),
				'qtd_mun'	=> $dadosControle['total_mun_4']['municipio'],
				'estado'	=> $dadosControle['total_mun_4']['estado'],
				'tipoobra'	=> $dadosControle['total_mun_4']['tipoobra'],
		),
	);
}

$dados = montaArrayTabela( $dadosControle );

?>

<div id="dialog" title="Solicitações do Processo" style="display:none;">
</div>

<style>



}
</style>


<form name="form_filtro" id="form_filtro" method="post" action="" >
	
			
		<table  class="tabela"  cellSpacing="1" cellPadding="3" align="center">
		<tr bgcolor="#AAAAAA"  >
	       	<td width="50%"  class="">
	       		<center> &nbsp;<b>Situação da Obra</b></center>
	       	</td>
	        <td >
	        	<center> &nbsp;<b>Quantidade de Obras</b></center>
        	</td>
	        <td >
	        	<center> &nbsp;<b>Quantidade de municípios</b></center>
        	</td>
      	</tr>
      	
		<?php
		
		$i = 1;
      	foreach($dados as $k => $v)
		{
			
			if($v['titulo'] == 'null' || $v['titulo'] == "Subtotal da fase" || $v['titulo'] == "Total")
			{
				if($v['titulo'] == 'null')
				{
					$bgcolor  = "#707070";
					$bgcolorTitle  ="";
					$titulo 	= '';
				}
				elseif($v['titulo'] == "Total")
				{
					$bgcolor  = "#8E8D8D";
					$bgcolorTitle  ="#8E8D8D";
					$qtdObras	= ($v['qtd_obras']) ? $v['qtd_obras'] : 0;
					$qtdMun		= ($v['qtd_mun']) 	? $v['qtd_mun'] : 0;
					$titulo 	= $v['titulo'];
				}
				else
				{
					$bgcolorTitle  ="#AAAAAA";
					
					$titulo 	= $v['titulo'];
					$bgcolor 	=($i % 2) ? "#F5F5F5" : "#D9D9D9";
					$i++;
					
				}
				
				$qtdObras	= $v['qtd_obras'];
				$qtdMun		= $v['qtd_mun'];
				
			}	
			else
			{
				$bgcolorTitle  ="#AAAAAA";
				$bgcolor = ($i % 2) ? "#F5F5F5" : "#D9D9D9";
				$i++;
				$qtdObras	= ($v['qtd_obras']) ? $v['qtd_obras'] : 0;
				$qtdMun		= ($v['qtd_mun']) 	? $v['qtd_mun'] : 0;
				$titulo 	= $v['titulo'];
				
			}
				
			?>
      		<tr bgcolor="<?=$bgcolor?>" id="element">
      		<?php 
      		if($v['titulo'] == 'null' || $v['titulo'] == "Subtotal da fase")
			{
			?>
		       	<td colspan="3" bgcolor="<?=$bgcolorTitle ?>" id="element"   class="">
		       		<center> <b><?= $titulo ?> </b></center>
		       	</td>
	        <?php
			}
			elseif($v['titulo'] == 'Total')
			{
	        ?>
		       	<td bgcolor="<?=$bgcolorTitle ?>" id="element"   class="">
		       		&nbsp;<b><?= $titulo ?>: </b>
		       	</td>
		        <td  width="25%">
		        	<center> <b><?= $qtdObras ?>	 </b></center>			
	        	</td  width="25%">
		        <td>	
		        	<center> <b><?= $qtdMun ?>		 </b></center>
	        	</td>
	      	</tr>
	      	<?php
			}
			else
			{
	        ?>
		       	<td bgcolor="<?=$bgcolorTitle ?>" id="element"   class="">
		       		<center> <b><?= $titulo ?> </b></center>
		       	</td>
		        <td  width="25%">
		        	<center> <b><a class="mostraobra" id="<?php echo $v['estado']; ?>" tipoobra="<?php echo $v['tipoobra'];?>"><?= $qtdObras ?></a>	 </b></center>			
	        	</td  width="25%">
		        <td>	
		        	<center> <b><a class="mostramunicipio" id="<?php echo $v['estado']; ?>" tipoobra="<?php echo $v['tipoobra'];?>"><?= $qtdMun ?></a>	 </b></center>
	        	</td>
	      	</tr>
	      	<?php
			}
	      	?>
      	<?php 
      	}
		?>
		<tr>
			<td colspan="3" bgcolor="" > </td>
		</tr>
		</table>
</form>
<div id="lista">
</div>
<div id="dialog_jquery" title="" style="display: none" >
	<div style="padding:5px;text-align:justify; overflow: scroll; height: 500px;" id="retornoDialog_jquery"></div>
</div>

<script type="text/javascript">
	jQuery('.mostraobra').click(function(){
		var esdid = jQuery(this).attr('id');
		var tipoobra = jQuery(this).attr('tipoobra');

		jQuery.ajax({
			type: "POST",
			url: window.location.href,
			data: "requisicao=mostraDadosObras&esdid="+esdid+"&tipoobra="+tipoobra,
			async: false,
			success: function(msg){
				jQuery( "#dialog_jquery").attr('title', 'Controle Gerencial Reformulações MI para Convencional');
				jQuery( "#dialog_jquery").show();
				jQuery( "#retornoDialog_jquery").html(msg);
				jQuery( '#dialog_jquery').dialog({
						resizable: false,
						width: 800,
						modal: true,
						show: { effect: 'drop', direction: "up" },
						buttons: {
							'Fechar': function() {
								jQuery( "#retornoDialog_jquery").html('');
								jQuery( this ).dialog( 'close' );
							}
						}
				});
			}
		});
		
	});
	
	jQuery('.mostramunicipio').click(function(){
		var esdid = jQuery(this).attr('id');
		var tipoobra = jQuery(this).attr('tipoobra');

		jQuery.ajax({
			type: "POST",
			url: window.location.href,
			data: "requisicao=mostraDadosMunicipio&esdid="+esdid+"&tipoobra="+tipoobra,
			async: false,
			success: function(msg){
				jQuery( "#dialog_jquery").attr('title', 'Controle Gerencial Reformulações MI para Convencional');
				jQuery( "#dialog_jquery").show();
				jQuery( "#retornoDialog_jquery").html(msg);
				jQuery( '#dialog_jquery').dialog({
						resizable: false,
						width: 800,
						modal: true,
						show: { effect: 'drop', direction: "up" },
						buttons: {
							'Fechar': function() {
								jQuery( "#retornoDialog_jquery").html('');
								jQuery( this ).dialog( 'close' );
							}
						}
				});
			}
		});
		
	});
</script>