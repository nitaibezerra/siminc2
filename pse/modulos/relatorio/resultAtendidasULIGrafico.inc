<?php
include "resultSession.php";

function obterSqlGrafico($anoEspecifico = null) {
	
	if( $anoEspecifico <> 10 ){
		$where = "AND pp.porano = ".$anoEspecifico;
		$whereAtend = "AND e.espano = ".($anoEspecifico + 1);
		$select = $anoEspecifico." as ano,";
		$group = ", ano";		
	}
	else{
		$where = '';
		$whereAtend = "AND e.espano in ('2009','2010')";
	}
	
	if( $anoEspecifico <> 10 ){
	
	$sql = "
			SELECT DISTINCT
			pp.poruf as ufb,
				p.contamunicipio as municipios_atendidos,
				ende.total as total,
				ee.totalescolas  as escolas_atendidas
			FROM 
			  pse.portariapse AS pp
			   
			LEFT JOIN (SELECT poruf, count(pp.pormunicipio) AS contamunicipio
				FROM pse.portariapse AS pp WHERE 1=1 $where group by poruf ) AS p on p.poruf = pp.poruf
				 
			LEFT JOIN (SELECT ende.estuf, count(ent.entid) AS total
				FROM 
					pse.portariapse pp				
				INNER JOIN
					territorios.municipio m ON m.muncod = pp.pormunicipio
				INNER JOIN
					entidade.endereco ende ON ende.muncod = m.muncod
				INNER JOIN
					entidade.entidade ent ON ent.entid = ende.entid
				INNER JOIN
					entidade.funcaoentidade fe ON fe.entid = ent.entid
				WHERE
					fe.funid = 3 AND
					ent.tpcid in (1, 3) AND
					ent.entstatus='A'
					$where group by ende.estuf) ende ON ende.estuf = pp.poruf
					
			LEFT JOIN (SELECT DISTINCT ee.estuf, count(distinct ep.entid) as totalescolas FROM pse.escolapse ep
				INNER JOIN entidade.endereco ee ON ee.entid = ep.entid
				INNER JOIN pse.portariapse pp ON pp.pormunicipio = ee.muncod
				--INNER JOIN pse.rulresposta rul ON rul.espid = ep.espid
				WHERE ep.espano IS NOT NULL
				$where 
				AND ep.entid in (
							SELECT
								entid
							FROM
								(SELECT ent.entid,
									(select count(entid) from pse.equipesaudefamilia eq inner join pse.escolapse e on e.espid=eq.espid where e.entid=ent.entid $whereAtend) as p2,
									(select count(entid) from pse.educadoreferencia ed inner join pse.escolapse e on e.espid=ed.espid where e.entid=ent.entid $whereAtend) as p3,
									(select count(pp.nieid) from pse.escolapse ep inner join pse.equipesaudefamilia ef on ef.espid = ep.espid inner join pse.pseanoescola pp on ef.esfid = pp.esfid where ep.entid = ent.entid
									) as p4,
									(select count(entid) from pse.atorequipe eq inner join pse.escolapse e on e.espid=eq.espid where e.entid=ent.entid $whereAtend) AS p5								
								FROM
									entidade.entidade ent
								INNER JOIN
									entidade.funcaoentidade fe ON fe.entid = ent.entid
								WHERE
									ent.entstatus='A' 
									AND fe.funid = 3									
									AND (ent.tpcid = 1 or ent.tpcid = 3)
								) as f
							WHERE
								((f.p2 <> 0) AND
								(f.p3 <> 0) AND
								(f.p4 <> 0) AND
								(f.p5 <> 0))
							)
							group by ee.estuf ) ee on ee.estuf = pp.poruf
			WHERE
			1 = 1
			 $where
			ORDER BY
			ufb
			";

	} else {
		
		$sql = "
				SELECT DISTINCT
				pp.poruf as ufb,
					p.contamunicipio as municipios_atendidos,
					ende.total as total,
					ee.totalescolas  as escolas_atendidas
				FROM 
				  pse.portariapse AS pp
				   
				LEFT JOIN (SELECT poruf, count(pp.pormunicipio) AS contamunicipio
					FROM pse.portariapse AS pp WHERE 1=1 $where group by poruf ) AS p on p.poruf = pp.poruf
					 
				LEFT JOIN (SELECT ende.estuf, count(ent.entid) AS total
					FROM 
						pse.portariapse pp				
					INNER JOIN
						territorios.municipio m ON m.muncod = pp.pormunicipio
					INNER JOIN
						entidade.endereco ende ON ende.muncod = m.muncod
					INNER JOIN
						entidade.entidade ent ON ent.entid = ende.entid
					INNER JOIN
						entidade.funcaoentidade fe ON fe.entid = ent.entid
					WHERE
						fe.funid = 3 AND
						ent.tpcid in (1, 3) AND
						ent.entstatus='A'
						$where group by ende.estuf) ende ON ende.estuf = pp.poruf
						
				LEFT JOIN (SELECT  DISTINCT
								ee.estuf, --ep.espano, pp.porano, 
								count(distinct ep.espid) as totalescolas 
							FROM 
								pse.escolapse ep
							INNER JOIN entidade.endereco ee ON ee.entid = ep.entid
							INNER JOIN pse.portariapse pp ON pp.pormunicipio = ee.muncod
							WHERE 
								ep.espano IS NOT NULL
								AND ep.espid in (
											(SELECT espid FROM pse.escolapse esp WHERE esp.espano = 2010 AND esp.entid IN (
							
											SELECT DISTINCT
												entid
											FROM
												(SELECT ent.entid,
													(select count(entid) from pse.equipesaudefamilia eq inner join pse.escolapse e on e.espid=eq.espid where e.entid=ent.entid AND e.espano in ('2010')) as p2,
													(select count(entid) from pse.educadoreferencia ed inner join pse.escolapse e on e.espid=ed.espid where e.entid=ent.entid AND e.espano in ('2010')) as p3,
													(select count(pp.nieid) from pse.escolapse ep inner join pse.equipesaudefamilia ef on ef.espid = ep.espid inner join pse.pseanoescola pp on ef.esfid = pp.esfid where ep.entid = ent.entid) as p4,
													(select count(entid) from pse.atorequipe eq inner join pse.escolapse e on e.espid=eq.espid where e.entid=ent.entid AND e.espano in ('2010')) AS p5								
												FROM
													entidade.entidade ent
												INNER JOIN
													entidade.funcaoentidade fe ON fe.entid = ent.entid
												WHERE
													ent.entstatus='A' 
													AND fe.funid = 3									
													AND (ent.tpcid = 1 or ent.tpcid = 3)
												) as f
											WHERE
												((f.p2 <> 0) AND
												(f.p3 <> 0) AND
												(f.p4 <> 0) AND
												(f.p5 <> 0))
											))
							
											UNION ALL
							
											(SELECT espid FROM pse.escolapse esp WHERE esp.espano = 2009 AND esp.entid IN (
							
											SELECT DISTINCT
												entid
											FROM
												(SELECT ent.entid,
													(select count(entid) from pse.equipesaudefamilia eq inner join pse.escolapse e on e.espid=eq.espid where e.entid=ent.entid AND e.espano in ('2009')) as p2,
													(select count(entid) from pse.educadoreferencia ed inner join pse.escolapse e on e.espid=ed.espid where e.entid=ent.entid AND e.espano in ('2009')) as p3,
													(select count(pp.nieid) from pse.escolapse ep inner join pse.equipesaudefamilia ef on ef.espid = ep.espid inner join pse.pseanoescola pp on ef.esfid = pp.esfid where ep.entid = ent.entid) as p4,
													(select count(entid) from pse.atorequipe eq inner join pse.escolapse e on e.espid=eq.espid where e.entid=ent.entid AND e.espano in ('2009')) AS p5								
												FROM
													entidade.entidade ent
												INNER JOIN
													entidade.funcaoentidade fe ON fe.entid = ent.entid
												WHERE
													ent.entstatus='A' 
													AND fe.funid = 3									
													AND (ent.tpcid = 1 or ent.tpcid = 3)
												) as f
											WHERE
												((f.p2 <> 0) AND
												(f.p3 <> 0) AND
												(f.p4 <> 0) AND
												(f.p5 <> 0))
											))
							
										) 
							AND (ep.espano = pp.porano + 1)
							group by ee.estuf ) ee on ee.estuf = pp.poruf
				WHERE
				1 = 1
				 $where
				ORDER BY
				ufb
				";
		
	}
	
//	ver ($sql);	  
	return $sql;
}


