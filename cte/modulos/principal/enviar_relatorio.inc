<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?php

cte_verificaSessao();
$muncod = cte_pegarMuncod( $_SESSION['inuid'] );
$descricao = cte_pegarMundescricao( $muncod );
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, $descricao );

$estado	= cte_pegarMuncodEstatual($_SESSION["inuid"]);
$itrid 	= cte_pegarItrid($_SESSION["inuid"]); 

?> 
<table class="tabela" align="center">
<tr>
<td>
<?
$inuid = (integer) $_SESSION['inuid'];

function recuperavaloresgerais($subacao, $escola){
		global $db;
		
		if($escola == 't'){ // SQL quando a subação for por escola
			$select = "	sum(cos.cosvlruni * ecs.ecsqtd) 		AS valor";
			$inner = "INNER JOIN cte.escolacomposicaosubacao ecs ON cos.cosid = ecs.cosid ";
			
		}else{ // SQL quando a subação for global
			$select = "	sum(cos.cosqtd * cos.cosvlruni ) 			AS valor";
			$inner = "INNER JOIN cte.subacaoparecertecnico spt ON sba.sbaid = spt.sbaid AND sptano = date_part('year', current_date)";
		}
		
		$sql=	"	select ".$select."	
					from 
						cte.subacaoindicador sba
					INNER JOIN
						cte.composicaosubacao cos ON sba.sbaid = cos.sbaid AND cosano = date_part('year', current_date)
					".$inner."
					where sba.sbaid = ".$subacao."
					group by sba.sbaid";
		return  $db->pegaUm( $sql );
	}

function recuperaPlanointernoAcaoSubacao(){
		global $db;
		
		$municipio = cte_pegarMuncod( $_SESSION["inuid"] );
		if($municipio != NULL){ 		//*** Municipio ***//
			$select  = "m.muncod		AS codigoibge,
					    m.mundescricao	AS nomeentidade,
					    m.mundescricao 	AS nomeMunicipio,";
			$from    = "INNER JOIN territorios.municipio  m  ON m.muncod = iu.muncod";
			$where   = "m.muncod = '".$municipio."' AND 
						iu.inuid 	='".$_SESSION["inuid"]."' and" ; 
			
		}else{ 							//*** Estado ***//
			$estado	= cte_pegarMuncodEstatual($_SESSION["inuid"]);
			$itrid 	= cte_pegarItrid($_SESSION["inuid"]);  
			
			$select = "m.estdescricao	AS nomeentidade,";
			$from   = "INNER JOIN territorios.estado m ON m.estuf = iu.estuf";
			$where  = "m.muncodcapital 	= '".$estado."' 
					   AND iu.inuid 	= '".$_SESSION["inuid"]."' 
					   AND iu.itrid 	= '".$itrid."' AND";
		}
	
	$sql = "
				SELECT 
						di.dimcod as d,
						ad.ardcod as a,
						i.indcod  as i,
						c.ctrpontuacao as c,
						s.sbadsc  as subacao,
						u.unddsc  as unidadedemedida,
						'' as ano2008,
						--'' as total,
						--'' as envio,
						s.sbaid							AS idsubacao,
						s.sbaporescola					AS cronogramaporescola		
				FROM cte.dimensao d
					INNER JOIN cte.areadimensao 	  ad  ON ad.dimid = d.dimid
					inner join cte.dimensao 		  di on di.dimid = ad.dimid
					INNER JOIN cte.indicador 	  	  i   ON i.ardid  = ad.ardid
					INNER JOIN cte.criterio		  	  c   ON c.indid  = i.indid
					INNER JOIN cte.pontuacao 	  	  p   ON p.crtid  = c.crtid and p.indid = i.indid and p.ptostatus = 'A'
					INNER JOIN cte.instrumentounidade iu  ON iu.inuid = p.inuid
					".$from."
					INNER JOIN cte.acaoindicador 	  		a   ON a.ptoid   = p.ptoid
					INNER JOIN cte.subacaoindicador   		s   ON s.aciid   = a.aciid
					LEFT JOIN cte.subacaoparecertecnico 	spt ON spt.sbaid = s.sbaid and sptano = date_part('year', current_date)
					INNER JOIN cte.unidademedida 	  		u   ON u.undid   = s.undid
					INNER JOIN cte.programa 	  	  pr  ON pr.prgid = s.prgid
					LEFT  JOIN cte.convenioretorno    cvr ON pr.prgplanointerno = cvr.prgplanointerno and cvr.inuid = iu.inuid
				WHERE
					".$where."
					 s.frmid in(11,3)   AND -- assistencia financeira.
					spt.ssuid = 3 	 	AND -- aprovada pela comissão.
					s.sbaid in ( select sac.sbaid from cte.subacaoconvenio sac ) -- subacões que foram enviadas para o convênio ainda.
				ORDER BY 
					di.dimcod,
					ad.ardcod,
					i.indcod,
					c.ctrpontuacao";
	//dbg($sql,1);
		$dados = $db->carregar( $sql );
		return  $dados;
	}

$dados = recuperaPlanointernoAcaoSubacao();

$total = count($dados);

if(is_array($dados)){
	for($cont = 0; $cont < $total; $cont++ ){
		$valor =  recuperavaloresgerais($dados[$cont]['idsubacao'], $dados[$cont]['cronogramaporescola']);
		$valor = number_format($valor, 2, ',', '.');
		$dados[$cont]['ano2008'] = $valor;
		array_pop($dados[$cont]);
		array_pop($dados[$cont]);
	}
}

$colunas = array(
	"D",
	"A",
	"I",
	"C",
	"Subação",
	"Unidade de Medida",
	"Valor 2008"
);
$db->monta_lista_array( $dados, $colunas, 29, 10000, null, null, null );

?>
</td>
</tr>
<td align='right'>D - Dimensão | A - Área | I - Indicador | C - Critério</td>
</tr>
</table>