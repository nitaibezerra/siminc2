<?php

ini_set( "memory_limit", "1024M" );

function pegaSql(){
	
	$sql = "select 
	cod_ibge, uf, municipio, 
	( SELECT sum(mprqtd::integer) FROM par.municipioadesaoprograma WHERE pfaid = 5 and muncod = cod_ibge  )  as cota2012,
	qtd_solicitada_2012, 
	qtd_aprovada_2012, 
	qtd_empenhada_2012, 
	aderiu_2012,
	( SELECT sum(mprqtd::integer) FROM par.municipioadesaoprograma WHERE pfaid = 5 and muncod = cod_ibge  )::integer - qtd_empenhada_2012 as cota_2013,
	qtd_solicitada_2013, 
	qtd_aprovada_2013, 
	qtd_empenhada_2013,
	aderiu_2013,
	-- saldo as saldo,
	CASE WHEN entrou = 1 THEN saldo ELSE ( ( SELECT sum(mprqtd::integer) FROM par.municipioadesaoprograma WHERE pfaid = 5 and muncod = cod_ibge  ) - (qtd_empenhada_2012 + qtd_empenhada_2013) )  END AS saldo,
	aderiu_algum_ano
from (
select  muncod as cod_ibge, uf, municipio, 
		sum(cota::integer) as  cota_2012, 
		sum( qtd_solicitada_2012) as  qtd_solicitada_2012, 
		sum( qtd_aprovada_2012) as  qtd_aprovada_2012, 
		sum( qtd_empenhada_2012)::integer as  qtd_empenhada_2012, 
		CASE WHEN sum(aderiu_2012)::integer > 0 then  
			'Sim'
		ELSE
			'Não'
		END
			as aderiu_2012,
		sum(cota2013) as  cota_2013,
		sum( qtd_solicitada_2013) as  qtd_solicitada_2013, 
		sum( qtd_aprovada_2013) as  qtd_aprovada_2013, 
		sum( qtd_empenhada_2013)::integer as  qtd_empenhada_2013,
		CASE WHEN sum(aderiu_2013)::integer > 0 then
			'Sim'
		ELSE
			'Não'
		END
			as aderiu_2013,
		(sum(cota) -  ( sum( qtd_empenhada_2013)::integer + sum( qtd_empenhada_2012)::integer) ) as saldo,
		
		CASE 
		WHEN sum(aderiu_2012)::integer > 0 then
			'Sim'
		WHEN sum(aderiu_2013)::integer > 0 then
			'Sim'
		ELSE
			'Não'
		END
			as aderiu_algum_ano	,
			1 as entrou	
	from (

		select  m.muncod, m.estuf AS uf, m.mundescricao as municipio,
			( SELECT sum(mprqtd::integer) FROM par.municipioadesaoprograma WHERE pfaid = 5 and muncod = m.muncod  )  as cota,
			0 as cota2013,
			sum(sic.icoquantidade) as  qtd_solicitada_2012,
			sum(sic.icoquantidadetecnico)  qtd_aprovada_2012,
			(
				select (sum(es.eobvalorempenho) / 132000.00) from par.empenhosubacao es
				inner join par.empenho e on e.empid = es.empid and empsituacao = '2 - EFETIVADO' and eobstatus = 'A' and empstatus = 'A' 
				inner join par.subacao sa1  ON es.sbaid = sa1.sbaid AND sa1.sbastatus = 'A' and sa1.ppsid IN(1074,1075) 
				where eobano = 2012 and sa1.sbaid = sa.sbaid
			) AS  qtd_empenhada_2012,
			
			(

				SELECT
					
					CASE WHEN pfa.adpresposta = 'S' THEN
						1
					ELSE
						0
					END
					
						
				from 
						par.pfadesaoprograma pfa 
				inner join par.instrumentounidade iuu on iuu.inuid = pfa.inuid
				left join territorios.municipio m on m.muncod = iuu.muncod
				where 
						pfa.pfaid in (5)
				AND 
					iuu.inuid =	
				iu.inuid
				limit 1

			)
			as aderiu_2012,
			0 as  qtd_solicitada_2013,
			0 as  qtd_aprovada_2013,
			0 as  qtd_empenhada_2013,
			(

				SELECT
					
					CASE WHEN pfa.adpresposta = 'S' THEN
						1
					ELSE
						0
					END
					
						
				from 
						par.pfadesaoprograma pfa 
				inner join par.instrumentounidade iuu on iuu.inuid = pfa.inuid
				left join territorios.municipio m on m.muncod = iuu.muncod
				where 
						pfa.pfaid in (11)
				AND 
					iuu.inuid = iu.inuid
				
				limit 1

			)
			as aderiu_2013

		FROM territorios.municipio m
		INNER JOIN par.instrumentounidade   iu  ON iu.muncod = m.muncod
		INNER JOIN par.pontuacao p ON p.inuid = iu.inuid AND p.ptostatus = 'A'
		INNER JOIN par.acao a ON p.ptoid = a.ptoid AND a.acistatus = 'A'
		INNER JOIN par.subacao sa  ON sa.aciid = a.aciid AND sa.sbastatus = 'A'
		LEFT JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sa.sbaid  AND sic.icoano = '2012'
		where sa.ppsid IN(1074,1075)   
		-- and m.muncod = '1200708'
		 AND iu.muncod in ( SELECT muncod FROM par.municipioadesaoprograma WHERE pfaid = 5 ) 
		group by sa.sbaid, m.muncod, m.estuf , m.mundescricao, iu.inuid
		
	union all
		select   m.muncod, m.estuf AS uf, m.mundescricao as municipio, 
			0  as cota,
			( 
				select sum(ma.mprqtd::integer)::integer - ( 
					SELECT	CASE WHEN f.sbaid IS NULL THEN 0 ELSE (COALESCE(SUM(icoquantidadetecnico),0)) END AS qtdAderido
					FROM par.subacaoitenscomposicao i
					LEFT JOIN 
						( SELECT DISTINCT sbaid 
							FROM par.empenhosubacao es
							INNER JOIN 	par.empenho e ON e.empid = es.empid and empstatus = 'A'
							WHERE 		eobstatus = 'A' AND e.empsituacao = '2 - EFETIVADO' AND eobano = '2012'
						) AS f ON f.sbaid = i.sbaid						
					WHERE 		i.sbaid = sa.sbaid AND i.icoano = 2012 AND i.icovalidatecnico = 'S'
					GROUP BY  	f.sbaid
					)::integer 
				FROM par.municipioadesaoprograma ma WHERE pfaid = 5 AND ma.muncod =  m.muncod

			 ) as cota2013,
			0 as  qtd_solicitada_2012,
			0 as  qtd_aprovada_2012,
			0 as  qtd_empenhada_2012,
			0 as aderiu_2012,
			sum(sic.icoquantidade) as  qtd_solicitada_2013,
			sum(sic.icoquantidadetecnico)  qtd_aprovada_2013,
			(
				select (sum(es.eobvalorempenho) / 132000.00) from par.empenhosubacao es
				inner join par.empenho e on e.empid = es.empid and empsituacao = '2 - EFETIVADO' and eobstatus = 'A' and empstatus = 'A' 
				inner join par.subacao sa1  ON es.sbaid = sa1.sbaid AND sa1.sbastatus = 'A' and sa1.ppsid IN(1074,1075) 
				where eobano = 2013 and sa1.sbaid = sa.sbaid
			) AS  qtd_empenhada_2013,
			0 as aderiu_2013
		--	iu.inuid
			
		FROM territorios.municipio m
		INNER JOIN par.instrumentounidade   iu  ON iu.muncod = m.muncod
		INNER JOIN par.municipioadesaoprograma ma ON ma.muncod = m.muncod AND ma.pfaid = 11
		INNER JOIN par.pontuacao p ON p.inuid = iu.inuid AND p.ptostatus = 'A'
		INNER JOIN par.acao a ON p.ptoid = a.ptoid AND a.acistatus = 'A'
		INNER JOIN par.subacao sa  ON sa.aciid = a.aciid AND sa.sbastatus = 'A'
		LEFT JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sa.sbaid  AND sic.icoano = '2013'
		where sa.ppsid IN(1074,1075)  -- and m.muncod = '1200708'
		 AND iu.muncod in ( SELECT muncod FROM par.municipioadesaoprograma WHERE pfaid = 11 )
		group by sa.sbaid, m.estuf,  m.muncod, m.mundescricao
	) as foo -- where muncod = '1200708' 
	group by muncod, uf,  municipio

union all

	SELECT m.muncod as cod_ibge , m.estuf AS uf, m.mundescricao as municipio,
		sum(ma.mprqtd::integer)::integer  as  cota, 
		'0'  as  cota2013,
		0 as  qtd_solicitada_2012, 
		0 as  qtd_aprovada_2012, 
		'0' as  qtd_empenhada_2012,
		'0' as aderiu_2012, 
		0 as  qtd_solicitada_2013, 
		0 as  qtd_aprovada_2013, 
		0 as qtd_empenhada_2013,
		'0' as aderiu_2013,
		0 as saldo,
		'0' aderiu_algum_ano,
		0 as  entrou	
	FROM par.municipioadesaoprograma ma 
	inner join  territorios.municipio m on  m.muncod = ma.muncod
	WHERE pfaid = 5
	and m.muncod not in (
		select  muncod
		from (
			select  m.muncod
			FROM territorios.municipio m
			INNER JOIN par.instrumentounidade   iu  ON iu.muncod = m.muncod
			INNER JOIN par.pontuacao p ON p.inuid = iu.inuid AND p.ptostatus = 'A'
			INNER JOIN par.acao a ON p.ptoid = a.ptoid AND a.acistatus = 'A'
			INNER JOIN par.subacao sa  ON sa.aciid = a.aciid AND sa.sbastatus = 'A'
			LEFT JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sa.sbaid  AND sic.icoano = '2012'
			where sa.ppsid IN(1074,1075)   
			 and m.muncod = '1200708'
			 AND iu.muncod in ( SELECT muncod FROM par.municipioadesaoprograma WHERE pfaid = 5 )
			group by sa.sbaid, m.muncod, m.mundescricao
		
		union all
			
			select   m.muncod
			FROM territorios.municipio m
			INNER JOIN par.instrumentounidade   iu  ON iu.muncod = m.muncod
			INNER JOIN par.municipioadesaoprograma ma ON ma.muncod = m.muncod AND ma.pfaid = 11
			INNER JOIN par.pontuacao p ON p.inuid = iu.inuid AND p.ptostatus = 'A'
			INNER JOIN par.acao a ON p.ptoid = a.ptoid AND a.acistatus = 'A'
			INNER JOIN par.subacao sa  ON sa.aciid = a.aciid AND sa.sbastatus = 'A'
			LEFT JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sa.sbaid  AND sic.icoano = '2013'
			where sa.ppsid IN(1074,1075)  -- and m.muncod = '1200708'
			 AND iu.muncod in ( SELECT muncod FROM par.municipioadesaoprograma WHERE pfaid = 11 )
			group by sa.sbaid, m.muncod, m.mundescricao
		) as foo 
	)  
	group by m.muncod, m.estuf,  m.mundescricao

	) as foo";
	
	return $sql;
}