function gerarGraficoComBarrasVerticais ($titulo,$dados){
	$parametros = new ParametroComponenteGrafico($titulo,$dados); 
	
	$parametros->determinarTituloGrafico();
	$parametros->determinarBarraDeDados($dados); 
	$parametros->determinarEixoHorizontalX($dados);
	$parametros->determinarEixoVerticalY($dados);
	$parametros->determinarRotulosDeAjudaMouse();

	return $parametros->gerarCodigoGrafico();
}

class ParametroComponenteGrafico 
{
	private $bar_stack;
	private $chart;
	private $titulo;
	private $dados;
	
	function ParametroComponenteGrafico($titulo,$dados) {
		$this->bar_stack = new bar_stack();
		$this->chart = new open_flash_chart();
		$this->titulo = $titulo;
		$this->dados = $dados;		
	}
	
	function determinarTituloGrafico() {
		$title = new title( $this->removeacentosGrafico($this->titulo) );
		$title->set_style( "{font-size: 20px; color: #F24062; text-align: center;}" );
		$this->chart->set_title( $title );
	}
	
	function determinarBarraDeDados($dados) {
		$this->bar_stack->set_colours( array( '#C4D318', '#50284A' ) );
		$this->bar_stack->set_tooltip( '#x_label#' );
		
		foreach( $dados as $linhaRegistro ){
			$linhaDadosParaBarra = $this->adicionarLinhaDadosParaBarra($linhaRegistro);
			$this->bar_stack->append_stack( $linhaDadosParaBarra );
		}
		
		$this->chart->add_element( $this->bar_stack );
	}
	
