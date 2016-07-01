<?php
include "resultSession.php";

function obterSqlGrafico($anoEspecifico = null) {
	
	if( $anoEspecifico <> 10 ){
		$where = " AND portariapse.porano = ".$anoEspecifico;
		$select = $anoEspecifico." as ano,";
		$group = ", ano";		
	}
	
	$sql = "
			select distinct
			  portariapse.poruf        as ufb   ,
			  $select   
			  count(municatend.muncod) as municipios_atendidos , 
			  municexist.qtexistentes  as total 
			  
			from 
			  pse.portariapse as portariapse 
			
			  inner join territorios.municipio as municatend 
			  on portariapse.pormunicipio = municatend.muncod
			
			  left join 
				( select 
					municexist.estuf         as uf , 
					count(municexist.muncod) as qtexistentes
				  from 
					territorios.municipio municexist 
				  group by 
					uf
				) as municexist 
			  on portariapse.poruf = municexist.uf 
			where
				1 = 1
				$where 
			group by 
			  ufb $group, total
			
			order by 
			  ufb $group
			";
		  
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
			$quantiaMunicipiosAtendidos = (int) $linhaRegistro['municipios_atendidos'];
			$quantiaMunicipiosTotal = (int) $linhaRegistro['total'];
			$quantiaDiferenca = $quantiaMunicipiosTotal - $quantiaMunicipiosAtendidos;  
			return array( $quantiaMunicipiosAtendidos, $quantiaDiferenca );			
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
		$y->set_range( 0, $this->determinarTetoDoGrafico($dados), 70 );
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