function pegaCabecalho(){
	
	$cabecalho = Array("IBGE","UF","Município","Cota 2012","Quantidade solicitada 2012","Quantidade aprovada 2012","Quantidade empenahda 2012","Aderiu 2012",
					   "Cota 2013","Quantidade solicitada 2013","Quantidade aprovada 2013","Quantidade empenhada 2013", "Aderiu 2013", "Saldo", "Aderiu algum ano");
	
	return $cabecalho;
}
function lista(){
	
	global $db;
	
	$sql = pegaSql();
	
	$dadosLista = $db->carregar($sql);
	$cabecalho 	= pegaCabecalho();
	$db->monta_lista_array($dadosLista, $cabecalho,50,10,'N','center');
	
}

function xls(){
	
	global $db;
	
	ob_clean();
	$sql 		= pegaSql();
	$cabecalho 	= pegaCabecalho();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatTransp".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_RelatTransp".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($sql,$cabecalho,100000,5,'N','100%');
	exit;
}

if( $_REQUEST['req'] ){
	$_REQUEST['req']($_REQUEST);
	die();
}

include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';
monta_titulo( $titulo_modulo, '' ); 

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

$(document).ready(function(){
	$.ajax({
		type	: "POST",
		url		: window.location,
		data	: {'req' : 'lista'},
		success	: function (data){
			$('#lista').html(data.replace("formlista","") );
			$('#lista').append("<form method='post' name='formlista' id='name='formlista'>"
								+ "<input type='Hidden' value='' name='formlista_numero'>"
								+ "<input type='Hidden' value='' name='formlista_ordemlista'>"
								+ "<input type='Hidden' value='' name='formlista_ordemlistadir'>"
								+ "<input type='hidden' value='lista' name='req'>"
								+ "</form>");
			$('#carregando').hide();
		}
	});	

	$('.xls').click(function(){
		windowOpen( window.location.href+'&req=xls','blank','height=50,width=50,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	});
});
</script>
<div style="width:110%;height:110%;text-align:center;margin-left:-5px;margin-top:-3%;z-index: 0;position:absolute;background-color:white;opacity:0.65" id="carregando">
	<div style="margin-top:15%;width:91%">
		<img src="../imagens/carregando.gif" border="0" align="middle"><br />Aguarde. Carregando...
	</div>
</div>
<form method="post" name="formulario" id="formulario">
	<input type="hidden" id="exibirxls" name="exibirxls" value="">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr bgcolor="#D0D0D0">
			<td align="center" colspan="2">
				<input type="button" name="buscar" value="Relatório XLS 2012/2013" class="xls"/>
			</td>
		</tr>
	</table>
</form>
<br>
<div  id="lista">

</div>