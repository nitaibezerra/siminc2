<?php
include "resultSession.php";

class ParametroComponenteGrafico 
{
	private $condicoesWhere;
	private $campoRepresentacaoMaior;
	private $campoPorcaoSomatorio;
	private $httpPost;
	
	public function ParametroComponenteGrafico($httpPost) {
		$this->condicoesWhere = " ";
		$this->httpPost = $httpPost;
	}
	
	function settarAgrupamentosMaiorEMaior() {
		$this->campoRepresentacaoMaior = $this->obterCampoParaSql("nivelrepresentacaomaior");
		$this->campoPorcaoSomatorio    = $this->obterCampoParaSql("nivelporcaosomatorio");
	}
	
	function obterCampoParaSql($nomeAtributo) {
		$nivel = $this->lerParametroHttp($nomeAtributo);
		switch ($nivel) {
			case 1: return " 'BRASIL'		"; break;
			case 2: return " ende.estuf 	"; break;
			case 3: return " m.mundescricao	"; break;
			case 4: return " CASO ESPECIAL  "; break; // TODO caso especial   componente   lancar EXCEPTION 
			case 5: return " a.atvdescricao "; break;  
			case 6: return " NULL 			"; break;
			default: return " ";
		}
		return " ";
	}
	
	function lerParametroHttp($nomeAtributo) {
		return $this->httpPost[$nomeAtributo]; 
	}	
	
	function settarConformeArrayParametros() {
		$this->addCondicaoWhereConformeFiltro(" AND ende.estuf ", "estado" ); 
		$this->addCondicaoWhereConformeFiltro(" AND m.muncod "  , "municipio" ); 
		$this->addCondicaoWhereConformeFiltroComVetor(" AND a.atvid " , array("comp1","comp2","comp3","comp4") ); 
	}

	function addCondicaoWhereConformeFiltro($subClausulaWhere, $nomeAtributo) {
		if( $this->estaPreenchido($nomeAtributo) ) {
			$valoresSeparadosPorVirgula = implode( "','", $this->lerParametroHttp($nomeAtributo) );
			$this->condicoesWhere .= $subClausulaWhere . " IN  ('" . $valoresSeparadosPorVirgula . "') ";
		}
	}

	function estaPreenchido($nomeAtributo) {
		if( $this->httpPost[$nomeAtributo."_campo_flag"] ) {
			return true;			
		}
		return false;
	}
		
	function addCondicaoWhereConformeFiltroComVetor($subClausulaWhere, $vetorNomesAtributos = array()) {
		foreach( $vetorNomesAtributos as $nomeAtributo ) {
			if( $this->estaPreenchido($nomeAtributo) ) {
				$haValorAcumulado = true;
				$valoresSeparadosPorVirgula = implode( "','", $this->lerParametroHttp($nomeAtributo) );
				$acumuladorValores .= "'" . $valoresSeparadosPorVirgula . "' ";
			}
		}
		if( $haValorAcumulado ) {
			$this->condicoesWhere .= $subClausulaWhere . " IN  (" . $acumuladorValores . ") ";
		}
	}
	 	
	function getCondicoesWhere() {
		return $this->condicoesWhere; 
	}
	
	function getCampoRepresentacaoMaior() {
		return $this->campoRepresentacaoMaior; 
	}

	function getCampoPorcaoSomatorio() {
		return $this->campoPorcaoSomatorio; 
	}	
}

function obterSqlGrafico($httpPost) {
	performSessionConfiguration();
	$parametros = new ParametroComponenteGrafico($httpPost);
	$parametros->settarAgrupamentosMaiorEMaior();
	$parametros->settarConformeArrayParametros();
	
	// caso mais comum
	return obterSqlGraficoCasoMaisComum($parametros);

	// caso especial: componente/subcomponente ... 
	// só precisará se o usuário desejar ver ambos juntos .......... 
	// 1o momento .... despreze o subcomponente ..........  
	// ... 
}