	function adicionarLinhaDadosParaBarra($linhaRegistro) {
			$quantiaEscolasAtendidas = (int) $linhaRegistro['escolas_atendidas'];
			$quantiaEscolasTotal = (int) $linhaRegistro['total'];
			$quantiaDiferenca = $quantiaEscolasTotal - $quantiaEscolasAtendidas;  
			return array( $quantiaEscolasAtendidas, $quantiaDiferenca );			
	}
	
	function determinarEixoHorizontalX($dados) {
		foreach( $dados as $linhaRegistro ){
			$arrayRotulacaoUf[] = $linhaRegistro['ufb'];
		}
		$x = new x_axis();
		
		$x->set_labels_from_array( $arrayRotulacaoUf );	
		$this->chart->set_x_axis( $x );
	}
	
	function determinarEixoVerticalY($dados) {
		$y = new y_axis();
		$y->set_range( 0, $this->determinarTetoDoGrafico($dados), 250 );
		$this->chart->add_y_axis( $y );	
	}
	
	function determinarTetoDoGrafico($dados) {
		foreach( $dados as $linhaRegistro ){
			$arrayMaximo[] = (int) $linhaRegistro['total'];
		}
		$TETO_ADICIONAL_PARA_BARRA = 10; 
		return (max($arrayMaximo) + $TETO_ADICIONAL_PARA_BARRA);
	}
	
	function determinarRotulosDeAjudaMouse() {
		$tooltip = new tooltip();
		$tooltip->set_hover();
		$this->chart->set_tooltip( $tooltip );
	}
	
	function gerarCodigoGrafico() {
		return $this->chart->toPrettyString();
	}
	
	function removeacentosGrafico($var) {
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
}
?>