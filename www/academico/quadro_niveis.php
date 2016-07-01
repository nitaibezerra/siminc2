<?php
//die;
header( 'Content-type: text/html; charset=iso-8859-1' );

//
//echo "<pre>";
//var_dump($_REQUEST);
//die;

// carrega as funções gerais
include_once "config.inc";
//include "verificasistema.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";


// carrega as funções específicas do módulo
include_once '_constantes.php';
//include_once '_funcoes.php';
//include_once '_componentes.php';


// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$tpeid = $_REQUEST['tpeid'];
$edpid = $_REQUEST['edpid'];

if ( !$tpeid || !$edpid ){
	die();	
}

$select    = array();
$selectInt = array();
$from      = array();
$where     = array(); 

$sql = "SELECT
			tpeid
		FROM
			academico.editalportaria
		WHERE
			edpid = " . $edpid;

$tpeidSql = $db->pegaUm($sql);


if ( $tpeidSql == ACA_TPEDITAL_PUBLICACAO ){
	switch ( $tpeid ){
		CASE ACA_TPEDITAL_PUBLICACAO:		
			$titulo = "Totais Publicados";
			$select[] = "COALESCE(SUM(publicado), 0) AS somatoria";
			$selectInt[] = "COALESCE( SUM(lep.lepvlrpublicacao), 0) as publicado"; 
			$from[]   = "INNER JOIN academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid AND
											    							  lep.lepstatus = 'A'";
		BREAK;
		CASE ACA_TPEDITAL_HOMOLOGACAO:
			$titulo = "Totais Homologados";
			$select[] = "COALESCE(SUM(homologado), 0) AS somatoria";
			$selectInt[] = "COALESCE( SUM(lep.lepvlrhomologado), 0) as homologado"; 
			$from[] = "INNER JOIN academico.editalportaria ep1 ON ep1.edpidhomo = ep.edpid AND 
																  ep1.edpstatus = 'A'";
			$from[] = "INNER JOIN academico.lancamentoeditalportaria lep ON lep.edpid = ep1.edpid AND
											     							lep.lepstatus = 'A'";
		BREAK;
		CASE ACA_TPEDITAL_NOMEACAO:
			$titulo = "Totais Efetivados";		
			$select[] = "COALESCE(SUM(efetivado), 0) AS somatoria";
			$selectInt[] = "COALESCE( SUM(lep.lepvlrprovefetivados), 0) AS efetivado"; 
			$from[] = "INNER JOIN academico.editalportaria ep1 ON ep1.edpidhomo = ep.edpid AND 
																  ep1.edpstatus = 'A'";
			$from[] = "INNER JOIN academico.editalportaria ep2 ON ep2.edpideditalhomologacao = ep1.edpid AND 
																  ep2.edpstatus = 'A'";
			$from[] = "INNER JOIN academico.lancamentoeditalportaria lep ON lep.edpid = ep2.edpid AND
											     							lep.lepstatus = 'A'";		
		BREAK;	
	}
	
	$sql = "SELECT
				" . ($select ? implode(" , ", $select) . "," : '') . "
				c.clsdsc
			FROM 
				academico.classes c
			INNER JOIN academico.cargos cg ON cg.clsid = c.clsid AND 
							 cg.crgstatus = 'A'
			LEFT JOIN (SELECT
					" . ($selectInt ? implode(" , ", $selectInt) . "," : '') . "
					lep.crgid
				   FROM
					academico.editalportaria ep
					" . ($from ? implode(" ", $from) : '') . "
				   WHERE
				   	" . ($where ? implode(" AND ", $where) : '') . "
					ep.edpstatus = 'A' AND
					ep.tpeid in ( " . ACA_TPEDITAL_PUBLICACAO . " ) AND
					ep.edpid = {$edpid}
				   GROUP BY 
					lep.crgid
					) lep ON lep.crgid = cg.crgid
			GROUP BY
				c.clsdsc,
				c.clsid
			ORDER BY
				c.clsid
	";
}elseif ( $tpeidSql == ACA_TPEDITAL_HOMOLOGACAO ){
	$titulo = "Totais Homologados";
	
	$sql = "SELECT
				COALESCE(SUM(lep.homologado ), 0) AS somatoria,
				c.clsdsc
			FROM 
				academico.classes c
				INNER JOIN academico.cargos cg ON cg.clsid = c.clsid AND 
								  cg.crgstatus = 'A'
				LEFT JOIN (
							SELECT
								COALESCE(SUM(lep.lepvlrhomologado ), 0) AS homologado,
								lep.crgid
							FROM
								academico.editalportaria ep 
							INNER JOIN academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid and 
													     lep.lepstatus = 'A' 
							WHERE 
								ep.edpstatus = 'A' AND
								ep.tpeid in ( " . ACA_TPEDITAL_HOMOLOGACAO . " ) AND
								ep.edpid = {$edpid}
							GROUP BY
								lep.crgid
				) lep ON lep.crgid = cg.crgid
			GROUP BY 	
				c.clsdsc,
				c.clsid
			ORDER BY
				c.clsid";
	
	
}elseif (  $tpeidSql == ACA_TPEDITAL_NOMEACAO  ){
	$titulo = "Totais Efetivados";
	
	$sql = "SELECT
				COALESCE(SUM(lep.efetivado ), 0) as somatoria,
				c.clsdsc
			FROM 
				academico.classes c
				INNER JOIN academico.cargos cg ON cg.clsid = c.clsid AND 
								  cg.crgstatus = 'A'
				LEFT JOIN (
							SELECT
								COALESCE(SUM(lep.lepvlrprovefetivados ), 0) AS efetivado,
								lep.crgid
							FROM
								academico.editalportaria ep 
							INNER JOIN academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid and 
													     lep.lepstatus = 'A' 
							WHERE 
								ep.edpstatus = 'A' AND
								ep.tpeid in ( " . ACA_TPEDITAL_NOMEACAO . " ) AND
								ep.edpid = {$edpid}
							GROUP BY
								lep.crgid
				) lep ON lep.crgid = cg.crgid
			GROUP BY 	
				c.clsdsc,
				c.clsid
			ORDER BY
				c.clsid";
}
//dbg($sql,1);

$dados = $db->carregar($sql);

//dbg($dados, 1);
?>

<br/>
<table border="0">
	<tr>
		<td colspan="2"><?=$titulo?></td>
	</tr>
<? 
	if ( is_array($dados) && !empty($dados) ){
		foreach($dados as $val){				
			echo "<tr>
					<td><b>{$val['clsdsc']}</b></td>
					<td>{$val['somatoria']}</td>		
				  </tr>";
		}
	}
?>
</table>