function obterSqlGraficoCasoMaisComum($parametros) {
	$sql = "
			SELECT * FROM (
				SELECT distinct
					". $parametros->getCampoRepresentacaoMaior() ." as representacaoMaior   ,
					". $parametros->getCampoPorcaoSomatorio()    ." as campoPorcaoSomatorio , 
					sum(q.qtdescola) as quantidade
				FROM 
					pse.componente c
				INNER JOIN pse.atividade a ON c.copid = a.copid 
				INNER JOIN pse.quantidadeescola q ON a.atvid = q.atvid
				INNER JOIN pse.estadomunicipiopse e ON e.empid = q.empid
				INNER JOIN territorios.municipio m ON m.muncod = e.muncod
				INNER JOIN entidade.endereco ende ON ende.muncod = e.muncod
				
				WHERE
					a.copid IN (2,3,4,5,6,7,8)
					".$parametros->getCondicoesWhere()."
					AND q.atvid IS NOT NULL  
				GROUP BY 
					representacaoMaior    , 
					campoPorcaoSomatorio  
			
				UNION ALL 
			
				SELECT distinct
					NULL as representacaoMaior   ,
					NULL as campoPorcaoSomatorio , 
					0 as quantidade
				FROM 
					pse.componente c 
					LEFT JOIN pse.atividade a ON c.copid = a.copid 
			
				WHERE
					a.atvid NOT in (
						SELECT distinct
							a.atvid as pergunta
						FROM 
							pse.componente c
						INNER JOIN pse.atividade a ON c.copid = a.copid 
						INNER JOIN pse.quantidadeescola q ON a.atvid = q.atvid
						INNER JOIN pse.estadomunicipiopse e ON e.empid = q.empid
						INNER JOIN territorios.municipio m ON m.muncod = e.muncod
						INNER JOIN entidade.endereco ende ON ende.muncod = e.muncod
						WHERE
							a.copid IN (2,3,4,5,6,7,8) 
							".$parametros->getCondicoesWhere()." 
						)
			) AS main
			
			ORDER BY
				1 DESC 
	";
	
	//ver($sql);
	
	return $sql;
}

function obterSqlGraficoCasoComponenteSubComponente() {
	$parametros = obterParametros();
	
	$sql = "
			SELECT * FROM (
				SELECT distinct
					ende.uf  as estado , 
					sum(q.qtdescola) as quantidade
				FROM 
					pse.componente c
				INNER JOIN pse.atividade a ON c.copid = a.copid 
				INNER JOIN pse.quantidadeescola q ON a.atvid = q.atvid
				INNER JOIN pse.estadomunicipiopse e ON e.empid = q.empid
				INNER JOIN territorios.municipio m ON m.muncod = e.muncod
				INNER JOIN entidade.endereco ende ON ende.muncod = e.muncod
				
				WHERE
					a.copid IN (2,3,4,5,6,7,8)
					".$parametros->getCondicoesWhere()."
					AND q.atvid IS NOT NULL  
				GROUP BY 
					estado 
			
				UNION ALL 
			
				SELECT distinct
					NULL as estado , 
					0 as quantidade
				FROM 
					pse.componente c 
					LEFT JOIN pse.atividade a ON c.copid = a.copid 
			
				WHERE
					a.atvid NOT in (
						SELECT distinct
							a.atvid as pergunta
						FROM 
							pse.componente c
						INNER JOIN pse.atividade a ON c.copid = a.copid 
						INNER JOIN pse.quantidadeescola q ON a.atvid = q.atvid
						INNER JOIN pse.estadomunicipiopse e ON e.empid = q.empid
						INNER JOIN territorios.municipio m ON m.muncod = e.muncod
						INNER JOIN entidade.endereco ende ON ende.muncod = e.muncod
						WHERE
							a.copid IN (2,3,4,5,6,7,8) 
							".$parametros->getCondicoesWhere()." 
			) AS main
			
			ORDER BY
				1,2
	";
	
	return $sql;
}

function obterSqlComboBoxEstados() {
	return "SELECT	estuf AS codigo,
					estdescricao AS descricao
			FROM  	territorios.estado";;
}

function obterSqlComboBoxMunicipios() {
	return "SELECT	  muncod AS codigo,
					  estuf || ' - ' || mundescricao AS descricao
			FROM	  territorios.municipio	
			WHERE  	  1=1
			ORDER BY  descricao";
}

function obterSqlComboBoxComponenteUm() {
	return "SELECT	  atvid as codigo, 
					  atvdescricao as descricao
			FROM	  pse.atividade
			WHERE	  copid IN (2,3,4)
			ORDER BY  atvid";
}

function obterSqlComboBoxComponenteDois() {
	return "SELECT 	  atvid as codigo, 
					  atvdescricao as descricao
			FROM	  pse.atividade
			WHERE 	  copid IN (5)
			ORDER BY  atvid";
}

function obterSqlComboBoxComponenteTres() {
	return "SELECT	  atvid as codigo, 
					  atvdescricao as descricao
			FROM 	  pse.atividade
			WHERE 	  copid IN (6)
			ORDER BY  atvid"; 
} 

function obterSqlComboBoxComponenteQuatro() {
	return "SELECT 	  atvid as codigo, 
					  atvdescricao as descricao
			FROM 	  pse.atividade
			WHERE 	  copid IN (7,8)
			ORDER BY  atvid";
}

function retornaGrafico ( $titulo , $dados ){
	$n = 1;
	foreach( $dados as $valor ){
		$arrValor[] = new pie_value ( (int)$valor['quantidade'] , $n );
		$n++;
	}
	
	$title = new title( $titulo );
	$title->set_style( "{font-size: 16px; font-weight: bold; text-align: center}" );
	
	$pie = new pie();
	$pie->set_alpha(1.0);
	$pie->set_start_angle( 35 );
	$pie->add_animation( new pie_fade() );
	$pie->set_tooltip( '#val# de #total#<br>#percent# de 100%' );
	
	$arraCores = array('#6495ED','#66CDAA','#990000','#FFD700','#CDC8B1',' #000000','#FF0000','#008B45','#8B008B','#FFE4E1','#0000FF',' #7CFC00','#8B4513','#FF1493','#FFFAFA','#00008B','#7FFFD4','#8B8B00','#FF6A6A','#8B1A1A','#8B0A50','#828282');
	
	$pie->set_colours( $arraCores );
	
	$pie->set_values( $arrValor );
	
	$chart = new open_flash_chart();
	$chart->set_title( $title );
	$chart->add_element( $pie );
	$chart->set_bg_colour( '#ffffff' );
	
	$chart->x_axis = null;
	
	return $chart->toPrettyString();
	
}

function removeacentosGrafico ($var)
{
       $ACENTOS   = array("À","Á","Â","Ã","à","á","â","ã");
       $SEMACENTOS= array("A","A","A","A","a","a","a","a");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
      
       $ACENTOS   = array("È","É","Ê","Ë","è","é","ê","ë");
       $SEMACENTOS= array("E","E","E","E","e","e","e","e");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
       $ACENTOS   = array("Ì","Í","Î","Ï","ì","í","î","ï");
       $SEMACENTOS= array("I","I","I","I","i","i","i","i");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
      
       $ACENTOS   = array("Ò","Ó","Ô","Ö","Õ","ò","ó","ô","ö","õ");
       $SEMACENTOS= array("O","O","O","O","O","o","o","o","o","o");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
     
       $ACENTOS   = array("Ù","Ú","Û","Ü","ú","ù","ü","û");
       $SEMACENTOS= array("U","U","U","U","u","u","u","u");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
       $ACENTOS   = array("Ç","ç","ª","º","°");
       $SEMACENTOS= array("C","c","a.","o.","o.");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);      

       return $var;
}

?>